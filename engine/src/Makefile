all: AsciiToBinary BinaryToAscii Locations dat2json sizes
	echo "make all ran successfully"

# uncrustify -c ~/fhc/.uncrustify.cfg Locations.c
# mv Locations.c.uncrustify Locations.c

############################################################################
CFLAGS=-Wno-format-contains-nul
LDFLAGS=

############################################################################
AsciiToBinary_o = AsciiToBinary.o abend.o get_star.o get_plan.o sav_star.o sav_plan.o utils.o
AsciiToBinary: $(AsciiToBinary_o)
	cc -o ../bin/AsciiToBinary $(CFLAGS) $(AsciiToBinary_o)

BinaryToAscii_o = BinaryToAscii.o abend.o get_gal.o get_star.o get_plan.o utils.o
BinaryToAscii: $(BinaryToAscii_o)
	cc -o ../bin/BinaryToAscii $(CFLAGS) $(BinaryToAscii_o)

Locations_o = Locations.o utils.o do_locs.o get_gal.o get_plan.o get_star.o sav_plan.o abend.o
Locations: $(Locations_o)
	cc -o ../bin/Locations $(CFLAGS) $(Locations_o)

dat2json_o = dat2json.o abend.o io.o get_gal.o get_star.o get_plan.o get_spec.o
dat2json: $(dat2json_o)
	cc -o ../bin/dat2json $(CFLAGS) $(dat2json_o)

sizes_o = sizes.o
sizes: $(sizes_o)
	cc -o ../bin/sizes $(CFLAGS) $(sizes_o)

############################################################################
AsciiToBinary.o: AsciiToBinary.c fh.h
	cc $(CFLAGS) -c AsciiToBinary.c

BinaryToAscii.o: BinaryToAscii.c fh.h
	cc $(CFLAGS) -c BinaryToAscii.c

Locations.o: Locations.c fh.h
	cc $(CFLAGS) -c Locations.c

abend.o: abend.c fh.h
	cc $(CFLAGS) -c abend.c

dat2json.o: dat2json.c fh.h
	cc $(CFLAGS) -DTHIS_IS_MAIN -c dat2json.c

get_gal.o: get_gal.c fh.h
	cc $(CFLAGS) -c get_gal.c

get_plan.o: get_plan.c fh.h
	cc $(CFLAGS) -c get_plan.c

get_spec.o: get_spec.c fh.h
	cc $(CFLAGS) -c get_spec.c

get_star.o: get_star.c fh.h
	cc $(CFLAGS) -c get_star.c

io.o: io.c fh.h
	cc $(CFLAGS) -c io.c

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
	rm -f $(AsciiToBinary_o) $(BinaryToAscii_o) \
		$(dat2json_o) $(sizes_o)
check:
	../bin/sizes
