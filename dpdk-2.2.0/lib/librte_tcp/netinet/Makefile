CFLAGS+= -fPIC -shared -I./ -I.. -ggdb -I../port -I../opt  -D_KERNEL \
	-z muldefs -D_INET -D_SOCKBUF_DEBUG -D__NO_STRICT_ALIGNMENT \
	-DBYTE_ORDER=LITTLE_ENDIAN -Ofast
	# -DHW_CSUM -Ofast

#CFLAGS_HWIF+=-fPIC -shared -I./ -I../sys -ggdb
OBJS=	accf_data.o cpu_in_cksum.o if_arp.o in.o in_cksum.o in_offload.o in_pcb.o \
	tcp_congctl.o tcp_subr.o tcp_input.o tcp_output.o ip_output.o \
	ip_input.o in_pcb.o ip_encap.o tcp_debug.o tcp_timer.o in.o \
	in_proto.o ip_etherip.o ip_mroute.o tcp_usrreq.o if_arp.o \
	in_selsrc.o ip_flow.o \
	tcp_sack.o in4_cksum.o raw_ip.o ip_icmp.o ip_id.o \
	udp_usrreq.o sockintf.o netbsd_sockutil.o \

OBJS += uipc_socket2.o uipc_socket.o

	
#	udp_usrreq.c \


#ip_enc.o ip_id.o if_atm.c in_gif.c ip_carp.c

# OBJS_HWIF=hwif.c hwif_pfpacket.c
# TEST_BINARY=test

SHARED_LIB=libnetinet.so
#SHARED_LIB_HWIF=libhwif.so

$(SHARED_LIB): ${OBJS} $(OBJS_HWIF)
	$(CC) -o $@ $^ $(CFLAGS)

#$(SHARED_LIB_HWIF): $(OBJS_HWIF)
#	$(CC) -o $@ $^ $(CFLAGS_HWIF)

#all: $(SHARED_LIB) $(SHARED_LIB_HWIF) $(TEST_BINARY)
# all: $(SHARED_LIB) $(TEST_BINARY)

# $(TEST_BINARY):
#	$(CC) -o test -ggdb -I./ -I../sys -I../net -I../altq test.c -lrt -lnet -L./
install:
	install ${SHARED_LIB} /usr/local/lib

clean:
	$(RM) *.o *.so test

