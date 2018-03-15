
// UsbTestDlg.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "UsbTest.h"
#include "UsbTestDlg.h"
#include "afxdialogex.h"

/**/
#include <winioctl.h>
/**/

// max buffer size 16MB
#define MAX_BUFFER_SIZE 16777216
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
	ON_BN_CLICKED(IDOK, &CUsbTestDlg::OnBnClickedOk)
	ON_WM_TIMER()
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
	getDisk();
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
	CA_List.RemoveAll(); // Clear array list(cstring)

	/*�錾*/
	BOOL bReturn;
	// �h���C�u�̃n���h�����擾
	CString strDrivePath;
	HANDLE hDrive = INVALID_HANDLE_VALUE;
	STORAGE_DEVICE_DESCRIPTOR* pDescriptor;
	STORAGE_PROPERTY_QUERY sQuery;
	BYTE* pcbData;
	DWORD dwLen;
	DWORD dwRet;
	dwLen = 4096;

	STORAGE_BUS_TYPE BusType;

	GET_LENGTH_INFORMATION DriverLength;// drive size
	CString strCombo;// �R���{�{�b�N�X�ɓo�^���镶����
	CAtlString	_strProductID;// device size
	CString strDiskSize;
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
		if (hDrive == INVALID_HANDLE_VALUE)
			continue;

		// drive size
		memset(&DriverLength, 0, sizeof(GET_LENGTH_INFORMATION));
		bReturn = DeviceIoControl(
			hDrive,
			IOCTL_DISK_GET_LENGTH_INFO,
			NULL,
			0,
			&DriverLength,
			sizeof(GET_LENGTH_INFORMATION),
			&dwRet,
			NULL);
		if (!bReturn)
		{
			dwErr = GetLastError();
			CloseHandle(hDrive);
			return FALSE;
		}
		strDiskSize.Format(_T("%.2f MB"),
			(float)DriverLength.Length.QuadPart / 1024 / 1024);

		// bus type
		pcbData = new BYTE[dwLen];
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
			dwErr = GetLastError();
			CloseHandle(hDrive);
			delete[] pcbData;
			return FALSE;
		}
		pDescriptor = (STORAGE_DEVICE_DESCRIPTOR*)pcbData;

		if (pDescriptor->ProductIdOffset)
			_strProductID = (char*)pDescriptor + pDescriptor->ProductIdOffset;

		BusType = BusTypeUnknown;
		BusType = pDescriptor->BusType;

		// set string
		strCombo.Format(_T("*Disk%d %s (%s)"), dwIndex, _strProductID, strDiskSize);
		if (BusType == BusTypeUsb)
		{
			// USB
			//AfxMessageBox(_T("A USB device."));

			// ToDo �R���{�{�b�N�X�ɓo�^�A���X�g�ɒǉ�
			m_xc_ComboCon.AddString(strCombo);
			CA_List.Add(strDrivePath);
		}

		delete[] pcbData;
		CloseHandle(hDrive);
	}

	return TRUE;
}


void CUsbTestDlg::OnBnClickedOk()
{
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
	// CDialogEx::OnOK();

	// �N���b�N���ꂽ�Ƃ��R���{�{�b�N�X�ɑI������Ă���f�o�C�X�̃n���h�������A�X�s�[�h�e�X�g���J�n����B

	//
	// ���ӓ_
	// �^�C�}�[�̐ݒu CFile::Open
	// �L���V���[���p���Ȃ��iUSB�H�j
	// �e�X�g�t�@�C���̍쐬�A�T�C�Y�i8MB�H�j
	// �ǂݎ������t�@�C���̍폜

	BOOL bReturn;
	// get device from combobox
	CString strDevice;
	//CString strTestFile;
	HANDLE hDevice = INVALID_HANDLE_VALUE;

	// get device size
	LARGE_INTEGER liTotalSize;
	GET_LENGTH_INFORMATION DriverLength;
	DWORD dwRet;

	DWORD dwBytesPerSetor;

	// read size
	DWORD dwReadSize;

	int iIndex = m_xc_ComboCon.GetCurSel();
	if (iIndex < 0)
	{
		getDisk();
		return;
	}
	strDevice = CA_List.GetAt(iIndex);
	//strTestFile.Format(_T("%s\\RW_File.UXD", strDevice));

	// createfile
	hDevice = CreateFile(
		strDevice,
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if (hDevice == INVALID_HANDLE_VALUE)
	{
		// create file failed, get error
		dwErr = GetLastError();
		MessageBox(_T("Create test file failed."), _T("USBTest"), MB_ICONERROR | MB_OK);
		return;
	}

	// get device size
	// will find a better way

	memset(&DriverLength, 0, sizeof(GET_LENGTH_INFORMATION));
	bReturn = DeviceIoControl(
		hDevice,
		IOCTL_DISK_GET_LENGTH_INFO,
		NULL,
		0,
		&DriverLength,
		sizeof(GET_LENGTH_INFORMATION),
		&dwRet,
		NULL);
	if (!bReturn)
	{
		dwErr = GetLastError();
		CloseHandle(hDevice);
		return;
	}
	liTotalSize.QuadPart = DriverLength.Length.QuadPart;

	// get sector size 
	DISK_GEOMETRY dg;

	bReturn = DeviceIoControl(
		hDevice,
		IOCTL_DISK_GET_DRIVE_GEOMETRY,
		NULL,
		0,
		&dg,
		sizeof(DISK_GEOMETRY),
		&dwRet,
		(LPOVERLAPPED)NULL);
	if (!bReturn)
	{
		// get sector size error.

		CloseHandle(hDevice);
		return;
	}
	dwBytesPerSetor = dg.BytesPerSector; // sector size

	// SetFilePointer() set read point default start at 0 (do test)(up to read 80MB)
	// read size and buffer size.
	// number of sectors from ini
	dwReadSize = 5 * 163840 * dwBytesPerSetor;//80MB

	if (dwReadSize > liTotalSize.QuadPart)
	{
		// read size > device size
		dwReadSize = (DWORD)liTotalSize.QuadPart / 2;
	}


	DWORD dwStart, dwReadTime;
	char *buffer;
	buffer = (char*)malloc(MAX_BUFFER_SIZE);
	dwStart = GetTickCount();
	int iMaxRead = (int)dwReadSize / MAX_BUFFER_SIZE;// �ő�ǂݍ��݉�
	for (int i = 0; i < iMaxRead; i++)
	{
		bReturn = ReadFile(hDevice, buffer, MAX_BUFFER_SIZE, &dwRet, NULL);
		if (!bReturn || dwRet != MAX_BUFFER_SIZE)
		{
			// read failed
			dwErr = GetLastError();
			CloseHandle(hDevice);
			free(buffer);
			return;
		}
	}

	dwReadTime = (GetTickCount() - dwStart) / 1000; // seconds

	DWORD dwSpeed;
	dwSpeed = dwReadSize / dwReadTime;

	CString strResult;
	strResult.Format(_T("Read speed is %d Mbps."), dwSpeed / 1024 / 1024);
	AfxMessageBox(strResult);

	// close handle
	CloseHandle(hDevice);
}


void CUsbTestDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B
	switch (nIDEvent)
	{
	case 1:

		break;
	default:
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}
