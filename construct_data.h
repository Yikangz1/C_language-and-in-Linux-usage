#ifndef PRODUCE_DATA_H
#define PRODUCE_DATA_H
#include "tft_type.h"

Ttft_tbl * makeup_tft_tbl_data();
int free_tft_tbl_data(Ttft_tbl *tft_tbl_ptr);
Tpdu_data_req * makeup_pdu_data();
int free_pdu_data(Tpdu_data_req * pdu_data_ptr);
Osa_status check_result(Tpdu_data_req* pdu_data_ptr, Uint8* expect_result);

#endifv
