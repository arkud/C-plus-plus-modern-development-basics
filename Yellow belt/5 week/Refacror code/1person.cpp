#include <iostream>
#include <string>
#include <vector>

using namespace std;


class Human {
public:
    Human(const string& type, const string& name) :
            type_(type),
            name_(name)
    {}

    string get_type() const {
        return type_;
    }

    string get_name() const {
        return name_;
    }

    virtual void Walk(const string& destination) const {
        cout << type_ << ": " << name_ << " walks to: " << destination << endl;
    }

    virtual ~Human() = default;

private:
    const string type_;
    const string name_;
};


class Student : public Human {
public:

    Student(const string& name, const string& favouriteSong) :
            Human("Student", name),
            favouriteSong_(favouriteSong)
    {}

    string student_name() const{
        return "Student: " + get_name();
    }

    void Learn() const {
        cout << student_name() << " learns" << endl;
    }

    void SingSong() const {
        cout << student_name() << " sings a song: " << favouriteSong_ << endl;
    }

    void Walk(const string& destination) const override {
        cout << student_name() << " walks to: " << destination << endl;
        SingSong();
    }


private:
    const string favouriteSong_;
};


class Teacher : public Human {
public:

    Teacher(const string& name, const string& subject) :
            Human("Teacher", name),
            subject_(subject)
    {}


    void Teach() {
        cout << "Teacher: " << get_name() << " teaches: " << subject_ << endl;
    }

private:
    const string subject_;
};


class Policeman : public Human {
public:
    Policeman(const string& name) :
            Human("Policeman", name)
    {}


    void Check(const Human& h) {
        cout << "Policeman: " << get_name() << " checks "<<h.get_type() + "." <<
             h.get_type() + "'s name is: " << h.get_name() << endl;
    }

};


void VisitPlaces(Human& h, const vector<string>& places) {
    for (const auto& p : places) {
        h.Walk(p);
    }
}


int main() {
    Teacher t("Jim", "Math");
    Student s("Ann", "We will rock you");
    Policeman p("Bob");

    VisitPlaces(t, { "Moscow", "London" });
    p.Check(s);
    VisitPlaces(s, { "Moscow", "London" });
    return 0;
}

