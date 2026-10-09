/*
 * train_list.c
 * ------------
 * Work with the Singly Linked List (SLL) of trains in memory.
 *
 *   train_head --> [Train 1] --> [Train 2] --> [Train 3] --> NULL
 *
 * Every train is a "node" created with malloc(). `next` points to the
 * following node; the last node has next = NULL.
 */
#include "train.h"

/* Adds a new train node at the END of the list. */
void add_train_to_list(Train *new_train)
{
    Train *current;

    new_train->next = NULL;

    if (train_head == NULL) {            /* the list is empty */
        train_head = new_train;
        return;
    }
    current = train_head;
    while (current->next != NULL)        /* walk to the last node */
        current = current->next;
    current->next = new_train;
}

/* Searches the list for a train number. Returns the node, or NULL. */
Train *find_train(int train_number)
{
    Train *current = train_head;

    while (current != NULL) {
        if (current->train_number == train_number)
            return current;
        current = current->next;
    }
    return NULL;
}

/* Returns 1 if some train already has this name (CAPITAL/small ignored). */
int train_name_exists(const char *name)
{
    Train *current = train_head;

    while (current != NULL) {
        if (same_text_ignore_case(current->train_name, name))
            return 1;
        current = current->next;
    }
    return 0;
}

/* Asks for a train number and finds that train. Returns NULL (with a message) if none. */
Train *ask_train(void)
{
    int number;
    Train *train;

    number = read_int("Train number: ");
    train = find_train(number);
    if (train == NULL)
        printf("  ERROR: no train with number %d.\n", number);
    return train;
}

/* Prints the list of all trains as a table. */
void print_all_trains(void)
{
    Train *current = train_head;

    printf("\n%-9s %-22s %-14s %-14s %s\n",
           "TRAIN_NO", "TRAIN_NAME", "SOURCE", "DESTINATION", "TOTAL_SEATS");
    printf("-----------------------------------------------------------------------\n");

    if (current == NULL)
        printf("No trains available.\n");

    while (current != NULL) {
        printf("%-9d %-22s %-14s %-14s %d\n", current->train_number,
               current->train_name, current->source, current->destination,
               current->total_seats);
        current = current->next;
    }
    printf("\n");
}

/*
 * Recalculates the free seats of a train for ONE journey date:
 *      available = total seats - seats already booked on that date
 * and stores it in the train node (it is later written to trains_info.txt).
 */
void update_available_seats(Train *train, const char *date)
{
    train->available_seats = train->total_seats
                             - count_booked_seats(train->train_number, date);
}

/* Gives all the train nodes back to the system (free). */
void free_train_list(void)
{
    Train *current = train_head;
    Train *next_node;

    while (current != NULL) {
        next_node = current->next;       /* remember the next one BEFORE free */
        free(current);
        current = next_node;
    }
    train_head = NULL;
}

