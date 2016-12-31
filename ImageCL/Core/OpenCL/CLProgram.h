#pragma once


class CCLProgram
{
public:
	CCLProgram( );

	virtual ~CCLProgram( );

public:
	bool LoadFile( std::shared_ptr< std::wstring >&& szPath );

};