#include "construct_data.h"
/*      该construct_data.c完成以下几个功能：（1）Ttft_tbl* makeup_tft_tbl_data() 完成映射表的内存初始化分配和映射表中值的初始化，返回一个映射表内第一个子表的地址，返回指针类型Ttft_tbl* ；          */  
/*                                        （2）int free_tft_tbl_data(Ttft_tbl* tft_tbl_ptr) 将用于存放映射表的内存free掉，注意先free掉（子）指针，再free掉外部的指针（返回0）              */
/*                                        （3）声明若干个用于调用的子函数，这些子函数主要完成IP包头中关键字的赋值  （这些子函数无返回值，都是在利用指针在给内存空间赋值）              */
/*                                        （4）Tpdu_data_req* makeup_pdu_data()完成4个数据包的地址空间的分配和其中各个内存变量的赋值，返回一个Tpdu_data_req* pdu_data_ptr类型的指针     */
/*                                         (5) int free_pdu_data(Tpdu_data_req* pdu_data_ptr) 利用传入的Tpdu_data_req* pdu_data_ptr指针将用于存放数据包的内存free掉（返回0）     */
/*                                        （6）Osa_status check_result(Tpdu_data_req* pdu_data_ptr, Uint8* expect_result)用来对比包中的rb_id是否等于expect_result*/

Ttft_tbl* makeup_tft_tbl_data()
{

	Ttft_tbl* tft_rb_tbl_tail;
	Ttft_tbl* tft_rb_tbl_node;
	Ttft_tbl* tft_rb_tbl_head;
	tft_rb_tbl_tail = (Ttft_tbl *)osa_malloc(sizeof(Ttft_tbl));
	tft_rb_tbl_tail->pf_ptr = (Tpacket_filter *)osa_malloc(sizeof(Tpacket_filter));
	/*begin init packet filter content*/
	tft_rb_tbl_tail->pf_ptr->pfi = 4;
	tft_rb_tbl_tail->pf_ptr->epi = 5;
	tft_rb_tbl_tail->pf_ptr->tft_type = 0x10;
	tft_rb_tbl_tail->pf_ptr->spi = 0x0F80F000;
	/*end init packet filter content*/
	tft_rb_tbl_tail->rb_id = 8;
	tft_rb_tbl_tail->next_ptr = (Ttft_tbl*)NULLPTR;


	tft_rb_tbl_node = (Ttft_tbl *)osa_malloc(sizeof(Ttft_tbl));
	tft_rb_tbl_node->pf_ptr = (Tpacket_filter *)osa_malloc(sizeof(Tpacket_filter));
	/*begin init packet filter content*/
	tft_rb_tbl_node->pf_ptr->pfi = 3;
	tft_rb_tbl_node->pf_ptr->epi = 3;
	tft_rb_tbl_node->pf_ptr->tft_type = 0x20;
	tft_rb_tbl_node->pf_ptr->tos_traffic_cls = 0x28;
	tft_rb_tbl_node->pf_ptr->tos_traffic_mask = 0xFC;
	/*end init packet filter content*/
	tft_rb_tbl_node->rb_id = 7;
	tft_rb_tbl_node->next_ptr = tft_rb_tbl_tail;

	tft_rb_tbl_head = (Ttft_tbl *)osa_malloc(sizeof(Ttft_tbl));
	tft_rb_tbl_head->pf_ptr = (Tpacket_filter *)osa_malloc(sizeof(Tpacket_filter));
	/*begin init packet filter content*/
	tft_rb_tbl_head->pf_ptr->pfi = 1;
	tft_rb_tbl_head->pf_ptr->epi = 0;
	tft_rb_tbl_head->pf_ptr->tft_type = 0xb;
	tft_rb_tbl_head->pf_ptr->sasm_num = 4;
	tft_rb_tbl_head->pf_ptr->pn_nh = 6;
	tft_rb_tbl_head->pf_ptr->remote_address[0] = 172;
	tft_rb_tbl_head->pf_ptr->remote_address[1] = 168;
	tft_rb_tbl_head->pf_ptr->remote_address[2] = 8;
	tft_rb_tbl_head->pf_ptr->remote_address[3] = 0;
	tft_rb_tbl_head->pf_ptr->remote_subnet_mask[0] = 255;
	tft_rb_tbl_head->pf_ptr->remote_subnet_mask[1] = 255;
	tft_rb_tbl_head->pf_ptr->remote_subnet_mask[2] = 255;
	tft_rb_tbl_head->pf_ptr->remote_subnet_mask[3] = 0;
	tft_rb_tbl_head->pf_ptr->remote_port_low = 5003;
	tft_rb_tbl_head->pf_ptr->remote_port_high = 5003;
	/*end init packet filter content*/
	tft_rb_tbl_head->rb_id = 6;
	tft_rb_tbl_head->next_ptr = tft_rb_tbl_node;

	return tft_rb_tbl_head;

}

