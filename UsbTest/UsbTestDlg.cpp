
// UsbTestDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "UsbTest.h"
#include "UsbTestDlg.h"
#include "afxdialogex.h"

/**/
#include <winioctl.h>
/**/

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define MAX_DRIVE_COUNT 128		// 最大ドライブの数（仮定）
// アプリケーションのバージョン情報に使われる CAboutDlg ダイアログ

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ダイアログ データ
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

// 実装
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CUsbTestDlg ダイアログ



CUsbTestDlg::CUsbTestDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CUsbTestDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CUsbTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_USB_LIST, m_xc_ComboCon);
}

BEGIN_MESSAGE_MAP(CUsbTestDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()


// CUsbTestDlg メッセージ ハンドラー

BOOL CUsbTestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// "バージョン情報..." メニューをシステム メニューに追加します。

	// IDM_ABOUTBOX は、システム コマンドの範囲内になければなりません。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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

	// このダイアログのアイコンを設定します。アプリケーションのメイン ウィンドウがダイアログでない場合、
	//  Framework は、この設定を自動的に行います。
	SetIcon(m_hIcon, TRUE);			// 大きいアイコンの設定
	SetIcon(m_hIcon, FALSE);		// 小さいアイコンの設定

	// TODO: 初期化をここに追加します。
	// UI追加予定あり
	// getUSB

	return TRUE;  // フォーカスをコントロールに設定した場合を除き、TRUE を返します。
}

void CUsbTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// ダイアログに最小化ボタンを追加する場合、アイコンを描画するための
//  下のコードが必要です。ドキュメント/ビュー モデルを使う MFC アプリケーションの場合、
//  これは、Framework によって自動的に設定されます。

void CUsbTestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 描画のデバイス コンテキスト

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// クライアントの四角形領域内の中央
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// アイコンの描画
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ユーザーが最小化したウィンドウをドラッグしているときに表示するカーソルを取得するために、
//  システムがこの関数を呼び出します。
HCURSOR CUsbTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


/*
	20180314
	hanyu
	Get all USB divice, add list
*/
BOOL CUsbTestDlg::getDisk()
{
	// 
	// コンボボックスに登録されているデータをリセット
	m_xc_ComboCon.ResetContent();

	BOOL bReturn;
	// ドライブのハンドルを取得
	CString strDrivePath;
	HANDLE hDrive = INVALID_HANDLE_VALUE;
	/*宣言*/
	STORAGE_DEVICE_DESCRIPTOR* pDescriptor;
	STORAGE_PROPERTY_QUERY sQuery;
	BYTE* pcbData;
	DWORD dwLen;
	DWORD dwRet;
	dwLen = 4096;
	pcbData = new BYTE[dwLen];

	STORAGE_BUS_TYPE BusType;
	/**/
	for (DWORD dwIndex = 0; dwIndex < MAX_DRIVE_COUNT; dwIndex++)
	{
		strDrivePath.Format(_T("\\\\.\\PhysicalDrive%d"), dwIndex);
		// get handle
		hDrive = CreateFile(
			strDrivePath,
			GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL);
		if (hDrive==INVALID_HANDLE_VALUE)
			continue;

		// bus type
		ZeroMemory(pcbData, dwLen);
		sQuery.PropertyId = StorageDeviceProperty;
		sQuery.QueryType = PropertyStandardQuery;
		sQuery.AdditionalParameters[0] = 0;
		bReturn = DeviceIoControl(
			hDrive,
			IOCTL_STORAGE_QUERY_PROPERTY,
			&sQuery,
			sizeof(STORAGE_PROPERTY_QUERY),
			pcbData,
			dwLen,
			&dwRet,
			NULL);
		if (!bReturn)
		{
			// IOCTL_STORAGE_QUERY_PROPERTY error
			// TODO
			delete[] pcbData;
			continue;
		}
		pDescriptor = (STORAGE_DEVICE_DESCRIPTOR*)pcbData;

		BusType = BusTypeUnknown;
		BusType = pDescriptor->BusType;

		if (BusType==BusTypeUsb)
		{
			// get usb
		}

	}
	return TRUE;
}
