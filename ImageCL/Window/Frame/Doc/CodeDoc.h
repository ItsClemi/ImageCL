#pragma once 

class CCodeDoc : public CDocument
{
	DECLARE_DYNCREATE( CCodeDoc )

public:
	CCodeDoc( );
	virtual ~CCodeDoc( );


public:
	virtual BOOL OnNewDocument( );
	virtual void Serialize( CArchive& ar );

private:


	DECLARE_MESSAGE_MAP( )

private:


};