int free_tft_tbl_data(Ttft_tbl* tft_tbl_ptr)
{
	Ttft_tbl* p1 = tft_tbl_ptr;
	Ttft_tbl* p2;
	while(p1 != NULLPTR)
	{
		p2 = p1->next_ptr;
		osa_free(p1->pf_ptr);
		p1->pf_ptr = (Tpacket_filter*)NULLPTR;
		osa_free(p1);
		p1 = p2;
	}
	return 0;
}

void pdu_data_set_ip_type(Uint8* ip_pdu_ptr, Uint8 ip_type)
{
	*ip_pdu_ptr = ip_type;
}
void pdu_data_set_tos(Uint8* ip_pdu_ptr, Uint8 tos)
{
	//IPV4
	if(4 == ((*ip_pdu_ptr & 0xF0) >> 4))
	{
		*(ip_pdu_ptr+1) = tos;
	}
	//IPV6
	else if(6 == ((*ip_pdu_ptr & 0xF0) >> 4))
	{
		*ip_pdu_ptr = (*ip_pdu_ptr & 0xF0) | ((tos & 0xF0) >> 4);
		*(ip_pdu_ptr+1) = ((tos & 0x0F) << 4) | (*(ip_pdu_ptr+1) & 0x0F) ;
	}
}

void pdu_data_set_flow_label(Uint8* ip_pdu_ptr, Uint32 flow_label)
{
	*(ip_pdu_ptr+1) = (*(ip_pdu_ptr+1) & 0xF0) | ((flow_label & 0x0F0000) >> 16);
	*(ip_pdu_ptr+2) = (flow_label & 0xFF00) >> 8;
	*(ip_pdu_ptr+3) = flow_label & 0xFF;
}

void pdu_data_set_total_length(Uint8* ip_pdu_ptr,Uint16 total_length)
{
	//IPV4
	if(4 == ((*ip_pdu_ptr & 0xF0) >> 4))
	{
		*(ip_pdu_ptr+2) = (total_length & 0xFF00) >> 8;
		*(ip_pdu_ptr+3) = (total_length & 0xFF);
	}
	//IPV6
	else if(6 == ((*ip_pdu_ptr & 0xF0) >> 4))
	{
		*(ip_pdu_ptr+4) = (total_length & 0xFF00) >> 8;
		*(ip_pdu_ptr+5) = (total_length & 0xFF);
	}
}

void pdu_data_set_protocol(Uint8* ip_pdu_ptr,Uint8 protocol)
{
	//IPV4
	if(4 == ((*ip_pdu_ptr & 0xF0) >> 4))
	{
		*(ip_pdu_ptr+9) = protocol;
	}
	//IPV6
	else if(6 == ((*ip_pdu_ptr & 0xF0) >> 4))
	{
		*(ip_pdu_ptr+6) = protocol;
	}
}

