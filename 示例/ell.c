
typedef void (*ELLLOG)( const char* message , ... ) ;
ELLLOG Elllog = 0 ;

void EllRegister ( int* address , int length ) {
	
	//	������ROMϵͳ�㴫�ݹ����ĺ�����ַ
	Elllog = (ELLLOG ) (address[0]) ;

}

void EllRender ( short* buffer , int width , int height ) {
	
	int x = 0 ;

	Elllog ( "EllRender - buffer %x , width %d , height %d" , buffer , width , height ) ;

	if ( !buffer ) return ;

	for ( x = 0 ; x < width*height ; x ++ ) {
		buffer [x] = 0 ;
	}

}

void EllKeypad ( int key , int cases ) {
}

void EllMouse ( int cases , int x , int y ) {
}

void EllEntry () {
}