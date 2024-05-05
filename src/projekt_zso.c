#include "projekt_zso.h"

#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

uint round_current = 1;
uint stop_current = 0;
uint passengers_on_bus = 0;
bool bus_cruising = true;
bool reaction = false;

pthread_mutex_t lock_bus = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock_stop[STOP_COUNT] = {PTHREAD_MUTEX_INITIALIZER};

pthread_cond_t cond_board[STOP_COUNT] = {PTHREAD_COND_INITIALIZER};
pthread_cond_t cond_leave[STOP_COUNT] = {PTHREAD_COND_INITIALIZER};

#ifdef SLOW
void msleep(const uint millis)
{
    usleep(millis*1000);
}
#else
void msleep(const int millis) {}
#endif

#ifdef VERBOSE
void super_print(const char* format, ...)
{
    printf("[%d] ", round_current);

    va_list list;
    va_start(list, format);

    vprintf(format, list);

    va_end(list);

    printf("\n");
}
#else
void super_print(const char* format, ...) {}
#endif

#ifdef FANCY
const uint DOT_COUNT = BUS_DRIVES / DOT_FOR;
void drive_bus()
{
    printf("[-] "); fflush(stdout);
    for (uint i = 0; i < DOT_COUNT; i++)
    {
        printf("."); fflush(stdout);
        msleep(DOT_FOR);
    }
    printf("\n"); fflush(stdout);
}
#else
void drive_bus()
{
    msleep(BUS_DRIVES);
}
#endif

void* bus_thread(void* arg)
{
    pthread_mutex_lock(&lock_bus);
    super_print("Bus begins cruising.");

    while (round_current <= ROUNDS)
    {
        pthread_mutex_lock(&lock_stop[stop_current]);
        super_print("Bus arrives at stop %d.", stop_current);

        do
        {
            reaction = false;
            pthread_cond_signal(&cond_leave[stop_current]);
            msleep(PASSENGER_MOVE);
        }
        while (reaction == true);

        reaction = true;
        while (reaction == true && passengers_on_bus < BUS_CAPACITY)
        {
            reaction = false;
            pthread_cond_signal(&cond_board[stop_current]);
            msleep(PASSENGER_MOVE);
        }

        pthread_mutex_unlock(&lock_stop[stop_current]);
        super_print("Bus departs from stop %d.", stop_current);

        drive_bus();

        #ifdef LAZY
        if (passengers_on_bus == 0) break;
        #endif
        stop_current = (stop_current + 1) % STOP_COUNT;
        if (stop_current == 0) round_current++;
    }
    
    round_current = 0;  // Signaling that the day is over
    bus_cruising = false;
    super_print("Bus arrives at stop 0, finishing work for today.");

    // Cleanup
    for (int i = 0; i < STOP_COUNT; i++) pthread_cond_broadcast(&cond_board[i]);
    for (int i = 0; i < STOP_COUNT; i++) pthread_cond_broadcast(&cond_leave[i]);
    pthread_mutex_unlock(&lock_bus);
    pthread_exit(NULL);
}

void* passenger_thread(void* arg)
{
    int start = rand() % STOP_COUNT;
    int destination = rand() % STOP_COUNT;

    // Passenger attempts to get in queue to board (unable if bus already arrived)
    pthread_mutex_lock(&lock_stop[start]);
    pthread_mutex_unlock(&lock_stop[start]);

    // Passenger waits in boarding queue
    pthread_cond_wait(&cond_board[start], &lock_bus);
    if (!bus_cruising)
    {
        super_print("Passenger (%d->%d) died of boredom.", start, destination);
        pthread_mutex_unlock(&lock_bus);
        pthread_exit(NULL);
    }
    super_print("%d/%d (+)", ++passengers_on_bus, BUS_CAPACITY);
    reaction = true;

    // Passenger waits in leaving queue
    pthread_cond_wait(&cond_leave[destination], &lock_bus);
    if (!bus_cruising)
    {
        super_print("Passenger (%d->%d) was kicked off at stop %d.", start, destination, stop_current);
        pthread_mutex_unlock(&lock_bus);
        pthread_exit(NULL);
    }
    super_print("%d/%d (-)", --passengers_on_bus, BUS_CAPACITY);
    reaction = true;

    pthread_mutex_unlock(&lock_bus);
    pthread_exit(NULL);
}

int projekt_zso()
{
    pthread_t bus;
    pthread_t passengers[PASSENGER_TOTAL];
    int i = 0;

    srand(SEED);
    
    for (; i < PASSENGER_TOTAL && bus_cruising; i++)
    {
        msleep(PASSENGER_EVERY);
        pthread_create(&passengers[i], NULL, passenger_thread, NULL);
    }

    pthread_create(&bus, NULL, bus_thread, NULL);

    for (int j = 0; j < i; j++)
    {
        pthread_join(passengers[i], NULL);
    }

    pthread_join(bus, NULL);
    
    return 0;
}
