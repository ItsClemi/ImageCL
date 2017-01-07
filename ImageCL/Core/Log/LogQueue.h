#pragma once


class CLogQueue
{
public:
	CLogQueue( );
	~CLogQueue( );

public:
	void Process( );

	inline void EnqueueLog( SLogEntry* pEntry )
	{
		ASSERT( m_bActive );

		m_queueLogs.push( pEntry );
	}


private:
	std::atomic_bool	m_bActive = true;

	Concurrency::concurrent_queue< SLogEntry* >			m_queueLogs;
};