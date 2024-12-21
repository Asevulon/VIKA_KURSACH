
// VIKA_KURSACH.h: главный файл заголовка для приложения PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "включить pch.h до включения этого файла в PCH"
#endif

#include "resource.h"		// основные символы


// CVIKAKURSACHApp:
// Сведения о реализации этого класса: VIKA_KURSACH.cpp
//

class CVIKAKURSACHApp : public CWinApp
{
public:
	CVIKAKURSACHApp();

// Переопределение
public:
	virtual BOOL InitInstance();

// Реализация

	DECLARE_MESSAGE_MAP()
};

extern CVIKAKURSACHApp theApp;
