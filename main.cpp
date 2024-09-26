#include <iostream>
#include "SearchingTree.h"

int main() {
    my_bst::SearchingTree<int, int> tree;

    tree.insert(2, 2);
    tree.insert(1, 1);
    tree.insert(3, 3);
    tree.insert(-1, -1);
    tree.insert(6, 6);
    tree.insert(4, 4);
    tree.insert(5, 5);
    tree.insert(0, 0);
    tree.insert(9, 9);
    tree.erase(6);

    auto found = tree.find(3);

    while (!found.is_end()) {
        std::cout << (*found).first << " ";
        ++found;
    }
    std::cout << std::endl << std::endl;

    for (const auto [key, value] : tree) {
        std::cout << key << " ";
    }

    std::cout << std::endl << std::endl;

    for (const auto [key, value] : tree.range(2, 5)) {
        std::cout << key << " ";
    }

    return 0;
}
