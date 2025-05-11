#pragma once
#include "afxdialogex.h"


// Диалоговое окно CustomImageDlg
#include"Drawer.h"
class CustomImageDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CustomImageDlg)

public:
	CustomImageDlg(CWnd* pParent = nullptr);   // стандартный конструктор
	virtual ~CustomImageDlg();

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	Drawer CustomImage;
	virtual BOOL OnInitDialog();
};
