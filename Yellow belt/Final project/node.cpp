#include "node.h"


bool EmptyNode::Evaluate(const Date &date, const string &event) const {
    return true;
}

LogicalOperationNode::LogicalOperationNode(const LogicalOperation &lo, const shared_ptr<Node> &left,
                                           const shared_ptr<Node> &right) : pf_l_operation(lo), pf_l_value(left),
                                                                            pf_r_value(right) {}

bool LogicalOperationNode::Evaluate(const Date &date, const string &event) const {
    switch (pf_l_operation) {
        case LogicalOperation::Or :
            return pf_l_value->Evaluate(date, event) || pf_r_value->Evaluate(date, event);
        case LogicalOperation::And :
            return pf_l_value->Evaluate(date, event) && pf_r_value->Evaluate(date, event);
    }
}

DateComparisonNode::DateComparisonNode(Comparison cmp, const Date &r_value) : pf_cmp_operation(cmp),
                                                                              pf_r_value(r_value) {}

bool DateComparisonNode::Evaluate(const Date &date, const string &event) const {
    switch (pf_cmp_operation) {
        case Comparison::Equal :
            return date == pf_r_value;
        case Comparison::Greater :
            return date > pf_r_value;
        case Comparison::GreaterOrEqual :
            return date >= pf_r_value;
        case Comparison::Less :
            return date < pf_r_value;
        case Comparison::LessOrEqual :
            return date <= pf_r_value;
        case Comparison::NotEqual :
            return date != pf_r_value;

    }
}

EventComparisonNode::EventComparisonNode(Comparison cmp, const string &value) : pf_cmp_operation(cmp),
                                                                                pf_r_value(value) {}

bool EventComparisonNode::Evaluate(const Date &date, const string &event) const {
    switch (pf_cmp_operation) {
        case Comparison::Equal :
            return event == pf_r_value;
        case Comparison::Greater :
            return event > pf_r_value;
        case Comparison::GreaterOrEqual :
            return event >= pf_r_value;
        case Comparison::Less :
            return event < pf_r_value;
        case Comparison::LessOrEqual :
            return event <= pf_r_value;
        case Comparison::NotEqual :
            return event != pf_r_value;

    }
}
