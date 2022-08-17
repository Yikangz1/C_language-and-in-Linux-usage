#include <stddef.h>
#include <stdio.h>
#include "tft_type.h"

/*****************************************************************************
* Function name:    get_packet_info_from_ip_pack() 
* Purpose:          decode ip packet and return packet_info from it
* Relation:
* Function Storage Type: file-local
* Parameters:                                               
*                                                           
* Parameter Name     Data Type      Input/Output      Description                        
* --------------     ---------      -----------       ---------------                   
*   ip_data_ptr     Uint8               input         ipv4 packet head
*   packet_info     Tpacket_info    output        some IP data field  
*
* Return Value: 
* 
*/
/* Osa_status get_packet_info_from_ip_pack(Uint8* ip_data_ptr, Tpacket_info* packet_info, Tpdu_data_block_debug_info* block_debug_info)函数的功能是将第[i]个packet的IP包头地址指针+“篮子”的地址指针+debug memory block */
/* 模块的地址指针传入，来提取第[i]个packet的包头信息到“篮子”中，并返回一个布尔值OSA_FAILURE/OSA_SUCCESS表征提取数据的是否成功  */
/* Uint8 tft_matched(Ttft_tbl* tft_tbl_ptr, Tpacket_info* packet_info_ptr, Tpdu_data_block_debug_info* block_debug_info)函数的功能是将映射表的地址指针+存放第[i]个packet IP包头信息的“篮子”的地址指针+debug moeory clock */
/* 的地址指针传入，将“篮子“中存放的第[i]个包头信息与映射表中的各个filter分别比对，返回一个Uint8的变量（若这个packet信息与某个filter match，则将这个filter对应的rb_id返回；否则将0xff返回）  */
/* Osa_status tft_process(Tpdu_data_req* pdu_data_ptr, Ttft_tbl* tft_tbl_ptr, Ttft_debug_info* tft_debug_info, Uint8 run_mode)函数的功能是将存放data memory block的地址指针+映射表的表头地址+debug memory clock  */
/* 的地址指针传入，将各个packet的包头信息取出来后，分别与映射表中的各个filter分别比对，来更新 Tpdu_data_req结构体中各个packet对应的rb_id的值（其中若第[i]个packet与某个filer 匹配则rb_id=bear_id;否则rb_id=0xff）.最后回传OSA_SUCCESS表示tft对比结束*/
Osa_status get_packet_info_from_ip_pack(Uint8* ip_data_ptr, Tpacket_info* packet_info, Tpdu_data_block_debug_info* block_debug_info)
{
    Uint8  ipv4_hd_len = 0; /* header length (ipv4 only) */
    Uint8 * temp_ptr = NULL;

    Uint8   tmp_protocol = MMA_IP_PROTOCOL_TCP;
    
    if (NULLPTR==ip_data_ptr || NULLPTR==packet_info)
    {
		if(NULLPTR != block_debug_info)
		{
			block_debug_info->parse_result = OSA_FAILURE;
			block_debug_info->parse_fail_cause = MMA_PACKET_PARSE_FAIL_NULL_POINTER;
		}
        return OSA_FAILURE;
    }
    if(((*ip_data_ptr&0xf0)>>0x04) == 4)/* ipv4 */
    {
        ipv4_hd_len = ((*ip_data_ptr)&0x0f)*(32/8);/* IPV4的包头长度是固定的20字节 */
        if (ipv4_hd_len<20)
        {
			if(NULLPTR != block_debug_info)
			{
				block_debug_info->parse_result = OSA_FAILURE;
				block_debug_info->parse_fail_cause = MMA_PACKET_PARSE_FAIL_IPV4_HD_LEN_ERROR;
			}
            return OSA_FAILURE;
        }

        packet_info->pdp_type = MMA_PDP_IPV4_TYPE;
        
        packet_info->tos = *(ip_data_ptr+1);
        packet_info->protocol = *(ip_data_ptr+9);

        if (MMA_IP_PROTOCOL_AH == packet_info->protocol)
        {
            packet_info->spi = osa_get_u32(ip_data_ptr+ipv4_hd_len+4);
        }

        else if (MMA_IP_PROTOCOL_ESP == packet_info->protocol)
        {
            packet_info->spi = osa_get_u32(ip_data_ptr+ipv4_hd_len);
        }
        else /* SPI is not exist in other protocol's IP pack, fill 0 here. */
        {
            packet_info->spi = 0;/* 若protocol既不是50也不是51，则在构建packet的时候spi是无法有效写入的。相应的，在读取spi的时候，若protocol既不是50或51，自然也没有有效的spi返回 */
        }

        temp_ptr = ip_data_ptr+12;/*source address*/
        osa_memcpy(packet_info->ip_src_addr, temp_ptr, MMA_IPV4_LEN_OF_ADR);
        
        temp_ptr = ip_data_ptr+16;/*destination address*/
        
        osa_memcpy(packet_info->ip_dest_addr, temp_ptr, MMA_IPV4_LEN_OF_ADR);
        
        temp_ptr = ip_data_ptr+ipv4_hd_len;
        
        if (MMA_IP_PROTOCOL_TCP == packet_info->protocol
            || MMA_IP_PROTOCOL_UDP == packet_info->protocol
            || MMA_IP_PROTOCOL_DCCP == packet_info->protocol
            || MMA_IP_PROTOCOL_IPINIP == packet_info->protocol
            || MMA_IP_PROTOCOL_ESP == packet_info->protocol
            || MMA_IP_PROTOCOL_AH == packet_info->protocol
            || MMA_IP_PROTOCOL_MTP == packet_info->protocol
            || MMA_IP_PROTOCOL_UDPLITE == packet_info->protocol
            || MMA_IP_PROTOCOL_SCTP == packet_info->protocol
            || MMA_IP_PROTOCOL_PGM == packet_info->protocol
            || MMA_IP_PROTOCOL_RDP == packet_info->protocol
            )
        {
            if (MMA_IP_PROTOCOL_RDP == packet_info->protocol)
            {
                temp_ptr+=2;
            }
            packet_info->src_port = osa_get_u16(temp_ptr);
            temp_ptr += 2;
            packet_info->dest_port = osa_get_u16(temp_ptr);
        }
    }

    /* Parse IPv6 info according to RFC 2460. */
    else if (((*ip_data_ptr&0xF0)>>0x04) == 6)/* IPv6 */
    {
        /*PDP type*/
        packet_info->pdp_type = MMA_PDP_IPV6_TYPE;
        
        /*Traffic Class*/
        packet_info->tos = (((*ip_data_ptr&0x0F)<<4)|((*(ip_data_ptr+1)&0xF0)>>4));

        /*Flow Label*/
        packet_info->flow_label = ((*(ip_data_ptr+1)&0x0F)<<16) +
                                ((*(ip_data_ptr+2))<<8) +
                                 (*(ip_data_ptr+3));

        /*Next Header*/
        packet_info->protocol = *(ip_data_ptr+6);
        tmp_protocol = packet_info->protocol;

        /*Source Address*/
        temp_ptr = ip_data_ptr+8;
        osa_memcpy(packet_info->ip_src_addr, temp_ptr, MMA_IPV6_LEN_OF_ADR);

        /*Destination Address*/
        temp_ptr = ip_data_ptr+24;
        osa_memcpy(packet_info->ip_dest_addr, temp_ptr, MMA_IPV6_LEN_OF_ADR);

        temp_ptr = ip_data_ptr+40;/*Skip IPv6 header*/

        if (MMA_IP_PROTOCOL_HOP_BY_HOP_OP == tmp_protocol)/*Hop-by-Hop Options header*/
        {
            packet_info->protocol = *temp_ptr;
            tmp_protocol = packet_info->protocol;
            temp_ptr += ((*(temp_ptr+1))*8 + 8);
        }
        if (MMA_IP_PROTOCOL_DESTINATION_OP == tmp_protocol)/*Destination Options header*/
        {
            /*  It's format is as same as Hop-by-Hop Options header. */
            packet_info->protocol = *temp_ptr;
            tmp_protocol = packet_info->protocol;
            temp_ptr += ((*(temp_ptr+1))*8 + 8);
        }
        if (MMA_IP_PROTOCOL_ROUTING == tmp_protocol)/*Routing header*/
		{
            packet_info->protocol = *temp_ptr;
            tmp_protocol = packet_info->protocol;
            temp_ptr += ((*(temp_ptr+1))*8 + 8);
        }
        if (MMA_IP_PROTOCOL_FRAGMENT == tmp_protocol)/*Fragment header*/
        {
			if(NULLPTR != block_debug_info)
			{
				block_debug_info->parse_result = OSA_FAILURE;
				block_debug_info->parse_fail_cause = MMA_PACKET_PARSE_FAIL_IPV6_FRAGMENT_NOT_SUPPORT;
			}
            printf("PSDIH fpsdih_get_tft_info_from_ip_pack: receive IPv6 segment, not supported!\n");
            return OSA_FAILURE;
        }
        if (MMA_IP_PROTOCOL_AH == tmp_protocol)/*Authentication header*/
        {
            tmp_protocol = *temp_ptr;
            packet_info->spi = osa_get_u32(temp_ptr+4);/*SPI*/
            temp_ptr += ((*(temp_ptr+1))*4 + 2*4);
        }
        if (MMA_IP_PROTOCOL_ESP == tmp_protocol)/*Encapsulating Security Payload header*/
        {
            tmp_protocol = *temp_ptr;
            packet_info->spi = osa_get_u32(temp_ptr);/*SPI*/
            return OSA_SUCCESS;/*Data after ESP header is encrypted, so return.*/
        }
        if (MMA_IP_PROTOCOL_DESTINATION_OP == tmp_protocol)/*Destination Options header*/
        {
            /*  It's format is as same as Hop-by-Hop Options header. */
            tmp_protocol = *temp_ptr;
            temp_ptr += ((*(temp_ptr+1))*8 + 8);
        }

        if (MMA_IP_PROTOCOL_TCP == tmp_protocol
            || MMA_IP_PROTOCOL_UDP == tmp_protocol
            || MMA_IP_PROTOCOL_DCCP == tmp_protocol
            || MMA_IP_PROTOCOL_IPINIP == tmp_protocol
            || MMA_IP_PROTOCOL_MTP == tmp_protocol
            || MMA_IP_PROTOCOL_UDPLITE == tmp_protocol
            || MMA_IP_PROTOCOL_SCTP == tmp_protocol
            || MMA_IP_PROTOCOL_PGM == tmp_protocol
            || MMA_IP_PROTOCOL_RDP == tmp_protocol
            )
        {
            if (MMA_IP_PROTOCOL_AH != packet_info->protocol)
            {
                packet_info->protocol = tmp_protocol;
            }
            if (MMA_IP_PROTOCOL_RDP == tmp_protocol)
            {
                temp_ptr += 2;
            }
            packet_info->src_port = osa_get_u16(temp_ptr);
            temp_ptr += 2;
            packet_info->dest_port = osa_get_u16(temp_ptr);
        }
    }
    else    /* Other type of packet reserved. */
    {
		if(NULLPTR != block_debug_info)
		{
			block_debug_info->parse_result = OSA_FAILURE;
			block_debug_info->parse_fail_cause = MMA_PACKET_PARSE_FAIL_HD_VERSION_ERROR;
		}
        return OSA_FAILURE;
    }
    if(NULLPTR != block_debug_info)
	{
		block_debug_info->parse_result = OSA_SUCCESS;
	}
    return OSA_SUCCESS;
}

