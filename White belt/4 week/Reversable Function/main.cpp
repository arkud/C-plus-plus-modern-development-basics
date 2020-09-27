#include <iostream>
#include <vector>
#include <algorithm>


class FunctionPart {
public:
    FunctionPart(char arg_operation, double arg_value) {
        operation = arg_operation;
        value = arg_value;
    }

    double Apply(double arg_value) const {
        if (operation == '+') {
            arg_value += value;
            return arg_value;
        } else if (operation == '-') {
            arg_value -= value;
            return arg_value;
        }
        if (operation == '*') {
            arg_value *= value;
            return arg_value;
        } else {
            arg_value /= value;
            return arg_value;
        }
    }

    void Invert() {
        if (operation == '+')
            operation = '-';
        else if (operation == '-')
            operation = '+';

        if (operation == '*')
            operation = '/';
        else if (operation == '/')
            operation = '*';

    }

private:
    char operation;
    double value;
};

class Function {
public:
    void AddPart(char arg_operation, double arg_value) {
        function.push_back(FunctionPart(arg_operation, arg_value));
    }

    double Apply(double arg_value) const {
        for (auto part: function) {
            arg_value = part.Apply(arg_value);
        }
        return arg_value;
    }

    void Invert() {
        for (auto &part: function) {
            part.Invert();
        }
        std::reverse(function.begin(), function.end());
    }

private:
    std::vector<FunctionPart> function;
};

/*
struct Image {
    double quality;
    double freshness;
    double rating;
};

struct Params {
    double a;
    double b;
    double c;
};

Function MakeWeightFunction(const Params& params,
                            const Image& image) {
    Function function;
    function.AddPart('*', params.a);
    function.AddPart('-', image.freshness * params.b);
    function.AddPart('+', image.rating * params.c);
    return function;
}

double ComputeImageWeight(const Params& params, const Image& image) {
    Function function = MakeWeightFunction(params, image);
    return function.Apply(image.quality);
}

double ComputeQualityByWeight(const Params& params,
                              const Image& image,
                              double weight) {
    Function function = MakeWeightFunction(params, image);
    function.Invert();
    return function.Apply(weight);
}

int main() {
    Image image = {10, 2, 6};
    Params params = {4, 2, 6};
    std::cout << ComputeImageWeight(params, image) << std::endl;
    std::cout << ComputeQualityByWeight(params, image, 52) << std::endl;
    return 0;
}*/
