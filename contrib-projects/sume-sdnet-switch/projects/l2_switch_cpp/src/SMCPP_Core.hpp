#ifndef SMCPP_CORE_HPP_
#define SMCPP_CORE_HPP_

#include <ap_int.h>

	typedef ap_uint<48> EthAddr_t;

	#pragma pack(push,1)

	/** Ethernet header description **/
	struct Ethernet_h {
		EthAddr_t dstAddr;
		EthAddr_t srcAddr;
		ap_uint<16> etherType;
	};

	struct Vlan_h {
		ap_uint<3> prio;
		ap_uint<1> dropEligible;
		ap_uint<12> vlanId;
		ap_uint<16> tpid;
	};

	struct ParsedPacket_t {
		Ethernet_h ethernet;
	};

	struct DigestData_t {
		ap_uint<1> vld_signal;
		ap_uint<256> data;
	};

	template<unsigned D>
	struct axi_stream_u {
		ap_uint<D>   TDATA;
		ap_uint<D/8> TKEEP;
		ap_uint<1>   TLAST;
		ap_uint<1>   TVALID;
		ap_uint<1>   TREADY;
	};

	#pragma pack(pop)

	enum State {
		IDLE = 	    0,
		START =     1,
		ADMIT =     2,
		REJECT =    3,
		TRANSPASS = 4
	};

	namespace port {
		/* source and destination ports format:
		 * bit-7     bit-6     bit-5     bit-4     bit-3     bit-2     bit-1     bit-0
		(nf3_dma)-(nf3_phy)-(nf2_dma)-(nf2_phy)-(nf1_dma)-(nf1_phy)-(nf0_dma)-(nf0_phy) */
		static const ap_uint<8> nf0 = 0b00000001;
		static const ap_uint<8> nf1 = 0b00000100;
		static const ap_uint<8> nf2 = 0b00010000;
		static const ap_uint<8> nf3 = 0b01000000;
		static const ap_uint<8> drop = 0b00000000;
	}


#endif
