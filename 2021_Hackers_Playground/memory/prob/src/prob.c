#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include "./file.h"
#include <openssl/sha.h>
#include <unistd.h>
#include <sys/types.h>
#include <dlfcn.h>
#include <time.h>
#include <signal.h>
#include <regex.h>
#include <dirent.h>
#include <error.h>
#include <errno.h>

char* tarfile = "backup.tar";
char* bakfile = "backup.bak";
char* resfile = "restore.bak";

regex_t regex;
unsigned char *base64_decode(const char *data,
                             size_t input_length,
                             size_t *output_length);
 
void printContents(char *filename){
    FILE *file;
    char str[1024];
    file = fopen(filename,"r");
    if(file){
        while(fgets(str,sizeof str,file)!=NULL)
            printf("%s",str);
        fclose(file);
    }
    else
        printf("%s not exist\n",filename);
}

void addCustomHeader(char *before, char *after){
    int i=0,count=0,len=0;
    char buf[1024]={0,};
    struct Custom_header custom_header;
    
    init_header(before,&custom_header);

    int fd_pre = open(before,O_RDWR);
    int fd_post = open(after,O_RDWR|O_CREAT,S_IRUSR|S_IWUSR);
    if ( fd_pre == -1 || fd_post == -1){
        printf(" Can't not open the backup file\n");
        exit(-1);
    }
    write(fd_post,&custom_header,40);
    lseek(fd_pre,0,SEEK_SET); 
    while(1){
        memset(buf,0,1024);
        if ( count >= custom_header.size )
            break;
        else if ( count + 1024 > custom_header.size )
            len = custom_header.size % 1024;
        else
            len = 1024;

        read(fd_pre,buf,len);
        write(fd_post,buf,len);
        count += len;
    }
    close(fd_pre);
    close(fd_post);
}

void ByteArrayLE2Int(unsigned char *byteArray, int*x)
{
      *x=byteArray[0] | (byteArray[1]<<8) | (byteArray[2]<<16) | (byteArray[3]<<24);
}

int restore(void)
{
    int c;
	long size;
    char buff[20]={0,};
    char head[40]={0,};
    char *backup = 0;
    struct Custom_header cHeader;
    char file_size[5];
    int ret = 0;

    unsigned char hash[SHA256_DIGEST_LENGTH];

    printf("->input restore file size : ");
    read(0,buff,20);
    size = atoi(buff);
    if ( size <= 40 ){
        printf("error\n");
        return 0;
    }
    printf("recv size is %d\n",size);
    backup = (char *)malloc(sizeof(char)*size);
    if (backup == 0){
        printf("malloc error\n");
        exit(-1);
    }
    
    printf("->input backup file binary contents : ");
    int count = 0;
    while(count<size){
	    read(0,backup+count,1);
	    count++;
    }

    char *decoded_data = base64_decode(backup,size,&size);
    free(backup);
    backup = decoded_data;

    memcpy(cHeader.magic,backup,4);
    memcpy(file_size,backup+4,4);
    ByteArrayLE2Int(file_size,&size);
    cHeader.size = size;
    memcpy(cHeader.hash,backup+8,32);
    printf("download restore file info\n");
    printf("magic : %s\n",cHeader.magic);
    printf("size : %d\n",cHeader.size);
    printf("hash: ");
    int i = 0;
    for ( i = 0; i < 32; i++)
        printf("%02x",cHeader.hash[i]);
    printf("\n");
    if(strncmp(cHeader.magic,"SCTF",4) == 0 )
    {
        int fd = open(resfile,O_RDWR|O_CREAT,S_IRUSR|S_IWUSR);
        write(fd,backup+40,cHeader.size);
        close(fd);
    }
    if( checkIntegrity(resfile,cHeader.hash) )
        ret = 1;
    else{
        printf("!check error\n");
        remove(resfile);
    }

    free(backup);
    printf("Download Done\n");
    return ret;
}



void check_header(){
    int fd;
    struct Custom_header header;

    fd = open(resfile,O_RDONLY , S_IRUSR | S_IWUSR);
    read(fd,header.magic,4);
    unsigned char file_size[4]={0,};
    read(fd,file_size,4);
    header.size = atoi(file_size);
    read(fd,header.hash,32);

    printf("magic : %s\n",header.magic);
    printf("size : %d\n",header.size);
    printf("hash : ");
    int i = 0;
    for ( i = 0; i < 32; i++){
        printf("%02x",header.hash[i]);
    }

}
void print_menu(){
    printf("=================\n");
    printf("1.write\n");
    printf("2.view\n");
    printf("3.all view\n");
    printf("4.backup data\n");
    printf("5.restore data\n");
    printf("6.exit\n");
    printf("=================\n");
    printf("input menu : ");
}

