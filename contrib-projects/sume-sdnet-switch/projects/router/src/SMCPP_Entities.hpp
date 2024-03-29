#ifndef SMCPP_ENTITIES_HPP_
#define SMCPP_ENTITIES_HPP_

#include <ap_int.h>

#include "SMCPP_Core.hpp"

/*****************************************
	Parser
	description:
 *****************************************/
void Parser(State 			&state,
			ap_uint<256> 	&packet_in,
			ParsedPacket_t 	&parsed_packet,
			DigestData_t 	&digest_data);

/*****************************************
	Pipeline
	description:
 *****************************************/
void Pipeline(State 	     &state,
			  ParsedPacket_t &parsedPacket,
			  ap_uint<1> 	 &in_sume_metadata_VALID,
			  ap_uint<128> 	 &in_sume_metadata,
			  ap_uint<1> 	 &out_sume_metadata_VALID,
			  ap_uint<128> 	 &out_sume_metadata);

/*****************************************
	Deparser
	description:
 *****************************************/
void Deparser(State 		 &state,
			  ap_uint<256>   &packet_out,
			  ap_uint<256>   &packet_in,
			  ParsedPacket_t &p);

#endif
