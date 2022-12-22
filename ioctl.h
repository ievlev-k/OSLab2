
#ifndef UNTITLED11_IOCTL_H
#define UNTITLED11_IOCTL_H


#define IOCTL_GET_THREAD_STRUCT _IOR('i', 0, struct thread_parameters*)
#define IOCTL_GET_INODE_STRUCT _IOR('i', 1, struct inode_parameters*)
struct user_thread_struct {
    unsigned long sp;
    unsigned short es;
    unsigned short ds;
    unsigned short fsindex;
    unsigned short gsindex;
    unsigned long fsbase;
    unsigned long gsbase;
};

struct thread_parameters {
    struct user_thread_struct* write_pointer;
    uint32_t pid;
};


struct user_inode_struct{
    unsigned long long i_ino; /* номер индекса */
    unsigned long long i_mode; /* права доступа */
    unsigned int  i_nlink;        /* количество жестких ссылок */
    unsigned long long  i_size;         /* размер файла в байтах */
    unsigned long i_mtime;  /* время последнего изменения файла в наносекундах*/
    unsigned long  i_count;        /* счетчик ссылок */

};

struct inode_parameters{
    struct user_inode_struct* write_pointer;
    char *path;
    int path_len;
};


#endif //UNTITLED11_IOCTL_H
