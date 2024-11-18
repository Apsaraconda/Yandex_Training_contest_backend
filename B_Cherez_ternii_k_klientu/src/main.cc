#include <algorithm>
#include <cstring>
#include <iostream>
#include <limits>
#include <map>
#include <vector>

int Search(std::string& c, std::string& out) {
  int error = 0;
  std ::string ::size_type pos = c.find(" ");
  if (pos != std ::string ::npos) {
    out = c.substr(0, pos);
    c.erase(0, pos + 1);
  } else {
    error = 1;
  }
  return error;
}

struct Event {
  int day;
  int hour;
  int minute;
  int id;
  char status;
};

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
  std::vector<Event> events;
  for (unsigned int i = 0; i < string_data.size(); ++i) {
    Event event;
    std::string status;
    std::string id;
    std::string minute;
    std::string day;
    std::string hour;
    Search(string_data[i], day);
    Search(string_data[i], hour);
    Search(string_data[i], minute);
    Search(string_data[i], id);
    status = string_data[i];
    event.day = atoi(&day[0]);
    event.hour = atoi(&hour[0]);
    event.minute = atoi(&minute[0]);
    event.id = atoi(&id[0]);
    event.status = string_data[i][0];
    events.push_back(event);
  }
  std::map<int, std::vector<Event>> events_byid;
  for (const auto& event : events) {
    events_byid[event.id].push_back(event);
  }
  for (auto& [id, eventList] : events_byid) {
    std::sort(eventList.begin(), eventList.end(),
              [](const Event& a, const Event& b) {
                if (a.day != b.day) return a.day < b.day;
                if (a.hour != b.hour) return a.hour < b.hour;
                return a.minute < b.minute;
              });
  }
  for (const auto& [id, eventList] : events_byid) {
    int counter = 0;
    int start_time = 0;
    int finish_time = 0;
    for (const auto& event : eventList) {
      if (event.status == 'A') {
        start_time = event.day * 24 * 60 + event.hour * 60 + event.minute;
      } else if (event.status == 'S' || event.status == 'C') {
        finish_time = event.day * 24 * 60 + event.hour * 60 + event.minute;
        counter = counter + abs(finish_time - start_time);
      }
    }
    std::cout << counter << ' ';
  }
  return 0;
}