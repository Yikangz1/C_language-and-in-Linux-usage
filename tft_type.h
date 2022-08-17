#ifndef TFT_TYPE_H
#define TFT_TYPE_H
#include "osa.h"

/*IPv4 IPv6 Header related*/
#define MMA_IPV6_LEN_OF_ADR 16
#define MMA_IPV4_LEN_OF_ADR 4
#define MMA_IP_PROTOCOL_ICMP 1
#define MMA_IP_PROTOCOL_TCP  6
#define MMA_IP_PROTOCOL_UDP 17
#define MMA_IP_PROTOCOL_IPINIP 4
#define MMA_IP_PROTOCOL_DCCP 33
#define MMA_IP_PROTOCOL_ESP 50
#define MMA_IP_PROTOCOL_AH 51
#define MMA_IP_PROTOCOL_MTP 92
#define MMA_IP_PROTOCOL_UDPLITE 136
#define MMA_IP_PROTOCOL_SCTP 132
#define MMA_IP_PROTOCOL_PGM 113
#define MMA_IP_PROTOCOL_RDP 27
/* Define for IPv6 Next Header(same as IPv4 Protocol), see RFC 2460. */
#define MMA_IP_PROTOCOL_HOP_BY_HOP_OP   0
#define MMA_IP_PROTOCOL_DESTINATION_OP  60
#define MMA_IP_PROTOCOL_ROUTING         43
#define MMA_IP_PROTOCOL_FRAGMENT        44

#define MMA_TFT_OPTION_REMOTE_ADDR		0X01 /*Remote Address and Subnet Mask*/
#define MMA_TFT_OPTION_PROTOCOL_NUM		0X02 /*Protocol Number (IPv4) / Next Header (IPv6)*/
#define MMA_TFT_OPTION_LOCAL_PORT		0X04 /*Local Port Range*/
#define MMA_TFT_OPTION_REMOTE_PORT		0X08 /*Remote Port Range*/
#define MMA_TFT_OPTION_SPI				0X10 /*IPSec SPI*/
#define MMA_TFT_OPTION_TOS				0X20 /*TOS (IPv4) / Traffic Class (IPv6) and Mask*/
#define MMA_TFT_OPTION_FLOW_LABEL       0X40 /*Flow Label (IPv6)*/
#define MMA_TFT_OPTION_LOCAL_ADDR		0X80 /*Local Address and Subnet Mask*/

#define MMA_PDP_IPV4_TYPE               33  /*define "IP" type*/
#define MMA_PDP_IPV6_TYPE               87  /*define "IPv6" type*/

#define MMA_PACKET_PARSE_FAIL_NULL_POINTER 0X01
#define MMA_PACKET_PARSE_FAIL_IPV4_HD_LEN_ERROR 0X02
#define MMA_PACKET_PARSE_FAIL_IPV6_FRAGMENT_NOT_SUPPORT 0X04
#define MMA_PACKET_PARSE_FAIL_HD_VERSION_ERROR 0X08

#define MMA_TFT_UNMATCH_CAUSE_NULL_POINTER 0X01
#define MMA_TFT_UNMATCH_CAUSE_TOS 0X02
#define MMA_TFT_UNMATCH_CAUSE_REMOTE_ADDR 0X03
#define MMA_TFT_UNMATCH_CAUSE_LOCAL_ADDR 0X04
#define MMA_TFT_UNMATCH_CAUSE_PROTOCOL_NUM 0X5
#define MMA_TFT_UNMATCH_CAUSE_LOCAL_PORT 0X6
#define MMA_TFT_UNMATCH_CAUSE_REMOTE_PORT 0X7
#define MMA_TFT_UNMATCH_CAUSE_SPI 0X8
#define MMA_TFT_UNMATCH_CAUSE_FLOW_LABEL 0X9

typedef struct Tpacket_info_st
{
    Uint16  src_port;
    Uint16  dest_port;
    Uint8   tos;/*type of service (tos) (ipv4) / traffic class (ipv6)*/
    Uint8   protocol;/*protocol number (ipv4) / next header (ipv6)*/
    /*OSA_FILLER2*/
    Uint8   pdp_type;
    Uint8	FILLER;
	Uint8   ip_src_addr[16];
    Uint8   ip_dest_addr[16];
    Uint32  spi;/*Security Parameters Index (SPI)*/
    Uint32 flow_label;/*flow label (ipv6)*/
}Tpacket_info;

typedef struct Tpacket_filter_st
{
    Uint8  pfi;/*packet filter identifier*/
    Uint8  epi;/*evaluation precedence index*/
	Uint8  sasm_num;/*address length*/ 
	Uint8  FILLER1;
    Uint32  tft_type;   
    Uint8  single_dport;
    Uint8  single_sport;
    Uint16 pn_nh;/*protocol number (ipv4) / next header (ipv6)*/
	Uint8  remote_address[MMA_IPV6_LEN_OF_ADR];/*remote address*/
    Uint8  remote_subnet_mask[MMA_IPV6_LEN_OF_ADR];/*remote subnet mask*/
	Uint8  local_address[MMA_IPV6_LEN_OF_ADR];/*local address*/
    Uint8  local_subnet_mask[MMA_IPV6_LEN_OF_ADR];/*local subnet mask*/
    Uint16 remote_port_low;/*remote port range*/
    Uint16 remote_port_high;/*remote port range*/
    Uint16 local_port_low;/*local port range*/
    Uint16 local_port_high;/*local port range*/
    Uint32 spi;/*ipsec security parameter index (spi)*/
    Uint32 flow_label;/*flow label (ipv6)>: */
    Uint8  tos_traffic_cls;/*type of service (tos) (ipv4) and mask / traffic class (ipv6) and mask*/
    Uint8  tos_traffic_mask;/*type of service (tos) (ipv4) and mask / traffic class (ipv6) and mask*/
    Uint8  direction;/*filter direction*/
    Uint8  FILLER2;
}Tpacket_filter;

typedef struct Ttft_tbl_st
{
	Uint8  rb_id;
	Uint8  FILLER1;
	Uint16  FILLER2;
	Tpacket_filter*  pf_ptr;
	struct Ttft_tbl_st*  next_ptr;
}Ttft_tbl;

typedef struct Tpdu_data_block_st
{
    Uint8* w_buffer_ptr;
    Uint32 w_data_offset; 
    Uint32 w_data_len;
	Uint32 rb_id;
} Tpdu_data_block;

typedef struct Tpdu_data_req_st
{
    Tpdu_data_block  pkt_info[64];      /* 数据信息         */
    Uint8           pkts_num;
    Uint8			FILLER[3];
}Tpdu_data_req;

typedef struct Tpdu_data_block_debug_info_st
{
	Uint8		parse_result;
	Uint8		parse_fail_cause;
	Uint16		FILLER;
	Uint8		unmatch_cause[64];
}Tpdu_data_block_debug_info;

typedef struct Ttft_debug_info_st
{
	Tpdu_data_block_debug_info block_debug_info[64];
}Ttft_debug_info;

Osa_status get_packet_info_from_ip_pack(Uint8* ip_data_ptr, Tpacket_info* packet_info, Tpdu_data_block_debug_info* block_debug_info);
Uint8 tft_matched(Ttft_tbl* tft_tbl_ptr, Tpacket_info* packet_info_ptr, Tpdu_data_block_debug_info* block_debug_info);
Osa_status tft_process(Tpdu_data_req* pdu_data_ptr, Ttft_tbl* tft_tbl_ptr, Ttft_debug_info* tft_debug_info, Uint8 run_mode);

#endif
