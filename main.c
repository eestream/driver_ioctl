#include <stdio.h>
#include <fcntl.h>
#include "ioctl_basic.h"

int main()
{
    int fd = open("/dev/temp", O_RDWR);

    if(fd == -1)
    {
        perror("error");
        exit(-1);
    }


    ioctl(fd, IOCTL_HELLO);

    close(fd);

    return 0;
}
