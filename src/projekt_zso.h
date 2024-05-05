#pragma once

#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

// CONFIG (comment to toggle)
#define VERBOSE // Print logs to stdout
#define FANCY   // Animates driving between stops
#define SLOW    // Delay for each task
#define LAZY    // Driver ends work if the bus is ever empty

// NUMBER
#define ROUNDS 10
#define STOP_COUNT 4
#define BUS_CAPACITY 50
#define PASSENGER_TOTAL 500
#define SEED 621

// TIME IN MILLISECONDS
#define BUS_DELAY 2000
#define BUS_DRIVES 1000
#define PASSENGER_EVERY 100
#define PASSENGER_MOVE 25

// FANCINESS
#ifdef FANCY
#define DOT_FOR 100
#endif

void msleep(const uint millis);             // Sleep [millis] milliseconds
void super_print(const char* format, ...);  // Printf wrapper with custom formatting
void* bus_thread(void* arg);                // Thread representing a bus
void* passenger_thread(void* arg);          // Thread representing a passenger
int projekt_zso();                          // Main program executor