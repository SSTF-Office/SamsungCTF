#include <iostream>

#define ull unsigned long long
using namespace std;

extern const int num_size = 70;
ull* ptr[num_size]={0};

void clean(){
  for(int i=0; i<num_size; i++){
    delete ptr[i];
    ptr[i] = 0;
  }
}
extern void test();

int main(){
  for(int tc=0; tc<1; tc++){
    
    ptr[0] = new ull(0);
    ptr[1] = new ull(0);
    cin >> *ptr[0];
    cin >> *ptr[1];

    test();
    
    for(int i=0; i<num_size; i++){
      cout << *ptr[i] << endl;
    }
    clean();
  }
  return 0;
}
