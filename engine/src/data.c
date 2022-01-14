/* Convert from a useful set of data structures to a difference set. */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>

#include "fh.h"

const char *semver = "7.5.2";

typedef struct GalaxyData      galaxy_data_t;
typedef struct NamedPlanetData nampla_data_t;
typedef struct PlanetData      planet_data_t;
typedef struct SpeciesData     species_data_t;
typedef struct StarData        star_data_t;

struct GalaxyData {
    int32_t	         turn_number;  /* Current turn number. */
    int32_t	         radius;       /* Galactic radius in parsecs. */
    int32_t          num_stars;
    star_data_t    **stars;
    int32_t          num_planets;
    planet_data_t  **planets;
    int32_t          num_species;
    species_data_t **species;
    int32_t          num_namplas;
    named_planet_t **namplas;
    int32_t          num_ships;
    ship_data_t    **ships;
};

struct NamedPlanetData {
    int32_t      id;
    star_data_t *star;
    int32_t      orbit;
    const char  *name;	/* Name of planet. */
    uint8_t	status;		/* Status of planet. */
    uint8_t	hiding;		/* HIDE order given. */
    uint8_t	hidden;		/* Colony is hidden. */
    int16_t	siege_eff;	/* Siege effectiveness - a percentage between					0 and 99. */
    int16_t	shipyards;	/* Number of shipyards on planet. */
    int32_t	IUs_needed;	/* Incoming ship with only CUs on board. */
    int32_t	AUs_needed;	/* Incoming ship with only CUs on board. */
    int32_t	auto_IUs;	/* Number of IUs to be automatically installed. */
    int32_t	auto_AUs;	/* Number of AUs to be automatically installed. */
    int32_t	IUs_to_install;	/* Colonial mining units to be installed. */
    int32_t	AUs_to_install;	/* Colonial manufacturing units to be installed. */
    int32_t	mi_base;	/* Mining base times 10. */
    int32_t	ma_base;	/* Manufacturing base times 10. */
    int32_t	pop_units;	/* Number of available population units. */
    int32_t	use_on_ambush;	/* Amount to use on ambush. */
    int32_t	message;	/* Message associated with this planet,					if any. */
    int32_t	item_quantity[MAX_ITEMS]; /* Quantity of each item available. */
}

struct PlanetData {
    int32_t id;
    star_data_t *star;
    int32_t orbit;
    uint8_t	temperature_class;  /* Temperature class, 1-30. */
    uint8_t	pressure_class;	    /* Pressure class, 0-29. */
    uint8_t	special;	    /* 0 = not special, 1 = ideal home planet, 2 = ideal colony planet, 3 = radioactive hellhole. */
    uint8_t gas[4];		    /* Gas in atmosphere. Zero if none. */
    uint8_t	gas_percent[4];	    /* Percentage of gas in atmosphere. */
    int16_t	diameter;	    /* Diameter in thousands of kilometers. */
    int16_t	gravity;	    /* Surface gravity. Multiple of Earth gravity times 100. */
    int16_t	mining_difficulty;  /* Mining difficulty times 100. */
    int16_t	econ_efficiency;    /* Economic efficiency. Always 100 for a home planet. */
    int16_t	md_increase;	    /* Increase in mining difficulty. */
    int32_t	message;	    /* Message associated with this planet, if any. */
    named_planet_t *colonies[MAX_SPECIES+1]; /* set only if the species has a colony on the planet */
};

