/**
 * Copyright Xhargh 2016-2020
 */

#include "gidd_utils.hpp"
#include <sstream>
#include <stack>

using namespace std;

vector<string> split(const string &s, char delimiter) {
  vector<string> result;
  stringstream ss (s);
  string item;

  while (getline (ss, item, delimiter)) {
    if (item.length()) {
      result.push_back(item);
    }
  }

  return result;
}


string removeDots(const string input) {
  auto slashIdx = input.find("/");
  string result = "";

  if(slashIdx == string::npos) {
    return input;
  }
  else if(slashIdx == 0) {

    auto inp = split(input, '/');
    vector<string> s;
    for (auto &i : inp) {
      if (i == ".." && !s.empty()) {
        s.pop_back();
      } else if (i != ".") {
        s.push_back(i);
      }
    }



    for (auto &p : s) {
      result = result + "/" + p;
    }
  } else {
    return input.substr(0, slashIdx) + removeDots(input.substr(slashIdx));
  }

  return result;
}




string ReplaceString(string subject, const string& search,
                     const string& replace) {
  size_t pos = 0;
  while ((pos = subject.find(search, pos)) != string::npos) {
    subject.replace(pos, search.length(), replace);
    pos += replace.length();
  }
  return subject;
}
