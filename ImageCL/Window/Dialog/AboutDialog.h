#pragma once

#include <Resource.h>


class CAboutDialog : public CDialogEx
{
	enum { IDD = IDD_ABOUTBOX };

public:
	CAboutDialog( );

protected:
	virtual void DoDataExchange( CDataExchange* pDX );   

														  
protected:
	DECLARE_MESSAGE_MAP( )
};


