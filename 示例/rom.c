
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

	//	注册ebuffer，将ell 加载到该区域
	EllMemoryRegister ( ebuffer , ebufferlength ) ;

	//	加载ell
	//	参数1 : 指令集
	//	参数2 : 应用程序(一个目录，ell以应用程序为单位被加载，
	//	一个ell应用程序可有n个目标文件构成，ell通过动态连接器将这些目标文件变为可执行)
	ell = EllInstall ( ELL_ARM32 , ELL_DYNAMIC , "GTKINGS" ) ;

	//	取得符号入口(符号类型不仅仅局限在函数，全局变量等也可以访问)
	ellregister = (ELLREGISTER) EllGetSymbolEntry ( ell , "EllRegister" ) ;
	ellentry = (ELLENTRY) EllGetSymbolEntry ( ell , "EllEntry" ) ;

	EllLog ( "ell register %x\n" , ellregister ) ;
	EllLog ( "ell entry %x\n" , ellentry ) ;

	if ( ellregister ) {
		
		int apiaddress [1] = {
			
			(int)EllLog ,

		} ;

		//	将ROM系统层接口地址传递给ell 
		ellregister ( apiaddress , sizeof (apiaddress) ) ;

	}
	
	if ( ellentry ) ellentry () ;

}
