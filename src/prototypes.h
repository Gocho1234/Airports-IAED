/* 103124 - Gonçalo Sampaio Bárias - goncalo.barias@tecnico.ulisboa.pt */

typedef struct {
	char id[ID_LENGTH];
	char country[MAX_COUNTRY_LENGTH];
	char city[MAX_CITY_LENGTH];
} airport;

typedef struct {
	char flight_code[MAX_CODE_LENGTH];


	char date[DATE_LENGTH];
	char time[TIME_LENGTH];
	int duration;
	int max_passengers;
} flight;
