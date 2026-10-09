/*
 * train.h
 * -------
 * The ONE shared header file of the project.
 * Every .c file includes it. It contains:
 *    1. the standard libraries we use
 *    2. constants (file names, limits)
 *    3. the structures (Train, Passenger)  -> used as Singly Linked Lists
 *    4. the enums (named results for passwords and dates)
 *    5. the list of all functions, grouped by the .c file they live in
 */
#ifndef TRAIN_H
#define TRAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

/* ---------- 2. constants ---------- */
#define USER_FILE        "usr_logins.txt"       /* usernames + passwords  */
#define TRAIN_FILE       "trains_info.txt"      /* all trains             */
#define PASSENGER_FILE   "passengers_info.txt"  /* all booked passengers  */

#define ADMIN_NAME         "admin"   /* the special user who can add trains */
#define MAX_USER_LEN       29        /* longest username / password         */
#define MIN_PASSWORD_LEN   8
#define MAX_PASSWORD_TRIES 5         /* sign-up: wrong passwords allowed    */
#define MAX_LOGIN_RETRIES  3         /* sign-in: extra tries after 1st fail */
#define LOCK_SECONDS       300       /* 5 minutes                           */
#define MAX_DAYS_AHEAD     3         /* tickets only for today + 3 days     */
#define FIRST_PNR          1001      /* the first PNR number ever given     */

/* ---------- 3. structures (each one is a node of a linked list) ---------- */
typedef struct Train {
    int  train_number;
    char train_name[50];
    char source[30];
    char destination[30];
    int  total_seats;
    int  available_seats;
    struct Train *next;          /* SLL: pointer to the next train */
} Train;

typedef struct Passenger {
    int  pnr;                    /* booking number (same for one booking) */
    int  train_number;
    char journey_date[11];       /* DD-MM-YYYY                            */
    int  seat_number;
    char name[50];
    int  age;
    char gender;                 /* 'M' or 'F'                            */
    struct Passenger *next;      /* SLL: pointer to the next passenger    */
} Passenger;

/* ---------- 4. enums ---------- */
typedef enum {                   /* result of checking a password */
    PASSWORD_OK,
    PASSWORD_TOO_SHORT,
    PASSWORD_TOO_LONG,
    PASSWORD_HAS_SPACE,
    PASSWORD_NO_UPPER,
    PASSWORD_NO_LOWER,
    PASSWORD_NO_DIGIT,
    PASSWORD_NO_SPECIAL
} PasswordStatus;

typedef enum {                   /* result of checking a journey date */
    DATE_OK,
    DATE_BAD_FORMAT,
    DATE_IN_PAST,
    DATE_TOO_FAR
} DateStatus;

/* ---------- global variables (defined in main.c) ---------- */
extern Train     *train_head;        /* first node of the train list      */
extern Passenger *passenger_head;    /* first node of the passenger list  */
extern char       current_user[30];  /* name of the logged-in user        */

/* ---------- 5. functions ---------- */

/* main.c */
void save_and_exit(void);

/* utils.c */
void read_line(char *buffer, int size);
void read_text(const char *prompt, char *dest, int size);
int  read_int(const char *prompt);
char read_char_upper(const char *prompt);
void trim_spaces(char *text);
int  same_text_ignore_case(const char *a, const char *b);

/* date_utils.c */
DateStatus check_journey_date(const char *text);
int  is_valid_date(const char *text);
void print_date_error(DateStatus status);
int  ask_date(char *date, int check_window);

/* train_list.c */
void   add_train_to_list(Train *new_train);
Train *find_train(int train_number);
int    train_name_exists(const char *name);
Train *ask_train(void);
void   print_all_trains(void);
void   update_available_seats(Train *train, const char *date);
void   free_train_list(void);

/* train_file.c */
void load_trains(void);
void save_train(Train *train);
void sync_train_file(void);

/* passenger_list.c */
void       add_passenger_to_list(Passenger *new_passenger);
int        count_booked_seats(int train_number, const char *date);
Passenger *find_passenger(int train_number, const char *date, int seat);
int        next_free_seat(int train_number, const char *date);
int        next_pnr(void);
int        remove_passenger(int train_number, const char *date, int seat);
void       free_passenger_list(void);

/* passenger_file.c */
void load_passengers(void);
void save_passenger(Passenger *passenger);
void sync_passenger_file(void);

/* signup.c */
PasswordStatus check_password(const char *password);
void do_signup(void);

/* signin.c */
int do_signin(void);

/* reserve.c, cancel.c, booking.c, admin.c */
void reserve_ticket(void);
void cancel_ticket(void);
void show_booking_details(void);
void add_new_train(void);

#endif

