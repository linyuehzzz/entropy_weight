#ifndef EWTHEME_H
#define EWTHEME_H

#include "EWDataSource.h"
#include "EWModel.h"

class EWTheme
{
public:
    EWTheme(const char *readFileName, const char *writeFileName);
    ~EWTheme();

    void RunModel();

private:
    const char* readFileName;         //读取文件路径
    const char* writeFileName;	      //输出文件路径

    EWDataSource* d;
    EWModel* m;
};

#endif // EWTHEME_H
