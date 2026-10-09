/*
 * booking.c
 * ---------
 * MENU-2, option B: show the booking details of the seats the user asks for.
 */
#include "train.h"

/* Prints one passenger as a table row. */
static void print_passenger_row(const Passenger *p)
{
    printf("%-8d %-6d %-26s %-4d %c\n", p->pnr, p->seat_number,
           p->name, p->age, p->gender);
}

/* The whole "booking details" task. */
void show_booking_details(void)
{
    Train *train;
    char date[11];
    int count, i, seat;
    Passenger *p;

    printf("\n---------------- BOOKING DETAILS ----------------\n");

    train = ask_train();
    if (train == NULL)
        return;
    if (!ask_date(date, 0))
        return;

    count = read_int("How many seat numbers do you want to check? ");
    if (count < 1 || count > train->total_seats) {
        printf("  ERROR: that is not a valid number of seats.\n");
        return;
    }

    printf("\nTrain %d (%s) on %s\n", train->train_number, train->train_name, date);
    printf("%-8s %-6s %-26s %-4s %s\n", "PNR", "SEAT", "NAME", "AGE", "G");
    printf("---------------------------------------------------\n");

    for (i = 1; i <= count; i++) {
        seat = read_int("  Seat number: ");
        p = find_passenger(train->train_number, date, seat);
        if (p != NULL)
            print_passenger_row(p);
        else
            printf("%-8s %-6d (not booked)\n", "-", seat);
    }
}

