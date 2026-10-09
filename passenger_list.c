/*
 * passenger_list.c
 * ----------------
 * Work with the Singly Linked List of passengers in memory.
 *
 *   passenger_head --> [seat 1] --> [seat 2] --> ... --> NULL
 *
 * One passenger node = one booked seat of one train on one date.
 */
#include "train.h"

/* Adds a new passenger node at the END of the list. */
void add_passenger_to_list(Passenger *new_passenger)
{
    Passenger *current;

    new_passenger->next = NULL;

    if (passenger_head == NULL) {
        passenger_head = new_passenger;
        return;
    }
    current = passenger_head;
    while (current->next != NULL)
        current = current->next;
    current->next = new_passenger;
}

/* Counts the seats already booked on one train for one date. */
int count_booked_seats(int train_number, const char *date)
{
    Passenger *current = passenger_head;
    int count = 0;

    while (current != NULL) {
        if (current->train_number == train_number
            && strcmp(current->journey_date, date) == 0)
            count++;
        current = current->next;
    }
    return count;
}

/* Finds the passenger sitting in one seat (train + date + seat). NULL if empty. */
Passenger *find_passenger(int train_number, const char *date, int seat)
{
    Passenger *current = passenger_head;

    while (current != NULL) {
        if (current->train_number == train_number
            && strcmp(current->journey_date, date) == 0
            && current->seat_number == seat)
            return current;
        current = current->next;
    }
    return NULL;
}

/*
 * Gives the smallest seat number (1, 2, 3 ...) that is still free.
 * After a cancellation, the free seat number is used again.
 */
int next_free_seat(int train_number, const char *date)
{
    int seat = 1;

    while (find_passenger(train_number, date, seat) != NULL)
        seat++;
    return seat;
}

/* Gives a new PNR number: one more than the biggest PNR used so far. */
int next_pnr(void)
{
    Passenger *current = passenger_head;
    int biggest = FIRST_PNR - 1;

    while (current != NULL) {
        if (current->pnr > biggest)
            biggest = current->pnr;
        current = current->next;
    }
    return biggest + 1;
}

/*
 * Removes one passenger (one seat) from the list.
 * Returns 1 if it was removed, 0 if that seat was not booked.
 */
int remove_passenger(int train_number, const char *date, int seat)
{
    Passenger *current = passenger_head;
    Passenger *previous = NULL;

    while (current != NULL) {
        if (current->train_number == train_number
            && strcmp(current->journey_date, date) == 0
            && current->seat_number == seat) {

            if (previous == NULL)                /* removing the first node */
                passenger_head = current->next;
            else                                 /* skip over the node      */
                previous->next = current->next;

            free(current);
            return 1;
        }
        previous = current;
        current = current->next;
    }
    return 0;
}

/* Gives all the passenger nodes back to the system (free). */
void free_passenger_list(void)
{
    Passenger *current = passenger_head;
    Passenger *next_node;

    while (current != NULL) {
        next_node = current->next;
        free(current);
        current = next_node;
    }
    passenger_head = NULL;
}

