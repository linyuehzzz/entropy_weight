/**************************************
Author:胡琦
Date:2017/11/15
Update:2017/11/17
Description:读取和输出文件的头文件，目前只读取CSV文件，之后根据主题的需要进行修改
***************************************/
#ifndef EWDATASOURCE_H
#define EWDATASOURCE_H
#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <algorithm>
#include <string>

using namespace std;


class EWDataSource
{
public:
    EWDataSource();
    ~EWDataSource();
    void Read_File(const char* readFileName);                                //读取文件
    bool Output_File(const char* writeFileName, vector<double> scores);        //输出文件

    vector<vector<double>* > data;         //储存读取到的指标

private:
    void StringToNum(string str);     //将读取到的字符串转为数字

	
};

#endif // EWDATASOURCE_H
