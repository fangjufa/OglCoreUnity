#pragma once
#include<string>
#include<fstream>
//���������õ���ͷ�ļ���
#include<stdarg.h>

using namespace std;

class FileDebug
{
public:
	static void Log(string message)
	{
		int j=0;
		ofstream fout;
		//ios_base::app�����׷�����ݶ������ǵ�֮ǰ������,����Append����д��Ĭ���ǻḲ�ǵ�֮ǰ���ݵġ�
		//���һ��ʼû�������־�ļ��Ļ�����ô���ᴴ�����Ŀ¼�ļ���������ļ�·��������˵��
		fout.open("LogFile.txt", ios_base::app);
		fout << message;

		//va_list arg_ptr;    //�������ָ������б�ı���
		//va_start(arg_ptr, message);//���������������ʼ��.������ָ������б�

		//while (j != -1)
		//{
		//	//��ȡarg_ptrָ��ĵ�ǰ����.���������������va_arg�ĵ�2������ָ��
		//	j = va_arg(arg_ptr, int);
		//	if(j != -1)
		//		fout << j;
		//}
		//va_end(arg_ptr);    //��4��,��һЩ������

		fout << "\n";
		
		fout.close();
	}

	static void Log(string message,int n)
	{
		int j = 0;
		ofstream fout;
		//ios_base::app�����׷�����ݶ������ǵ�֮ǰ������,����Append����д��Ĭ���ǻḲ�ǵ�֮ǰ���ݵġ�
		//���һ��ʼû�������־�ļ��Ļ�����ô���ᴴ�����Ŀ¼�ļ���������ļ�·��������˵��
		fout.open("LogFile.txt", ios_base::app);
		fout << message<<n;

		//va_list arg_ptr;    //�������ָ������б�ı���
		//va_start(arg_ptr, message);//���������������ʼ��.������ָ������б�

		//while (j != -1)
		//{
		//	//��ȡarg_ptrָ��ĵ�ǰ����.���������������va_arg�ĵ�2������ָ��
		//	j = va_arg(arg_ptr, int);
		//	if(j != -1)
		//		fout << j;
		//}
		//va_end(arg_ptr);    //��4��,��һЩ������

		fout << "\n";

		fout.close();
	}
};