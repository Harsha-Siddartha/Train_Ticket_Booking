/*
 * train_file.c
 * ------------
 * Reads and writes trains_info.txt. One train per line, fields separated by '|':
 *
 *   12951|Mumbai Rajdhani|Mumbai|New Delhi|100|100
 *   number|name|source|destination|total_seats|available_seats
 *
 * (We use '|' so that names with spaces, like "Mumbai Rajdhani", work.)
 */
#include "train.h"

/* Reads the whole file and builds the linked list. Called once at start. */
void load_trains(void)
{
    FILE *fp;
    Train *train;
    int fields_read;

    fp = fopen(TRAIN_FILE, "r");
    if (fp == NULL) {
        printf("(No %s found - starting with no trains.)\n", TRAIN_FILE);
        return;
    }

    while (1) {
        train = (Train *) malloc(sizeof(Train));
        if (train == NULL) {
            printf("Out of memory while loading trains!\n");
            break;
        }
        /* %49[^|] means: read up to 49 characters, stop at a '|' */
        fields_read = fscanf(fp, "%d|%49[^|]|%29[^|]|%29[^|]|%d|%d",
                             &train->train_number, train->train_name,
                             train->source, train->destination,
                             &train->total_seats, &train->available_seats);
        if (fields_read != 6) {          /* end of file (or a broken line) */
            free(train);
            break;
        }
        add_train_to_list(train);
    }
    fclose(fp);
}

/* Adds ONE train at the end of the file (used when the admin adds a train). */
void save_train(Train *train)
{
    FILE *fp = fopen(TRAIN_FILE, "a");       /* "a" = append */

    if (fp == NULL) {
        printf("ERROR: cannot open %s for writing.\n", TRAIN_FILE);
        return;
    }
    fprintf(fp, "%d|%s|%s|%s|%d|%d\n", train->train_number, train->train_name,
            train->source, train->destination, train->total_seats,
            train->available_seats);
    fclose(fp);
}

/* Rewrites the WHOLE file from the linked list (used after seats change). */
void sync_train_file(void)
{
    FILE *fp = fopen(TRAIN_FILE, "w");       /* "w" = start a new empty file */
    Train *current = train_head;

    if (fp == NULL) {
        printf("ERROR: cannot open %s for writing.\n", TRAIN_FILE);
        return;
    }
    while (current != NULL) {
        fprintf(fp, "%d|%s|%s|%s|%d|%d\n", current->train_number,
                current->train_name, current->source, current->destination,
                current->total_seats, current->available_seats);
        current = current->next;
    }
    fclose(fp);
}

