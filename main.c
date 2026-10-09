/*
 * main.c
 * ------
 * The starting point of the Train Ticket Booking App (mini IRCTC).
 *
 * Program flow:
 *   1. load the files into the linked lists
 *   2. show MENU-1 (sign-up / sign-in)
 *   3. after a good sign-in, show the trains and MENU-2
 *   4. on quit, save the linked lists back to the files
 */
#include "train.h"

/* the two linked lists and the logged-in user (shared by all files) */
Train     *train_head = NULL;
Passenger *passenger_head = NULL;
char       current_user[30] = "";

/* Saves everything to the files, frees the memory and ends the program. */
void save_and_exit(void)
{
    sync_train_file();
    sync_passenger_file();
    free_train_list();
    free_passenger_list();
    printf("\nAll data saved. Thank you for using MyIRCTC. Bye!\n");
    exit(0);
}

/* Prints MENU-2. The admin gets one extra line. */
static void print_menu2(void)
{
    printf("\n------------------MENU-2---------------------------\n");
    printf("R/r : To reserve ticket\n");
    printf("C/c : To cancel ticket\n");
    printf("B/b : Booking details\n");
    if (strcmp(current_user, ADMIN_NAME) == 0)
        printf("A/a : Add a new train (admin only)\n");
    printf("Q/q : Quit from app\n");
}

/* MENU-2: repeats until the user quits. */
static void menu2_loop(void)
{
    char choice;

    print_all_trains();

    while (1) {
        print_menu2();
        choice = read_char_upper("Enter your choice: ");

        if (choice == 'R')
            reserve_ticket();
        else if (choice == 'C')
            cancel_ticket();
        else if (choice == 'B')
            show_booking_details();
        else if (choice == 'A' && strcmp(current_user, ADMIN_NAME) == 0)
            add_new_train();
        else if (choice == 'Q')
            save_and_exit();
        else
            printf("  Invalid choice. Please try again.\n");
    }
}

/* MENU-1: sign-up / sign-in. Repeats until the user exits. */
static void menu1_loop(void)
{
    int choice;

    while (1) {
        printf("\n------------------MENU-1---------------------------\n");
        printf("1: sign-up ( to create new login )\n");
        printf("2: sign-in ( to login existing users )\n");
        printf("3: exit\n");
        choice = read_int("Enter your choice: ");

        if (choice == 1)
            do_signup();
        else if (choice == 2) {
            if (do_signin())
                menu2_loop();             /* ends the program when user quits */
        }
        else if (choice == 3)
            save_and_exit();
        else
            printf("  Invalid choice. Please type 1, 2 or 3.\n");
    }
}

int main(void)
{
    printf("========== WELCOME TO MyIRCTC - TRAIN TICKET BOOKING ==========\n");

    load_trains();             /* files -> linked lists */
    load_passengers();

    menu1_loop();
    return 0;
}

