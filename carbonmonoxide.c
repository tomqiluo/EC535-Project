#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/timer.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/proc_fs.h>
#include <linux/fcntl.h>
#include <asm/system_misc.h>
#include <linux/uaccess.h>
#include <asm/uaccess.h>
#include <linux/sched.h>
#include <linux/jiffies.h>

// carbon monoxide AIN0 and GPIO44
#define MQ7_PIN 44



MODULE_LICENSE("GPL");
MODULE_AUTHOR("Qi Luo");
MODULE_DESCRIPTION("A simple kernel module");


static struct timer_list mytimer;

static int carbonmonoxide_init(void);
static void carbonmonoxide_exit(void);
static ssize_t carbonmonoxide_read(struct file *filp, char *buf, size_t count, loff_t *f_pos);
static ssize_t carbonmonoxide_write(struct file *filp, const char *buf, size_t count, loff_t *f_pos);
static int carbonmonoxide_open(struct inode *inode, struct file *filp);
static int carbonmonoxide_release(struct inode *inode, struct file *filp);
void mytimer_callback(struct timer_list *timer);

struct file_operations carbonmonoxide_fops = {
	read: carbonmonoxide_read,
	write: carbonmonoxide_write,
	open: carbonmonoxide_open,
	release: carbonmonoxide_release
};








const unsigned user_input_capacity = 1024;
const unsigned user_output_capacity = 1024;
static unsigned bite = 1024;

static const int carbonmonoxide_major = 61;


// Buffer to store data from user
static char *user_input;

// Buffer to store data to send to user
static char *user_output;

float voltage_value;


void mytimer_callback(struct timer_list *timer)
{
    int value;
    value = gpio_get_value(MQ7_PIN);
    voltage_value = value;
    printk("Voltage : %f \n",voltage_value);
    mod_timer(&mytimer, jiffies + msecs_to_jiffies(1000)); // mod timer for 1 second
}







static int __init carbonmonoxide_init(void)
{
int result,ret;
    printk(KERN_INFO "My module loaded\n");

    result = register_chrdev(carbonmonoxide_major, "carbonmonoxide", &carbonmonoxide_fops);
	if (result < 0)
	{
		printk(KERN_ALERT
			"carbonmonoxide: cannot obtain major number %d\n", carbonmonoxide_major);
		return result;
	}


   
    
	ret = gpio_request(MQ7_PIN, "mq7");
if (ret != 0) {
    printk(KERN_ERR "Error requesting MQ7_PIN\n");
    return ret;
}

printk(KERN_INFO "Initializing timer module\n");
    timer_setup(&mytimer, mytimer_callback, 0);
    mod_timer(&mytimer, jiffies + msecs_to_jiffies(1000));

   return 0;
}

static void __exit carbonmonoxide_exit(void)
{   
    unregister_chrdev(carbonmonoxide_major, "carbonmonoxide");
	/* Freeing buffer memory */
    gpio_free(MQ7_PIN);

      printk(KERN_INFO "Cleaning up timer module\n");
    del_timer(&mytimer);

	
}

static int carbonmonoxide_open(struct inode *inode, struct file *filp)
{
	/* Success */
	return 0;
}

static int carbonmonoxide_release(struct inode *inode, struct file *filp)
{
	/* Success */
	return 0;
}

static ssize_t carbonmonoxide_read(struct file *filp, char *buf, size_t count, loff_t *f_pos)
{ 
	int temp;
	char tbuf[256], *tbptr = tbuf;
	unsigned int carbonmonoxide_len = strlen(user_output);

	/* end of buffer reached */
	if (*f_pos >= carbonmonoxide_len)
	{
		return 0;
	}

	/* do not go over the end */
	if (count > carbonmonoxide_len - *f_pos)
		count = carbonmonoxide_len - *f_pos;

	/* Transfer data to user space */ 
	if (copy_to_user(buf, user_output + *f_pos, count))
	{
		return -EFAULT;
	}

	/* Log the read operation */
	tbptr += sprintf(tbptr, "read called: process id %d, command %s, count %zu, offset %lld chars",
		current->pid, current->comm, count, *f_pos);

	/* Add the read data to the log */
	for (temp = *f_pos; temp < count + *f_pos; temp++)					  
		tbptr += sprintf(tbptr, "%c", user_output[temp]);

	/* Append the MQ7 sensor value to the output */


	/* Reset output buffer */
	strcpy(user_output, "");

	return count; 
}
 

static ssize_t carbonmonoxide_write(struct file *filp, const char *buf, size_t count, loff_t *f_pos)
{
    // end of buffer reached
    if (*f_pos >= user_input_capacity) {
        return -ENOSPC;
    }
    
    // do not eat more than a bite
    if (count > bite) {
        count = bite;
    }
    
    // do not go over the end
    if (count > user_input_capacity - *f_pos) {
        count = user_input_capacity - *f_pos;
    }
    
    // copy data from user
    if (copy_from_user(user_input + *f_pos, buf, count)) {
        return -EFAULT;
    }
    
    return count;
}

module_init(carbonmonoxide_init);
module_exit(carbonmonoxide_exit);
