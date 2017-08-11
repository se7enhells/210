#include "dm9000.h"
#include "arp.h"

#define SROM_BW       			( *((volatile unsigned int *)0x00000000) )
#define SROM_BC0     			( *((volatile unsigned int *)0xE8000004) )
#define GPH0CON     			( *((volatile unsigned int *)0xE0200C00) )
#define EXT_INT_0_CON     		( *((volatile unsigned int *)0xE0200E00) )
#define EXT_INT_0_MASK     		( *((volatile unsigned int *)0xE0200F00) )
#define VIC0INTENABLE     		( *((volatile unsigned int *)0xF2000010) )
#define VIC0VECTADDR7     		( *((volatile unsigned int *)0xF200011C) )
#define EXT_INT_0_PEND     		( *((volatile unsigned int *)0xE0200F40) )
#define VIC0ADDRESS     		( *((volatile unsigned int *)0xF2000F00) )

#define DM_ADD (*((volatile unsigned short *)0x20000300))
#define DM_DAT (*((volatile unsigned short *)0x20000304))


/*��������*/
void dm9000_isr(void);

/*��������*/
u8 buffer[1500];// = (char*)(&arpbuf);    //DM9000 ���� ���� ������

u8 mac_addr[6] = {9,8,7,6,5,4};
u8 ip_addr[4] = {192,168,152,100};
u8 host_mac_addr[6] = {0xff,0xff,0xff,0xff,0xff,0xff};
u8 host_ip_addr[4] = {192,168,152,1};
u16 packet_len;


/*ѡ��DM9000*/
void cs_init()
{
	SROM_BW &= ~(0b111<<1);
	SROM_BW |= (0b1<<0);
	SROM_BC0 = (0<<28)|(0<<24)|(0<<21)|(0b01110<<16)|(1<<12)|(0<<8)|(2<<4)|(0<<2)|(0<<0);
}

/*�жϳ�ʼ��*/
void int_init(void)
{
	GPH0CON |=  ( 0xf<<(7*4) ); //��������Ϊ�ж�ģʽ 
	EXT_INT_0_CON &=~(0xf<<(7*4));//�ж�ģʽ����Ϊ�ߵ�ƽ
    EXT_INT_0_CON |=(0x1<<(7*4));
	EXT_INT_0_MASK &= ~(1<<7);		//�ж����ο�����
	VIC0INTENABLE |= (0b1<<(7*1));   // �ж��������ʹ��    
	
	VIC0VECTADDR7 = (int)dm9000_isr; 	//ָ���жϴ�������ַ
	
	__asm__( 
    /*���ж�*/  
    "mrs r0,cpsr\n"
    "bic r0, r0, #0x80\n"
    "msr cpsr_c, r0\n"            
    : 
    : 
  );
  
    /* ����ж� */    
    EXT_INT_0_PEND &=  ~(1<<7); 	
    VIC0ADDRESS = 0;
	
}
/*д����*/
void dm9000_reg_write(u16 reg,u16 data)
{
	DM_ADD = reg;
	DM_DAT = data;	
}

/*������*/
u8 dm9000_reg_read(u16 reg)
{
	DM_ADD = reg;
	return DM_DAT;	
}

/*��λ�豸*/
void dm9000_reset(void)
{
	dm9000_reg_write(DM9000_GPCR, GPCR_GPIO0_OUT);
    dm9000_reg_write(DM9000_GPR, 0);	
    
    dm9000_reg_write(DM9000_NCR, (NCR_LBK_INT_MAC | NCR_RST));
    dm9000_reg_write(DM9000_NCR, 0);
    
    dm9000_reg_write(DM9000_NCR, (NCR_LBK_INT_MAC | NCR_RST));
    dm9000_reg_write(DM9000_NCR, 0);
}

/*����dm9000*/
void dm9000_probe(void)
{
	u32 id_val;
	id_val = dm9000_reg_read(DM9000_VIDL);
	id_val |= dm9000_reg_read(DM9000_VIDH) << 8;
	id_val |= dm9000_reg_read(DM9000_PIDL) << 16;
	id_val |= dm9000_reg_read(DM9000_PIDH) << 24;
	if (id_val == DM9000_ID) 
	{
		printf("dm9000 is found !\n");
		return ;
	}
	else 
	{
		printf("dm9000 is not found !\n");
		return ;
	}
	
}	

