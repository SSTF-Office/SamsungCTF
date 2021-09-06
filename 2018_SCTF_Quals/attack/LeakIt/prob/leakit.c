#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <time.h>
#include <signal.h>

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

#define DEFAULT_BUF_SIZE 64
#define DEFAULT_CMD_SIZE 1024
#define DEFAULT_PATH "/home/leakit/database/"

void error(char *str) {
    fputs(str, stderr);
    //puts(str);
}

void rstrip_newline(char *buf) {
    char *last = &buf[strlen(buf)-1];

    if (*last == '\n')
        *last = '\0';

}

char *do_command(char *cmd){
    FILE *p;
    char *output;
    char buf[DEFAULT_CMD_SIZE];
    size_t n = 0;
    size_t chunk_size = sizeof(buf);
    size_t output_size = chunk_size * 10;

    output = (char *) malloc(output_size);
    if(!output) {
        perror("[-] failed to malloc output buffer");
        return 0;
    }
    memset(output, 0, output_size);

    p = popen(cmd, "r");
    if(!p) {
        perror("[-] failed to open the pipe");
        return 0;
    }

    setvbuf(p, NULL, _IONBF, 0);

    while(!feof(p) && (n + chunk_size) < output_size) {
        fgets(buf, chunk_size, p);
        strcat(output, buf);
        n += chunk_size;
    }

    if (pclose(p) < 0) {
        perror("[-] failed to close the pipe");
        return 0;
    }

    return output;
}

int show_logo(){
    char logo[] =
"  /$$$$$$  /$$$$$$$   /$$$$$$        /$$      /$$                                                                                \n"
" /$$__  $$| $$__  $$ /$$__  $$      | $$$    /$$$                                                                                \n"
"| $$  \\__/| $$  \\ $$| $$  \\__/      | $$$$  /$$$$  /$$$$$$   /$$$$$$$  /$$$$$$$  /$$$$$$  /$$$$$$$   /$$$$$$   /$$$$$$   /$$$$$$ \n"
"| $$ /$$$$| $$$$$$$/| $$ /$$$$      | $$ $$/$$ $$ /$$__  $$ /$$_____/ /$$_____/ /$$__  $$| $$__  $$ /$$__  $$ /$$__  $$ /$$__  $$\n"
"| $$|_  $$| $$____/ | $$|_  $$      | $$  $$$| $$| $$$$$$$$|  $$$$$$ |  $$$$$$ | $$$$$$$$| $$  \\ $$| $$  \\ $$| $$$$$$$$| $$  \\__/\n"
"| $$  \\ $$| $$      | $$  \\ $$      | $$\\  $ | $$| $$_____/ \\____  $$ \\____  $$| $$_____/| $$  | $$| $$  | $$| $$_____/| $$      \n"
"|  $$$$$$/| $$      |  $$$$$$/      | $$ \\/  | $$|  $$$$$$$ /$$$$$$$/ /$$$$$$$/|  $$$$$$$| $$  | $$|  $$$$$$$|  $$$$$$$| $$      \n"
" \\______/ |__/       \\______/       |__/     |__/ \\_______/|_______/ |_______/  \\_______/|__/  |__/ \\____  $$ \\_______/|__/      \n"
"                                                                                                    /$$  \\ $$                    \n"
"                                                                                                   |  $$$$$$/                    \n"
"                                                                                                    \\______/                     \n\\n"
"[+] You can make a complain to the admin using this messenger!\n"
"[+] Have Fun!\n\n\n";
    puts(logo);
}


int check_input(char *buf){
    size_t i, buflen = strlen(buf);
    for (i=0; i<buflen; i++)
        if (!isalnum(buf[i]))
            return -1;
    return 0;
}

int get_input(char *dst){
    char buf[DEFAULT_BUF_SIZE];
    memset(buf, 0, sizeof(buf));
    read(0, buf, sizeof(buf));
    rstrip_newline(buf);

    if (check_input(buf)) {
        error("Invalid character has been detected.");
        return -1;
    }
    memcpy(dst, buf, strlen(buf));
    return 0;
}

int initialize_id(char *id){
    int ret;
    printf("what is your id?\n");
    printf("id: ");
    ret = get_input(id);
    return ret;
}

int initialize_passphrase(char *pw){
    int ret;
    printf("what is your passphrase?\n");
    printf("passphrase: ");
    ret = get_input(pw);
    return ret;
}

