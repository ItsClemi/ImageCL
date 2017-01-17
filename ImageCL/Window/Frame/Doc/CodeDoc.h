#pragma once 

class CCodeView;
class CCodeDoc : public CDocument
{
	DECLARE_DYNCREATE( CCodeDoc )

public:
	CCodeDoc( );
	virtual ~CCodeDoc( );

public:
	virtual BOOL OnNewDocument( );
	virtual void Serialize( CArchive& ar );
	
	virtual BOOL IsModified( );

public:
	//Concurrency::task< void > RunKernel( const SCLDevice* pDevice );

	const std::shared_ptr< std::vector< char > > GetCode( );

private:
	CCodeView* GetView( );

private:


	DECLARE_MESSAGE_MAP( )

private:


};