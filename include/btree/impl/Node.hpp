#ifndef INCLUDE_BTREE_IMPL_NODE_HPP
#define INCLUDE_BTREE_IMPL_NODE_HPP

#include <memory>
#include <vector>

namespace utility {
// Perform a static downcast on unique ptrs
template <typename Derived, typename Base, typename Del>
std::unique_ptr<Derived, Del> static_unique_ptr_cast(std::unique_ptr<Base, Del>&& p) {
    auto d = static_cast<Derived*>(p.release());
    return std::unique_ptr<Derived, Del>(d, std::move(p.get_deleter()));
}
}  // namespace utility

namespace btree {
namespace impl {
///////////////////////////////////////////////////////////////////////////////
// Internal node representation
// Two implementation are available for now: one with code duplication and one without
// Use the following switch to choose the implementation
//#define SIMPLE_NODE

#ifdef SIMPLE_NODE
// Yes there is code duplication, but this is a simple implementation with only 13 LOC

struct NodePolicyVector {
    template <typename TNode>
    using container = std::vector<TNode>;
};

// Node class with all data members
template <typename TKey, typename TValue, typename TNodePolicy>
struct Node {
    using TContainer = typename TNodePolicy::template container<Node>;
    TContainer childs;
    Node* parent{nullptr};
    TKey key;
    TValue value;
};

// Specialization of Node with no value member if the requested TValue type is void
template <typename TKey, typename TNodePolicy>
struct Node<TKey, void, TNodePolicy> {
    using TContainer = typename TNodePolicy::template container<Node>;
    TContainer childs;
    Node* parent{nullptr};
    TKey key;
};

template <size_t N>
struct NodePolicyArray {
    template <typename TNode>
    using container = std::array<TNode, N>;
};

template <size_t N, typename TKey, typename TValue = void>
using NodeArray = Node<NodePolicyArray<N>, TKey, TValue>;

#else
// Yes there is NO code duplication, but this is a far more complexe implementation with more than
// 40 LOC

// Base node class with the data members
template <typename TKey>
struct NodeBase {
    TKey key;

protected:
    std::unique_ptr<NodeBase> left{nullptr};
    std::unique_ptr<NodeBase> right{nullptr};
    NodeBase* parent{nullptr};
};

// Node interface implementing safe downcasts in setters/getters
template <typename TKey, typename TDerived>
struct NodeInterface : public NodeBase<TKey> {
    using Base = NodeBase<TKey>;

    // TODO: Assert the downcast is safe:
    //      - static_assert is base of
    //      - static_assert has no virtual dtor

    TDerived* left() const {
        return static_cast<TDerived*>(Base::left.get());
    }
    void left(std::unique_ptr<TDerived> uptr) {
        Base::left = utility::static_unique_ptr_cast<Base>(uptr);  // TODO: Wrong cast
    }

    TDerived* right() const {
        return static_cast<TDerived*>(Base::left.get());
    }
    void right(std::unique_ptr<TDerived> uptr) {
        Base::right = utility::static_unique_ptr_cast<Base>(uptr);
    }

    TDerived* parent() const {
        return static_cast<TDerived*>(Base::parent);
    }
    void parent(TDerived* ptr) {
        Base::parent = ptr;
    }
};

// Node class adding a value member
template <typename TKey, typename TValue = void>
struct NodeWithValue : public NodeInterface<TKey, NodeWithValue<TKey, TValue> > {
    TValue value;
};

// Specialization of NodeWithValue adding no value member if the requested TValue type is void
template <typename TKey>
struct NodeWithValue<TKey, void> : public NodeInterface<TKey, NodeWithValue<TKey, void> > {};

#endif

}  // namespace impl
}  // namespace btree

#endif  // INCLUDE_BTREE_IMPL_NODE_HPP
