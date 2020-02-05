/* �ο� linux-2.6.22.6\drivers\input\keyboard\gpio_keys.c */

#include <linux/module.h>
#include <linux/version.h>

#include <linux/init.h>
#include <linux/fs.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/sched.h>
#include <linux/pm.h>
#include <linux/sysctl.h>
#include <linux/proc_fs.h>
#include <linux/delay.h>
#include <linux/platform_device.h>
#include <linux/input.h>
#include <linux/irq.h>

#include <asm/gpio.h>
#include <asm/io.h>
#include <asm/arch/regs-gpio.h>

struct pin_desc{//���������ṹ��
	int irq;
	char *name;
	unsigned int pin;
	unsigned int mode;//������ʽ
	unsigned int key_val;
};

struct pin_desc pins_desc[4] = {
	{IRQ_EINT0,  "S2", S3C2410_GPF0, IRQT_BOTHEDGE, KEY_L},
	{IRQ_EINT2,  "S3", S3C2410_GPF2, IRQT_BOTHEDGE, KEY_S},
	{IRQ_EINT11, "S4", S3C2410_GPG3, IRQT_BOTHEDGE, KEY_ENTER},
	{IRQ_EINT19, "S5", S3C2410_GPG11,IRQT_BOTHEDGE, KEY_LEFTSHIFT},
};

static struct input_dev *buttons_dev;
static struct pin_desc *irq_pd;//�����ж�ʱ����������
static struct timer_list buttons_timer;

/*
 * �����޸Ķ�ʱ����ʱʱ��
*/
static irqreturn_t buttons_irq(int irq, void *dev_id)
{
	/* 10ms��������ʱ��(��HZ/100) */
	irq_pd = (struct pin_desc *)dev_id;
	mod_timer(&buttons_timer, jiffies+HZ/100);//jiffiesΪϵͳʱ���жϵ��ۼ�ֵ(1HZ = 100)

	return IRQ_RETVAL(IRQ_HANDLED);
}


static void buttons_timer_function(unsigned long data)
{
	struct pin_desc *pindesc = irq_pd;
	unsigned int pinval;
	
	if(!pindesc)//�ް����жϣ�����
		return;
	
	pinval = s3c2410_gpio_getpin(pindesc->pin);
	
	if(pinval){
		/* �ɿ� �ϱ��¼� 0-�ɿ�,1-���� */
		input_event(buttons_dev, EV_KEY, pindesc->key_val, 0);
		input_sync(buttons_dev);//�ϱ�ͬ���¼�
	}
	else{
		/* ���� */
		input_event(buttons_dev, EV_KEY, pindesc->key_val, 1);
		input_sync(buttons_dev);
	}
	
}

static int buttons_init(void)
{
	int i;
	/* 1.���� input_dev�ṹ�� */
	buttons_dev = input_allocate_device();
		
	/* 2.���� */
	/* 2.1 �ܲ��������¼�(������) */
	set_bit(EV_KEY, buttons_dev->evbit);
	set_bit(EV_REP, buttons_dev->evbit);//�ظ��ϱ�(��֧������)
	
	/* 2.2 �ܲ��������¼������Щ�¼� (L,S,ENTER,LEFTSHIFT) */
	set_bit(KEY_L,buttons_dev->keybit);
	set_bit(KEY_S,buttons_dev->keybit);	
	set_bit(KEY_ENTER,buttons_dev->keybit);
	set_bit(KEY_LEFTSHIFT,buttons_dev->keybit);	
	
	/* 3.ע�� */
	input_register_device(buttons_dev);
	
	/* 4.Ӳ����ز��� */
	init_timer(&buttons_timer);
	buttons_timer.function = buttons_timer_function;
	add_timer(&buttons_timer);
	
	for(i = 0; i < 4; i++){//ע���ж�
		request_irq(pins_desc[i].irq, buttons_irq, pins_desc[i].mode, pins_desc[i].name, &pins_desc[i]);
	}
	return 0;
}

static void buttons_exit(void)
{
	int i;
	for(i = 0; i < 4; i++){
		free_irq(pins_desc[i].irq, &pins_desc);
	}
	del_timer(&buttons_timer);
	input_unregister_device(buttons_dev);//ж���豸
	input_free_device(buttons_dev);//�ͷ��豸�ռ�
}


module_init(buttons_init);

module_exit(buttons_exit);

MODULE_LICENSE("GPL");

