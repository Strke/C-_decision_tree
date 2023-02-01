from data_read import data_load_clean, load_data, split_tt
from config import choose_feature

if __name__ == "__main__":
    # 只保留k个特征
    k = choose_feature
    data, pos, neg = data_load_clean(k)
    # 有数据了就加载数据，节省时间
    # data = load_data()
    # 划分测试集和训练集
    train_set, vail_set, test_set = split_tt(data, pos, neg)

    
