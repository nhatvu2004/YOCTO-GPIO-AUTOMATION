#include <linux/module.h>
#include <linux/gpio.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

// Các chân GPIO 
#define GPIO_17 17
#define GPIO_27 27
#define GPIO_22 22

// Các mức điện áp 
#define HIGH 1
#define LOW 0

// Các lệnh IOCTL
#define GPIO_SET_PIN _IOW('G', 0, int)
#define GPIO_CLEAR_PIN _IOW('G', 1, int)



static struct class *gpio_class;
static struct device *gpio_device;


static int gpio_open(struct inode *inode, struct file *file) {
    printk(KERN_INFO "GPIO Driver: Device opened\n");
    return 0;
}

static int gpio_release(struct inode *inode, struct file *file) {
    printk(KERN_INFO "GPIO Driver: Device closed\n");
    return 0;
}

static long gpio_ioctl(struct file *file, unsigned int cmd, unsigned long arg) {
    int gpio_pin;
    int ret;
   	if (copy_from_user(&gpio_pin, (int *)arg, sizeof(int))) {
   		printk(KERN_ERR "Copy from user error\n");
        return -EFAULT;
    }
	switch (cmd) {
        case GPIO_SET_PIN:
            ret = gpio_direction_output(gpio_pin, HIGH);
            if (ret) {
                printk(KERN_ERR "GPIO Driver: Failed to set GPIO%d HIGH\n", gpio_pin);
                return -EINVAL;
            }
            else {
            	printk(KERN_INFO "GPIO Driver: GPIO%d set HIGH\n", gpio_pin);
            }          
            break;
        case GPIO_CLEAR_PIN:
            ret = gpio_direction_output(gpio_pin, LOW);
            if (ret) {
                printk(KERN_ERR "GPIO Driver: Failed to set GPIO%d LOW\n", gpio_pin);
                return -EINVAL;
            }
            else {
            	printk(KERN_INFO "GPIO Driver: GPIO%d set LOW\n", gpio_pin);
            }            
            break;
    return 0;
}

static struct file_operations fops = {
    .open = gpio_open,
    .release = gpio_release,
    .unlocked_ioctl = gpio_ioctl,
};

static int __init gpio_driver_init(void) {
    int ret;

    // Yêu cầu quyền sử dụng GPIO 17, 27, 22
    ret = gpio_request(GPIO_17, "GPIO_17");
    if (ret) {
        printk(KERN_ERR "GPIO Driver: Failed to request GPIO17\n");
        return ret;
    }

    ret = gpio_request(GPIO_27, "GPIO_27");
    if (ret) {
        printk(KERN_ERR "GPIO Driver: Failed to request GPIO27\n");
        gpio_free(GPIO_17);
        return ret;
    }

    ret = gpio_request(GPIO_22, "GPIO_22");
    if (ret) {
        printk(KERN_ERR "GPIO Driver: Failed to request GPIO22\n");
        gpio_free(GPIO_17);
        gpio_free(GPIO_27);
        return ret;
    }

    // Đăng ký character device
    ret = register_chrdev(139, "gpio-driver", &fops);
    if (ret < 0) {
        printk(KERN_ERR "GPIO Driver: Failed to register character device\n");
    	gpio_free(GPIO_17);
    	gpio_free(GPIO_27);
    	gpio_free(GPIO_22);
    	return ret;
    }

    // Tạo class
    gpio_class = class_create(THIS_MODULE, "gpio-class");
    if (IS_ERR(gpio_class)) {
        unregister_chrdev(139, "gpio-driver");
    	gpio_free(GPIO_17);
    	gpio_free(GPIO_27);
    	gpio_free(GPIO_22);
    	printk(KERN_ERR "GPIO Driver: Failed to create class\n");
    	return PTR_ERR(gpio_class);
    }

    // Tạo device
    gpio_device = device_create(gpio_class, NULL, MKDEV(139, 0), NULL, "gpio-driver");
    if (IS_ERR(gpio_device)) {
    	class_destroy(gpio_class);
    	unregister_chrdev(139, "gpio-driver");
    	gpio_free(GPIO_17);
    	gpio_free(GPIO_27);
    	gpio_free(GPIO_22);
    	printk(KERN_ERR "GPIO Driver: Failed to create device\n");
    	return PTR_ERR(gpio_device);
    }

    printk(KERN_INFO "GPIO Driver: Initialized\n");
    return 0;
}

static void __exit gpio_driver_exit(void) {
        device_destroy(gpio_class, MKDEV(139, 0));
	class_destroy(gpio_class);
	unregister_chrdev(139, "gpio-driver");
	gpio_free(GPIO_17);
	gpio_free(GPIO_27);
	gpio_free(GPIO_22);
	printk(KERN_INFO "GPIO Driver: Exited\n");
}

module_init(gpio_driver_init);
module_exit(gpio_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("IOT22");
MODULE_DESCRIPTION("GPIO Driver for Raspberry Pi");

