#ifndef __CONFIG__
#define __CONFIG__

#include <string>

namespace lin{
namespace decision_tree{
     
int choose_feature = 7;
int double_distance = 1e-6;

std::string all_data_road = "../data/test_kunming.csv";
std::string after_clean_saving_data = "../data/data.csv";
std::string after_clean_saving_rate = "../data/rate.csv";
std::string model_save_road = "../data/BTree.pickle";

}//namespace decision_tree
}//namespace lin
#endif