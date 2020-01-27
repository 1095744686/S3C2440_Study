#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <asm/uaccess.h>
#include <asm/irq.h>
#include <asm/io.h>
#include <asm/arch/regs-gpio.h>
#include <asm/hardware.h>

static struct class *leddrv_class;
static struct class_device	*leddrv_class_dev;

volatile unsigned int  *gpfcon = NULL;
volatile unsigned int  *gpfdat = NULL;

static int s3c24xx_leds_open(struct inode *inode, struct file *file)
{
	//����GPF4��5��6Ϊ���ģʽ
	*gpfcon &= ~((0x3)<<(4*2) | (0x3)<<(5*2) | (0x3)<<(6*2));
	*gpfcon |=  ((0x1)<<(4*2) | (0x1)<<(5*2) | (0x1)<<(6*2));
	return 0;
}

static ssize_t s3c24xx_leds_write(struct file *file, const char __user *buf, size_t count, loff_t * ppos)
{
	int val;
	copy_from_user(&val, buf, count);//���û��ռ��ֵ��ȡ����val
	if(1 == val){//���
		*gpfdat &= ~(7<<4);
	}
	else{//���
		*gpfdat |= (7<<4);
	}
	return 0;
}

/* ����ṹ���ַ��豸��������ĺ���
 * ��Ӧ�ó�������豸�ļ�ʱ�����õ�open��read��write�Ⱥ�����
 * ���ջ��������ṹ��ָ���Ķ�Ӧ����
 */
static struct file_operations s3c24xx_leds_fops = {
  .owner = THIS_MODULE,    /* ����һ���꣬�������ģ��ʱ�Զ�������__this_module���� */
  .open  = s3c24xx_leds_open,      
	.write = s3c24xx_leds_write,	   
};

int major;
int led_drv_init(void)
{
	major = register_chrdev(0,"led_drv",&s3c24xx_leds_fops);//ע���ַ��������򣬸����ں�
	
	leddrv_class = class_create(THIS_MODULE, "leddrv");//�����豸��Ϣ���Զ������豸�ڵ�

	leddrv_class_dev = class_device_create(leddrv_class, NULL, MKDEV(major, 0), NULL, "led"); /* /dev/led */

	gpfcon = (volatile unsigned long *)ioremap(0x56000050, 16);
	gpfdat = gpfcon + 1;
	return 0;
}

void led_drv_exit(void)
{
	unregister_chrdev(major,"led_drv");//ж������
	
	class_device_unregister(leddrv_class_dev);
	class_destroy(leddrv_class);
	iounmap(gpfcon);
}


module_init(led_drv_init);
module_exit(led_drv_exit);

MODULE_LICENSE("GPL");