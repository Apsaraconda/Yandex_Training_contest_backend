#include <cstdint>
#include <cstring>
#include <iostream>
#include <limits>
#include <vector>

class Vertex {
 public:
  Vertex() : p_(nullptr), vl_(nullptr), vr_(nullptr), num_(0) {}
  Vertex(uint16_t number)
      : p_(nullptr), vl_(nullptr), vr_(nullptr), num_(number) {}
  uint16_t GetNum() const { return num_; }
  Vertex *GetP() const { return p_; }
  Vertex *GetVl() const { return vl_; }
  Vertex *GetVr() const { return vr_; }
  void SetP(Vertex *new_p) {
    if (new_p) {
      p_ = new_p;
    } else {
      p_ = nullptr;
    }
  }
  void SetVl(Vertex *new_vl) {
    if (new_vl) {
      vl_ = new_vl;
      vl_->SetP(this);
    } else {
      vl_ = nullptr;
    }
  }
  void SetVr(Vertex *new_vr) {
    if (new_vr) {
      vr_ = new_vr;
      vr_->SetP(this);
    } else {
      vr_ = nullptr;
    }
  }

 private:
  Vertex *p_;
  Vertex *vl_;
  Vertex *vr_;
  uint16_t num_;
};

void SwapP(Vertex &cur, Vertex *bufP, Vertex *bufVl, Vertex *bufVr) {
  if (bufP->GetVl() == &cur) {  //  Меняем потомков с прежним родителем
    bufP->SetVl(nullptr);
    cur.SetVl(bufP);
    bufP->SetVl(bufVl);
    cur.SetVr(bufVr);
  } else if (bufP->GetVr() == &cur) {
    bufP->SetVr(nullptr);
    cur.SetVr(bufP);
    bufP->SetVr(bufVr);
    cur.SetVl(bufVl);
  }
}

void Swap(Vertex &cur, Vertex *bufP, Vertex *bufVl,
          Vertex *bufVr) {  //  Меняемся с родителем
  if (&cur == bufP->GetVl()) {
    bufP->SetVl(nullptr);
    cur.SetVl(bufP);
    if (bufVl) bufP->SetVl(bufVl);
  } else if (&cur == bufP->GetVr()) {
    bufP->SetVr(nullptr);
    cur.SetVr(bufP);
    if (bufVr) bufP->SetVr(bufVr);
  }
}

void LVRTraversal(Vertex *v) {
  if (!v) return;
  LVRTraversal(v->GetVl());         // Левое поддерево
  std::cout << v->GetNum() << ' ';  // Текущий узел
  LVRTraversal(v->GetVr());         // Правое поддерево
}

void Rotate(uint16_t number, std::vector<Vertex> &src) {
  Vertex *cur = &src[number];
  Vertex *bufP = cur->GetP();
  if (bufP) {
    Vertex *bufVl = cur->GetVl();
    Vertex *bufVr = cur->GetVr();
    Vertex *bufPP = bufP->GetP();
    if (bufPP) {
      if (bufP == bufPP->GetVl()) {
        bufPP->SetVl(cur);  //  то v становится левым ребенком pp
        Swap(*cur, bufP, bufVl, bufVr);
      } else if ((bufP == bufPP->GetVr())) {
        bufPP->SetVr(cur);
        Swap(*cur, bufP, bufVl, bufVr);
      }
    } else if (!bufPP) {
      SwapP(*cur, bufP, bufVl, bufVr);
      cur->SetP(nullptr);
    }
    cur = &src[number];
    if (!cur->GetP()) {
      src[0] = *cur;
    }
  }
}

void PrintMap(uint16_t N, std::vector<Vertex> &src) {
  for (uint16_t i = 1; i <= N; ++i) {
    std::cout << src[i].GetNum() << ' ';
    if (src[i].GetVl()) std::cout << "Vl = " << src[i].GetVl()->GetNum() << ' ';
    if (src[i].GetVr()) std::cout << "Vr = " << src[i].GetVr()->GetNum() << ' ';
    if (src[i].GetP()) std::cout << "P = " << src[i].GetP()->GetNum() << ' ';
    std::cout << "\n";
  }
  std::cout << "Root = " << src[0].GetNum() << ' ';
  std::cout << "\n";
}

int main() {
  uint16_t N;
  uint32_t Q;
  std::cin >> N >> Q;
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  if (N != 0 && Q != 0) {
    std::vector<Vertex> src(N + 1);
    Vertex first(1);
    src[1] = first;
    src[0] = first;
    for (uint16_t i = 2; i <= N; ++i) {
      Vertex next(i);
      src[i] = next;
      uint16_t parent = i / 2;
      Vertex *temp_v = &src[parent];
      if (i % 2 == 0) {
        temp_v->SetVl(&src[i]);
      } else {
        temp_v->SetVr(&src[i]);
      }
      src[i].SetP(temp_v);
    }

    for (uint32_t i = 0; i < Q; ++i) {
      uint16_t number;
      std::cin >> number;
      Rotate(number, src);
    }

    uint16_t root = src[0].GetNum();
    LVRTraversal(&src[root]);
  }
  return 0;
}