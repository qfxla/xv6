#include "../kernel/types.h"
#include "../user/user.h"

int main(int argc, char **argv)
{
    char buf;
    int fd[2];
    pipe(fd);
    int fork_pid = fork();

    if (fork_pid > 0) {
        char p_send = 'a';
        write(fd[1], &p_send, sizeof(p_send));
        close(fd[1]);

        // wait child exit
        wait(0);

        read(fd[0], &buf, sizeof(buf));
        printf("%d: received pong\n", getpid(), buf);
        close(fd[0]);
    } else if (fork_pid == 0) {
        read(fd[0], &buf, sizeof(buf));
        printf("%d: received ping\n", getpid());

        char c_send = 'b';
        write(fd[1], &c_send, sizeof(c_send));

        close(fd[0]);
        close(fd[1]);
        exit(0);
    }
    exit(0);
}