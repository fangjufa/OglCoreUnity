#pragma once
#include "Debug.h"


namespace dxlib
{
	Debug* Debug::m_pInstance = NULL;


	void Debug::Log(const char * strFormat, ...)
	{
		if (m_pInstance == NULL) { Debug::GetInst(); }//如果单例对象还未构造那么就先构造一次
		if (!m_pInstance->isLog) { return; }	//如果控制是不输出日志
		if (NULL == strFormat) { return; }//如果输入参数为空

		va_list arg_ptr = NULL;
		va_start(arg_ptr, strFormat);

		char sTime[32];//打印到这个字符串里面
		Debug::GetTimeString(sTime);

		if (m_pInstance->isPrintTime)
			printf(sTime);//打印时间

		vprintf(strFormat, arg_ptr);//必须用vprintf 打印日志到控制台

		if (m_pInstance->logFile != NULL)//这里的logFile虽然是类的私有成员，但是仍然是可以访问的
		{
			if (m_pInstance->isPrintTime)
				fprintf(m_pInstance->logFile, sTime);//打印时间到文件

			vfprintf(m_pInstance->logFile, strFormat, arg_ptr);//打印日志到文件
			fflush(m_pInstance->logFile);
		}
		else
		{
			if (m_pInstance->isPrintTime)
				m_pInstance->tempStrInNoFile.append(sTime);
			char *sLog = new char[1024];//打印到这个字符串里面
			vsprintf(sLog, strFormat, arg_ptr);
			m_pInstance->tempStrInNoFile.append(sLog);//记录到暂存string
			delete sLog;
		}


		va_end(arg_ptr);
	}

	void Debug::LogF(const char * strFormat, ...)
	{
		if (m_pInstance == NULL) { Debug::GetInst(); }//如果单例对象还未构造那么就先构造一次
		if (!m_pInstance->isLog) { return; }	//如果控制是不输出日志
		if (NULL == strFormat) { return; }//如果输入参数为空

		va_list arg_ptr = NULL;
		va_start(arg_ptr, strFormat);

		char sTime[32];//打印到这个字符串里面
		Debug::GetTimeString(sTime);

		if (m_pInstance->logFile != NULL)//这里的logFile虽然是类的私有成员，但是仍然是可以访问的
		{
			if (m_pInstance->isPrintTime)
				fprintf(m_pInstance->logFile, sTime);//打印时间到文件

			vfprintf(m_pInstance->logFile, strFormat, arg_ptr);//打印日志到文件
			fflush(m_pInstance->logFile);
		}
		else
		{
			if (m_pInstance->isPrintTime)
				m_pInstance->tempStrInNoFile.append(sTime);
			char *sLog = new char[1024];//打印到这个字符串里面
			vsprintf(sLog, strFormat, arg_ptr);
			m_pInstance->tempStrInNoFile.append(sLog);//记录到暂存string
			delete sLog;
		}

		va_end(arg_ptr);
	}


	void Debug::_Log(const char *strFormat, ...)
	{
		if (!isLog)//如果控制是不输出日志，那么直接返回
		{
			return;
		}

		if (NULL == strFormat)
			return;

		va_list arg_ptr = NULL;
		va_start(arg_ptr, strFormat);

		char sTime[32];//打印到这个字符串里面
		Debug::GetTimeString(sTime);

		if (isPrintTime)
			printf(sTime);//打印时间

		vprintf(strFormat, arg_ptr);//必须用vprintf 打印日志到控制台

		if (logFile != NULL)
		{
			if (isPrintTime)
				fprintf(logFile, sTime);//打印时间到文件

			vfprintf(logFile, strFormat, arg_ptr);//必须使用带v的函数 打印日志到文件
			fflush(logFile);
		}
		else
		{
			if (this->isPrintTime)
				this->tempStrInNoFile.append(sTime);
			char *sLog = new char[1024];//打印到这个字符串里面
			vsprintf(sLog, strFormat, arg_ptr);
			this->tempStrInNoFile.append(sLog);//记录到暂存string
			delete sLog;
		}

		va_end(arg_ptr);//释放参数资源	
	}

	void Debug::_LogF(const char *strFormat, ...)
	{
		if (!isLog)//如果控制是不输出日志，那么直接返回
		{
			return;
		}

		if (NULL == strFormat)
			return;

		va_list arg_ptr = NULL;
		va_start(arg_ptr, strFormat);

		char sTime[32];//打印到这个字符串里面
		Debug::GetTimeString(sTime);

		if (logFile != NULL)
		{
			if (isPrintTime)
				fprintf(logFile, sTime);//打印时间到文件

			vfprintf(logFile, strFormat, arg_ptr);//必须使用带v的函数 打印日志到文件
			fflush(logFile);
		} 
		else
		{
			if (this->isPrintTime)
				this->tempStrInNoFile.append(sTime);
			char *sLog = new char[1024];//打印到这个字符串里面
			vsprintf(sLog, strFormat, arg_ptr);
			this->tempStrInNoFile.append(sLog);//记录到暂存string
			delete sLog;
		}
		va_end(arg_ptr);//释放参数资源	
	}

}