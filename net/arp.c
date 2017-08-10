/*	该文件用于处理    arp 相关函数     */

#include "arp.h"




/*发送 arp请求包*/

void arp_request()
{	
	/*构造arp请求包*/
    memcpy(arpbuf.ethhdr.d_mac,host_mac_addr,6);
    memcpy(arpbuf.ethhdr.s_mac,mac_addr,6);
    arpbuf.ethhdr.type = HON(0x0806);	//帧类型 arp协议
    arpbuf.hwtype = HON(1); 			//硬件地址类型 以太网类型
    arpbuf.protocol = HON(0x0800);		//协议地址类型 IP协议
    arpbuf.hwlen = 6;
    arpbuf.protolen = 4;
    arpbuf.opcode = HON(1);
	memcpy(arpbuf.smac,mac_addr,6);
    memcpy(arpbuf.sipaddr,ip_addr,4);
    memcpy(arpbuf.dipaddr,host_ip_addr,4);

    packet_len = 14+28;

    /*2.调用dm9000发送函数，发送请求包*/	
    dm9000_tx((char*)(&arpbuf),packet_len);
}


/*发送 arp应答包*/

void arp_answer(u8 *buf)
{	
   	/*1.构成arp请求包*/
    ARP_HDR *arp_p = (ARP_HDR *)buf;
	
	memcpy(arpbuf.ethhdr.d_mac,arp_p->smac,6);
    memcpy(arpbuf.ethhdr.s_mac,mac_addr,6);
    arpbuf.ethhdr.type = HON(0x0806);
    arpbuf.hwtype = HON(1);
    arpbuf.protocol = HON(0x0800);
    arpbuf.hwlen = 6;
    arpbuf.protolen = 4;     
    arpbuf.opcode = HON(2);
	memcpy(arpbuf.smac,mac_addr,6);
	memcpy(arpbuf.sipaddr,ip_addr,4);
	memcpy(arpbuf.dmac,arp_p->smac,6);
	memcpy(arpbuf.dipaddr,arp_p->sipaddr,4);

	packet_len = 14+28;
	/*2.调用dm9000发送函数，发送应答包*/	
	dm9000_tx((char*)(&arpbuf),packet_len);
}



/* arp包处理函数 */

u8 arp_process(u8 *buf, u32 len)
{

    u32 i; 
	ARP_HDR *arp_p = (ARP_HDR *)buf;
	
    if (packet_len<28)
        return 0;
    switch ( HON(arp_p->opcode) )
    {
		case (1):/*接收到arp请求包*/
		
				/*发送arp响应包*/
				arp_answer(buf);
		break;
		case (2):/*接收到arp响应包*/
		
				/*串口显示 对方 ip mac */
				memcpy(host_ip_addr,arp_p->sipaddr,4);
				printf("host ip is : ");
				for(i=0;i<4;i++)
				printf("%03d ",host_ip_addr[i]);
				printf("\n\r");
			
				memcpy(host_mac_addr,arp_p->smac,6);
				printf("host mac is : ");
				for(i=0;i<6;i++)
					printf("%02x ",host_mac_addr[i]);
				printf("\n\r");
 
		break;		
	}    
}

/* udp包处理函数 */

void udp_process(u8 *buf, u32 len)
{
	UDP_HDR *udphar_p =(UDP_HDR *) buf;
	tftp_process( buf, len, HON(udphar_p->sport) );
}


/* ip包处理函数 */

void ip_process(u8 *buf, u32 len)
{
	IP_HDR *iphar_p =(IP_HDR *) buf;
	switch(iphar_p->proto)
	{
		case PROTO_UDP:
			udp_process(buf, len);
		break;
		
		default:
		break;
	}

}


/*分析报头 确认是什么协议的数据*/

void net_handle(u8 *buf,u32 len)
{
	ETH_HDR *p =(ETH_HDR*)buf;
	switch( HON(p->type) )
	{
		case PROTO_ARP:
					arp_process(buf,len); 
		break;
		case PROTO_IP:
					ip_process(buf,len);
		break;
		default:
		break;
	}
}		
