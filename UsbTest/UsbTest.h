
// UsbTest.h : PROJECT_NAME アプリケーションのメイン ヘッダー ファイルです。
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"		// メイン シンボル


// CUsbTestApp:
// このクラスの実装については、UsbTest.cpp を参照してください。
//

class CUsbTestApp : public CWinApp
{
public:
	CUsbTestApp();

// オーバーライド
public:
	virtual BOOL InitInstance();

// 実装

	DECLARE_MESSAGE_MAP()
};

extern CUsbTestApp theApp;