static char *get_randstr_helper(char *str, size_t size, size_t offset) {
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    srand(time(NULL) ^ (getpid()<<16));
    if (size) {
        for (size_t n = 0; n < size; n++) {
            int key = rand() % (int) (sizeof charset - 1);
            str[offset + n] = charset[key];
        }
        str[offset + size] = '\0';
    }
    return str;
}


char *get_randstr() {
    size_t size = 20;
    size_t offset = strlen(DEFAULT_PATH);
    char *s = malloc(size + 1 + offset);
    strcpy(s, DEFAULT_PATH);
    if (s) {
        get_randstr_helper(s, size, offset);
    }
    return s;
}

int verify_key(char *id, char *pw){
    FILE *fp;
    char cmd[DEFAULT_CMD_SIZE];
    char *output;
    char *out_fname = get_randstr();
    char *sig_fname = malloc(strlen(out_fname) + 4);
    sprintf(sig_fname, "%s.sig", out_fname);

    fp = fopen(out_fname, "w");
    if(!fp) {
        perror("[-] failed to open the file");
        return -1;
    }
    fputs(out_fname, fp);
    fclose(fp);

    printf("[+] Verifying your GPG key ... \n");
    sprintf(cmd, "(echo \"%s\") | gpg --default-key \"%s@2018.eatpwnnosleep.com\""
                 " --passphrase-fd 0 --output %s"
                 " --no-tty -s %s 2>&1 \n",
                 pw, id, sig_fname, out_fname);

    output = do_command(cmd);
    if (!output) {
        free(sig_fname);
        unlink(out_fname);
        free(out_fname);
        return -1;
    }

    if (strstr(output, "signing failed")) {
        free(sig_fname);
        unlink(out_fname);
        free(out_fname);
        free(output);
        return -1;
    }

    unlink(sig_fname);
    free(sig_fname);
    unlink(out_fname);
    free(out_fname);
    free(output);
    return 0;
}

int generate_key(char *id, char *pw){
    FILE *fp;
    char script[] =
        "%%echo Generating a basic OpenPGP key\n"
        "Key-Type: 1\n"
        "Key-Length: 2048\n"
        "Name-Real: %s\n"
        "Name-Comment: account for sctf 2018\n"
        "Name-Email: %s@2018.eatpwnnosleep.com\n"
        "Expire-Date: 0\n"
        "Passphrase: %s\n"
        "%%commit\n"
        "%%echo done\n";
    char buf[DEFAULT_CMD_SIZE*4];
    char cmd[DEFAULT_CMD_SIZE];
    char *output;
    char *script_fname = get_randstr();
    sprintf(buf, script, id, id, pw);

    fp = fopen(script_fname, "w");
    if(!fp) {
        perror("[-] failed to open the file");
        return -1;
    }
    fputs(buf, fp);
    fclose(fp);

    sprintf(cmd, "gpg --no-tty --batch --gen-key %s 2>&1", script_fname);

    output = do_command(cmd);
    if (!output) {
        unlink(script_fname);
        free(script_fname);
        return -1;
    }

    unlink(script_fname);
    free(script_fname);
    free(output);
    return 0;
}

int initialize_key(char *id, char *pw){
    char cmd[DEFAULT_CMD_SIZE];
    char *output;

    // check if key already exists
    sprintf(cmd, "gpg --no-tty --list-keys \"%s\" 2>&1", id);
    output = do_command(cmd);
    if (!output)
        return -1;

    if(strstr(output, "not found")) {
        // if key does not exist, create a new key
        printf("[+] that key does not exist. creating a new key.\n");
        if (generate_key(id, pw) < 0) {
            error("[-] key generation failed.\n");
            free(output);
            return -1;
        }
    } else {
        // verify key
        if (verify_key(id, pw) < 0) {
            error("[-] key verification failed.\n");
            free(output);
            return -1;
        }

        // use this key
        printf("[+] successfully verified your key.\n");
    }

    free(output);
    return 0;
}


int initialize_gpg_config(char *id, char *pw){
    int ret;
    printf("[+] initializing your GPG configuration.\n");

    ret = initialize_id(id);
    if (ret < 0){
        puts("[-] GPG Key ID initialization failed.");
        return ret;
    }

    ret = initialize_passphrase(pw);
    if (ret < 0){
        puts("[-] Passphrase initialization failed.");
        return ret;
    }

    ret = initialize_key(id, pw);
    if (ret < 0){
        puts("[-] Key initialization failed.");
        return ret;
    }

    return ret;
}

