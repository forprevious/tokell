
# include "ellloader.h"

static int ell = 0 ;

typedef void (*ELLREGISTER ) ( int* address , int length ) ;
ELLREGISTER ellregister = 0 ;

typedef void (*ELLENTRY) () ;
ELLENTRY ellentry = 0 ;

unsigned char* ebuffer = 0 ;
int ebufferlength = 1024*4 ;

int main ( int argc , char* argv [] ) {

	ebuffer = (unsigned char* ) malloc ( ebufferlength ) ;

	if ( !ebuffer ) {
		EllLog ( "ell ebuffer is NULL\n" ) ;
		return 0 ;
	}

	//	ע��ebuffer����ell ���ص�������
	EllMemoryRegister ( ebuffer , ebufferlength ) ;

	//	����ell
	//	����1 : ָ�
	//	����2 : Ӧ�ó���(һ��Ŀ¼��ell��Ӧ�ó���Ϊ��λ�����أ�
	//	һ��ellӦ�ó������n��Ŀ���ļ����ɣ�ellͨ����̬����������ЩĿ���ļ���Ϊ��ִ��)
	ell  = EllInstall ( ELL_THUMB16_ROUTINE , "GTKINGS" ) ;

	//	ȡ�÷������(�������Ͳ����������ں�����ȫ�ֱ�����Ҳ���Է���)
	ellregister = (ELLREGISTER) EllGetSymbolEntry ( ell , "RomSendDataToEll" ) ;
	ellentry = (ELLENTRY) EllGetSymbolEntry ( ell  , "gtkings" ) ;

	EllLog ( "ell register %x\n" , ellregister ) ;
	EllLog ( "ell entry %x\n" , ellentry ) ;

	if ( ellregister ) {
		
		int apiaddress [4] = { 
		
			0 , 0 , 0 , 0

		} ;

		//	��ROMϵͳ��ӿڵ�ַ���ݸ�ell 
//		ellregister ( apiaddress , sizeof (apiaddress) ) ;

	}
	
//	if ( ellentry ) ellentry () ;

	EllUninstall ( ell ) ;
	//EllHalMemoryLeaked () ;

	return 1 ;

}
