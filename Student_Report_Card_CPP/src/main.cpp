#include "Database.h"

#include <iomanip>
#include <iostream>
#include <limits>
#include <string>

namespace {
void clearInput() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

std::string readLine(const std::string& prompt) {
    std::string value;
    std::cout << prompt;
    std::getline(std::cin, value);
    return value;
}

int readInt(const std::string& prompt) {
    int value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value) {
            clearInput();
            return value;
        }
        std::cout << "Please enter a valid whole number.\n";
        std::cin.clear();
        clearInput();
    }
}

double readMark() {
    double mark;
    while (true) {
        std::cout << "Enter mark (0-100): ";
        if (std::cin >> mark && mark >= 0 && mark <= 100) {
            clearInput();
            return mark;
        }
        std::cout << "Invalid mark. Enter a value from 0 to 100.\n";
        std::cin.clear();
        clearInput();
    }
}

void printReport(Database& database) {
    const std::string roll = readLine("Enter student roll number: ");
    Student student;
    ReportCard report;

    if (!database.getReport(roll, student, report)) {
        std::cout << "Student not found.\n";
        return;
    }

    std::cout << "\n========== STUDENT REPORT CARD ==========\n";
    student.display();
    std::cout << "-----------------------------------------\n";
    if (report.getMarks().empty()) {
        std::cout << "No marks have been entered for this student.\n";
    } else {
        std::cout << std::left << std::setw(25) << "Subject"
                  << std::right << std::setw(10) << "Marks\n";
        std::cout << "-----------------------------------------\n";
        for (const auto& item : report.getMarks()) {
            std::cout << std::left << std::setw(25) << item.first
                      << std::right << std::setw(8) << std::fixed
                      << std::setprecision(2) << item.second << '\n';
        }
        std::cout << "-----------------------------------------\n";
        std::cout << "Total: " << report.total() << " / "
                  << report.getMarks().size() * 100 << '\n';
        std::cout << "Percentage: " << report.percentage() << "%\n";
        std::cout << "Grade: " << report.grade() << '\n';
    }
    std::cout << "=========================================\n";
}

void showMenu() {
    std::cout << "\n===== STUDENT REPORT CARD SYSTEM =====\n"
              << "1. Add student\n"
              << "2. View all students\n"
              << "3. Add or update subject marks\n"
              << "4. View student report card\n"
              << "5. Search student\n"
              << "6. Delete student\n"
              << "7. View class summary\n"
              << "0. Exit\n";
}
}

int main() {
    try {
        Database database("students.db");
        std::cout << "Student Report Card System started.\n";

        while (true) {
            showMenu();
            const int choice = readInt("Choose an option: ");

            try {
                switch (choice) {
                case 1: {
                    const std::string name = readLine("Student name: ");
                    const std::string roll = readLine("Roll number: ");
                    const std::string department = readLine("Department: ");
                    const int semester = readInt("Semester (1-12): ");
                    if (name.empty() || roll.empty() || department.empty() ||
                        semester < 1 || semester > 12) {
                        std::cout << "Please enter valid student details.\n";
                        break;
                    }
                    database.addStudent(Student(0, name, roll, department, semester));
                    std::cout << "Student added successfully.\n";
                    break;
                }
                case 2: {
                    const auto students = database.getAllStudents();
                    if (students.empty()) {
                        std::cout << "No student records found.\n";
                    } else {
                        std::cout << "\n--- Student List ---\n";
                        for (const auto& student : students) student.display();
                    }
                    break;
                }
                case 3: {
                    const std::string roll = readLine("Student roll number: ");
                    const std::string subject = readLine("Subject name: ");
                    if (subject.empty()) {
                        std::cout << "Subject name cannot be empty.\n";
                        break;
                    }
                    const double mark = readMark();
                    database.saveMark(roll, subject, mark);
                    std::cout << "Mark saved successfully.\n";
                    break;
                }
                case 4:
                    printReport(database);
                    break;
                case 5: {
                    const std::string roll = readLine("Enter roll number: ");
                    Student student;
                    if (database.findStudentByRoll(roll, student)) {
                        student.display();
                    } else {
                        std::cout << "Student not found.\n";
                    }
                    break;
                }
                case 6: {
                    const std::string roll = readLine("Roll number to delete: ");
                    std::cout << (database.deleteStudent(roll)
                        ? "Student and associated marks deleted.\n"
                        : "Student not found.\n");
                    break;
                }
                case 7: {
                    const ClassSummary summary = database.getClassSummary();
                    std::cout << "\n--- Class Summary ---\n"
                              << "Total students: " << summary.studentCount << '\n'
                              << "Marks entries: " << summary.markCount << '\n'
                              << "Average student percentage: "
                              << std::fixed << std::setprecision(2)
                              << summary.averagePercentage << "%\n";
                    break;
                }
                case 0:
                    std::cout << "Thank you for using the system.\n";
                    return 0;
                default:
                    std::cout << "Invalid option. Please try again.\n";
                }
            } catch (const std::exception& error) {
                std::cout << "Operation failed: " << error.what() << '\n';
            }
        }
    } catch (const std::exception& error) {
        std::cerr << "Could not start application: " << error.what() << '\n';
        return 1;
    }
}
