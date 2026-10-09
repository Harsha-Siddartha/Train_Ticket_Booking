/*
 * cancel.c
 * --------
 * MENU-2, option C: cancel tickets.
 * The cancelled passengers are removed from the list AND from the file,
 * and the free-seat number of the train goes up again.
 */
#include "train.h"

/* The whole "cancel ticket" task. */
void cancel_ticket(void)
{
    Train *train;
    char date[11];
    int count, i, seat, cancelled = 0;

    printf("\n---------------- CANCEL TICKET ----------------\n");

    train = ask_train();
    if (train == NULL)
        return;
    if (!ask_date(date, 0))               /* 0 = any real date is fine */
        return;

    count = read_int("How many seats do you want to cancel? ");
    if (count < 1 || count > train->total_seats) {
        printf("  ERROR: that is not a valid number of seats.\n");
        return;
    }

    for (i = 1; i <= count; i++) {
        seat = read_int("  Seat number to cancel: ");
        if (remove_passenger(train->train_number, date, seat)) {
            printf("  -> Seat %d is cancelled.\n", seat);
            cancelled++;
        } else {
            printf("  -> Seat %d is not booked on this train/date. Nothing to cancel.\n", seat);
        }
    }

    if (cancelled > 0) {
        sync_passenger_file();            /* rewrite passengers_info.txt */
        update_available_seats(train, date);
        sync_train_file();                /* free seats go up in trains_info.txt */
    }
    printf("\n%d seat(s) cancelled. Seats free on %s: %d\n",
           cancelled, date, train->available_seats);
}

