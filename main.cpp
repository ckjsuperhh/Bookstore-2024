#include<iostream>
#include"cmd.h"
using namespace std;

[[noreturn]] int main() {
   string str;
   cmd::initialise();
   while (true) {
      try {
         while (getline(cin,str)) {
            cmd::analyse(str);
         }
      }catch (...) {
         cout<<"invalid\n";
      }
   }
}
