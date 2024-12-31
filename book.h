//
// Created by ckjsuperhh6602 on 24-12-23.
//

#ifndef BOOK_H
#define BOOK_H
#include <algorithm>

#include"DataBase.h"
#include<vector>
#include<map>
#include<iostream>

using std::vector;
using std::map;
using std::cout;

class book {
public:
    static vector<string> find_KeyWord(const string &a) {
        if(a[0]=='|'||a.back()=='|'){
            throw std::runtime_error("");
        }
        vector<string> tmp;
        int l = 0;
        for (int o = 0; o < a.size(); o++) {
            if (a[o] == '|' || o == a.size() - 1) {
                if(o==a.size() - 1){
                    tmp.push_back(a.substr(l, o - l +1 ));
                }else{
                    tmp.push_back(a.substr(l, o - l ));
                }
                l = o+1;
            }
        }
        std::ranges::sort(tmp);
        for(const auto& x:tmp){
            if(x.empty()){
                throw std::runtime_error("");
            }
        }
        if (std::ranges::unique(tmp).begin() - tmp.begin() != tmp.size()) {
            throw std::runtime_error("");
        }
        return tmp;
    }

    static void ISBN_show(char ISBN_[21]) {
        if (login_status.empty()) {
            throw std::runtime_error("");
        }
        auto ISBN_vector = ISBN_reference.search(ISBN_);
        if (ISBN_vector.empty()) {
            cout << '\n';
            return;
        }
        vector<book_info> tmp;
        for (auto x: ISBN_vector) {
            tmp.push_back(ISBN[x]);
        }
        std::ranges::sort(tmp, [](const book_info &a1, const book_info &a2)-> bool { return strcmp(a1.ISBN, a2.ISBN) > 0; });
        for (const auto x: tmp) {
            cout << x.ISBN << "\t" << x.BookName << "\t" << x.Author << "\t" << x.KeyWord << "\t" <<std::fixed<<std::setprecision(2)<< std::stod(x.Price) << "\t" << x.Storage << "\n";
        }
    }

    static void show() {
        if (login_status.empty()) {
            throw std::runtime_error("");
        }
        if (ISBN.empty()) {
            cout << '\n';
            return;
        }
        auto tmp = ISBN;
        std::ranges::sort(tmp, [](const book_info &a1, const book_info &a2)-> bool { return strcmp(a1.ISBN, a2.ISBN) < 0; });
        for (auto x: tmp) {
            cout << x.ISBN << "\t" << x.BookName << "\t" << x.Author << "\t" << x.KeyWord << "\t" <<std::fixed<<std::setprecision(2)<< std::stod(x.Price) << "\t" << x.Storage << "\n";
        }
    }

    static void other_show(char information[61], const string &a) {
        if (login_status.empty()) {
            throw std::runtime_error("");
        }
        if (a == "name") {
            auto ISBN_vector = BookName_reference.search(information);
            if (ISBN_vector.empty()) {
                cout << '\n';
                return;
            }
            vector<book_info> tmp;
            for (auto x: ISBN_vector) {
                tmp.push_back(ISBN[x]);
            }
            std::ranges::sort(tmp, [](const book_info &a1, const book_info &a2)-> bool { return strcmp(a1.ISBN, a2.ISBN) < 0; });
            for (const auto x: tmp) {
                cout << x.ISBN << "\t" << x.BookName << "\t" << x.Author << "\t" << x.KeyWord << "\t" <<std::fixed<<std::setprecision(2)<< std::stod(x.Price) << "\t" << x.Storage << "\n";
            }
        } else if (a == "author") {
            auto ISBN_vector = Author_reference.search(information);
            if (ISBN_vector.empty()) {
                cout << '\n';
                return;
            }
            vector<book_info> tmp;
            for (auto x: ISBN_vector) {
                tmp.push_back(ISBN[x]);
            }
            std::ranges::sort(tmp, [](const book_info &a1, const book_info &a2)-> bool { return strcmp(a1.ISBN, a2.ISBN) < 0; });
            for (const auto x: tmp) {
                cout << x.ISBN << "\t" << x.BookName << "\t" << x.Author << "\t" << x.KeyWord << "\t" <<std::fixed<<std::setprecision(2)<< std::stod(x.Price)<< "\t" << x.Storage << "\n";
            }
        } else if (a == "keyword") {
            // cout<<information<<std::endl;
            if (const auto key = find_KeyWord(information); key.size() > 1 || key.empty()) {
                throw std::runtime_error("");
            }
            auto ISBN_vector = KeyWord_reference.search(information);
            if (ISBN_vector.empty()) {
                cout << '\n';
                return;
            }
            vector<book_info> tmp;
            for (auto x: ISBN_vector) {
                tmp.push_back(ISBN[x]);
            }
            std::ranges::sort(tmp, [](const book_info &a1, const book_info &a2)-> bool { return strcmp(a1.ISBN, a2.ISBN) < 0; });
            for (const auto x: tmp) {
                cout << x.ISBN << "\t" << x.BookName << "\t" << x.Author << "\t" << x.KeyWord << "\t"<<std::fixed<<std::setprecision(2)<< std::stod(x.Price) << "\t" << x.Storage << "\n";
            }
        }
    }

