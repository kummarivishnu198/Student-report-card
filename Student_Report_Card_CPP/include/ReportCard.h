#ifndef REPORTCARD_H
#define REPORTCARD_H

#include <iomanip>
#include <map>
#include <numeric>
#include <string>

class ReportCard {
private:
    std::map<std::string, double> marks;

public:
    void addOrUpdateMark(const std::string& subject, double mark) {
        marks[subject] = mark;
    }

    const std::map<std::string, double>& getMarks() const {
        return marks;
    }

    double total() const {
        double sum = 0.0;
        for (const auto& item : marks) sum += item.second;
        return sum;
    }

    double percentage() const {
        return marks.empty() ? 0.0 : total() / marks.size();
    }

    std::string grade() const {
        if (marks.empty()) return "N/A";
        const double p = percentage();
        if (p >= 90) return "A+";
        if (p >= 80) return "A";
        if (p >= 70) return "B";
        if (p >= 60) return "C";
        if (p >= 50) return "D";
        return "F";
    }
};

#endif
