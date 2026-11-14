

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_STUDENTS  100
#define NUM_SEMESTERS 8
#define MAX_YEAR      4
#define NOT_ENTERED   (-1.0f)

struct Student {
    char  sapId[20];
    char  rollNo[20];
    char  name[50];
    char  phone[16];
    char  email[50];
    int   year;                      /* 1 to 4 */
    int   semester;                  /* current semester, 1 to 8 */
    float sgpa[NUM_SEMESTERS];       /* NOT_ENTERED (-1) if unavailable */
};

static struct Student students[MAX_STUDENTS];
static int studentCount = 0;

/* ---------- Prototypes ---------- */
void  readText(const char *prompt, char *buffer, size_t size);
int   readInt(const char *prompt, int min, int max);
float readSGPA(const char *prompt);

/* ---------- Input helpers ---------- */
static void readLine(const char *prompt, char *buffer, size_t size) {
    printf("%s", prompt);
    if (fgets(buffer, (int)size, stdin) == NULL) {
        printf("\nInput closed. Exiting.\n");
        exit(0);
    }
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    } else {
        int ch;                                   /* discard overly long input */
        while ((ch = getchar()) != '\n' && ch != EOF) { }
    }
}

static void trim(char *s) {
    size_t start = 0, len = strlen(s);
    while (s[start] && isspace((unsigned char)s[start])) start++;
    if (start > 0) memmove(s, s + start, len - start + 1);
    len = strlen(s);
    while (len > 0 && isspace((unsigned char)s[len - 1])) s[--len] = '\0';
}

void readText(const char *prompt, char *buffer, size_t size) {
    while (1) {
        readLine(prompt, buffer, size);
        trim(buffer);
        if (buffer[0] != '\0') return;
        printf("Input cannot be empty. Try again.\n");
    }
}

int readInt(const char *prompt, int min, int max) {
    char line[64];
    while (1) {
        readLine(prompt, line, sizeof(line));
        char *end;
        long v = strtol(line, &end, 10);
        while (*end && isspace((unsigned char)*end)) end++;
        if (end != line && *end == '\0' && v >= min && v <= max) return (int)v;
        printf("Invalid input. Enter a whole number between %d and %d.\n", min, max);
    }
}

/* Reads an SGPA in 0-10, or -1 to mark the semester as not available */
float readSGPA(const char *prompt) {
    char line[64];
    while (1) {
        readLine(prompt, line, sizeof(line));
        char *end;
        double v = strtod(line, &end);
        while (*end && isspace((unsigned char)*end)) end++;
        if (end != line && *end == '\0' && ((v >= 0.0 && v <= 10.0) || v == -1.0))
            return (float)v;
        printf("Invalid input. Enter SGPA between 0 and 10, or -1 if not available.\n");
    }
}

static int isDigitsOnly(const char *s) {
    for (; *s; s++) if (!isdigit((unsigned char)*s)) return 0;
    return 1;
}

static int isValidEmail(const char *s) {
    const char *at = strchr(s, '@');
    if (at == NULL || at == s || strchr(s, ' ') != NULL) return 0;
    const char *dot = strchr(at, '.');
    return dot != NULL && dot > at + 1 && dot[1] != '\0';
}

static int compareIgnoreCase(const char *a, const char *b) {
    while (*a && *b) {
        int ca = tolower((unsigned char)*a), cb = tolower((unsigned char)*b);
        if (ca != cb) return ca - cb;
        a++; b++;
    }
    return tolower((unsigned char)*a) - tolower((unsigned char)*b);
}


int main(void) {
    int choice;
    do {
        printf("\n========== STUDENT RECORD MANAGEMENT SYSTEM ==========\n");
        printf("1. Add Student\n2. Search Student\n3. Calculate Student CGPA\n");
        printf("4. Display Average CGPA by Year\n5. Display All Students\n");
        printf("6. Delete Student\n7. Display Students by SAP ID (increasing)\n");
        printf("8. Display Students by Name (increasing)\n9. AI-Assisted Academic Advisor\n10. Exit\n");
        choice = readInt("Enter your choice: ", 1, 10);
        switch (choice) {
            case 1: printf("This feature is not implemented yet.\n"); break;
            case 2: printf("This feature is not implemented yet.\n"); break;
            case 3: printf("This feature is not implemented yet.\n"); break;
            case 4: printf("This feature is not implemented yet.\n"); break;
            case 5: printf("This feature is not implemented yet.\n"); break;
            case 6: printf("This feature is not implemented yet.\n"); break;
            case 7: printf("This feature is not implemented yet.\n"); break;
            case 8: printf("This feature is not implemented yet.\n"); break;
            case 9: printf("This feature is not implemented yet.\n"); break;
            case 10: printf("Thank you! Goodbye.\n"); break;
        }
    } while (choice != 10);
    return 0;
}
