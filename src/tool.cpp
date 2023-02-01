#include "../include/tool.h"
#include <sstream>
#include <fstream>
#include <sstream>
#include <iostream>

namespace lin{
namespace decision_tree{

std::string Vector2String(std::vector<double>& sample){ 
    std::string res = "";
    for(auto x : sample){
        res += std::to_string(x);//只能保存六位小数
        res += " "; //补充空格分割数字
    }
    return res;
}

std::vector<double> String2Vector(std::string& sample){
    std::vector<double> res;
    double x = 0;
    std::istringstream iss(sample);
    double num;
    while(iss >> num){
        res.push_back(num);
    }
    return res;
}

std::vector<double> FrontFollow(Node* headnode){
    std::vector<double> res, left, right;
    if(headnode == nullptr) return res;
    res.push_back(headnode -> val);
    left = FrontFollow(headnode -> leftnode);
    right = FrontFollow(headnode -> rightnode);
    for(int i = 0; i < left.size(); i ++) res.push_back(left[i]);
    for(int i = 0; i < right.size(); i ++) res.push_back(right[i]);
    return res;
}

std::vector<double> MiddleFollow(Node* headnode){
    std::vector<double> res, left, right;
    if(headnode == nullptr) return res;
    left = FrontFollow(headnode -> leftnode);
    right = FrontFollow(headnode -> rightnode);
    for(int i = 0; i < left.size(); i ++) res.push_back(left[i]);
    res.push_back(headnode -> val);
    for(int i = 0; i < right.size(); i ++) res.push_back(right[i]);
    return res;
}

Node* TreeBuild(std::vector<double>& front, std::vector<double>& middle){
    std::vector<double> a, b, c, d;
    Node* headnode = (Node*)malloc(sizeof(Node));
    headnode -> val = front[0];
    int point = 0;
    for(point = 0; middle[point] != front[0]; point ++);
    a.assign(front.begin() + 1, front.begin() + point + 1);
    b.assign(middle.begin(), middle.begin() + point);
    c.assign(front.begin() + point + 1, front.end());
    d.assign(middle.begin() + point + 1, middle.end());
    headnode -> leftnode = TreeBuild(a, b);
    headnode -> rightnode = TreeBuild(c, d);
    return headnode;
}

std::vector<std::vector<double>> Data2Vector(std::string road){
	std::ifstream inFile(road, std::ios::in);
	std::string lineStr;
	std::vector<std::vector<double>> strArray;
	while (getline(inFile, lineStr))
	{
		// 打印整行字符串
		//std::cout << lineStr << std::endl;
		// 存成二维表结构
		std::stringstream ss(lineStr);
		std::string str;
		std::vector<std::string> lineArray;
		std::vector<double> doubleArray;
		// 按照逗号分隔
		while (getline(ss, str, ',')){
			std::istringstream iss(str);
    		double num; iss>>num;
			doubleArray.push_back(num);
		}
		strArray.push_back(doubleArray);
	}
    return strArray;
}
}
}