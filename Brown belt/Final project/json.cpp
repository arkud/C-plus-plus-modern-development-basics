#include <iostream>
#include "json.h"

using namespace std;

namespace Json {

  Document::Document(Node root) : root(move(root)) {
  }

  const Node& Document::GetRoot() const {
    return root;
  }

  Node LoadNode(istream& input);

  Node LoadArray(istream& input) {
    vector<Node> result;

    //cerr << "[\n";
    for (char c; input >> c && c != ']'; ) {
      if (c != ',') {
        input.putback(c);
      }
      result.push_back(LoadNode(input));
        //cerr << "\n";
    }
      //cerr << "]\n";

    return Node(move(result));
  }

  Node LoadInt(istream& input) {
    int result = 0;
    while (isdigit(input.peek())) {
      result *= 10;
      result += input.get() - '0';
    }
    return Node(result);
  }

  Node LoadNumber(istream& input) {
      double sign = 1.0;
      if(input.peek() == '-'){
          input.get();
          sign = -1.0;
      }

      int integer_part = sign * LoadInt(input).AsInt();
      if(input.peek() == '.'){
          input.get();
          double double_part = 0.0;
          double rank = 1.0;
          while (isdigit(input.peek())) {
              rank /= 10;
              double_part += (input.get() - '0') * rank;
          }
          //cerr << integer_part + double_part;
          return Node(integer_part + sign * double_part);
      } else{
          //cerr << integer_part;
          return Node(integer_part);
      }
  }

  Node LoadBool(istream& input){
      if(input.peek() == 'f'){
          for(int i = 0;i < 5; input.get(),i++);
          //cerr << "false";
          return Node(false);
      } else{
          for(int i = 0;i < 4; input.get(),i++);
          //cerr << "true";
          return Node(true);
      }
  }

  Node LoadString(istream& input) {
    string line;
    getline(input, line, '"');
      //cerr << "\""<< line <<"\"";
    return Node(move(line));
  }

  Node LoadDict(istream& input) {
    map<string, Node> result;

      //cerr << "{\n";
    for (char c; input >> c && c != '}'; ) {
      if (c == ',') {
        input >> c;
      }

      string key = LoadString(input).AsString();
        //cerr << ": ";
      input >> c;
      result.emplace(move(key), LoadNode(input));
        //cerr << "\n";
    }
      //cerr << "}\n";

    return Node(move(result));
  }

  Node LoadNode(istream& input) {
    char c;
    input >> c;

    if (c == '[') {
      return LoadArray(input);
    } else if (c == '{') {
      return LoadDict(input);
    } else if (c == '"') {
      return LoadString(input);
    } else if(isdigit(c) || c == '-'){
      input.putback(c);
      return LoadNumber(input);
    } else {
        input.putback(c);
        return LoadBool(input);
    }
  }

  Document Load(istream& input) {
    return Document{LoadNode(input)};
  }

}
