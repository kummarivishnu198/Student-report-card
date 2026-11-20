#ifndef STUDENT_H
#define STUDENT_H

#include "Person.h"
#include <string>

class Student : public Person {
private:
    std::string department;
    int semester{};

public:
    Student() = default;
    Student(int studentId, const std::string& studentName,
            const std::string& roll, const std::string& dept, int sem)
        : Person(studentId, studentName, roll),
          department(dept), semester(sem) {}

    const std::string& getDepartment() const { return department; }
    int getSemester() const { return semester; }

    void display() const override {
        std::cout << "ID: " << id
                  << " | Name: " << name
                  << " | Roll No: " << rollNumber
                  << " | Department: " << department
                  << " | Semester: " << semester << '\n';
    }
};

#endif
