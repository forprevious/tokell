
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

+		����֧�֡�������չ��ƽ̨���
+		��ӭ��ϵ���ǡ�

*/

# ifndef __ELLLOADER
# define __ELLLOADER

typedef enum {

	//	author : WANG QUANWEI
	//	since : 20100626
	//	(C)TOK
	
	ELL_ARM32_ROUTINE ,
	ELL_THUMB16_ROUTINE ,

} ELLROUTINE ;

# endif  

extern void EllMemoryRegister ( void* buffer,  int length ) ;
extern int EllInstall ( int routineset , char* application ) ;
extern int EllGetSymbolEntry ( int ell , char* symbol ) ;
extern void EllUninstall ( int ell ) ;

