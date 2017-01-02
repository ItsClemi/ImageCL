#pragma once



class CCLManager
{
public:
	CCLManager( );
	~CCLManager( );

public:
	bool Initialize( );

public:
	
	void InitializeOpenCLAsync( );


};


//const char* LoadTemplateCode( );