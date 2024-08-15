#include <stdio.h>

#define LOGT(format, ...) printf("\x1b[1m\x1b[35m[ trace " __FILE__ ":%d" " ]\x1b[0m " format "\n", __LINE__, __VA_ARGS__)
#define LOGD(format, ...) printf("\x1b[1m\x1b[34m[ debug " __FILE__ ":%d" " ]\x1b[0m " format "\n", __LINE__, __VA_ARGS__)
#define LOGI(format, ...) printf("\x1b[1m\x1b[32m[ info " __FILE__ ":%d" " ]\x1b[0m " format "\n", __LINE__, __VA_ARGS__)
#define LOGW(format, ...) printf("\x1b[1m\x1b[33m[ warn " __FILE__ ":%d" " ]\x1b[0m " format "\n", __LINE__, __VA_ARGS__)
#define LOGE(format, ...) printf("\x1b[1m\x1b[31m[ error " __FILE__ ":%d" " ]\x1b[0m " format "\n", __LINE__, __VA_ARGS__)

// inline void logd(const char *__restrict format, ...) {
//   printf("\x1b[33m%s\x1b[0m", )
// }
