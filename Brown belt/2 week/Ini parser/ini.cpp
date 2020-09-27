//
// Created by ar_kud on 12.08.2020.
//
#include "ini.h"

namespace Ini {

    Section &Document::AddSection(string name) {
        return sections[move(name)];
    }

    const Section &Document::GetSection(const string &name) const {
        return sections.at(name);
    }

    size_t Document::SectionCount() const {
        return sections.size();
    }

    Document Load(istream &input) {
        Document result;

        Section* buff;
        for (string line; getline(input, line);) {
            if(line.empty())
                continue;
            if (line.front() == '[' && line.back() == ']')
                buff = &result.AddSection(line.substr(1, line.size() - 2));
            else {
                auto delim = line.find('=');
                (*buff)[line.substr(0, delim)] = line.substr(delim + 1);
            }
        }

        return result;
    }
}
