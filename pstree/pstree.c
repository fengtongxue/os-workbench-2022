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

int processLen = 0;

struct dirent *processList[Len] = {0};
struct pNode *pNodeList[Len] = {0};

struct pNode *ROOT;

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
  int childLen;
} pNode;

void printNode(pNode *node, int ident)
{
  switch (ident)
  {
  case 0:
    printf("pid: %10d\n", node->pid);
    break;
  case 1:
    printf("pid: %20d\n", node->pid);
    break;
  case 2:
    printf("pid: %30d\n", node->pid);
    break;
  case 3:
    printf("pid: %40d\n", node->pid);
    break;
  default:
    printf("pid: %50d\n", node->pid);
    break;
  }
  // printf("pid: ", node->pid);
  // printf("pname: %s", node->pName);
  // printf("ppid: %d\n", node->ppid);
  // printf("childs Len: %d\n", node->childLen);
}

void printTree(pNode *node, int ident)
{
  printNode(node, ident);

  // 如果是叶子结点
  if (node->childLen == 0)
  {
    return;
  }

  // 如果有孩子子节点
  int nextIdent = ident + 1;
  for (size_t i = 0; i < node->childLen; i++)
  {
    printTree(node->childs[i], nextIdent);
  }
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
    if (c == ' ' || c == '\t')
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

void parseChildNode(pNode *parent)
{
  int childsIndex = 0;
  for (size_t i = 0; i < processLen; i++)
  {
    pNode *node = pNodeList[i];
    if (node->ppid == parent->pid)
    {
      parent->childs[childsIndex++] = node;
      node->parent = parent;
    }
  }
  parent->childLen = childsIndex;
  for (size_t i = 0; i < parent->childLen; i++)
  {
    pNode *childNode = parent->childs[i];
    parseChildNode(childNode);
  }
}

void parseTree()
{
  for (size_t i = 0; i < processLen; i++)
  {
    struct dirent *entry = processList[i];
    pNode *node = newPnode(entry);
    pNodeList[i] = node;
  }

  for (size_t i = 0; i < processLen; i++)
  {
    pNode *node = pNodeList[i];
    if (node->ppid == 0)
    {
      ROOT = node;
    }
  }

  parseChildNode(ROOT);
  printTree(ROOT, 0);
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
  processLen = offset;

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
