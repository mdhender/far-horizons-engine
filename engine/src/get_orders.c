static char *fbuf;

void expectEOL();
int expectInt();
char *expectString();

main (argc, argv)

int argc;
char *argv[];

{
    int		i, j, n, galaxy_fd;
	long fsize;

    char	*tp;

    FILE	*ascii_file;

    /* Open file 'ascii.dat'. */
    ascii_file = fopen ("ascii.dat", "r");
    if (ascii_file == NULL)
    {
	fprintf (stderr, "\n\tCannot open 'ascii.dat' for reading!\n\n");
	exit (-1);
    }
	fseek(ascii_file, 0, SEEK_END);
	fsize = ftell(ascii_file);
	fseek(ascii_file, 0, SEEK_SET);
	fbuf = malloc(fsize + 1);
	if (fbuf == 0) {
		fprintf(stderr, "out of memory loading file\n");
		exit(-1);
	}
	fread(fbuf, 1, fsize, ascii_file);
	fclose(ascii_file);
	fbuf[fsize] = 0;

    /* Do galaxy data. */
    printf ("Doing galaxy data...\n");
	galaxy.d_num_species = expectInt();
	galaxy.num_species = expectInt();
	galaxy.radius = expectInt();
	galaxy.turn_number = expectInt();
	expectEOL();

    galaxy_fd = creat ("galaxy.dat", 0600);
    if (galaxy_fd < 0)
    {
	fprintf (stderr, "\n  Cannot create new version of file galaxy.dat!\n");
	exit (-1);
    }

    n = write (galaxy_fd, &galaxy, sizeof (struct galaxy_data));
    if (n != sizeof (struct galaxy_data))
    {
	fprintf (stderr, "\n\tCannot write data to file 'galaxy.dat'!\n\n");
	exit (-1);
    }
    close (galaxy_fd);

    /* Do star data. */
    printf ("Doing star data...\n");
    num_stars = expectInt(); expectEOL();

    /* Allocate enough memory for all stars. */
    n = num_stars * sizeof(struct star_data);
    star_base = (struct star_data *) malloc (n);
    if (star_base == NULL)
    {
	fprintf (stderr, "\nCannot allocate enough memory for star data!\n\n");
	exit (-1);
    }

    for (i = 0; i < num_stars; i++)
    {
	star = star_base + i;

	/* Initialize structure to all zeros. */
	tp = (char *) star;
	for (j = 0; j < sizeof (struct star_data); j++) *tp++ = 0;

	n = expectInt(); 
	if (n != i+1)
	{
		fprintf(stderr, "\nassert(n == i+1)\nassert(%d == %d)\n", n, i+1);
	    fprintf (stderr, "\n\tStar data test failure!\n\n");
	    exit (-1);
	}

	n = expectInt();  star->x = n;
	n = expectInt();  star->y = n;
	n = expectInt();  star->z = n;
	n = expectInt();  star->type = n;
	n = expectInt();  star->color = n;
	n = expectInt();  star->size = n;
	n = expectInt();  star->num_planets = n;
	n = expectInt();  star->home_system = n;
	n = expectInt();  star->worm_here = n;
	n = expectInt();  star->worm_x = n;
	n = expectInt();  star->worm_y = n;
	n = expectInt();  star->worm_z = n;
	n = expectInt();  star->planet_index = n;
	n = expectInt();  star->message = n;
	for (j = 0; j < NUM_CONTACT_WORDS; j++)
	{
	    n = expectInt(); 
	    star->visited_by[j] = n;
	}
	expectEOL();
    }

    /* Do planet data. */
    printf ("Doing planet data...\n");
    num_planets = expectInt(); expectEOL();

    /* Allocate enough memory for all planets. */
    n = num_planets * sizeof(struct planet_data);
    planet_base = (struct planet_data *) malloc (n);
    if (planet_base == NULL)
    {
	fprintf (stderr, "\nCannot allocate enough memory for planet data!\n\n");
	exit (-1);
    }

    for (i = 0; i < num_planets; i++)
    {
	planet = planet_base + i;

	/* Initialize structure to all zeros. */
	tp = (char *) planet;
	for (j = 0; j < sizeof (struct planet_data); j++) *tp++ = 0;

	n = expectInt(); 
	if (n != i+1)
	{
	    fprintf (stderr, "\n\tPlanet data test failure!\n\n");
	    exit (-1);
	}

	n = expectInt();  planet->temperature_class = n;
	n = expectInt();  planet->pressure_class = n;
	for (j = 0; j < 4; j++)
	{
	    n = expectInt(); 
	    planet->gas[j] = n;
	}
	for (j = 0; j < 4; j++)
	{
	    n = expectInt(); 
	    planet->gas_percent[j] = n;
	}
	n = expectInt();  planet->diameter = n;
	n = expectInt();  planet->gravity = n;
	n = expectInt();  planet->mining_difficulty = n;
	n = expectInt();  planet->econ_efficiency = n;
	n = expectInt();  planet->md_increase = n;
	n = expectInt(); expectEOL(); planet->message = n;
    }

    /* Do species data. */
    for (species_index = 0; species_index < galaxy.num_species; species_index++)
    {
	data_modified[species_index] = FALSE;
	data_in_memory[species_index] = FALSE;
    }

    while (*fbuf != 0)
    {
	n = expectInt(); expectEOL();

	species_number = n;
	species_index = n - 1;

	data_in_memory[species_index] = TRUE;
	data_modified[species_index] = TRUE;

	species = &spec_data[species_index];

	/* Initialize structure to all zeros. */
	tp = (char *) species;
	for (j = 0; j < sizeof (struct species_data); j++) *tp++ = 0;

	strncpy(species->name, expectString(), sizeof(species->name));
	expectEOL();
	printf ("Doing species #%d, SP %s...\n", species_number, species->name);

	strncpy(species->govt_name, expectString(), sizeof(species->govt_name));
	expectEOL();
	strncpy(species->govt_type, expectString(), sizeof(species->govt_type));
	expectEOL();
	n = expectInt();  species->x = n;
	n = expectInt();  species->y = n;
	n = expectInt();  species->z = n;
	n = expectInt();  species->pn = n;
	n = expectInt();  species->required_gas = n;
	n = expectInt();  species->required_gas_min = n;
	n = expectInt();  species->required_gas_max = n;
	for (j = 0; j < 6; j++)
	{
	    n = expectInt(); 
	    species->neutral_gas[j] = n;
	}
	for (j = 0; j < 6; j++)
	{
	    n = expectInt(); 
	    species->poison_gas[j] = n;
	}
	n = expectInt();  species->auto_orders = n;
	n = expectInt();  species->num_namplas = n;
	n = expectInt(); expectEOL(); species->num_ships = n;
	for (j = 0; j < 6; j++)
	{
	    n = expectInt(); 
	    species->tech_level[j] = n;
	}
	for (j = 0; j < 6; j++)
	{
	    n = expectInt(); 
	    species->init_tech_level[j] = n;
	}
	expectEOL();
	for (j = 0; j < 6; j++)
	{
	    n = expectInt(); 
	    species->tech_knowledge[j] = n;
	}
	for (j = 0; j < 6; j++)
	{
	    n = expectInt(); 
	    species->tech_eps[j] = n;
	}
	n = expectInt();  species->econ_units = n;
	n = expectInt();  species->hp_original_base = n;
	n = expectInt();  species->fleet_cost = n;
	n = expectInt(); expectEOL(); species->fleet_percent_cost = n;
	for (j = 0; j < NUM_CONTACT_WORDS; j++)
	{
	    n = expectInt(); 
	    species->contact[j] = n;
	}
	for (j = 0; j < NUM_CONTACT_WORDS; j++)
	{
	    n = expectInt(); 
	    species->ally[j] = n;
	}
	for (j = 0; j < NUM_CONTACT_WORDS; j++)
	{
	    n = expectInt(); 
	    species->enemy[j] = n;
	}
	expectEOL();

	/* Allocate enough memory for all namplas. */
	n = species->num_namplas * sizeof (struct nampla_data);
	namp_data[species_index] = (struct nampla_data *) malloc (n);
	if (namp_data[species_index] == NULL)
	{
	    fprintf (stderr, "\nCannot allocate enough memory for nampla data!\n\n");
	    exit (-1);
	}

	/* Do namplas for this species. */
	nampla_base = namp_data[species_index];
	for (i = 0; i < species->num_namplas; i++)
	{
	    nampla = nampla_base + i;

	    /* Initialize structure to all zeros. */
	    tp = (char *) nampla;
	    for (j = 0; j < sizeof (struct nampla_data); j++) *tp++ = 0;

	    n = expectInt(); expectEOL(); 
	    if (n != i+1)
	    {
		fprintf (stderr,
		    "\n\tNampla data test failure, i = %d, n = %d\n\n", i, n);
		exit (-1);
	    }

	    strncpy(nampla->name, expectString(), sizeof(nampla->name));
	    expectEOL();

	    n = expectInt();  nampla->x = n;
	    n = expectInt();  nampla->y = n;
	    n = expectInt();  nampla->z = n;
	    n = expectInt();  nampla->pn = n;
	    n = expectInt();  nampla->status = n;
	    n = expectInt();  nampla->hiding = n;
	    n = expectInt();  nampla->hidden = n;
	    n = expectInt();  nampla->planet_index = n;
	    n = expectInt();  nampla->siege_eff = n;
	    n = expectInt();  nampla->shipyards = n;
	    n = expectInt();  nampla->IUs_needed = n;
	    n = expectInt();  nampla->AUs_needed = n;
	    n = expectInt();  nampla->auto_IUs = n;
	    n = expectInt();  nampla->auto_AUs = n;
	    n = expectInt();  nampla->IUs_to_install = n;
	    n = expectInt();  nampla->AUs_to_install = n;
	    n = expectInt();  nampla->mi_base = n;
	    n = expectInt();  nampla->ma_base = n;
	    n = expectInt(); expectEOL(); nampla->pop_units = n;
	    for (j = 0; j < MAX_ITEMS; j++)
	    {
		n = expectInt(); 
		nampla->item_quantity[j] = n;
	    }
	    n = expectInt();  nampla->use_on_ambush = n;
	    n = expectInt(); expectEOL(); nampla->message = n;
	}

	/* Allocate enough memory for all ships. */
	n = species->num_ships * sizeof (struct ship_data);
	ship_data[species_index] = (struct ship_data *) malloc (n);
	if (ship_data[species_index] == NULL)
	{
	    fprintf (stderr, "\nCannot allocate enough memory for ship data!\n\n");
	    exit (-1);
	}

	/* Do ships for this species. */
	ship_base = ship_data[species_index];
	for (i = 0; i < species->num_ships; i++)
	{
	    ship = ship_base + i;

	    /* Initialize structure to all zeros. */
	    tp = (char *) ship;
	    for (j = 0; j < sizeof (struct ship_data); j++) *tp++ = 0;

	    n = expectInt(); expectEOL(); 
	    if (n != i+1)
	    {
		fprintf (stderr,
		    "\n\tShip data test failure, i = %d, n = %d\n\n", i, n);
		exit (-1);
	    }

	    strncpy(ship->name, expectString(), sizeof(ship->name));
	    expectEOL();

	    n = expectInt();  ship->x = n;
	    n = expectInt();  ship->y = n;
	    n = expectInt();  ship->z = n;
	    n = expectInt();  ship->pn = n;
	    n = expectInt();  ship->status = n;
	    n = expectInt();  ship->type = n;
	    n = expectInt();  ship->dest_x = n;
	    n = expectInt();  ship->dest_y = n;
	    n = expectInt();  ship->dest_z = n;
	    n = expectInt();  ship->just_jumped = n;
	    n = expectInt();  ship->arrived_via_wormhole = n;
	    n = expectInt();  ship->class = n;
	    n = expectInt(); expectEOL(); ship->tonnage = n;
	    for (j = 0; j < MAX_ITEMS; j++)
	    {
		n = expectInt(); 
		ship->item_quantity[j] = n;
	    }
	    n = expectInt();  ship->age = n;
	    n = expectInt();  ship->loading_point = n;
	    n = expectInt();  ship->unloading_point = n;
	    n = expectInt(); expectEOL(); ship->remaining_cost = n;
	}
    }

    /* Save all binary data. */
    save_star_data ();
    save_planet_data ();
    save_species_data ();

    free_species_data ();
    free (planet_base);
    free (star_base);

    exit (0);
}

