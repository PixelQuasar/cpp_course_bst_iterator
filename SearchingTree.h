//
// Created by QUASARITY on 26.09.2024.
//

#ifndef SEARCHTREE_H
#define SEARCHTREE_H

#include <stack>

template<class K, class V>
struct TreeNode {
    TreeNode *left;
    TreeNode *right;
    K key;
    V value;

    TreeNode(const K key, const V value) : key(key), value(value) {}
};

template<class K, class V>
class TreeIter {
    using PtrType = TreeNode<K, V>;
private:
    bool m_is_end = false;
    std::stack<PtrType *> m_node_stack;
    PtrType *m_ptr;

    void fill(PtrType *root) {
        while (root) {
            m_node_stack.push(root);
            root = root->left;
        }
    }

public:
    explicit TreeIter() : m_ptr(nullptr) { this->fill(nullptr); }

    explicit TreeIter(PtrType *root) { this->fill(root); this->next(); }

    TreeIter(PtrType *ptr, std::stack<PtrType *> stack) { m_ptr = ptr; m_node_stack = stack; fill(ptr->right); }

    static TreeIter begin_iter(PtrType *root) { auto iter = TreeIter(root); return iter; }

    static TreeIter end_iter() { auto iter = TreeIter(); iter.m_is_end = true; return iter; }

    std::pair<K, V> operator*() const { return std::pair(m_ptr->key, m_ptr->value); }

    friend bool operator==(const TreeIter &a, const TreeIter &b) { return a.m_ptr == b.m_ptr || (a.m_is_end && b.m_is_end); };

    friend bool operator!=(const TreeIter &a, const TreeIter &b) { return !(a == b); };

    TreeIter &operator++() { next(); return *this; }

    TreeIter operator++(int) { TreeIter tmp = *this; next(); return tmp; }

    bool is_end() { return m_is_end; }

    void next() {
        if (has_next()) {
            PtrType *target = m_node_stack.top();
            m_node_stack.pop();
            fill(target->right);
            m_ptr = target;
        } else {
            m_is_end = true;
        }
    }

    bool has_next() { return !m_node_stack.empty(); }
};

template<class K, class V>
class TreeRange {
private:
    using IterType = TreeIter<K, V>;
    IterType m_begin, m_end;
public:
    TreeRange(const IterType &begin, const IterType &end) : m_begin(begin), m_end(end) {}

    IterType begin() { return m_begin; }

    IterType end() { return m_end; }
};

template<class K, class V>
class SearchingTree {
private:
    TreeNode<K, V> *m_root;

    void recursiveInsert(TreeNode<K, V> *root, const K &key, const V &value) {
        if(root == nullptr) {
            m_root = new  TreeNode(key, value);
            return;
        }
        if (key < root->key) {
            if(root->left == nullptr) {
                root->left = new  TreeNode(key, value);
                return;
            } else {
                SearchingTree<K, V>::recursiveInsert(root->left, key, value);
            }
        }
        if (key > root->key) {
            if(root->right == nullptr) {
                root->right = new  TreeNode(key, value);
                return;
            } else {
                SearchingTree<K, V>::recursiveInsert(root->right, key, value);
            }
        }
    }

    TreeNode<K, V> *recursiveDelete(TreeNode<K, V> *current, const K &key) {
        if (!current) return nullptr;
        if (current->key > key) {
            current->left = SearchingTree<K, V>::recursiveDelete(current->left, key);
        }
        else if (current->key < key) {
            current->right = SearchingTree<K, V>::recursiveDelete(current->right, key);
        }
        else {
            if (!current->left) {
                auto temp = current->right;
                delete current;
                return temp;
            }
            else if (!current->right) {
                auto temp = current->left;
                delete current;
                return temp;
            } else {
                auto successor = SearchingTree<K, V>::getSuccessor(current);
                current->key = successor->key;
                current->value = successor->value;
                current->right = SearchingTree<K, V>::recursiveDelete(current->right, successor->key);
            }
        }
        return current;
    }

    static TreeNode<K, V> *getSuccessor(TreeNode<K, V> *current) {
        current = current->right;
        while (current != NULL && current->left != NULL) current = current->left;
        return current;
    }
public:
    SearchingTree() : m_root(nullptr) {}

    void insert(const K &key, const V &value) { recursiveInsert(m_root, key, value); }

    void erase(const K &key) { recursiveDelete(m_root, key); }

    TreeIter<K, V> find(const K &key) {
        TreeNode<K, V>* current = m_root;
        std::stack<TreeNode<K, V>*> upper_stack = {};

        while (current) {
            if (current->key == key) {
                return TreeIter<K, V>(current, upper_stack);
            }
            if (current->key < key) {
                current = current->right;
            }
            else {
                upper_stack.push(current);
                current = current->left;
            }
        }

        return TreeIter<K, V>::end_iter();
    }

    TreeIter<K, V> begin() { return TreeIter<K, V>::begin_iter(m_root); };

    TreeIter<K, V> end() { return TreeIter<K, V>::end_iter(); };

    TreeRange<K, V> range(K from, K to) {
        auto iter = TreeIter<K, V>(m_root);
        TreeIter<K, V> begin, end;
        while ((*iter).first != from && !iter.is_end()) {
            iter.next();
        }
        begin = iter;
        while ((*iter).first != to && !iter.is_end()) {
            iter.next();
        }
        TreeRange range(begin, iter);
        return range;
    }
};

#endif //SEARCHTREE_H
