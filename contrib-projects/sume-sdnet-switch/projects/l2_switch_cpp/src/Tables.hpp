#ifndef SMCPP_TABLES_HPP_
#define SMCPP_TABLES_HPP_

#include "SMCPP_Core.hpp"

namespace forwarding_table {
	static const ap_uint<48> mac[] =
	{					//   MAC Address      | Port
		0x081111111108, // 08:11:11:11:11:08  -> nf0
		0x082222222208, // 08:22:22:22:22:08  -> nf1
		0x083333333308, // 08:33:33:33:33:08  -> nf2
		0x084444444408  // 08:44:44:44:44:08  -> nf3
	};

	static void getPort(ap_uint<48> &mac, ap_uint<8> &port)
	{
		if (mac == forwarding_table::mac[0])
			port = port::nf0;
		else if (mac == forwarding_table::mac[1])
			port = port::nf1;
		else if (mac == forwarding_table::mac[2])
			port = port::nf2;
		else if (mac == forwarding_table::mac[3])
			port = port::nf3;
		else
			port = port::drop;
	};
}

#endif
