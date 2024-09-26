#include <iostream>
#include "SearchingTree.h"

int main() {
    SearchingTree<int, int> tree;

    TreeIter<int, int>::end_iter();

    tree.insert(2, 2);
    tree.insert(1, 1);
    tree.insert(3, 3);
    tree.insert(-1, -1);
    tree.insert(6, 6);
    tree.insert(4, 4);
    tree.insert(5, 5);
    tree.insert(0, 0);
    tree.insert(9, 9);
    tree.insert(3, 3);

    tree.erase(6);

    auto found = tree.find(0);

    for (const auto [key, value] : tree) {
        std::cout << key << " " << value << std::endl;
    }

    return 0;
}
