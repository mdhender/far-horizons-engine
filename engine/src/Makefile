all: sizes
	echo "make all ran successfully"

############################################################################
CFLAGS=-Wno-format-contains-nul
LDFLAGS=

############################################################################
sizes_o = sizes.o
sizes: $(sizes_o)
	cc -o ../bin/sizes $(CFLAGS) $(sizes_o)

############################################################################
sizes.o: sizes.c fh.h
	cc $(CFLAGS) -c sizes.c
