#ifndef USER_H
#define USER_H

#include"DataBase.h"
#include"book.h"
#include<unordered_map>
#include<vector>

using std::vector;
using std::unordered_map;

class user_operation {
public:
    static void _login(char user_id[31], const string &password) {
        auto user_info = user_file.search(user_id);
        if (user_info.empty()) {
            throw std::runtime_error("");
        }
        // cout<<user_storage[user_info[0]].password<<std::endl;
        if (strcmp(user_storage[user_info[0]].password, password.data()) != 0) {
            throw std::runtime_error("");
        }
        login_map[user_id]++;

        login_status.emplace_back(char_more<char[31]>(user_id).get_char().data(), user_storage[user_info[0]].privilege);
    }

    static void _login(char user_id[31]) {
        auto user_info = user_file.search(user_id);
        if (user_info.empty()) {
            throw std::runtime_error("");
        }
        if (login_status.empty()) {
            throw std::runtime_error("");
        }
        if (user_storage[user_info[0]].privilege >= login_status.back().privilege) {
            throw std::runtime_error("");
        } //判断目前账号权限是否高于目标
        login_map[user_id]++;
        login_status.emplace_back(char_more<char[31]>(user_id).get_char().data(), user_storage[user_info[0]].privilege);
    }

    static void _logout() {
        if (login_status.empty()) {
            throw std::runtime_error("");
        }
        login_map[login_status.back().id]--;
        login_status.pop_back();
    }

    static void _register(char user_id[31], char password[31], char username[31]) {
        if (!user_file.search(user_id).empty()) {
            throw std::runtime_error("");
        }
        user_file.insert(user_id, static_cast<int>(user_storage.size()));
        user_storage.emplace_back(password, username, 1);
    }

    static void _modify(char user_id[31], const string & current_password, char new_password[31]) {
        const auto user_info = user_file.search(user_id);
        if (user_info.empty()) {
            throw std::runtime_error("");
        }
        // std::cout<<user_storage[user_info[0]].password<<std::endl;
        // std::cout<<current_password<<std::endl;
        if (strcmp(user_storage[user_info[0]].password, current_password.data())!=0) {
            throw std::runtime_error("");
        }
        if (login_status.empty()) {
            throw std::runtime_error("");
        }
        user_file.Val_delete(user_id, user_info[0]);
        user_file.insert(user_id, user_info[0]);
        strcpy(user_storage[user_info[0]].password, new_password);
    }

    static void _modify(char user_id[31], char new_password[31]) {
        auto user_info = user_file.search(user_id);
        if (user_info.empty()) {
            throw std::runtime_error("");
        }
        if (login_status.back().privilege != 7) {
            throw std::runtime_error("");
        }
        user_file.Val_delete(user_id, user_info[0]);
        user_file.insert(user_id, user_info[0]);
        strcpy(user_storage[user_info[0]].password, new_password);
    }

    static void _useradd(char user_id[31], char password[31], int privilege, char username[31]) {
        auto user_info = user_file.search(user_id);
        if (!user_info.empty()) {
            throw std::runtime_error("");
        }
        // cout<<login_status.back().privilege<<std::endl;
        if (!(login_status.back().privilege >= 3 && login_status.back().privilege > privilege)) {
            throw std::runtime_error("");
        }
        user_file.insert(user_id, static_cast<int>(user_storage.size()));
        user_storage.emplace_back(password, username, privilege);
    }

    static void _delete(char user_id[31]) {
        if (login_map[string(user_id)]) {
            throw std::runtime_error("");
        }
        if (login_status.back().privilege != 7) {
            throw std::runtime_error("");
        }
        auto user_info = user_file.search(user_id);
        if (user_info.empty()) {
            throw std::runtime_error("");
        }
        user_file.Val_delete(user_id, user_info[0]);
    }
};
#endif //USER_H
