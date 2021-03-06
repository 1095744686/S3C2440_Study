#ifndef __LCD_CONTROLLER_H
#define __LCD_CONTROLLER_H

#include "lcd.h"

/* 抽象LCD控制器结构体
 * 向上：接收不同的LCD参数
 * 向下：使用这些参数设置对应的LCD Controller
*/
typedef struct lcd_controller{
	char *name;
	void (*init)(p_lcd_params plcdparams); //初始化
	void (*enable)(void);  //使
	void (*disable)(void); //禁止 
	void (*init_palette)(void);

}lcd_controller,*p_lcd_controller;



int lcd_controller_init(p_lcd_params plcdparams);

int select_lcd_controller(char *name);

int lcd_controller_enable(void);

int lcd_controller_disable(void);


#endif /* __LCD_CONTROLLER_H */


