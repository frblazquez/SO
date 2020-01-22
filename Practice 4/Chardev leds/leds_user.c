#include <stdio.h>
#include<stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <mtd/mtd-user.h>
#include <errno.h>

/*
*   This requires chardev_leds module to be installed in the kernel.
*
*   > make charcev_leds
*   > sudo insmod chardev_leds.ko 
*   > make leds_user
*   > sudo ./leds_user
*/
int main( void )
{
    int fd;
    char leds[1][3]={"1","2","3"};

    fd = open("/dev/leds", O_RDWR);
        
    while(true){
        lseek(fd, 0, SEEK_SET);
        write(fd, leds[0][i%3] , 1);
        delay(1000);
    }        
        
    close(fd);

    return 0;
}
