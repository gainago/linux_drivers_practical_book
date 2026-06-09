/*
 * kb_leds.c - blinking leds of keyboard
 */

#include <linux/init.h>
#include <linux/kd.h> /* For KDSETLED. */
#include <linux/module.h>
#include <linux/tty.h> /* For tty_struct. */
#include <linux/vt.h> /* For MAX_NR_CONSOLES */
#include <linux/vt_kern.h> /* For fg_console. */
#include <linux/console_struct.h> /* For vc_cons. */

MODULE_DESCRIPTION("Example module illustrating the use of Keyboard"
		"LEDs.");

static struct timer_list my_timer;
static struct tty_driver *my_driver;
static unsigned long kb_led_status = 0; /* Keyboard LED status. */

#define BLINK_DELAY HZ / 5
#define ALL_LEDS_ON 0x07
#define RESTORE_LEDS 0xFF

static void my_timer_func(struct timer_list *unused)
{
	struct tty_struct *t = vc_cons[fg_console].d->port.tty;

	if (kb_led_status == ALL_LEDS_ON)
		kb_led_status = RESTORE_LEDS;
	else
		kb_led_status = ALL_LEDS_ON;
	(my_driver->ops->ioctl)(t, KDSETLED, kb_led_status);

	my_timer.expires = jiffies + BLINK_DELAY;
	add_timer(&my_timer);
}

static int __init kb_leds_init(void)
{
	int i = 0;

	pr_info("kb_leds: loading\n");
	pr_info("kb_leds: fgconsole is %x\n", fg_console);
	for (i = 0; i < MAX_NR_CONSOLES; i++) {
		if (!vc_cons[i].d)
			break;
		pr_info("poet_atkm: console[%i/%i] #%i, tty %p\n", i,
				MAX_NR_CONSOLES, vc_cons[i].d->vc_num, 					(void*)vc_cons[i].d->port.tty);
	}

	pr_info("kb_leds: finished scanning consoles\n");

	my_driver = vc_cons[fg_console].d->port.tty->driver;

	/* Firs setup timer for blinking LEDs. */
	timer_setup(&my_timer, my_timer_func, 0);
	my_timer.expires = jiffies + BLINK_DELAY;
	add_timer(&my_timer);

	return 0;
}

static void __exit kb_leds_cleanup(void)
{
	pr_info("%s: unloading...\n", __func__);
	del_timer(&my_timer);
	(my_driver->ops->ioctl)(vc_cons[fg_console].d->port.tty,
		       	KDSETLED, RESTORE_LEDS);
}

module_init(kb_leds_init);
module_exit(kb_leds_cleanup);

MODULE_LICENSE("GPL");
		
