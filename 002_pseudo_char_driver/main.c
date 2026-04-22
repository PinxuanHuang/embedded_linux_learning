#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>

#define DEV_MEM_SIZE 512

/* pseudo device memory */
char device_buffer[DEV_MEM_SIZE];

/* Cdev variable */
struct cdev pcd_cdev;

/* file operations of the driver */
struct file_operations pcd_fops;

/* hold device number */
dev_t device_number;

static int __init pcd_driver_init(void)
{
    /* Dynamically allocate a device number */
    alloc_chrdev_region(&device_number, 0, 1, "pcd");

    /* Initialize the cdev structure with fops */
    cdev_init(&pcd_cdev, &pcd_fops);

    /* register a device cdev structure with VFS */
    pcd_cdev.owner = THIS_MODULE;
    cdev_add(&pcd_cdev, device_number, 1);

    return 0;
}

static void __exit pcd_driver_cleanup(void)
{
    return 0;
}

module_init(pcd_driver_init);
module_exit(pcd_driver_cleanup);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("GARYHUANG");
MODULE_DESCRIPTION("A pseudo device driver");