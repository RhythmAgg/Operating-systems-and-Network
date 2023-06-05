#include "user/user.h"
#include "kernel/types.h"
#include "kernel/defs.h"

int main(int argc,char** argv)
{
    set_priority(atoi(argv[1]),atoi(argv[2]));
    exit(0);
}