//
// Created by ckjsuperhh6602 on 24-12-22.
//

#ifndef DATABASE_1_H
#define DATABASE_1_H
#include <cstring>
#include <fstream>
#include <map>
#include <string>
#include <utility>
#include <vector>
#include <set>
#include <unordered_map>

using std::string;
using std::fstream;
using std::ifstream;
using std::ofstream;
using std::vector;
using std::pair;
using std::set;

inline int n;
struct name_and_password;
constexpr int MAXN = 74;

template<class T>
struct TreeNode {
    T reference[MAXN];
    int pointer_son[MAXN + 1]{};
    int pointer_father;

    TreeNode(): pointer_father(-1) {
        memset(pointer_son, -1, sizeof(pointer_son));
    }
};

template<class T, int info_len = 4>
class MemoryRiver {
private:
    fstream file;
    string file_name;
    int sizeofT = sizeof(T);

public:
    MemoryRiver() = default;

    explicit MemoryRiver(string file_name) : file_name(std::move(file_name)) {
    }

    ~MemoryRiver() { file.close(); }

    void initialise(const string &FN = "") {
        if (!FN.empty()) file_name = FN;
        ifstream check_file;
        check_file.open(file_name, std::ios::in | std::ios::out);
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
        file.seekg(static_cast<int>((n - 1) * sizeof(int)));
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
        file.seekp(static_cast<int>(sizeof(int) * (n - 1)));
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
        write_info(index + sizeofT, 1);
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
struct Leaf {
    vector<pair<T, bool> > value;
    int maxsize = 4;
    int current_size = 0;

    Leaf() = default;

    Leaf(T t, bool boolean) {
        value.resize(4);
        value[current_size++] = std::make_pair(t, boolean);
    }
};

template<class T, int info_len = 2>
class MemoryRiver_Leaf {
    //第一个数字为index:现在的最后一个Leaf类存到了哪个位置(取尾位置)
private:
    fstream file;
    string file_name;

public:
    MemoryRiver_Leaf() = default;

    explicit MemoryRiver_Leaf(string file_name) : file_name(std::move(file_name)) {
    }

    ~MemoryRiver_Leaf() { file.close(); }

    void initialise(const string &FN = "") {
        if (!FN.empty()) file_name = FN;
        ifstream check_file;
        check_file.open(file_name, std::ios::in | std::ios::out);
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
        file.seekg(static_cast<int>((n - 1) * sizeof(int)));
        file.read(reinterpret_cast<char *>(&tmp), sizeof(int));
    }

    int get_info(const int n) {
        int tmp;
        get_info(tmp, n);
        return tmp;
    }

    void write_info(int tmp, int n) {
        if (n > info_len) return;
        file.seekp(static_cast<int>(sizeof(int) * (n - 1)));
        file.write(reinterpret_cast<char *>(&tmp), sizeof(int));
    }

    int write(Leaf<T> &t) {
        int index = get_info(1);
        if (index == 0) {
            index += sizeof(int) * info_len;
        }
        file.seekp(index);
        file.write(reinterpret_cast<char *>(&t.current_size), sizeof(int));
        file.write(reinterpret_cast<char *>(&t.maxsize), sizeof(int));
        file.write(reinterpret_cast<char *>(t.value.data()), t.maxsize * sizeof(pair<T, bool>));
        write_info(index + 2 * sizeof(int) + sizeof(pair<T, bool>) * t.maxsize, 1);
        return index;
    }

    void update(Leaf<T> &t, int index) {
        file.seekp(index);
        file.write(reinterpret_cast<char *>(&t.current_size), sizeof(int));
        file.write(reinterpret_cast<char *>(&t.maxsize), sizeof(int));
        file.write(reinterpret_cast<char *>(t.value.data()), t.value.size() * sizeof(pair<T, bool>));
    }

    Leaf<T> read(const int index) {
        Leaf<T> tmp;
        file.seekg(index);
        file.read(reinterpret_cast<char *>(&tmp.current_size), sizeof(int));
        file.read(reinterpret_cast<char *>(&tmp.maxsize), sizeof(int));
        tmp.value.resize(tmp.maxsize);
        file.read(reinterpret_cast<char *>(tmp.value.data()), tmp.maxsize * sizeof(pair<T, bool>));
        return tmp;
    }
};

template<class T1, class T2>
class BPT {
    MemoryRiver<TreeNode<T1> > node_file;
    MemoryRiver_Leaf<T2> leaf_file;

    static int son_number(const TreeNode<T1> &t) {
        for (int i = 0; i < MAXN; i++) {
            if (t.pointer_son[i] == -1) {
                return i;
            }
        }
        return MAXN;
    }

    void Split(const int now_node, TreeNode<T1> &a) {
        TreeNode<T1> new_node;
        const int new_index = node_file.write(new_node);
        for (int i = 0; i < MAXN / 2; i++) {
            new_node.pointer_son[i] = a.pointer_son[i + MAXN / 2];
            auto tmp = node_file.read(new_node.pointer_son[i]);
            tmp.pointer_father = new_index;
            node_file.update(tmp, new_node.pointer_son[i]);
            a.pointer_son[i + MAXN / 2] = -1;
            strcpy(new_node.reference[i], a.reference[i + MAXN / 2]);
        } //把a作为原本的节点，再开一个新节点，两个节点作为兄弟一起连到父亲上边
        if (a.pointer_father == -1) {
            //这个需要分裂的节点已然是根
            TreeNode<T1> root;
            strcpy(root.reference[0], a.reference[MAXN / 2 - 1]); //比如四个叉树，分裂的节点顶多三个索引，四个节点儿子，把第二个索引拿出来，也就是下标为1
            const int index = node_file.write(root);
            a.pointer_father = new_node.pointer_father = index;
            root.pointer_son[0] = now_node;
            root.pointer_son[1] = new_index;
            node_file.update(root, index);
            node_file.update(a, now_node);
            node_file.update(new_node, new_index);
            node_file.write_info(index, 3);
            node_file.write_info(node_file.get_info(2) + 1, 2);
            return;
        }
        auto father = node_file.read(a.pointer_father);
        new_node.pointer_father = a.pointer_father;
        node_file.update(a, now_node);
        node_file.update(new_node, new_index);
        const int num_son = son_number(father);
        for (int i = 0; i < num_son; i++) {
            if (father.pointer_son[i] == now_node) {
                //塞在i+1的位置！
                for (int j = num_son - 1; j >= i + 1; --j) {
                    father.pointer_son[j + 1] = father.pointer_son[j]; //in case of RE,I set my array size to MAXN+1
                    strcpy(father.reference[j], father.reference[j - 1]);
                }
                father.pointer_son[i + 1] = new_index;
                strcpy(father.reference[i], a.reference[MAXN / 2 - 1]);
                break;
            }
        }
        if (num_son + 1 == MAXN) {
            Split(a.pointer_father, father);
        } else {
            node_file.update(father, a.pointer_father);
        }
    }

    int search_to_leaf(T1 index) {
        int depth = node_file.get_info(2), now_node = node_file.get_info(3);
        while (depth > 1) {
            --depth;
            TreeNode<T1> a = node_file.read(now_node);
            const int num = son_number(a);
            for (int i = 0; i < num; ++i) {
                if (i == num - 1 || strcmp(a.reference[i], index) > 0) {
                    now_node = a.pointer_son[i];
                    break;
                }
            }
        }
        return now_node;
    }

public:
    BPT() = default;

    void initialise(const string &s1, const string &s2) {
        node_file.initialise(s1);
        leaf_file.initialise(s2);
    }

    //第一位存index，第二位存depth，第三位root
    void insert(T1 index, const T2 value) {
        if (const int depth = node_file.get_info(2); !depth) {
            TreeNode<T1> root;
            Leaf<T2> a(value, true);
            root.pointer_son[0] = leaf_file.write(a);
            strcpy(root.reference[0], index);
            node_file.write_info(node_file.write(root), 3); //write the address of the root into position 3
            node_file.write_info(1, 2); //change depth into 1
            return;
        }
        const int now_node = search_to_leaf(index);
        TreeNode<T1> a = node_file.read(now_node);
        const int num = son_number(a);
        int pos = 0;
        for (int i = 0; i <= num; ++i) {
            const int state = i < num ? strcmp(a.reference[i], index) : 1;
            if (!state) {
                if (Leaf<T2> tmp = leaf_file.read(a.pointer_son[i]); tmp.current_size + 1 == tmp.maxsize) {
                    tmp.maxsize <<= 1;
                    tmp.value.resize(tmp.maxsize);
                    tmp.value[tmp.current_size++] = {value, true};
                    a.pointer_son[i] = leaf_file.write(tmp);
                    node_file.update(a, now_node);
                } else {
                    tmp.value[tmp.current_size++] = {value, true};
                    leaf_file.update(tmp, a.pointer_son[i]);
                }
                return;
            } // 原本就有
            if (state > 0) {
                pos = i;
                break;
            } // 原本没有
        }
        for (int i = num - 1; i >= pos; --i) {
            strcpy(a.reference[i + 1], a.reference[i]);
            a.pointer_son[i + 1] = a.pointer_son[i];
        }
        Leaf<T2> new_leaf(value, true);
        strcpy(a.reference[pos], index);
        a.pointer_son[pos] = leaf_file.write(new_leaf);
        node_file.update(a, now_node);
        if (num != MAXN - 1) {
            return;
        }
        TreeNode<T1> new_node;
        for (int i = 0; i < MAXN / 2; i++) {
            new_node.pointer_son[i] = a.pointer_son[i + MAXN / 2];
            a.pointer_son[i + MAXN / 2] = -1;
            strcpy(new_node.reference[i], a.reference[i + MAXN / 2]);
        }
        node_file.update(a, now_node);
        if (a.pointer_father == -1) {
            TreeNode<T1> root;
            strcpy(root.reference[0], new_node.reference[0]);
            const int root_node = node_file.write(root);
            a.pointer_father = new_node.pointer_father = root_node;
            root.pointer_son[0] = now_node;
            root.pointer_son[1] = node_file.write(new_node);
            node_file.update(a, now_node);
            node_file.update(root, root_node);
            node_file.write_info(root_node, 3);
            node_file.write_info(2, 2);
            return;
        }
        auto father = node_file.read(a.pointer_father);
        new_node.pointer_father = a.pointer_father;
        const int num_son = son_number(father);
        for (int i = 0; i < num_son; i++) {
            if (father.pointer_son[i] == now_node) {
                //塞在i+1的位置！
                for (int j = num_son - 1; j >= i + 1; --j) {
                    father.pointer_son[j + 1] = father.pointer_son[j];
                    strcpy(father.reference[j], father.reference[j - 1]);
                }
                father.pointer_son[i + 1] = node_file.write(new_node);
                strcpy(father.reference[i], new_node.reference[0]);
                break;
            }
        }
        if (num_son + 1 == MAXN) {
            Split(a.pointer_father, father);
        } else {
            node_file.update(father, a.pointer_father);
        }
    }

    vector<T2> search(T1 index) {
        if (const int depth = node_file.get_info(2); !depth)
            return {};
        const int now_node = search_to_leaf(index);
        const auto a = node_file.read(now_node);
        const int num = son_number(a);
        for (int i = 0; i < num; ++i) {
            const int state = strcmp(a.reference[i], index);
            if (!state) {
                Leaf<T2> tmp = leaf_file.read(a.pointer_son[i]);
                set<T2> S;
                for (int j = 0; j < tmp.current_size; j++) {
                    if (const auto [fi,se] = tmp.value[j]; se) {
                        S.insert(fi);
                    } else {
                        S.erase(fi);
                    }
                }
                vector<T2> res;
                for (const auto t: S)
                    res.push_back(t);
                if (res.size() != tmp.value.size()) //更新value
                {
                    tmp.current_size = static_cast<int>(res.size());
                    for (int j = 0; j < static_cast<int>(res.size()); ++j) {
                        tmp.value[j] = std::make_pair(res[j], true);
                    }
                }
                return res;
            }
            if (state > 0)
                return {};
        }
        return {};
    }

    void Val_delete(T1 index, const T2 value) {
        if (const int depth = node_file.get_info(2); !depth)
            return;
        const int now_node = search_to_leaf(index);
        auto a = node_file.read(now_node);
        const int num = son_number(a);
        for (int i = 0; i <= num; ++i) {
            const int state = i < num ? strcmp(a.reference[i], index) : 1;
            if (!state) {
                if (Leaf<T2> tmp = leaf_file.read(a.pointer_son[i]); tmp.current_size + 1 == tmp.maxsize) {
                    tmp.maxsize <<= 1;
                    tmp.value.resize(tmp.maxsize);
                    tmp.value[tmp.current_size++] = {value, false};
                    a.pointer_son[i] = leaf_file.write(tmp);
                    node_file.update(a, now_node);
                } else {
                    tmp.value[tmp.current_size++] = {value, false};
                    leaf_file.update(tmp, a.pointer_son[i]);
                }
                return;
            } // 原本就有
            if (state > 0) {
                return;
            } // 原本没有
        }
    }
};

using std::vector;
using std::map;
using std::unordered_map;

template<class T>
class char_more {
private:
    char a[61]{};
    int the_size;

public:
    explicit char_more(const string &str): the_size(sizeof(T) / sizeof(char)) {
        if (str.size() > the_size) {
            throw std::runtime_error("");
        }
        for (int i = 0; i < str.size(); i++) {
            a[i] = str[i];
        }
    }

    char_more(): the_size(sizeof(T) / sizeof(char)) {
    }

    string get_char() {
        T tmp;
        strncpy(tmp, a, sizeof(T) / sizeof(char));
        return string(tmp);
    }

    [[nodiscard]] int get_size() const {
        return the_size;
    }
};

struct name_and_password {
    char password[31]{};
    char username[31]{};
    int privilege{};

    name_and_password() = default;

    name_and_password(char password[31], char username[31], int privilege): privilege(privilege) {
        strcpy(this->password, password);
        strcpy(this->username, username);
    }
};

struct book_info {
    char ISBN[21] = "";
    char BookName[61] = "";
    char Author[61] = "";
    char KeyWord[61] = "";
    char Price[14] = "0";
    int Storage = 0;

    book_info() = default;

    explicit book_info(const char ISBN[21]) {
        strcpy(this->ISBN, ISBN);
    }
};

struct finance_info {
    double income;
    double outcome;

    finance_info(const double &income, const double &outcome): income(income), outcome(outcome) {
    }
};

struct stack_id {
    char id[31]{};
    int privilege;
    int num = -1;

    stack_id(char a[31], const int &privilege): privilege(privilege) {
        strcpy(id, a);
    }

    void change_book(const int num) {
        this->num = num;
    }
};

inline BPT<char[31], int> user_file; //利用存储编号，作为一个BPT
inline vector<name_and_password> user_storage; // 利用编号存储密码，用户名，权限等信息
inline unordered_map<string, int> login_map; //记录某个id登录了几次
inline vector<stack_id> login_status; //用一个vector按顺序维护id、选取图书的编号和privilege的栈(没有选取图书则是-1)
inline BPT<char[21], int> ISBN_reference; //维护ISBN到对应位置/编号的BPT
inline vector<book_info> ISBN; //维护编号到ISBN的对应
inline BPT<char[61], int> KeyWord_reference;
inline BPT<char[61], int> BookName_reference;
inline BPT<char[61], int> Author_reference;
inline vector<finance_info> finance_list; //存下每一笔收入支出

#endif //DATABASE_1_H
