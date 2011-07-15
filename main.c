
# include "ellhal.h"
# include "ellloader.h"
# include "ell.h"

static int elllibrary = 0 ;

typedef void (*ELLREGISTER ) ( int* address , int length ) ;
ELLREGISTER ellregister = 0 ;

typedef void (*ELLENTRY) () ;
ELLENTRY ellentry = 0 ;

unsigned char* ebuffer = 0 ;
int ebufferlength = 1024*1024 ;

int main ( int argc , char* argv [] ) {

	ebuffer = (unsigned char* ) EllMalloc ( ebufferlength ) ;

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
	elllibrary  = EllInstall ( ELL_THUMB16_ROUTINE , "GTKINGS" ) ;

	//	ȡ�÷������(�������Ͳ����������ں�����ȫ�ֱ�����Ҳ���Է���)
	ellregister = (ELLREGISTER) EllGetSymbolEntry ( elllibrary , "RomSendDataToEll" ) ;
	ellentry = (ELLENTRY) EllGetSymbolEntry ( elllibrary  , "gtkings" ) ;

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

	EllUninstall ( elllibrary ) ;
	MMTCheckOverflow () ;
	MMTCheckLeaked () ;

	return 1 ;

}
