#include <linux/syscalls.h> /* For SYSCALL_DEFINEi() */
#include <linux/kernel.h>
#include <asm-generic/errno.h>
#include <linux/tty.h>      /* For fg_console */
#include <linux/kd.h>       /* For KDSETLED */
#include <linux/vt_kern.h>
#include <asm-generic/errno-base.h>
#include <linux/errno.h>

/*Marta Rodenas de Miguel & Cristian Pinto Lozano*/

struct tty_driver* kbd_driver= NULL;  /* Driver of leds */

/* Get driver handler */
struct tty_driver* get_kbd_driver_handler(void){
   printk(KERN_INFO "sys_ledctl: loading\n");
   printk(KERN_INFO "sys_ledctl: fgconsole is %x\n", fg_console);
   return vc_cons[fg_console].d->port.tty->driver;
}

/* Set led state to that specified by mask */
static inline int set_leds(struct tty_driver* handler, unsigned int mask){
    return (handler->ops->ioctl) (vc_cons[fg_console].d->port.tty, KDSETLED,mask);
}

/* Set input num to correctly mask */
int mask(int mask) {
	int i;
	switch(mask) 
	{
		case 0x1: i=0x1; break;
		case 0x2: i=0x4; break;
		case 0x3: i=0x5; break;
		case 0x4: i=0x2; break;
		case 0x5: i=0x3; break;
		case 0x6: i=0x6;break;
		case 0x7: i=0x7; break;
		default : i=0x0; break;
	}
	return i;
}

/* This syscall modify the the led state */
SYSCALL_DEFINE1(ledctl,unsigned int,leds) {
	if((leds<0)||(leds>7))
		return -EINVAL;
	kbd_driver= get_kbd_driver_handler();

	if(kbd_driver==NULL)
		return -ENODEV;

	leds = mask(leds);
	if(set_leds(kbd_driver,leds) < 0)
		return -ENOTSUPP;
	return 0;
}

/* Authors:
 *--------- Cristian Pinto Lozano
 *--------- Marta Rodenas de Miguel
 */
