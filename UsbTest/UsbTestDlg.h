
// UsbTestDlg.h : ヘッダー ファイル
//

#pragma once
#include "afxwin.h"

struct DISKINFO
{

	DWORD dwDisk;			// disk number
	DWORD dwPar;			// par number
};

// CUsbTestDlg ダイアログ
class CUsbTestDlg : public CDialogEx
{
// コンストラクション
public:
	CUsbTestDlg(CWnd* pParent = NULL);	// 標準コンストラクター

// ダイアログ データ
	enum { IDD = IDD_USBTEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート


// 実装
protected:
	HICON m_hIcon;

	// 生成された、メッセージ割り当て関数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
	BOOL getDisk();
	// コンボボックスのコントロール
	CComboBox m_xc_ComboCon;
};
