
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

# ifndef __ELLSL
# define __ELLSL

typedef struct KCN {

	//	author : WANG QUANWEI
	//	since : 20100609
	//	(C)TOK

	//	notes : list

	int element ;
	
	struct KCN* head ;
	struct KCN* next ;

} ELLSLLIST ;

typedef enum {

	//	author : WANG QUANWEI
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

