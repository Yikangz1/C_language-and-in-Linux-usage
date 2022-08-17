/**
 * \file      TFT.c
 * \author    
 * \copyright (C) 2013 VeriSilicon.  All rights reserved.
 * \date
 * \brief     TFT
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "construct_data.h" 

#if 0
#include "zsp_util.h"

void send_stdout (char *str);


/** Define Host specific (POSIX), or target specific global time variables. */
static unsigned long long start_time_val, stop_time_val;

/* Function : start_time
        This function will be called right before starting the timed portion of
   the benchmark.

        Implementation may be capturing a system timer (as implemented in the
   example code) or zeroing some system parameters - e.g. setting the cpu clocks
   cycles to 0.
*/
void
start_time(void)
{
    start_time_val = ZSP_get_cycle();
}
/* Function : stop_time
        This function will be called right after ending the timed portion of the
   benchmark.

        Implementation may be capturing a system timer (as implemented in the
   example code) or other system parameters - e.g. reading the current value of
   cpu cycles counter.
*/
void
stop_time(void)
{
    stop_time_val = ZSP_get_cycle();;
}
#endif
/*int main(void)函数的功能大致如下：（1）Tpdu_data_req * pdu_data_ptr = makeup_pdu_data()；完成用于存放packets的data memory block的内存分布和packet的构造。    */
/*                                （2）Ttft_tbl *tft_tbl_ptr = makeup_tft_tbl_data();用于映射表的内存分配和映射表的构造*/
/*                                （3）Ttft_debug_info* tft_debug_info_ptr = (Ttft_debug_info*)osa_malloc(sizeof(Ttft_debug_info)); 为结构体Ttft_debug_info分配内存（用来存放debug信息）*/
/*                                （4）process_status = tft_process(pdu_data_ptr,tft_tbl_ptr, tft_debug_info_ptr, 1);将上述三个构造的memory block的地址指针传入tft_process函数，通过 */
/*                                     将data中的packet包头与映射表的各个子表对照后，更新data中各个packet对应的rb_id,并回传一个OSA_SUCCESS   */
/*                                （5）result = check_result(pdu_data_ptr,expect_result);将pdu_data_ptr指针传入该函数寻址pdu_data_ptr->pkt_info[i].rb_id，分别与expect_result[i]对比，从而验证TFT的加速结果是否正确
/*                                （6）free （1-3）中三个指针指向的内存空间 */
int main(void) 
{
	Uint8 process_status = 1;
		Uint8 result = 0;
		Uint8 expect_result[4] = {6,7,8,255};
		Uint8 i = 0;
		//case1
		Tpdu_data_req * pdu_data_ptr = makeup_pdu_data();
		Ttft_tbl *tft_tbl_ptr = makeup_tft_tbl_data();
		Ttft_debug_info* tft_debug_info_ptr = (Ttft_debug_info*)osa_malloc(sizeof(Ttft_debug_info)); 
		//start_time();
		process_status = tft_process(pdu_data_ptr,tft_tbl_ptr, tft_debug_info_ptr, 1);/*  tft_process将三个地址指针传进去，将data中的packet包头与映射表的各个子表对照后，更新data中各个packet对应的rb_id,并回传一个OSA_SUCCESS */
		//stop_time();
		if(OSA_SUCCESS == process_status)
		{

			result = check_result(pdu_data_ptr,expect_result);
			if(OSA_SUCCESS == result)
			{
		//		printf("case1 success,time:%d\n", stop_time_val - start_time_val);
				printf("case1 success,time:\n");
			}
			else
			{
				printf("case1 fail\n");
			}
		}
		else
		{
			printf("case1 fail\n");
		}
		free_tft_tbl_data(tft_tbl_ptr);
		free_pdu_data(pdu_data_ptr);
		osa_free(tft_debug_info_ptr);

		//case2
		pdu_data_ptr = makeup_pdu_data();
		tft_tbl_ptr = NULL;
		tft_debug_info_ptr = (Ttft_debug_info*)osa_malloc(sizeof(Ttft_debug_info));
		//start_time();
		process_status = tft_process(pdu_data_ptr,tft_tbl_ptr, tft_debug_info_ptr, 1);
		//stop_time();
		if(OSA_SUCCESS == process_status)
		{
			for(i=0;i<4;i++)
			{
				expect_result[i] = 0xFF;
			}
			result = check_result(pdu_data_ptr,expect_result);/* */
			if(OSA_SUCCESS == result)
			{
				//printf("case2 success,time:%d\n",stop_time_val-start_time_val);
				printf("case2 success,time:\n");
			}
			else
			{
				printf("case2 fail\n");
			}
		}
		else
		{
			printf("case2 fail\n");
		}
		free_tft_tbl_data(tft_tbl_ptr);
		free_pdu_data(pdu_data_ptr);
		osa_free(tft_debug_info_ptr);

	//	system("pause");/*  暂停程序执行的作用 */
		return 0;
	//send_stdout ("Hello ZSP\n");
	//return EXIT_SUCCESS;
}

/**
 * \brief Function controls how the stdout is sent. This depends on
 * type of the debugger interface used.
 * \param str is the pointer to the ASCIIZ string to be sent out
 */
void send_stdout (char *str) 
{
	fwrite (str, 1, strlen(str), stdout);
}
 
