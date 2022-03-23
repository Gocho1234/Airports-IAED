/* 103124 - Gonçalo Sampaio Bárias - goncalo.barias@tecnico.ulisboa.pt

*--------------------------------------------------------------------*
*                       Airport Manager                              *
*                                                                    *
* 1st IAED Project - 21/22                                           *
* Computer Science and Engineering (Alameda)                         *
* Instituto Superior Técnico                                         *
*--------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "proj1.h"

/* Global variables */
int totalAirports; 							/* tracks the total amount of airports added by the user */
int totalFlights; 							/* tracks the total amount of flights added by the user */
airport allAirports[MAX_AIRPORTS]; 			/* stores all of the current airports */
flight allFlights[MAX_FLIGHTS]; 			/* stores all of the current flights */
char sortedIDs[MAX_AIRPORTS][ID_LENGTH]; 	/* stores all of the IDs sorted by alphabetical order */

/**
 * Handles command input.
 * Reads one letter inserted by the user and executes the right command.
 * If the "run" variable has a value of 0 the program stops.
 */
int main() {
	char c;
	int run = 1;

	while (run) {
		c = getchar();
		switch(c) {
			case 'q':
				/* 'q' command stops the program */
				run = 0;
				break;
			case 'a':
				AddAirport();
				break;
			case 'l':
				ListAirports();
				break;
			case 'v':
				AddFlight_ListFlights();
				break;
			case 'p':
				FlightDeparturesInAirport();
				break;
			case 'c':
				FlightArrivalsInAirport();
				break;
			case 't':
				AdvanceSystemDate();
				break;
		}
	}

	return 0;
}

/**
 * Handles the 'a' command.
 * Adds a new airport to the system with the specified ID, country and city.
 */
void AddAirport() {
	char id[ID_LENGTH], country[MAX_COUNTRY_LENGTH], city[MAX_CITY_LENGTH];

	GetOneArgument(id, 0);
	GetOneArgument(country, 0);
	GetOneArgument(city, 1);

	if (CheckAddAirportErrors(id))
		return;

	/* actually add the aiport to the system */
	strcpy(allAirports[totalAirports].id, id);
	strcpy(allAirports[totalAirports].country, country);
	strcpy(allAirports[totalAirports].city, city);
	allAirports[totalAirports].num_flight_departures = 0;

	AddSortedAirportID(id);

	printf("airport %s\n", allAirports[totalAirports].id);

	totalAirports++;
}

/**
 * Handles the 'l' command.
 * If no arguments are provided it will print all of the airports in
 * alphabetical order.
 * Otherwise, the function prints the airports with the specified IDs.
 */
void ListAirports() {
	char id[ID_LENGTH];
	int i, mode;

	while ((mode = GetOneArgument(id, 0)) != 0) {
		if (CheckAirportValidity(id))
			return;

		i = GetAirportFromID(id);
		printf("%s %s %s %d\n", id, allAirports[i].city, allAirports[i].country,
		 					allAirports[i].num_flight_departures);

		if (mode == 2)
			return;
	}

	if (mode == 0)
		ListAllAirports();
}

/**
 * Handles the 'v' command.
 * If no arguments are provided it will print all of the flights in alphabetical
 * order.
 * Otherwise, the function adds a new flight to the system with the specified
 * flight code, airport of departure id, airport of arrival id, date of
 * departure, time of departure, duration of flight and capacity of the flight.
 */
void AddFlight_ListFlights() {
	char argument[MAX_COMMAND_LENGTH];
	int i, mode;

	for (i = 0; i <= 6; i++) {
		mode = GetOneArgument(argument, 0);
		if (mode == 0) {
			ListAllFlights();
			break;
		}
		switch(i) {
			case 0: /* flight code */
				if (CheckFlightCodeErrors(argument))
					return;
				strcpy(allFlights[totalFlights].flight_code, argument);
				break;
			case 1: /* arrival id */
				if (CheckAirportValidity(argument))
					return;
				break;
			case 2: /* departure id */
				if (CheckAirportValidity(argument))
					return;
				break;
			case 3: /* date */
				if (CheckDateErrors(argument))
					return;
				strcpy(allFlights[totalFlights].date, argument);
				break;
			case 4: /* time */
				strcpy(allFlights[totalFlights].time, argument);
				break;
			case 5: /* duration */
				if (CheckDurationErrors(argument))
					return;
				allFlights[totalFlights].duration = atoi(argument);
				break;
			case 6: /* capacity */
				if (CheckCapacityErrors(argument))
					return;
				allFlights[totalFlights].capacity = atoi(argument);
				break;
		}
	}

	totalFlights++;
}

/**
 * Handles the 'p' command.
 * Lists all of the flights that depart from the airport with the given ID.
 */
void FlightDeparturesInAirport() {
	/**/
}

/**
 * Handles the 'c' command.
 * Lists all of the flights that arrive at the airport with the given ID.
 */
void FlightArrivalsInAirport() {
	/**/
}

/**
 * Handles the 't' command.
 * Forwards the date of the system.
 */
void AdvanceSystemDate() {
	/**/
}

/**
 *
 */
char GetOneArgument(char *argument, const int mode) {
	int c, i = 0;

	while ((c = getchar()) != '\n' && c != EOF) {
		if (i == 0 && isspace(c))
			continue;
		if (mode == 0 && isspace(c))
			break;
		argument[i++] = c;
	}
	argument[i] = '\0';

	if (argument[0] == '\0')
		return 0;
	else if (c != '\n')
		return 1;
	else
		return 2;
}
