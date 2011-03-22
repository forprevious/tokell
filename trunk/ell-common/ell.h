
/*

+	Executable Linking-Library 1.0.0.
+	Architecture : ARMv

+	You can redistribute it and/or modify it under the terms of the gnu general public license	
+	as published by the free software foundation, either version 3 of the license or any later 	
+	version.this program is distributed in the hope	that it will be useful,but without any 		
+	warranty.without even the implied warranty of merchantability or fitness for a particular 	
+	purpose.																					
																												
+	(c)	Techniques of Knowledge
+		an open source group since 2008
+		page : http://www.tok.cc
+		email : wqw85@sina.com

+		技术支持、功能扩展、平台搭建。
+		欢迎联系我们。

*/

# ifndef __ELL
# define __ELL

# include "elf.h"
# include <string.h>
# include <stdarg.h> 
# include <stdio.h> 

//# define ELL_DEBUG

typedef struct {

	//	author : WANG QUANWEI

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

# endif

extern int EllDynamicPoolCreate () ;
extern void EllDynamicPoolSetCurrent ( int ella ) ;
extern int EllDynamicPoolInsertApplication ( char* application ) ;
extern int EllDynamicPoolCreateSymbols ( int obid , int totall ) ;
extern int EllDynamicPoolGetLborder ( int obid ) ;
extern int EllDynamicPoolInsertSymbol ( int obid , void* buffer , char* name , int index ) ;
extern Elf32_Sym* EllDynamicPoolLocalGetSymbol ( int obid , int info ) ;
extern Elf32_Sym* EllDynamicPoolGlobalGetSymbol ( int obidborder , char* name ) ;
extern int EllDynamicPoolGetSymbolEntry ( char* name ) ;
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
extern void EllDump ( char* path , void* buffer , int length ) ;
extern void EllLog ( const   char* message , ... ) ;

