# Student Record AI C

Console-based, menu-driven **AI-Assisted Student Record and Result Management System** written in C.

## Features
- Add, search, display and delete student records (SAP ID, Roll No., Name, Phone, Email, Year, Semester, SGPA for 8 semesters)
- CGPA of a student = average of the available SGPA values (semesters entered as `-1` are ignored)
- Average CGPA for each year (1st to 4th)
- Display all students in increasing order of SAP ID or Name
- AI-assisted academic advisor: if-else logic on CGPA (advice categories designed with an AI tool, then reviewed and modified by me)

## Build and run
```
gcc -Wall -Wextra -std=c99 590032465.c -o student_system
./student_system
```
Run with the sample data (5 students):
```
./student_system < tests/sample_input.txt > tests/sample_output.txt
```

## Academic advisor rules
| CGPA | Advice |
|------|--------|
| >= 9.0 | Excellent Performance |
| >= 7.5 | Good Performance |
| >= 6.0 | Needs Improvement |
| < 6.0  | Requires Immediate Attention |

## Validation
Phone must be 10 digits, email must look like `name@domain.ext`, SAP ID and Roll No. must be unique, semester must match the year, SGPA must be 0-10 (or -1 if unavailable). Maximum 100 students.

## Files
- `590032465.c` - source code 
- `AI_USAGE_LOG.md` - record of AI tool usage
- `tests/` - sample input and output
