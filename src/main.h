/*
 *		File: main.h
 * 		Author: Gonçalo Sampaio Bárias (ist1103124)
 *		Description: Main header file for the project. Includes
 *					constants and prototypes.
 */

#include "structures.h"

#ifndef _MAIN_H_
#define _MAIN_H_

/**
 * Constants
 */

/* airports */
#define ID_LENGTH 4
#define MAX_COUNTRY_LENGTH 31
#define MAX_CITY_LENGTH 51
#define MAX_AIRPORTS 40

#define AIRPORT_PRINT "%s %s %s %d\n"
#define AIRPORT_ADD_PRINT "airport %s\n"

#define AIRPORT_ERR_TOO_MANY "too many airports\n"
#define AIRPORT_ERR_INVALID "invalid airport ID\n"
#define AIRPORT_ERR_DUPLICATE "duplicate airport\n"
#define AIRPORT_ERR_NO_ID "%s: no such airport ID\n"

/* flights */
#define MAX_FLIGHT_CODE_LENGTH 7
#define MAX_CAPACITY_LENGTH 3
#define MAX_FLIGHTS 30000
#define MIN_PASSENGERS 10

#define FLIGHT_PRINT "%s %s"
#define FLIGHT_FULL_PRINT "%s %s %s"

#define FLIGHT_ERR_INVALID "invalid flight code\n"
#define FLIGHT_ERR_DUPLICATE "flight already exists\n"
#define FLIGHT_ERR_TOO_MANY "too many flights\n"
#define FLIGHT_ERR_INVALID_DURATION "invalid duration\n"
#define FLIGHT_ERR_INVALID_CAPACITY "invalid capacity\n"

/* dates */
#define DATE_LENGTH 11
#define TIME_LENGTH 6
#define MAX_DURATION_MINS 720
#define MONTHS 12
#define MINS_YEAR 525600
#define MINS_DAY 1440
#define MINS_HOUR 60
#define START_DAY "00:00"

#define DATE_PRINT "%02d-%02d-%04d\n"
#define DATE_FULL_PRINT " %02d-%02d-%04d %02d:%02d\n"

#define DATE_ERR_INVALID "invalid date\n"

/* booking */
#define MAX_ARG_LENGTH 65535
#define MIN_BOOKING_CODE_LENGTH 10

#define BOOKING_FULL_PRINT "%s %d\n"

#define BOOKING_ERR_INVALID "invalid reservation code\n"
#define BOOKING_ERR_FLIGHT_CODE "%s: flight does not exist\n"
#define BOOKING_ERR_DUPLICATE "%s: flight reservation already used\n"
#define BOOKING_ERR_TOO_MANY "too many reservations\n"
#define BOOKING_ERR_PASSENGER "invalid passenger number\n"
#define BOOKING_FLIGHT_NA "not found\n"

/**
 * Structs
 */

typedef struct {
	int day;
	int month;
	int year;
	int hours;
	int minutes;
} clock;

typedef struct {
	char* flight_code;
	clock date_departure;
	char* booking_code;
	int res_num;
} booking;

typedef struct {
	char* flight_code;
	char departure_id[ID_LENGTH];
	char arrival_id[ID_LENGTH];
	clock date_departure;
	int duration;
	clock date_arrival;
	long int occupation;
	long int capacity;
} flight;

typedef struct {
	char id[ID_LENGTH];
	char country[MAX_COUNTRY_LENGTH];
	char city[MAX_CITY_LENGTH];
	int departures;
} airport;

typedef struct {
	int totalAirports; 					/* tracks the total amount of airports added by the user */
	int totalFlights;					/* tracks the total amount of flights added by the user */
	airport allAirports[MAX_AIRPORTS]; 	/* stores all of the current airports */
	int sortedAirports[MAX_AIRPORTS]; 	/* stores the indexes of all the airports, sorted by the alphabetical order of the IDs */
	list_t* allFlights;					/* */
	hashtable* bookingsTable;			/* */
	clock date;							/* stores the system date of the system */
	clock max_date;						/* stores the date that is one year in future from the system date */
} global_store;

/**
 * Prototypes
 */

/* proj1.c */

int HandleCommands(global_store* global);

void AddAirport(global_store* global);

void ListAirports(global_store* global);

void AddFlight_ListFlights(global_store* global);

void FlightDeparturesInAirport(global_store* global);

void FlightArrivalsInAirport(global_store* global);

void AdvanceSystemDate(global_store* global);

global_store* GlobalInit();

char GetOneArgument(char *argument, const int mode);

/* airports.c */

void ReadAirport(airport *new_airport);

int CheckAddAirportErrors(global_store* global, const char id[]);

int CheckAirportExistence(global_store* global, const char id[]);

int GetAirport(global_store* global, const char id[]);

void AddSortedAirport(global_store* global, airport airport_1);

void ListAllAirports();

void PrintAirport(airport airport_1);

/* flights.c */

void ReadFlight(flight *new_flight);

int CheckAddFlightErrors(global_store* global, flight* new_flight);

int CheckFlightCodeErrors(const char flight_code[]);

int CheckDuplicateFlight(global_store* global, flight* flight_1);

char* GetFlightKey(void* flight_1);

void ClearFlight(void* flight_delete);

void ListAllFlights(global_store* global);

flight* GetFlight(global_store* global, char* flight_code, clock date_departure);

int CheckFlightExistence(global_store* global, char* flight_code);

/* dates.c */

int CheckDateErrors(global_store* global, clock date_depart);

int ConvertDatesToMins(clock formatted_date);

clock UpdateDate(clock date, int duration);

int CompareDates(clock date_1, clock date_2, const int mode);

int CompareFlightDatesDeparture(void* flight_1, void* flight_2);

int CompareFlightDatesArrival(void* flight_1, void* flight_2);

clock ReadClock(char calendar_date[], char hours_mins[]);

int ReadDuration(char duration[]);

void PrintClock(clock date);

/* proj2.c */

void AddBooking_ListBookings(global_store* global);

void DeleteBooking_Flight(global_store* global);

void ExitProgram(global_store* global);

void* SecureMalloc(unsigned int allocation);

int IsPrime(int x);

int GetPrime(int minNum);

int IsUpperCase(char c);

int IsDigit(char c);

void swap_vars(void** a, void** b);

/* bookings.c */

booking* ReadBooking();

int CheckAddBookingErrors(global_store* global, booking* new_booking);

int CheckBookingCodeErrors(char* booking_code);

void ListBookings(global_store* global, char* flight_code, clock date);

int CompareBookings(void* booking_1, void* booking_2);

char* GetBookingKey(void* booking_1);

void ClearBooking(void* booking_delete);

booking* GetBooking(char* booking_code);

#endif
