/*
 * signup.c
 * --------
 * MENU-1, option 1: create a new login and store it in usr_logins.txt
 * (one login per line:   username password )
 */
#include "train.h"

/*
 * Checks the password rules and says which rule is broken (if any):
 * at least 8 characters, 1 CAPITAL, 1 small, 1 digit, 1 special character.
 */
PasswordStatus check_password(const char *password)
{
    int i;
    int has_upper = 0, has_lower = 0, has_digit = 0, has_special = 0;

    if (strlen(password) < MIN_PASSWORD_LEN)
        return PASSWORD_TOO_SHORT;
    if (strlen(password) > MAX_USER_LEN)
        return PASSWORD_TOO_LONG;

    for (i = 0; password[i] != '\0'; i++) {
        unsigned char c = (unsigned char)password[i];

        if (isspace(c))
            return PASSWORD_HAS_SPACE;
        else if (isupper(c))
            has_upper = 1;
        else if (islower(c))
            has_lower = 1;
        else if (isdigit(c))
            has_digit = 1;
        else
            has_special = 1;             /* anything else: @ # $ % ! ... */
    }

    if (!has_upper)   return PASSWORD_NO_UPPER;
    if (!has_lower)   return PASSWORD_NO_LOWER;
    if (!has_digit)   return PASSWORD_NO_DIGIT;
    if (!has_special) return PASSWORD_NO_SPECIAL;
    return PASSWORD_OK;
}

/* Prints a friendly message for the broken password rule. */
static void print_password_error(PasswordStatus status)
{
    switch (status) {
    case PASSWORD_TOO_SHORT:
        printf("  ERROR: password must be at least %d characters long.\n", MIN_PASSWORD_LEN);
        break;
    case PASSWORD_TOO_LONG:
        printf("  ERROR: password can have at most %d characters.\n", MAX_USER_LEN);
        break;
    case PASSWORD_HAS_SPACE:
        printf("  ERROR: password must not contain spaces.\n");
        break;
    case PASSWORD_NO_UPPER:
        printf("  ERROR: password needs at least one UPPERCASE letter.\n");
        break;
    case PASSWORD_NO_LOWER:
        printf("  ERROR: password needs at least one lowercase letter.\n");
        break;
    case PASSWORD_NO_DIGIT:
        printf("  ERROR: password needs at least one digit (0-9).\n");
        break;
    case PASSWORD_NO_SPECIAL:
        printf("  ERROR: password needs at least one special character (@ # $ %% ! ...).\n");
        break;
    default:
        break;
    }
}

/* Returns 1 if this USR_NAME is already in usr_logins.txt. */
static int username_exists(const char *username)
{
    FILE *fp = fopen(USER_FILE, "r");
    char file_user[MAX_USER_LEN + 1];
    char file_pass[MAX_USER_LEN + 1];
    int found = 0;

    if (fp == NULL)                       /* no file yet = nobody exists */
        return 0;

    while (fscanf(fp, "%29s %29s", file_user, file_pass) == 2) {
        if (same_text_ignore_case(file_user, username)) {
            found = 1;
            break;
        }
    }
    fclose(fp);
    return found;
}

/* Adds one login line at the end of usr_logins.txt. */
static void save_login(const char *username, const char *password)
{
    FILE *fp = fopen(USER_FILE, "a");

    if (fp == NULL) {
        printf("ERROR: cannot open %s for writing.\n", USER_FILE);
        return;
    }
    fprintf(fp, "%s %s\n", username, password);
    fclose(fp);
}

/* Keeps asking until the user gives a valid username that is not taken yet. */
static void ask_new_username(char *username)
{
    char text[100];

    while (1) {
        read_text("Enter USR_NAME: ", text, sizeof(text));

        if (strlen(text) == 0)
            printf("  ERROR: the name cannot be empty.\n");
        else if (strchr(text, ' ') != NULL)
            printf("  ERROR: the name must not contain spaces.\n");
        else if (strlen(text) > MAX_USER_LEN)
            printf("  ERROR: the name can have at most %d characters.\n", MAX_USER_LEN);
        else if (username_exists(text))
            printf("  ERROR: USR_NAME '%s' is already taken. Please give another one.\n", text);
        else
            break;                        /* a good, unique name */
    }
    strcpy(username, text);
}

/* The whole sign-up: unique username, then a strong password (5 tries). */
void do_signup(void)
{
    char username[MAX_USER_LEN + 1];
    char password[100];
    int tries;
    PasswordStatus status;

    printf("\n----------------- SIGN-UP -----------------\n");
    ask_new_username(username);

    printf("\nPassword rules: at least %d characters with 1 uppercase,\n", MIN_PASSWORD_LEN);
    printf("1 lowercase, 1 digit and 1 special character.\n");

    for (tries = 1; tries <= MAX_PASSWORD_TRIES; tries++) {
        read_text("Enter PASSWORD: ", password, sizeof(password));
        status = check_password(password);

        if (status == PASSWORD_OK) {
            save_login(username, password);
            printf("\nSign-up successful! You can now sign-in as '%s'.\n", username);
            return;
        }
        print_password_error(status);
        printf("  (wrong password attempt %d of %d)\n", tries, MAX_PASSWORD_TRIES);
    }

    printf("\nTIME OUT! Too many wrong passwords. Please sign-up again.\n");
}

