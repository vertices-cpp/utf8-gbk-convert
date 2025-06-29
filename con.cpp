#include "utf8_gbk_conv.h"

//#include <windows.h>  //���ÿ���̨������

//#include <iostream>
#include <fstream>  
#include <string> 
#include <sstream>

std::string read_file_1(const std::string &fileName)
{
	std::fstream  in(fileName.c_str(), std::ios::in | std::ios::binary);//���ļ�����
	if (!in)
	{
		printf("��%s ����!!\n", fileName.c_str());
		return "";
	} 
	std::stringstream os;
	os << in.rdbuf();//��ȡȫ��
	in.close();//�ر��ļ���
	return os.str();
}

std::string read_file_2(const std::string &fileName)
{ 
	std::fstream  in(fileName.c_str(), std::ios::in | std::ios::binary);
	if (!in)
	{
		printf("��%s ����!!\n", fileName.c_str());
		return "";
	}

	std::string tmp;
	in.seekg(0, std::ios::end); //���ù����β��
	size_t len = in.tellg();//��ȡ�ļ���С
	in.seekg(0, std::ios::beg);//���ù�굽��ͷ

	char *s = new   char[len + 1];//��ȡ+1��С��

	if (s == nullptr)//���󷵻ؿմ�
		return "";
	memset(s, 0, len + 1);//��ʼ��0

	in.read((char*)s, len);//��ȡ�����С��s
	in.close();//�ر��ļ�

	tmp = s;//�ڲ��´����string
	delete[] s;//�ͷ�

	return tmp; //������string����������
}

void write_file(std::string &file_name,std::string& data)
{ 
	
	std::fstream out(file_name.c_str(), std::ios::out | std::ios::binary);
	out.write(data.c_str(), data.size());
	out.close();
 
	 
//	printf(" %S д��ɹ� <%s> ��鿴����\n", std::string('-',20).c_str(),file_name.c_str());
}

int main() {
	 
	std::string fileName = "con.cpp";
	std::string insertUTF8Name  = "-����UTF-8";
	std::string insertGBKName  = "-����GBK";

	std::string src_data, utf8_str, gbk_str;

	int pos = 0;

	printf("�ļ���:%s\n", fileName.c_str());
	 

	////----------------Gbk To Utf8---------------------------
	src_data = read_file_1(fileName);//������ 
	if (src_data.empty()) {// ���ÿ���̨ΪGBK����
		printf("GBK Դû������\n");
		return -1;
	}
	utf8_str = convert(src_data,GbkToUtf8);

	//-----------------�滻�ļ���--------------------------
	 pos = fileName.find_last_of('.');  //ReadMe.txt -> ReadMe-����UTF8.txt
	if (pos == std::string::npos)
		pos = fileName.length();
	fileName.replace(pos, 0, insertUTF8Name);

	//-----------------д�� �ļ�����ӡUTF8--------------------------

	write_file(fileName, utf8_str);
	//---------------�������--------------------------
	//printf("�ļ���:%s\n", fileName.c_str());
	 
// 	if (!utf8_str.empty()) {// ���ÿ���̨ΪUTF8����
// 		SetConsoleOutputCP(65001);
// 		printf("UTF8 �ַ���: %s\n", utf8_str.c_str()); 
// 		SetConsoleOutputCP(936);
// 	}
// 	else
// 	{
// 		printf("UTF8 û������\n");
// 		return -1;
// 	}
	//==========================utf8 to gbk==========================
	

	 src_data.clear();//�������
	 src_data =  read_file_2(fileName);//��ȡ���ɵ�utf�ļ�
	 if (src_data.empty()) { 
		 printf("UTF8 Դû������\n");

	 }
	 gbk_str = convert(src_data, Utf8ToGbk);//��ʼת��
	 //----------------�滻�ļ���--------------------------
	  
	   pos = fileName.find(insertUTF8Name);  //ReadMe-����UTF8.txt -> ReadMe-����GBK.txt
	 if (pos == std::string::npos)
		 pos = fileName.length();

	 fileName.replace(pos, insertUTF8Name.length(), insertGBKName);

	 //---------------д���ļ�-------------------------- 
	 write_file(fileName, gbk_str);
	 //---------------�������-------------------------- 
	 //printf("�ļ���:%s\n", fileName.c_str());

	 //if (!gbk_str.empty()) {// ���ÿ���̨ΪGBK����
		// SetConsoleOutputCP(936);
		// printf("GBK �ַ���: %s\n", gbk_str.c_str());

	 //}
	 //else
	 //{
		// printf("GBK û������\n");
	 //}
	 //---------------�������-------------------------- 
	return 0;
}