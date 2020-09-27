//
// Created by ar_kud on 12.08.2020.
//

#ifndef INI_PARSER_INI_H
#define INI_PARSER_INI_H

#endif //INI_PARSER_INI_H

#include <unordered_map>
#include <sstream>

using namespace std;

namespace Ini {
    using Section = unordered_map<string, string>;

    class Document {
    public:
        Section &AddSection(string name);

        const Section &GetSection(const string &name) const;

        size_t SectionCount() const;

    private:
        unordered_map<string, Section> sections;
    };

    Document Load(istream &input);

}