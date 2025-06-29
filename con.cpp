#include "utf8_gbk_conv.h"

//#include <windows.h>  //设置控制台编码用

//#include <iostream>
#include <fstream>  
#include <string> 
#include <sstream>

std::string read_file_1(const std::string &fileName)
{
	std::fstream  in(fileName.c_str(), std::ios::in | std::ios::binary);//读文件内容
	if (!in)
	{
		printf("打开%s 错误!!\n", fileName.c_str());
		return "";
	} 
	std::stringstream os;
	os << in.rdbuf();//读取全部
	in.close();//关闭文件流
	return os.str();
}

std::string read_file_2(const std::string &fileName)
{ 
	std::fstream  in(fileName.c_str(), std::ios::in | std::ios::binary);
	if (!in)
	{
		printf("打开%s 错误!!\n", fileName.c_str());
		return "";
	}

	std::string tmp;
	in.seekg(0, std::ios::end); //设置光标在尾部
	size_t len = in.tellg();//获取文件大小
	in.seekg(0, std::ios::beg);//设置光标到开头

	char *s = new   char[len + 1];//获取+1大小的

	if (s == nullptr)//错误返回空串
		return "";
	memset(s, 0, len + 1);//初始化0

	in.read((char*)s, len);//读取这个大小到s
	in.close();//关闭文件

	tmp = s;//内部会拷贝给string
	delete[] s;//释放

	return tmp; //后续用string类型来接收
}

void write_file(std::string &file_name,std::string& data)
{ 
	
	std::fstream out(file_name.c_str(), std::ios::out | std::ios::binary);
	out.write(data.c_str(), data.size());
	out.close();
 
	 
//	printf(" %S 写入成功 <%s> 请查看编码\n", std::string('-',20).c_str(),file_name.c_str());
}

int main() {
	 
	std::string fileName = "con.cpp";
	std::string insertUTF8Name  = "-生成UTF-8";
	std::string insertGBKName  = "-生成GBK";

	std::string src_data, utf8_str, gbk_str;

	int pos = 0;

	printf("文件名:%s\n", fileName.c_str());
	 

	////----------------Gbk To Utf8---------------------------
	src_data = read_file_1(fileName);//读内容 
	if (src_data.empty()) {// 设置控制台为GBK编码
		printf("GBK 源没有内容\n");
		return -1;
	}
	utf8_str = convert(src_data,GbkToUtf8);

	//-----------------替换文件名--------------------------
	 pos = fileName.find_last_of('.');  //ReadMe.txt -> ReadMe-生成UTF8.txt
	if (pos == std::string::npos)
		pos = fileName.length();
	fileName.replace(pos, 0, insertUTF8Name);

	//-----------------写入 文件并打印UTF8--------------------------

	write_file(fileName, utf8_str);
	//---------------输出结束--------------------------
	//printf("文件名:%s\n", fileName.c_str());
	 
// 	if (!utf8_str.empty()) {// 设置控制台为UTF8编码
// 		SetConsoleOutputCP(65001);
// 		printf("UTF8 字符串: %s\n", utf8_str.c_str()); 
// 		SetConsoleOutputCP(936);
// 	}
// 	else
// 	{
// 		printf("UTF8 没有内容\n");
// 		return -1;
// 	}
	//==========================utf8 to gbk==========================
	

	 src_data.clear();//清空内容
	 src_data =  read_file_2(fileName);//读取生成的utf文件
	 if (src_data.empty()) { 
		 printf("UTF8 源没有内容\n");

	 }
	 gbk_str = convert(src_data, Utf8ToGbk);//开始转换
	 //----------------替换文件名--------------------------
	  
	   pos = fileName.find(insertUTF8Name);  //ReadMe-生成UTF8.txt -> ReadMe-生成GBK.txt
	 if (pos == std::string::npos)
		 pos = fileName.length();

	 fileName.replace(pos, insertUTF8Name.length(), insertGBKName);

	 //---------------写入文件-------------------------- 
	 write_file(fileName, gbk_str);
	 //---------------输出部分-------------------------- 
	 //printf("文件名:%s\n", fileName.c_str());

	 //if (!gbk_str.empty()) {// 设置控制台为GBK编码
		// SetConsoleOutputCP(936);
		// printf("GBK 字符串: %s\n", gbk_str.c_str());

	 //}
	 //else
	 //{
		// printf("GBK 没有内容\n");
	 //}
	 //---------------输出结束-------------------------- 
	return 0;
}