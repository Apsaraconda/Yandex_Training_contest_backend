#include <algorithm>
#include <cstdint>
#include <iostream>
#include <limits>
#include <vector>

class Zakaz {
 public:
  Zakaz(uint32_t start = 0, uint32_t end = 0, uint32_t cost = 0)
      : start_(start), end_(end), cost_(cost) {}

  uint32_t GetCost() const { return cost_; }
  uint32_t GetStart() const { return start_; }
  uint32_t GetEnd() const { return end_; }

 private:
  uint32_t start_;
  uint32_t end_;
  uint32_t cost_;
};

class Zapros {
 public:
  Zapros(uint32_t start = 0, uint32_t end = 0, uint8_t type = 0)
      : start_(start), end_(end), type_(type) {}

  uint8_t GetType() const { return type_; }
  uint32_t GetStart() const { return start_; }
  uint32_t GetEnd() const { return end_; }

 private:
  uint32_t start_;
  uint32_t end_;
  uint8_t type_;
};

class SegmentTree {
 public:
  SegmentTree(size_t n) : size_(n), data_(2 * n, 0) {}

  // Функция для добавления значения в дерево
  void Build(uint32_t pos, uint32_t value) { data_[pos + size_] = value; }
  void BuildSums() {
    for (uint32_t i = size_ - 1; i > 0; --i) {
      data_[i] = data_[2 * i] + data_[2 * i + 1];
    }
  }

  void Update(uint32_t pos, uint32_t value) {
    data_[pos += size_] = value;
    for (pos /= 2; pos > 0; pos /= 2)
      data_[pos] = data_[2 * pos] + data_[2 * pos + 1];
  }

  uint64_t Sum(uint32_t l, uint32_t r) {
    uint64_t res = 0;
    l += size_, r += size_;
    for (; l <= r; l /= 2, r /= 2) {
      if (l % 2 == 1) res += data_[l++];
      if (r % 2 == 0) res += data_[r--];
    }
    return res;
  }

 private:
  size_t size_;
  std::vector<uint64_t> data_;
};

int main() {
  uint32_t N;
  std::cin >> N;

  std::vector<Zakaz> zakazy(N);

  for (uint32_t i = 0; i < N; ++i) {
    uint32_t start, end, cost;
    std::cin >> start >> end >> cost;
    zakazy[i] = Zakaz(start, end, cost);
  }
  uint32_t tree_size = N;
  if (N % 2 != 0) tree_size = tree_size + 1;
  SegmentTree startTree(tree_size);
  SegmentTree endTree(tree_size);
  std::stable_sort(zakazy.begin(), zakazy.end(),
                   [](const Zakaz &a, const Zakaz &b) {
                     return a.GetStart() < b.GetStart();
                   });
  std::vector<Zakaz> zakazy_sorted_by_end = zakazy;
  std::stable_sort(
      zakazy_sorted_by_end.begin(), zakazy_sorted_by_end.end(),
      [](const Zakaz &a, const Zakaz &b) { return a.GetEnd() < b.GetEnd(); });

  for (uint32_t i = 0; i < N; ++i) {
    startTree.Build(i, zakazy[i].GetCost());
    endTree.Build(i, zakazy_sorted_by_end[i].GetEnd() -
                         zakazy_sorted_by_end[i].GetStart());
  }
  startTree.BuildSums();
  endTree.BuildSums();

  uint32_t Q;
  std::cin >> Q;
  std::vector<Zapros> zaprosy(Q);
  for (uint32_t i = 0; i < Q; ++i) {
    uint32_t start, end;
    uint8_t type;
    std::cin >> start >> end >> type;
    zaprosy[i] = Zapros(start, end, type);
  }

  for (const auto &zapros : zaprosy) {
    uint32_t sum = 0;

    if (zapros.GetType() == '1') {  // Type 1 запрос
      uint32_t begin = 0;
      uint32_t end = 0;
      for (uint32_t i = 0; i < zakazy.size(); ++i) {
        if (zakazy[i].GetStart() >= zapros.GetStart() &&
            zakazy[i].GetStart() <= zapros.GetEnd()) {
          if (!begin) begin = i;
        } else {
          if (begin && i) end = i - 1;
          break;
        }
      }
      sum = startTree.Sum(begin, end);
    } else if (zapros.GetType() == '2') {  // Type 2 запрос
      uint32_t begin = 0;
      uint32_t end = 0;
      for (uint32_t i = 0; i < zakazy_sorted_by_end.size(); ++i) {
        if (zakazy_sorted_by_end[i].GetEnd() >= zapros.GetStart() &&
            zakazy_sorted_by_end[i].GetEnd() <= zapros.GetEnd()) {
          if (!begin) begin = i;
        } else {
          if (begin && i) end = i - 1;
          break;
        }
      }
      sum = endTree.Sum(begin, end);
    }
    std::cout << sum << ' ';
  }
  return 0;
}