struct SpeciesData {
    int32_t id;
    const char *name;		/* Name of species. */
    const char *govt_name;		/* Name of government. */
    const char *govt_type;		/* Type of government. */
    star_t          *home_system;
    planet_t        *home_planet;
    int32_t          num_namplas;
    named_planet_t **namplas; // first nampla is always the home world
    uint8_t	required_gas;		/* Gas required by species. */
    uint8_t	required_gas_min;	/* Minimum needed percentage. */
    uint8_t	required_gas_max;	/* Maximum allowed percentage. */
    uint8_t	neutral_gas[6];		/* Gases neutral to species. */
    uint8_t	poison_gas[6];		/* Gases poisonous to species. */
    uint8_t	auto_orders;		/* AUTO command was issued. */
    int16_t	tech_level[6];		/* Actual tech levels. */
    int16_t	init_tech_level[6];	/* Tech levels at start of turn. */
    int16_t	tech_knowledge[6];	/* Unapplied tech level knowledge. */
    int32_t num_namplas;		/* Number of named planets, including home planet and colonies. */
    int32_t num_ships;		/* Number of ships. */
    int32_t	tech_eps[6];		/* Experience points for tech levels. */
    int32_t	hp_original_base;	/* If non-zero, home planet was bombed either by bombardment or germ warfare and has not yet fully recovered. Value is total economic base before bombing. */
    int32_t	econ_units;		/* Number of economic units. */
    int32_t	fleet_cost;		/* Total fleet maintenance cost. */
    int32_t	fleet_percent_cost;	/* Fleet maintenance cost as a percentage times one hundred. */
    species_t *contact[MAX_SPECIES+1]; /* A bit is set if corresponding species has been met. */
    species_t *ally[MAX_SPECIES+1]; /* A bit is set if corresponding species is considered an ally. */
    species_t *enemy[MAX_SPECIES+1]; /* A bit is set if corresponding species is considered an enemy. */
};

struct StarData {
    int32_t id;
    uint8_t x, y, z; /* Coordinates. */
    uint8_t	type;		/* Dwarf, degenerate, main sequence or giant. */
    uint8_t	color;		/* Star color. Blue, blue-white, etc. */
    uint8_t	size;		/* Star size, from 0 thru 9 inclusive. */
    uint8_t num_planets;	/* Number of usable planets in star system. */
    uint8_t home_system;	/* TRUE if this is a good potential home system. */
    uint8_t worm_here;	/* TRUE if wormhole entry/exit. */
    int32_t	message;	/* Message associated with this star system, if any. */
    planet_data_t  *planets[9+1]; // add 1 so we can index by orbit or PN
    species_data_t *visited_by[MAX_SPECIES+1]; /* A bit is set if corresponding species has been here. */
};

star_data_      *FindStar(star_t **stars, uint8_t x, uint8_t y, uint8_t z);
galaxy_data_t   *LoadData(void);
galaxy_data_t   *LoadGalaxyData(void);
void             LoadNamedPlanetData(galaxy_data_t *g);
void             LoadPlanetData(galaxy_data_t *g);
void             LoadSpeciesData(galaxy_data_t *g);
void             LoadStarData(galaxy_data_t *g);
species_data_t **SpeciesSet(int32_t mask[NUM_CONTACT_WORDS], species_data_t **species);

// todo: is there a better way than brute force search to find the home world?
star_data_ *FindStar(star_t **stars, uint8_t x, uint8_t y, uint8_t z) {
    for (int i = 0; stars[i] != 0; i++) {
        if (stars[i]->x == x && stars[i]->y == y && stars[i]->z == z) {
            return stars[i];
        }
    }
    return 0;
}


galaxy_data_t *LoadData(void) {
    // load all the binary data files
    get_galaxy_data();
    get_star_data();
    get_planet_data();
    get_species_data();

    // return them
    return LoadGalaxyData();
}

