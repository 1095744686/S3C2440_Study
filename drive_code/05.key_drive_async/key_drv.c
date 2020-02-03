#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/irq.h>
#include <linux/poll.h>

#include <asm/uaccess.h>
#include <asm/irq.h>
#include <asm/io.h>
#include <asm/arch/regs-gpio.h>
#include <asm/hardware.h>

static struct class *keydrv_class;
static struct class_device *keydrv_class_dev;

volatile unsigned long *gpfcon;
volatile unsigned long *gpfdat;

volatile unsigned long *gpgcon;
volatile unsigned long *gpgdat;

/* �ȴ�����: 
 * ��û�а���������ʱ������н��̵���s3c24xx_buttons_read������
 * ��������
 */
static DECLARE_WAIT_QUEUE_HEAD(button_waitq);

/* �ж��¼���־, �жϷ����������1��s3c24xx_buttons_read������0 */
static volatile int ev_press = 0;

/* ��ֵ: ����ʱ, 0x01, 0x02, 0x03, 0x04 */
/* ��ֵ: �ɿ�ʱ, 0x81, 0x82, 0x83, 0x84 */
static unsigned char key_val;

struct pin_desc{
	unsigned int pin;
	unsigned int key_val;
};

struct pin_desc pins_desc[4] = {
	{S3C2410_GPF0, 0x01},
	{S3C2410_GPF2, 0x02},
	{S3C2410_GPG3, 0x03},
	{S3C2410_GPG11, 0x04},
};

static struct fasync_struct *button_async;
/* 
 * ȷ����ֵ
 * ����4�����������¼�ֵΪ: 0x01,0x02,0x03,0x04
 * �ɿ���ֵΪ: 0x81,0x82,0x83,0x84
*/
static irqreturn_t buttons_irq(int irq, void *dev_id)
{
	struct pin_desc *pindesc = (struct pin_desc *)dev_id;
	unsigned int pinval;
	
	pinval = s3c2410_gpio_getpin(pindesc->pin);

	if(pinval){
		/* �ɿ� */
		key_val = 0x80 | pindesc->key_val;
	}
	else{
		/* ���� */
		key_val = pindesc->key_val;
	}

	ev_press = 1; //��ʾ�жϷ�����
  /* �������ߵĽ��� */
  wake_up_interruptible(&button_waitq);

  kill_fasync(&button_async, SIGIO, POLL_IN);
  
	return IRQ_HANDLED;
}


static int s3c24xx_keys_open(struct inode *inode, struct file *file)
{
	/* ����GPF0,2 GPG3,11Ϊ �ⲿ�ж�ģʽ */
	request_irq(IRQ_EINT0,  buttons_irq, IRQT_BOTHEDGE, "S2", &pins_desc[0]);  
	request_irq(IRQ_EINT2,  buttons_irq, IRQT_BOTHEDGE, "S3", &pins_desc[1]);
	request_irq(IRQ_EINT11, buttons_irq, IRQT_BOTHEDGE, "S4", &pins_desc[2]);
	request_irq(IRQ_EINT19, buttons_irq, IRQT_BOTHEDGE, "S5", &pins_desc[3]);
	return 0;
}
static ssize_t s3c24xx_keys_read (struct file *file, char __user *buf, size_t size, loff_t *ppos)
{
	if(size != 1)
		return -EINVAL;
  /* ���ev_press����0������ */
  wait_event_interruptible(button_waitq, ev_press);

	/* ����а������������ؼ�ֵ */
	copy_to_user(buf, &key_val, 1);
	ev_press = 0;//���������Ѿ�������0��������
	
	return 1; 
}

static int s3c24xx_keys_close(struct inode *inode, struct file *file)
{
	/* �ͷ��жϣ��Ǹ����жϺţ��Լ��豸ID */
	free_irq(IRQ_EINT0,  &pins_desc[0]);
	free_irq(IRQ_EINT2,  &pins_desc[1]);
	free_irq(IRQ_EINT11, &pins_desc[2]);	
	free_irq(IRQ_EINT19, &pins_desc[3]);
				
	return 0;
}

/* �ȴ�һ��ʱ��ֱ�ӷ��� */
static unsigned int s3c24xx_keys_poll(struct file *file, struct poll_table_struct *wait)
{
	unsigned int mask = 0;
	poll_wait(file, &button_waitq, wait);
	if(ev_press){
		mask |= POLLIN;//��ͨ�����ȼ������ݿɶ� ��־
	}
	return mask;
}

static int s3c24xx_keys_fasync (int fd, struct file *filp, int on)
{
	printk("drive: s3c24xx_keys_fasync\n");
	return fasync_helper (fd, filp, on, &button_async);
}

/* ����ṹ���ַ��豸��������ĺ���
 * ��Ӧ�ó�������豸�ļ�ʱ�����õ�open��read��write�Ⱥ�����
 * ���ջ��������ṹ��ָ���Ķ�Ӧ����
 */
static struct file_operations s3c24xx_keys_fops = {
  .owner   = THIS_MODULE,    /* ����һ���꣬�������ģ��ʱ�Զ�������__this_module���� */
  .open    = s3c24xx_keys_open,      
	.read    = s3c24xx_keys_read,	 
	.release = s3c24xx_keys_close,
	.poll    = s3c24xx_keys_poll,
	.fasync  = s3c24xx_keys_fasync,
};

static int major;
static int key_drv_init(void)
{
	major = register_chrdev(0, "key_drv", &s3c24xx_keys_fops);//ע��
	
	/* ���ں��ṩ �豸��Ϣ�������Զ������豸�ڵ� */
	keydrv_class = class_create(THIS_MODULE, "key_drv");
	keydrv_class_dev = class_device_create(keydrv_class, NULL, MKDEV(major, 0), NULL, "buttons");

	gpfcon = (volatile unsigned long *)ioremap(0x56000050, 16);//ӳ�������ַ
	gpfdat = gpfcon + 1;

	gpgcon = (volatile unsigned long *)ioremap(0x56000060, 16);
	gpgdat = gpgcon + 1;
	return 0;
}

static void key_drv_exit(void)
{
	unregister_chrdev(major, "key_drv");//ж��
	
	class_device_unregister(keydrv_class_dev);//ע����Ϣɾ��
	class_destroy(keydrv_class);

	iounmap(gpfcon);//���ӳ��
	iounmap(gpgcon);
}


module_init(key_drv_init);
module_exit(key_drv_exit);

MODULE_LICENSE("GPL");