/*MAC��ʼ��*/	/*����MAC��ַ*/
void MAC_init(void)
{
	u32 i ;
	/*MAC��ʼ��*/
	/* Program operating register, only internal phy supported */
	dm9000_reg_write(DM9000_NCR, 0x0);
	/* TX Polling clear */
	dm9000_reg_write(DM9000_TCR, 0);
	/* Less 3Kb, 200us */
	dm9000_reg_write(DM9000_BPTR, BPTR_BPHW(3) | BPTR_JPT_600US);
	/* Flow Control : High/Low Water */
	dm9000_reg_write(DM9000_FCTR, FCTR_HWOT(3) | FCTR_LWOT(8));
	/* SH FIXME: This looks strange! Flow Control */
	dm9000_reg_write(DM9000_FCR, 0x0);
	/* Special Mode */
	dm9000_reg_write(DM9000_SMCR, 0);
	/* clear TX status */
	dm9000_reg_write(DM9000_NSR, NSR_WAKEST | NSR_TX2END | NSR_TX1END);
	/* Clear interrupt status */
	dm9000_reg_write(DM9000_ISR, ISR_ROOS | ISR_ROS | ISR_PTS | ISR_PRS);
	
	/*����MAC��ַ*/
	for (i = 0; i < 6; i++)
        dm9000_reg_write(DM9000_PAR+i, mac_addr[i]);
    
}

/*����DM9000*/
void dm9000_enable(void)
{
	/*����DM9000*/
   	dm9000_reg_write(DM9000_RCR, RCR_DIS_LONG | RCR_DIS_CRC | RCR_RXEN|RCR_ALL);
	/* Enable TX/RX interrupt mask */
	dm9000_reg_write(DM9000_IMR, IMR_PAR);
}


/*dm9000 ��ʼ��*/
int dm9000_init()
{
	
	/*ѡ��DM9000*/
	cs_init();
	
	/*�жϳ�ʼ��*/
	int_init();
	
	/*��λ�豸*/
	 dm9000_reset();
	 
	/*����dm9000*/
    dm9000_probe();
	
	/*MAC��ʼ��*/ /*����MAC��ַ*/
	MAC_init();
	
	/*����DM9000*/
	dm9000_enable();
}

void dm9000_tx(u8 *data,u32 length)
{
	u32 i;
	/*��ֹ�ж�*/
	dm9000_reg_write(DM9000_IMR,0x80);

	/*д�뷢�����ݳ���*/
	dm9000_reg_write(DM9000_TXPLL, length & 0xff);
    dm9000_reg_write(DM9000_TXPLH, (length >> 8) & 0xff);
    
	/*д������͵�����*/
	DM_ADD = DM9000_MWCMD;
    for(i=0;i<length;i+=2)
    {
    	DM_DAT = data[i] | (data[i+1]<<8);
    }
    
	/*��������*/
	 dm9000_reg_write(DM9000_TCR, TCR_TXREQ); 
    
	/*�ȴ����ͽ���*/
    while(1)
    {
       u8 status;
       status = dm9000_reg_read(DM9000_TCR);
       if((status&0x01)==0x00)
           break;	
    }
	
	/*�������״̬*/
    dm9000_reg_write(DM9000_NSR,0x2c);

	/*�ָ��ж�ʹ��*/
	dm9000_reg_write(DM9000_IMR,0x81);

}

#define PTK_MAX_LEN 1522

u32 dm9000_rx(u8 *data)
{
    u16 status,len;
    u16 tmp;
    u32 i;
    u8 ready = 0;
	
    /*�ж��Ƿ�����жϣ������*/
    if(dm9000_reg_read(DM9000_ISR) & 0x01)
        dm9000_reg_write(DM9000_ISR,0x01);
    else
        return 0;
        
    /*�ն�   ����Ƿ��յ���*/
    ready = dm9000_reg_read(DM9000_MRCMDX);
	
	if ((ready & 0x01) != 0x01)
    {
    	ready = dm9000_reg_read(DM9000_MRCMDX);
    	if ((ready & 0x01) != 0x01)
		{
    	    return 0;
		}
    }
    
    /*��ȡ״̬*/
    status = dm9000_reg_read(DM9000_MRCMD);
    
    /*��ȡ������*/
    len = DM_DAT;
    
    /*��ȡ������*/
    if(len<PTK_MAX_LEN)
    {
       for(i=0;i<len;i+=2)
       {
           tmp = DM_DAT;
           data[i] = tmp & 0x0ff;
           data[i+1] = (tmp>>8)&0x0ff;
       }
    }
}

void dm9000_isr()
{
    packet_len = dm9000_rx(&buffer[0]);	 //��ȡ��������
	
	net_handle();						//���紦��
    
	
   /* ����ж� */    
    EXT_INT_0_PEND &=  ~(1<<7); 	
    VIC0ADDRESS = 0;
}


void dm9000_arp()
{
    while(1)
        arp_request();	
}