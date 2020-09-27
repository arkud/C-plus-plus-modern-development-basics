#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Person {
public:
    Person(string name, string profession) : name_(name), profession_(profession) {}

    string GetName() const {
        return name_;
    }
    string GetProfession() const {
        return profession_;
    }
    virtual void Walk(const string& destination) const = 0;

protected:
    string name_;
    string profession_;
};

class Student : public Person {
public:

    Student(string name, string favouriteSong) : Person(name, "Student" ), FavouriteSong(favouriteSong) {}

    void Learn() {
        cout << "Student: " << GetName() << " learns" << endl;
    }

    void SingSong() const {
        cout << "Student: " << GetName() << " sings a song: " << FavouriteSong << endl;
    }

    void Walk(const string& destination) const override {
        cout << "Student: " << GetName() << " walks to: " << destination << endl;
        SingSong();
    }

private:
    string FavouriteSong;
};

class Teacher : public Person {
public:

    Teacher(string name, string subject) : Person(name, "Teacher"), Subject(subject) {}

    void Teach() {
        cout << "Teacher: " << GetName() << " teaches: " << Subject << endl;
    }

    void Walk(const string& destination) const override {
        cout << "Teacher: " << GetName() << " walks to: " << destination << endl;
    }

private:
    string Subject;
};

class Policeman : public Person {
public:
    Policeman(string name) : Person(name, "Policeman") {}

    void Check(const Person& pe) {
        cout << "Policeman: " << GetName() << " checks " << pe.GetProfession() << ". " << pe.GetProfession() << "'s name is: " << pe.GetName() << endl;
    }

    void Walk(const string& destination) const override {
        cout << "Policeman: " << GetName() << " walks to: " << destination << endl;
    }

};

void VisitPlaces(Person& pe, vector<string> places) {
    for (auto p : places) {
        pe.Walk(p);
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