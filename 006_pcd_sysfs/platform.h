struct pcdev_platform_data
{
    /* data */
    int size;
    int perm;
    const char *serial_number;
};

/*permission codes */
#define RDONLY 0x01
#define WRONLY 0X10
#define RDWR 0x11