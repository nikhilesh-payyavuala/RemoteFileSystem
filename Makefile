all: target

target: hw2.x client.c server.c

	rpcgen -aN  hw2.x
	cc -c hw2_xdr.c 
	cc client.c hw2_clnt.c hw2_xdr.o -o client -lnsl
	cc server.c hw2_svc.c hw2_xdr.o -o server -lnsl

clean:
	rm *.hw2
	rm hw2_*
	rm hw2.h
	rm client
	rm server