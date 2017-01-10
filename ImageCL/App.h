#pragma once


class CApp : public CWinAppEx
{
public:
	CApp( );

private:
	virtual BOOL InitInstance( ) override;
	virtual int ExitInstance( ) override;

	virtual BOOL OnIdle( LONG lCount ) override;


private:
	afx_msg void OnFileOpen( );
	afx_msg void OnAppAbout( );

	DECLARE_MESSAGE_MAP( )

private:
	ULONG_PTR	m_gdiToken;

};


extern CApp gApp;
