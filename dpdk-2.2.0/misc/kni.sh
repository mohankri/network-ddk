./kni -c 0xf -n 4 -d librte_pmd_ixgbe.so -- -p 0x1 -P --config="(0,1,3)"

ifconfig vEth0 172.16.1.251/24 up

