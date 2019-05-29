#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <string.h>
#include "md5.h"
#define MSGSIZE 21
#define HASHSIZE 33

int boolean = 0;
char *strSig;

int isHashed(char *str)
{
    if (strlen(str) == 32 * sizeof(char))
    {
        return 1;
    }
    return -1;
}

void sigHash()
{
    signal(SIGINT, sigHash);
    boolean = isHashed(strSig);
}

int main(int argc, char *argv[])
{
    signal(SIGINT, sigHash);
    char inbuf[MSGSIZE];
    char hashBuf[MSGSIZE];
    char hashed[HASHSIZE];
    char *getHash;
    int parentToChild[2], childToParent[2], pid, nbytes;

    if (pipe(parentToChild) < 0)
    {
        exit(1);
    }

    if (pipe(childToParent) < 0)
    {
        exit(1);
    }

    /* continued */
    if ((pid = fork()) > 0)
    {
        if (fgets(inbuf, sizeof(inbuf), stdin) != NULL)
        {

            write(parentToChild[1], inbuf, MSGSIZE);

            if (nbytes = read(childToParent[0], hashed, HASHSIZE) > 0)
            {
                strSig = hashed;
                pause();
                if (boolean == 1)
                {
                    printf("encrypted by process %d: %s\n", getpid() ,hashed);
                    kill(pid, SIGKILL);
                }
                else
                {
                    printf("the given string isn't hashed.\n");
                }
            }
        }
    }

    else
    {
        read(parentToChild[0], hashBuf, MSGSIZE);
        //getHash = "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAaa";

        getHash=md5(hashBuf);
        printf("plain text: %s\n", hashBuf);
        write(childToParent[1], getHash, HASHSIZE);
        pid_t parentPid = getppid();
        sleep(1);
        kill(parentPid, SIGINT);
        exit(0);
    }
    return 0;
}
