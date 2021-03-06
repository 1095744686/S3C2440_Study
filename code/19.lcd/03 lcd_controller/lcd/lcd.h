#ifndef __LCD_H
#define __LCD_H

enum{
	NORMAL = 0,
	INVERT = 1
};
/* 引脚极性结构体
 * NORMAL：正常极性
 * INVERT：反转极性
 */
typedef struct pins_polarity{
	int vclk;  /* normal：下降沿获取数据 */
	int hsync; /* normal：高脉冲 */
	int vsync; /* normal：高脉冲 */

	int rgb;   /* normal：高电平表示1 */

	int de;    /* normal：高电平使能 data enable */
	int pwren; /* normal：高电平使能power enable */
}pins_polarity, *p_pins_polarity;

/* 时序结构体
 * NORMAL：正常极性
 * INVERT：反转极性
 */
typedef struct time_sequence{
	/* 垂直方向 */
	int tvp; /* Vsync脉冲宽度 */
	int tvb; /* 上边黑框 Vertical Back porch */
	int tvf; /* 下边黑框 Vertical Front porch */
	/* 水平方向 */
	int thp; /* Hsync脉冲宽度 */
	int thb; /* 左边黑框 Horizontal Back porch*/
	int thf; /* 右边黑框 Horizontal Front porch */

	int vlck;
}time_sequence,*p_time_sequence;

typedef struct lcd_params{
	char *name;
	/* 引脚极性 */
	pins_polarity pins_pol;
	/* 时序 */
	time_sequence time_seq;
	/* 分辨率，BPP*/
	int xres;
	int yres;
	int bpp;
	/* framebuffer的地址 */
	unsigned int fb_base;
}lcd_params,*p_lcd_params;


int register_lcd(p_lcd_params plcd);
int select_lcd(char *name);

#endif /* __LCD_H */
