#include <iostream>

struct Specialization {
    std::string value;

    explicit Specialization(const std::string &arg_new_value) {
        value = arg_new_value;
    }
};

struct Course {
    std::string value;

    explicit Course(const std::string &arg_new_value) {
        value = arg_new_value;
    }
};

struct Week {
    std::string value;

    explicit Week(const std::string &arg_new_value) {
        value = arg_new_value;
    }
};

struct LectureTitle {
    std::string specialization;
    std::string course;
    std::string week;

    LectureTitle(const Specialization &arg_new_specialization, const Course &arg_new_course, const Week &arg_new_week) {
        specialization = arg_new_specialization.value;
        course = arg_new_course.value;
        week = arg_new_week.value;
    }
};


int main() {
    LectureTitle title(
            Specialization("C++"),
            Course("White belt"),
            Week("4th")
    );

    return 0;
}
