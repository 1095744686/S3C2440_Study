
#include "s3c2440_soc.h"
#include "uart.h"
#include "led.h"
#include "init.h"
#include "printf.h"
char g_char = 'A'; //定义全局变量

/*
 * NAND FLASH测试
*/
int main(void)
{
	printf("test..\n\r");

	//lcd_test();
	i2c_test();
	return 0;
}


