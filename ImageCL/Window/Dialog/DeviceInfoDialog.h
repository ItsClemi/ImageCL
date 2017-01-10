#pragma once

class CDeviceInfoDialog : public CDialogEx
{
public:
	CDeviceInfoDialog( );



protected:
	virtual void DoDataExchange( CDataExchange* pDX ) override;
	virtual BOOL OnInitDialog( ) override;

private:
	afx_msg void OnChangeDevice( );

	DECLARE_MESSAGE_MAP( )


private:
	CComboBox	m_wndDeviceList;

	CEdit		m_wndDeviceName;
	CEdit		m_wndVendor;
	CEdit		m_wndPlatformVersion;

	CEdit		m_wndComputeUnits;
	CEdit		m_wndGlobalMemory;
	CEdit		m_wndLocalMemory;
	CEdit		m_wndWorkGroups;
	CListBox	m_wndDeviceExtensions;

};