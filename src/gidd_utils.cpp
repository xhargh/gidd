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
  auto inp = split(input, '/');
  vector<string> s;
  for (auto &i : inp) {
    if (i == "..") {
      s.pop_back();
    } else if (i != ".") {
      s.push_back(i);
    }
  }

  string result = "";

  for (auto &p : s) {
    result = result + "/" + p;
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
