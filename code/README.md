# 1.程序文件夹说明

| 序号 | 文件夹名 | 注解 | 内容 | 
| :---: | :--- | :--- | :--- |
| 01 | led_on_by_asm | 点亮LED汇编程序 | 学习基础汇编指令ldr、str | 
| 02 | led_on_by_c | 点亮LED C程序| 分析反汇编程序，局部变量如何保存在栈中、程序与数据存储分布 |
| 03 | led_params | 参数判断 | 分析如何函数间传递参数 | 
| 04 | led_loop | 流水灯 | 关闭看门狗、自动分辨nand/nor启动、流水灯程序 | 
| 05 | led_key | 按键控灯 | 对应按键控制对应LED | 
| 06 | clock | 芯片时钟 | 利用PLL提高系统时钟频率 |
| 07 | uart | 串口 | 串口初始化输入输出 | 
| 08 | printf | printf函数 | 构建简易printf函数,学习可变参数和变参函数 | 
| 09 | nor_access_cycle | nor读取周期 | 简单分析nor读取时序,控制读取时间,改变读取效率 |
| 10 | sdram | sdram读写 | 初始化并读写sdram | 
| 11 | relocate | 重定位代码 | 内存中重定位概念以及链接脚本 | 
| 12 | thumb | thumb指令集 | thumb指令集编译、使得代码以thunb状态运行 | 
| 13 | und_exception | 未定义指令异常 | 分析未定义指令异常的处理流程及代码实现 | 
| 14 | swi_exception | 软中断异常 | 软中断实现过程 | 
| 15 | eint | 外部中断 | 外部中断的初始化及处理过程 | 
| 16 | timer | 定时器中断 | 定时器中断的初始化及处理过程 | 
| 17 | nor_flash | nor操作 | nor识别、读写、擦除 | 
| 18 | nand_flash | nand操作 | nor识别、读写、擦除、改进代码重定位 | 
| 19 | lcd | lcd操作 | lcd控制器框架、lcd时序 | 
| 20 | i2c | i2c操作 | i2c控制器框架、i2c中断处理 | 