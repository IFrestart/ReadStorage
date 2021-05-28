#pragma once
#include <iostream>
//#include <Windows.h>
#include <objbase.h>
#include <string.h>
#include <vector>

using std::vector;


/*enum tagSTGTY
		{
			STGTY_STORAGE = 1,		// 表示存储元素是存储对象
			STGTY_STREAM = 2,		// 表示存储元素是一个流对象
			STGTY_LOCKBYTES = 3,	// 表示存储对象是字节数组对象
			STGTY_PROPERTY = 4		// 表示示存储元素是属性存储对象
		} 	STGTY;*/


//将宽字节Wchar转为string
char* wideCharToMultiByte(const wchar_t* pWCStrKey)
{
	//第一次调用确认转换后单字节字符串的长度，用于开辟空间
	int pSize = WideCharToMultiByte(CP_OEMCP, 0, pWCStrKey, wcslen(pWCStrKey), NULL, 0, NULL, NULL);
	char* pCStrKey = new char[pSize + 1];
	//第二次调用将双字节字符串转换成单字节字符串
	WideCharToMultiByte(CP_OEMCP, 0, pWCStrKey, wcslen(pWCStrKey), pCStrKey, pSize, NULL, NULL);
	pCStrKey[pSize] = '\0';
	return pCStrKey;

	//如果想要转换成string，直接赋值即可
	//string pKey = pCStrKey;
}

