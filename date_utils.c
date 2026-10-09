/*
 * date_utils.c
 * ------------
 * Everything about dates. A date is written as DD-MM-YYYY, e.g. 15-10-2026.
 *
 * Rule of the project: a ticket can only be reserved for a journey
 * from TODAY up to MAX_DAYS_AHEAD (3) days later.
 */
#include "train.h"

/*
 * Checks that `text` is a real date in DD-MM-YYYY form.
 * If yes: returns 1 and puts that day (at noon) in *journey_time.
 * If not (wrong format, or a date like 31-02-2026): returns 0.
 */
static int read_date(const char *text, time_t *journey_time)
{
    int i, day, month, year;
    struct tm journey;

    /* 1. the shape must be DD-MM-YYYY */
    if (strlen(text) != 10)
        return 0;
    for (i = 0; i < 10; i++) {
        if (i == 2 || i == 5) {
            if (text[i] != '-')
                return 0;
        } else if (!isdigit((unsigned char)text[i])) {
            return 0;
        }
    }
    if (sscanf(text, "%d-%d-%d", &day, &month, &year) != 3)
        return 0;

    /* 2. let the C library build the date; it fixes impossible dates,
          so if the numbers change afterwards the date was not real */
    memset(&journey, 0, sizeof(journey));
    journey.tm_mday = day;
    journey.tm_mon = month - 1;
    journey.tm_year = year - 1900;
    journey.tm_hour = 12;               /* noon: avoids day-light-saving trouble */
    journey.tm_isdst = -1;
    *journey_time = mktime(&journey);

    if (*journey_time == (time_t)-1 || journey.tm_mday != day
        || journey.tm_mon != month - 1 || journey.tm_year != year - 1900)
        return 0;
    return 1;
}

/* Returns: (the given day) minus (today), in whole days. Past days are negative. */
static int days_from_today(time_t journey_time)
{
    time_t now = time(NULL);
    time_t today_time;
    struct tm today;
    double days;

    today = *localtime(&now);
    today.tm_hour = 12;                 /* today also at noon */
    today.tm_min = 0;
    today.tm_sec = 0;
    today.tm_isdst = -1;
    today_time = mktime(&today);

    days = difftime(journey_time, today_time) / 86400.0;
    if (days >= 0)
        return (int)(days + 0.5);       /* round to a whole number */
    return -(int)(-days + 0.5);
}

/* For RESERVING: the date must be real AND from today to today+3 days. */
DateStatus check_journey_date(const char *text)
{
    time_t journey_time;
    int difference;

    if (!read_date(text, &journey_time))
        return DATE_BAD_FORMAT;
    difference = days_from_today(journey_time);
    if (difference < 0)
        return DATE_IN_PAST;
    if (difference > MAX_DAYS_AHEAD)
        return DATE_TOO_FAR;
    return DATE_OK;
}

/* For CANCEL / DETAILS: the date only has to be a real date. */
int is_valid_date(const char *text)
{
    time_t journey_time;

    return read_date(text, &journey_time);
}

/* Prints a friendly message for a wrong date. */
void print_date_error(DateStatus status)
{
    if (status == DATE_BAD_FORMAT)
        printf("  ERROR: not a valid date. Please use DD-MM-YYYY, e.g. 15-10-2026.\n");
    else if (status == DATE_IN_PAST)
        printf("  ERROR: that date is already over. Choose today or a later date.\n");
    else if (status == DATE_TOO_FAR)
        printf("  ERROR: tickets can only be reserved within %d days from today.\n",
               MAX_DAYS_AHEAD);
}

/*
 * Asks the user for the date of journey.
 * check_window = 1 -> also apply the "within 3 days" rule (reserve)
 * check_window = 0 -> any real date is fine (cancel, details)
 * Returns 1 and fills `date` (needs room for 11 chars) if the date is good.
 */
int ask_date(char *date, int check_window)
{
    char text[100];
    DateStatus status;

    if (check_window)
        printf("(Journey date must be from today up to %d days later)\n", MAX_DAYS_AHEAD);
    read_text("Date of journey (DD-MM-YYYY): ", text, sizeof(text));

    if (check_window)
        status = check_journey_date(text);
    else
        status = is_valid_date(text) ? DATE_OK : DATE_BAD_FORMAT;

    if (status != DATE_OK) {
        print_date_error(status);
        return 0;
    }
    strcpy(date, text);          /* safe: a valid date is exactly 10 characters */
    return 1;
}

