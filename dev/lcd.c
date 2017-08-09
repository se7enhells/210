#define GPF0CON  ( *(volatile unsigned long *)0xe0200120 )
#define GPF1CON  ( *(volatile unsigned long *)0xe0200140 )
#define GPF2CON  ( *(volatile unsigned long *)0xe0200160 )
#define GPF3CON  ( *(volatile unsigned long *)0xe0200180 )

/*  Òý½ÅÐÅÏ¢   VD0-VD23 GPF0_0---GPF3_2 0010*/   
void lcd_port_init(void)
{
	GPF0CON = 0x22222222;
	GPF1CON = 0x22222222;
	GPF2CON = 0x22222222;
	GPF3CON &= ~( (0xd<<(0*4)) | (0xd<<(1*4)) | (0xd<<(2*4)) );
	GPF3CON |=  ( (0x2<<(0*4)) | (0x2<<(1*4)) | (0x2<<(2*4)) );
}



void lcd_contrl_init()
{
	
}