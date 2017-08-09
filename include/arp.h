typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;


typedef struct eth_hdr
{
    u8 d_mac[6];  //以太网目的地址
    u8 s_mac[6];  //以太网源地址
    u16 type;     //帧类型 
}ETH_HDR;

typedef struct arp_hdr
{
	ETH_HDR ethhdr; //以太网头
    u16 hwtype;     //硬件类型
    u16 protocol;	//	协议类型
    u8 hwlen;		//硬件地址长度
    u8 protolen;	//	协议地址长度
    u16 opcode;		//ARP请求包还是应答包 1 请求 2 应答
    u8 smac[6];		//发送端以太网地址
    u8 sipaddr[4];	//发送端ip地址
    u8 dmac[6];		//接收端以太网地址
    u8 dipaddr[4];	//接收端ip地址
	
}ARP_HDR;

ARP_HDR arpbuf_tx;
ARP_HDR arpbuf_rx;


extern u8 host_mac_addr[6];
extern u8 mac_addr[6];
extern u8 ip_addr[4];
extern u8 host_ip_addr[4];
extern u16 packet_len;

extern u8 *buffer_tx;
extern u8 *buffer_rx;