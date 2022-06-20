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

//Todo 解析 argv

// 保存所有的进程编号到列表里面

// 对列表里面每个 拿到父亲是谁 解析 status 拿到 parent

// 建立好树

// 打印到终端上


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
  struct stat statbuf;  

  const char *dirname = "/proc";
  dp = opendir(dirname);

    while ((entry = readdir(dp)) != NULL) {
      lstat(entry->d_name, &statbuf);
      if (S_ISDIR(statbuf.st_mode)) {
          if (strcmp(entry->d_name, ".") == 0 ||   
              strcmp(entry->d_name, "..") == 0 ) {
                continue;  q
              }
          if (isDigitString(entry->d_name)) {
            printf("  %s\n", entry->d_name);
          }
      }
    }

    closedir(dp);
}


void testDisgit() {
  bool b = isDigitString("12342");
  printf("%d", b);
  b = isDigitString("123423i2");
  printf("%d", b);

}

int main(int argc, char *argv[]) {
  for (int i = 0; i < argc; i++) {
    assert(argv[i]);
    printf("argv[%d] = %s\n", i, argv[i]);
  }
  assert(!argv[argc]);
  walk();
  return 0;
}
 