/**************************************************************************
 *  prolog make Secret table from FLAG.  -v 
 *  case1. recv attacker's plain message
 *  case2. Secret table Indexing with plain text chractor
 *  case3. make hashing and return it.
 * ************************************************************************/
#include<stdio.h>
#include<string.h>
#include <time.h>
#include <stdlib.h>
#include <stdint.h>

#include "ipc.h"
#include "pd.h"


/* TEST HEADER FOR CLFUSH */
#ifdef _MSC_VER
#include <intrin.h> /* for rdtscp and clflush */
#pragma optimize("gt",on)
#elif ARM
#else 
#include <x86intrin.h> /* for rdtscp and clflush */
#endif
#define CACHE_HIT_THRESHOLD 80
/* end of Test header */

uint8_t SECRET_KEY[32]; // ascii capital area 

unsigned int hashing(char * v1)
{
  unsigned int hash = 5381;
  while(*v1)
  {
    hash = (((hash << 5) + hash) + *v1) % 100003;
    v1++;
  }
  return (hash % 100003);
}

int readFlag()
{
  printf("The key is ONLY CAPITAL WORDS with space '_' ex) SCTF[THE_SECERT_KEY].\n");
  FILE *fp = fopen("/flag","r");
  fread(SECRET_KEY,32,1,fp);
  #ifdef DEBUG
  printf("[D] secret key [%s]\n",SECRET_KEY);
  #endif
  fclose(fp);
}

int main(int argc, char *argv[])
{
  unsigned int idx =0, msg_len =0;
  uint8_t IV[32]={0,};
  unsigned int hash_value = 0;
  
  arr_type * ptr = NULL;
  
  setIPC();
  readFlag();
  
  while (1)
  {
    /* read msg */
    while (1)
    {
      if (client_msg->status == 1)
      {
        #ifdef DEBUG
        printf("status:[%d], what:[%d], len:[%ld], msg:[%s]\n",client_msg->status,  client_msg->what, client_msg->msg_len ,client_msg->msg);
        #endif
        client_msg->status = 0;
        msg_len = (client_msg->msg_len < MAX_LEN ? client_msg->msg_len : MAX_LEN);
        break;
      }
    }
    /*message switch */
    switch (client_msg->what)
    {
    case 1:
      for(int i =0; i< msg_len; i++)
      {
        idx = client_msg->msg[i]  - 'A';
        #ifdef DEBUG
        printf("i:[%d], idx:[%d]\n", i, idx);
        #endif  
        if ( 0x00 <= idx && idx <= 0x20 )
        {
          idx = SECRET_KEY[idx];

          #ifdef DEBUG
          printf("idx:[%d], SECRET:[%s]  SECRET_KEY[idx]:[%c]\n",idx, SECRET_KEY, SECRET_KEY[idx]);
          #endif

          ptr = (arr_type *) get_eTable_ptr(idx);
          IV[ (i % 32) ] += ptr->val1 ^ ptr->val2;
        }
      }
      break;
    case 2:
      idx =0;
      for(int i = 0; i < 32; i++)
      {
        if (IV[i])  idx++;
      }  
      // if(idx <31) break; 
  
      hash_value = hashing(IV);
      #ifdef DEBUG
      printf("Hash value : %s\n", server_msg->msg);      
      #endif 
      if(hash_value == 0) // success 
      {
        sprintf(server_msg->msg, "Mining Success!");
        server_msg->msg_len = strlen("Mining Success!");

      }
      else  //failure 
      {          
        sprintf(server_msg->msg, "Mining Fail!");
        server_msg->msg_len = strlen("Mining Fail!");
      }
    
      server_msg->status  = 1;
      break;

    default:
      printf("[I] choice: 1~2 \n");
      break;
    }
  }
  releaseIPC();
  return 0;
}
