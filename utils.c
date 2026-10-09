/*
 * utils.c
 * -------
 * Small helper functions used everywhere: safe keyboard input and text tools.
 *
 * WHY NOT scanf()?  scanf leaves the Enter key in the input and breaks when
 * the user types a wrong thing. Reading a whole line with fgets() and then
 * checking it is much safer for a beginner project.
 */
#include "train.h"

/*
 * Reads one line from the keyboard into `buffer` (without the Enter key).
 * If the keyboard input has ended (Ctrl+D / Ctrl+Z), we save the data and quit,
 * so the program can never get stuck in an endless loop.
 */
void read_line(char *buffer, int size)
{
    int ch;

    if (fgets(buffer, size, stdin) == NULL) {
        printf("\nNo more input. Saving data and closing the app...\n");
        save_and_exit();
    }

    /* no Enter found: the user typed more than fits, throw the rest away */
    if (strchr(buffer, '\n') == NULL) {
        while ((ch = getchar()) != '\n' && ch != EOF) {
            /* do nothing, just read and ignore */
        }
    }
    buffer[strcspn(buffer, "\r\n")] = '\0';    /* cut the Enter key off */
}

/* Removes spaces from the start and the end of a text. */
void trim_spaces(char *text)
{
    int start = 0;
    int end = strlen(text);

    while (end > 0 && isspace((unsigned char)text[end - 1]))
        end--;
    text[end] = '\0';

    while (text[start] != '\0' && isspace((unsigned char)text[start]))
        start++;
    if (start > 0)
        memmove(text, text + start, strlen(text + start) + 1);
}

/*
 * Shows a question and reads a text answer into `dest`.
 * The '|' character is replaced by a space, because we use '|' to
 * separate the fields inside our data files.
 */
void read_text(const char *prompt, char *dest, int size)
{
    int i;

    printf("%s", prompt);
    read_line(dest, size);
    trim_spaces(dest);

    for (i = 0; dest[i] != '\0'; i++)
        if (dest[i] == '|')
            dest[i] = ' ';
}

/* Shows a question and keeps asking until the user types a whole number. */
int read_int(const char *prompt)
{
    char line[100];
    char *end;
    long value;

    while (1) {
        printf("%s", prompt);
        read_line(line, sizeof(line));
        trim_spaces(line);

        value = strtol(line, &end, 10);
        if (end != line && *end == '\0' && value >= 0 && value < 1000000)
            return (int)value;

        printf("  Please type a valid whole number.\n");
    }
}

/* Shows a question and returns the first letter typed, in CAPITAL letters. */
char read_char_upper(const char *prompt)
{
    char line[100];

    printf("%s", prompt);
    read_line(line, sizeof(line));
    trim_spaces(line);
    return (char)toupper((unsigned char)line[0]);   /* '\0' if nothing typed */
}

/* Returns 1 if two texts are the same, ignoring CAPITAL / small letters. */
int same_text_ignore_case(const char *a, const char *b)
{
    int i = 0;

    while (a[i] != '\0' && b[i] != '\0') {
        if (tolower((unsigned char)a[i]) != tolower((unsigned char)b[i]))
            return 0;
        i++;
    }
    return a[i] == b[i];      /* both must end together */
}

