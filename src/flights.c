/* 103124 - Gonçalo Sampaio Bárias - goncalo.barias@tecnico.ulisboa.pt */

#include <stdio.h>
#include <string.h>
#include "proj1.h"

/**
 * Checks if the flight code it receives is valid.
 * If so, it returns 0, otherwise it returns 1.
 * Auxiliary function of the 'v' command and the CheckAddFlightErrors function.
 */
int CheckFlightCodeErrors(const char flight_code[], clock date_depart) {
	int i;

	for (i = 0; i < 2; i++) {
		if (flight_code[i] < 'A' || flight_code[i] > 'Z') {
			printf(FLIGHT_ERR_INVALID); /* it doesn't start with two uppercase letters */
			return 1;
		}
	}

	while (flight_code[i] != '\0') {
		if (i == 2 && flight_code[i] == '0') {
			printf(FLIGHT_ERR_INVALID); /* the digits start with a zero */
			return 1;
		}
		if (flight_code[i] < '0' || flight_code[i] > '9') {
			printf(FLIGHT_ERR_INVALID); /* the digits are invalid */
			return 1;
		}
		if (i >= 6) {
			printf(FLIGHT_ERR_INVALID); /* length of flight code is too big */
			return 1;
		}
		i++;
	}

	for (i = 0; i < totalFlights; i++) {
		if (strcmp(allFlights[i].flight_code, flight_code) == 0 &&
			CompareDates(allFlights[i].date_departure, date_depart, 0) == 0) {
			printf(FLIGHT_ERR_DUPLICATE); /* another flight exists on the same day with the same code */
			return 1;
		}
	}

	return 0;
}

/**
 * Checks if a new airport is valid.
 * If so, it returns 0, otherwise returns 1.
 * Auxiliary function of the 'v' command.
 */
int CheckAddFlightErrors(flight new_flight) {
	if (CheckFlightCodeErrors(new_flight.flight_code,
						   new_flight.date_departure)) {
		return 1;
	}

	if (CheckAirportExistence(new_flight.arrival_id)) {
		printf(AIRPORT_ERR_NO_ID, new_flight.arrival_id); /* no arrival id exists */
		return 1;
	}

	if (CheckAirportExistence(new_flight.departure_id)) {
		printf(AIRPORT_ERR_NO_ID, new_flight.departure_id); /* no departure id exists */
		return 1;
	}

	if (totalFlights >= MAX_FLIGHTS) {
		printf(FLIGHT_ERR_TOO_MANY); /* too many flights */
		return 1;
	}

	if (CheckDateErrors(new_flight.date_departure)) return 1;

	if (new_flight.duration > MAX_DURATION) {
		printf(FLIGHT_ERR_INVALID_DURATION);
		return 1;
	}

	if (new_flight.capacity > MAX_PASSENGERS
		|| new_flight.capacity < MIN_PASSENGERS) {
		printf(FLIGHT_ERR_INVALID_CAPACITY);
		return 1;
	}

	return 0;
}

/**
 * Uses binary search to insert a flight into an array sorted by date.
 * It uses the sort array to insert the index of the new flight on the right position.
 * It compares the flight in the middle position with the flight it receives on each
 * iteration. If the mode is 0 it will insert sorted by arrival date, otherwise it
 * will be sorted by departure date.
 */
void AddSortedFlight(int sort[], flight new_flight, const int mode) {
	int left = 0, right = totalFlights - 1, middle, comp, i;

	while (left <= right) {
		middle = (left + right) / 2;
		comp = CompareFlightDates(allFlights[sort[middle]], new_flight, mode);
		if (comp < 0) {
			left = middle + 1;
		} else {
			right = middle - 1;
		}
	}

	for (i = totalFlights - 1; i >= left; i--) {
		sort[i + 1] = sort[i];
	}
	sort[left] = totalFlights;
}

/**
 * Lists all of the flights by the order they were created.
 * Auxiliary function of the 'v' command.
 */
void ListAllFlights() {
	int i;

	for (i = 0; i < totalFlights; i++) {
		printf(FLIGHT_FULL_PRINT, allFlights[i].flight_code,
		 						allFlights[i].departure_id,
								allFlights[i].arrival_id);

		PrintClock(allFlights[i].date_departure);
	}
}
