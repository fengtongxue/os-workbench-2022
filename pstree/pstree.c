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
#define MAX_LINE_LENGTH 80

struct dirent *processList[Len] = {0};

// Todo 解析 argv

// 保存所有的进程编号到列表里面

// 对列表里面每个 拿到父亲是谁 解析 status 拿到 parent
//  read line, spit, key  name  ppid

// 建立好树

// 打印到终端上

typedef struct pNode
{
  struct pNode *parent;
  pid_t pid;
  pid_t ppid;
  char *pName;
  struct pNode *childs[Len];
} pNode;

void printNode(pNode* node) 
{
  printf("pid: %d\n", node->pid);
  printf("pname: %s\n", node->pName);
  printf("ppid: %d\n", node->ppid);

}

void parseLine(pNode *node, char line[])
{
  char key[MAX_LINE_LENGTH] = {};
  int keyLen = 0;
  char *value = malloc(MAX_LINE_LENGTH);
  int valueLen = 0;

  // a new line
  int lineOffset = 0;
  bool findColon = false;

  while (line[lineOffset] != '\n')
  {
    char c = line[lineOffset];
    lineOffset++;
    if (c == ':')
    {
      findColon = true;
      continue;
    }
    if (c == ' '|| c == '\t')
    {
      continue;
    }
    if (!findColon)
    {
      key[keyLen++] = c;
    }
    else
    {
      value[valueLen++] = c;
    }
  }
  if (strcmp(key, "Name") == 0)
  {
    node->pName = malloc(100);
    strncpy(node->pName, value, valueLen);
  }
  else if (strcmp(key, "PPid") == 0)
  {
    int i = atoi(value);
    node->ppid = i;
  }
}

pNode *newPnode(struct dirent *entry)
{
  pNode *p = malloc(sizeof(pNode));
  int i = atoi(entry->d_name);
  p->pid = (pid_t)i;
  char stausPath[100] = {};
  sprintf(stausPath, "/proc/%d/status", i);

  unsigned int line_count = 0;
  char line[MAX_LINE_LENGTH] = {0};
  FILE *file = fopen(stausPath, "r");
  if (!file)
  {
    perror(stausPath);
    assert(-1);
  }
  while (fgets(line, MAX_LINE_LENGTH, file))
  {
    /* Print each line */
    // printf("line[%06d]: %s", ++line_count, line);
    if (line[strlen(line) - 1] == '\n')
    {
      parseLine(p, line);
    }
  }
  /* Close file */
  if (fclose(file))
  {
    assert(-1);
    perror(stausPath);
  }
  return p;
}

void parseTree()
{
  for (size_t i = 0; i < Len; i++)
  {
    struct dirent *entry = processList[i];
    pNode *node = newPnode(entry);
    printNode(node);
    return;
    // if (entry == 0) {
    //   continue;
    // } else {

    // }
  }
}

bool isDigitString(char *s)
{
  int index = 0;
  while (s[index] != '\0')
  {
    if (!isdigit(s[index]))
    {
      return false;
    }
    index++;
  }
  return true;
}

void walk()
{
  DIR *dp;
  struct dirent *entry;

  const char *dirname = "/proc";
  dp = opendir(dirname);

  int offset = 0;
  while ((entry = readdir(dp)) != NULL)
  {
    if (entry->d_type == DT_DIR)
    {
      if (isDigitString(entry->d_name))
      {
        processList[offset++] = entry;
      }
    }
  }

  closedir(dp);
}



void printDirent()
{
  for (size_t i = 0; i < Len; i++)
  {
    struct dirent *entry = processList[i];
    if (entry == 0)
    {
      continue;
    }
    else
    {
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

int main(int argc, char *argv[])
{
  for (int i = 0; i < argc; i++)
  {
    assert(argv[i]);
    printf("argv[%d] = %s\n", i, argv[i]);
  }
  assert(!argv[argc]);
  walk();
  // printDirent();
  parseTree();
  return 0;
}
