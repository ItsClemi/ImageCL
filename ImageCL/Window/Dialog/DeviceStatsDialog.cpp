#include "stdafx.h"
#include "DeviceStatsDialog.h"


BEGIN_MESSAGE_MAP( CDeviceStatsDialog, CDialogEx )
	ON_LBN_SELCHANGE( IDC_DEVICE_LIST,  &CDeviceStatsDialog::OnChangeDevice )

END_MESSAGE_MAP( )


CDeviceStatsDialog::CDeviceStatsDialog( )
	: CDialogEx( IDD_DEVICE_STATS )
{

}

BOOL CDeviceStatsDialog::OnInitDialog( )
{
	if( !CDialogEx::OnInitDialog( ) )
	{
		return FALSE;
	}

	SetIcon( AfxGetApp( )->LoadIconW( IDR_MAINFRAME ), FALSE );

	return TRUE;
}

void CDeviceStatsDialog::DoDataExchange( CDataExchange* pDX )
{
	CDialogEx::DoDataExchange( pDX );


	DDX_Control( pDX, IDC_DEVICE_LIST, m_wndDeviceList );

	DDX_Control( pDX, IDC_DEVICE_NAME, m_wndDeviceName );
	DDX_Control( pDX, IDC_DEVICE_TYPE, m_wndDeviceType );
	DDX_Control( pDX, IDC_DEVICE_VENDOR, m_wndVendor );
	DDX_Control( pDX, IDC_DEVICE_COMPUTE_UNITS, m_wndComputeUnits );
	DDX_Control( pDX, IDC_DEVICE_GLOBAL_MEMORY, m_wndGlobalMemory );
	DDX_Control( pDX, IDC_DEVICE_LOCAL_MEMORY, m_wndLocalMemory );
	DDX_Control( pDX, IDC_DEVICE_WORK_GROUPS, m_wndWorkGroups );
	DDX_Control( pDX, IDC_DEVICE_EXTENSIONS, m_wndDeviceExtensions );

	gEnv->pClManger->ForDevices( [ & ]( const CCLDevice* pDevice ) { 
		m_wndDeviceList.AddString( pDevice->GetName( ).c_str( ) );
	} );
}

void CDeviceStatsDialog::OnChangeDevice( )
{
	int nSel = m_wndDeviceList.GetCurSel( );

	if( nSel != LB_ERR )
	{
		const auto pDevice = gEnv->pClManger->GetDeviceAt( static_cast< size_t >( nSel ) );

		m_wndDeviceName.SetWindowTextW(  pDevice->GetName( ).c_str( ) );
		m_wndDeviceType.SetWindowTextW( pDevice->GetTypeString( ) );
		m_wndVendor.SetWindowTextW( pDevice->GetParentPlatform( )->GetVendor( ).c_str( ) );

		m_wndComputeUnits.SetWindowTextW( std::to_wstring( pDevice->GetMaxComputeUnits( ) ).c_str( ) );
		m_wndGlobalMemory.SetWindowTextW( std::to_wstring( pDevice->GetGlobalMemorySize( ) ).c_str( ) );
		m_wndLocalMemory.SetWindowTextW( std::to_wstring( pDevice->GetLocalMemorySize( ) ).c_str( ) );
		m_wndWorkGroups.SetWindowTextW( std::to_wstring( pDevice->GetMaxWorkGroupSize( ) ).c_str( ) );

		m_wndDeviceExtensions.ResetContent( );

		pDevice->ForDeviceExtension( [ & ]( const std::wstring& szExtension) { 
			m_wndDeviceExtensions.AddString( szExtension.c_str( ) );
		} );
	}
}
