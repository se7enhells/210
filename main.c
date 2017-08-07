int gboot_main()
{
	unsigned char buf[2048];
	int num;
	
#ifdef MMU_ON
    mmu_init();
#endif
    led_init();
    button_init();
    init_irq();
 
    uart_init();
	///*-----nand读写 测试 -----*///
	// led_off();
	// buf[0] = 6;
	// nand_erase(64*5+1);
	// nand_write_page(64*5+1,buf);
	// buf[0] = 9;
	// nand_read_page(64*5+1,buf);
	
	// if( 6 == buf[0] )
	// {
		 // led_on();
	// }
	///*-----nand  读写测试 到此 -----*///
	
	///*-----串口 测试-----*///
	
	// putc("0x0d");
	// putc("0x0a");
	// putc("L");

	///*-----串口 测试 到此 -----*///
	//
    while(1)
	{
	///*-----控制台程序 -----*///
	    printf("\n***************************************\n\r");
    	printf("\n*****************GBOOT*****************\n\r");
    	printf("1:  Download Linux Kernel from TFTP Server!\n\r");
    	printf("2:  Boot Linux from RAM!\n\r");
    	printf("3:  Boor Linux from Nand Flash!\n\r");
    	printf("\n Plese Select:");	
		scanf("%d",&num);
	
		switch(num)
		{
			case 1:
				// tftp_load();
			break;
				
			case 2:
				// boot_linux_ram();
			break;
			
			case 3:
				// boot_linux_nand();
			break;
			default:
				printf("Error: wrong selection!\n\r");
			break;
		}	
	///*-----控制台程序 到此 -----*///

	}
    
    return 0;    
}