void pdu_data_set_src_addr(Uint8* ip_pdu_ptr, Uint8* ip_addr)
{
	Uint8 i = 0;
	//IPV4
	if(4 == ((*ip_pdu_ptr & 0xF0) >> 4))
	{
		for(i=0;i<4;i++)
		{
			*(ip_pdu_ptr+12+i) = *(ip_addr+i);
		}
	}
	//IPV6
	else if(6 == ((*ip_pdu_ptr & 0xF0) >> 4))
	{
		for(i=0;i<16;i++)
		{
			*(ip_pdu_ptr+8+i) = *(ip_addr+i);
		}
	}
}

void pdu_data_set_dest_addr(Uint8* ip_pdu_ptr, Uint8* ip_addr)
{
	Uint8 i = 0;
	//IPV4
	if(4 == ((*ip_pdu_ptr & 0xF0) >> 4))
	{
		for(i=0;i<4;i++)
		{
			*(ip_pdu_ptr+16+i) = *(ip_addr+i);
		}
	}
	//IPV6
	else if(6 == ((*ip_pdu_ptr & 0xF0) >> 4))
	{
		for(i=0;i<16;i++)
		{
			*(ip_pdu_ptr+24+i) = *(ip_addr+i);
		}
	}
}

void pdu_data_set_src_port(Uint8* ip_pdu_ptr, Uint16 port)
{
	//IPV4
	if(4 == ((*ip_pdu_ptr & 0xF0) >> 4))
	{
		*(ip_pdu_ptr+20) = (port & 0xFF00) >> 8;
		*(ip_pdu_ptr+21) = (port & 0xFF);
	}
	//IPV6
	else if(6 == ((*ip_pdu_ptr & 0xF0) >> 4))
	{
		*(ip_pdu_ptr+40) = (port & 0xFF00) >> 8;
		*(ip_pdu_ptr+41) = (port & 0xFF);
	}
}

void pdu_data_set_dest_port(Uint8* ip_pdu_ptr, Uint16 port)
{
	//IPV4
	if(4 == ((*ip_pdu_ptr & 0xF0) >> 4))
	{
		*(ip_pdu_ptr+22) = (port & 0xFF00) >> 8;
		*(ip_pdu_ptr+23) = (port & 0xFF);
	}
	//IPV6
	else if(6 == ((*ip_pdu_ptr & 0xF0) >> 4))
	{
		*(ip_pdu_ptr+42) = (port & 0xFF00) >> 8;
		*(ip_pdu_ptr+43) = (port & 0xFF);
	}
}

void pdu_data_set_spi(Uint8* ip_pdu_ptr, Uint8* spi)
{
	Uint8 i = 0;
	//IPV4
	if(4 == ((*ip_pdu_ptr & 0xF0) >> 4))
	{
		if(50 == *(ip_pdu_ptr+9))
		{
			for(i=0;i<4;i++)
			{
				*(ip_pdu_ptr+20+i) = spi[i];
			}
		}
		if(51 == *(ip_pdu_ptr+9))
		{
			for(i=0;i<4;i++)
			{
				*(ip_pdu_ptr+24+i) = spi[i];
			}
		}
	}
	//IPV6
	else if(6 == ((*ip_pdu_ptr & 0xF0) >> 4))
	{
		if(50 == *(ip_pdu_ptr+9))
		{
			for(i=0;i<4;i++)
			{
				*(ip_pdu_ptr+40+i) = spi[i];
			}
		}
		if(51 == *(ip_pdu_ptr+9))
		{
			for(i=0;i<4;i++)
			{
				*(ip_pdu_ptr+44+i) = spi[i];
			}
		}
	}
}


