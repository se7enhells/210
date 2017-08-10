#define HON(n) ((((u16)((n) & 0xff)) << 8) | (((n) & 0xff00) >> 8) ) //两字节 主机序 和网络序 转化

#define PROTO_ARP			(0x0806)
#define PROTO_IP			(0x0800)
#define PROTO_UDP 0x11

typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;

//以太网头 14字节
typedef struct eth_hdr  
{
    u8 d_mac[6];  //以太网目的地址
    u8 s_mac[6];  //以太网源地址
    u16 type;     //帧类型 
}ETH_HDR;

//ARP头 14+28字节
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

//IP头 14+14字节
typedef struct ip_hdr
{
    ETH_HDR ethhdr;
    u8 vhl;
    u8 tos;
    u16 len;
    u16 ipid;
    u16 ipoffset;
    u8 ttl;
    u8 proto;
    u16 ipchksum;
    u8 srcipaddr[4];
    u8 destipaddr[4];
}IP_HDR;


//UDP头 28+8字节
typedef struct udp_hdr
{
    IP_HDR iphdr;
    u16 sport;
    u16 dport;
    u16 len;
    u16 udpchksum;
}UDP_HDR;


//TFTP 数据包 5字节
typedef struct tftp_package
{
    u16 opcode;
    u16 blocknum;
    u8 data[0];	
}TFTP_PAK;


ARP_HDR arpbuf;  // arp包

extern u8 host_mac_addr[6];
extern u8 mac_addr[6];
extern u8 ip_addr[4];
extern u8 host_ip_addr[4];
extern u16 packet_len;
