#include "stdafx.h"
#include "DeviceInfoDialog.h"

#include "Core/OpenCL/CLDevice.h"

BEGIN_MESSAGE_MAP( CDeviceInfoDialog, CDialogEx )
	ON_CBN_SELCHANGE( IDC_DEVICE_LIST,  &CDeviceInfoDialog::OnChangeDevice )

END_MESSAGE_MAP( )


CDeviceInfoDialog::CDeviceInfoDialog( )
	: CDialogEx( IDD_DEVICE_STATS )
{
}

void CDeviceInfoDialog::DoDataExchange( CDataExchange* pDX )
{
	CDialogEx::DoDataExchange( pDX );

	DDX_Control( pDX, IDC_DEVICE_LIST, m_wndDeviceList );

	DDX_Control( pDX, IDC_DEVICE_NAME, m_wndDeviceName );
	DDX_Control( pDX, IDC_DEVICE_VENDOR, m_wndVendor );
	DDX_Control( pDX, IDC_PLATFORM_VERSION, m_wndPlatformVersion );

	DDX_Control( pDX, IDC_DEVICE_COMPUTE_UNITS, m_wndComputeUnits );
	DDX_Control( pDX, IDC_DEVICE_GLOBAL_MEMORY, m_wndGlobalMemory );
	DDX_Control( pDX, IDC_DEVICE_LOCAL_MEMORY, m_wndLocalMemory );
	DDX_Control( pDX, IDC_DEVICE_WORK_GROUPS, m_wndWorkGroups );
	DDX_Control( pDX, IDC_DEVICE_EXTENSIONS, m_wndDeviceExtensions );
}


BOOL CDeviceInfoDialog::OnInitDialog( )
{
	if( !CDialogEx::OnInitDialog( ) )
	{
		return FALSE;
	}

	SetIcon( AfxGetApp( )->LoadIconW( IDR_MAINFRAME ), FALSE );


	for( const auto i : gEnv->pClManger->GetDevices( ) )
	{
		int nPos = m_wndDeviceList.AddString( i->GetDeviceName( ) );
		m_wndDeviceList.SetItemDataPtr( nPos, i );
	}

	m_wndDeviceList.SetCurSel( 0 );
	OnChangeDevice( );

	return TRUE;
}


void CDeviceInfoDialog::OnChangeDevice( )
{
	int nSel = m_wndDeviceList.GetCurSel( );

	if( nSel != CB_ERR )
	{
 		const auto pDevice = reinterpret_cast< CCLDevice* >( 
 			m_wndDeviceList.GetItemDataPtr( nSel ) 
 			);
	
		CString szDevice;
		szDevice.Format( L"%s (%s)", pDevice->GetDeviceName(), pDevice->GetDeviceTypeString( ) );
		m_wndDeviceName.SetWindowTextW( szDevice );

		CString szVendor;
		szVendor.Format( L"%s", pDevice->GetPlatform()->m_szVendor );
		m_wndVendor.SetWindowTextW( szVendor );

		CString szPlatformVersion;
		szPlatformVersion.Format( L"%s", pDevice->GetPlatform()->m_szVersion );
		m_wndPlatformVersion.SetWindowTextW( szPlatformVersion );

		CString szMemorySize;
		szMemorySize.Format( L"%I64d mb (%I64d bytes)", (pDevice->GetGlobalMemorySize() / 1024 ) / 1024, pDevice->GetGlobalMemorySize( ) );
		m_wndGlobalMemory.SetWindowTextW( szMemorySize );

		CString szLocalMemory;
		szLocalMemory.Format( L"%I64d kb (%I64d bytes)", pDevice->GetLocalMemorySize() / 1024, pDevice->GetLocalMemorySize());
		m_wndLocalMemory.SetWindowTextW( szLocalMemory );

		CString szComputeUnits;
		szComputeUnits.Format( L"%d", pDevice->GetComputeUnits() );
		m_wndComputeUnits.SetWindowTextW( szComputeUnits );

		CString szWorkGroups;
		szWorkGroups.Format( L"%I64d", pDevice->GetMaxWorkGroups() );
		m_wndWorkGroups.SetWindowTextW( szWorkGroups );


		m_wndDeviceExtensions.ResetContent( );

		pDevice->ForExtensions( [ & ]( const auto szExtension ) { 
			m_wndDeviceExtensions.AddString( szExtension );
		} );
	}
}
