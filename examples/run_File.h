#ifndef _RUN_FILE_H_
#define _RUN_FILE_H_

#include "DEV_Config.h"

#define fileNumber 100
#define fileLen 100
#define maxLineLength 128

typedef struct {
    int mode;
    int min_refresh;
    int max_refresh;
} Config;


char sdTest(void);
void sdInitTest(void);

void run_mount(void);
void run_unmount(void);

void file_cat(void);

void sdScanDir(void);

char isFileExist(const char *path);
void setFilePath(void);

void updatePathIndex(void);
void file_sort();

bool selectRandomFile(char *filePath, size_t pathSize);
int readConfig(const char *path, Config *cfg);

#endif