/*****************************************************************************
* Function name:    fpsdih_is_tft_matched() 
* Purpose:          get bear_id from searching for the first matched TFT prioritized by epi, 
*                       set bear_id to zero if nothing matches
* Relation:
* Function Storage Type: file-local
* Parameters:                                               
*                                                           
* Parameter Name     Data Type      Input/Output        Description                        
* --------------     ---------      -----------         ---------------                   
*   tft_tbl_ptr			Ttft_tbl        input          contains tft_info to be matched with
*   packet_info_ptr		Tpacket_info    input           packet_info to match with info from packet_filter_ptr  
*
* Return Value: 
*   rb_id if is matched
*   0xff otherwise
*
* Note:
*   IPv6 not supported
*/
Uint8 tft_matched(Ttft_tbl* tft_tbl_ptr, Tpacket_info* packet_info_ptr, Tpdu_data_block_debug_info* block_debug_info)
{
    Uint8 i=0;
	Uint8 bear_id=0xff;
	Uint8 pf_current_num = 0;
	Bool is_match = False;
    //Tpsdih_pdp_pkt *packet_filter_ptr
    if (NULLPTR==packet_info_ptr || NULLPTR==tft_tbl_ptr)
    {
		if(NULLPTR != block_debug_info)
		{
			block_debug_info->unmatch_cause[pf_current_num++] = MMA_TFT_UNMATCH_CAUSE_NULL_POINTER;
		}
        return 0xff;
    }
	do{
		Tpacket_filter *packet_filter_ptr = tft_tbl_ptr->pf_ptr;
		bear_id = tft_tbl_ptr->rb_id;
		if(NULLPTR==packet_filter_ptr)
		{
			if(NULLPTR != block_debug_info)
			{
				block_debug_info->unmatch_cause[pf_current_num++] = MMA_TFT_UNMATCH_CAUSE_NULL_POINTER;
			}
			return 0xff;
		}
		if (packet_filter_ptr->tft_type & MMA_TFT_OPTION_TOS)
		{
			if ((packet_filter_ptr->tos_traffic_cls&packet_filter_ptr->tos_traffic_mask) 
				!= (packet_info_ptr->tos&packet_filter_ptr->tos_traffic_mask))
			{
				if(NULLPTR != block_debug_info)
				{
					block_debug_info->unmatch_cause[pf_current_num++] = MMA_TFT_UNMATCH_CAUSE_TOS;
				}
				tft_tbl_ptr = tft_tbl_ptr->next_ptr;
				continue;
			}
		}
		if (packet_filter_ptr->tft_type & MMA_TFT_OPTION_REMOTE_ADDR)
		{
			if (MMA_IPV4_LEN_OF_ADR == packet_filter_ptr->sasm_num && MMA_PDP_IPV4_TYPE == packet_info_ptr->pdp_type)
			{
				is_match = True;
				for (i=0;i<MMA_IPV4_LEN_OF_ADR;i++)
				{	
					if ((packet_filter_ptr->remote_address[i] & packet_filter_ptr->remote_subnet_mask[i])
						!= (packet_info_ptr->ip_dest_addr[i] & packet_filter_ptr->remote_subnet_mask[i]))
					{
						is_match = False;
						break;
					}
				}
				if(!is_match)
				{
					if(NULLPTR != block_debug_info)
					{
						block_debug_info->unmatch_cause[pf_current_num++] = MMA_TFT_UNMATCH_CAUSE_REMOTE_ADDR;
					}
					tft_tbl_ptr = tft_tbl_ptr->next_ptr;
					continue;
				}
			}
			else if (MMA_IPV6_LEN_OF_ADR == packet_filter_ptr->sasm_num && MMA_PDP_IPV6_TYPE == packet_info_ptr->pdp_type)
			{
				is_match = True;
				for (i=0;i<MMA_IPV6_LEN_OF_ADR;i++)
				{
					if ((packet_filter_ptr->remote_address[i] & packet_filter_ptr->remote_subnet_mask[i])
						!= (packet_info_ptr->ip_dest_addr[i] & packet_filter_ptr->remote_subnet_mask[i]))
					{
						is_match = False;
						break;
					}
				}
				if(!is_match)
				{
					if(NULLPTR != block_debug_info)
					{
						block_debug_info->unmatch_cause[pf_current_num++] = MMA_TFT_UNMATCH_CAUSE_REMOTE_ADDR;
					}
					tft_tbl_ptr = tft_tbl_ptr->next_ptr;
					continue;
				}
			}
			else
			{
				if(NULLPTR != block_debug_info)
				{
					block_debug_info->unmatch_cause[pf_current_num++] = MMA_TFT_UNMATCH_CAUSE_REMOTE_ADDR;
				}
				tft_tbl_ptr = tft_tbl_ptr->next_ptr;
				continue;
			}
		}
		if (packet_filter_ptr->tft_type & MMA_TFT_OPTION_LOCAL_ADDR)
		{
			if (MMA_IPV4_LEN_OF_ADR == packet_filter_ptr->sasm_num && MMA_PDP_IPV4_TYPE == packet_info_ptr->pdp_type)
			{
				is_match = True;
				for (i=0;i<MMA_IPV4_LEN_OF_ADR;i++)
				{	
					if ((packet_filter_ptr->local_address[i] & packet_filter_ptr->local_subnet_mask[i])
						!= (packet_info_ptr->ip_src_addr[i] & packet_filter_ptr->local_subnet_mask[i]))
					{
						is_match = False;
					}
				}
				if(!is_match)
				{
					if(NULLPTR != block_debug_info)
					{
						block_debug_info->unmatch_cause[pf_current_num++] = MMA_TFT_UNMATCH_CAUSE_LOCAL_ADDR;
					}
					tft_tbl_ptr = tft_tbl_ptr->next_ptr;
					continue;
				}
			}
			else if (MMA_IPV6_LEN_OF_ADR == packet_filter_ptr->sasm_num && MMA_PDP_IPV6_TYPE == packet_info_ptr->pdp_type)
			{
				is_match = True;
				for (i=0;i<MMA_IPV6_LEN_OF_ADR;i++)
				{
					if ((packet_filter_ptr->local_address[i] & packet_filter_ptr->local_subnet_mask[i])
						!= (packet_info_ptr->ip_src_addr[i] & packet_filter_ptr->local_subnet_mask[i]))
					{
						is_match = False;
					}
				}
				if(!is_match)
				{
					if(NULLPTR != block_debug_info)
					{
						block_debug_info->unmatch_cause[pf_current_num++] = MMA_TFT_UNMATCH_CAUSE_LOCAL_ADDR;
					}
					tft_tbl_ptr = tft_tbl_ptr->next_ptr;
					continue;
				}
			}
			else
			{
				if(NULLPTR != block_debug_info)
				{
					block_debug_info->unmatch_cause[pf_current_num++] = MMA_TFT_UNMATCH_CAUSE_LOCAL_ADDR;
				}
				tft_tbl_ptr = tft_tbl_ptr->next_ptr;
				continue;
			}
		}
		if (packet_filter_ptr->tft_type & MMA_TFT_OPTION_PROTOCOL_NUM)
		{
			if (packet_filter_ptr->pn_nh!= packet_info_ptr->protocol)
			{
				if(NULLPTR != block_debug_info)
				{
					block_debug_info->unmatch_cause[pf_current_num++] = MMA_TFT_UNMATCH_CAUSE_PROTOCOL_NUM;
				}
				tft_tbl_ptr = tft_tbl_ptr->next_ptr;
				continue;
			}
		}
		if (packet_filter_ptr->tft_type & MMA_TFT_OPTION_LOCAL_PORT)
		{
			if (MMA_IP_PROTOCOL_TCP != packet_info_ptr->protocol
				&& MMA_IP_PROTOCOL_UDP != packet_info_ptr->protocol
				&& MMA_IP_PROTOCOL_DCCP != packet_info_ptr->protocol
				&& MMA_IP_PROTOCOL_IPINIP != packet_info_ptr->protocol
				&& MMA_IP_PROTOCOL_ESP != packet_info_ptr->protocol
				&& MMA_IP_PROTOCOL_AH != packet_info_ptr->protocol
				&& MMA_IP_PROTOCOL_MTP != packet_info_ptr->protocol
				&& MMA_IP_PROTOCOL_UDPLITE != packet_info_ptr->protocol
				&& MMA_IP_PROTOCOL_SCTP != packet_info_ptr->protocol
				&& MMA_IP_PROTOCOL_PGM != packet_info_ptr->protocol
				&& MMA_IP_PROTOCOL_RDP != packet_info_ptr->protocol
				)
			{
				if(NULLPTR != block_debug_info)
				{
					block_debug_info->unmatch_cause[pf_current_num++] = MMA_TFT_UNMATCH_CAUSE_LOCAL_PORT;
				}
				tft_tbl_ptr = tft_tbl_ptr->next_ptr;
				continue;
			}
			if (!(packet_info_ptr->src_port <= packet_filter_ptr->local_port_high 
				&& packet_info_ptr->src_port >= packet_filter_ptr->local_port_low))
			{
				if(NULLPTR != block_debug_info)
				{
					block_debug_info->unmatch_cause[pf_current_num++] = MMA_TFT_UNMATCH_CAUSE_LOCAL_PORT;
				}
				tft_tbl_ptr = tft_tbl_ptr->next_ptr;
				continue;
			}
		}
		if (packet_filter_ptr->tft_type & MMA_TFT_OPTION_REMOTE_PORT)
		{
			if (MMA_IP_PROTOCOL_TCP != packet_info_ptr->protocol
				&& MMA_IP_PROTOCOL_UDP != packet_info_ptr->protocol
				&& MMA_IP_PROTOCOL_DCCP != packet_info_ptr->protocol
				&& MMA_IP_PROTOCOL_IPINIP != packet_info_ptr->protocol
				&& MMA_IP_PROTOCOL_ESP != packet_info_ptr->protocol
				&& MMA_IP_PROTOCOL_AH != packet_info_ptr->protocol
				&& MMA_IP_PROTOCOL_MTP != packet_info_ptr->protocol
				&& MMA_IP_PROTOCOL_UDPLITE != packet_info_ptr->protocol
				&& MMA_IP_PROTOCOL_SCTP != packet_info_ptr->protocol
				&& MMA_IP_PROTOCOL_PGM != packet_info_ptr->protocol
				&& MMA_IP_PROTOCOL_RDP != packet_info_ptr->protocol
				)
			{
				if(NULLPTR != block_debug_info)
				{
					block_debug_info->unmatch_cause[pf_current_num++] = MMA_TFT_UNMATCH_CAUSE_REMOTE_PORT;
				}
				tft_tbl_ptr = tft_tbl_ptr->next_ptr;
				continue;
			}
			/* note: dst in pdp means local */
			if (!(packet_info_ptr->dest_port <= packet_filter_ptr->remote_port_high
				&& packet_info_ptr->dest_port >= packet_filter_ptr->remote_port_low))
			{
				if(NULLPTR != block_debug_info)
				{
					block_debug_info->unmatch_cause[pf_current_num++] = MMA_TFT_UNMATCH_CAUSE_REMOTE_PORT;
				}
				tft_tbl_ptr = tft_tbl_ptr->next_ptr;
				continue;
			}
		}

		if (packet_filter_ptr->tft_type & MMA_TFT_OPTION_SPI)
		{
			if (MMA_IP_PROTOCOL_ESP == packet_info_ptr->protocol || 
				MMA_IP_PROTOCOL_AH == packet_info_ptr->protocol)
			{
				if (packet_info_ptr->spi != packet_filter_ptr->spi)
				{
					if(NULLPTR != block_debug_info)
					{
						block_debug_info->unmatch_cause[pf_current_num++] = MMA_TFT_UNMATCH_CAUSE_SPI;
					}
					tft_tbl_ptr = tft_tbl_ptr->next_ptr;
					continue;
				}
			}
		}
		if ((MMA_PDP_IPV6_TYPE == packet_info_ptr->pdp_type)&&
			 (packet_filter_ptr->tft_type & MMA_TFT_OPTION_FLOW_LABEL))
		{
			if (packet_info_ptr->flow_label != packet_filter_ptr->flow_label)
			{
				if(NULLPTR != block_debug_info)
				{
					block_debug_info->unmatch_cause[pf_current_num++] = MMA_TFT_UNMATCH_CAUSE_FLOW_LABEL;
				}
				tft_tbl_ptr = tft_tbl_ptr->next_ptr;
				continue;
			}
		}
		return bear_id;/* 当某个filter与传进来的第[i]个packet 的包头信息相匹配的话，上述的若干个if语句就都不会跳转到continue，会一直执行到这里将实现存下来的filter的bear_id return出去 */
	}while(NULLPTR!=tft_tbl_ptr);   
	return 0xff;/* 当映射表中的这几个filter都没有与 第[i]个packet 的包头信息相匹配的话，将0xff return */
}

