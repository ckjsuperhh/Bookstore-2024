#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <utility>
#include <set>

using std::string;
using std::fstream;
using std::ifstream;
using std::ofstream;
using std::vector;
using std::pair;
using std::set;

constexpr int MAXN=2;

template<class T, int info_len = 4>
class MemoryRiver {
private:
    fstream file;
    string file_name;
    int sizeofT = sizeof(T);

public:
    MemoryRiver() = default;

    MemoryRiver(string file_name) : file_name(std::move(file_name)) {
    }

    ~MemoryRiver() { file.close(); }

    void initialise(const string &FN = "") {
        if (!FN.empty()) file_name = FN;
        ifstream check_file;
        check_file.open(file_name,std::ios::in|std::ios::out);
        if (!check_file.good()) {
            file.open(file_name, std::ios::out);
            int tmp = 0;
            for (int i = 0; i < info_len; ++i)
                file.write(reinterpret_cast<char *>(&tmp), sizeof(int));
            file.close();
        }
        check_file.close();
        file.open(file_name, std::ios::in | std::ios::out);
    }

    //读出第n个int的值赋给tmp，1_base
    void get_info(int &tmp, const int n) {
        if (n > info_len) return;
        file.seekg((n - 1) * sizeof(int));
        file.read(reinterpret_cast<char *>(&tmp), sizeof(int));
    }

    int get_info(const int n) {
        int tmp;
        get_info(tmp, n);
        return tmp;
    }

    //将tmp写入第n个int的位置，1_base
    void write_info(int tmp, int n) {
        if (n > info_len) return;
        file.seekp(sizeof(int) * (n - 1));
        file.write(reinterpret_cast<char *>(&tmp), sizeof(int));
    }

    //在文件合适位置写入类对象t，并返回写入的位置索引index
    //位置索引意味着当输入正确的位置索引index，在以下三个函数中都能顺利的找到目标对象进行操作
    //返回的位置索引index可以取为对象写入的起始位置
    //过程中会将index+sizeofT写入info(1)
    int write(T &t) {
        int tmp;
        get_info(tmp, 1);
        int index;
        if (tmp == 0) { index = sizeof(int) * info_len; } else { index = tmp; }
        file.seekp(index);
        file.write(reinterpret_cast<char *>(&t), sizeofT);
        write_info(index+sizeofT, 1);
        return index;
    }


    //用t的值更新位置索引index对应的对象，保证调用的index都是由write函数产生
    void update(T &t, const int index) {
        file.seekp(index);
        file.write(reinterpret_cast<char *>(&t), sizeofT);
    }

    //读出位置索引index对应的T对象的值并赋值给t，保证调用的index都是由write函数产生
    void read(T &t, const int index) {
        file.seekg(index);
        file.read(reinterpret_cast<char *>(&t), sizeofT);
    }

    T read(const int index) {
        T tmp;
        read(tmp, index);
        return tmp;
    }
};

template<class T>
struct TreeNode {
    T reference[MAXN];
    int pointer_son[MAXN+1];
    int pointer_father;

    TreeNode(): pointer_father(-1) {
        memset(pointer_son, -1, sizeof(pointer_son));
    }
};

struct Leaf {
    vector<pair<int, bool> > value;
    int maxsize{};
    int current_size{};

    Leaf()=default;

    Leaf(int t, bool boolean): maxsize(4){
        value.resize(4);
        value[current_size++]=std::make_pair(t, boolean);
    }

    void insert(int t, bool boolean) {
        if (maxsize == current_size) {
            maxsize <<= 1;
            value.resize(maxsize);
        }
        value[current_size++]=std::make_pair(t,boolean);
    }
};

template<class T, int info_len = 2>
class MemoryRiver_Leaf {
    //第一个数字为index:现在的最后一个Leaf类存到了哪个位置(取尾位置)
private:
    fstream file;
    string file_name;
    int sizeofT = sizeof(T);

public:
    MemoryRiver_Leaf() = default;

    MemoryRiver_Leaf(string file_name) : file_name(std::move(file_name)) {
    }

    ~MemoryRiver_Leaf() { file.close(); }

