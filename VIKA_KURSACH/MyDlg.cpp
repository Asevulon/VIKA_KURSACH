﻿
// MyDlg.cpp: файл реализации
//

#include "pch.h"
#include "framework.h"
#include "VIKA_KURSACH.h"
#include "MyDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Диалоговое окно CAboutDlg используется для описания сведений о приложении

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

// Реализация
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// Диалоговое окно MyDlg



MyDlg::MyDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_VIKA_KURSACH_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void MyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SIGNAL_PICTURE, SignalDrw);
	DDX_Control(pDX, IDC_FT_PICTURE, FTDrw);
	DDX_Control(pDX, IDC_WT_PICTURE, WTDrw);
	DDX_Control(pDX, IDC_FT_PICTURE2, WTIDDrw);
	DDX_Control(pDX, IDC_FT_PICTURE3, WTSUBDRW);
	//DDX_Control(pDX, IDC_SLIDER1, Slider);
}

BEGIN_MESSAGE_MAP(MyDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &MyDlg::OnBnClickedOk)
	//ON_BN_CLICKED(IDC_BUTTON_WT, &MyDlg::OnBnClickedButtonWt)
	//ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER1, &MyDlg::OnNMCustomdrawSlider1)
	ON_BN_CLICKED(IDC_BUTTON_DEBUG, &MyDlg::OnBnClickedButtonDebug)
END_MESSAGE_MAP()


// Обработчики сообщений MyDlg

BOOL MyDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Добавление пункта "О программе..." в системное меню.

	// IDM_ABOUTBOX должен быть в пределах системной команды.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Задает значок для этого диалогового окна.  Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок

	// TODO: добавьте дополнительную инициализацию
	SignalDrw.SetTitle(L"Сигнал");
	SignalDrw.SetPadding(22, 5, 10, 10);

	FTDrw.SetTitle(L"Спектр Фурье");
	FTDrw.SetPadding(22, 5, 10, 10);

	WTDrw.SetTitle(L"Вейвлет разложение");
	WTDrw.SetPadding(22, 5, 10, 15);

	WTIDDrw.SetTitle(L"Материнский вейвлет");
	WTIDDrw.SetPadding(22, 5, 10, 10);

	WTSUBDRW.SetTitle(L"Указанный уровень Вейвлет разложения (полуразности)");
	WTSUBDRW.SetPadding(22, 5, 10, 10);

	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
}

void MyDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// При добавлении кнопки свертывания в диалоговое окно нужно воспользоваться приведенным ниже кодом,
//  чтобы нарисовать значок.  Для приложений MFC, использующих модель документов или представлений,
//  это автоматически выполняется рабочей областью.

void MyDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // контекст устройства для рисования

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Выравнивание значка по центру клиентского прямоугольника
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Нарисуйте значок
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// Система вызывает эту функцию для получения отображения курсора при перемещении
//  свернутого окна.
HCURSOR MyDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void MyDlg::OnBnClickedOk()
{
	// TODO: добавьте свой код обработчика уведомлений
	if (pdlg.DoModal() != IDOK)return;
	m.SetDt(1. / pdlg.dt);
	m.SetN(pdlg.N);
	m.SetSignalMode(pdlg.sm);
	m.SetSin1(pdlg.A1, pdlg.W1, pdlg.Fi1, pdlg.N1);
	m.SetSin2(pdlg.A2, pdlg.W2, pdlg.Fi2, pdlg.N2);
	m.SetSin3(pdlg.A3, pdlg.W3, pdlg.Fi3, pdlg.N3);
	m.SetNoise(pdlg.nm);
	m.SetNoiseLevel(pdlg.NoiseLevel);
	m.SetFmax(pdlg.Fmax);
	m.SetFmin(pdlg.Fmin);
	m.SetFn(pdlg.Fn);
	m.main();
	
	SignalDrw.SetData(m.GetSignal(), m.GetSignalKeys());
	SignalDrw.Invalidate();

	FTDrw.SetData(m.GetFT(), m.GetFTKeys());
	FTDrw.Invalidate();

	WTDrw.SetData(m.GetWT());
	auto times = m.GetSignalKeys();
	WTDrw.SetGraphRange(times.front(), times.back(), pdlg.Fmin, pdlg.Fmax);
	WTDrw.Invalidate();

	WTIDDrw.SetData(m.GetWavelet(), m.GetWaveletkeys());

	WTSUBDRW.SetData(m.GetIWT(), m.GetWaveletkeys());
	/*WTDrw.SetData(m.GetWTFilled());
	WTDrw.Invalidate();

	WTIDDrw.SetData(m.GetWT(0));
	WTIDDrw.Invalidate();

	WTSUBDRW.SetData(m.GetWTSUB(0));
	WTSUBDRW.Invalidate();
	Slider.SetRange(0, m.GetWTLen() - 1);
	Slider.SetPos(0);*/
}

//void MyDlg::OnBnClickedButtonWt()
//{
//	// TODO: добавьте свой код обработчика уведомлений
//	m.swapwt();
//	WTDrw.SetData(m.GetWTFilled());
//	WTDrw.Invalidate();
//}
//
//
//void MyDlg::OnNMCustomdrawSlider1(NMHDR* pNMHDR, LRESULT* pResult)
//{
//	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
//	// TODO: добавьте свой код обработчика уведомлений
//	*pResult = 0;
//	/*int id = Slider.GetPos();
//	WTIDDrw.SetData(m.GetWT(id));
//	WTIDDrw.Invalidate();
//
//	WTSUBDRW.SetData(m.GetWTSUB(id));
//	WTSUBDRW.Invalidate();*/
//}

void MyDlg::OnBnClickedButtonDebug()
{
	// TODO: добавьте свой код обработчика уведомлений
	if (pdlg.DoModal() != IDOK)return;
	m.SetDt(1. / pdlg.dt);
	m.SetN(pdlg.N);
	m.SetSignalMode(pdlg.sm);
	m.SetSin1(pdlg.A1, pdlg.W1, pdlg.Fi1, pdlg.N1);
	m.SetSin2(pdlg.A2, pdlg.W2, pdlg.Fi2, pdlg.N2);
	m.SetSin3(pdlg.A3, pdlg.W3, pdlg.Fi3, pdlg.N3);
	m.SetNoise(pdlg.nm);
	m.SetNoiseLevel(pdlg.NoiseLevel);
	m.SetFmax(pdlg.Fmax);
	m.SetFmin(pdlg.Fmin);
	m.SetFn(pdlg.Fn);
	m.debug();

	vector<vector<double>>& spec = *(vector<vector<double>>*)m.debugdata;
	auto fs = spec.back();
	spec.pop_back();

	auto ifft = spec.back();
	spec.pop_back();
	auto sig = spec.back();
	spec.pop_back();

	CString str;
	bool awdwad = true;
	for (int i = spec.size() - 1; i > -1; --i)
	{
		auto window = new CustomImageDlg;
		str.Format(L"spec %d, s: %.2f", i, fs[i / 2]);
		window->SetWindowTextW(str);
		window->CustomImage.SetData(spec[i]);
		if(awdwad)window->SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOSIZE);
		else window->SetWindowPos(NULL, 800, 0, 0, 0, SWP_NOSIZE);
		awdwad = !awdwad;
	}
}
