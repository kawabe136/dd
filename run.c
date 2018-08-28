#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <memory.h>
#include <errno.h>
#include <fcntl.h>


#define ENABLE    1
#define DISENABLE 3
#define SET_DATA  4
#define GET_DATA  5

int main(int argc, char* argv[])
{
    int fd = 0;
    int ret = 0;
    int param = -1;

    /* open */
    fd = open("/dev/test_ioctl", O_RDWR);
    if(fd <= 0) {
        printf("open error.\n");
    }

    /* enable */
    ret = ioctl(fd, ENABLE, &param);
    printf("enable:%d\n", param);
    if(ret < 0) {
        printf("ioctl error. param=%d\n", param);
    }

    /* set data */
    param = -33;
    ret = ioctl(fd, SET_DATA, &param);
    printf("set_data:%d\n", param);
    if(ret < 0) {
        printf("ioctl error. param=%d\n", param);
    }

    /* get data */
    ret = ioctl(fd, GET_DATA, &param);
    printf("get_data:%d\n", param);
    if(ret < 0) {
        printf("ioctl error. param=%d\n", param);
    }

    /* default */
    ret = ioctl(fd, 10, &param);
    printf("default:%d\n", param);
    if(ret < 0) {
        printf("ioctl error. param=%d\n", param);
    }

    /* disable */
    ret = ioctl(fd, DISENABLE, &param);
    printf("disable:%d\n", param);
    if(ret < 0) {
        printf("ioctl error. param=%d\n", param);
    }

    close(fd);
    exit(0);
}