// convert galaxy data
galaxy_data_t *LoadGalaxyData() {
    extern struct species_data spec_data[MAX_SPECIES];

    galaxy_data_t *g = calloc(1, sizeof(galaxy_data_t));
    assert(g != 0);

    g->turn_number = galaxy.turn_number;
    g->radius = galaxy.radius;

    g->num_stars = num_stars;
    g->stars = calloc(num_stars + 2, sizeof(star_data_t *));
    assert(stars != 0);
    for (int i = 0; i < num_stars; i++) {
        stars[i] = calloc(1, sizeof(star_data_t));
        assert(stars[i] != 0);
        stars[i]->id = i + 1;
    }

    g->num_planets = num_planets;
    g->planets = calloc(g->num_planets + 2, sizeof(planet_data_t *));
    assert(g->planets != 0);
    for (int i = 0; i < g->num_planets; i++) {
        g->planets[i] = calloc(1, sizeof(planet_data_t));
        assert(g->planets[i] != 0);
        g->planets[i]->id = i + 1;
    }

    g->num_species = galaxy.num_species;
    g->species = calloc(g->num_species+2, sizeof(species_data_t *));
    assert(g->species != 0);
    for (int i = 0; i < num_species; i++) {
        g->species[i] = calloc(1, sizeof(species_data_t));
        assert(g->species[i] != 0);
        g->species[i]->id = i + 1;
        g->species[i]->num_namplas = spec_data[i].num_namplas;
        g->species[i]->namplas = calloc(g->species[i]->num_namplas+1, sizeof(nampla_data_t *));
        assert(g->species[i]->namplas != 0);
        g->species[i]->num_ships = spec_data[i].num_ships;
        g->species[i]->ships = calloc(g->species[i]->num_ships+1, sizeof(ship_data_t *));
        assert(g->species[i]->ships != 0);
    }

    // named planet data
    for (int i = 0; i < num_species; i++) {
        g->num_namplas += g->species[i]->num_namplas;
    }
    g->namplas = calloc(g->num_namplas+2, sizeof(nampla_data_t*));
    assert(g->namplas != 0);
    for (int i = 0; i < num_species; i++) {
        g->namplas[i] = calloc(1, sizeof(nampla_data_t));
        assert(g->namplas[i] != 0);
        g->namplas[i]->id = i + 1;
    }

    // ship data
    for (int i = 0; i < num_species; i++) {
        g->num_ships += g->species[i]->num_ships;
    }
    g->ships = calloc(g->num_ships+2, sizeof(ship_data_t*));
    assert(g->ships != 0);
    for (int i = 0; i < num_ships; i++) {
        g->ships[i] = calloc(1, sizeof(ship_data_t));
        assert(g->ships[i] != 0);
        g->ships[i]->id = i + 1;
    }

    LoadStarData(g);
    LoadPlanetData(g);
    LoadSpeciesData(g);
    LoadNamedPlanetData(g);

    return g;
}

void LoadNamedPlanetData(galaxy_data_t *g) {
    extern struct nampla_data *namp_data[MAX_SPECIES];
    extern struct species_data spec_data[MAX_SPECIES];

    for (int i = 0; i < g->num_species; i++) {
        g->species[i]->namplas = calloc()
    }
    for (int j = 0; j < sp->num_namplas; j++) {
        struct nampla_data *np = namp_data[species_number - 1][j];
    }
}

void LoadPlanetData(galaxy_data_t *g) {
    extern struct planet_data *planet_base;
    extern int num_planets;

    for (int i = 0; i < num_planets; i++) {
        struct planet_data *planet = planet_base + i;

        // star and orbit are set in LoadStarData
        g->planets[i]->temperature_class = planet->temperature_class;
        g->planets[i]->pressure_class = planet->pressure_class;
        g->planets[i]->special = planet->special;
        g->planets[i]->diameter = planet->diameter;
        g->planets[i]->gravity = planet->gravity;
        g->planets[i]->mining_difficulty = planet->mining_difficulty;
        g->planets[i]->econ_efficiency = planet->econ_efficiency;
        g->planets[i]->md_increase = planet->md_increase;
        g->planets[i]->message = planet->message;
        for (int j = 0; j < 4; j++){
            g->planets[i]->gas[j] = planet->gas[j];
            g->planets[i]->gas_percent[j] = planet->gas_percent[j];
        }
    }
}

