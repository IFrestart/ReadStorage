#pragma once
#include <iostream>
//#include <Windows.h>
#include <objbase.h>
#include <string.h>
#include <vector>

using std::vector;


/*enum tagSTGTY
		{
			STGTY_STORAGE = 1,		// ��ʾ�洢Ԫ���Ǵ洢����
			STGTY_STREAM = 2,		// ��ʾ�洢Ԫ����һ��������
			STGTY_LOCKBYTES = 3,	// ��ʾ�洢�������ֽ��������
			STGTY_PROPERTY = 4		// ��ʾʾ�洢Ԫ�������Դ洢����
		} 	STGTY;*/


//�����ֽ�WcharתΪstring
char* wideCharToMultiByte(const wchar_t* pWCStrKey)
{
	//��һ�ε���ȷ��ת�����ֽ��ַ����ĳ��ȣ����ڿ��ٿռ�
	int pSize = WideCharToMultiByte(CP_OEMCP, 0, pWCStrKey, wcslen(pWCStrKey), NULL, 0, NULL, NULL);
	char* pCStrKey = new char[pSize + 1];
	//�ڶ��ε��ý�˫�ֽ��ַ���ת���ɵ��ֽ��ַ���
	WideCharToMultiByte(CP_OEMCP, 0, pWCStrKey, wcslen(pWCStrKey), pCStrKey, pSize, NULL, NULL);
	pCStrKey[pSize] = '\0';
	return pCStrKey;

	//�����Ҫת����string��ֱ�Ӹ�ֵ����
	//string pKey = pCStrKey;
}

