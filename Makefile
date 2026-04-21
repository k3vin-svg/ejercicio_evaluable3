CC = gcc
RPCGEN = rpcgen

CPPFLAGS = -Iinclude -I. -I/usr/include/tirpc -D_REENTRANT
CFLAGS = -Wall -Wextra -g -fPIC
LDLIBS = -ltirpc -lpthread

LIB = libclaves.so
CLIENT = app-cliente
SERVER = clavesRPC_server
RPCSRC = clavesRPC.x

RPC_GEN = clavesRPC.h clavesRPC_xdr.c clavesRPC_clnt.c clavesRPC_svc.c

LIB_OBJS = src/proxy-rpc.o clavesRPC_clnt.o clavesRPC_xdr.o
CLIENT_OBJS = src/app-cliente.o
SERVER_OBJS = src/server-rpc.o clavesRPC_svc.o clavesRPC_xdr.o src/claves.o

all: $(LIB) $(CLIENT) $(SERVER)

$(LIB): $(LIB_OBJS)
	$(CC) -shared -o $@ $(LIB_OBJS) $(LDLIBS)

$(CLIENT): $(CLIENT_OBJS) $(LIB)
	$(CC) -o $@ $(CLIENT_OBJS) -L. -lclaves -Wl,-rpath,. $(LDLIBS)

$(SERVER): $(SERVER_OBJS)
	$(CC) -o $@ $(SERVER_OBJS) $(LDLIBS)

regen-rpc:
	$(RPCGEN) -aNM $(RPCSRC)

clavesRPC_clnt.o: clavesRPC.h
clavesRPC_svc.o: clavesRPC.h
clavesRPC_xdr.o: clavesRPC.h
src/server-rpc.o: clavesRPC.h include/claves.h
src/proxy-rpc.o: include/claves.h clavesRPC.h
src/app-cliente.o: include/claves.h
src/claves.o: include/claves.h

clean:
	rm -f *.o src/*.o $(LIB) $(CLIENT) $(SERVER)

distclean: clean
	rm -f clavesRPC.h clavesRPC_xdr.c clavesRPC_clnt.c clavesRPC_svc.c clavesRPC_client.c clavesRPC_server.c