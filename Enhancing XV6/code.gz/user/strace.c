#include "user/user.h"

int main(int argc,char** argv)
{
    if(argc < 3){
        printf("no. of commands are less for strace\n");
        exit(0);
    }
    int pid = fork();
    if(pid == 0)
    {
        trace(atoi(argv[1]));
        exec(argv[2],&argv[2]);
    }
    wait(0);
    exit(0);
}