    void initialise(const string &FN = "") {
        if (!FN.empty()) file_name = FN;
        ifstream check_file;
        check_file.open(file_name,std::ios::in|std::ios::out);
        if (!check_file.good()) {
            file.open(file_name, std::ios::out);
            int tmp = 0;
            for (int i = 0; i < info_len; ++i)
                file.write(reinterpret_cast<char *>(&tmp), sizeof(int));
            file.close();
        }
        check_file.close();
        file.open(file_name, std::ios::in | std::ios::out);
    }

    void get_info(int &tmp, const int n) {
        if (n > info_len) return;
        file.seekg((n - 1) * sizeof(int));
        file.read(reinterpret_cast<char *>(&tmp), sizeof(int));
    }

    int get_info(const int n) {
        int tmp;
        get_info(tmp, n);
        return tmp;
    }

    void write_info(int tmp, int n) {
        if (n > info_len) return;
        file.seekp(sizeof(int) * (n - 1));
        file.write(reinterpret_cast<char *>(&tmp), sizeof(int));
    }

    int write(Leaf &t) {
        int index = get_info(1);
        if (index == 0) {
            index += sizeof(int) * info_len;
        }
        file.seekp(index);
        file.write(reinterpret_cast<char *>(&t.current_size), sizeof(int));
        file.write(reinterpret_cast<char *>(&t.maxsize), sizeof(int));
        file.write(reinterpret_cast<char *>(t.value.data()), t.maxsize * sizeof(std::pair<int, bool>));
        write_info(index + 2 * sizeof(int) + sizeof(pair<int, bool>) * t.maxsize, 1);
        return index;
    }

    void update(Leaf &t, int index) {
        file.seekp(index);
        file.write(reinterpret_cast<char *>(&t.current_size), sizeof(int));
        file.write(reinterpret_cast<char *>(&t.maxsize), sizeof(int));
        file.write(reinterpret_cast<char *>(t.value.data()), t.value.size() * sizeof(pair<int, bool>));
    }
    

    Leaf read(int index) {
        Leaf tmp;
        file.seekg(index);
        file.read(reinterpret_cast<char *>(&tmp.current_size), sizeof(int));
        file.read(reinterpret_cast<char *>(&tmp.maxsize), sizeof(int));
        tmp.value.resize(tmp.maxsize);
        file.read(reinterpret_cast<char *>(tmp.value.data()), tmp.maxsize*sizeof(pair<int, bool>));
        return tmp;
    }

    int read_current_size(int index) {
        int tmp;
        file.seekg(index);
        file.read(reinterpret_cast<char *>(&tmp), sizeof(int));
        return tmp;
    }

    int read_maxsize(int index) {
        int tmp;
        file.seekg(index + sizeof(int));
        file.read(reinterpret_cast<char *>(&tmp), sizeof(int));
        return tmp;
    }
    

    vector<pair<int, bool> > read_vector(int index) {
        int maxn = read_maxsize(index);
        vector<pair<int, bool> > tmp(maxn);
        file.seekg(index + 2 * sizeof(int));
        file.read(reinterpret_cast<char *>(&tmp), sizeof(pair<int, bool>) * maxn);
        return tmp;
    }

};

class BPT {
    MemoryRiver<TreeNode<char[65]> > node_file;
    MemoryRiver_Leaf<Leaf> leaf_file;

    static int son_number(const TreeNode<char[65]> &t) {
        for (int i = 0; i < MAXN; i++) {
            if (t.pointer_son[i] == -1) {
                return i;
            }
        }
        return MAXN;
    }

