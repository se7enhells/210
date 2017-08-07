#define GPH2CON  ( *(volatile unsigned long *)0xe0200c40 )
#define GPH3CON  ( *(volatile unsigned long *)0xe0200c60 )
/*
 * K1,K2,K3,K4对应GPH2_0、GPH2_1、GPH2_2、GPH2_3
 * K4,K5,K6,K7对应GPH2_0、GPH3_1、GPH3_2、GPH3_3
 */
 
#define GPH2_0_msk    (0xf<<(0*4))
#define GPH2_1_msk    (0xf<<(1*4))
#define GPH2_2_msk    (0xf<<(2*4))
#define GPH2_3_msk    (0xf<<(3*4))
#define GPH3_0_msk    (0xf<<(0*4))
#define GPH3_1_msk    (0xf<<(1*4))
#define GPH3_2_msk    (0xf<<(2*4))
#define GPH3_3_msk    (0xf<<(3*4))

#define GPH2_0_int    (0xf<<(0*4))
#define GPH2_1_int    (0xf<<(1*4))
#define GPH2_2_int    (0xf<<(2*4))
#define GPH2_3_int    (0xf<<(3*4))
#define GPH3_0_int    (0xf<<(0*4))
#define GPH3_1_int    (0xf<<(1*4))
#define GPH3_2_int    (0xf<<(2*4))
#define GPH3_3_int    (0xf<<(3*4))

void button_init()
{   
    GPH2CON &= ~(GPH2_0_msk | GPH2_1_msk | GPH2_2_msk | GPH2_3_msk);
	GPH2CON |=   GPH2_0_int | GPH2_1_int | GPH2_2_int | GPH2_3_int;
	
	GPH3CON &= ~(GPH3_0_msk | GPH3_1_msk | GPH3_2_msk | GPH3_3_msk);
	GPH3CON |=   GPH3_0_int | GPH3_1_int | GPH3_2_int | GPH3_3_int;
}