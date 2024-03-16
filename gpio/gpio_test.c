#include <linux/module.h>
#include <linux/init.h>
#include <linux/gpio.h>

#define IO_LED 11
#define IO_TASTER 24

static int __init my_init(void)
{
	int status;

	status = gpio_request(IO_LED, "my_led");
	if(status != 0) {
		printk("gpio_test - Fehler beim Anfordern des IOs %d\n", IO_LED);
		return status;
	}

	status = gpio_request(IO_TASTER, "my_button");
	if(status != 0) {
		printk("gpio_test - Fehler beim Anfordern des IOs %d\n", IO_TASTER);
		goto free_led;
	}

	status = gpio_direction_output(IO_LED, 0);
	if(status != 0) {
		printk("gpio_test - Fehler bei der Konfiguration des IOs %d als Ausgang\n", IO_LED);
		goto free_button;
	}

	status = gpio_direction_input(IO_TASTER);
	if(status != 0) {
		printk("gpio_test - Fehler bei der Konfiguration des IOs %d als Eingang\n", IO_TASTER);
		goto free_button;
	}

	/* Setzen des IOs */
	gpio_set_value(IO_LED, 1);

	/* Lesen des Tasters */
	printk("gpio_test - Taster ist %sgedrückt\n", gpio_get_value(IO_TASTER) ? "" : "nicht ");

	return 0;
free_button:
	gpio_free(IO_TASTER);
free_led:
	gpio_free(IO_LED);
	return status;
}

static void __exit my_exit(void)
{
	gpio_set_value(IO_LED, 0);
	gpio_free(IO_TASTER);
	gpio_free(IO_LED);
}

module_init(my_init);
module_exit(my_exit);

/* Meta Information */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Johannes Roith");
MODULE_DESCRIPTION("Ein Kernelmodul, das die Nutzung von GPIOs demonstriert");

