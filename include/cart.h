#ifndef __CART__
#define __CART__

#include "config.h"
#include <vector>

namespace lin{
namespace decision_tree{

typedef struct treenode{
    double val;
    Node* leftnode;
    Node* rightnode;
}Node;


class Cart
{
    public:
        Cart();
        Cart(std::vector<std::vector<double>>&);  //使用一个二维数组构造类

    private:
        std::vector<std::vector<double>> data;   //data代表一个二维数据矩阵
        int row = 0;  //row代表数据行数
        int col = 0;  //col代表数据列数
        int choose_col = 0;
        int choose_row = 0;
        std::string model_road = "../data/BTree.pickle";
        Node* head;
        //模型中-1表示yes
        //-2表示no
        //-3表示空元素。

    
    public:
        double gini(std::vector<double>&);   //求指定一列元素的基尼系数，该列元素只包含0和1
        void sort_by_kth_col(std::vector<std::vector<double>>&, int);
        inline
        Node* buildTree(std::vector<std::vector<double>>, int); //建树的具体实现
        void train_model();  //模型训练
        void save_model(); //保存模型
        void load_model(); //加载模型
};


}//namespace decision_tree
}//namespace lin
#endif