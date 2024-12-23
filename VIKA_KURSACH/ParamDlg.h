#pragma once
#include "afxdialogex.h"


// Диалоговое окно ParamDlg

class ParamDlg : public CDialogEx
{
	DECLARE_DYNAMIC(ParamDlg)

public:
	ParamDlg(CWnd* pParent = nullptr);   // стандартный конструктор
	virtual ~ParamDlg();

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_PARAM };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	double A1;
	double W1;
	double Fi1;
	double A2;
	double W2;
	double Fi2;
	double A3;
	double W3;
	double Fi3;
	CComboBox SignalMode;
	CString sm;
	CString nm;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	int N1;
	int N2;
	int N3;
	int N;
	double dt;
	CComboBox Noiser;
	double NoiseLevel;
	double Fmin;
	double Fmax;
	int Fn;
};
