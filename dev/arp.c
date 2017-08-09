#include "arp.h"

#define HON(n) ((((u16)((n) & 0xff)) << 8) | (((n) & 0xff00) >> 8))

/*发送 arp请求包*/
void arp_request()
{	
	/*构造arp请求包*/
    memcpy(arpbuf_tx.ethhdr.d_mac,host_mac_addr,6);
    memcpy(arpbuf_tx.ethhdr.s_mac,mac_addr,6);
    arpbuf_tx.ethhdr.type = HON(0x0806);	//帧类型 arp协议
    arpbuf_tx.hwtype = HON(1); 			//硬件地址类型 以太网类型
    arpbuf_tx.protocol = HON(0x0800);		//协议地址类型 IP协议
    arpbuf_tx.hwlen = 6;
    arpbuf_tx.protolen = 4;
    arpbuf_tx.opcode = HON(1);
	memcpy(arpbuf_tx.smac,mac_addr,6);
    memcpy(arpbuf_tx.sipaddr,ip_addr,4);
    memcpy(arpbuf_tx.dipaddr,host_ip_addr,4);

    packet_len = 14+28;

    /*2.调用dm9000发送函数，发送应答包*/	
    dm9000_tx(buffer_tx,packet_len);
}

/*解析arp应答包，提取mac地址*/

u8 arp_process(void)
{

    u32 i; 
    if (packet_len<28)
        return 0;
    
    memcpy(host_ip_addr,arpbuf_rx.sipaddr,4);
    printf("host ip is : ");
    for(i=0;i<4;i++)
        printf("%03d ",host_ip_addr[i]);
    printf("\n\r");
    
    memcpy(host_mac_addr,arpbuf_rx.smac,6);
    printf("host mac is : ");
    for(i=0;i<6;i++)
        printf("%02x ",host_mac_addr[i]);
    printf("\n\r");

}