int send_msg(char *id, char *pw) {
    FILE *fp;
    char msg[DEFAULT_CMD_SIZE * 4];
    char buf[DEFAULT_CMD_SIZE];
    char *out_fname;
    char *enc_fname;
    char *output;
    int ret;
    size_t n=0;

    out_fname = get_randstr();
    enc_fname = malloc(strlen(out_fname) + 4);

    sprintf(enc_fname, "%s.enc", out_fname);
    memset(msg, 0, sizeof(msg));

    printf("Your message will be signed with your secret key, and encrypted with the admin's public key\n");
    printf("Write your complain message:\n");

    // vulnerability here. you can use this to trigger BOF.
    while (n < sizeof(msg)) {
        memset(buf, 0, sizeof(buf));

        printf("msg (type 'SEND' to send): ");
        read(0, buf, sizeof(buf));

        if (!strncmp(buf, "SEND", 4))
            break;

        memcpy(msg + n, buf, sizeof(buf));
        n += strlen(buf);
    }

    fp = fopen(out_fname, "w");
    if(!fp) {
        puts("[-] failed to open the file");
        free(out_fname);
        free(enc_fname);
        return -1;
    }
    fputs(msg, fp);
    fclose(fp);

    printf("[+] Sign and encrypt your message for the admin\n");
    sprintf(buf, "(echo \"%s\") | gpg --default-key \"%s@2018.eatpwnnosleep.com\""
                 " --passphrase-fd 0 --output %s.enc"
                 " --no-tty -se -r admin@2018.eatpwnnosleep.com %s 2>&1\n",
                 pw, id, out_fname, out_fname);

    output = do_command(buf);
    if (!output) {
        error("[-] command execution failed.");
        free(enc_fname);
        unlink(out_fname);
        free(out_fname);
        return -1;
    }

    if (strstr(output, "failed")) {
        error("[--] Sign-then-Encrypt failed.");
        free(enc_fname);
        unlink(out_fname);
        free(out_fname);
        free(output);
        return -1;
    }

    printf("[++] admin will check your message. Thanks for your opinion!\n");

    unlink(enc_fname);
    free(enc_fname);
    unlink(out_fname);
    free(out_fname);
    free(output);
    return 0;
}

int recv_msg(char *id, char *pw) {
    puts("Admin's message will be signed with the admin's secret key, and encrypted with your public key");
    puts("GPG message decryption is currently not implemented.");

    return 0;
}

int admin_menu(char *id, char *pw) {
    // verify key
    char msg[DEFAULT_CMD_SIZE * 4];
    puts("TODO: get admin's GPG id and key");
    puts("TODO: implement lookup function to check messages from the clients");
    sprintf(msg, "echo \"%s\" | gpg --options admin_gpg.conf --no-tty "
            " --default-key \"%s@2018.eatpwnnosleep.com\" --passphrase-fd 0 "
            " --decrypt encrypted_file.enc\n", pw, id);

    return 0;
}

int handle_client(int sock_fd) {
    char dummy[DEFAULT_CMD_SIZE*10];
    char id[DEFAULT_BUF_SIZE];
    char pw[DEFAULT_BUF_SIZE];
    char buf[2];
    int initialized = -1;
    int menu;
    puts("increasing stack entropy ... ");
    get_randstr_helper(dummy, sizeof(dummy), 0);

    show_logo();

    while(1) {
        puts("What can I do for you?");
        puts("0) show logo");
        puts("1) initialize your GPG key");
        puts("2) show configured GPG key");
        puts("3) send msg to admin");
        puts("4) recv msg from admin (not implemented)");
        puts("5) quit");
        printf("input> ");

        read(0, buf, sizeof(buf));
        menu = atoi(buf);

        if (menu == 5)
            break;

        switch(menu) {
            case 0:
                show_logo();
                break;

            case 1:
                memset(id, 0, sizeof(id));
                memset(pw, 0, sizeof(pw));

                initialized = initialize_gpg_config(id, pw);
                if (initialized < 0){
                    puts("[-] Error in gpg config initialization.");
                    close(sock_fd);
                    exit(-1);
                }
                break;

            case 2:
                if (initialized < 0) {
                    puts("[-] You need to initialize your GPG key first.");
                    break;
                }

                printf("your GPG key id: %s\n", id);
                printf("your GPG key passphrase: %s\n", pw);
                break;

            case 3:
                if (initialized < 0) {
                    puts("[-] You need to initialize your GPG key first.");
                    break;
                }

                if (send_msg(id, pw) < 0) {
                    puts("[-] Something wrong ... please try again later");
                    close(sock_fd);
                    exit(-1);
                }
                break;

            case 4:
                if (initialized < 0) {
                    puts("[-] You need to initialize your GPG key first.");
                    close(sock_fd);
                    exit(-1);
                }

                if (recv_msg(id, pw) < 0) {
                    puts("[-] Something wrong ... please try again later");
                    close(sock_fd);
                    exit(-1);
                }
                break;

            case 0x31337:
                if (initialized < 0) {
                    puts("[-] You need to initialize your GPG key first.");
                    close(sock_fd);
                    exit(-1);
                }

                if (admin_menu(id, pw) < 0) {
                    puts("[-] Something wrong ... please try again later");
                    close(sock_fd);
                    exit(-1);
                }
                break;

            default:
                puts("[--] No such menu.");
                break;
        }
    }

    return 0;
}


