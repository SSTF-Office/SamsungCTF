#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#define MAX 0x10
#define MAX_SIZE 0x1000

struct VFILE{
  char name[VNAME_SIZE];
  char *data;
  size_t size;
  struct VFILE* nxt;
};

struct VFILE *head = 0;
size_t cnt = 0;

void error(char *s){
  puts(s);
  exit(1);
}

char *calloc_chk(size_t size){
  char * ret = calloc(size,1);
  if(ret == 0){
    error("calloc error");
  }
  return ret;
}

size_t read_len(char *buf, size_t size){
  size_t ret = 0;
  char tmp ;
  while(ret < size){
    if(read(0,&tmp,1)){
      if(tmp == '\n'){
        break;
      }
      buf[ret] = tmp;
      ret ++;
    }
  }

  if(ret <size){
    buf[ret] = 0;
  }else{
    buf[ret-1] = 0;
  }
  return ret;
}
int get_int(){
  char buf[GET_INT_BUF];
  read_len(buf,GET_INT_BUF_OVER);
  return atoi(buf);
}
void init(){
  setbuf(stdin,0);
  setbuf(stdout,0);
  setbuf(stderr,0);
}
void welcome(){
  puts("This is simple file system.");
}

void help(){
  puts("1. Create File");
  puts("2. Modify File");
  puts("3. Read File");
  puts("4. List File");
  puts("5. Delete File");
  puts("6. Exit");
}

void add_file(struct VFILE * new){
  struct VFILE * iter;
  if(head ==0){
    head = new;
  }else{
    for(iter=head; iter->nxt ; iter= iter->nxt);
    iter->nxt = new;
  }
  cnt ++;
}

struct VFILE* find_file(char *name){
  struct VFILE *iter;
  for(iter =head; iter; iter = iter->nxt){
    if(!strcmp(iter->name,name)){
      return iter;
    }
  }
  return 0;
}
int exist_name(char *name){
  return find_file(name) !=0;
}
//first vuln
struct VFILE* get_file(char *msg){
  char name[VNAME_SIZE];
  struct VFILE *iter;
  puts(msg);
  read_len(name,GET_FILE_BUF_OVER);
  iter = find_file(name);
  if(iter==0)error("There is no such file.");
  return iter;
}
void create_file(){
  struct VFILE *new ;
  if(cnt < MAX){
    new = (struct VFILE *) calloc_chk(sizeof(struct VFILE));
    puts("File name :");
    read_len(new->name,VNAME_SIZE);
    if(exist_name(new->name)){
      error("Already exists");
    }
    puts("File size :");
    new->size = get_int();
    if(new->size > MAX_SIZE){
      error("Too big");
    }
    new->data = calloc_chk(new->size);
    puts("File Contents :");
    read(0,new->data,new->size);
    add_file(new);
    puts("DONE");
    
  }else{
    error("Too many files");
  }

}
// second vuln -- >heap
void modify_file(){
  char name[VNAME_SIZE_OVER];
  struct VFILE* iter = get_file("Name to modify");
  puts("New name :");
  read_len(name,VNAME_SIZE_OVER);
  if(exist_name(name)){
    error("Already exists");
  }
  strcpy(iter->name,name);
  puts("New file size :");
  iter->size = get_int();
  if(iter->size > MAX_SIZE){
    error("Too big");
  }
  free(iter->data);
  iter->data = calloc_chk(iter->size);
  puts("New file contents :");
  read(0,iter->data,iter->size);
  puts("DONE");
}
void read_file(){
  struct VFILE* iter = get_file("Name to read");
  write(1,iter->data,iter->size);
  puts("DONE");
}
void delete_file(){
  struct VFILE* iter = get_file("Name to delete");
  struct VFILE* prev ;
  if(iter == head){
    head = iter->nxt;
  }else{
    for(prev = head; prev->nxt !=iter; prev = prev->nxt);
    prev->nxt = iter->nxt;
  }
  free(iter->data);
  free(iter);
  cnt--;
  puts("DONE");
}
void list_file(){
  struct VFILE * iter;
  for(iter = head; iter; iter = iter->nxt){
    puts(iter->name);
  }
}
int main(){
  int menu;
  init();
  welcome();
  while(1){
    help();
    menu = get_int();
    switch(menu){
      case 1:
        create_file();
        break;
      case 2:
        modify_file();
        break;
      case 3:
        read_file();
        break;
      case 4:
        list_file();
        break;
      case 5:
        delete_file();
        break;
      default:
        exit(0);
    }
  }

}
