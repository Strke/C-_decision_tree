#include "../include/cart.h"
#include <fstream>
#include <iostream>
#include <algorithm>

namespace lin{
namespace decision_tree{

Cart::Cart(std::vector<std::vector<double>>& x)
:data(x), row(data.size()), col(data[0].size()) {};

double Cart::gini(std::vector<double>& p){
    double yes = 0;
    for(auto elem: p) yes += elem;
    double no = row - yes;
    double res = 1 - (yes / row)*(yes / row) - (no / row)*(no / row);
    return res;
}

bool cmp_(std::vector<double> a, std::vector<double> b){
    int k = a[a.size() - 1];
    return a[k] > b[k];
}

void Cart::sort_by_kth_col(std::vector<std::vector<double>>& data_part, int k){
    for(auto x : data_part) x.push_back(k);
    std::sort(data_part.begin(), data_part.end(), cmp_);
    for(auto x : data_part) x.pop_back();
}

inline
Node* Cart::buildTree(std::vector<std::vector<double>> data_for_every_tree, int k){
    std::vector<double> judge;
    double g = 0;
    for(int i = 0; i < row; i ++){
        judge.push_back(data_for_every_tree[i][row - 1]);
        g += data_for_every_tree[i][row - 1];
    }
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
    sort_by_kth_col(data_for_every_tree, k);
    double gini_d = 1 - (g / row) * (g / row) - ((row - g)/row) * ((row - g)/row);
    std::vector<double> import_col;
    for(auto x : data_for_every_tree) import_col.push_back(x[k - 1]);
    std::vector<double> decision_col;
    for(int i = 0; i < row; i ++) decision_col.push_back((import_col[i] + import_col[i + 1]) / 2);

    //计算最优分类点
    std::vector<double> gini_array;
    for(int i = 0; i < row; i ++){
        std::vector<double> d1, d2;
        for(int j = 0; j <= i; j ++) d1.push_back(data_for_every_tree[j][col - 1]);
        for(int j = i + 1; j < row; j ++) d2.push_back(data_for_every_tree[j][col - 1]);
        gini_array.push_back(((i+1)/row)* gini(d1)+((row-i-1)/row)* gini(d2)-gini_d);
    }
    int pos = 0, minn = 100000;
    for(int i = 0; i < row; i ++)
        if(gini_array[i] < minn){
            minn = gini_array[i];
            pos = i;
        }
    std::vector<std::vector<double>> less_data, greater_data;
    for(int i = 0; i <= pos; i ++){
        std::vector<double> x;
        for(int j = 0; j < col; j ++){
            x.push_back(data_for_every_tree[i][j]);
        }
        less_data.push_back(x);
    }
    for(int i = pos + 1; i < row; i ++){
        std::vector<double> x;
        for(int j = 0; j < col; j ++){
            x.push_back(data_for_every_tree[i][j]);
        }
        greater_data.push_back(x);
    }
    ans -> val = decision_col[pos];
    ans -> leftnode = buildTree(less_data, k + 1);
    ans -> leftnode = buildTree(greater_data, k + 1);
};

void Cart::train_model(){
    buildTree(data, 0);
}

void Cart::save_model(){
    std::ofstream outFile;
    outFile.open(model_road);
   // for(auto x : model){
    //    outFile << x;
     //   outFile << " ";
    //}
    outFile.close();
}

void Cart::load_model(){
}


}//namespace decision_tree
}//namespace lin