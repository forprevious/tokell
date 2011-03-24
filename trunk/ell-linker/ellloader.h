
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

# ifndef __ELLLOADER
# define __ELLLOADER

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
	
	ELL_ARM32_ROUTINE ,
	ELL_THUMB16_ROUTINE ,

} ELLROUTINE ; 

# endif  

extern void EllMemoryRegister ( void* buffer,  int length ) ;
extern int EllInstall ( int routineset , int elltype , char* application ) ;
extern int EllGetSymbolEntry ( int ell , char* symbol ) ;
extern void EllUninstall ( int ell ) ;

