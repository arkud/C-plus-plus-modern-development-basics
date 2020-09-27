#include <iostream>
#include <string>
#include <vector>

using namespace std;


class Person {
protected:
    const string Name;
    const string Kind;

    void Do(const string& what) const {
        cout << Kind << ": " << Name << " " << what << endl;
    }
public:
    Person(const string& name, const string& kind): Name(name), Kind(kind) {}
    string GetName() const { return Name; }
    string GetKind() const { return Kind; }
    virtual void Walk(const string& destination) const {
        cout << Kind << ": " << Name << " walks to: " << destination << endl;
    }
};

class Student : public Person {
    const string FavouriteSong;
public:

    Student(const string& name, const string& favouriteSong)
            : Person(name, "Student"), FavouriteSong(favouriteSong) {}

    void Learn() const { Do("learns"); }

    void Walk(const string& destination) const override {
        Person::Walk(destination);
        SingSong();
    }

    void SingSong() const { Do("sings a song: " + FavouriteSong); }
};


class Teacher : public Person {
    const string Subject;
public:
    Teacher(const string& name, const string& subject) : Person(name, "Teacher"), Subject(subject) {}
    void Teach() const { Do("teaches: " + Subject); }
};


class Policeman : public Person {
public:
    Policeman(const string& name) : Person(name, "Policeman") {}

    void Check(const Person& p) {
        Do("checks " + p.GetKind() + ". " + p.GetKind() + "'s name is: " + p.GetName());
    }
};

void VisitPlaces(const Person& person, const vector<string>& places) {
    for (const auto& p : places) {
        person.Walk(p);
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