static char encoding_table[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
                                'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
                                'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
                                'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
                                'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
                                'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
                                'w', 'x', 'y', 'z', '0', '1', '2', '3',
                                '4', '5', '6', '7', '8', '9', '+', '/'};
static char *decoding_table = NULL;
static int mod_table[] = {0, 2, 1};
 
void build_decoding_table() {
 
    decoding_table = malloc(256);
 
    for (int i = 0; i < 64; i++)
        decoding_table[(unsigned char) encoding_table[i]] = i;
}
 
 
void base64_cleanup() {
    free(decoding_table);
} 
 
char *base64_encode(const unsigned char *data,
                    size_t input_length,
                    size_t *output_length) {
 
    *output_length = 4 * ((input_length + 2) / 3);
 
    char *encoded_data = malloc(*output_length);
    if (encoded_data == NULL) return NULL;
 
    for (int i = 0, j = 0; i < input_length;) {
 
        uint32_t octet_a = i < input_length ? (unsigned char)data[i++] : 0;
        uint32_t octet_b = i < input_length ? (unsigned char)data[i++] : 0;
        uint32_t octet_c = i < input_length ? (unsigned char)data[i++] : 0;
 
        uint32_t triple = (octet_a << 0x10) + (octet_b << 0x08) + octet_c;
 
        encoded_data[j++] = encoding_table[(triple >> 3 * 6) & 0x3F];
        encoded_data[j++] = encoding_table[(triple >> 2 * 6) & 0x3F];
        encoded_data[j++] = encoding_table[(triple >> 1 * 6) & 0x3F];
        encoded_data[j++] = encoding_table[(triple >> 0 * 6) & 0x3F];
    }
 
    for (int i = 0; i < mod_table[input_length % 3]; i++)
        encoded_data[*output_length - 1 - i] = '=';
 
    return encoded_data;
}
 
 
unsigned char *base64_decode(const char *data,
                             size_t input_length,
                             size_t *output_length) {
 
    if (decoding_table == NULL) build_decoding_table();
 
    if (input_length % 4 != 0) return NULL;
 
    *output_length = input_length / 4 * 3;
    if (data[input_length - 1] == '=') (*output_length)--;
    if (data[input_length - 2] == '=') (*output_length)--;
 
    unsigned char *decoded_data = malloc(*output_length);
    if (decoded_data == NULL) return NULL;
 
    for (int i = 0, j = 0; i < input_length;) {
 
        uint32_t sextet_a = data[i] == '=' ? 0 & i++ : decoding_table[data[i++]];
        uint32_t sextet_b = data[i] == '=' ? 0 & i++ : decoding_table[data[i++]];
        uint32_t sextet_c = data[i] == '=' ? 0 & i++ : decoding_table[data[i++]];
        uint32_t sextet_d = data[i] == '=' ? 0 & i++ : decoding_table[data[i++]];
 
        uint32_t triple = (sextet_a << 3 * 6)
        + (sextet_b << 2 * 6)
        + (sextet_c << 1 * 6)
        + (sextet_d << 0 * 6);
 
        if (j < *output_length) decoded_data[j++] = (triple >> 2 * 8) & 0xFF;
        if (j < *output_length) decoded_data[j++] = (triple >> 1 * 8) & 0xFF;
        if (j < *output_length) decoded_data[j++] = (triple >> 0 * 8) & 0xFF;
    }
 
    return decoded_data;
}

void printBackupFile(void){
    int i,size,offset,len;
    int fd = open(bakfile,O_RDONLY,S_IRUSR | S_IWUSR);
    char buf[1024]={0,};
	char *data;
    if ( fd == -1 )
        printf("error\n");
    
    size = lseek(fd,0,SEEK_END); 
    lseek(fd,0,SEEK_SET);
    offset = 0;

    printf("backup Contents\n");
	data = (char *)malloc(sizeof(char)*size+1);
	read(fd,data,size);
	//write(1,data,size);

	long input_size = size;
	long output_size ;
	char *encoded_data = base64_encode(data,input_size,&input_size);
	write(1,encoded_data,strlen(encoded_data));
	free(encoded_data);
	free(data);
	close(fd);
	
    /*while(1){
        memset(buf,0,1024);
        if ( offset >= size )
            break;
        else if ( offset + 1024 > size )
            len = size % 1024;
        else
            len = 1024;

        read(fd,buf,len);
        write(1,buf,len);
        offset += len;
    }
    close(fd);*/
}

