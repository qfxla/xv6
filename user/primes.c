#include "../kernel/types.h"
#include "../user/user.h"
#define MSGSIZE 36
#define ONE '1'
#define ZERO '0'

void prime(int pipe_read, int pipe_write) {
    char buf[MSGSIZE];
    int val = 0;
    read(pipe_read, buf, MSGSIZE);
    for (int i = 0;i < MSGSIZE; i++) {
        if (buf[i] == ONE) {
            val = i;
            break;
        }
    }
    if (val == 0) {
        exit(0);
    }
    printf("prime %d\n", val);
    buf[val] = ZERO;
    for(int i = 0; i < MSGSIZE; i++) {
        if(i % val == 0) {
            buf[i] = ZERO;
        }
    }
    int pid = fork();
    if (pid > 0) {
        write(pipe_write, buf, MSGSIZE);
        wait(0);
    } else if (pid == 0) {
        prime(pipe_read, pipe_write);
    }
    exit(0);
}
int main (int argc, char* argv) {
    int fd[2];
    pipe(fd);
    char buf[MSGSIZE];
    for (int i = 0; i < MSGSIZE; i++) {
        buf[i] = ONE;
    }
    
    int pid = fork();
    if (pid > 0) {
        buf[0] = ZERO;
        buf[1] = ZERO;
        write(fd[1], buf, MSGSIZE);
        wait(0);
    } else if (pid == 0) {
        prime(fd[0], fd[1]);
        wait(0);
    }
    exit(0);
}

