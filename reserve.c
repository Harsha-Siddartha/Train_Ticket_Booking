/*
 * reserve.c
 * ---------
 * MENU-2, option R: reserve tickets.
 *
 * Steps: train -> date -> number of seats -> details of every passenger.
 * Seat numbers are given automatically, starting from 1 for each train/date.
 */
#include "train.h"

/* Asks name, age and gender of ONE passenger and keeps asking until valid. */
static void ask_passenger_details(Passenger *p)
{
    char gender;

    do {
        read_text("   Name         : ", p->name, sizeof(p->name));
        if (strlen(p->name) == 0)
            printf("   The name cannot be empty.\n");
    } while (strlen(p->name) == 0);

    do {
        p->age = read_int("   Age          : ");
        if (p->age < 1 || p->age > 120)
            printf("   Age must be between 1 and 120.\n");
    } while (p->age < 1 || p->age > 120);

    do {
        gender = read_char_upper("   Gender (M/F) : ");
        if (gender != 'M' && gender != 'F')
            printf("   Please type M or F.\n");
    } while (gender != 'M' && gender != 'F');
    p->gender = gender;
}

/*
 * Creates ONE passenger node, fills it, gives it the next free seat,
 * adds it to the linked list and writes it to the file.
 * Returns 1 on success.
 */
static int book_one_passenger(Train *train, const char *date, int pnr, int number)
{
    Passenger *p = (Passenger *) malloc(sizeof(Passenger));

    if (p == NULL) {
        printf("  ERROR: out of memory!\n");
        return 0;
    }

    printf("\n Passenger %d:\n", number);
    ask_passenger_details(p);

    p->pnr = pnr;
    p->train_number = train->train_number;
    strcpy(p->journey_date, date);
    p->seat_number = next_free_seat(train->train_number, date);

    add_passenger_to_list(p);             /* into the linked list */
    save_passenger(p);                    /* into passengers_info.txt */

    printf("  -> Seat %d is booked for %s.\n", p->seat_number, p->name);
    return 1;
}

/*
 * Decides how many seats can really be booked.
 * If the user wants more than what is free, only the free seats are booked
 * and an error message is printed. Returns 0 if nothing can be booked.
 */
static int decide_seats_to_book(Train *train, const char *date, int wanted)
{
    int free_seats;

    free_seats = train->total_seats - count_booked_seats(train->train_number, date);
    printf("Seats available on %s: %d\n", date, free_seats);

    if (free_seats <= 0) {
        printf("  ERROR: sorry, this train is FULL on that date.\n");
        return 0;
    }
    if (wanted > free_seats) {
        printf("  ERROR: only %d seat(s) are available, but you asked for %d.\n",
               free_seats, wanted);
        printf("         Booking the first %d passenger(s); %d seat(s) cannot be booked.\n",
               free_seats, wanted - free_seats);
        return free_seats;
    }
    return wanted;
}

/* The whole "reserve ticket" task. */
void reserve_ticket(void)
{
    Train *train;
    char date[11];
    int wanted, to_book, pnr, i, booked = 0;

    printf("\n---------------- RESERVE TICKET ----------------\n");
    print_all_trains();

    train = ask_train();
    if (train == NULL)
        return;
    if (!ask_date(date, 1))               /* 1 = apply the "within 3 days" rule */
        return;

    wanted = read_int("No. of seats to reserve: ");
    if (wanted < 1) {
        printf("  ERROR: please ask for at least 1 seat.\n");
        return;
    }

    to_book = decide_seats_to_book(train, date, wanted);
    if (to_book == 0)
        return;

    pnr = next_pnr();
    for (i = 1; i <= to_book; i++)
        booked += book_one_passenger(train, date, pnr, i);

    update_available_seats(train, date);  /* seats left on this date */
    sync_train_file();                    /* write the new number to the file */

    printf("\nBOOKING DONE: PNR %d, %d seat(s) booked on train %d (%s) for %s.\n",
           pnr, booked, train->train_number, train->train_name, date);
    printf("Seats still free on that date: %d\n", train->available_seats);
}

