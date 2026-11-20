#include "Database.h"

#include <iostream>
#include <stdexcept>

namespace {
class Statement {
private:
    sqlite3_stmt* stmt_{nullptr};

public:
    Statement(sqlite3* db, const std::string& sql) {
        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt_, nullptr) != SQLITE_OK) {
            throw std::runtime_error(sqlite3_errmsg(db));
        }
    }
    ~Statement() { sqlite3_finalize(stmt_); }
    sqlite3_stmt* get() const { return stmt_; }
};
}

Database::Database(const std::string& filename) {
    if (sqlite3_open(filename.c_str(), &db) != SQLITE_OK) {
        std::string message = db ? sqlite3_errmsg(db) : "Could not open database";
        if (db) sqlite3_close(db);
        db = nullptr;
        throw std::runtime_error(message);
    }
    execute("PRAGMA foreign_keys = ON;");
    initialize();
}

Database::~Database() {
    if (db) sqlite3_close(db);
}

void Database::execute(const std::string& sql) {
    char* errorMessage = nullptr;
    if (sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errorMessage) != SQLITE_OK) {
        std::string message = errorMessage ? errorMessage : sqlite3_errmsg(db);
        sqlite3_free(errorMessage);
        throw std::runtime_error(message);
    }
}

void Database::initialize() {
    execute(
        "CREATE TABLE IF NOT EXISTS students ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "name TEXT NOT NULL,"
        "roll_number TEXT NOT NULL UNIQUE,"
        "department TEXT NOT NULL,"
        "semester INTEGER NOT NULL CHECK(semester BETWEEN 1 AND 12)"
        ");"
    );
    execute(
        "CREATE TABLE IF NOT EXISTS marks ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "student_id INTEGER NOT NULL,"
        "subject TEXT NOT NULL,"
        "mark REAL NOT NULL CHECK(mark BETWEEN 0 AND 100),"
        "UNIQUE(student_id, subject),"
        "FOREIGN KEY(student_id) REFERENCES students(id) ON DELETE CASCADE"
        ");"
    );
}

void Database::addStudent(const Student& student) {
    Statement statement(db,
        "INSERT INTO students(name, roll_number, department, semester) VALUES(?,?,?,?);");
    sqlite3_bind_text(statement.get(), 1, student.getName().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(statement.get(), 2, student.getRollNumber().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(statement.get(), 3, student.getDepartment().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(statement.get(), 4, student.getSemester());

    if (sqlite3_step(statement.get()) != SQLITE_DONE) {
        throw std::runtime_error(sqlite3_errmsg(db));
    }
}

std::vector<Student> Database::getAllStudents() {
    std::vector<Student> students;
    Statement statement(db,
        "SELECT id, name, roll_number, department, semester FROM students ORDER BY name;");
    while (sqlite3_step(statement.get()) == SQLITE_ROW) {
        students.emplace_back(
            sqlite3_column_int(statement.get(), 0),
            reinterpret_cast<const char*>(sqlite3_column_text(statement.get(), 1)),
            reinterpret_cast<const char*>(sqlite3_column_text(statement.get(), 2)),
            reinterpret_cast<const char*>(sqlite3_column_text(statement.get(), 3)),
            sqlite3_column_int(statement.get(), 4)
        );
    }
    return students;
}

bool Database::findStudentByRoll(const std::string& roll, Student& result) {
    Statement statement(db,
        "SELECT id, name, roll_number, department, semester FROM students WHERE roll_number=?;");
    sqlite3_bind_text(statement.get(), 1, roll.c_str(), -1, SQLITE_TRANSIENT);
    if (sqlite3_step(statement.get()) != SQLITE_ROW) return false;

    result = Student(
        sqlite3_column_int(statement.get(), 0),
        reinterpret_cast<const char*>(sqlite3_column_text(statement.get(), 1)),
        reinterpret_cast<const char*>(sqlite3_column_text(statement.get(), 2)),
        reinterpret_cast<const char*>(sqlite3_column_text(statement.get(), 3)),
        sqlite3_column_int(statement.get(), 4)
    );
    return true;
}

bool Database::deleteStudent(const std::string& roll) {
    Statement statement(db, "DELETE FROM students WHERE roll_number=?;");
    sqlite3_bind_text(statement.get(), 1, roll.c_str(), -1, SQLITE_TRANSIENT);
    if (sqlite3_step(statement.get()) != SQLITE_DONE) {
        throw std::runtime_error(sqlite3_errmsg(db));
    }
    return sqlite3_changes(db) > 0;
}

void Database::saveMark(const std::string& roll, const std::string& subject, double mark) {
    Statement statement(db,
        "INSERT INTO marks(student_id, subject, mark) "
        "SELECT id, ?, ? FROM students WHERE roll_number=? "
        "ON CONFLICT(student_id, subject) DO UPDATE SET mark=excluded.mark;");
    sqlite3_bind_text(statement.get(), 1, subject.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_double(statement.get(), 2, mark);
    sqlite3_bind_text(statement.get(), 3, roll.c_str(), -1, SQLITE_TRANSIENT);

    if (sqlite3_step(statement.get()) != SQLITE_DONE) {
        throw std::runtime_error(sqlite3_errmsg(db));
    }
    if (sqlite3_changes(db) == 0) {
        throw std::runtime_error("Student roll number was not found.");
    }
}

bool Database::getReport(const std::string& roll, Student& student, ReportCard& report) {
    if (!findStudentByRoll(roll, student)) return false;

    Statement statement(db,
        "SELECT subject, mark FROM marks WHERE student_id=? ORDER BY subject;");
    sqlite3_bind_int(statement.get(), 1, student.getId());
    while (sqlite3_step(statement.get()) == SQLITE_ROW) {
        const auto* subject = reinterpret_cast<const char*>(sqlite3_column_text(statement.get(), 0));
        report.addOrUpdateMark(subject ? subject : "", sqlite3_column_double(statement.get(), 1));
    }
    return true;
}

ClassSummary Database::getClassSummary() {
    ClassSummary summary;
    Statement students(db, "SELECT COUNT(*) FROM students;");
    if (sqlite3_step(students.get()) == SQLITE_ROW) {
        summary.studentCount = sqlite3_column_int(students.get(), 0);
    }

    Statement marks(db, "SELECT COUNT(*) FROM marks;");
    if (sqlite3_step(marks.get()) == SQLITE_ROW) {
        summary.markCount = sqlite3_column_int(marks.get(), 0);
    }

    Statement average(db,
        "SELECT AVG(student_average) FROM "
        "(SELECT AVG(mark) AS student_average FROM marks GROUP BY student_id);");
    if (sqlite3_step(average.get()) == SQLITE_ROW && sqlite3_column_type(average.get(), 0) != SQLITE_NULL) {
        summary.averagePercentage = sqlite3_column_double(average.get(), 0);
    }
    return summary;
}
