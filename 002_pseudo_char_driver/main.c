#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/kdev_t.h>
#include <linux/uaccess.h>

#define DEV_MEM_SIZE 512

/* pseudo device memory */
char device_buffer[DEV_MEM_SIZE];

/* Cdev variable */
struct cdev pcd_cdev;

loff_t pcd_lseek(struct file *filp, loff_t offset, int whence)
{
    loff_t temp;
    pr_info("lseek requested\n");
    pr_info("current file position = %lld\n", filp->f_pos);

    switch (whence)
    {
    case SEEK_SET:
        if ((offset > DEV_MEM_SIZE) || (offset < 0))
            return -EINVAL;
        filp->f_pos = offset;
        break;
    case SEEK_CUR:
        temp = filp->f_pos + offset;
        if ((temp > DEV_MEM_SIZE) || (temp < 0))
            return -EINVAL;
        filp->f_pos = temp;
    case SEEK_END:
        temp = DEV_MEM_SIZE + offset;
        if ((temp > DEV_MEM_SIZE) || (temp < 0))
            return -EINVAL;
        filp->f_pos = temp;

    default:
        return -EINVAL;
        break;
    }

    pr_info("Updated file position = %lld\n", filp->f_pos);
    return filp->f_pos;
}

ssize_t pcd_read(struct file *filp, char __user *buff, size_t count, loff_t *f_pos)
{
    pr_info("read requested for %zu bytes\n", count);
    pr_info("current file position = %lld\n", *f_pos);

    /* Adjust the count first */
    if ((*f_pos + count) > DEV_MEM_SIZE)
        count = DEV_MEM_SIZE - *f_pos;

    /* copy to user space mem */
    if (copy_to_user(buff, &device_buffer[*f_pos], count))
    {
        return -EFAULT;
    }

    /* update the current file position */
    *f_pos += count;

    pr_info("Number of bytes successfully read = %zu\n", count);
    pr_info("Updated file position = %lld\n", *f_pos);

    /* return number of bytes which have been successfully read */
    return count;
}

ssize_t pcd_write(struct file *filp, const char __user *buff, size_t count, loff_t *f_pos)
{
    pr_info("write requested for %zu bytes\n", count);
    pr_info("current file position = %lld\n", *f_pos);

    /* Adjust the count first */
    if ((*f_pos + count) > DEV_MEM_SIZE)
        count = DEV_MEM_SIZE - *f_pos;

    if (!count)
        return -ENOMEM;

    /* copy from user space mem */
    if (copy_from_user(&device_buffer[*f_pos], buff, count))
    {
        return -EFAULT;
    }

    /* update the current file position */
    *f_pos += count;

    pr_info("Number of bytes successfully write = %zu\n", count);
    pr_info("Updated file position = %lld\n", *f_pos);

    /* return number of bytes which have been successfully write */
    return count;
}

int pcd_open(struct inode *inode, struct file *filp)
{
    pr_info("open requested\n");
    return 0;
}

int pcd_release(struct inode *inode, struct file *filp)
{
    pr_info("release requested\n");
    return 0;
}

/* file operations of the driver */
struct file_operations pcd_fops = {
    .open = pcd_open,
    .write = pcd_write,
    .read = pcd_read,
    .llseek = pcd_lseek,
    .release = pcd_release,
    .owner = THIS_MODULE};

/* hold device number */
dev_t device_number;

struct class *class_pcd;

struct device *device_pcd;

static int __init pcd_driver_init(void)
{
    int ret;

    /* Dynamically allocate a device number */
    ret = alloc_chrdev_region(&device_number, 0, 1, "pcd_devices");
    if (ret < 0)
    {
        pr_err("Alloc chrdev failed\n");
        goto out;
    }

    pr_info("Device number <major>:<minor> = %d:%d\n", MAJOR(device_number), MINOR(device_number));

    /* Initialize the cdev structure with fops */
    cdev_init(&pcd_cdev, &pcd_fops);

    /* register a device cdev structure with VFS */
    pcd_cdev.owner = THIS_MODULE;
    ret = cdev_add(&pcd_cdev, device_number, 1);
    if (ret < 0)
    {
        pr_err("cdev_add failed\n");
        goto unreg_chrdev;
    }

    /* create device class under /sys/class/ */
    class_pcd = class_create(THIS_MODULE, "pcd_class");
    if (IS_ERR(class_pcd))
    {
        pr_err("Class creation failed\n");
        ret = PTR_ERR(class_pcd);
        goto cdev_del;
    }

    /* populate the sysfs with device information */
    device_pcd = device_create(&class_pcd, NULL, device_number, NULL, "pcd");
    if (IS_ERR(device_pcd))
    {
        pr_err("device creation failed\n");
        ret = PTR_ERR(device_pcd);
        goto class_del;
    }

    pr_info("PCD module init successful\n");

    return 0;
class_del:
    class_destroy(class_pcd);
cdev_del:
    cdev_del(&pcd_cdev);
unreg_chrdev:
    unregister_chrdev_region(device_number, 1);
out:
    pr_info("Module insertion failed\n");
    return ret;
}

static void __exit pcd_driver_cleanup(void)
{
    return 0;
}

static void pcd_driver_cleanup(void)
{
    device_destroy(class_pcd, device_number);
    class_destroy(class_pcd);
    cdev_del(&pcd_cdev);
    unregister_chrdev_region(device_number, 1);
    pr_info("module unregister\n");
    return;
}

module_init(pcd_driver_init);
module_exit(pcd_driver_cleanup);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("GARYHUANG");
MODULE_DESCRIPTION("A pseudo device driver");