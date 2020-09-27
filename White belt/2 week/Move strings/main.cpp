#include <string>
#include <vector>

void MoveStrings(std::vector<std::string> &source, std::vector<std::string> &destination) {

    for(auto item : source){
        destination.push_back(item);
    }
    source.clear();

}
