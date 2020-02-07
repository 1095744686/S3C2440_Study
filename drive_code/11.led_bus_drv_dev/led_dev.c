
#include <linux/module.h>
#include <linux/version.h>

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/interrupt.h>
#include <linux/list.h>
#include <linux/timer.h>
#include <linux/init.h>
#include <linux/serial_core.h>
#include <linux/platform_device.h>


/* ����/����/ע��һ��platform_device */

/* ��Դ(�����޸ļĴ���) */
static struct resource s3c2440_led_resource[] = {
    [0] = {
        .start = 0x56000050, //led GPFCON��ʼ��ַ
        .end   = 0x56000050 + 8 -1,//led GPFDAT��ʼ��ַ
        .flags = IORESOURCE_MEM,//��ʾ��ַ��Դ(ò�������׼������Ҫ��ֻ����������)
    },
    [1] = {
        .start = 4,//��һλ��ʼ(����) GPF4-GPF6
        .end   = 6,
        .flags = IORESOURCE_IRQ,//��ʾ�ж���Դ
    }

};

static void led_release(struct device *dev)
{
	printk("led_release\n");
}

static struct platform_device led_dev = {
    .name          = "s3c2440-led",//��Ӧ��platform_driver����������
    .id            = -1,//��ʾֻ��һ���豸
    .num_resources = ARRAY_SIZE(s3c2440_led_resource),//��Դ����,ARRAY_SIZE()����:��ȡ����
    .resource      = s3c2440_led_resource,//��Դ����led_resource
    .dev = {
			.release = led_release,//�ͷź������������ں��ṩһ��release����,����ж��ʱ,�ں��Ҳ����ú����ᱨ��
    }
};

static int led_dev_init(void)
{
	platform_device_register(&led_dev);
	return 0;
}

static void led_dev_exit(void)
{
	platform_device_unregister(&led_dev);
}

module_init(led_dev_init);
module_exit(led_dev_exit);
MODULE_LICENSE("GPL");

