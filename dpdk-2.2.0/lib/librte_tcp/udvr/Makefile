CFLAGS+= -fPIC -shared -I./ -I.. -ggdb -I../port -I../opt \
	-I../uld-driver -I../net \
	-D__NO_STRICT_ALIGNMENT -DBYTE_ORDER=LITTLE_ENDIAN -Ofast
OBJS=dpdk_if.o dpdk_net.o dpdk_socket.o

SHARED_LIB=libhwif.so
$(SHARED_LIB): ${OBJS}
	$(CC) -o $@ $^ $(CFLAGS)

install:
	install ${SHARED_LIB} /usr/local/lib

clean:
	-rm *.o $(SHARED_LIB)
	
	