static int lineNo = 1;
static int colNo = 1;

void expectEOL() {
	int ch;
	for (;*fbuf != 0;) {
		ch = *fbuf; fbuf++;
		if (ch == '\n') {
			lineNo++;
			colNo = 1;
			return;
		}
	}
	fprintf(stderr, "error: expected eol near line %d, column %d\n", lineNo, colNo);
	exit(-1);
}

int expectInt() {
	int ch = 0;
	int n = 0;
	int digits = 0;

	for (;*fbuf != 0;) {
		ch = *fbuf; fbuf++;
		if (ch == ' ' || ch == '\t' || ch == '\r') {
			continue;
		}
		fbuf--;
		break;
	}

	for (;*fbuf != 0;) {
		ch = *fbuf; fbuf++;
		if (!isdigit(ch)) {
			fbuf--;
			break;
		}
		n = n*10 + (ch - '0');
		digits++;
	}
	if (digits == 0) {
		fprintf(stderr, "error: expected number near line %d, column %d\n", lineNo, colNo);
		exit(-1);
	}
	colNo++;
	return n;
}

char *expectString() {
	static char s[257];
	int ch;
	int i = 0;
	char *p = fbuf;
	for (;*fbuf != 0;) {
		if (i > 256) {
			fprintf(stderr, "error: string overflow near line %d, column %d\n", lineNo, colNo);
			exit(-1);
		}
		ch = *fbuf; fbuf++;
		if (ch == '\r') {
			continue;
		} else if (ch == '\n') {
			fbuf--;
			break;
		}
		i++;
	}
	if (i == 0) {
		s[0] = 0;
	} else {
		strncpy(s, p, i);
		s[i] = 0;
	}
	return s;
}
