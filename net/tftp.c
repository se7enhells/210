/*	该文件用于处理   tftp 相关函数     */

#include "string.h"
#include "arp.h"

#define SDRAM_KERNEL_START ( 0x21000000 )

u8 tftp_sendbuf[1024];   //tftp 发送 缓冲区
u8* tftp_down_addr ; //tftp 接收 缓冲区  这里选择开发板内存位置 用于tftp 下载程序
u16 curblock = 1; //块编码 初始值 


u16 checksum(u8 *ptr, int len)
{
    u32 sum = 0;
    u16 *p = (u16 *)ptr;

    while (len > 1)
    {
        sum += *p++;
        len -= 2;
    }
    
    if(len == 1)
        sum += *(u8 *)p;
    
    while(sum>>16)
        sum = (sum&0xffff) + (sum>>16);
    
    return (u16)((~sum)&0xffff);
}


/* tftp 请求 发送 函数*/

void tftp_send_request(const char* filename)
{
	u8 *tftp_p = &(tftp_sendbuf[200]);
	u32 tftp_len = 0;
	UDP_HDR *udp_p = (UDP_HDR*)(tftp_p-sizeof(UDP_HDR));
	u8 *iphdr_p =  tftp_p-sizeof(UDP_HDR)+ sizeof(ETH_HDR);

	/*tftp帧头*/
	tftp_p[0] = 0x00;	//操作码 1 代表 读请求
	tftp_p[1] = 0x01;	//网络字节序 故 高低位 交换
	tftp_len += 2;
	sprintf(&(tftp_p[tftp_len]) , "%s", filename); // 文件名
	tftp_len += strlen(filename);
	tftp_p[tftp_len] = '\0';
	tftp_len += 1;
	sprintf(&(tftp_p[tftp_len]) , "%s", "octet"); // 模式
	tftp_len += strlen("octet");
	tftp_p[tftp_len] = '\0';
	tftp_len += 1;
	
	/*UDP 帧头*/
	udp_p->sport = HON(0x48915);
	udp_p->dport = HON(0x69);
	udp_p->len = HON(tftp_len+sizeof(UDP_HDR)-sizeof(IP_HDR));
    udp_p->udpchksum = 0x00;

	/*IP 帧头*/
    udp_p->iphdr.vhl = 0x45;
    udp_p->iphdr.tos = 0x00;
    udp_p->iphdr.len = HON(tftp_len+sizeof(UDP_HDR)-sizeof(ETH_HDR));
    udp_p->iphdr.ipid = HON(0x00);
    udp_p->iphdr.ipoffset = HON(0x4000);
    udp_p->iphdr.ttl = 0xff;
    udp_p->iphdr.proto = 17;
	memcpy(udp_p->iphdr.srcipaddr,ip_addr,4);
    memcpy(udp_p->iphdr.destipaddr,host_ip_addr,4);
    udp_p->iphdr.ipchksum = 0;
    udp_p->iphdr.ipchksum = checksum(iphdr_p,20);
	
	/*以太网 帧头*/
	memcpy(udp_p->iphdr.ethhdr.s_mac,mac_addr,6);
    memcpy(udp_p->iphdr.ethhdr.d_mac,host_mac_addr,6);
    udp_p->iphdr.ethhdr.type = HON(PROTO_IP);
	
	/*dm9000发送请求*/
	dm9000_tx((u32 *)udp_p,sizeof(UDP_HDR)+tftp_len);

}


/* tftp 应答 函数*/

void tftp_send_ack(u16 blocknum, u16 serverport)
{
    u8 *tftp_p = &(tftp_sendbuf[200]);
	u32 tftp_len = 0;
	UDP_HDR *udp_p = (UDP_HDR*)(tftp_p-sizeof(UDP_HDR));
	u8 *iphdr_p =  tftp_p-sizeof(UDP_HDR)+ sizeof(ETH_HDR);

	/*tftp帧头*/
	tftp_p[0] = 0x00;	//操作码 4 代表 应答包
	tftp_p[1] = 0x04;	//网络字节序 故 高低位 交换
	tftp_len += 2;
	
	tftp_p[2] = ( blocknum & 0xff00 )>>8;	//操作码 4 代表 应答包
	tftp_p[3] = ( blocknum & 0x00ff );	//网络字节序 故 高低位 交换
	tftp_len += 2;
	
	
	/*UDP 帧头*/
	udp_p->sport = HON(0x48915);
	udp_p->dport = HON(serverport);
	udp_p->len = HON(tftp_len+sizeof(UDP_HDR)-sizeof(IP_HDR));
    udp_p->udpchksum = 0x00;

	/*IP 帧头*/
    udp_p->iphdr.vhl = 0x45;
    udp_p->iphdr.tos = 0x00;
    udp_p->iphdr.len = HON(tftp_len+sizeof(UDP_HDR)-sizeof(ETH_HDR));
    udp_p->iphdr.ipid = HON(0x00);
    udp_p->iphdr.ipoffset = HON(0x4000);
    udp_p->iphdr.ttl = 0xff;
    udp_p->iphdr.proto = 17;
	memcpy(udp_p->iphdr.srcipaddr,ip_addr,4);
    memcpy(udp_p->iphdr.destipaddr,host_ip_addr,4);
    udp_p->iphdr.ipchksum = 0;
    udp_p->iphdr.ipchksum = checksum(iphdr_p,20);
	
	/*以太网 帧头*/
	memcpy(udp_p->iphdr.ethhdr.s_mac,mac_addr,6);
    memcpy(udp_p->iphdr.ethhdr.d_mac,host_mac_addr,6);
    udp_p->iphdr.ethhdr.type = HON(PROTO_IP);
	
	/*dm9000发送请求*/
	dm9000_tx((u32 *)udp_p,sizeof(UDP_HDR)+tftp_len);
}




/* tftp处理函数  只适用于 tftp数据包*/  

void tftp_process(u8 *buf, u32 len, u16 port)
{
	u32 i = 0;
	TFTP_PAK *tftp_p =(TFTP_PAK *) buf;
	u32 tftp_len = len - sizeof(UDP_HDR);
	if( 3 == HON(tftp_p->opcode) )//判断是否数据包
	{
		if(  curblock == HON(tftp_p->blocknum) )
     	{
     	      tftp_down_addr = (u8*)( SDRAM_KERNEL_START + (curblock-1)*512);
			//接受数据
			for( i = 0; i < tftp_len-4; i++)
			{
				*(tftp_down_addr++) = *(tftp_p->data+i);
			}
		
			//发送应答包
			tftp_send_ack(HON(tftp_p->blocknum), port);
			
			curblock++;
			if ((tftp_len-4)<512)
			{
                curblock = 1;
				printf("tftp donwload over!\n\r");
			}
		}
	}
}