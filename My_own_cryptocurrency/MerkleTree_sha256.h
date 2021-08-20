#pragma once


#include <string>
#include <vector>


namespace My_own_crypto {

    struct Node {

        std::string hash;
        Node* left;
        Node* right;

        Node(std::string data);
    };
  
    struct MerkleTree {
        

        Node* root;
        std::vector<Node*> leaves;

        MerkleTree(const std::vector <std::string> hash_list);

        ~MerkleTree();

        void printTree(Node* n, int indent);

    private:
        
        void buildTree(std::vector<Node*> blocks);

        void deleteTree(Node* n);

    };

}