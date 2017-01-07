#pragma once

class CDeviceStatsDialog : public CDialogEx
{
public:
	CDeviceStatsDialog( );



protected:
	virtual BOOL OnInitDialog( ) override;
	virtual void DoDataExchange( CDataExchange* pDX ) override;

private:
	afx_msg void OnChangeDevice( );

	DECLARE_MESSAGE_MAP( )


private:
	CListBox	m_wndDeviceList;

	CEdit		m_wndDeviceName;
	CEdit		m_wndDeviceType;
	CEdit		m_wndVendor;

	CEdit		m_wndComputeUnits;
	CEdit		m_wndGlobalMemory;
	CEdit		m_wndLocalMemory;
	CEdit		m_wndWorkGroups;
	CListBox	m_wndDeviceExtensions;

};