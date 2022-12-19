



#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>

#include <linux/ioctl.h>
#include <linux/fs.h> //for file operations
#include <linux/export.h> //for file operations
#include <linux/pid.h>
#include <linux/processor.h>
#include <linux/uaccess.h> //for user copy
#include <linux/path.h>
#include <linux/version.h>
#include <linux/sched.h>
#include <linux/namei.h>
#include <linux/netdevice.h>



#include "ioctl.h"








MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Module for sending debug info about thread_struct and pci_dev");
MODULE_VERSION("0.01");
static int ret_val;
long device_ioctl(struct file* file,  unsigned int cmd, unsigned long arg){

    struct thread_parameters thread_params;
    struct task_struct* task;
    struct thread_struct thread;
    struct user_thread_struct ret_thread;

    struct inode_parameters inode_params;
    struct user_inode_struct ret_inode;
    struct inode* inode;
    struct path path;
    if (cmd == IOCTL_GET_THREAD_STRUCT){
        if (copy_from_user(&thread_params, (struct thread_parameters*) arg, sizeof(struct thread_parameters)) != 0 ) {
            pr_err("Couldn't read the parameters\n");
            return -1;
        }

        printk("Got PID %d", thread_params.pid);
        task = get_pid_task(find_get_pid(thread_params.pid), PIDTYPE_PID);
        if (task == NULL) {
            pr_err("Could not read thread with PID %d\n", thread_params.pid);
            return -1;
        }
        thread = task->thread;
        ret_thread.es = thread.es;
        ret_thread.ds = thread.ds;
        ret_thread.fsindex = thread.fsindex;
        ret_thread.gsindex = thread.gsindex;
        ret_thread.fsbase = thread.fsbase;
        ret_thread.gsbase = thread.gsbase;
        ret_thread.sp = thread.sp;
        copy_to_user(thread_params.write_pointer, &ret_thread, sizeof(struct user_thread_struct));
    }else if (cmd == IOCTL_GET_INODE_STRUCT){
        if (copy_from_user(&inode_params, (struct inode_parameters*) arg, sizeof (struct inode_parameters)) != 0){
            pr_err("Couldn't read the parameters\n");
            return -1;
        }


        printk(inode_params.path);




        if ( kern_path(inode_params.path, LOOKUP_FOLLOW, &path) != 0){
            pr_err("File not found!\n");
            return  -1;
        }



        inode = path.dentry-> d_inode;
        ret_inode.i_ino = inode -> i_ino;
        ret_inode.i_count = inode -> i_count.counter;
//        ret_inode.i_mode = (unsigned  long long )inode -> i_mode;
        ret_inode.i_size = inode -> i_size;
//        ret_inode.i_mtime = inode -> i_mtime.tv_sec;
//        ret_inode.i_nlink = inode -> i_nlink;

        copy_to_user(inode_params.write_pointer, &ret_thread, sizeof (struct inode_parameters));


    }
    return 0;
}

static struct file_operations fops = {
        .owner = THIS_MODULE,

        .unlocked_ioctl = device_ioctl,
};
static int device_init(void){

    pr_info("Loading the module...\n");
    ret_val = register_chrdev(0,  "etc_driver", &fops);
    if (ret_val < 0) {
        pr_err("Failed to register the device %s, return code %d\n", "etc_driver", ret_val);
        return ret_val;
    }
    pr_info("Successfully registered the device %s with major number %d\n", "etc_driver", ret_val);
    return 0;
}

static void device_exit(void) {
    unregister_chrdev(ret_val,  "etc_driver");
    printk("Successfully unregistered the device %s\n",  "etc_driver");
}


module_init(device_init);
module_exit(device_exit);