void loadLib(char *cmd){
    void *lib_handle = NULL;
    void (*func)(char *) = NULL;
    lib_handle = dlopen("./lib/libutil.so",RTLD_NOW);
    if (lib_handle == NULL)
        exit(-1);
    func = dlsym(lib_handle,"execute");
    if (dlerror()!=NULL)
        exit(-1);
    func(cmd);
    dlclose(lib_handle);
    func = NULL;
}
int main(void){
    int ans,c,fd;
    setbuf(stdout,0);
    setbuf(stdin,0);
    alarm(20);

    while(1){
        print_menu();
        scanf("%d",&ans);
        while ((c = getchar()) != '\n' && c != EOF) { }    
        switch(ans){
            case 1:
                {
                    time_t now = time(NULL);
                    struct tm *local = localtime(&now);
                    int day,month,year;
                    char today[15];
                    char contents[10000];
                    
                    printf("1.write\n");
                    day = local->tm_mday;
                    month = local->tm_mon + 1;
                    year = local->tm_year + 1900;

                    sprintf(today,"data/%d-%02d-%02d",year,month,day);
                    printf("Write on %s\n",today);
                    printf("Please wirte your contents : \n");

                    fd = open(today,O_WRONLY | O_APPEND | O_CREAT,S_IRUSR|S_IWUSR);
                    fgets(contents,sizeof contents,stdin);
                    write(fd,contents,strlen(contents));
                    close(fd);

                    memset(contents,0,10000);
                    printf("Done\n");
                }
                break;
            case 2:
                {
                    int reti;
                    char msgbuf[10];
                    char fileName[100];

                    printf("2.veiwing\n");
                    printf("input date(YYYY-MM-DD) : \n");
                    read(0,msgbuf,10);
                    while ((c = getchar()) != '\n' && c != EOF) { }    
                    reti = regcomp(&regex,"[0-9]{4}-[0-9]{2}-[0-9]{2}",REG_EXTENDED);
                    if (reti){
                        printf("could not compile regex\n");
                        exit(-1);
                    }

                    reti = regexec(&regex,msgbuf,0,NULL,0);
                    if(!reti){
                        sprintf(fileName,"data/%s",msgbuf);
                        if ( access(fileName,0) )
                            printf("there is no history\n");
                        else{
                            printContents(fileName);
                        }
                    }
                    else{
                        printf(" not correct format \n");
                    }
                    regfree(&regex);
                    printf("Done\n");
                }
                break;
            case 3:
                {
                    DIR *dir;
                    struct dirent *ent;
                    char filename[10000];
                    dir = opendir("data");
                    if(dir!=NULL){
                        struct dirent **namelist;
                        int count;
                        int idx;
                        if ((count = scandir("data",&namelist,NULL,alphasort)) == -1){
                            fprintf(stderr,"Directory Scan Error: %s\n",strerror(errno));
                            continue;
                        }

                        for ( idx = 2; idx < count; idx++){
                            printf("%s\n",namelist[idx]->d_name);
                        	sprintf(filename,"data/%s",namelist[idx]->d_name);
                            printContents(filename);
                        }

                        for (idx = 0; idx < count; idx++)
                            free(namelist[idx]);
                        free(namelist);
                        /*
                        while((ent = readdir(dir)) != NULL){
                            if(!strcmp(".",ent->d_name)||!strcmp("..",ent->d_name))
                                continue;
                            printf("--- %s ---\n",ent->d_name);
                            sprintf(filename,"data/%s",ent->d_name);
                            printContents(filename);
                        }
                        closedir(dir);*/
                    }
                }
                break;
            case 4:
                    printf("4.backup data\n");
                    loadLib("tar cvfP backup.tar ./data/*");
                    addCustomHeader(tarfile,bakfile);
                    printBackupFile();
                    printf("\nDone\n");
            break;
            case 5:
                printf("5.restore data\n");
                int ret = restore();
                if(ret)
                    loadLib("tar xvfP restore.bak -C ./data/");
                else
                    exit(-1);
                printf("Done\n");
            break;
            case 6:
                printf("bye\n");
                return 0;
            default:
            break;
        }
    }
    return 0;
}
