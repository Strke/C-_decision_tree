#include "../include/config.h"
#include "../include/tool.h"
#include "../include/cart.h"
#include <iostream>

void print(std::vector<std::vector<double>>& data){
    int n = data.size();
    int m = data[0].size();
    for(int i = 0; i < n; i ++){
        for(int j = 0; j < m; j ++)
            std::cout << data[i][j] << " ";
        std::cout << std::endl;
    }
}

int main(){
    std::vector<std::vector<double>> data =  lin::decision_tree::Data2Vector(lin::decision_tree::train_data_road);
    //print(data);
    lin::decision_tree::Cart cart(data);
    std::cout << "compelete inital" << std::endl;
    cart.train_model();
    std::cout << "compelete train_model" << std::endl;
    cart.save_model();
    std::cout << "complete save_model" << std::endl;
    return 0;
}