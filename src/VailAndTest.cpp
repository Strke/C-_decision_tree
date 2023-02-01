#include "../include/VailAndTest.h"

namespace lin{
namespace decision_tree{

std::string run(std::vector<double>& testdata, Node* head, int k){
    if(head->val == -1 || head->val == -2){
        if(head->val == -1) return "yes";
        else return "no";
    }
    if(testdata[k] <= head->val) return run(testdata, head->leftnode, k + 1);
    else return run(testdata, head->rightnode, k + 1);
}

}//namespace decision_tree
}//namespace lin