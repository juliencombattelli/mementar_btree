#ifndef INCLUDE_BTREE_BTREE_HPP
#define INCLUDE_BTREE_BTREE_HPP

#include <iostream>
#include <optional>
#include <unordered_map>

#include "impl/Node.hpp"

namespace btree {

// Doubly linked value type
template <typename T>
struct LinkedValue {
    using value_type = T;
    LinkedValue* next;
    LinkedValue* prev;
    T value;
};

template <typename TK, typename TV, typename TValueDispatcher = TV>
struct ValueSpecific;

template <typename TK, typename TV>
using DefaultValueSpecific = ValueSpecific<TK, TV, void>;

// The non-specialized version is calling the default specialized one
template <typename TK, typename TV, typename>
struct ValueSpecific : DefaultValueSpecific<TK, TV> {};

///////////////////////////////////////////////////////////////////////////////

// Binary tree class
template <typename TKey, typename TValue = void>
class BTree {
public:
#ifdef SIMPLE_NODE
    using Node = impl::Node<TKey, TValue, impl::NodePolicyVector>;
#else
    using Node = impl::NodeInterface<TKey, impl::NodeWithValue<TKey, TValue>>;
#endif
    // Insert a (`key`, `value`) pair in the tree
    void insert(const TKey& key, std::optional<TValue> value = std::nullopt) {  // TODO: remove opt
        insert(m_root.get(), key, value);
    }
    // Remove the node corresponding to `key`
    void remove(const TKey& key);
    // Find the node corresponding to `key` and return an iterator to it
    void /* iterator */ find(const TKey& key);

protected:
    // Insert a (`key`, `value`) pair in the subtree having `node` as root
    void insert(Node* node, const TKey& key, std::optional<TValue> value) {
        ValueSpecific<TKey, TValue>().insert(node, key, value);
    }

    // Remove the node corresponding to `key` in the subtree having `node` as root
    Node* remove(Node* node, const TKey& key);

private:
    // Customization point for TValue-specific operations
    template <typename, typename, typename>
    friend struct ValueSpecific;

    // Root node of the tree
    std::unique_ptr<Node> m_root;
};

template <typename TKey, typename TValue>
using BPTree = BTree<TKey, LinkedValue<TValue>>;

// Default implementation for TValue-specific operations doing nothing
template <typename TK, typename TV>
struct ValueSpecific<TK, TV, void> {
    using BTree_ = BTree<TK, TV>;
    using Node = typename BTree_::Node;
    void insert(Node*, const TK&, std::optional<TV>) {
        std::puts("default");
    }
    Node* remove(Node*, const TK&) {
        std::puts("default");
    }
};

// Implementation used when TValue is LinkedValue
template <typename TK, typename TV>
struct ValueSpecific<TK, TV, LinkedValue<typename TV::value_type>> : DefaultValueSpecific<TK, TV> {
    using typename DefaultValueSpecific<TK, TV>::BTree;
    using typename DefaultValueSpecific<TK, TV>::Node;
    void insert(Node*, const TK&, std::optional<TV>) {
        std::puts("LinkedValue");
    }
};

}  // namespace btree

#endif  // INCLUDE_BTREE_BTREE_HPP
