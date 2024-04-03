#ifndef SMCPP_TABLES_HPP_
#define SMCPP_TABLES_HPP_

#include "SMCPP_Core.hpp"


/*
forward_table set_dmac 10.0.1.1 => 08:11:11:11:11:08 0b00000001
forward_table set_dmac 10.0.1.2 => 08:22:22:22:22:08 0b00000100
forward_table set_dmac 10.0.1.3 => 08:33:33:33:33:08 0b00010000
forward_table set_dmac 10.0.1.4 => 08:44:44:44:44:08 0b01000000
*/
namespace table_forward {
	static const ap_uint<48> mac[] =
	{					//   MAC Address      | Port
		0x081111111108, // 08:11:11:11:11:08  -> nf0
		0x082222222208, // 08:22:22:22:22:08  -> nf1
		0x083333333308, // 08:33:33:33:33:08  -> nf2
		0x084444444408  // 08:44:44:44:44:08  -> nf3
	};

	static const ap_uint<48> ip[] =
	{		
		0x0a000101, // 10.0.1.1
		0x0a000102, // 10.0.1.2
		0x0a000103, // 10.0.1.3
		0x0a000104  // 10.0.1.4
	};

	static void action_set_dmac(ap_uint<32> &ip, ap_uint<48> &mac, ap_uint<8> &port)
	{
		if (ip == table_forward::ip[0]) {
			mac = table_forward::mac[0];
			port = port::nf0;
		}
		else if (ip == table_forward::ip[1]) {
			mac = table_forward::mac[1];
			port = port::nf1;
		}
		else if (ip == table_forward::ip[2]) {
			mac = table_forward::mac[2];
			port = port::nf2;
		}
		else if (ip == table_forward::ip[3]) {
			mac = table_forward::mac[3];
			port = port::nf3;
		}
		else {
			mac = 0x000000000000;
			port = port::drop;
		}
	};
}

/*
ipv4_nhop set_nhop 10.0.1.1 => 10.0.1.1
ipv4_nhop set_nhop 10.0.1.2 => 10.0.1.2
ipv4_nhop set_nhop 10.0.1.3 => 10.0.1.3
ipv4_nhop set_nhop 10.0.1.4 => 10.0.1.4
*/
namespace table_ipv4_nhop {
	static const ap_uint<48> ip[] =
	{
		0x0a000101, // 10.0.1.1
		0x0a000102, // 10.0.1.2
		0x0a000103, // 10.0.1.3
		0x0a000104  // 10.0.1.4
	};

	static void action_set_nhop(ap_uint<32> &ip, ap_uint<32> &nhop)
	{
		if (ip == table_ipv4_nhop::ip[0])
			nhop = table_ipv4_nhop::ip[0];
		else if (mac == table_ipv4_nhop::ip[1])
			nhop = table_ipv4_nhop::ip[1];
		else if (mac == table_ipv4_nhop::ip[2])
			nhop = table_ipv4_nhop::ip[2];
		else if (mac == table_ipv4_nhop::ip[3])
			nhop = table_ipv4_nhop::ip[3];
		else
			nhop = 0x0000;
	};
}


/*
send_frame set_smac 0b00000001 => 05:11:11:11:11:05
send_frame set_smac 0b00000100 => 05:22:22:22:22:05
send_frame set_smac 0b00010000 => 05:33:33:33:33:05
send_frame set_smac 0b01000000 => 05:44:44:44:44:05
*/
namespace table_send_frame {
	static const ap_uint<48> mac[] =
	{					//   MAC Address      | Port
		0x081111111108, // 08:11:11:11:11:08  -> nf0
		0x082222222208, // 08:22:22:22:22:08  -> nf1
		0x083333333308, // 08:33:33:33:33:08  -> nf2
		0x084444444408  // 08:44:44:44:44:08  -> nf3
	};

	static void action_set_smac(ap_uint<8> &port, ap_uint<48> &smac)
	{
		if (port == port::nf0)
			smac = table_send_frame::mac[0];
		else if (port == port::nf1)
			smac = table_send_frame::mac[1];
		else if (port == port::nf2)
			smac = table_send_frame::mac[2];
		else if (port == port::nf3)
			smac = table_send_frame::mac[3];
		else
			smac = 0x000000000000;
	};
}

#endif
