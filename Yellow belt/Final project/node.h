#pragma once

#include <string>
#include <memory>
#include "date.h"

using namespace std;

enum class Comparison {
    Less,
    LessOrEqual,
    Greater,
    GreaterOrEqual,
    Equal,
    NotEqual
};

enum class LogicalOperation {
    Or,
    And
};

class Node {
public:
    virtual bool Evaluate(const Date &date, const string &event) const = 0;
};

class EmptyNode : public Node {
public:
    bool Evaluate(const Date &date, const string &event) const override;
};

class LogicalOperationNode : public Node {
public:
    LogicalOperationNode(const LogicalOperation &lo, const shared_ptr<Node> &left, const shared_ptr<Node> &right);

    bool Evaluate(const Date &date, const string &event) const override;

private:
    LogicalOperation pf_l_operation;
    shared_ptr<Node> pf_r_value;
    shared_ptr<Node> pf_l_value;
};

class DateComparisonNode : public Node {
public:
    DateComparisonNode(const Comparison cmp, const Date &value);

    bool Evaluate(const Date &date, const string &event) const override;

private:
    Comparison pf_cmp_operation;
    Date pf_r_value;
};

class EventComparisonNode : public Node {
public:
    EventComparisonNode(Comparison cmp, const std::string &value);

    bool Evaluate(const Date &date, const string &event) const override;

private:
    Comparison pf_cmp_operation;
    string pf_r_value;
};
