#define ull unsigned long long

extern ull* ptr[100];
extern int num_size;

ull myfunc(int i){
  if(ptr[i]!=0) return *ptr[i];
  ptr[i] = new ull;
  return *ptr[i] = (myfunc(i-1)+myfunc(i-2))&0x0FFFFFFF;
}

void test(){
  for(int i=0; i<num_size; i++)
    myfunc(i);
}
