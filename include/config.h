#ifndef __CONFIG__
#define __CONFIG__

#include <string>

namespace lin{
namespace decision_tree{
     
static int choose_feature = 7;
static int double_distance = 1e-6;

static int compare_col = 0;

static std::string all_data_road = "../data/test_kunming.csv";
static std::string after_clean_saving_data = "../data/data.csv";
static std::string after_clean_saving_rate = "../data/rate.csv";
static std::string train_data_road = "../data/train_data.csv";
static std::string model_save_road = "../data/BTree.pickle";

}//namespace decision_tree
}//namespace lin
#endif