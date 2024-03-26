

    Copyright (c) 2024 Pablo Rodrigues
    All rights reserved.

    This software was developed by Institute of Informatics of the Federal University of Rio Grande do Sul (INF-UFRGS)
    as part of the bachelor's degree final project.
   
    @NETFPGA_LICENSE_HEADER_START@
   
    Licensed to NetFPGA C.I.C. (NetFPGA) under one or more contributor
    license agreements.  See the NOTICE file distributed with this work for
    additional information regarding copyright ownership.  NetFPGA licenses this
    file to you under the NetFPGA Hardware-Software License, Version 1.0 (the
    "License"); you may not use this file except in compliance with the
    License.  You may obtain a copy of the License at:
   
      http://www.netfpga-cic.org
   
    Unless required by applicable law or agreed to in writing, Work distributed
    under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
    CONDITIONS OF ANY KIND, either express or implied.  See the License for the
    specific language governing permissions and limitations under the License.
   
    @NETFPGA_LICENSE_HEADER_END@


## SimpleModelCpp

### System Setup (Ubuntu 20.04)

- Install basic packets:

```console
sudo apt install build-essential libglvnd-dev pkg-config -y
sudo apt install vim -y
```

- Install Haguiche and Hamachi for remote access:

```console
sudo add-apt-repository -y ppa:ztefn/haguichi-stable
sudo apt update
sudo apt install -y haguichi
wget https://www.vpn.net/installers/logmein-hamachi_2.1.0.203-1_amd64.deb
sudo apt install ./logmein-hamachi_2.1.0.203-1_amd64.deb
```

Log into the Haguiche network:

```console
Network Name: LanUFRGSInternal
Password: LanUFRGSInternal
```

- Setup package dependence:

```console
sudo apt-get install gcc-multilib
sudo apt-get install curl
sudo apt install python2
curl https://bootstrap.pypa.io/pip/2.7/get-pip.py -o get-pip.py && python2 get-pip.py
pip2 install scapy==2.2.0-dev
```

- Download and install Vivado 2018.2 and SDNet:

```console
https://ufrgscpd-my.sharepoint.com/:f:/g/personal/00298445_ufrgs_br/EsNtPC0BBv5OiukEBr7eyZcBfDoSNQrmy06SL6CqTyfhXg?e=KpsKWh
```

- Setup License :

```console
echo "" >> ~/.bashrc
echo "export $SUME_SERVER_IP_HAGUICHI=25.58.93.<something>" >> ~/.bashrc
echo "source /tools/Xilinx/Vivado/2018.2/settings64.sh" >> ~/.bashrc
echo "export PATH=${PATH}:/tools/Xilinx/Vivado/2018.2/bin/unwrapped/lnx64.o" >> ~/.bashrc
echo "" >> ~/.bashrc
echo "export XILINXD_LICENSE_FILE=2100@${SUME_SERVER_IP_HAGUICHI}" >> ~/.bashrc
echo "export LM_LICENSE_FILE=2100@${SUME_SERVER_IP_HAGUICHI}" >> ~/.bashrc
```

- Clone this repo:

```console
git clone https://github.com/pfrrodrigues/SimpleModelCpp.git
```

- Setup Project variables (first fix the **SUME_FOLDER** path if needed):

```console
cd SimpleModelCpp/tools
source settings.sh
```

- Build basic libraries:

```console
./make_library.sh
```

- Build Project:

```console
cd ../contrib-projects/sume-sdnet-switch/projects/l2_switch_cpp/
make
make install
```

- Run simulation:

```console
cd ${SUME_FOLDER}/tools/scripts
./nf_test.py sim --major switch --minor default
```

- Generate Bitstream:

```console
cd $NF_DESIGN_DIR && make
```

The Makefile calls `${NF_DESIGN_DIR}/tcl/simple_sume_switch.tcl` that creates a Vivado project folder: `${NF_DESIGN_DIR}/hw/project/`