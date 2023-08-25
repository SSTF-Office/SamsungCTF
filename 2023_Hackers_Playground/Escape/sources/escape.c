// start server: socat TCP-LISTEN:5000,reuseaddr,fork EXEC:"./escape2"
// chown root flag.txt
// chmod 644 flag.txt

// https://man7.org/linux/man-pages/man2/seccomp.2.html
#include <linux/seccomp.h> /* Definition of SECCOMP_* constants */
#include <linux/filter.h>  /* Definition of struct sock_fprog */
#include <sys/prctl.h>
#include <sys/syscall.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stddef.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>

// this is for dynamic memory management in getline() by POSIX 7, see https://stackoverflow.com/questions/314401/how-to-read-a-line-from-the-console-in-c
#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <linux/limits.h>
#include <sys/sendfile.h>

#define JIT_ADDR 0x50510000

// returns 0 = false in case of a system error
static inline int setup_seccomp(void)
{
    // must set to PR_SET_NO_NEW_PRIVS to 1, otherwise SECCOMP_MODE_FILTER will fail.
    // https://man7.org/linux/man-pages/man2/seccomp.2.html
    if (prctl(PR_SET_NO_NEW_PRIVS, 1, 0, 0, 0) == -1)
        return 0;

    // BPF filter program
    struct sock_filter filter[] = {
        BPF_STMT(BPF_LD | BPF_W | BPF_ABS, (offsetof(struct seccomp_data, nr))), // get syscall number
        BPF_JUMP(BPF_JMP | BPF_JGE | BPF_K, __X32_SYSCALL_BIT, 11, 0), // prevent user from calling x32 ABI
        BPF_JUMP(BPF_JMP | BPF_JEQ | BPF_K, __NR_execve, 10, 0),         // if (syscall == open()) kill else allow
        BPF_JUMP(BPF_JMP | BPF_JEQ | BPF_K, __NR_execveat, 9, 0),         // if (syscall == open()) kill else allow
        BPF_JUMP(BPF_JMP | BPF_JEQ | BPF_K, __NR_openat, 8, 0),         // if (syscall == open()) kill else allow
        BPF_JUMP(BPF_JMP | BPF_JEQ | BPF_K, __NR_open, 7, 0),         // if (syscall == open()) kill else allow
        BPF_JUMP(BPF_JMP | BPF_JEQ | BPF_K, __NR_chmod, 6, 0),        // if (syscall == chmod()) kill else allow
        BPF_JUMP(BPF_JMP | BPF_JEQ | BPF_K, __NR_fchmod, 5, 0),       // if (syscall == fchmod()) kill else allow
        BPF_JUMP(BPF_JMP | BPF_JEQ | BPF_K, __NR_chown, 4, 0),        // if (syscall == chown()) kill else allow
        BPF_JUMP(BPF_JMP | BPF_JEQ | BPF_K, __NR_fchown, 3, 0),       // if (syscall == fchown()) kill else allow
        BPF_JUMP(BPF_JMP | BPF_JEQ | BPF_K, __NR_lchown, 2, 0),       // if (syscall == lchown()) kill else allow
        BPF_JUMP(BPF_JMP | BPF_JEQ | BPF_K, __NR_chdir, 1, 0),        // if (syscall == chdir()) kill else allow
        BPF_JUMP(BPF_JMP | BPF_JEQ | BPF_K, __NR_fchdir, 0, 2),       // if (syscall == fchdir()) kill else allow

        // issue is here: we still do not check the use of x32 API, thus it can be called from the assembly
        BPF_STMT(BPF_RET | BPF_K, SECCOMP_RET_KILL),  // return kill
        BPF_STMT(BPF_RET | BPF_K, SECCOMP_RET_TRACE), // return trace
        BPF_STMT(BPF_RET | BPF_K, SECCOMP_RET_ALLOW), // return allow
    };

    struct sock_fprog bpf_prog = {
        .len = (unsigned short)(sizeof(filter) / sizeof(filter[0])),
        .filter = filter,
    };

    // enable SECCOMP filter mode
    return prctl(PR_SET_SECCOMP, SECCOMP_MODE_FILTER, &bpf_prog) == -1 ? 0 : 1;
}

void *setup_shellcode_executor(void **sc, size_t *sc_size)
{
    void *addr;
    size_t page_size;

    page_size = (size_t)sysconf(_SC_PAGESIZE); // get page size
    // mmap an +rwx memory page
    addr = mmap((void *)JIT_ADDR, page_size, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_PRIVATE | MAP_FIXED | MAP_ANONYMOUS, -1, 0);
    if (addr == MAP_FAILED)
        return 0;

    *sc = addr;
    *sc_size = page_size;

    return addr;
}

int main()
{
    // if (!prepare_chroot())
    //     return -1;

    if (!setup_seccomp())
        return -1;

    void *sc;
    size_t sc_size;

    if (!setup_shellcode_executor(&sc, &sc_size))
        return -1;
    // fill with NOPs and ends with RET instruction
    memset(sc, 0x90, sc_size);
    ((unsigned char *)sc)[sc_size - 1] = 0xC3;

    char buffer[128];
    while (1)
    {
        printf("Enter: \n");
        fflush(stdout);

        // SECCOMP blocks read() here, so it must be allowed unfortunately
        if (fgets(buffer, sizeof(buffer), stdin) != buffer)
            return -2;
        // exit prompt
        if (!strncmp(buffer, "done", 4))
            break;
        // printf
        printf("Entered: ");
        printf(buffer);
        fflush(stdout);
    }

    // execute it
    ((void (*)(void))sc)();
    // remove shellcode
    munmap(sc, sc_size);
    
    return 0;
}
