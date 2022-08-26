#include <iostream>
#include <string>
#include <vector>
#include "obfuscator.hpp"

#define NUM 8

using namespace std;

class test{
        private:
                string range = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789@_";
                string flag = OBFUSCATE("ubx1uP9vh@kq9xXxF4Cxp93u319085");
                string input;
                char encoder[NUM][NUM]={0,};

                void padding(vector<char>& plain){
                        unsigned int i;
                        unsigned int input_len = input.length();
                        for ( i = 0; i < input_len; i++){
                                if ( input[i] == '\n' )
                                        break;
                                else if ( input[i] == ' ' )
                                        continue;
                                else if ( range.find(input[i]) != string::npos) 
                                        plain.push_back(input[i]);
                                else
                                        continue;
                        }
                }

                void convert_string(vector<char> &plain,vector<char> &converted){
                        unsigned int i,j;
                        for ( i = 0,j = 0; i < plain.size(); ){
                                converted.push_back(plain[i]);
                                i++;
                                if ( i == plain.size() )
                                {
                                        converted.push_back('X');
                                        break;
                                }

                                if ( plain[i] == converted[j] )
                                {
                                        converted.push_back('X');
                                        j++;
                                }

                                else if ( plain[i] != converted[j] ){
                                        j++;
                                        converted.push_back(plain[i]);
                                        i = i + 1;
                                }
                                j++;
                        }
                }

                void get_pos(char p, int& r, int& c)
                {
                        int i,j;
                        for ( i = 0; i < NUM; i++ ){
                                for ( j = 0; j < NUM; j++ ){
                                       if (p == encoder[i][j] ){
                                               r = i;
                                               c = j;
                                               return ;
                                       }
                                }
                        }
                        return;
                }

                void same_row(int r, vector<char>& code, int c1, int c2)
                {
                        code.push_back(encoder[r][(c1 + 1) % NUM]);
                        code.push_back(encoder[r][(c2 + 1) % NUM]);
                        return;
                }

                void same_column(int c, vector<char>& code, int r1, int r2)
                {
                        code.push_back(encoder[(r1 + 1) % NUM][c]);
                        code.push_back(encoder[(r2 + 1) % NUM][c]);
                        return;
                }

                void diff_col_row(int r1, int c1, vector<char>& code, int r2, int c2)
                {
                        code.push_back(encoder[r1][c2]);
                        code.push_back(encoder[r2][c1]);
                        return;
                }

                void encode(vector<char> converted,vector<char> &code){
                        unsigned int len = converted.size();
                        unsigned int i = 0;
                        int r1=0,c1=0,r2=0,c2=0;
                        while(i<len){
                                get_pos(converted[i],r1,c1);
                                i++;
                                get_pos(converted[i],r2,c2);
                                if( r1 == r2 )
                                        same_row(r1,code,c1,c2);
                                else if ( c1 == c2 )
                                        same_column(c1,code,r1,r2);
                                else
                                        diff_col_row(r1,c1,code,r2,c2);
                                i++;
                        }
                }

        public:

                test(string s){
                        input = s;
                        int i,j,c;
                        c = 0;
                        for ( i = 0; i < NUM; i++){
                                for ( j = 0; j < NUM; j++){
                                        encoder[i][j] = range[c++];
                                }
                        }
                }

                bool exec(){
                        vector<char> plain;
                        vector<char> converted;
                        vector<char> code;
                        padding(plain);
                        convert_string(plain,converted);
                        encode(converted,code);
                        string str1(code.begin(),code.end());
                        return str1 == flag;
                }
        
};



int main(void){
        string input;
        string flag;

        cout << "input password : ";
        cin  >> input;

        bool ans;

        test *t = new test(input);
        ans = t->exec();
        if ( ans == true ){
                cout << "congratulations" << endl;
                flag = "flag={"+input+"}";
                cout << flag << endl;
        }
        else{
                cout << "sorry, It is not correct" << endl;
        }
        return 0;
}
