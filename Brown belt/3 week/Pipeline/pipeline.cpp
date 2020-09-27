#include "test_runner.h"
#include <functional>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>
#include <queue>

using namespace std;


struct Email {
    string from;
    string to;
    string body;
};


class Worker {
public:
    virtual ~Worker() = default;

    virtual void Process(unique_ptr<Email> email) = 0;

    virtual void Run() {
        // только первому worker-у в пайплайне нужно это имплементировать
        throw logic_error("Unimplemented");
    }

protected:
    unique_ptr<Worker> queue;

    // реализации должны вызывать PassOn, чтобы передать объект дальше
    // по цепочке обработчиков
    void PassOn(unique_ptr<Email> email) const {
        if (queue) {
            queue->Process(move(email));
        }
    }

public:
    void SetNext(unique_ptr<Worker> next) {
        queue = move(next);
    }

    Worker *get() {
        return queue.get();
    }
};


class Reader : public Worker {
    istream &in;
public:
    Reader(istream &input) : in(input) {}

    void Run() override {
        auto email = make_unique<Email>();
        while (getline(in, email->from)) {
            getline(in, email->to);
            getline(in, email->body);

            PassOn(move(email));
            email = make_unique<Email>();
        }
    }

    void Process(unique_ptr<Email> email) override {
        PassOn(move(email));
    }
};


class Filter : public Worker {
public:
    using Function = function<bool(const Email &)>;
    Function predicate;
public:
    Filter(Function function) : predicate(move(function)) {}

    void Process(unique_ptr<Email> email) override {
        if (predicate(*email))
            PassOn(move(email));
    }
};


class Copier : public Worker {
    string receiver;
public:
    Copier(string receiver) : receiver(move(receiver)) {}

    void Process(unique_ptr<Email> email) override {
        if (email->to != receiver) {
            auto new_email = make_unique<Email>();
            new_email->to = receiver;
            new_email->body = email->body;
            new_email->from = email->from;

            PassOn(move(email));
            PassOn(move(new_email));
        } else {
            PassOn(move(email));
        }
    }
};


class Sender : public Worker {
    ostream &out;
public:
    Sender(ostream &output) : out(output) {}

    void Process(unique_ptr<Email> email) override {
        out << email->from << "\n"
            << email->to << "\n"
            << email->body << "\n";
        PassOn(move(email));
    }
};


// реализуйте класс
class PipelineBuilder {
private:
    unique_ptr<Worker> begin;
    Worker *current;
public:
    // добавляет в качестве первого обработчика Reader
    explicit PipelineBuilder(istream &in) {
        begin = make_unique<Reader>(in);
        current = begin.get();
    }

    // добавляет новый обработчик Filter
    PipelineBuilder &FilterBy(Filter::Function filter) {
        current->SetNext(make_unique<Filter>(move(filter)));
        current = current->get();
        return *this;
    }

    // добавляет новый обработчик Copier
    PipelineBuilder &CopyTo(string recipient) {
        current->SetNext(make_unique<Copier>(move(recipient)));
        current = current->get();
        return *this;
    }

    // добавляет новый обработчик Sender
    PipelineBuilder &Send(ostream &out) {
        current->SetNext(make_unique<Sender>(out));
        current = current->get();
        return *this;
    }

    // возвращает готовую цепочку обработчиков
    unique_ptr<Worker> Build() {
        return move(begin);
    }
};


void TestSanity() {
    string input = (
            "erich@example.com\n"
            "richard@example.com\n"
            "Hello there\n"

            "erich@example.com\n"
            "ralph@example.com\n"
            "Are you sure you pressed the right button?\n"

            "ralph@example.com\n"
            "erich@example.com\n"
            "I do not make mistakes of that kind\n"
    );
    istringstream inStream(input);
    ostringstream outStream;

    PipelineBuilder builder(inStream);
    builder.FilterBy([](const Email &email) {
        return email.from == "erich@example.com";
    });
    builder.CopyTo("richard@example.com");
    builder.Send(outStream);
    auto pipeline = builder.Build();

    pipeline->Run();

    string expectedOutput = (
            "erich@example.com\n"
            "richard@example.com\n"
            "Hello there\n"

            "erich@example.com\n"
            "ralph@example.com\n"
            "Are you sure you pressed the right button?\n"

            "erich@example.com\n"
            "richard@example.com\n"
            "Are you sure you pressed the right button?\n"
    );

    ASSERT_EQUAL(expectedOutput, outStream.str());
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestSanity);
    return 0;
}
