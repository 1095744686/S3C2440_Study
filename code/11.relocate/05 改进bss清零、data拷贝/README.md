### 1.问题记录
- 在start.S中的清除bss段代码中把`str r3,[r1]  //清零`，从而造成了把data段数据破坏（被清零）

### 2.原因
- data段位于0x30000000 占用2个字节（两个char类型字符），bss段位于0x30000002，占用8个字节（两个int型变量）
- **这是因为0x30000002并不是向4取整的地址**，由于内存对齐为4字节对齐，会把r3的值存放到到0x30000000（向4字节取整的地址），因此把data段清零


### 3.解决办法
- 修改链接脚本，将bss段地址设置为向4取整，设置`. = ALIGN(4);`
- 则bss段起始位置从原来的0x30000002变化为`0x30000004`

- 同理，data段也需要设置向4对齐
