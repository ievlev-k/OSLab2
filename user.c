
#include <stdio.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <inttypes.h>


#include "ioctl.h"

#define DEVICE_FILE "/dev/etc_driver"



int main(int argc, char **argv){
    if (argc < 3){
        printf("Program need 2 arguments!\n");
        return -1;
    }



    int option = 0;

    if (strcmp(argv[1], "thread_struct") == 0) option = 1;
    else if (strcmp(argv[1], "inode") == 0) option = 2;
    else {
        printf("Please check your arguments. The second argument thread_struct or inode\n");
        return -1;
    }

    int fd = open("/dev/etc_driver", O_WRONLY);
    if(fd < 0){
        printf("Can't open device file!\n");
        return -1;
    }

    if (option == 1){
        struct user_thread_struct thread;
        uint32_t pid = strtoul(argv[2], NULL, 10);
        struct thread_parameters thread_params = { .write_pointer = &thread, .pid = pid };
        uint8_t ret = ioctl(fd, IOCTL_GET_THREAD_STRUCT, &thread_params);
        if (ret == 0) {
            printf("es: %hu\n", thread.es);
            printf("ds: %hu\n", thread.ds);
            printf("fsindex: %hu\n", thread.fsindex);
            printf("gsindex: %hu\n", thread.gsindex);
            printf("frame base: %lu\n", thread.fsbase);
            printf("gsbase: %lu\n", thread.gsbase);
            printf("[GR1] kernel stack pointer: %lu\n", thread.sp);
        }
        else printf("Could not get the information. Please check the parameters.\n");
    } else if (option == 2){
        struct user_inode_struct inode;
        char *path = argv[2];
        struct inode_parameters inode_params = { .write_pointer = &inode, .path = path};

        uint8_t ret = ioctl(fd, IOCTL_GET_INODE_STRUCT, &inode_params);
        if (ret == 0) {
            printf("inode number: %llu\n", inode.i_ino);
            printf("inode file size: %llu\n", inode.i_size);
            printf("inode references count: %lu\n", inode.i_count);
//            printf("inode access permissions: %llu\n", inode.i_mode);
//            printf("inode last modify time: %lu", inode.i_mtime);
//            printf("inode hard link count: %d", inode.i_nlink);
        }
        else printf("Could not get the information. Please check the parameters.\n");
    }
    return 0;
}


