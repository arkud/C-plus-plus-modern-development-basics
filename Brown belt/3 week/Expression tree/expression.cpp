//
// Created by ar_kud on 26.08.2020.
//

#include "Common.h"

using namespace std;

enum class OperationCode {
    Sum = 43,
    Multiply = 42
};

class Operation : public Expression {
    ExpressionPtr left;
    OperationCode operation;
    ExpressionPtr right;
public:
    Operation(ExpressionPtr left, ExpressionPtr right, OperationCode operation) :
            left(move(left)), right(move(right)), operation(operation) {}

    int Evaluate() const override {
        if (operation == OperationCode::Sum) {
            return left->Evaluate() + right->Evaluate();
        } else if (operation == OperationCode::Multiply) {
            return left->Evaluate() * right->Evaluate();
        }
    }

    std::string ToString() const override {
        return "(" + left->ToString() + ")" + static_cast<char>(operation) + "(" + right->ToString() + ")";
    }
};

class Valu : public Expression {
    int value;
public:
    explicit Valu(int value) : value(value) {}

    int Evaluate() const override {
        return value;
    }

    std::string ToString() const override {
        return to_string(value);
    }
};

ExpressionPtr Value(int value) {
    return make_unique<Valu>(value);
}

ExpressionPtr Sum(ExpressionPtr left, ExpressionPtr right) {
    return make_unique<Operation>(move(left), move(right), OperationCode::Sum);
}

ExpressionPtr Product(ExpressionPtr left, ExpressionPtr right) {
    return make_unique<Operation>(move(left), move(right), OperationCode::Multiply);
}


