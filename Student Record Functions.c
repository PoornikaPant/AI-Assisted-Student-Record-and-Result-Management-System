

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
int   findStudent(const char *id);
float calculateCGPA(struct Student s);
void  displayStudent(struct Student s);
void  addStudent(void);
void  searchStudent(void);
void  displayAll(void);
void  deleteStudent(void);

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


int findStudent(const char *id) {
    for (int i = 0; i < studentCount; i++)
        if (strcmp(students[i].sapId, id) == 0) return i;
    return -1;
}

/* Average of the available SGPA values; -1 if none are available */
float calculateCGPA(struct Student s) {
    float total = 0;
    int n = 0;
    for (int i = 0; i < NUM_SEMESTERS; i++) {
        if (s.sgpa[i] >= 0) {
            total += s.sgpa[i];
            n++;
        }
    }
    return n > 0 ? total / n : -1.0f;
}

void displayStudent(struct Student s) {
    static const char *yearName[] = {"", "1st", "2nd", "3rd", "4th"};
    printf("SAP ID   : %s\n", s.sapId);
    printf("Roll No. : %s\n", s.rollNo);
    printf("Name     : %s\n", s.name);
    printf("Phone    : %s\n", s.phone);
    printf("Email    : %s\n", s.email);
    printf("Year     : %s\n", yearName[s.year]);
    printf("Semester : %d\n", s.semester);
    for (int i = 0; i < NUM_SEMESTERS; i++) {
        if (s.sgpa[i] >= 0) printf("SGPA Sem %d: %.2f\n", i + 1, s.sgpa[i]);
        else                printf("SGPA Sem %d: N/A\n", i + 1);
    }
    float cgpa = calculateCGPA(s);
    if (cgpa < 0) printf("CGPA     : N/A\n");
    else          printf("CGPA     : %.2f\n", cgpa);
}

void addStudent(void) {
    if (studentCount >= MAX_STUDENTS) {
        printf("Student list is full (max %d).\n", MAX_STUDENTS);
        return;
    }
    struct Student s;
    memset(&s, 0, sizeof(s));

    readText("Enter SAP ID: ", s.sapId, sizeof(s.sapId));
    if (findStudent(s.sapId) != -1) {
        printf("A student with this SAP ID already exists.\n");
        return;
    }
    readText("Enter Roll No.: ", s.rollNo, sizeof(s.rollNo));
    for (int i = 0; i < studentCount; i++) {
        if (strcmp(students[i].rollNo, s.rollNo) == 0) {
            printf("A student with this Roll No. already exists.\n");
            return;
        }
    }
    readText("Enter name: ", s.name, sizeof(s.name));

    while (1) {
        readText("Enter 10-digit phone number: ", s.phone, sizeof(s.phone));
        if (strlen(s.phone) == 10 && isDigitsOnly(s.phone)) break;
        printf("Invalid phone number.\n");
    }
    while (1) {
        readText("Enter email ID: ", s.email, sizeof(s.email));
        if (isValidEmail(s.email)) break;
        printf("Invalid email ID.\n");
    }

    s.year = readInt("Enter year (1-4): ", 1, MAX_YEAR);
    s.semester = readInt("Enter current semester: ", s.year * 2 - 1, s.year * 2);

    printf("Enter SGPA for each semester (0-10), or -1 if not available.\n");
    for (int i = 0; i < NUM_SEMESTERS; i++) {
        char prompt[40];
        snprintf(prompt, sizeof(prompt), "  SGPA for semester %d: ", i + 1);
        s.sgpa[i] = readSGPA(prompt);
    }

    students[studentCount++] = s;
    printf("Student added successfully. Total students stored: %d\n", studentCount);
}

void searchStudent(void) {
    char id[20];
    readText("Enter SAP ID to search: ", id, sizeof(id));
    int index = findStudent(id);
    if (index == -1) {
        printf("Student not found.\n");
        return;
    }
    displayStudent(students[index]);
}

void displayAll(void) {
    if (studentCount == 0) {
        printf("No student records available.\n");
        return;
    }
    for (int i = 0; i < studentCount; i++) {
        printf("\n---------- Student %d ----------\n", i + 1);
        displayStudent(students[i]);
    }
}

void deleteStudent(void) {
    char id[20];
    readText("Enter SAP ID to delete: ", id, sizeof(id));
    int index = findStudent(id);
    if (index == -1) {
        printf("Student not found.\n");
        return;
    }
    for (int i = index; i < studentCount - 1; i++) students[i] = students[i + 1];
    studentCount--;
    printf("Student deleted successfully.\n");
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
            case 1: addStudent(); break;
            case 2: searchStudent(); break;
            case 3: printf("This feature is not implemented yet.\n"); break;
            case 4: printf("This feature is not implemented yet.\n"); break;
            case 5: displayAll(); break;
            case 6: deleteStudent(); break;
            case 7: printf("This feature is not implemented yet.\n"); break;
            case 8: printf("This feature is not implemented yet.\n"); break;
            case 9: printf("This feature is not implemented yet.\n"); break;
            case 10: printf("Thank you! Goodbye.\n"); break;
        }
    } while (choice != 10);
    return 0;
}
