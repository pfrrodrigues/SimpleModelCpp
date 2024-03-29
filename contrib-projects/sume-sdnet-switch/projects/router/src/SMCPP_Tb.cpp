#include <stdint.h>
#include <iostream>
#include <ap_int.h>
#include <hls_stream.h>
#include <ap_axi_sdata.h>

#include "SMCPP_Entities.hpp"

void SMCPP_Top(axi_stream_u<256> &s_axis,
			   axi_stream_u<256> &m_axis,
			   ap_uint<1> 		 &enable_processing,
			   ap_uint<1> 		 &tuple_in_sume_metadata_VALID,
			   ap_uint<128> 	 &tuple_in_sume_metadata_DATA,
			   ap_uint<1> 		 &tuple_out_sume_metadata_VALID,
			   ap_uint<128> 	 &tuple_out_sume_metadata_DATA,
			   ap_uint<1> 		 &tuple_out_digest_data_VALID,
			   ap_uint<256> 	 &tuple_out_digest_data_DATA,
			   ap_uint<9> 		 &control_S_AXI_AWADDR,
			   ap_uint<1>	 	 &clk_lookup,
			   ap_uint<1> 		 &clk_lookup_rst,
			   ap_uint<1> 		 &internal_rst_done);


int main()
{


	axi_stream_u<256> s_axis_tb;
	s_axis_tb.TDATA.range(47,0) = 0x082222222208;
	s_axis_tb.TDATA.range(95,48) = 0x081111111108;
	s_axis_tb.TDATA.range(111,96) = 0x0800;
	s_axis_tb.TVALID = 1;

	axi_stream_u<256> m_axis_tb;
	m_axis_tb.TREADY = 1;
	m_axis_tb.TLAST = 1;

	ap_uint<1> enable_processing_tb = 1;

	ap_uint<128> in_sume_metadata_tb = 0x00000000000000000000000000010000;
	ap_uint<1> in_sume_metadata_VALID_tb = 1;
	ap_uint<128> out_sume_metadata_tb;
	ap_uint<1> out_sume_metadata_VALID_tb;

	ap_uint<256> digest_data_DATA_tb = 0;
	ap_uint<1> digest_data_VALID_tb = 1;
	ap_uint<9> control_S_AXI_AWADDR_tb = 1;

	ap_uint<1> clk_lookup_tb = 0;
	ap_uint<1> clk_lookup_rst_tb = 0;
	ap_uint<1> internal_rst_done_tb = 0;

	SMCPP_Top(s_axis_tb,
			  m_axis_tb,
			  enable_processing_tb,
			  in_sume_metadata_VALID_tb,
			  in_sume_metadata_tb,
			  out_sume_metadata_VALID_tb,
			  out_sume_metadata_tb,
			  digest_data_VALID_tb,
			  digest_data_DATA_tb,
			  control_S_AXI_AWADDR_tb,
			  clk_lookup_tb,
			  clk_lookup_rst_tb,
			  internal_rst_done_tb
	);

	std::cout << "source MAC: " << m_axis_tb.TDATA.range(95, 48).to_string(16) << std::endl;
	std::cout << "destination MAC: " << s_axis_tb.TDATA.range(47,0).to_string(16) << std::endl;
	std::cout << "etherType: " << s_axis_tb.TDATA.range(111,96).to_string(16) << std::endl;
	std::cout << "tuple_in_sume_metadata_VALID: " << in_sume_metadata_VALID_tb << std::endl;
	std::cout << "tuple_in_sume_metadata_DATA: " << in_sume_metadata_tb.toStringUnsigned(16) << std::endl;
	std::cout << "tuple_out_sume_metadata_VALID: " << out_sume_metadata_VALID_tb << std::endl;
	std::cout << "tuple_out_sume_metadata_DATA: " << out_sume_metadata_tb.toStringUnsigned(16) << std::endl;
	return 0;
}
