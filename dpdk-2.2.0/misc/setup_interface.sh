#!/bin/bash

#ifconfig eth2 down
#modprobe uio_pci_generic
#echo 1024 > /sys/kernel/mm/hugepages-2048kB/nr_hugepages
#mkdir /mnt/huge
#mount -t hugetlbfs nodev /mnt/huge
../tools/dpdk_nic_bind.py --status
#../tools/dpdk_nic_bind.py --bind=uio_pci_generic eth2
../tools/dpdk_nic_bind.py --bind=uio_pci_generic 05:00.1
insmod ../x86_64-native-linuxapp-gcc/kmod/rte_kni.ko

