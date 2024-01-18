cd $DRIVER_FOLDER
make all
make install
modprobe sume_riffa
lsmod | grep sume_riffa