/*interrupt registes*/
#define EXT_INT_2_CON       ( *((volatile unsigned int *)0xE0200E08) )
#define EXT_INT_3_CON       ( *((volatile unsigned int *)0xE0200E0C) )

#define EXT_INT_2_MASK      ( *((volatile unsigned int *)0xE0200F08) ) 
#define EXT_INT_3_MASK      ( *((volatile unsigned int *)0xE0200F0C) )  

#define VIC0INTENABLE       ( *((volatile unsigned int *)0xF2000010) ) 
#define key_VICADDR         ( *((volatile unsigned int *)0xF2000140) )    
#define VIC0ADDRESS         ( *((volatile unsigned int *)0xF2000F00) )  

#define EXT_INT_2_PEND      ( *((volatile unsigned int *)0xE0200F48) )  
#define EXT_INT_3_PEND      ( *((volatile unsigned int *)0xE0200F4C) ) 
void key_isr()
{
    volatile unsigned int key_code_1,key_code_2;
    
    __asm__( 
    
    "sub lr, lr, #4\n"  
    "stmfd sp!, {r0-r12, lr}\n"       
    : 
    : 
   );

 
    key_code_1 = EXT_INT_2_PEND & 0xff; 
	key_code_2 = EXT_INT_3_PEND & 0xff; 
    switch(key_code_1) 
    {
        case (0b1<<(0*1)): //K1
            led_on();
            break;
		
        case (0b1<<(1*1)): //K2
            led_off();
            break;
		case (0b1<<(2*1)): //K1
            led_on();
            break;
		
        case (0b1<<(4*1)): //K2
            led_off();
            break;
			
        default:
            break;
    }
	switch(key_code_2) 
    {
        case (0b1<<(0*1)): //K1
            led_on();
            break;
		
        case (0b1<<(1*1)): //K2
            led_off();
            break;
		case (0b1<<(2*1)): //K1
            led_on();
            break;
		
        case (0b1<<(3*1)): //K2
            led_off();
            break;
			
        default:
            break;
    }
    /* 清除中断 */    
    EXT_INT_2_PEND &=  ~key_code_1 & 0xff; 	
	EXT_INT_3_PEND &=  ~key_code_2 & 0xff; 
    VIC0ADDRESS = 0;
   
    __asm__( 
    "ldmfd sp!, {r0-r12, pc}^ \n"       
    : 
    : 
  );
}



void init_irq()
{
	 /*下降沿触发中断*/
    EXT_INT_2_CON &= ~( (0b101<<(0*4)) | (0b101<<(1*4)) | (0b101<<(2*4)) | (0b101<<(3*4)) );
	EXT_INT_2_CON |=  ( (0b010<<(0*4)) | (0b010<<(1*4)) | (0b010<<(2*4)) | (0b010<<(3*4)) ); 
	EXT_INT_3_CON &= ~( (0b101<<(0*4)) | (0b101<<(1*4)) | (0b101<<(2*4)) | (0b101<<(3*4)) );
	EXT_INT_3_CON |=  ( (0b010<<(0*4)) | (0b010<<(1*4)) | (0b010<<(2*4)) | (0b010<<(3*4)) ); 
    
	/* 取消屏蔽外部中断*/  
	
	EXT_INT_2_MASK = 0;                   
	EXT_INT_3_MASK = 0;
	
	/* 使能指定外部中断*/   
    VIC0INTENABLE |= (0b1<<(16*1));              
   
   /*指定中断处理函数地址*/
	key_VICADDR = (int)key_isr;          
    
    __asm__( 
    /*开中断*/  
    "mrs r0,cpsr\n"
    "bic r0, r0, #0x80\n"
    "msr cpsr_c, r0\n"            
    : 
    : 
  );
}