Tpdu_data_req* makeup_pdu_data()
{
	char pdu_data[500] = {0x00};
	Uint8 addr[16],spi[4];
	Uint8* buff_base;
	Uint8 i = 0;
	//char chr;
	Tpdu_data_req* pdu_data_ptr = (Tpdu_data_req *)osa_malloc(sizeof(Tpdu_data_req));

	osa_memset(pdu_data,0,500);
	pdu_data_ptr->pkts_num = 4;	
	//pdu[0]
	pdu_data_ptr->pkt_info[0].w_data_offset = 14;
	pdu_data_ptr->pkt_info[0].w_data_len = 500;
	pdu_data_ptr->pkt_info[0].w_buffer_ptr = (Uint8 *)osa_malloc(sizeof(char)*500);
	
	buff_base = pdu_data_ptr->pkt_info[0].w_buffer_ptr + pdu_data_ptr->pkt_info[0].w_data_offset;
	//IP type && version header len
	pdu_data_set_ip_type(buff_base,(4 << 4) + 5);
	//TOS
	pdu_data_set_tos(buff_base,232);
	//total_len
	pdu_data_set_total_length(buff_base,500);
	//Protocol
	pdu_data_set_protocol(buff_base,6);
	//Source Address
	addr[0] = 180;
	addr[1] = 101;
	addr[2] = 49;
	addr[3] = 12;
	pdu_data_set_src_addr(buff_base,addr);
	//Destination Address
	addr[0] = 172;
	addr[1] = 168;
	addr[2] = 8;
	addr[3] = 8;
	pdu_data_set_dest_addr(buff_base,addr);
	//Source Port
	pdu_data_set_src_port(buff_base,2001);
	//Destination Port
	pdu_data_set_dest_port(buff_base,5003);

	//pdu[1]
	i = 0;
	osa_memset(pdu_data,0,500);
	pdu_data_ptr->pkt_info[1].w_data_offset = 14;
	pdu_data_ptr->pkt_info[1].w_data_len = 500;
	pdu_data_ptr->pkt_info[1].w_buffer_ptr = (Uint8 *)osa_malloc(sizeof(char)*500);
	
	buff_base = pdu_data_ptr->pkt_info[1].w_buffer_ptr + pdu_data_ptr->pkt_info[1].w_data_offset;
	//IP type && version header len
	pdu_data_set_ip_type(buff_base,(4 << 4) + 5);
	//TOS
	pdu_data_set_tos(buff_base,43);
	//total_len
	pdu_data_set_total_length(buff_base,500);
	//Protocol
	pdu_data_set_protocol(buff_base,50);
	//Source Address
	addr[0] = 172;
	addr[1] = 168;
	addr[2] = 10;
	addr[3] = 5;
	pdu_data_set_src_addr(buff_base,addr);
	//Destination Address
	addr[0] = 180;
	addr[1] = 101;
	addr[2] = 49;
	addr[3] = 12;
	pdu_data_set_dest_addr(buff_base,addr);
	//SPI
	spi[0] = 0x0F;
	spi[1] = 0x80;
	spi[2] = 0xF0;
	spi[3] = 0x00;
	pdu_data_set_spi(buff_base,spi);

	//pdu[2]
	i = 0;
	osa_memset(pdu_data,0,500);
	pdu_data_ptr->pkt_info[2].w_data_offset = 14;
	pdu_data_ptr->pkt_info[2].w_data_len = 500;
	pdu_data_ptr->pkt_info[2].w_buffer_ptr = (Uint8 *)osa_malloc(sizeof(char)*500);
	
	buff_base = pdu_data_ptr->pkt_info[2].w_buffer_ptr + pdu_data_ptr->pkt_info[2].w_data_offset;
	//IP type && version header len
	pdu_data_set_ip_type(buff_base,(4 << 4) + 5);
	//TOS
	pdu_data_set_tos(buff_base,169);
	//total_len
	pdu_data_set_total_length(buff_base,500);
	//Protocol
	pdu_data_set_protocol(buff_base,17);
	//Source Address
	addr[0] = 172;
	addr[1] = 168;
	addr[2] = 10;
	addr[3] = 6;
	pdu_data_set_src_addr(buff_base,addr);
	//Destination Address
	addr[0] = 180;
	addr[1] = 101;
	addr[2] = 49;
	addr[3] = 12;
	pdu_data_set_dest_addr(buff_base,addr);
	//Source Port
	pdu_data_set_src_port(buff_base,2003);
	//Destination Port
	pdu_data_set_dest_port(buff_base,5005);

	//SPI
	spi[0] = 0x0F;
	spi[1] = 0x80;
	spi[2] = 0xF0;
	spi[3] = 0x00;
	pdu_data_set_spi(buff_base,spi);

	//pdu[3]
	i = 0;
	osa_memset(pdu_data,0,500);
	pdu_data_ptr->pkt_info[3].w_data_offset = 14;
	pdu_data_ptr->pkt_info[3].w_data_len = 500;
	pdu_data_ptr->pkt_info[3].w_buffer_ptr = (Uint8 *)osa_malloc(sizeof(char)*500);
	
	buff_base = pdu_data_ptr->pkt_info[3].w_buffer_ptr + pdu_data_ptr->pkt_info[3].w_data_offset;
	//IP type && tos high byte part
	pdu_data_set_ip_type(buff_base,(6 << 4));
	//TOS
	pdu_data_set_tos(buff_base,0x6F);
	//payload length
	pdu_data_set_total_length(buff_base,500);
	//Next Header protocol
	pdu_data_set_protocol(buff_base,51);

	//Source Address
	addr[0] = 0xFE;
	addr[1] = 0x80;
	addr[2] = 0xF8;
	addr[3] = 0x0F;
	addr[4] = 0x6D;
	addr[5] = 0x4C;
	addr[6] = 0x43;
	addr[7] = 0x10;
	addr[8] = 0xAB;
	addr[9] = 0xCD;
	addr[10] = 0xEF;
	addr[11] = 0x01;
	addr[12] = 0x23;
	addr[13] = 0x45;
	addr[14] = 0x67;
	addr[15] = 0x89;
	pdu_data_set_src_addr(buff_base,addr);

	//Destination Address
	addr[0] = 0xFE;
	addr[1] = 0x80;
	addr[2] = 0xEF;
	addr[3] = 0x01;
	addr[4] = 0x6D;
	addr[5] = 0x4C;
	addr[6] = 0x43;
	addr[7] = 0x10;
	addr[8] = 0xAB;
	addr[9] = 0xCD;
	addr[10] = 0xEF;
	addr[11] = 0x01;
	addr[12] = 0x23;
	addr[13] = 0x45;
	addr[14] = 0x67;
	addr[15] = 0x89;
	pdu_data_set_dest_addr(buff_base,addr);
	//Source Port
	pdu_data_set_src_port(buff_base,2004);
	//Destination Port
	pdu_data_set_dest_port(buff_base,5006);
	return pdu_data_ptr;
}

int free_pdu_data(Tpdu_data_req* pdu_data_ptr)
{
	Uint8 i = 0;
	if(NULLPTR == pdu_data_ptr)
	{
		return 0;
	}
	for(i=0;i<pdu_data_ptr->pkts_num;i++)
	{
		if(NULLPTR != pdu_data_ptr->pkt_info[i].w_buffer_ptr)
		{
			osa_free(pdu_data_ptr->pkt_info[i].w_buffer_ptr);
			pdu_data_ptr->pkt_info[i].w_buffer_ptr = (Uint8*)NULLPTR;
		}
	}
	osa_free(pdu_data_ptr);
	pdu_data_ptr = (Tpdu_data_req*)NULLPTR;
	return 0;
}

Osa_status check_result(Tpdu_data_req* pdu_data_ptr, Uint8* expect_result)
{
	Uint8 i = 0;
	if(NULLPTR == pdu_data_ptr)
	{
		return OSA_FAILURE;
	}
	for(i=0;i<pdu_data_ptr->pkts_num;i++)
	{
		if(pdu_data_ptr->pkt_info[i].rb_id != expect_result[i])
		{
			return OSA_FAILURE;
		}
	}
	return OSA_SUCCESS;
}