    static void buy(char ISBN_[21], const int quantity) {
        if (login_status.empty()) {

            throw std::runtime_error("");
        }
        if (quantity <= 0) {
            throw std::runtime_error("");
        }
        const auto ISBN_vector = ISBN_reference.search(ISBN_);
        if (ISBN_vector.empty()) {
            throw std::runtime_error("");
        }
        if (ISBN[ISBN_vector[0]].Storage < quantity) {
            throw std::runtime_error("");
        }
        cout << std::fixed << std::setprecision(2) << std::stod(ISBN[ISBN_vector[0]].Price) * quantity << '\n';
        finance_list.emplace_back(std::stod(ISBN[ISBN_vector[0]].Price) * quantity, 0);
        ISBN[ISBN_vector[0]].Storage -= quantity;
    }

    static void select(char ISBN_[21]) {
        if (login_status.back().privilege < 3) {
            throw std::runtime_error("");
        }
        if (login_status.empty()) {
            throw std::runtime_error("");
        }
        if (ISBN_reference.search(ISBN_).empty()) {
            ISBN_reference.insert(ISBN_, static_cast<int>(ISBN.size()));
            ISBN.emplace_back(ISBN_);
        }
        login_status.back().num=ISBN_reference.search(ISBN_)[0];
    }

    static void modify_other(const string &a, char information[61]) {
        if (a == "name") {
            if (!BookName_reference.search(ISBN[login_status.back().num].BookName).empty()) {
                BookName_reference.Val_delete(ISBN[login_status.back().num].BookName, login_status.back().num);
            }
            strcpy(ISBN[login_status.back().num].BookName, information);
            BookName_reference.insert(information, login_status.back().num);
        } else if (a == "author") {
            if (!Author_reference.search(ISBN[login_status.back().num].Author).empty()) {
                 Author_reference.Val_delete(ISBN[login_status.back().num].Author, login_status.back().num);
            }
            strcpy(ISBN[login_status.back().num].Author, information);
            Author_reference.insert(information, login_status.back().num);
        } else if (a == "keyword") {
            const auto keys1=find_KeyWord(information);
            const auto keys2=find_KeyWord(ISBN[login_status.back().num].KeyWord);
            for (const auto& x:keys2) {
                // cout<<"key_word="<<x<<std::endl;
                KeyWord_reference.Val_delete(char_more<char[61]>(x).get_char().data(), login_status.back().num);
            }
            for (const auto& x:keys1) {
                // cout<<"key_word="<<x<<std::endl;
                KeyWord_reference.insert(char_more<char[61]>(x).get_char().data(), login_status.back().num);
            }
            strcpy(ISBN[login_status.back().num].KeyWord, information);
        } else if (a == "price") {
            // cout<<"price"<<ISBN[login_status.back().num].Price<<std::endl;
            // cout<<"changed price"<<information<<std::endl;
            strncpy(ISBN[login_status.back().num].Price, information, 14);
            // cout<<"price"<<ISBN[login_status.back().num].Price<<std::endl;
            // cout<<"changed price"<<information<<std::endl;
        }
    }

    static void modify_ISBN(char ISBN_[21]) {

        //std::cout<<"delete:"<<ISBN[login_status.back().num].ISBN<<' '<<login_status.back().num<<"\tchange:"<<ISBN[login_status.back().num].ISBN<<"->"<<ISBN_<<"\tinsert:"<<ISBN_<<" "<<login_status.back().num<<std::endl;
        ISBN_reference.Val_delete(ISBN[login_status.back().num].ISBN, login_status.back().num);
        strcpy(ISBN[login_status.back().num].ISBN, ISBN_);
        ISBN_reference.insert(ISBN_, login_status.back().num);
    }

    static void import(const int quantity, double total_cost) {
        if (login_status.empty()||login_status.back().privilege < 3) {
            throw std::runtime_error("");
        }
        if (login_status.back().num == -1 || quantity <= 0 || total_cost <= 0) {
            throw std::runtime_error("");
        }
        finance_list.emplace_back(0, total_cost);
        ISBN[login_status.back().num].Storage += quantity;
    }
};

#endif //BOOK_H