void LoadSpeciesData(galaxy_data_t *g) {
    extern struct nampla_data *namp_data[MAX_SPECIES];
    extern struct species_data spec_data[MAX_SPECIES];

    // convert species data
    for (int i = 0; i < num_species; i++) {
        struct species_data *sp = &spec_data[i];

        species[i]->name = strdup(sp->name);
        assert(species[i]->name != 0);
        species[i]->govt_name = strdup(sp->govt_name);
        assert(species[i]->govt_name != 0);
        species[i]->govt_type = strdup(sp->govt_type);
        assert(species[i]->govt_type != 0);
        species[i]->home_system = FindStar(stars, sp->x), sp->y, sp->z);
        assert(species[i]->home_system != 0);
        species[i]->home_world = species[i]->home_system->planets[sp->pn];
        assert(species[i]->home_world != 0);
        species[i]->required_gas = sp->required_gas;
        species[i]->required_gas_min = sp->required_gas_min;
        species[i]->required_gas_max = sp->required_gas_max;
        for (j = 0; j < 6; j++) {
            species[i]->neutral_gas[j] = sp->neutral_gas[j];
            species[i]->poison_gas[j] = sp->poison_gas[j];
        }
        species[i]->auto_orders = sp->auto_orders;
        for (j = 0; j < 6; j++) {
            species[i]->tech_level[j] = sp->tech_level[j];
            species[i]->init_tech_level[j] = sp->init_tech_level[j];
            species[i]->tech_knowledge[j] = sp->tech_knowledge[j];
            species[i]->tech_eps[j] = sp->tech_eps[j];
        }
        species[i]->num_namplas = sp->num_namplas;
        species[i]->num_ships = sp->num_ships;
        species[i]->hp_original_base = sp->hp_original_base;
        species[i]->econ_units = sp->econ_units;
        species[i]->fleet_cost = sp->fleet_cost;
        species[i]->fleet_percent_cost = sp->fleet_percent_cost;
        species[i]->ally = SpeciesSet(sp->ally, species);
        assert(species[i]->ally != 0);
        species[i]->contact = SpeciesSet(sp->contact, species);
        assert(species[i]->contact != 0);
        species[i]->enemy = SpeciesSet(sp->enemy, species);
        assert(species[i]->enemy != 0);
    }
}

void LoadStarData(galaxy_data_t *g) {
    extern int num_stars;
    extern int star_data_modified;
    extern struct star_data *star_base;

    for (int i = 0; i < num_stars; i++) {
        struct star_data *star = star_base + i;

        g->stars[i]->x = star->x;
        g->stars[i]->y = star->y;
        g->stars[i]->z = star->z;
        g->stars[i]->type = star->type;
        g->stars[i]->color = star->color;
        g->stars[i]->size = star->size;
        g->stars[i]->num_planets = star->num_planets;
        g->stars[i]->home_system = star->home_system;
        g->stars[i]->worm_here = star->worm_here;
        g->stars[i]->message = star->message;
        g->stars[i]->visited_by = SpeciesSet(star->visited_by, species);
        assert(g->stars[i]->visited_by != 0)

        // link the star to its planets and its planets to the star
        for (int i = 0; i < num_stars; i++) {
            for (int j = 0; j < g->stars[i]->num_planets; j++) {
                g->stars[i]->planets[j] = g->planets[star->planet_index + j];
                g->stars[i]->planets[j]->star = g->stars[i];
                g->stars[i]->planets[j]->orbit = j + 1;
            }
        }
    }
}

species_data_t **SpeciesSet(int32_t mask[NUM_CONTACT_WORDS], species_data_t **species) {
    int off;
    int setSize = 0;
    species_data_t **set;
    for (int sp = 0; sp < MAX_SPECIES; sp++) {
        if ((mask[sp / 32] & 1 << (sp % 32)) != 0) {
            setSize++;
        }
    }
    set = calloc(setSize+1, sizeof(species_data_t *));
    assert(set != 0);
    off = 0;
    for (int sp = 0; sp < MAX_SPECIES; sp++) {
        if ((mask[sp / 32] & 1 << (sp % 32)) != 0) {
            set[off++] = species[sp];
        }
    }
    return set;
}

