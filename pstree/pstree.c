#include <stdio.h>
#include <assert.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>  
#include <string.h>  
#include <sys/stat.h>  
#include <stdlib.h>  
#include <stdbool.h>
#include <ctype.h>


#define Len 1024
struct dirent *processList[Len] = {0};

// Todo 解析 argv

// 保存所有的进程编号到列表里面

// 对列表里面每个 拿到父亲是谁 解析 status 拿到 parent
  read line, spit, key  name  ppid 
  
// 建立好树

// 打印到终端上


typedef struct pNode
{
  struct pNode *parent;
  pid_t pid;
  char* pName;
  struct pNode *childs [Len];
} pNode;

pNode* newPnode(struct dirent* entry) {
  pNode* p = malloc(sizeof(pNode));
  int i = atoi(entry->d_name);
  p->pid = (pid_t) i;
  p->pName = entry->d_name;
  printf("name %s", p->pName);
  return p;
}

void parseTree() {
  for (size_t i = 0; i < Len; i++) {
    struct dirent* entry = processList[i];
    pNode * node = newPnode(entry);
    return;
    // if (entry == 0) {
    //   continue;
    // } else {
      
    // }
  }
}

bool isDigitString(char *s) {
  int index = 0;
  while (s[index] != '\0') {
      if (!isdigit(s[index])) {
        return false;
      }
      index ++;
  }
  return true;
}


void walk() {
  DIR *dp;  
  struct dirent *entry;  

  const char *dirname = "/proc";
  dp = opendir(dirname);

  int offset = 0;
  while ((entry = readdir(dp)) != NULL) {
    if (entry->d_type == DT_DIR) {
      if (isDigitString(entry->d_name)) {
          processList[offset++] = entry;
          printf("  %d   ", entry->d_type);
          printf("  %s\n", entry->d_name);
        }
      }
    }

    closedir(dp);
}

void printDirent() {
  for (size_t i = 0; i < Len; i++)
  {
    struct dirent* entry = processList[i];
    if (entry == 0) {
      continue;
    } else {
      printf("  %d   ", entry->d_type);
      printf("  %s\n", entry->d_name);
    }
  }
  
}

// void testDisgit() {
//   bool b = isDigitString("12342");
//   printf("%d", b);
//   b = isDigitString("123423i2");
//   printf("%d", b);
// }

int main(int argc, char *argv[]) {
  for (int i = 0; i < argc; i++) {
    assert(argv[i]);
    printf("argv[%d] = %s\n", i, argv[i]);
  }
  assert(!argv[argc]);
  walk();
  printDirent();
  parseTree();
  return 0;
}
 