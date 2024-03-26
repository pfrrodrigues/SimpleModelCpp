#!/usr/bin/env python2

#
# Copyright (c) 2017 Stephen Ibanez
# All rights reserved.
#
# This software was developed by Stanford University and the University of Cambridge Computer Laboratory 
# under National Science Foundation under Grant No. CNS-0855268,
# the University of Cambridge Computer Laboratory under EPSRC INTERNET Project EP/H040536/1 and
# by the University of Cambridge Computer Laboratory under DARPA/AFRL contract FA8750-11-C-0249 ("MRC2"), 
# as part of the DARPA MRC research programme.
#
# @NETFPGA_LICENSE_HEADER_START@
#
# Licensed to NetFPGA C.I.C. (NetFPGA) under one or more contributor
# license agreements.  See the NOTICE file distributed with this work for
# additional information regarding copyright ownership.  NetFPGA licenses this
# file to you under the NetFPGA Hardware-Software License, Version 1.0 (the
# "License"); you may not use this file except in compliance with the
# License.  You may obtain a copy of the License at:
#
#   http://www.netfpga-cic.org
#
# Unless required by applicable law or agreed to in writing, Work distributed
# under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
# CONDITIONS OF ANY KIND, either express or implied.  See the License for the
# specific language governing permissions and limitations under the License.
#
# @NETFPGA_LICENSE_HEADER_END@
#


from cpp_sss_headers import *
from nf_sim_tools import *
import random
from collections import OrderedDict
import sss_sdnet_tuples

DEF_PKT_SIZE = 256  # default packet size (in bytes)
HEADER_SIZE = 42    # size of Ether/IP/UDP headers
DEF_PKT_NUM = 20    # default number of packets to simulation

########################
# pkt generation tools #
########################

pktsApplied = []
pktsExpected = []

# Pkt lists for SUME simulations
nf_applied = OrderedDict()
nf_applied[0] = []
nf_applied[1] = []
nf_applied[2] = []
nf_applied[3] = []
nf_expected = OrderedDict()
nf_expected[0] = []
nf_expected[1] = []
nf_expected[2] = []
nf_expected[3] = []

nf_port_map = {"nf0":0b00000001, "nf1":0b00000100, "nf2":0b00010000, "nf3":0b01000000, "dma0":0b00000010}
nf_id_map = {"nf0":0, "nf1":1, "nf2":2, "nf3":3}
inv_nf_id_map = {0:"nf0", 1:"nf1", 2:"nf2", 3:"nf3"}
dst_host_map = {0:1, 1:0, 2:3, 3:2}

sss_sdnet_tuples.clear_tuple_files()

def applyPkt(pkt, ingress, time):
    pktsApplied.append(pkt)
    sss_sdnet_tuples.sume_tuple_in['src_port'] = nf_port_map[ingress]
    sss_sdnet_tuples.sume_tuple_expect['src_port'] = nf_port_map[ingress]
    pkt.time = time
    nf_applied[nf_id_map[ingress]].append(pkt)

def expPkt(pkt, egress):
    pktsExpected.append(pkt)
    sss_sdnet_tuples.sume_tuple_expect['dst_port'] = nf_port_map[egress]
    sss_sdnet_tuples.write_tuples()
    if egress in ["nf0","nf1","nf2","nf3"]:
        nf_expected[nf_id_map[egress]].append(pkt)
    elif egress == 'bcast':
        nf_expected[0].append(pkt)
        nf_expected[1].append(pkt)
        nf_expected[2].append(pkt)
        nf_expected[3].append(pkt)

def print_summary(pkts):
    for pkt in pkts:
        print "summary = ", pkt.summary()

def write_pcap_files():
    wrpcap("src.pcap", pktsApplied)
    wrpcap("dst.pcap", pktsExpected)

    for i in nf_applied.keys():
        if (len(nf_applied[i]) > 0):
            wrpcap('nf{0}_applied.pcap'.format(i), nf_applied[i])

    for i in nf_expected.keys():
        if (len(nf_expected[i]) > 0):
            wrpcap('nf{0}_expected.pcap'.format(i), nf_expected[i])

    for i in nf_applied.keys():
        print "nf{0}_applied times: ".format(i), [p.time for p in nf_applied[i]]

#####################
# generate testdata #
#####################

MAC_addr_H = {}
MAC_addr_H[nf_id_map["nf0"]] = "08:11:11:11:11:08"
MAC_addr_H[nf_id_map["nf1"]] = "08:22:22:22:22:08"
MAC_addr_H[nf_id_map["nf2"]] = "08:33:33:33:33:08"
MAC_addr_H[nf_id_map["nf3"]] = "08:44:44:44:44:08"

IP_addr_H = {}
IP_addr_H[nf_id_map["nf0"]] = "10.0.1.0"
IP_addr_H[nf_id_map["nf1"]] = "10.0.1.1"
IP_addr_H[nf_id_map["nf2"]] = "10.0.1.2"
IP_addr_H[nf_id_map["nf3"]] = "10.0.1.3"

pktCnt = 0

# DEF_PKT_NUM = 4

# Test the addition functionality
def create_pkt():
    # Create a packet to test the addition operation using OP1 and OP2
    # and apply it to the switch. Also create the expected packet and
    # indicate it should be expected on a particular interface.
    global pktCnt
    src_index = 0

    for time in range(DEF_PKT_NUM):
        src_MAC = MAC_addr_H[src_index]
        dst_MAC = MAC_addr_H[dst_host_map[src_index]]
        src_IP = IP_addr_H[src_index]
        dst_IP = IP_addr_H[dst_host_map[src_index]]

        pkt = Ether(dst=dst_MAC, src=src_MAC, type=0x0800) / \
              IP(src=src_IP, dst=dst_IP, ttl=20) / \
              UDP(sport=5056, dport=2021) / \
              ((DEF_PKT_SIZE-HEADER_SIZE)*"A")
        pkt = pad_pkt(pkt, DEF_PKT_SIZE)
        applyPkt(pkt, inv_nf_id_map[src_index], time)

        pkt = Ether(dst=dst_MAC, src=src_MAC, type=0x0800) / \
              IP(src=src_IP, dst=dst_IP, ttl=20) / \
              UDP(sport=5056, dport=2021) / \
              ((DEF_PKT_SIZE-HEADER_SIZE)*"A")
        pkt = pad_pkt(pkt, DEF_PKT_SIZE)
        expPkt(pkt, inv_nf_id_map[dst_host_map[src_index]])

        if (src_index == 3):
            src_index = 0
        else:
            src_index += 1


# while op1 < op2:
#     op2 = random.randint(0,2**31)
create_pkt()
# test_sub(op1, op2)
# test_lookup(op1)
# test_reg(op1, op2)

write_pcap_files()

