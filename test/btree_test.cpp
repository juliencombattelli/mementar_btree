#include "BTree.hpp"

#include <iostream>

struct Key {
    int value;
};

template <typename TK, typename TV>
struct btree::ValueSpecific<TK, TV, int> {
    using BTree = BTree<TK, TV>;
    using Node = typename BTree::Node;
    static void insert(Node*, const TK&, std::optional<TV>) {
        std::puts("int");
    }
};

int main() {
    btree::BTree<Key, char> bt;
    bt.insert(Key{1});

    btree::BPTree<Key, char> bpt;
    bpt.insert(Key{1});

    btree::BTree<Key, int> bti;
    bti.insert(Key{1});
}