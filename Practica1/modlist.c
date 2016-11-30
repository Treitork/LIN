#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/string.h>
#include <linux/vmalloc.h>
#include <asm-generic/uaccess.h>
#include <linux/list.h>


MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("ModList Kernel Module");
MODULE_AUTHOR("Marta Rodenas de Miguel & Cristian Pinto Lozano");

#define BUFFER       1028

static struct proc_dir_entry *proc_entry; 

struct list_item_t{
  int data;
  struct list_head links;
};
static struct list_head modlist; 

void remove_list(void){
  struct list_item_t *aux, *elem = NULL;
  
  list_for_each_entry_safe(elem, aux, &modlist, links){
    list_del(&(elem->links));
    vfree(elem);
  }
}

static ssize_t modlist_write(struct file *filp, const char __user *buf, size_t len, loff_t *off) {

  int buffer = BUFFER-1, data;
  char kbuf[BUFFER];

  struct list_item_t *item = NULL;
  struct list_head *node = NULL, *aux =NULL;

  if ((*off) > 0)
    return 0;
  
  if (len > buffer) {
    printk(KERN_INFO "modlist: run out of space!!\n");
    return -ENOSPC; 
  }

  if (copy_from_user( kbuf, buf, len ))  
    return -EFAULT;
  
  kbuf[len]='\0';
  *off+=len;

  if ( strcmp(kbuf,"cleanup\n") == 0){ 
    remove_list();
  }
  else if( sscanf(kbuf,"remove %d",&data)) { 
    list_for_each_safe(node, aux, &modlist) { 
      item = list_entry(node, struct list_item_t, links);
      if((item->data) == data){
        list_del(node);
        vfree(item);
      }
    }
  }
  else if( sscanf(kbuf,"add %d",&data)) { 
    item=(struct list_item_t *) vmalloc(sizeof (struct list_item_t));
    item->data = data;
    list_add_tail(&item->links, &modlist);
  }

  return len;
}

static ssize_t modlist_read(struct file *filp, char __user *buf, size_t len, loff_t *off) {
  
  struct list_item_t *item = NULL; 
  struct list_head *node = NULL; 
  
  char kbuf[BUFFER] = "";
  char *sbuffer = kbuf;


  if ((*off) > 0)
      return 0;
  
  
  list_for_each(node, &modlist) { 
    item = list_entry(node, struct list_item_t, links);
    sbuffer += sprintf(sbuffer, "%d\n", item->data);
  }
    
  if (len<sbuffer-kbuf)
    return -ENOSPC; 
  
  if (copy_to_user(buf, kbuf, sbuffer-kbuf))
    return -EINVAL;
    
  (*off)+=len; 
  return sbuffer-kbuf; 
}

static const struct file_operations proc_entry_fops = {
    .read = modlist_read,
    .write = modlist_write,    
};



int init_modlist_module( void )
{
  int ret = 0;
  INIT_LIST_HEAD(&modlist);
  proc_entry = proc_create( "modlist", 0666, NULL, &proc_entry_fops); 
  if (proc_entry == NULL) {
    ret = -ENOMEM;
    printk(KERN_INFO "Modlist: Error while creating /proc entry\n");
  }else {
    printk(KERN_INFO "Modlist: Module succesfuly loaded\n");
  }
  return ret;
}


void exit_modlist_module( void )
{
  remove_list();
  remove_proc_entry("modlist", NULL); // eliminar la entrada del /proc
  printk(KERN_INFO "Modlist: Module correctly unloaded.\n");
}


module_init( init_modlist_module );
module_exit( exit_modlist_module );
