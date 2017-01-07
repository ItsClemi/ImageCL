

//I = 0.299* R + 0.587* G + 0.114* B

__kernel void histogram_kernel( 
	__global uchar4* in, 
	__global uchar* outLuminosity, 
	__global uchar* outRed, 
	__global uchar* outGreen, 
	__gloabl uchar* outBlue  
)
{
    const int gid = get_global_id( 0 );
    const char4 px = in[ gid ];

	const int nLuminosity = ( int )( 0.299f * px.r + 0.587f * px.g + 0.114f * px.b );
    
    
	atom_inc( &outLuminosit[ nLuminosity ] );

	atom_inc( &outRed[ px.r ] );
	atom_inc( &outGreen px.g ] );
	atom_inc( &putBlue[ px.b ] );
}
