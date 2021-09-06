#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#define FOLDER "/home/memo/db"
#define MAX 10
#define BUF_SIZE 0x40
#define FNAME_LENGTH 10

static char charset[] =
  "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"; 

struct Memo{
  char *content;
  size_t size;
  struct Memo* nxt;
};

struct User{
  char id[0x10];
  size_t cnt;
  struct Memo* memo;
};

void error(char *msg){
  puts(msg);
  abort();
}

char *rand_string(size_t length) {
  char *ret = NULL;
  if (length) {
    ret = malloc(sizeof(char) * (length +1));
    if (ret) {            
      for (int n = 0;n < length;n++) {            
        int key = rand() % (int)(sizeof(charset) -1);
        ret[n] = charset[key];
      }
      ret[length] = '\0';
    }
  }
  return ret;
}

void list_dir(char *dir){
  DIR *dp;
  struct dirent *ep;
  int cnt = 0;
  dp = opendir(dir);
  if(dp != NULL){
    puts("====================");
    while (ep = readdir(dp)){
      if(ep->d_name[0] != '.'){
        printf("%d. %s\n", cnt, ep->d_name);
        cnt ++;
      }
    }
    puts("\n====================");
  }else error("Cannot open the directory");
}

int check_string(char *s){
  int size = strlen(s);
  for(int i = 0; i < size; i++){
    if(!isalnum(s[i])) return 0;
  }
  return 1;
}

void init(){
  int seed;
  int fd = open("/dev/urandom", 0);
  if(fd < 0) error("open /dev/urandom error");
  read(fd, &seed, 4);
  close(fd);
  srand(seed);
  setbuf(stdin, NULL);
  setbuf(stdout, NULL);
  setbuf(stderr, NULL);
  alarm(0x10);
}

void help(){
  puts("1. List memo");
  puts("2. Write memo");
  puts("3. Edit memo");
  puts("4. Delete memo");
  puts("5. Save memo");
  puts("6. Load memo");
  puts("7. Exit");
}

int read_len(char *buf, size_t size){
  size_t i = 0;
  char tmp;
  puts("> ");
  while(i < size){
    if(read(0, &tmp, 1)){
      if(tmp == '\n'){
        buf[i] = 0;
        return i;
      }
      buf[i] = tmp;
      i++;
    }
  }
  buf[size-1] = 0;
  return i;
}

int get_int(){
  char buf[0x10];
  read_len(buf, 0x10);
  return atoi(buf);
}
void change_dir(char *id, int is_init){
  char buf[0x40];
  snprintf(buf, sizeof(buf), "%s/%s", FOLDER, id);
  if(is_init == 1 && mkdir(buf, 0770)){
    error("Makedir error!");
  }
  if(chdir(buf)){
    error("There is no such user!");
  }
}

void login(struct User* user){
  char buf[0x20];
  puts("Input User ID");
  read_len(buf, 0x10);
  if(check_string(buf) && strlen(buf) > 0){
    strncpy(user->id, buf, 0x10);
    change_dir(user->id, 0);
  }else error("Invalid user ID!");
}

struct User* init_user(){
  char buf[0x40];
  int menu;
  struct User* user = calloc(1, sizeof(struct User));
  puts("Do you have an user ID? (1/0)");
  menu = get_int();
  if(menu) login(user);
  else{
    strncpy(user->id, rand_string(10), 0x10);
    change_dir(user->id, 1);
  }
  printf("User ID : %s\n", user->id);
  return user;
}

void list_memo(struct User* user){
  int cnt = 0;
  puts("====================");
  for(struct Memo* iter = user->memo; iter != NULL; iter = iter->nxt){
    printf("\n%d.\n", cnt);
    write(1, iter->content, iter->size);
    cnt++;
  }
  puts("\n====================");
}

void add_memo(struct User* user, struct Memo* memo){
  if(user->cnt == 0){
    user->memo = memo;
  }else{
    struct Memo* iter;
    for(iter = user->memo; iter->nxt != NULL; iter = iter->nxt);
    iter->nxt = memo;
  }
  user->cnt ++;
}

