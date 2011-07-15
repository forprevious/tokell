
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

	//	注册ebuffer，将ell 加载到该区域
	EllMemoryRegister ( ebuffer , ebufferlength ) ;

	//	加载ell
	//	参数1 : 指令集
	//	参数2 : 应用程序(一个目录，ell以应用程序为单位被加载，
	//	一个ell应用程序可有n个目标文件构成，ell通过动态连接器将这些目标文件变为可执行)
	elllibrary  = EllInstall ( ELL_THUMB16_ROUTINE , "GTKINGS" ) ;

	//	取得符号入口(符号类型不仅仅局限在函数，全局变量等也可以访问)
	ellregister = (ELLREGISTER) EllGetSymbolEntry ( elllibrary , "RomSendDataToEll" ) ;
	ellentry = (ELLENTRY) EllGetSymbolEntry ( elllibrary  , "gtkings" ) ;

	EllLog ( "ell register %x\n" , ellregister ) ;
	EllLog ( "ell entry %x\n" , ellentry ) ;

	if ( ellregister ) {
		
		int apiaddress [4] = { 
		
			0 , 0 , 0 , 0

		} ;

		//	将ROM系统层接口地址传递给ell 
//		ellregister ( apiaddress , sizeof (apiaddress) ) ;

	}
	
//	if ( ellentry ) ellentry () ;

	EllUninstall ( elllibrary ) ;
	MMTCheckOverflow () ;
	MMTCheckLeaked () ;

	return 1 ;

}
