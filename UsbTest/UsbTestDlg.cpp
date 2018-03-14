
// UsbTestDlg.cpp : �����t�@�C��
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

#define MAX_DRIVE_COUNT 128		// �ő�h���C�u�̐��i����j
// �A�v���P�[�V�����̃o�[�W�������Ɏg���� CAboutDlg �_�C�A���O

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

// ����
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


// CUsbTestDlg �_�C�A���O



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


// CUsbTestDlg ���b�Z�[�W �n���h���[

BOOL CUsbTestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// "�o�[�W�������..." ���j���[���V�X�e�� ���j���[�ɒǉ����܂��B

	// IDM_ABOUTBOX �́A�V�X�e�� �R�}���h�͈͓̔��ɂȂ���΂Ȃ�܂���B
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

	// ���̃_�C�A���O�̃A�C�R����ݒ肵�܂��B�A�v���P�[�V�����̃��C�� �E�B���h�E���_�C�A���O�łȂ��ꍇ�A
	//  Framework �́A���̐ݒ�������I�ɍs���܂��B
	SetIcon(m_hIcon, TRUE);			// �傫���A�C�R���̐ݒ�
	SetIcon(m_hIcon, FALSE);		// �������A�C�R���̐ݒ�

	// TODO: �������������ɒǉ����܂��B
	// UI�ǉ��\�肠��
	// getUSB

	return TRUE;  // �t�H�[�J�X���R���g���[���ɐݒ肵���ꍇ�������ATRUE ��Ԃ��܂��B
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

// �_�C�A���O�ɍŏ����{�^����ǉ�����ꍇ�A�A�C�R����`�悷�邽�߂�
//  ���̃R�[�h���K�v�ł��B�h�L�������g/�r���[ ���f�����g�� MFC �A�v���P�[�V�����̏ꍇ�A
//  ����́AFramework �ɂ���Ď����I�ɐݒ肳��܂��B

void CUsbTestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �`��̃f�o�C�X �R���e�L�X�g

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// �N���C�A���g�̎l�p�`�̈���̒���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �A�C�R���̕`��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ���[�U�[���ŏ��������E�B���h�E���h���b�O���Ă���Ƃ��ɕ\������J�[�\�����擾���邽�߂ɁA
//  �V�X�e�������̊֐����Ăяo���܂��B
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
	// �R���{�{�b�N�X�ɓo�^����Ă���f�[�^�����Z�b�g
	m_xc_ComboCon.ResetContent();

	BOOL bReturn;
	// �h���C�u�̃n���h�����擾
	CString strDrivePath;
	HANDLE hDrive = INVALID_HANDLE_VALUE;
	/*�錾*/
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