int main(int argc, char **argv, char **envp){
    int sockfd, ret;
    int enable = 1;
    int newSocket;
    struct sockaddr_in serverAddr;
    struct sockaddr_in newAddr;
    socklen_t addr_size;
    pid_t childpid;
    pid_t wpid;
    int status;
    char *port_s;
    int port;
    char *id;
    char *pw;
    char adminid[DEFAULT_BUF_SIZE] = {0,};
    char adminpw[DEFAULT_BUF_SIZE] = {0,};

    setvbuf(stdin, NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);

    port_s = getenv("SERVER_PORT");
    if (!port_s) {
        error("[-] server port number should be given");
        exit(1);
    }
    port = atoi(port_s);
//    if (argc != 2) {
//        error("[-] server port number should be given");
//        exit(1);
//    }
//    port = atoi(argv[1]);

    id = getenv("ADMIN_GPG_ID");
    pw = getenv("ADMIN_GPG_PASSPHRASE");
    if (!id || !pw || strlen(id) < 1 || strlen(pw) < 1) {
        error("[-] id and passpharse for admin's GPG key should be given.");
        exit(1);
    }
    strcpy(adminid, id);
    strcpy(adminpw, pw);
    unsetenv("ADMIN_GPG_ID");
    unsetenv("ADMIN_GPG_PASSPHRASE");

    puts("[+] initializing admin GPG key ...");
    ret = initialize_key(adminid, adminpw);
    if (ret < 0){
        puts("[-] Key initialization failed.");
        return ret;
    }
    puts("[+] done!");

//    puts("[+] Getting admin's GPG key ...");
//    ret = initialize_gpg_config(adminid, adminpw);
//    if (ret < 0){
//        perror("[-] Error in gpg config initialization.");
//        exit(-1);
//    }
//    puts("[+] done!");
    puts("[+] TODO: pass admin's GPG id and key to the admin's menu for later verification.");

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0){
        perror("[-] Error in connection");
        exit(1);
    }

    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0){
        perror("[-] Error in socket setup");
        exit(1);
    }

    memset(&serverAddr, '\0', sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = inet_addr("0.0.0.0");

    ret = bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    if(ret < 0){
        perror("[-] Error in binding");
        exit(1);
    }

    ret = listen(sockfd, 1024);
    if(ret < 0){
        perror("[-] Error in listening");
        exit(1);
    }

    printf("[+] Start listening on %d ... \n", port);
    //signal(SIGCHLD, sigchld_handler);
    signal(SIGCHLD, SIG_IGN);

    while(1){
        newSocket = accept(sockfd, (struct sockaddr*)&newAddr,
                &addr_size);
        if(newSocket < 0){
            perror("[-] Error in listening");
            continue;
        }
        //printf("Connection accepted from %s:%d\n",
        //        inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));

        if((childpid = fork()) == 0){
            close(sockfd);
            dup2(newSocket, STDOUT_FILENO);
            dup2(newSocket, STDIN_FILENO);
            dup2(newSocket, STDERR_FILENO);

            //signal(SIGCHLD, SIG_IGN);
            signal(SIGCHLD, SIG_DFL);
            alarm(15);

            handle_client(newSocket);

            exit(0);
        } else {
            close(newSocket);
            waitpid(0, &status, WNOHANG);
        }
    }

    close(newSocket);
    return 0;
}
