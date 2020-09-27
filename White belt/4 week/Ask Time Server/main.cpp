#include <iostream>
#include <exception>
#include <string>

using namespace std;

class TimeServer {
public:
    string GetCurrentTime() {
        try {
            std::string save = AskTimeServer();
            last_fetched_time = save;
        } catch (system_error &er) {
            return last_fetched_time;
        }
        return last_fetched_time;
    }

private:
    string last_fetched_time = "00:00:00";
};
/*

int main() {
    TimeServer ts;
    try {
        cout << ts.GetCurrentTime() << endl;
    } catch (exception &e) {
        cout << "Exception got: " << e.what() << endl;
    }
    return 0;
}*/
