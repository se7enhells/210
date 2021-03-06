/*	该文件用于处理  BOOT 相关函数     */
#include "atag.h"
#include "string.h"

#define SDRAM_KERNEL_START ( 0x21000000 )
#define SDRAM_TAGS_START   ( 0x20000100 )
#define SDRAM_ADDR_START   ( 0x20000000 )
#define SDRAM_TOTAL_SIZE   ( 0x04000000 )

void (*theKernel)(int , int , unsigned int );

/*变量定义*/
TAG* pCurTag;
const char *cmdline = "console=ttySAC0,115200 init=/init";


/*核心启动参数设置函数*/

void setup_core_tag(void)
{
    pCurTag = (TAG*)SDRAM_TAGS_START;
	
	pCurTag->hdr.tag = (TAG*)ATAG_CORE;
	pCurTag->hdr.size = tag_size(tag_core); 
     
    pCurTag->u.core.flags = 0;
    pCurTag->u.core.pagesize = 4096;
    pCurTag->u.core.rootdev = 0;
    
    pCurTag = tag_next(pCurTag);
}
    

/*内存参数设置函数*/

void setup_mem_tag(void)
{
	
	 pCurTag->hdr.tag = ATAG_MEM;
     pCurTag->hdr.size = tag_size(tag_mem32); 
     
     pCurTag->u.mem.start = SDRAM_ADDR_START;
     pCurTag->u.mem.size = SDRAM_TOTAL_SIZE;
     
     pCurTag = tag_next(pCurTag);
}

    
/*命令行参数设置函数*/

void setup_cmdline_tag(void)
{
	int linelen = strlen(cmdline);
    
    pCurTag->hdr.tag = ATAG_CMDLINE;
    pCurTag->hdr.size = (sizeof(struct tag_header)+linelen+1+4)>>2;
    
    strcpy(pCurTag->u.cmdline.cmdline,cmdline);
    pCurTag = tag_next(pCurTag);
}

    
/*参数结束标志设置函数*/

void setup_end_tag(void)
{
	pCurTag->hdr.tag = ATAG_NONE;
	pCurTag->hdr.size = 0;
}

void  boot_linux(void)
{
	
	/*1.获取linux启动地址*/
	theKernel = (void (*)(int , int , unsigned int ))SDRAM_KERNEL_START;
 
	/*2.设置启动参数*/
    //2.1 设置核心启动参数
    setup_core_tag();
    
    //2.2 设置内存参数
    setup_mem_tag();
    
    //2.3 设置命令行参数
    setup_cmdline_tag();
    
    //2.4 设置参数结束标志
    setup_end_tag();
	
	/*3.启动linux内核*/
	theKernel(0,3466,SDRAM_TAGS_START); //168
}