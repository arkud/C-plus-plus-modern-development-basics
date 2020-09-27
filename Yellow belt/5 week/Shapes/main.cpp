#include <iostream>
#include <iomanip>
#include <vector>
#include <memory>
#include <cmath>

class Figure {
public:
    virtual std::string Name() = 0;

    virtual double Perimeter() = 0;

    virtual double Area() = 0;
};

class Rect : public Figure {
public:
    Rect(uint8_t width, uint8_t height) : width(width), height(height) {

    }

    std::string Name() override {
        return "RECT";
    }

    double Perimeter() override {
        return 2 * width + 2 * height;
    }

    double Area() override {
        return width * height;
    }

private:
    const uint8_t width, height;

};

class Triangle : public Figure {
public:
    Triangle(uint8_t _a, uint8_t _b, uint8_t _c) : a(_a), b(_b), c(_c) {}

    std::string Name() override {
        return "TRIANGLE";
    }

    double Perimeter() override {
        return a + b + c;
    }

    double Area() override {
        double p = (a + b + c) / 2.0;

        return std::sqrt(p * (p - a) * (p - b) * (p - c));
    }

private:
    const uint8_t a, b, c;
};

class Circle : public Figure {
public:
    Circle(uint8_t rad) : radius(rad) {}

    std::string Name() override {
        return "CIRCLE";
    }

    double Perimeter() override {
        return radius * 3.14 * 2;
    }

    double Area() override {
        return 3.14 * radius * radius;
    }

private:
    const uint8_t radius;
};

std::shared_ptr<Figure> CreateFigure(std::istringstream &is) {
    std::string figure;
    is >> figure;
    if (figure == "RECT") {
        int width, height;
        is >> width >> height;
        return std::make_shared<Rect>(width, height);
    } else if (figure == "TRIANGLE") {
        int a, b, c;
        is >> a >> b >> c;
        return std::make_shared<Triangle>(a, b, c);
    } else if (figure == "CIRCLE") {
        int r;
        is >> r;
        return std::make_shared<Circle>(r);
    }
}

int main() {
    std::vector<std::shared_ptr<Figure>> figures;
    for (std::string line; getline(std::cin, line);) {
        std::istringstream is(line);

        std::string command;
        is >> command;
        if (command == "ADD") {
            figures.push_back(CreateFigure(is));
        } else if (command == "PRINT") {
            for (const auto &current_figure : figures) {
                std::cout << std::fixed << std::setprecision(3)
                          << current_figure->Name() << " "
                          << current_figure->Perimeter() << " "
                          << current_figure->Area() << std::endl;
            }
        }
    }
    return 0;
}
