CFLAGS+= -fPIC -shared -I./ -I.. -ggdb -I../port -I../opt -I../udvr \
	-D__NO_STRICT_ALIGNMENT -DBYTE_ORDER=LITTLE_ENDIAN # -O1
#CFLAGS_HWIF+=-fPIC -shared -I./ -I../sys -ggdb
OBJS=	radix.o  \
	if.o \
	netbsd_callout.o \
	rtsock.o \
	route.o \
	uipc_mbuf.o \
	uipc_mbuf2.o \
	uipc_domain.o \
	link_proto.o \
	raw_cb.o \
	raw_usrreq.o \
	bpf_filter.o \
	if_loop.o \
	if_ethersubr.o \
	if_etherip.o \
	if_media.o \
	pfil.o \
	net_stats.o \
	if_ecosubr.o \
	if_arcsubr.o \
	if_bridge.o \
	bridgestp.o \
	bsd-comp.o \
	subr_hash.o \
	util.o \
	netbsd_mpool.o \

#	if_atmsubr.o \
#	 zlib.o \

# OBJS_HWIF=hwif.c hwif_pfpacket.c
# TEST_BINARY=test

#	if.o \
#	if_ethersubr.o \
#	if_loop.o \
#	if_media.o \
#	if_vlan.o \
#	link_proto.o \
#	net_stats.o \
#	radix.o \
#	raw_cb.o \
#	raw_usrreq.o \
#	route.o \
#	rtsock.o \
#	zlib.o 

SHARED_LIB=libnet.so
#SHARED_LIB_HWIF=libhwif.so

#$(SHARED_LIB): ${OBJS} $(OBJS_HWIF)
$(SHARED_LIB): ${OBJS} 
	$(CC) -o $@ $^ $(CFLAGS) -L../udvr -lhwif

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

