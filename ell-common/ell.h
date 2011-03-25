
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

# ifndef __ELL
# define __ELL

# include "elf.h"
# include <string.h>
# include <stdarg.h> 
# include <stdio.h> 

//	for MTK system
//# define MTK_ELL

//# define ELL_DEBUG

typedef enum {

	//	author : Jelo Wang
	//	since : 20110324
	//	(C)TOK

	//	stataic linking
	//	load executable ELF
	ELL_STATIC ,
	//	dynamic linking
	//	load relocatable ELF
	ELL_DYNAMIC,

} ELLTYPE ;

typedef enum {

	//	author : Jelo Wang
	//	since : 20100626
	//	(C)TOK
	
	ELL_ARM32 ,
	ELL_THUMB16 ,

} ELLROUTINE ; 

typedef struct {

	//	author : Jelo Wang

	//	dynamic or static
	int type ;
	//	instruction sets , value between ELL_ARM32_ROUTINE and ELL_THUMB16_ROUTINE
	int set ;
	
	int ObjectList ;
	int ObjTotall ;
	int* ObjectBased ;

	struct {
		Elf32_Sym** elf32_sym ;
	} Sym ;
	
	struct {
		Elf32_Shdr** elf32_shdr ;
	} Shdr ;

	struct {
		Elf32_Rel** elf32_rel ;
	} TextRel ;
	
	struct {
		Elf32_Rel** elf32_rel ;
	} DataRel ;

	struct {
		Elf32_Rela** elf32_rela ;
	} TextRela ;
	
	struct {
		Elf32_Rela** elf32_rela ;
	} DataRela ;	

} ELL ;

extern ELL* ell ;

#define ELL_4BYTES_ALIGN(address)\
	while ( address % 4 ) address ++ ;\

# endif

extern int EllDynamicPoolCreate ( int elltype , int routineset ) ;
extern void EllDynamicPoolSetCurrent ( int ella ) ;
extern int EllDynamicPoolInsertApplication ( char* application ) ;
extern int EllDynamicPoolCreateSymbols ( int obid , int totall ) ;
extern int EllDynamicPoolGetLborder ( int obid ) ;
extern int EllDynamicPoolInsertSymbol ( int obid , void* buffer , char* name , int index ) ;
extern Elf32_Sym* EllDynamicPoolLocalGetSymbol ( int obid , int info ) ;
extern Elf32_Sym* EllDynamicPoolGlobalGetSymbol ( int obidborder , char* name ) ;
extern int EllDynamicPoolGetSymbolEntry ( char* name ) ;
extern int EllElfMapCheckHeader ( char* value ) ;
extern int EllDynamicPoolDestroy () ;
extern int EllGetAllObjectFileFromDirectory ( int list , char* application ) ;
extern int EllElfMapNolSectCreate ( int obid , int totall ) ;
extern int EllElfMapNolSectInsert ( int obid , int index , void* buffer , const char* name ) ;
extern int EllElfMapNolSectGet ( int obid , char* name ) ;
extern int EllElfMapNolSectGetWithIndex ( int obid , int index ) ;
extern int EllElfMapNolSectGetLborder ( int obid ) ;
extern void EllElfMapNolSectDestroy ( int obidborder ) ;
extern int EllElfMapRelocRelCreate ( Elf32_Rel** elf32_rel , int obid , int totall ) ;
extern int EllElfMapRelocRelInsert ( Elf32_Rel** elf32_rel , int obid , void* buffer ) ;
extern int EllElfMapRelocGetLborder ( Elf32_Rel** elf32_rel , int obid ) ; 
extern void EllElfMapRelocDestroy ( Elf32_Rel** elf32_rel , int obidborder ) ;
extern void EllElfMapRelocRelaDestroy ( Elf32_Rela** elf32_rela , int obidborder ) ;
extern char* EllWStrcpy ( signed char* strDestination , const signed char* strSource ) ;
extern int EllUnicodeToAscii ( unsigned char* pOutBuffer , signed char* pInBuffer ) ;
extern void EllAsciiToUnicode ( char *outbuffer , char *inbuffer ) ;
extern void EllDump ( char* path , void* buffer , int length ) ;
extern void EllLog ( const   char* message , ... ) ;

extern void EllMemoryRegister ( void* buffer,  int length ) ;
extern int EllInstall ( int routineset , int elltype , char* application ) ;
extern int EllGetSymbolEntry ( int ell , char* symbol ) ;
extern void EllUninstall ( int ell ) ;

