#ifndef PERSON_H
#define PERSON_H

#include <iostream>
#include <string>

class Person {
protected:
    int id{};
    std::string name;
    std::string rollNumber;

public:
    Person() = default;
    Person(int studentId, const std::string& studentName,
           const std::string& roll)
        : id(studentId), name(studentName), rollNumber(roll) {}

    virtual ~Person() = default;

    int getId() const { return id; }
    const std::string& getName() const { return name; }
    const std::string& getRollNumber() const { return rollNumber; }

    virtual void display() const = 0;
};

#endif
