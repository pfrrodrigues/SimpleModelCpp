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
			   ap_uint<1> 		 &internal_rst_done)
{
#pragma HLS INTERFACE ap_ctrl_none port=return
#pragma HLS INTERFACE ap_none port=s_axis
#pragma HLS INTERFACE ap_none port=m_axis
#pragma HLS INTERFACE ap_none port=enable_processing
#pragma HLS INTERFACE ap_none port=tuple_in_sume_metadata_VALID
#pragma HLS INTERFACE ap_none port=tuple_in_sume_metadata_DATA
#pragma HLS INTERFACE ap_none port=tuple_out_sume_metadata_VALID
#pragma HLS INTERFACE ap_none port=tuple_out_sume_metadata_DATA
#pragma HLS INTERFACE ap_none port=tuple_out_digest_data_VALID
#pragma HLS INTERFACE ap_none port=tuple_out_digest_data_DATA
#pragma HLS INTERFACE s_axilite port=control_S_AXI_AWADDR clock=clk_control
#pragma HLS INTERFACE ap_none port=clk_lookup
#pragma HLS INTERFACE ap_none port=clk_lookup_rst
#pragma HLS INTERFACE ap_none port=internal_rst_done

	// AXI4-Stream with side-channels control
	internal_rst_done = 0;
	m_axis.TKEEP = s_axis.TKEEP;
	m_axis.TLAST = s_axis.TLAST;
	State state = State::IDLE;
	ap_uint<1> prior_TVALID = 0;

	if (s_axis.TVALID == 1) {
		// Data received
		m_axis.TVALID = 1;

		ap_uint<256> packet_in = s_axis.TDATA;
		ParsedPacket_t parsed_packet;
		DigestData_t digest_data;
		if (prior_TVALID == 0) {
			// Detects rising edges in order to start processing a new packet
			state = State::START;

			/*****************************************
							PARSER
			 *****************************************/
			Parser(state, packet_in, parsed_packet, digest_data);
			tuple_out_digest_data_VALID = digest_data.vld_signal;
			tuple_out_digest_data_DATA = digest_data.data;

			/*****************************************
			 *				PIPELINE
			 *****************************************/
			 Pipeline(state, parsed_packet, tuple_in_sume_metadata_VALID, tuple_in_sume_metadata_DATA, tuple_out_sume_metadata_VALID, tuple_out_sume_metadata_DATA);
		}
		else {
			// Runs the data through the stages without processing it
			state = State::TRANSPASS;
		}

		if (m_axis.TREADY) {
			// Transfers the data to the output port (emits the outgoing packet)

			/*****************************************
			*				DEPARSER
			*****************************************/
			Deparser(state, m_axis.TDATA, s_axis.TDATA, parsed_packet);
			s_axis.TREADY = 1;
			state = State::IDLE;
		}
		else {
			// Blocks a new data transfer until the current data is ready to be delivered
			s_axis.TREADY = 0;
		}
	}
	else {
		// Signals to indicate the module is ready to receive new data
		m_axis.TVALID = 0;
		s_axis.TREADY = 1;
	}

	prior_TVALID = s_axis.TVALID;
}
