#define GPJ2CON ( *((volatile unsigned long*)0xE0200280) )
#define GPJ2DAT ( *((volatile unsigned long*)0xE0200284) )


#define GPJ2_0_msk    (0xf<<(0*4))
#define GPJ2_1_msk    (0xf<<(1*4))
#define GPJ2_2_msk    (0xf<<(2*4))
#define GPJ2_3_msk    (0xf<<(3*4))

#define GPJ2_0_int    (0x1<<(0*4))
#define GPJ2_1_int    (0x1<<(1*4))
#define GPJ2_2_int    (0x1<<(2*4))
#define GPJ2_3_int    (0x1<<(3*4))



void led_init()
{   
    GPJ2CON &= ~(GPJ2_0_msk | GPJ2_1_msk | GPJ2_2_msk | GPJ2_3_msk);
	GPJ2CON |=   GPJ2_0_int | GPJ2_1_int | GPJ2_2_int | GPJ2_3_int;
}

void led_off()
{
    GPJ2DAT = 0x0f;
}

void led_on()
{
    GPJ2DAT = 0x00;
}
