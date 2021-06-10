all: AsciiToBinary BinaryToAscii sizes
	echo "make all ran successfully"

############################################################################
CFLAGS=-Wno-format-contains-nul
LDFLAGS=

############################################################################
AsciiToBinary_o = AsciiToBinary.o get_star.o get_plan.o sav_star.o sav_plan.o utils.o
AsciiToBinary: $(AsciiToBinary_o)
	cc -o ../bin/AsciiToBinary $(CFLAGS) $(AsciiToBinary_o)

BinaryToAscii_o = BinaryToAscii.o get_gal.o get_star.o get_plan.o utils.o
BinaryToAscii: $(BinaryToAscii_o)
	cc -o ../bin/BinaryToAscii $(CFLAGS) $(BinaryToAscii_o)

sizes_o = sizes.o
sizes: $(sizes_o)
	cc -o ../bin/sizes $(CFLAGS) $(sizes_o)

############################################################################
AsciiToBinary.o: AsciiToBinary.c fh.h
	cc $(CFLAGS) -c AsciiToBinary.c

BinaryToAscii.o: BinaryToAscii.c fh.h
	cc $(CFLAGS) -c BinaryToAscii.c

get_gal.o: get_gal.c fh.h
	cc $(CFLAGS) -c get_gal.c

get_plan.o: get_plan.c fh.h
	cc $(CFLAGS) -c get_plan.c

get_star.o: get_star.c fh.h
	cc $(CFLAGS) -c get_star.c

sav_plan.o: sav_plan.c fh.h
	cc $(CFLAGS) -c sav_plan.c

sav_star.o: sav_star.c fh.h
	cc $(CFLAGS) -c sav_star.c

sizes.o: sizes.c fh.h
	cc $(CFLAGS) -c sizes.c

utils.o: utils.c fh.h
	cc $(CFLAGS) -c utils.c

############################################################################
.PHONY: clean check
clean:
	rm -f $(AsciiToBinary_o) $(BinaryToAscii_o) $(sizes_o)
check:
	../bin/sizes