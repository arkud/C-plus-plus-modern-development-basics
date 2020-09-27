#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

class Rational {
public:

    Rational() {

    }

    Rational(int numerator, int denominator) {
    }

    int Numerator() const {
    }

    int Denominator() const {
    }
};

template<class T>
ostream &operator<<(ostream &os, const vector<T> &s) {
    os << "{";
    bool first = true;
    for (const auto &x : s) {
        if (!first) {
            os << ", ";
        }
        first = false;
        os << x;
    }
    return os << "}";
}

template<class T>
ostream &operator<<(ostream &os, const set<T> &s) {
    os << "{";
    bool first = true;
    for (const auto &x : s) {
        if (!first) {
            os << ", ";
        }
        first = false;
        os << x;
    }
    return os << "}";
}

template<class K, class V>
ostream &operator<<(ostream &os, const map<K, V> &m) {
    os << "{";
    bool first = true;
    for (const auto &kv : m) {
        if (!first) {
            os << ", ";
        }
        first = false;
        os << kv.first << ": " << kv.second;
    }
    return os << "}";
}

bool operator!=(const Rational &l, const Rational &r) {
    return l.Numerator() != r.Numerator() && l.Denominator() != r.Denominator();
}

ostream &operator<<(ostream &os, const Rational &item) {
    os << item.Numerator() << "/" << item.Denominator();
    return os;
}

template<class T, class U>
void AssertEqual(const T &t, const U &u, const string &hint = {}) {
    if (t != u) {
        ostringstream os;
        os << "Assertion failed: " << t << " != " << u;
        if (!hint.empty()) {
            os << " hint: " << hint;
        }
        throw runtime_error(os.str());
    }
}

void Assert(bool b, const string &hint) {
    AssertEqual(b, true, hint);
}

class TestRunner {
public:
    template<class TestFunc>
    void RunTest(TestFunc func, const string &test_name) {
        try {
            func();
            cerr << test_name << " OK" << endl;
        } catch (exception &e) {
            ++fail_count;
            cerr << test_name << " fail: " << e.what() << endl;
        } catch (...) {
            ++fail_count;
            cerr << "Unknown exception caught" << endl;
        }
    }

    ~TestRunner() {
        if (fail_count > 0) {
            cerr << fail_count << " unit tests failed. Terminate" << endl;
            exit(1);
        }
    }

private:
    int fail_count = 0;
};


void TestDefault() {
    {
        Rational test;
        Rational correct(0, 1);
        AssertEqual(test.Numerator(), correct.Numerator(), "Default constructor");
        AssertEqual(test.Denominator(), correct.Denominator(), "Default constructor");
    }

    {
        Rational test(INT32_MAX, 1);
        Rational correct(INT32_MAX, 1);
        AssertEqual(test.Numerator(), correct.Numerator(), "Default constructor");
        AssertEqual(test.Denominator(), correct.Denominator(), "Default constructor");
    }

}

void TestConstructor() {
    {
        Rational test(1, 2);
        Rational correct(1, 2);
        AssertEqual(test.Numerator(), correct.Numerator(), "Construction");
        AssertEqual(test.Denominator(), correct.Denominator(), "Construction");
    }
    {
        Rational test(10, 5);
        Rational correct(2, 1);
        AssertEqual(test.Numerator(), correct.Numerator(), "Reduce fraction");
        AssertEqual(test.Denominator(), correct.Denominator(), "Reduce fraction");
    }

    {
        Rational test(5, 10);
        Rational correct(1, 2);
        AssertEqual(test.Numerator(), correct.Numerator(), "Reduce fraction");
        AssertEqual(test.Denominator(), correct.Denominator(), "Reduce fraction");
    }

    {
        Rational test(-1, 2);
        Rational correct(-1, 2);
        AssertEqual(test.Numerator(), correct.Numerator(), "Negative fraction");
        AssertEqual(test.Denominator(), correct.Denominator(), "Negative fraction");
    }
    {
        Rational test(1, -2);
        Rational correct(-1, 2);
        AssertEqual(test.Numerator(), correct.Numerator(), "Negative fraction");
        AssertEqual(test.Denominator(), correct.Denominator(), "Negative fraction");
    }

    {
        Rational test(-1, -2);
        Rational correct(1, 2);
        AssertEqual(test.Numerator(), correct.Numerator(), "Reduce negation");
        AssertEqual(test.Denominator(), correct.Denominator(), "Reduce negation");
    }

    {
        Rational test(0, 2);
        Rational correct(0, 1);
        AssertEqual(test.Numerator(), correct.Numerator(), "Zero case");
        AssertEqual(test.Denominator(), correct.Denominator(), "Zero case");
    }

    {
        Rational test(0, 1);
        Rational correct(0, 1);
        AssertEqual(test.Numerator(), correct.Numerator(), "Zero case");
        AssertEqual(test.Denominator(), correct.Denominator(), "Zero case");
    }

    {
        Rational test(0, -1);
        Rational correct(0, 1);
        AssertEqual(test.Numerator(), correct.Numerator(), "Zero case");
        AssertEqual(test.Denominator(), correct.Denominator(), "Zero case");
    }

    {
        Rational test(INT32_MAX, INT32_MAX);
        Rational correct(1, 1);
        AssertEqual(test.Numerator(), correct.Numerator(), "Max int");
        AssertEqual(test.Denominator(), correct.Denominator(), "Max int");
    }

}

int main() {
    TestRunner runner;
    runner.RunTest(TestDefault, "TestDefault");
    runner.RunTest(TestConstructor, "TestConstructor");

    return 0;
}