#define HON(n) ((((u16)((n) & 0xff)) << 8) | (((n) & 0xff00) >> 8) ) //���ֽ� ������ �������� ת��

#define PROTO_ARP			(0x0806)
#define PROTO_IP			(0x0800)
#define PROTO_UDP 0x11

typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;

//��̫��ͷ 14�ֽ�
typedef struct eth_hdr  
{
    u8 d_mac[6];  //��̫��Ŀ�ĵ�ַ
    u8 s_mac[6];  //��̫��Դ��ַ
    u16 type;     //֡���� 
}ETH_HDR;

//ARPͷ 14+28�ֽ�
typedef struct arp_hdr
{
	ETH_HDR ethhdr; //��̫��ͷ
    u16 hwtype;     //Ӳ������
    u16 protocol;	//	Э������
    u8 hwlen;		//Ӳ����ַ����
    u8 protolen;	//	Э���ַ����
    u16 opcode;		//ARP���������Ӧ��� 1 ���� 2 Ӧ��
    u8 smac[6];		//���Ͷ���̫����ַ
    u8 sipaddr[4];	//���Ͷ�ip��ַ
    u8 dmac[6];		//���ն���̫����ַ
    u8 dipaddr[4];	//���ն�ip��ַ
	
}ARP_HDR;

//IPͷ 14+14�ֽ�
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


//UDPͷ 28+8�ֽ�
typedef struct udp_hdr
{
    IP_HDR iphdr;
    u16 sport;
    u16 dport;
    u16 len;
    u16 udpchksum;
}UDP_HDR;


//TFTP ���ݰ� 5�ֽ�
typedef struct tftp_package
{
    u16 opcode;
    u16 blocknum;
    u8 data[0];	
}TFTP_PAK;


ARP_HDR arpbuf;  // arp��

extern u8 host_mac_addr[6];
extern u8 mac_addr[6];
extern u8 ip_addr[4];
extern u8 host_ip_addr[4];
extern u16 packet_len;
