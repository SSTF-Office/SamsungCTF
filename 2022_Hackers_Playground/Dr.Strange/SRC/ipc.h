#ifndef _IPC_H
#define _IPC_H

#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <string.h> // memset
#include <unistd.h>


/* msg */
#define MSG_SIZE_MAX (1024)
#define MAX_LEN 128

/* permission */
#define PERM_FILE (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH)

/* shared memory */
#define SHM_NAME "/test_shm"
#define SHM_CLIENT_BUF_IDX (0)
#define SHM_SERVER_BUF_IDX (512)

struct shm_msg {
    int status;
    int what;
    size_t msg_len;
    char msg[MAX_LEN];
};

int server_fd;
void *addr;
static int ipc_fd;
static struct shm_msg *client_msg;
static struct shm_msg *server_msg;
//unsigned char msg[MSG_SIZE_MAX] = {0,};

/* initialize IPC */
static void setIPC()
{
	/* create shm */
    if((server_fd = shm_open(SHM_NAME, O_CREAT|O_RDWR,PERM_FILE)) == -1) {
        printf("shm_open error : %s\n", strerror(errno));
        return;
    }
	
	/* set size */
    if(ftruncate(server_fd, MSG_SIZE_MAX) == -1) {
        printf("ftruncate error : %s\n", strerror(errno));
        return;
    }
	
	/* mmap */
    addr = mmap(NULL, MSG_SIZE_MAX, PROT_READ | PROT_WRITE, MAP_SHARED, server_fd, 0);
    if(addr == MAP_FAILED) {
        printf("mmap error : %s\n", strerror(errno));
        return;
    }
    memset(addr, 0, MSG_SIZE_MAX);
	
	client_msg = (struct shm_msg *)((char*)addr + SHM_CLIENT_BUF_IDX); 
    server_msg = (struct shm_msg *)((char*)addr + SHM_SERVER_BUF_IDX); 
}
void releaseIPC()
{
	/* destroy shm */
    if(munmap(addr, MSG_SIZE_MAX) == -1) {
        printf("munmap error : %s\n", strerror(errno));
    }
	if(close(server_fd) == -1) {
        printf("close error : %s\n", strerror(errno));
    }
	if(shm_unlink(SHM_NAME) == -1) {
        printf("shm_unlink error : %s\n", strerror(errno));
    }
} 

/* connect client to server */
static int security_daemon_connect(void)
{	
	/* get shm */
    if((ipc_fd = shm_open(SHM_NAME, O_RDWR, PERM_FILE)) == -1) {
        printf("shm_open error : %s\n", strerror(errno));
        return -1;
    }
	
	/* mmap */
    addr = mmap(NULL, MSG_SIZE_MAX, PROT_READ | PROT_WRITE, MAP_SHARED, ipc_fd, 0);
    if(addr == MAP_FAILED) {
        printf("mmap error : %s\n", strerror(errno));
        goto out;
    }
	
	client_msg = (struct shm_msg *)((char*)addr + SHM_CLIENT_BUF_IDX);
    server_msg = (struct shm_msg *)((char*)addr + SHM_SERVER_BUF_IDX);
	
	return 0;

out:
	/* close shm */
    if(munmap(addr, MSG_SIZE_MAX) == -1) {
        printf("munmap error : %s\n", strerror(errno));
	}
    if(close(ipc_fd) == -1) {
        printf("close error : %s\n", strerror(errno));
    }
	
	return -1;
}



#endif
