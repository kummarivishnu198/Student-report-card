# Student Report Card Management System (C++)

A menu-driven Student Report Card Management System built with **C++17, OOP, and SQLite**. Student details and subject marks are stored in a local database, so records remain available after the program closes.

## Features

- Add, list, search, and delete students
- Add or update marks for each subject
- Generate a report card with total, percentage, and grade
- View a class summary
- SQLite persistence with prepared statements and foreign-key deletion
- Input validation and error handling

## OOP concepts demonstrated

- **Encapsulation:** student details and report-card marks are managed through classes.
- **Inheritance:** `Student` inherits from the abstract `Person` class.
- **Runtime polymorphism:** `Student::display()` overrides the virtual `Person::display()`.
- **Abstraction:** database operations are exposed through the `Database` class.
- **Composition:** a `ReportCard` holds a collection of subject marks.

## Requirements

- C++17 compiler (GCC, Clang, or Visual Studio 2019+)
- CMake 3.16+
- SQLite3 development package (headers and library)

### Windows

Install a C++ compiler and CMake. Install SQLite development files and make sure CMake can find `SQLite3`. With Visual Studio Build Tools, open the **Developer PowerShell** or **Developer Command Prompt**.

### Ubuntu / Debian

```bash
sudo apt update
sudo apt install build-essential cmake libsqlite3-dev
```

## Build and run

Run these commands from the project root:

```bash
cmake -S . -B build
cmake --build build
```

On Windows with a multi-configuration generator:

```powershell
.\build\Debug\student_report_card.exe
```

If you configured a Release build:

```powershell
.\build\Release\student_report_card.exe
```

On Linux:

```bash
./build/student_report_card
```

The application creates `students.db` in the current working directory on first run.

## Using the application

1. Choose `1` to add a student.
2. Choose `3` to enter subject marks. Marks must be between 0 and 100.
3. Choose `4` to display the report card.
4. Choose `2` to list students or `5` to search by roll number.
5. Choose `7` to view the class summary.
6. Choose `6` to delete a student and their marks.
7. Choose `0` to exit.

Entering marks for the same student and subject again updates the existing mark.

## Grade scale

| Percentage | Grade |
|---|---|
| 90–100 | A+ |
| 80–89.99 | A |
| 70–79.99 | B |
| 60–69.99 | C |
| 50–59.99 | D |
| Below 50 | F |

Percentage is calculated as the average of the marks entered for that student. The total denominator is therefore `number of entered subjects × 100`.

## Database design

- `students`: id, name, roll number (unique), department, semester
- `marks`: id, student id, subject, mark (0–100); one mark per subject per student

## GitHub upload

Create an empty GitHub repository, then run:

```bash
git init
git add .
git commit -m "Initial Student Report Card System"
git branch -M main
git remote add origin https://github.com/YOUR-USERNAME/YOUR-REPOSITORY.git
git push -u origin main
```

Replace the URL with your own repository URL. If `origin` already exists, use `git remote set-url origin YOUR-REPOSITORY-URL`.

## Notes

- The database file is generated locally and ignored by Git. The schema is created automatically at startup.
- Do not commit private student information. Use fictional data for demonstrations.
- This is an academic mini-project starter, not a production student-information system. It does not include authentication, role permissions, backups, or audit logs.
