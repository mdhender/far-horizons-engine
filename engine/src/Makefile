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

Jump_o = Jump.o utils.o parse.o do_jump.o get_transact.o do_move.o do_vis.o get_gal.o get_star.o get_plan.o sav_transact.o sav_star.o sav_plan.o get_loc.o get_ship.o gam_abo.o dis_ship.o do_worm.o abend.o
Jump: $(Jump_o)
	cc -o ../bin/Jump $(CFLAGS) $(Jump_o)

Locations_o = Locations.o utils.o do_locs.o get_gal.o get_plan.o get_star.o sav_plan.o abend.o
Locations: $(Locations_o)
	cc -o ../bin/Locations $(CFLAGS) $(Locations_o)

NoOrders_o = NoOrders.o utils.o parse.o get_gal.o get_plan.o get_star.o abend.o
NoOrders: $(NoOrders_o)
	cc -o ../bin/NoOrders $(CFLAGS) $(NoOrders_o)

Stats_o = Stats.o get_gal.o get_star.o get_plan.o utils.o combat_utils.o abend.o
Stats: $(Stats_o)
	cc -o ../bin/Stats $(CFLAGS) $(Stats_o)

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

Jump.o: Jump.c fh.h
	cc $(CFLAGS) -c Jump.c

Locations.o: Locations.c fh.h
	cc $(CFLAGS) -c Locations.c

NoOrders.o: NoOrders.c fh.h
	cc $(CFLAGS) -c NoOrders.c

Stats.o: Stats.c fh.h
	cc $(CFLAGS) -c Stats.c

abend.o: abend.c fh.h
	cc $(CFLAGS) -c abend.c

dat2json.o: dat2json.c fh.h
	cc $(CFLAGS) -DTHIS_IS_MAIN -c dat2json.c

dis_ship.o: dis_ship.c fh.h
	cc $(CFLAGS) -c dis_ship.c

do_jump.o: do_jump.c fh.h
	cc $(CFLAGS) -c do_jump.c

do_move.o: do_move.c fh.h
	cc $(CFLAGS) -c do_move.c

do_name.o: do_name.c fh.h
	cc $(CFLAGS) -c do_name.c

do_rename.o: do_rename.c fh.h
	cc $(CFLAGS) -c do_rename.c

do_worm.o: do_worm.c fh.h
	cc $(CFLAGS) -c do_worm.c

do_vis.o: do_vis.c fh.h
	cc $(CFLAGS) -c do_vis.c

gam_abo.o: gam_abo.c fh.h
	cc $(CFLAGS) -c gam_abo.c

get_gal.o: get_gal.c fh.h
	cc $(CFLAGS) -c get_gal.c

get_loc.o: get_loc.c fh.h
	cc $(CFLAGS) -c get_loc.c

get_plan.o: get_plan.c fh.h
	cc $(CFLAGS) -c get_plan.c

get_ship.o: get_ship.c fh.h
	cc $(CFLAGS) -c get_ship.c

get_spec.o: get_spec.c fh.h
	cc $(CFLAGS) -c get_spec.c

get_star.o: get_star.c fh.h
	cc $(CFLAGS) -c get_star.c

get_transact.o: get_transact.c fh.h
	cc $(CFLAGS) -c get_transact.c

io.o: io.c fh.h
	cc $(CFLAGS) -c io.c

parse.o: parse.c fh.h
	cc $(CFLAGS) -c parse.c

sav_plan.o: sav_plan.c fh.h
	cc $(CFLAGS) -c sav_plan.c

sav_star.o: sav_star.c fh.h
	cc $(CFLAGS) -c sav_star.c

sav_transact.o: sav_transact.c fh.h
	cc $(CFLAGS) -c sav_transact.c

sizes.o: sizes.c fh.h
	cc $(CFLAGS) -c sizes.c

utils.o: utils.c fh.h
	cc $(CFLAGS) -c utils.c

############################################################################
.PHONY: clean check
clean:
	rm -f $(AsciiToBinary_o) $(BinaryToAscii_o) $(Jump_o) $(Locations_o) $(NoOrders_o) $(Stats_o) \
		$(dat2json_o) $(sizes_o)
check:
	../bin/sizes
