
// MyDlg.h: файл заголовка
//

#pragma once
#include"Drawer.h"
#include"Drawer2D.h"
#include"ParamDlg.h"
#include"Main.h"
// Диалоговое окно MyDlg
class MyDlg : public CDialogEx
{
// Создание
public:
	MyDlg(CWnd* pParent = nullptr);	// стандартный конструктор

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_VIKA_KURSACH_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// поддержка DDX/DDV


// Реализация
protected:
	HICON m_hIcon;

	// Созданные функции схемы сообщений
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	Drawer SignalDrw;
	Drawer FTDrw;
	Drawer2D WTDrw;
	ParamDlg pdlg;
	afx_msg void OnBnClickedOk();
	Main m;
	afx_msg void OnBnClickedButtonWt();
	Drawer WTIDDrw;
	Drawer WTSUBDRW;
	CSliderCtrl Slider;
	afx_msg void OnNMCustomdrawSlider1(NMHDR* pNMHDR, LRESULT* pResult);
};
