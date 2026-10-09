/*
 * admin.c
 * -------
 * MENU-2, option A (only for the special user "admin"): add a new train.
 * Train number and train name must both be UNIQUE (no duplicate trains).
 */
#include "train.h"

/*
 * Asks all the details of a new train and fills the node `t`.
 * Returns 1 if everything is fine, 0 if something is wrong (message printed).
 */
static int fill_train_details(Train *t)
{
    t->train_number = read_int("Train number: ");
    if (t->train_number < 1) {
        printf("  ERROR: the train number must be 1 or more.\n");
        return 0;
    }
    if (find_train(t->train_number) != NULL) {
        printf("  ERROR: a train with number %d already exists.\n", t->train_number);
        return 0;
    }

    read_text("Train name: ", t->train_name, sizeof(t->train_name));
    if (strlen(t->train_name) == 0) {
        printf("  ERROR: the train name cannot be empty.\n");
        return 0;
    }
    if (train_name_exists(t->train_name)) {
        printf("  ERROR: a train named '%s' already exists.\n", t->train_name);
        return 0;
    }

    read_text("Source station: ", t->source, sizeof(t->source));
    read_text("Destination station: ", t->destination, sizeof(t->destination));
    if (strlen(t->source) == 0 || strlen(t->destination) == 0) {
        printf("  ERROR: source and destination cannot be empty.\n");
        return 0;
    }

    t->total_seats = read_int("Total seats: ");
    if (t->total_seats < 1) {
        printf("  ERROR: a train needs at least 1 seat.\n");
        return 0;
    }
    t->available_seats = t->total_seats;      /* nothing is booked yet */
    return 1;
}

/* The whole "add new train" task. */
void add_new_train(void)
{
    Train *t;

    if (strcmp(current_user, ADMIN_NAME) != 0) {
        printf("  ERROR: only the admin can add trains.\n");
        return;
    }

    printf("\n---------------- ADD NEW TRAIN ----------------\n");
    t = (Train *) malloc(sizeof(Train));
    if (t == NULL) {
        printf("  ERROR: out of memory!\n");
        return;
    }

    if (!fill_train_details(t)) {
        free(t);                              /* give the memory back */
        return;
    }

    add_train_to_list(t);                     /* into the linked list */
    save_train(t);                            /* into trains_info.txt */
    printf("\nTrain %d (%s) added successfully.\n", t->train_number, t->train_name);
}

