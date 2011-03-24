
/*

+	Executable Linking-Library 1.0.1.
+	Architecture : ARMv6

+	'Executable Linking-Library' is a Dynamic Linking solution for closed runing environment.
+	The project lunched by Jelo Wang since 2010 from 'Techniques of Knowledge' community. 

+	You can redistribute it and/or modify it under the terms of the gnu general public version 3 of 
+	the license as published by the free software foundation.this program is distributed in the hope 
+	that it will be useful,but without any warranty.without even the implied warranty of merchantability 
+	or fitness for a particular purpose.																					
																												
+	(C)	突壳开源Techniques of Knowledge
+		an open source community since 2008
+		Community : http://www.tok.cc
+		Contact Us : jelo.wang@gmail.com

+		技术支持、功能扩展、平台搭建，欢迎与我们联系。
+		我们将为您提供有偿的，强力的服务。

*/

# include "ell.h"
# include "ellsl.h"
# include "ellhal.h"
# include "ellrevor.h"
# include "ellloader.h"
# include "elllinker.h"

void EllMemoryRegister ( void* buffer , int length ) {

	//	author : Jelo Wang
	//	register a memory pool for ell loading & linking & run
	//	(C)TOK

	EllLinkerMemoryPool.pool = (unsigned char* ) buffer ;
	EllLinkerMemoryPool.base = (int ) buffer ;

	EllLinkerMemoryPool.looper = 0 ;
	EllLinkerMemoryPool.length = length ;

}

int EllInstall ( int routineset , int elltype , char* application ) {

	//	author : Jelo Wang
	//	notes : load elf files and linking
	//	(C)TOK	

	int obid = 0 ;
	int walker = 0 ;
	int results = 1 ;

	ELL* ell = (ELL* )EllDynamicPoolCreate () ;
	EllDynamicPoolSetCurrent ( (int)ell ) ;
	if ( !EllDynamicPoolInsertApplication ( application ) ) return 0 ;

	EllLog ( "EllInstall start\n" ) ;
	
	EllLinker.routineset = routineset ;
	
	for ( EllSlListSetIterator ( ell->ObjectList , ELLSLSEEK_HEAD ) ; EllSlListIteratorPermit  ( ell->ObjectList ) ; EllSlListIteratorNext ( ell->ObjectList ) ) {
		
		int ER_RW_Rel = 0 ;
		char* objectpath = (char*) EllSlListIteratorGetElement ( ell->ObjectList ) ;

		switch ( elltype ) {
			
			case ELL_STATIC :
				results = EllResolver ( obid , objectpath , &ER_RW_Rel ) ;				
				if ( 0 == results ) continue ;		
				results = EllLocalLinkerStatic ( obid , results , ER_RW_Rel ) ;				
				if ( 0 == results ) continue ;					
				obid ++ ;
			break ;

			//	dynamic linking
			case ELL_DYNAMIC :
				results = EllResolverEx ( obid , objectpath ) ;				
				if ( 0 == results ) continue ;				
				results = EllLocalLinker ( obid , results ) ;				
				if ( 0 == results ) continue ;				
				obid ++ ;
			break ;

		}
		
	}
	EllSlListDestroy ( ell->ObjectList ) ;

	//	dynamic linking
	if ( ELL_DYNAMIC == elltype ) { 

		//	link all of the object-files
		EllGlobalLinker ( obid ) ;
		
		EllElfMapNolSectDestroy ( EllLinker.obidborder ) ;
		EllElfMapRelocDestroy ( ell->TextRel.elf32_rel , EllLinker.obidborder ) ;
		EllElfMapRelocDestroy ( ell->DataRel.elf32_rel , EllLinker.obidborder ) ;
		EllElfMapRelocRelaDestroy ( ell->TextRela.elf32_rela , EllLinker.obidborder ) ;
		EllElfMapRelocRelaDestroy ( ell->DataRela.elf32_rela , EllLinker.obidborder ) ;	
		EllFreeEx ((void**)&ell->ObjectBased) ;

	}

	if ( !results ) EllDynamicPoolDestroy () ;
	
	EllDump ( "e:\\ellmem.elle" , (void*)EllLinkerMemoryPool.pool , EllLinkerMemoryPool.looper ) ;

	EllLog ( "EllInstall end\n" ) ;
	
	return (int)ell ;
	
}

int EllGetSymbolEntry ( int ell , char* symbol ) {

	//	author : Jelo Wang
	//	get symbol entry
	//	(C)TOK

	
	int address = 0 ;

	EllDynamicPoolSetCurrent ( ell ) ;
	address = EllDynamicPoolGetSymbolEntry ( symbol ) ;

	EllLog ( "EllGetSymbolEntry %d\n" , address ) ;
	EllLog ( "EllGetSymbolEntry %d\n" , EllLinkerMemoryPool.base ) ;
	
	if ( -1 != address ) { 

		if ( ELL_THUMB16_ROUTINE == EllLinker.routineset )
			address = address + EllLinkerMemoryPool.base + 1 ;
		else if ( ELL_ARM32_ROUTINE == EllLinker.routineset )
			address = address + EllLinkerMemoryPool.base ;
		
		return address ;

	} else return 0 ;

}

void EllUninstall ( int ell ) {

	//	author : Jelo Wang
	//	(C)TOK

	EllDynamicPoolSetCurrent ( ell ) ;
	EllDynamicPoolDestroy () ;
	
}

