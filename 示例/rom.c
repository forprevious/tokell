
int ell = 0 ;

typedef void (*ELLREGISTER ) ( int* address , int length ) ;
ELLREGISTER ellregister = 0 ;

typedef void (*ELLENTRY) () ;
ELLENTRY ellentry = 0 ;

unsigned char* ebuffer = 0 ;
int ebufferlength = 1024*4 ;

void Kernal (void)
{

	ebuffer = (unsigned char* ) malloc ( ebufferlength ) ;

	if ( !ebuffer ) {
		EllLog ( "ell ebuffer is NULL\n" ) ;
		return ;
	}

	//	ע��ebuffer����ell ���ص�������
	EllMemoryRegister ( ebuffer , ebufferlength ) ;

	//	����ell
	//	����1 : ָ�
	//	����2 : Ӧ�ó���(һ��Ŀ¼��ell��Ӧ�ó���Ϊ��λ�����أ�
	//	һ��ellӦ�ó������n��Ŀ���ļ����ɣ�ellͨ����̬����������ЩĿ���ļ���Ϊ��ִ��)
	ell = EllInstall ( ELL_ARM32 , ELL_DYNAMIC , "GTKINGS" ) ;

	//	ȡ�÷������(�������Ͳ����������ں�����ȫ�ֱ�����Ҳ���Է���)
	ellregister = (ELLREGISTER) EllGetSymbolEntry ( ell , "EllRegister" ) ;
	ellentry = (ELLENTRY) EllGetSymbolEntry ( ell , "EllEntry" ) ;

	EllLog ( "ell register %x\n" , ellregister ) ;
	EllLog ( "ell entry %x\n" , ellentry ) ;

	if ( ellregister ) {
		
		int apiaddress [1] = {
			
			(int)EllLog ,

		} ;

		//	��ROMϵͳ��ӿڵ�ַ���ݸ�ell 
		ellregister ( apiaddress , sizeof (apiaddress) ) ;

	}
	
	if ( ellentry ) ellentry () ;

}
