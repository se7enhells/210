int gboot_main()
{
	
	unsigned char buf[2048];
#ifdef MMU_ON
    mmu_init();
#endif
    led_init();
    button_init();
    init_irq();
   
	
	//nand  读写测试
	led_off();
	buf[0] = 6;
	nand_erase(64*5+1);
	nand_write_page(64*5+1,buf);
	buf[0] = 9;
	nand_read_page(64*5+1,buf);
	
	if( 6 == buf[0] )
	{
		 led_on();
	}
	//nand  读写测试 到此
	
    while(1);
    
    return 0;    
}

