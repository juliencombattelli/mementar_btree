#ifndef INCLUDE_BTREE_HPP
#define INCLUDE_BTREE_HPP

#include <memory>
#include <optional>
#include <unordered_map>

namespace btree {

template <typename TKey, typename TValue>
class BTree {
    // Internal node representation
    class Node {
        std::unique_ptr<Node> left{nullptr};
        std::unique_ptr<Node> right{nullptr};
        Node* parent{nullptr};
        TKey key;
        std::optional<TValue> data{std::nullopt};
    };

public:
    // Insert a (`key`, `value`) pair in the tree
    void insert(const TKey& key, std::optional<TValue> value = std::nullopt);
    // Remove the node corresponding to `key`
    void remove(const TKey& key);
    // Find the node corresponding to `key` and return an iterator to it
    void /* iterator */ find(const TKey& key);

private:
    // Insert a (`key`, `value`) pair in the subtree having `node` as root
    void insert(Node* node, const TKey& key, std::optional<TValue> value = std::nullopt);
    // Remove the node corresponding to `key` in the subtree having `node` as root
    Node* remove(Node* node, const TKey& key);

    std::unique_ptr<Node> m_root;
};

// Doubly linked value type
template <typename T>
struct LinkedValue {
    LinkedValue* next;
    LinkedValue* prev;
    T value;
};

template <typename TKey, typename TData>
using BPTree = BTree<TKey, LinkedValue<TData>>;

}  // namespace btree

#endif  // INCLUDE_BTREE_HPP
