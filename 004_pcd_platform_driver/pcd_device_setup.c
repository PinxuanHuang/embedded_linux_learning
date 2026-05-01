#include <linux/module.h>
#include <linux/platform_device.h>
#include "platform.h"

struct pcdev_platform_data pcdev_pdata[2] =
    {
        /* data */
        [0] = {.size = 512, .perm = RDWR, .serial_number = "PCDEVABC1111"},
        [1] = {.size = 1024, .perm = RDWR, .serial_number = "PCDEVXYZ2222"}};

struct platform_device platform_pcdev_1 =
    {
        /* data */
        .name = "pseudo-char-device",
        .id = 0,
        .dev = {.platform_data = &pcdev_pdata[0]}};

struct platform_device platform_pcdev_2 =
    {
        /* data */
        .name = "pseudo-char_device",
        .id = 1,
        .dev = {.platform_data = &pcdev_pdata[1]}};

static int __init pcdev_platform_init(void)
{
    /* register platform device */
    platform_device_register(&platform_pcdev_1);
    platform_device_register(&platform_pcdev_2);

    return 0;
}

static void __exit pcdev_platform_exit(void)
{
    platform_device_unregister(&platform_pcdev_1);
    platform_device_unregister(&platform_pcdev_2);
}

module_init(pcdev_platform_init);
module_exit(pcdev_platform_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("GARYHUANG");
MODULE_DESCRIPTION("Module register platform devices");