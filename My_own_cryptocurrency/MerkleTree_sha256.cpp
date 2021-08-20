
#include <string>
#include <iostream>
#include <vector>
#include <iomanip>

#include "Tools.h"

using namespace My_own_crypto;

struct Node {

    std::string hash;
    Node* left;
    Node* right;

    Node(std::string data) {
        this->hash = data;
    }
};

struct MerkleTree {

    Node* root;
    std::vector<Node*> leaves;
    
    void buildTree(std::vector<Node*> blocks) {
        std::vector<Node*> nodes;
        while (blocks.size() != 1) {
            for (int l = 0, n = 0; l < blocks.size(); l = l + 2, n++) {
                if (l != blocks.size() - 1) { // checks for adjacent block
                    nodes.push_back(new Node(Tools::hash_sha256(blocks[l]->hash + blocks[l + 1]->hash))); // combine and hash adjacent blocks
                    nodes[n]->left = blocks[l]; // assign children
                    nodes[n]->right = blocks[l + 1];
                }
                else {
                    nodes.push_back(blocks[l]);
                }
            }
            blocks = nodes;
            nodes.clear();

        }
        this->root = blocks[0];
    }

    MerkleTree(const std::vector <std::string> hash_list) {

        for (std::string i : hash_list) {
            leaves.push_back(new Node(i));
        }

        // initialize leaves
        for (unsigned int i = 0; i < leaves.size(); i++) {
            leaves[i]->left = NULL;
            leaves[i]->right = NULL;
        }

        buildTree(leaves);
    }

    
   
    ~MerkleTree() {

        for (unsigned int k = 0; k < leaves.size(); k++) {
            delete leaves[k];
        }

        deleteTree(root);
        std::cout << "Tree deleted" << std::endl;
    }

    void printTree(Node* n, int indent) {
        if (n) {
            if (n->left) {
                printTree(n->left, indent + 4);
            }
            if (n->right) {
                printTree(n->right, indent + 4);
            }
            if (indent) {
                std::cout << std::setw(indent) << ' ';
            }
            std::cout << n->hash[0] << "\n ";
        }
    }

    void deleteTree(Node* n) {
        if (n) {
            deleteTree(n->left);
            deleteTree(n->right);
            n = NULL;
            delete n;
        }
    }

};
