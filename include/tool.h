#ifndef __TOOL__
#define __TOOL__

#include <vector>
#include <string>
#include "cart.h"

namespace lin{
namespace decision_tree{

std::string Vector2String(std::vector<double>&);  //将vector转化为string
std::vector<double> String2Vector(std::string&);  //将string转化为vector

std::vector<double> FrontFollow(Node*);   //前缀表达式
std::vector<double> MiddleFollow(Node*);  //中缀表达式

Node* TreeBuild(std::vector<double>&, std::vector<double>&); //根据先序遍历和中序遍历重现决策树


}//namespace decision_tree
}//namespace lin

#endif