    void Split(const TreeNode<char[65]> &a) {
        TreeNode<char[65]> new1,new2;
        for (int i=0;i<MAXN/2;i++) {
            new1.pointer_son[i]=a.pointer_son[i];
            strcpy(new1.reference[i],a.reference[i]);
            strcpy(new2.reference[i],a.reference[i+MAXN/2]);
        }
        for (int i=0;i<=MAXN/2;i++) {
            new2.pointer_son[i]=a.pointer_son[i+MAXN/2];
        }
        if (a.pointer_father==-1) {
            TreeNode<char[65]> root;
            strcpy(root.reference[0],a.reference[MAXN/2-1]);
            int index=node_file.write(root);
            new1.pointer_father=new2.pointer_father=index;
            int index1=node_file.write(new1);
            int index2=node_file.write(new2);
            for (int i=0;i<MAXN/2;i++) {
                auto son=node_file.read(new1.pointer_son[i]);
                son.pointer_father=index1;
                node_file.update(son,new1.pointer_son[i]);
            }
            for (int i=0;i<=MAXN/2;i++) {
                auto son=node_file.read(new2.pointer_son[i]);
                son.pointer_father=index2;
                node_file.update(son,new2.pointer_son[i]);
            }
            root.pointer_son[0]=index1;
            root.pointer_son[1]=index2;
            node_file.update(root,index);
            node_file.write_info(index,3);
            node_file.write_info(node_file.get_info(2)+1,2);
            return;
        }
        auto father=node_file.read(a.pointer_father);
        new1.pointer_father=new2.pointer_father=a.pointer_father;
        const int index1=node_file.write(new1);
        const int index2=node_file.write(new2);
        for (int i=0;i<MAXN/2;i++) {
            auto son=node_file.read(new1.pointer_son[i]);
            son.pointer_father=index1;
            node_file.update(son,new1.pointer_son[i]);
        }
        for (int i=0;i<=MAXN/2;i++) {
            auto son=node_file.read(new2.pointer_son[i]);
            son.pointer_father=index2;
            node_file.update(son,new2.pointer_son[i]);
        }
        const int num_son=son_number(father);
        for (int i=0;i<num_son;i++) {
            if (strcmp(father.reference[i],a.reference[MAXN/2-1])>=0) {
                for (int j=num_son-1;j>=i;j--) {
                    father.pointer_son[j+1]=father.pointer_son[j];
                    strcpy(father.reference[j],father.reference[j-1]);
                }
                father.pointer_son[i]=index1;
                father.pointer_son[i+1]=index2;
                strcpy(father.reference[i],a.reference[MAXN/2-1]);
                node_file.update(father,a.pointer_father);
                break;
            }
        }
        if (num_son==MAXN) {
            Split(father);
        }
    }

    int search_to_leaf(char index[65]) {
        int depth = node_file.get_info(2),now_node = node_file.get_info(3);
        while (depth>1) {
            --depth;
            TreeNode<char[65]> a = node_file.read(now_node);
            const int num = son_number(a);
            for (int i = 0; i < num ; ++i) {
                if (i== num - 1 || strcmp(a.reference[i], index) >= 0) {
                    now_node=a.pointer_son[i];
                    break;
                }
            }
        }
        return now_node;
    }

public:
    BPT() = default;

    void initialise(const char s1[65], const string &s2) {
        node_file.initialise(s1);
        leaf_file.initialise(s2);
    }

    //第一位存index，第二位存depth，第三位root
    void insert(char index[65], int value) {
        int depth = node_file.get_info(2);
        if (!depth) {
            TreeNode<char[65]> root;
            Leaf a(value, true);
            root.pointer_son[0] = leaf_file.write(a);
            strcpy(root.reference[0], index);
            node_file.write_info(node_file.write(root), 3); //write the address of the root into position 3
            node_file.write_info(1, 2); //change depth into 1
            return ;
        }
        const int now_node = search_to_leaf(index);
        TreeNode<char[65]> a = node_file.read(now_node);
        const int num = son_number(a);
        int pos = 0;
        for (int i = 0; i <= num ; ++i) {
            const int state = i < num ? strcmp(a.reference[i], index) : 1;
            if (!state) {
                Leaf tmp = leaf_file.read(a.pointer_son[i]);
                tmp.insert(value, true);
                leaf_file.update(tmp, a.pointer_son[i]);
                return ;
            }// 原本就有
            if (state > 0) {
                pos = i;
                break;
            }// 原本没有
        }
        for (int i = num-1; i >= pos; --i) {
            strcpy(a.reference[i+1], a.reference[i]);
            a.pointer_son[i+1] = a.pointer_son[i];
        }
        Leaf new_leaf(value,true);
        strcpy(a.reference[pos], index);
        a.pointer_son[pos]=leaf_file.write(new_leaf);
        node_file.update(a, now_node);
        if (num != MAXN - 1) {
            return ;
        }
        TreeNode<char[65]> new1,new2;
        for (int i=0;i<MAXN/2;i++) {
            new1.pointer_son[i]=a.pointer_son[i];
            strcpy(new1.reference[i],a.reference[i]);
            new2.pointer_son[i]=a.pointer_son[i+MAXN/2];
            strcpy(new2.reference[i],a.reference[i+MAXN/2]);
        }
        if (a.pointer_father==-1) {
            TreeNode<char[65]> root;
            strcpy(root.reference[0],a.reference[MAXN/2-1]);
            int index=node_file.write(root);
            new1.pointer_father=new2.pointer_father=index;
            int index1=node_file.write(new1);
            int index2=node_file.write(new2);
            root.pointer_son[0]=index1;
            root.pointer_son[1]=index2;
            node_file.update(root,index);
            node_file.write_info(index,3);
            node_file.write_info(2,2);
            return;
        }
        auto father=node_file.read(a.pointer_father);
        new1.pointer_father=new2.pointer_father=a.pointer_father;
        const int index1=node_file.write(new1);
        const int index2=node_file.write(new2);
        const int num_son=son_number(father);
        //std::cerr<<"num_son="<<num_son<<std::endl;
        for (int i=0;i<num_son;i++) {
            if (strcmp(father.reference[i],a.reference[MAXN/2-1])>=0) {
                for (int j=num_son-1;j>=i;j--) {
                    father.pointer_son[j+1]=father.pointer_son[j];
                    strcpy(father.reference[j],father.reference[j-1]);
                }
                father.pointer_son[i]=index1;
                father.pointer_son[i+1]=index2;
                strcpy(father.reference[i],a.reference[MAXN/2-1]);
                node_file.update(father,a.pointer_father);
                break;
            }
        }
        if (num_son==MAXN) {
            Split(father);
        }
        //裂叶
        //to be written
    }

