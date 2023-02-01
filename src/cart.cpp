#include "../include/cart.h"
#include "../include/tool.h"
#include <fstream>
#include <iostream>
#include <algorithm>

namespace lin{
namespace decision_tree{

Cart::Cart(std::vector<std::vector<double>>& x)
:data(x), row(x.size()), col(x[0].size()){
    head = new Node;
};

Cart::~Cart(){
    if(head != NULL){
        delete head;
        head = NULL;
    }
}

double Cart::gini(std::vector<double>& p){
    double yes = 0;
    for(auto elem: p) yes += elem;
    double no = row - yes;
    double res = 1 - (yes / row)*(yes / row) - (no / row)*(no / row);
    return res;
}

static bool cmp_(std::vector<double> a, std::vector<double> b){ //根据第k列比较，从而达到行间互换的效果
    //int k = a[a.size() - 1];
    return a[compare_col] < b[compare_col];
}

void Cart::sort_by_kth_col(std::vector<std::vector<double>>& data_part, int k){
    //for(auto x : data_part) x.push_back(k); //把要排序的列号作为元素放入数组
    compare_col = k;
    std::sort(data_part.begin(), data_part.end(), cmp_);
    //std::cout << "compare_col = " << compare_col << std::endl;
    //for(auto x : data_part) x.pop_back();
}

Node* Cart::buildTree(std::vector<std::vector<double>> data_for_every_tree, int k){
    if(data_for_every_tree.size() == 0){
        return nullptr;
    }
    //data_for_every_tree代表构建每颗子树需要用到的数据，k表示当前是第k列数据
    //std::cout << k << "th cycle" << std::endl;
    //std::vector<double> judge;
    double g = 0;int i = 0;
    int n = data_for_every_tree.size();
    int m = data_for_every_tree[0].size();
    //std::cout << "data_for_every_tree大小" << data_for_every_tree.size() << std::endl;
    for(i = 0; i < row; i ++){
        //judge.push_back(data_for_every_tree[i][row - 1]);
        g = g + data_for_every_tree[i][col - 1];
    }
    std::cout << "g = " << g << std::endl;
    if(k + 1 == col){
        if(g > row - g){
            Node* x = (Node*)malloc(sizeof(Node));
            x -> val = -1;x -> leftnode = NULL;x -> rightnode = NULL;
            return x;
            }//return  yes
        else {
            Node* x = (Node*)malloc(sizeof(Node));
            x -> val = -2;x -> leftnode = NULL;x -> rightnode = NULL;
            return x;
            } // return no
    }
    if(abs(g) < double_distance){
            Node* x = (Node*)malloc(sizeof(Node));
            x -> val = -2;x -> leftnode = NULL;x -> rightnode = NULL;
            return x;
            } // return no
    if(abs(g - row) < double_distance){ // return yes
            Node* x = (Node*)malloc(sizeof(Node));
            x -> val = -1;x -> leftnode = NULL;x -> rightnode = NULL;
            return x;
            }

    Node* ans = (Node*)malloc(sizeof(Node));
    std::cout << "k = " << k << std::endl;
    sort_by_kth_col(data_for_every_tree, k);
    std::cout << "bug" << std::endl;
    double gini_d = 1 - (g / n) * (g / n) - ((n - g)/n) * ((n - g)/n);
    std::vector<double> import_col;
    for(auto x : data_for_every_tree) import_col.push_back(x[k]);
    std::vector<double> decision_col;
    for(int i = 0; i < n; i ++) decision_col.push_back((import_col[i] + import_col[i + 1]) / 2);
    //计算最优分类点
    std::vector<double> gini_array;
    for(int i = 0; i < n - 1; i ++){
        std::vector<double> d1, d2;
        for(int j = 0; j <= i; j ++) d1.push_back(data_for_every_tree[j][col - 1]);
        for(int j = i + 1; j < n; j ++) d2.push_back(data_for_every_tree[j][col - 1]);
        double t1 = (double(i+1)/n)*gini(d1);
        double t2 = (double(n-i-1)/n)*gini(d2);
        gini_array.push_back( t1 + t2 - gini_d);
    }
    int pos = 0;
    double minn = 100000;
    for(int i = 0; i < n - 1; i ++){
        if(gini_array[i] < minn){
            minn = gini_array[i];
            pos = i;
        }
    }
    std::vector<std::vector<double>> less_data, greater_data;
    for(int i = 0; i <= pos; i ++){
        std::vector<double> x;
        for(int j = 0; j < col; j ++){
            x.push_back(data_for_every_tree[i][j]);
        }
        less_data.push_back(x);
    }
    for(int i = pos + 1; i < n; i ++){
        std::vector<double> x;
        for(int j = 0; j < col; j ++){
            x.push_back(data_for_every_tree[i][j]);
        }
        greater_data.push_back(x);
    }
    std::cout << "less_data.size = " << less_data.size() << std::endl;
    std::cout << "greater_data.size = " << greater_data.size() << std::endl;
    ans -> val = decision_col[pos];
    ans -> leftnode = buildTree(less_data, k + 1);
    ans -> rightnode = buildTree(greater_data, k + 1);
    std::cout << "build compelete" << std::endl;
    return ans;
};

void Cart::train_model(){
    head = (Node*)malloc(sizeof(Node));
    std::cout << "begin build tree" << std::endl;
    head -> leftnode = buildTree(data, 0); //把模型的放在头结点的左孩子节点上
    head -> rightnode = nullptr;
    std::cout << "tree complete" << std::endl;
}

void Cart::save_model(){
    std::ofstream outFile;
    outFile.open(model_road);
    //首先把树转化为前序遍历和中序遍历
    std::vector<double> frt = FrontFollow(head->leftnode);
    std::vector<double> mid = MiddleFollow(head -> rightnode);
    //再转化为对应的字符串
    std::string frtstr = Vector2String(frt);
    std::string midstr = Vector2String(mid);
    //保存到保存路径下
    outFile << frtstr << "\n" << midstr;
    outFile.close();
}

void Cart::load_model(){
    std::string frstr, midstr;
    std::vector<double> frt, mid;
    std::ifstream inFile(model_road);
    getline(inFile, frstr);
    getline(inFile, midstr);
    frt = String2Vector(frstr);
    mid = String2Vector(midstr);
    head -> leftnode = TreeBuild(frt, mid);
}
}//namespace decision_tree
}//namespace lin