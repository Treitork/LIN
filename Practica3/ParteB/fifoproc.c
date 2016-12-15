#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/string.h>
#include <linux/vmalloc.h>
#include <asm-generic/uaccess.h>
#include <linux/list.h>
#include <linux/semaphore.h>
#include "cbuffer.h"

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("FifoProc Module - LIN");
MODULE_AUTHOR("Marta Rodenas de Miguel & Cristian Pinto Lozano");

#define BUFFER_LENGTH 50

DEFINE_SEMAPHORE(sem_mtx);
int prod_count, cons_count, producers_in_queue, consumers_in_queue;
struct semaphore prod, cons;
struct proc_dir_entry *proc_entry;
cbuffer_t *cbuffer;

static int open_fifoProc(struct inode *node, struct file *filp) {
	if(down_interruptible(&sem_mtx)) 
		return -EINTR;
	if(filp->f_mode & FMODE_READ) {
		cons_count++;
		if(producers_in_queue > 0) {
			up(&prod);	
			producers_in_queue--;		
		}

		while(prod_count==0) {
			consumers_in_queue++;
			up(&sem_mtx);
			if(down_interruptible(&cons)) {
				down(&sem_mtx);
				consumers_in_queue--;
				up(&sem_mtx);
				return -EINTR;
			}
			if(down_interruptible(&sem_mtx)) 
				return -EINTR;
		}		
	}
	else {
		prod_count++;
		if(consumers_in_queue > 0) {
			up(&cons);
			consumers_in_queue--;		
		}

		while(cons_count==0) {
			producers_in_queue++;
			up(&sem_mtx);
			if(down_interruptible(&prod)) {
				down(&sem_mtx);
				producers_in_queue--;
				up(&sem_mtx);
				return -EINTR;
			}
			if(down_interruptible(&sem_mtx)) 
				return -EINTR;
		}		
	}
	up(&sem_mtx);
	return 0;
}

static int release_fifoProc(struct inode *node, struct file *filp) {
	if(down_interruptible(&sem_mtx)) 
		return -EINTR;
	if(filp->f_mode & FMODE_READ) {
		cons_count--;
		if(producers_in_queue > 0) {
			up(&prod);
			producers_in_queue--;		
		}
	}
	else {
		prod_count--;
		if(consumers_in_queue > 0) {
			up(&cons);
			consumers_in_queue--;		
		}
	}
	if((cons_count==0)&&(prod_count==0))
		clear_cbuffer_t(cbuffer);
	up(&sem_mtx);
	return 0;
}

static ssize_t write_fifoProc(struct file *filp, const char __user *buf, size_t len, loff_t *off) {
	char kbuf[BUFFER_LENGTH+1];

	if ((*off) > 0)
    		return 0;

	if(len>BUFFER_LENGTH)
		return -ENOMEM;
	if (copy_from_user( kbuf, buf, len ))  
    	return -EFAULT;
	kbuf[len] = '\0';
	if(down_interruptible(&sem_mtx)) 
		return -EINTR;
		while((nr_gaps_cbuffer_t(cbuffer)<len) && (cons_count > 0)) {
			producers_in_queue++;
			up(&sem_mtx);
			if(down_interruptible(&prod)) {
				down(&sem_mtx);
				producers_in_queue--;
				up(&sem_mtx);
				return -EINTR;
			}
			if(down_interruptible(&sem_mtx)) 
				return -EINTR;
		}
		if(cons_count == 0) 
			return -EPIPE;
		insert_items_cbuffer_t(cbuffer, kbuf, len);
		
		if(consumers_in_queue > 0) {
			up(&cons);
			consumers_in_queue--;
		}
	up(&sem_mtx);
	return len;
}

static ssize_t read_fifoProc(struct file *filp, char __user *buf, size_t len, loff_t *off) {
	char kbuf[BUFFER_LENGTH+1];
	
	if ((*off) > 0)
    		return 0;
	if (len > BUFFER_LENGTH)
		return -ENOMEM;

	if(down_interruptible(&sem_mtx)) return -EINTR;
		while((len > size_cbuffer_t(cbuffer)) && (prod_count > 0)) {
			consumers_in_queue++;
			up(&sem_mtx);
			if(down_interruptible(&cons)) {
				down(&sem_mtx);
				consumers_in_queue--;
				up(&sem_mtx);
				return -EINTR; 
			}
			if(down_interruptible(&sem_mtx)) 
				return -EINTR;
		}
		if(prod_count == 0 && is_empty_cbuffer_t(cbuffer)) {
			up(&sem_mtx);
			return -EPIPE;
		}
		remove_items_cbuffer_t(cbuffer, kbuf, len);
		
		if(producers_in_queue > 0) {
			up(&prod);
			producers_in_queue--;
		}
	up(&sem_mtx);

	if (copy_to_user(buf, kbuf, len))
    	return -EINVAL;
	return len;
}

static const struct file_operations fops = {
	.open    = open_fifoProc,
	.read    = read_fifoProc,
	.write   = write_fifoProc,
	.release = release_fifoProc,
};

int init_fifoProc_module( void ) {
	int ret = 0;
	prod_count=0; producers_in_queue=0;
	cons_count=0; consumers_in_queue=0;
	sema_init(&prod, 0);
	sema_init(&cons,0);
	cbuffer = create_cbuffer_t(BUFFER_LENGTH);
	proc_entry = proc_create( "fifoProc", 0666, NULL, &fops); 

	if (proc_entry == NULL) {
		ret = -ENOMEM;
		printk(KERN_INFO "fifoProc: Error while creating /proc entry\n");
	}
	else
		printk(KERN_INFO "fifoProc: Module succesfuly loaded\n");
	return ret;
}


void exit_fifoProc_module( void ) {
	destroy_cbuffer_t ( cbuffer );
	remove_proc_entry("fifoProc", NULL);
	printk(KERN_INFO "fifoProc: Module succesfuly unloaded\n");
}


module_init( init_fifoProc_module );
module_exit( exit_fifoProc_module );
