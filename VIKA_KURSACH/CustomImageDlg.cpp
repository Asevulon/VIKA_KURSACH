// CustomImageDlg.cpp: файл реализации
//

#include "pch.h"
#include "VIKA_KURSACH.h"
#include "afxdialogex.h"
#include "CustomImageDlg.h"


// Диалоговое окно CustomImageDlg

IMPLEMENT_DYNAMIC(CustomImageDlg, CDialogEx)

CustomImageDlg::CustomImageDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{
	Create(IDD_DIALOG1);
}

CustomImageDlg::~CustomImageDlg()
{
}

void CustomImageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CUSTOM_PICTURE, CustomImage);
}


BEGIN_MESSAGE_MAP(CustomImageDlg, CDialogEx)
END_MESSAGE_MAP()


// Обработчики сообщений CustomImageDlg

BOOL CustomImageDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Добавить дополнительную инициализацию
	ShowWindow(SW_SHOW);
	return TRUE;  // return TRUE unless you set the focus to a control
	// Исключение: страница свойств OCX должна возвращать значение FALSE
}