Osa_status tft_process(Tpdu_data_req* pdu_data_ptr, Ttft_tbl* tft_tbl_ptr, Ttft_debug_info* tft_debug_info, Uint8 run_mode)
{
	Uint8 i = 0, pkts_num = 0;/* 从502行到518行的逻辑有待询问？？  */
	if(NULLPTR == pdu_data_ptr)
	{
		return OSA_SUCCESS;  /*  ??? 为什么传进来无效的packet地址指针会返回success？？？？？ */
	}
	pkts_num = pdu_data_ptr->pkts_num;
	if(NULLPTR == tft_tbl_ptr)
	{
		for(i=0;i<pdu_data_ptr->pkts_num;i++)
		{
			if(1 == run_mode && NULLPTR != tft_debug_info)
			{
				tft_debug_info->block_debug_info[i].unmatch_cause[0] = MMA_TFT_UNMATCH_CAUSE_NULL_POINTER; 
			}
			pdu_data_ptr->pkt_info[i].rb_id = 0xff;
		}
		return OSA_SUCCESS;  /*  ??? 为什么传进来无效的映射表地址指针会返回success？？？？？ */
	}
	for(i=0;i<pdu_data_ptr->pkts_num;i++)
	{
		Tpacket_info temp_packet_info;
		if(OSA_SUCCESS == get_packet_info_from_ip_pack(pdu_data_ptr->pkt_info[i].w_buffer_ptr+pdu_data_ptr->pkt_info[i].w_data_offset,&temp_packet_info, (1 == run_mode && tft_debug_info != NULL) ? &tft_debug_info->block_debug_info[i] : NULL))
		{
			pdu_data_ptr->pkt_info[i].rb_id = tft_matched(tft_tbl_ptr,&temp_packet_info, (1 == run_mode && tft_debug_info != NULL) ? &tft_debug_info->block_debug_info[i] : NULL);
		}
		else
		{
			pdu_data_ptr->pkt_info[i].rb_id = 0xff;
		}
	}
	return OSA_SUCCESS;
}
