/*
 * passenger_file.c
 * ----------------
 * Reads and writes passengers_info.txt. One passenger (one seat) per line:
 *
 *   1001|12951|15-10-2026|1|Ravi Kumar|25|M
 *   pnr|train|date|seat|name|age|gender
 */
#include "train.h"

/* Reads the whole file and builds the linked list. Called once at start. */
void load_passengers(void)
{
    FILE *fp;
    Passenger *p;
    int fields_read;

    fp = fopen(PASSENGER_FILE, "r");
    if (fp == NULL) {
        printf("(No %s found - starting with no bookings.)\n", PASSENGER_FILE);
        return;
    }

    while (1) {
        p = (Passenger *) malloc(sizeof(Passenger));
        if (p == NULL) {
            printf("Out of memory while loading passengers!\n");
            break;
        }
        fields_read = fscanf(fp, "%d|%d|%10[^|]|%d|%49[^|]|%d|%c",
                             &p->pnr, &p->train_number, p->journey_date,
                             &p->seat_number, p->name, &p->age, &p->gender);
        if (fields_read != 7) {          /* end of file (or a broken line) */
            free(p);
            break;
        }
        add_passenger_to_list(p);
    }
    fclose(fp);
}

/* Adds ONE passenger at the end of the file (used right after booking). */
void save_passenger(Passenger *passenger)
{
    FILE *fp = fopen(PASSENGER_FILE, "a");

    if (fp == NULL) {
        printf("ERROR: cannot open %s for writing.\n", PASSENGER_FILE);
        return;
    }
    fprintf(fp, "%d|%d|%s|%d|%s|%d|%c\n", passenger->pnr,
            passenger->train_number, passenger->journey_date,
            passenger->seat_number, passenger->name, passenger->age,
            passenger->gender);
    fclose(fp);
}

/* Rewrites the WHOLE file from the linked list (used after a cancellation). */
void sync_passenger_file(void)
{
    FILE *fp = fopen(PASSENGER_FILE, "w");
    Passenger *current = passenger_head;

    if (fp == NULL) {
        printf("ERROR: cannot open %s for writing.\n", PASSENGER_FILE);
        return;
    }
    while (current != NULL) {
        fprintf(fp, "%d|%d|%s|%d|%s|%d|%c\n", current->pnr,
                current->train_number, current->journey_date,
                current->seat_number, current->name, current->age,
                current->gender);
        current = current->next;
    }
    fclose(fp);
}

