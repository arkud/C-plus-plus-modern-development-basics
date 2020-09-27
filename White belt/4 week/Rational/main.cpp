#include <iostream>
#include <stdexcept>

class Rational {
public:
    Rational() {
        numerator = 0;
        denominator = 1;
    }

    Rational(int numerator, int denominator) {
        if (denominator == 0)
        {
            throw std::invalid_argument("");
        }

        if (numerator == 0) {
            this->numerator = 0;
            this->denominator = 1;
        }

        int save_numerator = std::abs(numerator);
        int save_denominator = std::abs(denominator);

        while (save_numerator != 0 && save_denominator != 0) {
            if (save_numerator > save_denominator)
                save_numerator = save_numerator % save_denominator;
            else
                save_denominator = save_denominator % save_numerator;
        }

        this->numerator = numerator / (save_numerator + save_denominator);
        this->denominator = denominator / (save_numerator + save_denominator);

        if (this->numerator < 0 && this->denominator < 0) {
            this->numerator *= -1;
            this->denominator *= -1;
        } else if (this->numerator < 0 || this->denominator < 0) {
            this->numerator = -std::abs(this->numerator);
            this->denominator = std::abs(this->denominator);
        }
    }

    int Numerator() const {
        return numerator;
    }

    int Denominator() const {
        return denominator;
    }

private:
    int numerator;
    int denominator;
};


bool operator==(const Rational &left, const Rational &right) {
    if (left.Numerator() != right.Numerator())
        return false;
    return left.Denominator() == right.Denominator();
}

Rational operator+(const Rational &left, const Rational &right) {
    return Rational(left.Numerator() * right.Denominator() + right.Numerator() * left.Denominator(),
                    left.Denominator() * right.Denominator());
}

Rational operator-(const Rational &left, const Rational &right) {
    return Rational(left.Numerator() * right.Denominator() - right.Numerator() * left.Denominator(),
                    left.Denominator() * right.Denominator());
}

Rational operator*(const Rational &left, const Rational &right) {
    return Rational(left.Numerator() * right.Numerator(),
                    left.Denominator() * right.Denominator());
}

Rational operator/(const Rational &left, const Rational &right) {
    if (right.Numerator() == 0)
        throw std::domain_error("");

    return Rational(left.Numerator() * right.Denominator(),
                    left.Denominator() * right.Numerator());
}

std::ostream &operator<<(std::ostream &stream, const Rational &to_print) {
    stream << to_print.Numerator() << "/" << to_print.Denominator();
    return stream;
}

std::istream &operator>>(std::istream &stream, Rational &to_init) {
    int numerator;
    int denominator;

    stream >> numerator;
    stream.ignore(1);
    stream >> denominator;

    if (!stream)
        return stream;

    to_init = Rational(numerator, denominator);
    return stream;
}

bool operator<(const Rational &left, const Rational &right) {
    return (left - right).Numerator() < 0;
}


bool operator>(const Rational &left, const Rational &right) {
    return (right - left).Numerator() < 0;
}


int main() {
    try {
        Rational r(1, 0);
        std::cout << "Doesn't throw in case of zero denominator" << std::endl;
        return 1;
    } catch (std::invalid_argument &) {
    }

    try {
        auto x = Rational(1, 2) / Rational(0, 1);
        std::cout << "Doesn't throw in case of division by zero" << std::endl;
        return 2;
    } catch (std::domain_error &) {
    }

    std::cout << "OK" << std::endl;
    return 0;
}