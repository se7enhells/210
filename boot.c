/*	���ļ����ڴ���  BOOT ��غ���     */
#include "atag.h"
#include "string.h"

#define SDRAM_KERNEL_START ( 0x21000000 )
#define SDRAM_TAGS_START   ( 0x20000100 )
#define SDRAM_ADDR_START   ( 0x20000000 )
#define SDRAM_TOTAL_SIZE   ( 0x04000000 )

void (*theKernel)(int , int , unsigned int );

/*��������*/
TAG* pCurTag;
const char *cmdline = "console=ttySAC0,115200 init=/init";


/*���������������ú���*/

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
    

/*�ڴ�������ú���*/

void setup_mem_tag(void)
{
	
	 pCurTag->hdr.tag = ATAG_MEM;
     pCurTag->hdr.size = tag_size(tag_mem32); 
     
     pCurTag->u.mem.start = SDRAM_ADDR_START;
     pCurTag->u.mem.size = SDRAM_TOTAL_SIZE;
     
     pCurTag = tag_next(pCurTag);
}

    
/*�����в������ú���*/

void setup_cmdline_tag(void)
{
	int linelen = strlen(cmdline);
    
    pCurTag->hdr.tag = ATAG_CMDLINE;
    pCurTag->hdr.size = (sizeof(struct tag_header)+linelen+1+4)>>2;
    
    strcpy(pCurTag->u.cmdline.cmdline,cmdline);
    pCurTag = tag_next(pCurTag);
}

    
/*����������־���ú���*/

void setup_end_tag(void)
{
	pCurTag->hdr.tag = ATAG_NONE;
	pCurTag->hdr.size = 0;
}

void  boot_linux(void)
{
	
	/*1.��ȡlinux������ַ*/
	theKernel = (void (*)(int , int , unsigned int ))SDRAM_KERNEL_START;
 
	/*2.������������*/
    //2.1 ���ú�����������
    setup_core_tag();
    
    //2.2 �����ڴ����
    setup_mem_tag();
    
    //2.3 ���������в���
    setup_cmdline_tag();
    
    //2.4 ���ò���������־
    setup_end_tag();
	
	/*3.����linux�ں�*/
	theKernel(0,3466,SDRAM_TAGS_START); //168
}