#include <linux/module.h>
#include <linux/init.h>
#include <linux/gpio.h>
#include <linux/fs.h>

#define IO_A 11
#define IO_B 9
#define IO_C 25
#define IO_D 8
#define IO_TASTER 24

ssize_t my_write(struct file *file, const char __user *buf, size_t len, loff_t *off)
{
	u8 val;

	if(len > 0) {
		val = buf[0] - '0';
		gpio_set_value(IO_A, val & 1);
		gpio_set_value(IO_B, (val & 2) > 0);
		gpio_set_value(IO_C, (val & 4) > 0);
		gpio_set_value(IO_D, (val & 8) > 0);
	}

	return len;
}

ssize_t my_read(struct file *file, char __user *buf, size_t len, loff_t *off)
{
	char kbuf[16];
	int to_copy = (len < sizeof(kbuf)) ? len : sizeof(kbuf); 
	sprintf(kbuf, "%d\n", gpio_get_value(IO_TASTER));

	return to_copy - copy_to_user(buf, kbuf, to_copy);
}

struct file_operations fops = {
	.read = my_read,
	.write = my_write,
};

int __init my_init(void) 
{
	int status;

	status = gpio_request(IO_A, "IO_A");
	if(status != 0) {
		printk("segment - Fehler beim Belegen von IO_A\n");
		return status;
	}
	status = gpio_request(IO_B, "IO_B");
	if(status != 0) {
		printk("segment - Fehler beim Belegen von IO_B\n");
		goto free_a;
	}
	status = gpio_request(IO_C, "IO_C");
	if(status != 0) {
		printk("segment - Fehler beim Belegen von IO_C\n");
		goto free_b;
	}
	status = gpio_request(IO_D, "IO_D");
	if(status != 0) {
		printk("segment - Fehler beim Belegen von IO_D\n");
		goto free_c;
	}
	status = gpio_request(IO_TASTER, "IO_TASTER");
	if(status != 0) {
		printk("segment - Fehler beim Belegen von IO_TASTER\n");
		goto free_d;
	}

	status = gpio_direction_output(IO_A, 0);
	if(status != 0) {
		printk("segment - Fehler bei der Konfiguration von IO_A\n");
		goto free_taster;
	}
	status = gpio_direction_output(IO_B, 0);
	if(status != 0) {
		printk("segment - Fehler bei der Konfiguration von IO_B\n");
		goto free_taster;
	}
	status = gpio_direction_output(IO_C, 0);
	if(status != 0) {
		printk("segment - Fehler bei der Konfiguration von IO_C\n");
		goto free_taster;
	}
	status = gpio_direction_output(IO_D, 0);
	if(status != 0) {
		printk("segment - Fehler bei der Konfiguration von IO_D\n");
		goto free_taster;
	}
	status = gpio_direction_input(IO_TASTER);
	if(status != 0) {
		printk("segment - Fehler bei der Konfiguration von IO_TASTER\n");
		goto free_taster;
	}

	status = register_chrdev(64, "segment", &fops);
	if(status != 0) {
		printk("segment - Fehler beim Anlegen des cdevs mit Major 64\n");
		goto free_taster;
	}
	return 0;

free_taster:
	gpio_free(IO_TASTER);
free_d:
	gpio_free(IO_D);
free_c:
	gpio_free(IO_C);
free_b:
	gpio_free(IO_B);
free_a:
	gpio_free(IO_A);
	return status;
}

void __exit my_exit(void)
{
	unregister_chrdev(42, "segment");
	gpio_free(IO_TASTER);
	gpio_free(IO_D);
	gpio_free(IO_C);
	gpio_free(IO_B);
	gpio_free(IO_A);
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Me");
MODULE_DESCRIPTION("Treiber für 7 Segmentanzeige");


