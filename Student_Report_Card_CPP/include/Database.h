#ifndef DATABASE_H
#define DATABASE_H

#include "ReportCard.h"
#include "Student.h"
#include <sqlite3.h>
#include <string>
#include <vector>

struct ClassSummary {
    int studentCount{};
    int markCount{};
    double averagePercentage{};
};

class Database {
private:
    sqlite3* db{nullptr};

    void execute(const std::string& sql);
    void initialize();

public:
    explicit Database(const std::string& filename);
    ~Database();

    Database(const Database&) = delete;
    Database& operator=(const Database&) = delete;

    void addStudent(const Student& student);
    std::vector<Student> getAllStudents();
    bool findStudentByRoll(const std::string& roll, Student& result);
    bool deleteStudent(const std::string& roll);
    void saveMark(const std::string& roll, const std::string& subject, double mark);
    bool getReport(const std::string& roll, Student& student, ReportCard& report);
    ClassSummary getClassSummary();
};

#endif
