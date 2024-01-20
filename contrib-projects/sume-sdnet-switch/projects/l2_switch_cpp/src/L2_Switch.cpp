#include "SMCPP_Entities.hpp"
#include "Tables.hpp"

#define IPV4_TYPE 0x0800

/*****************************************
	Parser
	description:
 *****************************************/
void Parser(State 			&state,
			ap_uint<256> 	&packet_in,
			ParsedPacket_t 	&parsed_packet,
			DigestData_t 	&digest_data)
{
#pragma INLINE
#pragma HLS INTERFACE ap_ctrl_none port=return
#pragma HLS INTERFACE ap_none port=packet_in
#pragma HLS INTERFACE ap_none port=parsed_packet
#pragma HLS INTERFACE ap_none port=digest_data

	if (state == State::START) {
		parsed_packet.ethernet.dstAddr = packet_in.range(47,0);
		parsed_packet.ethernet.srcAddr = packet_in.range(95, 48);
		parsed_packet.ethernet.etherType = packet_in.range(111, 96);
		digest_data.vld_signal = 1;
		digest_data.data = 0;

		switch (parsed_packet.ethernet.etherType){
		case IPV4_TYPE:
			state = State::ADMIT;
			break;
		default:
			state = State::REJECT;
			break;
		}
	}
	else {
		state = State::REJECT;
	}
}

/*****************************************
	Pipeline
	description:
 *****************************************/
void Pipeline(State 		 &state,
			  ParsedPacket_t &parsed_packet,
			  ap_uint<1> 	 &in_sume_metadata_VALID,
			  ap_uint<128> 	 &in_sume_metadata,
			  ap_uint<1> 	 &out_sume_metadata_VALID,
			  ap_uint<128> 	 &out_sume_metadata)
{
#pragma INLINE
#pragma HLS INTERFACE ap_ctrl_none port=return
#pragma HLS INTERFACE ap_none port=parsed_packet
#pragma HLS INTERFACE ap_none port=state
#pragma HLS INTERFACE ap_none port=in_sume_metadata_VALID
#pragma HLS INTERFACE ap_none port=in_sume_metadata
#pragma HLS INTERFACE ap_none port=out_sume_metadata_VALID
#pragma HLS INTERFACE ap_none port=out_sume_metadata

	if (state == State::ADMIT) {
		ap_uint<8> dst_port;
		forwarding_table::getPort(parsed_packet.ethernet.dstAddr, dst_port); // returns the destination port based on the destination MAC address

		ap_uint<128> tmp_metadata_out = in_sume_metadata;
		tmp_metadata_out.range(31, 24) = (dst_port);
		out_sume_metadata = tmp_metadata_out;
		out_sume_metadata_VALID = in_sume_metadata_VALID;
	}
	else {
		state = State::REJECT;
	}
}

/*****************************************
	Deparser
	description:
 *****************************************/
void Deparser(State 	     &state,
			  ap_uint<256>   &packet_out,
			  ap_uint<256>   &packet_in,
			  ParsedPacket_t &parsed_packet)
{
#pragma INLINE
#pragma HLS INTERFACE ap_ctrl_none port=return
#pragma HLS INTERFACE ap_none port=parsed_packet
#pragma HLS INTERFACE ap_none port=packet_out
#pragma HLS INTERFACE ap_none port=packet_in
	// emit (copy the parsed data in the packet out)
	packet_out = packet_in;
	if (state != State::TRANSPASS) {
		packet_out.range(47, 0) = parsed_packet.ethernet.dstAddr;
		packet_out.range(95, 48) = parsed_packet.ethernet.srcAddr;
		packet_out.range(111, 96) = parsed_packet.ethernet.etherType;
	}
}
