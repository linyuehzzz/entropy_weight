#include "EWModel.h"

EWModel::EWModel()
{

}

EWModel::~EWModel()
{
    //析构成员变量normalizedIndex
    for(vector<vector<double> *>::iterator iter = normalizedIndex.begin();
        iter != normalizedIndex.end();iter++)
    {
        if((*iter) != NULL){
            delete *iter;
            *iter = NULL;
        }
    }
    normalizedIndex.clear();

    //析构成员变量weight
    for(vector<vector<double> *>::iterator iter = weight.begin();
        iter != weight.end();iter++)
    {
        if((*iter) != NULL){
            delete *iter;
            *iter = NULL;
        }
    }
    weight.clear();

    //析构成员变量transposedNormalized
    for(vector<vector<double> *>::iterator iter = transposedNormalized.begin();
        iter != transposedNormalized.end();iter++)
    {
        if((*iter) != NULL){
            delete *iter;
            *iter = NULL;
        }
    }
    transposedNormalized.clear();
}

/// <summary>
/// 熵权法，得到每个评价项目的综合评价得分
/// </summary>
/// <param name="index">读取的原始指标数据</param>
void EWModel::EWProcess(vector<vector<double> *> index)
{
    Normalize(index);       //指标归一化处理
    GetWeight(normalizedIndex);       //计算各指标中每个值的权重
    transposedNormalized = Transpose(normalizedIndex);//转置
    GetEntropy(weight);               //计算各指标熵值
    GetEntropyWeight(entropy);        //计算各指标熵权
    GetScore(transposedNormalized,entropyWeight);    //计算各评价项目得分

}


/// <summary>
/// 指标归一化处理
/// </summary>
/// <param name="data">数据矩阵</param>
void EWModel::Normalize(vector<vector<double>* > data)
{
    vector<vector<double>* >::iterator iter;//相当于行，指标
    vector<double>::iterator it;//相当于列，记录
    vector<double>::iterator begin;//指向每行的第一个数，得到flag
    vector<double>* vec1 = NULL;//存储每项指标的原始数据
    vector<double>* vec2 = NULL;//存储每项指标归一化后的数据
    double max,min;//各项指标中的最大、最小值
    double flag;//指标类型，正向指标为1，负向指标为-1
    double x,nor;//原始数据和归一化后的数据

    for(iter = data.begin();iter != data.end();iter++)
    {
        vec1 = *iter;
        max = GetMax(*vec1);
        min = GetMin(*vec1);

        begin = vec1->begin();
        flag = *begin;  //获取指标类型
        ++ begin;
        vec2 = new vector<double>();
        for (it = begin;it != vec1->end();it++)
        {
            x = *it;
            if(flag == 1)
                nor = (x - min)/(max - min);    //当flag=1时，正向指标归一化
            else if(flag == -1)
                nor = (max - x)/(max - min);    //当flag=-1时，负向指标归一化
            vec2->push_back(nor);
        }
        normalizedIndex.push_back(vec2);
    }
}


/// <summary>
/// 计算各指标中每个值的权重
/// </summary>
/// <param name="data">归一化指标数据</param>
void EWModel::GetWeight(vector<vector<double>* > data)
{
    vector<vector<double>* >::iterator iter;//相当于行，指标
    vector<double>::iterator it;//相当于列，记录
    vector<double>* vec1 = NULL;//存储每项指标的标准化数据
    vector<double>* vec2 = NULL;//存储每项指标的数据权重
    double sum;//每项指标的数据和
    double x,w;//标准化数据和权重

    for(iter = data.begin();iter != data.end();iter++)
    {
        vec1 = *iter;
        sum = GetSum(*vec1);

        vec2 = new vector<double>();
        for (it = vec1->begin();it != vec1->end();it++)
        {
            x = *it;
            w = x/sum;
            vec2->push_back(w);
        }
        weight.push_back(vec2);
    }
}


/// <summary>
/// 计算各指标熵值
/// </summary>
/// <param name="data">指标值权重数据</param>
void EWModel::GetEntropy(vector<vector<double>* > data)
{
    vector<vector<double>* >::iterator iter;//相当于行，指标
    vector<double>::iterator it;//相当于列，记录
    vector<double>* vec1 = NULL;//存储每项指标的数据权重
//    vector<double>* vec2 = NULL;//存储每项指标的熵值
    double x,e;//数据权重和熵值
    double t,k,sum,count;

    for(iter = data.begin();iter != data.end(); iter++)
    {
        vec1 = *iter;
        sum = 0;
        count = 0;
        for (it = vec1->begin();it != vec1->end();it++)
        {
            x = *it;
            if(x != 0)
                t = x * log(x);
            else
                t = 0;
            sum += t;
            ++ count;
        }
        k = 1/log(count);
        e = -k * sum;
        entropy.push_back(e);
    }
}


