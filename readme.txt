软件：本工程是基于FreeRTOS的stm32源代码
硬件：神州IV号stm32开发板

/*************************************日志记录*********************/
2016.06.26
1、添加MODBUS协议，支持01、02、03、04、05、06和16功能码
2、使用USART1进行通信
3、协议栈在stm32_freertos\src\protocol文件夹下面

2016.05.04
1、添加神舟IV号开发板的USART模块，添加usart.c 、 usart.h两个文件
2、如果使用USARTx，那么就将USE_USARTx宏定义设置为1，否则设置为0
3、CONSOLE_SET宏定义是为了设置打印信息函数printf()是从哪个串口打印
4、需要注意的是，usart2使用了端口重映射功能，是和神舟IV号开发板匹配，如果你自己的板子没有 使用端口重映射，需要将
GPIO_PinRemapConfig(GPIO_Remap_USART2, ENABLE);注释掉


2016.04.10
1、添加神舟IV号开发板的LED模块，添加led.c 、led.h 、task_led_toggle.c、task_led_toggle.h文件
2、四个LED灯1S闪烁一次，测试通过
3、想要使能哪一个LED，需要在led.h中把USE_LEDx(x = 1\2\3\4)设置为1，不使能的话设置为0

2016.04.06
1、新建工程