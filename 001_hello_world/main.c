#include <linux/module.h>

static int __init helloworld_init(void)
{
    pr_info("hello world\n");
    return 0;
}

static void __exit helloworld_exit(void)
{
    pr_info("GoodBye world\n");
}

module_init(helloworld_init);
module_exit(helloworld_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("GARYHUANG");
MODULE_DESCRIPTION("A simple hello world kernel module");
MODULE_INFO("RPI4");