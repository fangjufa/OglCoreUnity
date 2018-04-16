#pragma once

#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <string>

namespace dxlib
{

	///-------------------------------------------------------------------------------------------------
	/// <summary> 日志信息. </summary>
	///
	/// <remarks> Dx, 2016/11/24. </remarks>
	///-------------------------------------------------------------------------------------------------
	class Debug
	{
	private:

		/// <summary> The debug object. </summary>
		static Debug* m_pInstance;

		/// <summary> 日志的文件. </summary>
		FILE* logFile;

		/// <summary> 当还没有创建一个日志文件的时候，就先将日志记录到这个里面. </summary>
		std::string tempStrInNoFile;

	public:


		///-------------------------------------------------------------------------------------------------
		/// <summary> Default constructor. </summary>
		///
		/// <remarks> Dx, 2017/3/11. </remarks>
		///-------------------------------------------------------------------------------------------------
		Debug()
		{
			logFile = NULL;
			isPrintTime = true;
			isLog = true;
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary> 输入一个路径创建一个log. </summary>
		///
		/// <remarks> Dx, 2017/3/11. </remarks>
		///
		/// <param name="path"> [in,out] If non-null, full
		/// 					pathname of the file. </param>
		///-------------------------------------------------------------------------------------------------
		Debug(const char* path)
		{
			logFile = NULL;
			isPrintTime = false;
			CreatLogFile(path);
			isLog = true;
		}


		///-------------------------------------------------------------------------------------------------
		/// <summary> Destructor. </summary>
		///
		/// <remarks> Dx, 2017/3/11. </remarks>
		///-------------------------------------------------------------------------------------------------
		~Debug()
		{
			Close();
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary> 获得单例. </summary>
		///
		/// <remarks> Dx, 2017/3/11. </remarks>
		///
		/// <returns>
		/// 返回单例对象，第一次调用会创建.
		/// </returns>
		///-------------------------------------------------------------------------------------------------
		static Debug* GetInst()
		{
			if (m_pInstance == NULL)  //判断是否第一次调用  
				m_pInstance = new Debug();
			return m_pInstance;
		}


		/// <summary> 是否在每条记录前面打印时间标签. </summary>
		bool isPrintTime;


		/// <summary> 是否进行日志的记录. </summary>
		bool isLog;

		///-------------------------------------------------------------------------------------------------
		/// <summary> 静态函数的输出日志，会同时输出到控制台和文件. </summary>
		///-------------------------------------------------------------------------------------------------
		static void Log(const char * strFormat, ...);


		///-------------------------------------------------------------------------------------------------
		/// <summary> 静态函数的输出日志，只输出到文件. </summary>
		///-------------------------------------------------------------------------------------------------
		static void LogF(const char * strFormat, ...);

		///-------------------------------------------------------------------------------------------------
		/// <summary> 创建日志文件. </summary>
		///
		/// <remarks> Xian.dai, 2017/2/15. </remarks>
		///
		/// <param name="path"> [in,out] If non-null, full
		/// 					pathname of the file. </param>
		///-------------------------------------------------------------------------------------------------
		void CreatLogFile(const char* path)
		{
			if (logFile != NULL)
			{
				Close();
			}
			if (isLog)//如果控制是要输出日志，那么才创建文件
			{
				printf("Debug.CreatLogFile():开始创建日志文件\r\n");
				logFile = fopen(path, "wb");
				if (logFile == NULL)
				{
					printf("Debug.CreatLogFile():创建日志文件失败!\r\n");
				}
				if (tempStrInNoFile.length() != 0)
				{
					fprintf(logFile, this->tempStrInNoFile.c_str());
					tempStrInNoFile.clear();
				}
			}
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary> 非静态函数的输出日志，会同时输出到控制台和文件. </summary>
		///-------------------------------------------------------------------------------------------------
		void _Log(const char * strFormat, ...);

		///-------------------------------------------------------------------------------------------------
		/// <summary> 非静态函数的输出日志，只输出到文件. </summary>
		///-------------------------------------------------------------------------------------------------
		void _LogF(const char * strFormat, ...);
		///-------------------------------------------------------------------------------------------------
		/// <summary> 关闭这个日志文件. </summary>
		///
		/// <remarks> Xian.dai, 2017/2/15. </remarks>
		///-------------------------------------------------------------------------------------------------
		void Close()
		{
			if (logFile != NULL)
			{
				printf("Debug.Close():关闭日志文件\r\n");
				fclose(logFile);
				logFile = NULL;
			}
		}

		

		///-------------------------------------------------------------------------------------------------
		/// <summary> 把一个时分秒的标签写到输入的地址. </summary>
		///
		/// <remarks> Dx, 2017/3/11. </remarks>
		///
		/// <param name="string"> [out] 要写的地址. </param>
		///-------------------------------------------------------------------------------------------------
		static void GetTimeString(char* string)
		{
 
			//clock_t	start = clock();
			time_t nowtime;
			struct tm *timeinfo;
			time(&nowtime);
			timeinfo = localtime(&nowtime);

			sprintf(string, "[%02d:%02d:%02d]", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
		}

	};



}