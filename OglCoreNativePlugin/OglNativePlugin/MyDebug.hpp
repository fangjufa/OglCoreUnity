#pragma once
#include<string>
#include<fstream>
//不定参数用到的头文件。
#include<stdarg.h>

using namespace std;

class FileDebug
{
public:
	static void Log(string message)
	{
		int j=0;
		ofstream fout;
		//ios_base::app即向后追加内容而不覆盖掉之前的内容,它是Append的缩写。默认是会覆盖掉之前内容的。
		//如果一开始没有这个日志文件的话，那么它会创建这个目录文件，具体的文件路径下面有说。
		fout.open("LogFile.txt", ios_base::app);
		fout << message;

		//va_list arg_ptr;    //定义这个指向参数列表的变量
		//va_start(arg_ptr, message);//把上面这个变量初始化.即让它指向参数列表

		//while (j != -1)
		//{
		//	//获取arg_ptr指向的当前参数.这个参数的类型由va_arg的第2个参数指定
		//	j = va_arg(arg_ptr, int);
		//	if(j != -1)
		//		fout << j;
		//}
		//va_end(arg_ptr);    //第4步,做一些清理工作

		fout << "\n";
		
		fout.close();
	}

	static void Log(string message,int n)
	{
		int j = 0;
		ofstream fout;
		//ios_base::app即向后追加内容而不覆盖掉之前的内容,它是Append的缩写。默认是会覆盖掉之前内容的。
		//如果一开始没有这个日志文件的话，那么它会创建这个目录文件，具体的文件路径下面有说。
		fout.open("LogFile.txt", ios_base::app);
		fout << message<<n;

		//va_list arg_ptr;    //定义这个指向参数列表的变量
		//va_start(arg_ptr, message);//把上面这个变量初始化.即让它指向参数列表

		//while (j != -1)
		//{
		//	//获取arg_ptr指向的当前参数.这个参数的类型由va_arg的第2个参数指定
		//	j = va_arg(arg_ptr, int);
		//	if(j != -1)
		//		fout << j;
		//}
		//va_end(arg_ptr);    //第4步,做一些清理工作

		fout << "\n";

		fout.close();
	}
};