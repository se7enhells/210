#define GPA0CON		( *((volatile unsigned long*)0xE0200000) )
#define ULCON0 		( *((volatile unsigned long*)0xE2900000) )
#define UCON0 		( *((volatile unsigned long*)0xE2900004) )
#define UBRDIV0 	( *((volatile unsigned long *)0xE2900028) )
#define UDIVSLOT0 	( *((volatile unsigned long *)0xE290002C) )
#define UTRSTAT0 	( *((volatile unsigned long *)0xE2900010) )
#define UTXH0 		( *((volatile unsigned long *)0xE2900020) )
#define URXH0 		( *((volatile unsigned long *)0xE2900024) )


#define PCLK_PSYS  66000000 // HCLK_PSYS=MPLL/(HCLK_PSYS_RATIO+1);  PCLK_PSYS=HCLK_PSYS/(PCLK_PSYS_RATIO+1); MPLL=667MHZ(参考时钟初始化那个专题)
#define BAUD 115200



#define GPA0_0_msk    (0xf<<(0*4))
#define GPA0_1_msk    (0xf<<(1*4))

#define GPA0_0_int    (0b0010<<(0*4))
#define GPA0_1_int    (0b0010<<(1*4))

void uart_init()
{
	/*设置引脚功能*/
	//GPA0_0 UART_0_RXD 0010
	//GPA0_1 UART_0_TXD 0010
	GPA0CON &= ~(GPA0_0_msk | GPA0_1_msk);
	GPA0CON |=   GPA0_0_int | GPA0_1_int;
	
	//设置数据格式 8位数据位 1位停止位 无奇偶校验
	ULCON0 &= ~0x0f;
	ULCON0 |= ~0b11;
	
	//设置工作模式 轮询模式 0b0101
	UCON0 &= ~(0b1010);
	UCON0 |=   0b0101;
	
	//设置波特率
	UBRDIV0 = (int)(PCLK_PSYS /(BAUD*16)-1);
	UDIVSLOT0 = 0xdddd;  //UDISLOT0=保存该公式计算后的小数部分*16
	
}


void putc(unsigned char ch)
{
	while(!(UTRSTAT0 & (1<<2)));//等待发缓冲区空
	UTXH0 = ch;	
}
unsigned char getc(void)
{
	unsigned char ret;
	while(!(UTRSTAT0 &(1<<0)));//等待收缓冲区空
	ret = URXH0;
	
	//回显功能
	if( (0x0d == ret)||(0x0a == ret) )
	{
		putc(0x0d);
	    putc(0x0a);	
	}
	else
	{
		putc(ret);
	}
	
	return ret;
	
	
}