    vector<int> search(char index[65]) {
        int depth = node_file.get_info(2);
        if (!depth)
            return {};
        const int now_node = search_to_leaf(index);
        auto a = node_file.read(now_node);
        int num = son_number(a);
        //std::cerr<<num<<" ???\n";
        for (int i = 0; i < num ; ++i) {
            const int state = strcmp(a.reference[i], index);
            if (!state) {
                Leaf tmp = leaf_file.read(a.pointer_son[i]);
                set<int> S;
                for (int j = 0 ;j < tmp.current_size ;j++) {
                    if (const auto [fi,se] = tmp.value[j]; se) {
                        S.insert(fi);
                    } else {
                        S.erase(fi);
                    }
                }
                vector<int> res;
                for (const auto t:S)
                    res.push_back(t);
                if (res.size() != tmp.value.size())//更新value
                {
                    tmp.current_size = static_cast<int>(res.size());
                    for (int j = 0; j < static_cast<int>(res.size()); ++j ) {
                        tmp.value[j] = std::make_pair(res[j], true);
                    }
                }
                return res;
            }
            if (state>0)
                return {};
        }
        return {};
    }

    void Val_delete(char index[65], const int value) {
        if (const int depth = node_file.get_info(2); !depth)
            return ;
        const int now_node = search_to_leaf(index);
        const auto a = node_file.read(now_node);
        const int num=son_number(a);
        for (int i = 0; i <= num ; ++i) {
            const int state = i < num ? strcmp(a.reference[i], index) : 1;
            if (!state) {
                Leaf tmp = leaf_file.read(a.pointer_son[i]);
                tmp.insert(value, false);
                leaf_file.update(tmp, a.pointer_son[i]);
                return ;
            }// 原本就有
            if (state > 0) {
                return;
            }// 原本没有
        }
    }
};

int main() {
    BPT database;
    database.initialise("ljy", "ckj");
    int n, i;
    string str;
    char index[65];
    std::cin >> n;
    for (; n > 0; n--) {
        std::cin >> str;
        //std::cerr << "str=" << str << std::endl;
        if (str == "insert") {
            std::cin >> index;
            std::cin >> i;
            database.insert(index, i);
        } else if (str == "find") {
            std::cin >> index;
            if (auto a = database.search(index); !a.size()) {
                std::cout << "null" << std::endl;
            } else {
                //std::cerr << "size=" << a.size() << std::endl;
                for (auto x: a) {
                    std::cout << x<< " ";
                }
                std::cout << '\n';
            }
        } else if (str == "delete") {
            std::cin >> index;
            std::cin >> i;
            database.Val_delete(index, i);
        }
    }
    return 0;
}
