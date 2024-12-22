// ParamDlg.cpp: файл реализации
//

#include "pch.h"
#include "VIKA_KURSACH.h"
#include "afxdialogex.h"
#include "ParamDlg.h"


// Диалоговое окно ParamDlg

IMPLEMENT_DYNAMIC(ParamDlg, CDialogEx)

ParamDlg::ParamDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_PARAM, pParent)
	, A1(1)
	, W1(100)
	, Fi1(0)
	, A2(1)
	, W2(200)
	, Fi2(0)
	, A3(1)
	, W3(300)
	, Fi3(0)
	, N1(128)
	, N2(128)
	, N3(256)
	, N(512)
	, dt(3000)
	, NoiseLevel(0.1)
{

}

ParamDlg::~ParamDlg()
{
}

void ParamDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, A1);
	DDX_Text(pDX, IDC_EDIT2, W1);
	DDX_Text(pDX, IDC_EDIT3, Fi1);
	DDX_Text(pDX, IDC_EDIT4, A2);
	DDX_Text(pDX, IDC_EDIT5, W2);
	DDX_Text(pDX, IDC_EDIT6, Fi2);
	DDX_Text(pDX, IDC_EDIT7, A3);
	DDX_Text(pDX, IDC_EDIT8, W3);
	DDX_Text(pDX, IDC_EDIT9, Fi3);
	DDX_Control(pDX, IDC_COMBO1, SignalMode);
	DDX_Text(pDX, IDC_EDIT10, N1);
	DDX_Text(pDX, IDC_EDIT11, N2);
	DDX_Text(pDX, IDC_EDIT12, N3);
	DDX_Text(pDX, IDC_EDIT16, N);
	DDX_Text(pDX, IDC_EDIT15, dt);
	DDX_Control(pDX, IDC_COMBO2, Noiser);
	DDX_Text(pDX, IDC_EDIT17, NoiseLevel);
}


BEGIN_MESSAGE_MAP(ParamDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &ParamDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// Обработчики сообщений ParamDlg


BOOL ParamDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Добавить дополнительную инициализацию
	SignalMode.AddString(L"Сумма");
	SignalMode.AddString(L"Последовательный");
	if (sm.IsEmpty())SignalMode.SetCurSel(0);
	else SignalMode.SetWindowTextW(sm);

	Noiser.AddString(L"Розовый");
	Noiser.AddString(L"Красный");
	Noiser.AddString(L"Без шума");
	if (nm.IsEmpty())Noiser.SetCurSel(0);
	else Noiser.SetWindowTextW(nm);
	return TRUE;  // return TRUE unless you set the focus to a control
	// Исключение: страница свойств OCX должна возвращать значение FALSE
}


void ParamDlg::OnBnClickedOk()
{
	// TODO: добавьте свой код обработчика уведомлений
	UpdateData();
	SignalMode.GetWindowTextW(sm);
	Noiser.GetWindowTextW(nm);
	CDialogEx::OnOK();
}