/// <summary>
/// 计算各指标熵权
/// </summary>
/// <param name="vec">指标熵值数据</param>
void EWModel::GetEntropyWeight(vector<double> vec)
{
    vector<double>::iterator it;
    double x,sum,ew;//指标熵值数据、熵值之和以及指标熵权

    sum = GetSum(vec);
    for(it = vec.begin(); it < vec.end();it++)
    {
        x = *it;
        ew=(1 - x)/(vec.size() - sum);
        entropyWeight.push_back(ew);
        printf("%lf ",ew);
    }
}


/// <summary>
/// 计算各评价项目得分
/// </summary>
/// <param name="data">原始指标数据</param>
/// <param name="vec">指标熵权数据</param>
void EWModel::GetScore(vector<vector<double>* > data,vector<double> vec)
{
    vector<vector<double>* >::iterator iter;//标准化数据迭代器，相当于行，指标
    vector<double>::iterator it;//标准化数据迭代器，相当于列，记录
//    vector<vector<double>* >::iterator begin;//指向标准化数据第一行
    vector<double>::iterator i;//熵权数据迭代器
    vector<double>* vec1;//存储每个评价项目的标准化数据
//    vector<double>* vec2;//存储每个评价项目的得分
    double x,ew,count;//标准化数据、指标熵权和指标序号
    double s,sum;//评价项目得分

    for(iter = data.begin();iter != data.end();iter++)
    {
        vec1 = *iter;
        count = 0;
        sum = 0;
        for (it = vec1->begin();it != vec1->end();it++)
        {
            x = *it;
            i = vec.begin()+count;
            ew = *i;
            s = x*ew;
            sum += s;
            ++ count;
        }
        score.push_back(sum);
    }
}


/// <summary>
/// 数据矩阵转置
/// </summary>
/// <param name="data">二维矩阵</param>
vector<vector<double>* > EWModel::Transpose(vector<vector<double>* > data)
{
    vector<vector<double>* > tr;
    int m = data.size();//行数
    int n = data.at(0)->size();//列数
    for(int i = 0; i < n; i++)
    {
        vector<double>* vec = new vector<double>();
        for (int j = 0; j < m; j++)
        {
            vector<double>* row = data.at(j);
            double x = row->at(i);
            vec->push_back(x);
        }
        tr.push_back(vec);
    }
    return tr;
}


/// <summary>
/// 计算一维数组的最大值
/// </summary>
/// <param name="vec">一维数组</param>
/// <returns>一维数组的最大值</returns>
double EWModel::GetMax(vector<double> vec)
{
    vector<double>::iterator it;
    vector<double>::iterator begin;//指向第一个数
    double x,max;//每项指标的原始数据和最大值

    begin = vec.begin();
    ++ begin;//跳过flag
    max = *begin;
    ++ begin;
    for(it = begin; it != vec.end();it++)
    {
        x = *it;
        if (x > max)
            max = x;
    }
    return max;
}


/// <summary>
/// 计算一维数组的最小值
/// </summary>
/// <param name="vec">一维数组</param>
/// <returns>一维数组的最小值</returns>
double EWModel::GetMin(vector<double> vec)
{
    vector<double>::iterator it;
    vector<double>::iterator begin;//指向第一个数
    double x,min;//每项指标的原始数据和最小值

    begin = vec.begin();
    ++ begin;//跳过flag
    min = *begin;
    ++ begin;
    for(it = begin; it < vec.end();it++)
    {
        x = *it;
        if (x < min)
            min = x;
    }
    return min;
}


/// <summary>
/// 一维数组求和
/// </summary>
/// <param name="vec">一维数组</param>
/// <returns>一维数组的数据和</returns>
double EWModel::GetSum(vector<double> vec)
{
    vector<double>::iterator it;
    double x,sum=0;//每项指标的原始数据和数据和

    for(it = vec.begin(); it < vec.end();it++)
    {
        x = *it;
        sum += x;
    }
    return sum;
}
