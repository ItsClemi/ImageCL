#pragma once


class CImageDoc : public CDocument
{
protected:
	CImageDoc( );
	DECLARE_DYNCREATE( CImageDoc )
public:
	virtual ~CImageDoc( );


public:
	virtual BOOL OnNewDocument( );
	virtual void Serialize( CArchive& ar );

#ifdef _DEBUG
	virtual void AssertValid( ) const;
	virtual void Dump( CDumpContext& dc ) const;
#endif


protected:
	DECLARE_MESSAGE_MAP( )
};
