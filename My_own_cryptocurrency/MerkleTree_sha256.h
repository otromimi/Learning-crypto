#pragma once


#include <string>
#include <vector>


namespace My_own_crypto {

    struct Node_mt {

        std::string hash;
        Node_mt* left;
        Node_mt* right;

        Node_mt(std::string data);
    };
  
    struct MerkleTree {
        

        Node_mt* root;
        std::vector<Node_mt*> leaves;

        void populateTree(const std::vector <std::string> hash_list);

        ~MerkleTree();

        void printTree(Node_mt* n, int indent);

    private:
        
        void buildTree(std::vector<Node_mt*> blocks);

        void deleteTree(Node_mt* n);

    };

}