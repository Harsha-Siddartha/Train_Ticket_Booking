/*
 * signin.c
 * --------
 * MENU-1, option 2: log in with an existing USR_NAME and PASSWORD.
 *
 * The user gets 1 first try + MAX_LOGIN_RETRIES (3) more tries.
 * After all of them fail, the app blocks sign-in for 5 minutes.
 */
#include "train.h"

static time_t locked_until = 0;       /* time until which sign-in is blocked */

/* Returns 1 if usr_logins.txt has a line with this exact name AND password. */
static int check_login(const char *username, const char *password)
{
    FILE *fp = fopen(USER_FILE, "r");
    char file_user[MAX_USER_LEN + 1];
    char file_pass[MAX_USER_LEN + 1];
    int found = 0;

    if (fp == NULL)
        return 0;

    while (fscanf(fp, "%29s %29s", file_user, file_pass) == 2) {
        if (strcmp(file_user, username) == 0 && strcmp(file_pass, password) == 0) {
            found = 1;
            break;
        }
    }
    fclose(fp);
    return found;
}

/* Returns 1 if sign-in is still blocked, and tells the user how long to wait. */
static int is_locked(void)
{
    time_t now = time(NULL);
    long seconds_left;

    if (now >= locked_until)
        return 0;

    seconds_left = (long)(locked_until - now);
    printf("\nSign-in is blocked. Please wait %ld min %ld sec and try again.\n",
           seconds_left / 60, seconds_left % 60);
    return 1;
}

/* Asks for the login details. Returns 1 if the login is correct, else 0. */
int do_signin(void)
{
    char username[100];
    char password[100];
    int attempt;
    int total_attempts = 1 + MAX_LOGIN_RETRIES;

    printf("\n----------------- SIGN-IN -----------------\n");
    if (is_locked())
        return 0;

    for (attempt = 1; attempt <= total_attempts; attempt++) {
        read_text("USR_NAME: ", username, sizeof(username));
        read_text("PASSWORD: ", password, sizeof(password));

        if (check_login(username, password)) {
            snprintf(current_user, sizeof(current_user), "%.29s", username);   /* a matching name is always <= 29 chars */
            printf("\nLogin successful. Welcome, %s!\n", current_user);
            return 1;
        }

        if (attempt < total_attempts)
            printf("  Wrong USR_NAME or PASSWORD. You can try %d more time(s).\n",
                   total_attempts - attempt);
    }

    printf("\nYou have reached max no. of attempts, please wait for 5 mins to login again.\n");
    locked_until = time(NULL) + LOCK_SECONDS;
    return 0;
}

