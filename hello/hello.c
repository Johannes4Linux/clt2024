#include <linux/module.h>
#include <linux/init.h>

int __init my_init(void) 
{
	printk("hello - Servus!\n");
	return 0;
}

void __exit my_exit(void)
{
	printk("hello - Mach's gut!\n");
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Me");
MODULE_DESCRIPTION("Hallo Welt km");


