
#define _GNU_SOURCE
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/user.h>
#include <sys/sendfile.h>

#ifndef PAGE_SIZE
#define PAGE_SIZE 4096
#endif

static void write_pipe();
static void read_pipe();

int p[2];
static char buffer[PAGE_SIZE];

static void write_pipe()
{
    int read_size = 0;
    int i = 0;
    printf("read size?>\n");
    scanf("%d",&read_size);
    if(read_size > PAGE_SIZE) return;
    printf("input>");
    memset(buffer,0,PAGE_SIZE);

    while(i < read_size)
    {
    	i+=read(0, buffer+i, 1);
    }
    write(p[1], buffer, strlen(buffer));

}
static void read_pipe()
{
    memset(buffer,0,PAGE_SIZE);
    read(p[0], buffer, PAGE_SIZE);
    printf("%s",buffer);
}

static int make_file(char *file_name)
{
    if(strlen(buffer)>0)
    {
        printf("[+] file_name:[%s]\n", file_name);
        int fd = open(file_name, O_WRONLY| O_CREAT, 0666);
        if(fd < 0) 
        {
            write(1,"[-] open error\n",15);
            return 0;
        }
        write(fd, buffer, PAGE_SIZE);
        close(fd);
    }
    return 1;
}

static int read_file(char * file_name, size_t read_size)
{
    printf("file_name:[%s] , read_size:[%ld]\n", file_name, read_size);
    int fd = open(file_name, O_RDONLY);
    if(fd < 0) 
    {
	printf("[-] open error\n");
        return 0;
    }
    sendfile(p[1], fd, 0, read_size);
    return 1;
}

void usage()
{
    write(1,"[+] usage\n",11);
    write(1,"[+] 1: read from pipe\n",23);
    write(1,"[+] 2: write to pipe\n",22);
    write(1,"[+] 3: make log file\n",22);
    write(1,"[+] 4: read file\n",18);
    write(1,"[+] 5: read info\n",18);

}
int main(int argc, char **argv)
{
    int cmd = 0;
    int read_size = 0;
    char * file_name = malloc(256);
    setbuf(stdin, NULL);
    setbuf(stdout, NULL);
    usage();
    
    if (pipe(p)) abort();
    //prepare_pipe();
    
    while(1)
    {
        cmd = 0;
        memset(file_name,0,256);
        
        printf("cmd>");
        scanf("%d",&cmd);
        switch(cmd)
        {
            case 1:
                read_pipe(); // read data from pipe to stdout
                break;
            case 2:
                write_pipe(); // write data from stdin, to pipe
                break;
            case 3:
		printf("[+] File path>");
		scanf("%100s",file_name);
                if(strlen(file_name) > 2)
                {
                    file_name = strtok(file_name,"\n");
                    make_file(file_name); // create and write buffer datat to file 
                }
                break;
            case 4:
		printf("[+] File Path>");
		scanf("%100s",file_name);

                if(strlen(file_name) > 2)
                {
                    file_name = strtok(file_name,"\n");
		    printf("[+] size?>");
                    scanf("%d",&read_size);
                    read_file(file_name, read_size); // read_file data to pipe 
                }
                break;
            case 5:
                system("sh /home/guest/info.sh");
                break;

            default:
                write(1,"[+] Bye~\n",10);
                return 0;
        }
    }
    return 1;
}
