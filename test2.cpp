#include <fstream>
#include <iostream>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <string>


int compare_col;
int double_distance = 1e-6;

struct Node{
    double val;
    Node* leftnode;
    Node* rightnode;
};   //构造的决策树结点结构


class Cart
{
    public:
        Cart();
        Cart(std::vector<std::vector<double>>&);  //使用一个二维数组构造类，该二维数组为全部数据
        ~Cart();

    private:
        std::vector<std::vector<double>> data;   //data代表一个二维数据矩阵，代表全部数据
        int row = 0;  //row代表数据行数
        int col = 0;  //col代表数据列数
        int choose_col = 0;   //当前选中的列序号
        int choose_row = 0;   //当前选中的行序号 
        std::string model_road = "../data/BTree.pickle";  //模型的保存路径
        Node* head;    //决策树的头结点，其从左孩子开始为真正的头结点
        //模型中-1表示yes
        //-2表示no
        //-3表示空元素。

    
    public:
        double gini(std::vector<double>&);   //求指定一列元素的基尼系数，该列元素只包含0和1
        void sort_by_kth_col(std::vector<std::vector<double>>&, int); //将所有数据根据第k列排序
        inline
        Node* buildTree(std::vector<std::vector<double>>, int); //建树的具体实现
        void train_model();  //模型训练
        void save_model(); //保存模型
        void load_model(); //加载模型
};


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
    if(headnode == nullptr || !headnode->val) return res;
    std::cout << headnode -> val << std::endl;
    res.push_back(headnode -> val);
    if(headnode -> leftnode != nullptr) left = FrontFollow(headnode -> leftnode);
    if(headnode -> rightnode != nullptr) right = FrontFollow(headnode -> rightnode);
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
    std::cout << "compare_col = " << compare_col << std::endl;
    //for(auto x : data_part) x.pop_back();
}

Node* Cart::buildTree(std::vector<std::vector<double>> data_for_every_tree, int k){
    if(data_for_every_tree.size() == 0){
        return nullptr;
    }
    //data_for_every_tree代表构建每颗子树需要用到的数据，k表示当前是第k列数据
    std::cout << k << "th cycle" << std::endl;
    //std::vector<double> judge;
    double g = 0;
    int n = data_for_every_tree.size();
    int m = data_for_every_tree[0].size();
    std::cout << "data_for_every_tree大小" << data_for_every_tree.size() << std::endl;
    for(int i = 0; i < n; i ++){
        //judge.push_back(data_for_every_tree[i][row - 1]);
        g = g + data_for_every_tree[i][m - 1];
    }
    std::cout << "g = " << g << std::endl;
    if(k + 1 == m){
        if(g > n - g){
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
    if(abs(g - n) < double_distance){ // return yes
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
    std::vector<double> mid = MiddleFollow(head->leftnode);
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

void print(std::vector<std::vector<double>>& data){
    int n = data.size();
    int m = data[0].size();
    for(int i = 0; i < n; i ++){
        for(int j = 0; j < m; j ++)
            std::cout << data[i][j] << " ";
        std::cout << std::endl;
    }
}

std::string train_data_road = "data/train_data.csv";

int main(){
    std::vector<std::vector<double>> data =  Data2Vector(train_data_road);
    //print(data);
    Cart cart(data);
    std::cout << "compelete inital" << std::endl;
    cart.train_model();
    std::cout << "compelete train_model" << std::endl;
    cart.save_model();
    std::cout << "complete save_model" << std::endl;
    return 0;
}
