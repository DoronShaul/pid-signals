#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

void sigCatcher()
{
    signal(SIGINT, sigCatcher);
    printf("PID %d caught one\n", getpid());
    kill(getpid() + 1, SIGINT);
}

int main()
{
    pid_t cpid;
    for (int i = 0; i < 5; i++)
    {
        if (cpid = fork() == 0)
        {
            printf("PID %d ready\n", getpid());
            break;
        }
    }

    signal(SIGINT, sigCatcher);

    if (cpid == 0)
    {
        sleep(1);
        kill(getpid() + 1, SIGINT);
    }

    if (cpid > 0)
    {
        pause();
        if (getpid() != getppid() + 5)
        {
            kill(getpid() + 1, SIGINT);
        }
        exit(0);
    }

    sleep(1);
    for (int i = 1; i < 6; i++)
    {
        waitpid(getpid() + i, 0, 0);
        printf("PID %d is dead\n",getpid()+i);
    }
    exit(0);

    return 0;
}