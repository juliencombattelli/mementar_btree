#include "BTree.hpp"

#include <iostream>

namespace mementar {

struct Key {
    int value;
};

}  // namespace mementar

int main() {
    btree::BPTree<mementar::Key, std::string> bt;
    (void)bt;
}