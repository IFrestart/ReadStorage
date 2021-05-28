# ReadStorage
复合文档解析
使用例子

	// WCHAR szPath[] = L"G:\\WorkSpace\\Registry.db";
	// WCHAR szPathName[] = L"E:\\WorkSpace\\QQDcyt\\Registry.db";
	WCHAR szPathName[] = L"E:\\文件夹测试目录\\文档\\20141012210616692.ppt";
	// WCHAR szPath[] = L"E:\\WorkSpace\\QQDcyt\\Registry2.0.db";

	IStorage* pIsStorage = NULL;
	// 枚举复合文档
	IEnumSTATSTG *iEnum = NULL;
	IEnumSTATSTG* iEnumStub = NULL;
	STATSTG stg = { 0 };
	STATSTG stgStub = { 0 };

	if (isStorageFile(szPathName))
	{
		pIsStorage = openStorageFile(szPathName);
	}

	iEnum = getEnumStorageFile(pIsStorage);
	stg = getNextEnumElements(iEnum);
	std::wstring strPath = L"E:\\文件夹测试目录\\文档\\20141012210616692";
	GetEnum(stg, pIsStorage, iEnum, strPath);

	pIsStorage->Release();
