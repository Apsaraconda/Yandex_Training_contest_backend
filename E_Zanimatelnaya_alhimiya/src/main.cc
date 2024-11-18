#include <cstdint>
#include <iostream>
#include <limits>
#include <vector>

void Impossible(std::vector<std::pair<uint64_t, uint64_t>> &zelies, uint32_t i,
                std::vector<std::vector<uint32_t>> &comp_list,
                std::vector<bool> &possible, std::vector<uint8_t> &status) {
  possible[i] = false;
  zelies[i] = {0, 0};
  comp_list[i].clear();
  status[i] = 2;
}

void Count(std::vector<std::pair<uint64_t, uint64_t>> &zelies, uint32_t i,
           std::vector<std::vector<uint32_t>> &comp_list,
           std::vector<bool> &possible, std::vector<uint8_t> &status) {
  uint32_t cur_zelie = i;
  if (!status[cur_zelie]) {
    status[cur_zelie] = 1;
    std::pair<uint64_t, uint64_t> componentsAB{0, 0};
    for (uint32_t j = 0; j < comp_list[i].size(); ++j) {
      uint32_t component = comp_list[i][j];
      if (component == 1) {
        componentsAB.first++;
      } else if (component == 2) {
        componentsAB.second++;
      } else {
        if (possible[component] && !zelies[component].first &&
            !zelies[component].second) {
          Count(zelies, component, comp_list, possible, status);
        }
        if (possible[component]) {
          componentsAB.first = componentsAB.first + zelies[component].first;
          componentsAB.second = componentsAB.second + zelies[component].second;
        } else {
          Impossible(zelies, cur_zelie, comp_list, possible, status);
          break;
        }
      }
    }
    if (possible[cur_zelie]) {
      zelies[cur_zelie].first = componentsAB.first + zelies[cur_zelie].first;
      zelies[cur_zelie].second = componentsAB.second + zelies[cur_zelie].second;
      status[cur_zelie] = 2;
    } else {
      Impossible(zelies, cur_zelie, comp_list, possible, status);
    }
  } else if (status[cur_zelie] == 1) {
    Impossible(zelies, cur_zelie, comp_list, possible, status);
  }
}

int main() {
  uint32_t N;
  std::cin >> N;
  std::vector<std::vector<uint32_t>> comp_list{N + 1};
  std::vector<std::pair<uint64_t, uint64_t>> zelies(N + 1);
  zelies[1] = {1, 0};
  zelies[2] = {0, 1};
  comp_list[1].push_back({1});
  comp_list[2].push_back({2});
  std::vector<bool> possible(N + 1, true);
  std::vector<uint8_t> status(N + 1, 0);
  for (uint32_t i = 3; i <= N; ++i) {
    uint32_t K;
    std::cin >> K;
    for (uint32_t j = 0; j < K; ++j) {
      uint32_t component;
      std::cin >> component;
      comp_list[i].push_back(component);
    }
  }

  for (uint32_t i = 3; i <= N; ++i) {
    if (possible[i] && !zelies[i].first && !zelies[i].second) {
      Count(zelies, i, comp_list, possible, status);
    }
  }

  uint32_t Q;
  std::cin >> Q;
  std::vector<uint8_t> result(Q);
  for (uint32_t i = 0; i < Q; ++i) {
    uint32_t x, y;
    uint32_t number;
    std::cin >> x >> y >> number;
    std::pair<uint64_t, uint64_t> values{zelies[number]};
    if (number <= N && possible[number] && values.first <= x &&
        values.second <= y && (values.first || values.second)) {
      result[i] = 1;
    } else {
      result[i] = 0;
    }
  }
  for (uint32_t i = 0; i < Q; ++i) {
    std::cout << (int)result[i];
  }
  return 0;
}