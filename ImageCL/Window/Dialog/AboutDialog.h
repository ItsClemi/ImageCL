#pragma once


class CAboutDialog : public CDialogEx
{
public:
	CAboutDialog( );

protected:
	virtual BOOL OnInitDialog( ) override;

	virtual void DoDataExchange( CDataExchange* pDX ) override;

														  
protected:
	DECLARE_MESSAGE_MAP( )
};


