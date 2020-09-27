#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Person {
public:
    Person(const string &name, const string &type) : Name(name), Type(type) {}

    virtual void Walk(const string &destination) const = 0;

    ostream &Introduce() const {
        return cout << Type << ": " << Name;
    }

    const string Name;
    const string Type;
};

class Student : public Person {
public:

    Student(const string &name, const string &favouriteSong) : Person(name, "Student"), FavouriteSong(favouriteSong) {
    }

    void Learn() const {
        Introduce() << Name << " learns" << endl;
    }

    void Walk(const string &destination) const override {
        Introduce() << " walks to: " << destination << endl;
        SingSong();
    }

    void SingSong() const {
        Introduce() << " sings a song: " << FavouriteSong << endl;
    }

private:
    const string FavouriteSong;
};


class Teacher : public Person {
public:

    Teacher(const string &name, const string &subject) : Person(name, "Teacher"), Subject(subject) {
    }

    void Teach() const {
        Introduce() << " teaches: " << Subject << endl;
    }

    void Walk(const string &destination) const override {
        Introduce() << " walks to: " << destination << endl;
    }

private:
    const string Subject;
};


class Policeman : public Person {
public:
    explicit Policeman(const string &name) : Person(name, "Policeman") {}

    void Check(const Person &t) const {
        Introduce() << " checks " << t.Type << ". " << t.Type << "'s name is: " << t.Name << endl;
    }

    void Walk(const string &destination) const override {
        Introduce() << Name << " walks to: " << destination << endl;
    }
};


void VisitPlaces(Person &t, const vector<string> &places) {
    for (const auto &p : places) {
        t.Walk(p);
    }
}


int main() {
    Teacher t("Jim", "Math");
    Student s("Ann", "We will rock you");
    Policeman p("Bob");

    VisitPlaces(t, {"Moscow", "London"});
    p.Check(s);
    VisitPlaces(s, {"Moscow", "London"});
    return 0;
}
