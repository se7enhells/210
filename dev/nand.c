#define NFCONF             ( *((volatile unsigned long*)0xB0E00000) )
#define NFCONT             ( *((volatile unsigned long*)0xB0E00004) )
#define NFSTAT             ( *((volatile unsigned char*)0xB0E00028) )
#define NFCMMD             ( *((volatile unsigned char*)0xB0E00008) )
#define NFADDR             ( *((volatile unsigned char*)0xB0E0000C) )
#define NFDATA             ( *((volatile unsigned char*)0xB0E00010) )

#define MP0_1CON 	   (*((volatile unsigned long *)0xE02002E0))
#define MP0_3CON 	   (*((volatile unsigned long *)0xE0200320))

#define MP0_1PUD 	   (*((volatile unsigned long *)0xE02002E8))
#define MP0_3PUD 	   (*((volatile unsigned long *)0xE0200328))

#define page_size   2048 //页字节数
 
 /* 设置时间参数 */
#define TACLS  7
#define TWRPH0 7
#define TWRPH1 7

/* 选中 */
void select_chip(void)
{
    NFCONT &= ~(1<<1);	
}

//取消片选
void deselect_chip(void)
{
    NFCONT |=  (1<<1);	
}

//清除RnB
void clean_RnB()
{
    NFSTAT |= (1<<4);
} 

//发命令
void send_cmd(unsigned char cmd)
{
    NFCMMD = cmd;   
}

//发地址 一次只能发送八位 先低再高
void send_addr(unsigned char addr)
{
	// 两个列地址 3个行地址
	NFADDR = 0x00; 			  
	NFADDR = 0x00; 		
	NFADDR = addr & 0xff;			
	NFADDR = (addr >> 8) & 0xff;
	NFADDR = (addr >> 16) & 0xff;
}

//等待RnB就绪
void wait_RnB(void)
{
    while(!(NFSTAT & (1<<4)));
}
//nand复位

//复位
void nand_reset(void)
{
    /* 选中 */
    select_chip();
    
    /* 清除RnB */
    clean_RnB();
    
    /* 发出复位信号 */
    send_cmd(0xff);
    
    /* 等待就绪 */
    wait_RnB();
    
    /* 取消选中 */
    deselect_chip();
}

//nandflash  初始化
void nand_init(void)
{
    // 配置引脚  
    	MP0_1CON &= ~(0xf<<8);
    	MP0_1CON |= (0x3<<8);
    	MP0_1PUD &= ~(0x3<<4);
    	
    	MP0_3CON &= ~0xFFFFFF;
    	MP0_3CON |= 0x22222222;
    	MP0_3PUD &= ~(0x3fff);
	
	//初始化NFCONF
	NFCONF &= ~( (0b1000<<12) | (0b1000<<8) | (0b1000<<4) );
	NFCONF |=  ( (0b0111<<12) | (0b0111<<8) | (0b0111<<4) );
	//初始化NFCONT
    NFCONT |= (0x1<<1)|(0x1<<0);

	//复位 
	nand_reset();
}


void nand_read_page(unsigned long addr, unsigned char* buff)
{
	int i = 0;
	 /* 选中芯片 */
    select_chip();
    
    /* 清除RnB */
    clean_RnB();
    
    /* 发出读命令0x00 */
    send_cmd(0x00);
    

    /* 发出地址 */
    send_addr(addr);
    
    /* 发出命令0x30 */
     send_cmd(0x30);
        
    /* 等待就绪 */
     wait_RnB();
        
    /* 读数据 */
     for(i = 0; i<page_size;)
    {
    	*buff++ = NFDATA;
    	 i++;
    }
          
     /* 取消片选 */
     deselect_chip();
}



void nand_to_ram(unsigned long start_addr, unsigned char* sdram_addr, int size)
{
	int i;
	unsigned int page_shift = 11;
	
	for(i = (start_addr>>page_shift) ; size > 0;)
	{
		nand_read_page(i , sdram_addr);
		size -= page_size;
		sdram_addr +=  page_size;
		i++;
	}
}

int nand_erase(unsigned long addr)
{
	
	int ret;

	/* 选中 */
    select_chip();
	
	/* 清除RnB */
	clean_RnB();
	
	/*发送写命令 0x60 */
	send_cmd(0x60);
	
	//发送行地址
	send_addr(addr&0xff);
	send_addr((addr>>8)&0xff);
	send_addr((addr>>16)&0xff);
	
	//发送命令D0
	send_cmd(0xD0);

	/*等待就绪*/
	wait_RnB();
	
	/*发送命令 0x70 */
	send_cmd(0x70);

	/*读取写入结果*/
	ret = NFDATA;
	
	 /* 取消片选 */
     deselect_chip();

	 return ret;
}
int nand_write_page(unsigned long addr ,unsigned char* buff)
{
	int ret;
	int i;

	/* 选中 */
    select_chip();
	
	/* 清除RnB */
	clean_RnB();
	
	/*发送写命令 0x80 */
	send_cmd(0x80);
	
	/* 发送地址 */
	send_addr(addr);

	/*写数据*/
	for(i = 0; i<page_size;i++)
    {
    	NFDATA = *buff++ ;
    }
	
	/*发送命令 0x10 */
	send_cmd(0x10);

	/*等待就绪*/
	wait_RnB();
	
	/*发送命令 0x70 */
	send_cmd(0x70);

	/*读取写入结果*/
	ret = NFDATA;
	
	 /* 取消片选 */
     deselect_chip();

	 return ret;
}