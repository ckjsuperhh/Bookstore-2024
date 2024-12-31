//
// Created by ckjsuperhh6602 on 24-12-24.
//

#ifndef CMD_H
#define CMD_H
#include"DataBase.h"
#include"log.h"
#include"user.h"

class cmd {
public:
    static void initialise() {
        user_file.initialise("user_file(1)", "user_file(2)");
        ISBN_reference.initialise("IBSN_file(1)", "IBSN_file(2)");
        KeyWord_reference.initialise("KeyWord_file(1)", "KeyWord_reference(2)");
        Author_reference.initialise("Author(1)", "Author(2)");
        BookName_reference.initialise("BookName(1)", "BookName(2)");
        user_file.insert(char_more<char[31]>("root").get_char().data(), 0);
        user_storage.emplace_back(char_more<char[31]>("sjtu").get_char().data(), char_more<char[31]>("root").get_char().data(), 7);
    }

    static void analyse(const string &a) {
        int cnt = 0;
        vector<string> tokens;
        int l = 0;
        while (a[l] == ' ') {
            l++;
        }
        int r = l;
        while (a[r] != ' ' && r != a.size()) {
            r++;
        }
        string info = a.substr(l, r - l);
        l = r;
        while (l != a.size()) {
            while (a[l] == ' '&&l<a.size()) {
                l++;
            }
            if (l==a.size()) {
                break;
            }
            r = l;
            while (a[r] != ' ' && r != a.size()) {
                r++;
            }
            cnt++;
            tokens.emplace_back(a.substr(l, r - l));
            l = r;
        }

        //cout<<info<<std::endl;
        if (info == "su") {
            if (cnt == 2) {
                user_operation::_login(char_more<char[31]>(tokens[0]).get_char().data(), char_more<char[31]>(tokens[1]).get_char().data());
            } else if (cnt == 1) {
                user_operation::_login(char_more<char[31]>(tokens[0]).get_char().data());
            } else {
                throw std::runtime_error("");
            }
        } else if (info == "logout") {
            if (cnt) {
                throw std::runtime_error("");
            }
            user_operation::_logout();
        } else if (info == "register") {
            if (cnt == 3) {
                user_operation::_register(char_more<char[31]>(tokens[0]).get_char().data(), char_more<char[31]>(tokens[1]).get_char().data(), char_more<char[31]>(tokens[2]).get_char().data());
            } else {
                throw std::runtime_error("");
            }
        } else if (info == "passwd") {
            if (cnt == 3) {
                user_operation::_modify(char_more<char[31]>(tokens[0]).get_char().data(), char_more<char[31]>(tokens[1]).get_char().data(), char_more<char[31]>(tokens[2]).get_char().data());
            } else if (cnt == 2) {
                user_operation::_modify(char_more<char[31]>(tokens[0]).get_char().data(), char_more<char[31]>(tokens[1]).get_char().data());
            } else {
                throw std::runtime_error("");
            }
        } else if (info == "useradd") {
            if (cnt == 4 && (tokens[2] == "1" || tokens[2] == "3")) {
                user_operation::_useradd(char_more<char[31]>(tokens[0]).get_char().data(), char_more<char[31]>(tokens[1]).get_char().data(), std::stoi(tokens[2]), char_more<char[31]>(tokens[3]).get_char().data());
            } else {
                throw std::runtime_error("");
            }
        } else if (info == "delete") {
            if (cnt == 1) {
                user_operation::_delete(char_more<char[31]>(tokens[0]).get_char().data());
            } else {
                throw std::runtime_error("");
            }
        } else if (info == "show") {
            if (!cnt) {
                book::show();
            } else if (cnt == 1 && tokens[0].contains('=') && tokens[0][0] == '-') {
                int o = 1;
                while (tokens[0][o] != '=') {
                    o++;
                }
                if (const string basic_string = tokens[0].substr(1, o - 1); basic_string == "ISBN") {
                    if (o + 1 == tokens[0].size()) {
                        throw std::runtime_error("");
                    }
                    book::ISBN_show(char_more<char[21]>(tokens[0].substr(o + 1)).get_char().data());
                } else if ((basic_string == "name" || basic_string == "author" || basic_string == "keyword") && tokens[0][o + 1] == '\"' && tokens[0].back() == '\"') {
                    if (o + 2 == tokens[0].size()) {
                        throw std::runtime_error("");
                    }
                    book::other_show(char_more<char[61]>(tokens[0].substr(o + 2, tokens[0].size() - o - 3)).get_char().data(), basic_string);
                } else {
                    throw std::runtime_error("");
                }
            } else if (cnt == 2 && tokens[0] == "finance") {
                log::show(std::stoi(tokens[1]));
            } else if (cnt == 1 && tokens[0] == "finance") {
                log::show();
            } else {
                throw std::runtime_error("");
            }
        } else if (info == "buy") {
            if (cnt == 2) {
                book::buy(char_more<char[21]>(tokens[0]).get_char().data(), std::stoi(tokens[1]));
            } else {
                throw std::runtime_error("");
            }
        } else if (info == "select") {
            if (cnt == 1) {
                book::select(char_more<char[21]>(tokens[0]).get_char().data());
            } else {
                throw std::runtime_error("");
            }
        } else if (info == "modify") {
            if (login_status.empty()) {
                throw std::runtime_error("");
            }
            if (login_status.back().privilege < 3) {
                throw std::runtime_error("");
            }
            if (login_status.back().id == "" || login_status.back().num == -1) {
                throw std::runtime_error("");
            }
            vector<string> types;
            for (const auto &x: tokens) {
                if (x.contains('=') && x[0] == '-') {
                    int o = 1;
                    while (x[o] != '=') {
                        o++;
                    }
                    types.push_back(x.substr(1, o - 1));
                } else {
                    throw std::runtime_error("");
                }
            }
            std::ranges::sort(types);
            if (std::ranges::unique(types).begin() - types.begin() != types.size()) {
                throw std::runtime_error("");
            }
            std::ranges::sort(tokens);
            vector<string> infos;
            for (int i = 0; i < types.size(); i++) {
                if (types[i] == "ISBN" && tokens[i].size() > 6) {
                    // cout<<ISBN[login_status.back().num].ISBN<<std::endl;
                    // cout<<char_more<char[21]>(tokens[i].substr(6)).get_char().data()<<std::endl;
                    if (strcmp(ISBN[login_status.back().num].ISBN, char_more<char[21]>(tokens[i].substr(6)).get_char().data()) == 0) {
                        throw std::runtime_error("");
                    }
                    if (!ISBN_reference.search(char_more<char[21]>(tokens[i].substr(6)).get_char().data()).empty()) {
                        throw std::runtime_error("");
                    }
                    infos.push_back(tokens[i].substr(5));
                } else if (types[i] == "name" && tokens[i][6] == '\"' && tokens[i].back() == '\"' && tokens[i].size() > 8) {
                    infos.push_back(tokens[i].substr(7, tokens[i].size() - 8));
                } else if (types[i] == "author" && tokens[i][8] == '\"' && tokens[i].back() == '\"' && tokens[i].size() > 10) {
                    infos.push_back(tokens[i].substr(9, tokens[i].size() - 10));
                } else if (types[i] == "keyword" && tokens[i][9] == '\"' && tokens[i].back() == '\"' && tokens[i].size() > 11) {
                    vector<string> keywords = book::find_KeyWord(tokens[i].substr(10, tokens[i].size() - 11));
                    infos.push_back(tokens[i].substr(10, tokens[i].size() - 11));
                } else if (types[i] == "price" && tokens[i].size() > 7) {
                    infos.push_back(tokens[i].substr(7));
                } else {
                    throw std::runtime_error("");
                }
            }
            for (int i = 0; i < types.size(); i++) {
                if (types[i] == "ISBN") {
                    book::modify_ISBN(char_more<char[21]>(tokens[i].substr(6)).get_char().data());
                } else if (types[i] == "name") {
                    book::modify_other("name", char_more<char[61]>(infos[i]).get_char().data());
                } else if (types[i] == "author") {
                    book::modify_other("author", char_more<char[61]>(infos[i]).get_char().data());
                } else if (types[i] == "keyword") {
                    book::modify_other("keyword", char_more<char[61]>(infos[i]).get_char().data());
                } else if (types[i] == "price") {
                    book::modify_other("price",char_more<char[61]>(infos[i]).get_char().data());
                }
            }
        } else if (info == "import") {
            if (cnt == 2) {
                book::import(std::stoi(tokens[0]), std::stod(tokens[1]));
            }
        } else if (info == "quit" || info == "exit") {
            exit(0);
        } else if (!info.empty()){
            throw std::runtime_error("");
        }
    }
};


#endif //CMD_H
