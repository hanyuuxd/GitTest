
// UsbTestDlg.h : �w�b�_�[ �t�@�C��
//

#pragma once
#include "afxwin.h"

// CUsbTestDlg �_�C�A���O
class CUsbTestDlg : public CDialogEx
{
// �R���X�g���N�V����
public:
	CUsbTestDlg(CWnd* pParent = NULL);	// �W���R���X�g���N�^�[

// �_�C�A���O �f�[�^
	enum { IDD = IDD_USBTEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �T�|�[�g


// ����
protected:
	HICON m_hIcon;

	// �������ꂽ�A���b�Z�[�W���蓖�Ċ֐�
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
	BOOL getDisk();
	// �R���{�{�b�N�X�̃R���g���[��
	CComboBox m_xc_ComboCon;

	/*
	modified by han 20180315
	*/
	CArray < CString > CA_List;
	DWORD dwErr; // general error code
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
