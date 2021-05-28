#pragma once
#include "common.h"
#include "imagehlp.h"
#include <io.h>
#include <direct.h>
#include<iostream>
#pragma comment(lib,"imagehlp.lib")
// 判断是否为复合文档
bool isStorageFile(WCHAR* szPath)
{
	HRESULT hResult = NULL;
	hResult = StgIsStorageFile(szPath);
	if (FAILED(hResult))
	{
		printf("不是复合文档！！！\n");
		return false;
	}
	return true;
}


// // 打开复合文档
IStorage* openStorageFile(WCHAR* szPathName)
{
	HRESULT hResult = NULL;
	IStorage* pIsStorage = NULL;

	if (isStorageFile(szPathName))
	{
		hResult = StgOpenStorage(szPathName, NULL,
			STGM_READWRITE | STGM_SHARE_EXCLUSIVE,  // 可读写并阻止其他人再打开流文件
			NULL, 0, &pIsStorage);
		if (hResult != S_OK)
		{
			printf("打开失败!!!\n");
			return NULL;
		}
	}
	return pIsStorage;
}

// 获取枚举器
IEnumSTATSTG* getEnumStorageFile(IStorage* pIsStorage)
{
	HRESULT hResult = NULL;
	IEnumSTATSTG *iEnum = NULL;
	hResult = pIsStorage->EnumElements(0, NULL, 0, &iEnum);

	if (FAILED(hResult))
	{
		printf("枚举器获取失败!!!\n");
		return NULL;
	}

	return iEnum;
}

// 获取枚举的下一个子项
STATSTG getNextEnumElements(IEnumSTATSTG *iEnum)
{
	STATSTG stg ;
	if (NOERROR == iEnum->Next(1, &stg, NULL))
	{
		return stg;
	}
	else
	{
		return { 0 };
	}
}

// 按字节读取Sream流
unsigned char* readStreamByte(IStorage* pIsStorage, STATSTG stg)
{
	IStream* pIsStream = NULL;
	HRESULT hResult = pIsStorage->OpenStream(stg.pwcsName, NULL, STGM_READWRITE | STGM_SHARE_EXCLUSIVE, 0, &pIsStream);
	if (hResult != S_OK)
	{
		MessageBox(NULL, L"Openstream错误", L"提示", MB_OK);
	}
	ULONG cb = stg.cbSize.LowPart;
	unsigned char* pV = new unsigned char[stg.cbSize.LowPart + 1];
	// vector<BYTE*> pV;
	ULONG pcbRead = 0;

	pIsStream->Read(pV, cb, &pcbRead);
	return pV;
}

void saveStubContext(unsigned char  *pV,char* szName,unsigned int n)
{
	FILE* fp = fopen(szName, "ab+");
	if (fp != NULL)
	{
		fwrite(pV, 1, n, fp);
		fclose(fp);
	}

}

// 释放打开的复合文档
void releaseStoragrFile(IStorage* pIsStorage, IStream* pIsStream)
{
	pIsStorage->Release();
	pIsStream->Release();
}

void GetEnum(STATSTG stg, IStorage* pIsStorage, IEnumSTATSTG* iEnum, std::wstring strPath)
{
	IStorage* pIsStubStorage = NULL;
	IEnumSTATSTG* iEnumStub = NULL;
	STATSTG stgStub = { 0 };
	for (; stg.pwcsName != NULL; stg = getNextEnumElements(iEnum))
	{
		if (STGTY_STORAGE == stg.type)
		{
			pIsStorage->OpenStorage(stg.pwcsName, NULL, STGM_READWRITE | STGM_SHARE_EXCLUSIVE, 0, NULL, &pIsStubStorage);
			iEnumStub = getEnumStorageFile(pIsStubStorage);
			stgStub = getNextEnumElements(iEnumStub);
			if (stgStub.pwcsName == NULL)  //空文件夹
			{
				std::wstring strPathNext = strPath + L"\\" + stg.pwcsName + L"\\";
				char* ch1;
				ch1 = wideCharToMultiByte(strPathNext.c_str());
				MakeSureDirectoryPathExists(ch1);
				continue;
			}
			std::wstring strPathNext = strPath + L"\\";
			strPathNext += stg.pwcsName;
			GetEnum(stgStub, pIsStubStorage, iEnumStub, strPathNext);
		}
		else if (STGTY_STREAM == stg.type)
		{
			pIsStorage->OpenStorage(stg.pwcsName, NULL, STGM_READWRITE | STGM_SHARE_EXCLUSIVE, 0, NULL, &pIsStubStorage);
			std::wstring strPathNext = strPath + L"\\";
			char* ch;
			ch = wideCharToMultiByte(strPathNext.c_str());
			if (_access(ch, 0) == -1)
			{
				MakeSureDirectoryPathExists(ch);
			}

			strPathNext += stg.pwcsName;
			char* ch1;
			ch1 = wideCharToMultiByte(strPathNext.c_str());
			printf("文件流：STGTY_STREAM 上级目录：%s\n", ch1);
			unsigned char* streamByte = readStreamByte(pIsStorage, stg);
			saveStubContext(streamByte, ch1, stg.cbSize.LowPart);
			delete[]streamByte;
		}
		else if (STGTY_LOCKBYTES == stg.type)
		{
			MessageBox(NULL, L"文件流:STGTY_LOCKBYTES", L"提示", MB_OK);
		}
		else if (STGTY_PROPERTY == stg.type)
		{
			MessageBox(NULL, L"文件流STGTY_PROPERTY", L"提示", MB_OK);
		}
	}
}
