
/*

+	Executable Linking-Library 1.0.0.
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

# ifndef __ELLSL
# define __ELLSL

typedef struct KCN {

	//	author : Jelo Wang
	//	since : 20100609
	//	(C)TOK

	//	notes : list

	int element ;
	
	struct KCN* head ;
	struct KCN* next ;

} ELLSLLIST ;

typedef enum {

	//	author : Jelo Wang
	//	since : 20100609
	//	(C)TOK

	//	notes : seek

	ELLSLSEEK_HEAD ,
	ELLSLSEEK_TAIL ,

} ELLSLSEEK ;

# endif

extern int EllSlListCreate () ;
extern int EllSlListInsert ( int lt , int element ) ;
extern int EllSlListDelete ( int lt , int element ) ;
extern int EllSlListDestroy ( int lt ) ;
extern int EllSlListSetIterator ( int lt , int position ) ;
extern int EllSlListIteratorPermit ( int lt ) ;
extern int EllSlListIteratorGetElement ( int lt ) ; 
extern int EllSlListIteratorNext ( int lt ) ;
extern int EllSlGetString ( int file , int position , char* name ) ;

