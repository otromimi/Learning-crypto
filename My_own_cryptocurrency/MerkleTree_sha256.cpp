
#include <iostream>
#include <iomanip>

#include "Tools.h"
#include "MerkleTree_sha256.h"

using namespace My_own_crypto;


Node_mt::Node_mt(std::string data) {
    this->hash = data;
}



void MerkleTree::buildTree(std::vector<Node_mt*> blocks) {
    std::vector<Node_mt*> nodes;
    while (blocks.size() != 1) {
        for (int l = 0, n = 0; l < blocks.size(); l = l + 2, n++) {
            if (l != blocks.size() - 1) { // checks for adjacent block
                nodes.push_back(new Node_mt(Tools::hash_sha256(blocks[l]->hash + blocks[l + 1]->hash))); // combine and hash adjacent blocks
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

void MerkleTree::populateTree(const std::vector <std::string> hash_list) {

    for (std::string i : hash_list) {
        leaves.push_back(new Node_mt(i));
    }

    // initialize leaves
    for (unsigned int i = 0; i < leaves.size(); i++) {
        leaves[i]->left = NULL;
        leaves[i]->right = NULL;
    }

    buildTree(leaves);
}

    
   
MerkleTree::~MerkleTree() {

    /*for (unsigned int k = 0; k < leaves.size(); k++) {
        delete leaves[k];
    }*/

    deleteTree(root);
    //std::cout << "Tree deleted" << std::endl;
}

void MerkleTree::printTree(Node_mt* n, int indent) {
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
        std::cout << n->hash << "\n ";
    }
}

void MerkleTree::deleteTree(Node_mt* n) {
    if (n) {
        deleteTree(n->left);
        deleteTree(n->right);
        n = NULL;
        delete n;
    }
}


