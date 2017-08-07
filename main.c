#define CON	( *(volatile unsigned long*)0xE0200280 )
#define DAT	( *(volatile unsigned long*)0xE0200284 )

int gboot_main()
{
    CON = 0x1;
    DAT = 0x0;
    
    return 0;    
}


