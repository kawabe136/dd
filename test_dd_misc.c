#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <memory.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/ioctl.h>


#define IKOMA_MAGIC 'k'
#define IKOMA_IOCTL_ENABLE              _IO(IKOMA_MAGIC,    0x01)
#define IKOMA_IOCTL_GET_DATA            _IOR(IKOMA_MAGIC,   0x02, int)
#define IKOMA_IOCTL_SET_DATA            _IOW(IKOMA_MAGIC,   0x03, int)
#define IKOMA_IOCTL_DISABLE             _IO(IKOMA_MAGIC,    0x04)


int main(int argc, char* argv[])
{
    int fd = 0;
    int ret = 0;
    int param = -1;

    /* open */
    fd = open("/dev/dd_misc", O_RDWR);
    if(fd <= 0) {
        printf("open error.\n");
    }

    /* enable */
    ret = ioctl(fd, IKOMA_IOCTL_ENABLE, &param);
    printf("enable:%d\n", param);
    if(ret < 0) {
        printf("ioctl error. param=%d\n", param);
    }

    /* set data */
    param = -3;
    ret = ioctl(fd, IKOMA_IOCTL_SET_DATA, &param);
    printf("set_data:%d\n", param);
    if(ret < 0) {
        printf("ioctl error. param=%d\n", param);
    }

    /* get data */
    ret = ioctl(fd, IKOMA_IOCTL_GET_DATA, &param);
    printf("get_data:%d\n", param);
    if(ret < 0) {
        printf("ioctl error. param=%d\n", param);
    }

    /* default */
    ret = ioctl(fd, 10, &param);
    printf("default:\n");
    if(ret < 0) {
        printf("ioctl error. param=%d\n", param);
    }

    /* disable */
    ret = ioctl(fd, IKOMA_IOCTL_DISABLE, &param);
    printf("disable:\n");
    if(ret < 0) {
        printf("ioctl error. param=%d\n", param);
    }

    close(fd);
    exit(0);
}

