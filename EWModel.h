/**************************************
Author:林玥
Date:2017/11/18
Update:2017/11/19
Description:熵权法模型，流程为：指标归一化->计算各指标值权重->
            计算各指标熵值->计算各指标熵权->计算各评价项目得分
***************************************/
#ifndef EWMODEL_H
#define EWMODEL_H

#include <vector>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <math.h>
using namespace std;


class EWModel
{
public:

    EWModel();
    ~EWModel();

    vector<double> score;//评价得分数组

    /* 熵权法，得到每个评价项目的综合评价得分
    index:读取的原始指标数据 */
    void EWProcess(vector<vector<double>* > index);

private:

    vector<vector<double>* > normalizedIndex;//归一化指标矩阵
    vector<vector<double>* > weight;//指标权重矩阵
    vector<double> entropy;//指标熵值数组
    vector<double> entropyWeight;//指标熵权数组
    vector<vector<double>* > transposedNormalized;//转置归一化指标矩阵

    /* 数据矩阵转置
    data:二维矩阵数据 */
    vector<vector<double>* > Transpose(vector<vector<double>* > data);

    /* 指标归一化
    data:原始数据矩阵 */
    void Normalize(vector<vector<double>* > data);

    /* 计算各指标中每个值的权重
    data:归一化指标数据 */
    void GetWeight(vector<vector<double>* > data);

    /* 计算各指标熵值
    data:指标值权重数据 */
    void GetEntropy(vector<vector<double>* > data);

    /* 计算各指标熵权
    vec:指标熵值数据 */
    void GetEntropyWeight(vector<double>  vec);

    /* 计算各评价项目得分
    data:读取的原始指标数据
    vec:指标熵权数据 */
    void GetScore(vector<vector<double>* > data,vector<double> vec);

    /* 计算一维数组的最大值
    vec:一维数组 */
    double GetMax(vector<double> vec);

    /* 计算一维数组的最小值
    vec:一维数组 */
    double GetMin(vector<double> vec);

    /* 一维数组求和
    vec:一维数组 */
    double GetSum(vector<double> vec);
};

#endif // EWMODEL_H
