#include "EWDataSource.h"

//使用文件路径构造DataSource对象，初始化数据容器
EWDataSource::EWDataSource()
{
}

EWDataSource::~EWDataSource()
{
    //析构成员变量data
    for(vector<vector<double> *>::iterator iter = data.begin();
        iter != data.end();iter++)
    {
        if((*iter) != NULL){
            delete *iter;
            *iter = NULL;
        }
    }
    data.clear();
}

//读取文件，将获得的数据存入容器中
void EWDataSource::Read_File(const char* readFileName)
{
//	MyLogger * myLoger = NULL;
//	myLoger = MyLogger::getInstance();

	string str;       //每次读取一行csv文件返回的字符串  
					
	ifstream infile(readFileName);
	//如果打开文件失败，则退出
	if (!infile) {
//		LOG4CPLUS_FATAL(myLoger->logger, "打开文件失败,errCode=[" << GetLastError() << "]");
		exit(1);
	}
//	else
//		LOG4CPLUS_DEBUG(myLoger->logger, "打开数据文件...");
	while (getline(infile, str))       //每次读取一行数据,保存在string中
	{
        StringToNum(str);             //从string中提取小数
	}
//	LOG4CPLUS_DEBUG(myLoger->logger, "数据读取完毕。");
	infile.close(); 
}

//从字符串中提取小数
void EWDataSource::StringToNum(string str)
{
	bool temp = false;     //用于判断是否读完了一个数据
	bool is_minus = false;   //用于判断符号正负
 	int ndata = 0;        //整数部分  
	double  fdata = 0;    //小数部分  
	int n = 0;            //小数部分的位数  
    vector<double>* record = new vector<double>();

	for (int i = 0; i<str.length(); i++)
	{
		while ((str[i] >= '0') && (str[i] <= '9') || (str[i] == '.'))      //当前字符是数据或者是小数点  
		{
			temp = true;
			if (str[i] == '.')       //遇到了小数点  
			{
				//不停的读取后面的小数  
				i++;        //小数后的下一位  
				while ((str[i] >= '0') && (str[i] <= '9'))
				{
					n--;
					fdata += (pow(10, n)*(str[i] - '0'));
					i++;
				}
			}
			//没有遇到小数点  
			else
			{
				ndata *= 10;
				ndata += (str[i] - '0');
				i++;
			}
		}
		if (str[i] == '-')
			is_minus = true;
		
		//刚读取了数据  
		if (temp)
		{
			double num = ndata + fdata;
			if (is_minus)
				num = -num;
			ndata = 0;
			fdata = 0;

			//将数据插入到合适的位置
            record->push_back(num);
			n = 0;
			temp = false;
			is_minus = false;
		}
	}
    data.push_back(record);
}


bool EWDataSource::Output_File(const char* writeFileName, vector<double> scores)
{
//	MyLogger * myLoger = NULL;
//	myLoger = MyLogger::getInstance();

    // 写文件
    ofstream outFile(writeFileName, ios::out);
    if (!outFile) {
//		LOG4CPLUS_FATAL(myLoger->logger, "打开文件失败,errCode=[" << GetLastError() << "]");
        return false;
    }
    else
//		LOG4CPLUS_DEBUG(myLoger->logger, "写入数据文件...");

    for (int i = 0; i < scores.size() + 1; i++)
    {
        for (int j = 0; j < data.size(); j++)
        {
            if (i == 0)
            {
                vector<double>* row = data.at(j);
                double x = row->at(i);
                outFile << x << ',';
                if (j == data.size() - 1)
                    outFile << endl;
            }
            else
            {
                vector<double>* row = data.at(j);
                double x = row->at(i);
                outFile << x << ',';
            }
        }
        if(i!=0)
            outFile << scores.at(i - 1) << endl;

    }
		
    outFile.close();
//	LOG4CPLUS_DEBUG(myLoger->logger, "写入数据完成");
    return true;
}

