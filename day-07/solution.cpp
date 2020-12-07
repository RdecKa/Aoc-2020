#include "aoclib.hpp"

class Node {
 private:
  std::string color;
  std::vector<std::pair<Node*, int>> outgoing;  // includes
  std::vector<Node*> incoming;                  // includedIn

 public:
  Node(std::string& color) : color(color) {}
  std::string getColor() { return this->color; }
  void addIncoming(Node* n) { this->incoming.push_back(n); }
  void addOutgoing(std::vector<std::pair<Node*, int>>& outgoing) { this->outgoing = outgoing; }
  std::vector<Node*> getIncoming() { return this->incoming; }
  std::vector<std::pair<Node*, int>> getOutgoing() { return this->outgoing; }
};

class Graph {
 private:
  std::unordered_map<std::string, Node> nodes;

 public:
  Graph(class std::vector<std::string>& input) {
    for (auto line : input) {
      this->addNode(line);
    }
  }

  Node* getOrCreateNode(std::string& color) {
    auto search = this->nodes.find(color);
    if (search == this->nodes.end()) {
      auto n = Node(color);
      this->nodes.insert(std::pair<std::string, Node>(color, n));
      search = this->nodes.find(color);
    }
    return &search->second;
  }

  void addNode(std::string& line) {
    std::vector<std::string> v;
    boost::split(v, line, boost::is_any_of(" "));
    std::string parentColor = v[0] + v[1];
    auto parent = this->getOrCreateNode(parentColor);

    int i = 4;
    std::vector<std::pair<Node*, int>> outgoing;
    while (i < v.size()) {
      if (v[i] == "no") {
        break;
      }
      int num = std::atoi(v[i].c_str());
      std::string childColor = v[i + 1] + v[i + 2];
      auto child = this->getOrCreateNode(childColor);
      child->addIncoming(parent);
      outgoing.push_back(std::pair<Node*, int>(child, num));
      i += 4;
    }
    parent->addOutgoing(outgoing);
    this->nodes.insert(std::pair<std::string, Node>(parentColor, *parent));
  }

  int getSize() { return this->nodes.size(); }
};

void getPredecessors(Graph& graph, Node* node, std::set<std::string>& currentSet) {
  auto predecessors = node->getIncoming();
  for (auto& p : predecessors) {
    auto parentColor = p->getColor();
    if (currentSet.find(parentColor) == currentSet.end()) {
      currentSet.insert(parentColor);
      getPredecessors(graph, p, currentSet);
    }
  }
}

void part1(Graph graph) {
  std::string shinyGoldName = "shinygold";
  auto shinyGold = graph.getOrCreateNode(shinyGoldName);
  std::set<std::string> predecessors;
  getPredecessors(graph, shinyGold, predecessors);
  std::cout << predecessors.size() << std::endl;
}

int countSubBags(Node* node) {
  auto outgoing = node->getOutgoing();
  if (outgoing.size() == 0) {
    return 1;
  }
  int sum = 1;
  for (auto out : outgoing) {
    sum += out.second * countSubBags(out.first);
  }
  return sum;
}

void part2(Graph graph) {
  std::string shinyGoldName = "shinygold";
  auto shinyGold = graph.getOrCreateNode(shinyGoldName);
  std::cout << countSubBags(shinyGold) - 1 << std::endl;
}

int main() {
  const std::string filename = "../day-07/input.txt";
  auto input = aoc::readStringInput(filename);
  auto graph = Graph(input);
  part1(graph);
  part2(graph);

  return 0;
}
