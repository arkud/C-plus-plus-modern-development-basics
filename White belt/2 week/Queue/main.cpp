#include <iostream>
#include <vector>

void worry(std::vector<bool> &arg_queue, int arg_i) {
    arg_queue[arg_i] = true;
}

void quiet(std::vector<bool> &arg_queue, int arg_i) {
    arg_queue[arg_i] = false;
}

void come(std::vector<bool> &arg_queue, int arg_k) {
    if (arg_k > 0) {
        arg_queue.resize(arg_queue.size() + arg_k, false);
    } else {
        arg_queue.erase(arg_queue.end() + arg_k, arg_queue.end());
    }
}

int worry_count(std::vector<bool> &arg_queue) {
    int result = 0;

    for (auto item:arg_queue) {
        if (item)
            result++;
    }

    return result;
}

int main() {
    std::vector<bool> queue;

    int n;
    std::cin >> n;

    for (int i = 0; i < n; i++) {
        std::string action;
        int index;

        std::cin >> action;
        if (action != "WORRY_COUNT")
            std::cin >> index;

        if (action == "COME")
            come(queue, index);
        else if (action == "WORRY") {
            worry(queue, index);
        } else if (action == "QUIET") {
            quiet(queue, index);
        } else if (action == "WORRY_COUNT")
            std::cout << worry_count(queue) << std::endl;

    }

    return 0;
}