struct Memo* get_memo(struct User* user, int idx){
  struct Memo *iter;
  for(iter = user->memo; idx > 0; idx--) iter = iter->nxt;
  return iter;
}

void write_memo(struct User* user){
  if(user->cnt < MAX){
    puts("Input memo");
    struct Memo* memo = calloc(1, sizeof(struct Memo));
    memo->content = calloc(1, BUF_SIZE);
    memo->size = read_len(memo->content, BUF_SIZE);
    add_memo(user, memo);
  }else error("Cannot write a memo anymore");
}

void edit_memo(struct User* user){
  puts("Input index");
  int idx = get_int();
  if(idx >= 0 && idx < user->cnt){
    puts("Input new memo");
    struct Memo *iter = get_memo(user, idx);
    iter->size = read_len(iter->content, BUF_SIZE);
  }else error("Invalid index");
}

void del_memo(struct User* user){
  puts("Input index");
  int idx = get_int();
  if(idx >= 0 && idx < user->cnt){
    struct Memo *cur, *prev;
    cur = get_memo(user, idx);
    if(idx == 0) user->memo = cur->nxt;
    else {
      prev = get_memo(user, idx-1);
      prev->nxt = cur->nxt;
    }
    free(cur->content);
    free(cur);
    user->cnt --;
  }else error("Invalid index");
}

void save_memo(struct User* user){
  int idx, fd;
  struct Memo *memo;
  char buf[0x40];
  char name[0x40];
  puts("Input index");
  idx = get_int();
  if(idx >= 0 && idx < user->cnt){
    memo = get_memo(user, idx);
    puts("Input memo name");
    read_len(buf, FNAME_LENGTH); 
    if(check_string(buf)){
      snprintf(name, sizeof(name), "%s/%s/%s", FOLDER, user->id, buf);
      fd = open(name, O_WRONLY | O_CREAT, 0660);
      if(fd < 0) error("Cannot save memo");
      write(fd, memo->content, memo->size);
      close(fd);
      puts("Done!");
    }else error("Invalid memo name");
  }else error("Invalid index");
}

void load_memo(struct User* user){
  char dir[0x40];
  char buf[0x40];
  char fname[0x40];
  int fd;
  size_t start, end;
  struct stat st;
  struct Memo *memo;
  if(user->cnt < MAX){
    snprintf(dir, sizeof(dir), "%s/%s", FOLDER, user->id);
    puts("You can load :");
    list_dir(dir);
    puts("Input memo name");
    read_len(buf, FNAME_LENGTH);
    if(check_string(buf)){
      snprintf(fname, sizeof(fname), "%s/%s", dir, buf);
      fd = open(fname, O_RDONLY);
      if(fd < 0) error("Cannot open a memo");
      if(fstat(fd, &st)) error("Cannot get a size of memo");
      printf("Input start pos [0-%ld]\n", st.st_size);
      start = get_int();
      if(start > st.st_size) error("Invalid start pos");
      printf("Input end pos [%ld-%ld]\n", start, st.st_size);
      end = get_int();
      if(end > st.st_size || start > end) error("Invalid end pos");
      lseek(fd, start, SEEK_SET);
      memo = calloc(1, sizeof(struct Memo));
      memo->content = calloc(1, BUF_SIZE);
      memo->size = read(fd, memo->content, end-start);
      add_memo(user, memo);
      close(fd);
    }else error("Invalid memo name");
  }else error("Cannot load a memo anymore");
}

int main(){
  int menu = 0;
  struct User* user;
  init();
  user = init_user();
  while(1){
    help();
    menu = get_int();
    switch(menu){
      case 1:
        list_memo(user);
        break;
      case 2:
        write_memo(user);
        break;
      case 3:
        edit_memo(user);
        break;
      case 4:
        del_memo(user);
        break;
      case 5:
        save_memo(user);
        break;
      case 6:
        load_memo(user);
        break;
      default:
        puts("Bye~");
        exit(0);
        break;
    }
  }

}
