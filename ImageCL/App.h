#pragma once


class CVisualStyle;
class CApp : public CWinAppEx
{
public:
	CApp( );

private:
	virtual BOOL InitInstance( ) override;
	virtual int ExitInstance( ) override;

	virtual void PreLoadState( ) override;
	virtual void LoadCustomState( ) override;
	virtual void SaveCustomState( ) override;


	virtual BOOL OnIdle( LONG lCount ) override;


private:
	afx_msg void OnFileNew( );
	afx_msg void OnFileOpen( );

	afx_msg void OnAppAbout( );

	DECLARE_MESSAGE_MAP( )

public:

	CVisualStyle* GetVisualStyle( );


private:
	ULONG_PTR	m_gdiToken;



};


extern CApp gApp;
