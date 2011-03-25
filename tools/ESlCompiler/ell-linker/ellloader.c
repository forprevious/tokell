
/*

+	Executable Linking-Library 1.0.0.
+	Architecture : ARMv6

+	'Executable Linking-Library' is a Dynamic Linking solution for closed runing environment.
+	The project lunched by Jelo Wang since 2010 from 'Techniques of Knowledge' community. 

+	You can redistribute it and/or modify it under the terms of the gnu general public version 3 of 
+	the license as published by the free software foundation.this program is distributed in the hope 
+	that it will be useful,but without any warranty.without even the implied warranty of merchantability 
+	or fitness for a particular purpose.																					
																												
+	(C)	ͻ�ǿ�ԴTechniques of Knowledge
+		an open source community since 2008
+		Community : http://www.tok.cc
+		Contact Us : jelo.wang@gmail.com

+		����֧�֡�������չ��ƽ̨�����ӭ��������ϵ��
+		���ǽ�Ϊ���ṩ�г��ģ�ǿ���ķ���

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

int EllInstall ( int file , int file2 ) {

	//	author : Jelo Wang
	//	notes : load elf files and linking
	//	(C)TOK	

	int obid = 0 ;
	int walker = 0 ;
	int results = 1 ;

	ELL* ell = (ELL* )EllDynamicPoolCreate () ;
	EllDynamicPoolSetCurrent ( (int)ell ) ;
	if ( !EllDynamicPoolInsertApplication ( "TOK" ) ) return 0 ;
	
	EllResolver ( 0 , file , file2 ) ;

	EllElfMapNolSectDestroy ( EllLinker.obidborder ) ;
	EllElfMapRelocDestroy ( ell->TextRel.elf32_rel , EllLinker.obidborder ) ;
	EllElfMapRelocDestroy ( ell->DataRel.elf32_rel , EllLinker.obidborder ) ;
	EllElfMapRelocRelaDestroy ( ell->TextRela.elf32_rela , EllLinker.obidborder ) ;
	EllElfMapRelocRelaDestroy ( ell->DataRela.elf32_rela , EllLinker.obidborder ) ;	
	EllFreeEx ((void**)&ell->ObjectBased) ;
	
	return 1;
	
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
