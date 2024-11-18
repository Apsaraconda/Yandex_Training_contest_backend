#include <cstring>
#include <iostream>
#include <limits>
#include <set>
#include <sstream>
#include <vector>

int Search(std::string &c, std::string &out) {
  int error = 0;
  std ::string ::size_type pos = c.find(",");
  if (pos != std ::string ::npos) {
    out = c.substr(0, pos);
    c.erase(0, pos + 1);
  } else {
    error = 1;
  }
  return error;
}

int Count(std::string c) {
  int result = 0;
  std::set<char> data;
  for (long unsigned int i = 0; i < c.size(); ++i) {
    data.insert(c[i]);
  }
  result = data.size();
  return result;
}

int main() {
  int N;
  std::cin >> N;
  std::vector<std::string> string_data;
  std::string line;
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  int count_line = 0;
  while (count_line < N && std::getline(std::cin, line) && !line.empty()) {
    string_data.push_back(line);
    count_line++;
  }
  for (unsigned int i = 0; i < string_data.size(); ++i) {
    std::string fam;
    std ::string im;
    std ::string ot;
    std ::string day;
    std ::string month;
    Search(string_data[i], fam);
    Search(string_data[i], im);
    Search(string_data[i], ot);
    std::string sum = fam + im + ot;
    int name_count = Count(sum);
    char v = toupper(fam[0]);
    int up = v - 'A' + 1;
    Search(string_data[i], day);
    Search(string_data[i], month);
    int daymonth = 0;
    for (long unsigned int i = 0; i < day.size(); ++i) {
      char temp = day[i];
      daymonth = daymonth + atoi(&temp);
    }
    for (long unsigned int i = 0; i < month.size(); ++i) {
      char temp = month[i];
      daymonth = daymonth + atoi(&temp);
    }
    daymonth = daymonth * 64;
    up = up * 256;
    int res = name_count + daymonth + up;
    std::stringstream ss;
    ss << std::hex << std::uppercase << res;
    std::string hexNum = ss.str();
    hexNum.erase(0, hexNum.size() - 3);
    std::cout << hexNum << ' ';
  }
  return 0;
}