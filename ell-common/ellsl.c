
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

# include "ellhal.h"
# include "ellsl.h"

int EllSlListCreate () {

	//	author : Jelo Wang
	//	since : 20100609
	//	(C)TOK

	//	notes : create a linked-list

	ELLSLLIST* list = (ELLSLLIST* ) EllMalloc ( sizeof(ELLSLLIST) ) ;

	if ( 0 == list ) return 0 ;

	list->element = 0 ;
	list->head = 0 ;
	list->next = 0 ;

	return (int)list ;

}

int EllSlListInsert ( int lt , int element ) {

	//	author : Jelo Wang
	//	since : 20100609
	//	(C)TOK

	//	notes : add a node

	ELLSLLIST* list = (ELLSLLIST* ) lt ;
	ELLSLLIST* newn = 0 ;

	if ( 0 == list ) return 0 ;

	newn = (ELLSLLIST* ) EllMalloc ( sizeof(ELLSLLIST) ) ;

	if ( 0 == newn ) return 0 ;

	newn->element = element ;
	newn->head = 0 ;
	newn->next = 0 ;

	if ( 0 == list->head ) {
		list->head = newn ;
		list->next = newn ;
	} else {
		list->next->next = newn ;
		list->next = newn ;
	}
	
	return 1 ;

}

int EllSlListDelete ( int lt , int element ) {

	//	author : Jelo Wang
	//	since : 20100609
	//	(C)TOK

	//	notes : delete a node	

	ELLSLLIST* list = (ELLSLLIST* ) lt ;
	ELLSLLIST* walker = 0 ;
	ELLSLLIST* pre = 0 ;

	if ( 0 == list ) return 0 ;

	for ( walker = list->head , pre = walker ; walker && walker->element != element ; pre = walker , walker = walker->next ) ;

	if ( walker ) {
		pre->next = walker->next ; 
		EllFree ( walker ) ;
	}

	return 1 ;

}

int EllSlListDestroy ( int lt ) {

	//	author : Jelo Wang
	//	since : 20100609
	//	(C)TOK

	//	notes : destroy a list

	ELLSLLIST* list = (ELLSLLIST* ) lt ;
	ELLSLLIST* walker = 0 ;

	if ( 0 == list ) return 1 ;

	for ( walker = list->head ; walker ; ) {

		list->next = walker->next ; 
		EllFree ( walker ) ;
		walker = list->next ; 

	}

	EllFree ( list ) ;

	return 1 ;

}


int EllSlListSetIterator ( int lt , int position ) {

	//	author : Jelo Wang
	//	since : 20100609
	//	(C)TOK

	//	notes : set an iterator

	ELLSLLIST* list = (ELLSLLIST* ) lt ;
	ELLSLLIST* walker = 0 ;

	if ( 0 == list ) return 0 ;
	
	if ( ELLSLSEEK_HEAD == position ) {
		//	将迭代器指向头节点
		list->next = list->head ; 
	} else if ( ELLSLSEEK_TAIL == position ) {
		//	将迭代器指向尾节点
		for ( walker = list->head ; walker && walker->next ; walker = walker->next ) ;
		if ( walker ) list->next = walker ;
	}

	return 1 ; 

}

int EllSlListIteratorPermit ( int lt ) {

	//	author : Jelo Wang
	//	since : 20100609
	//	(C)TOK

	//	notes : 判断一个迭代器是否可以继续运行

	ELLSLLIST* list = (ELLSLLIST* ) lt ;
	ELLSLLIST* walker = 0 ;

	if ( 0 == list ) return 0 ;
	if ( 0 == list->next ) return 0 ;

	return 1 ; 

}

int EllSlListIteratorGetElement ( int lt ) {

	//	author : Jelo Wang
	//	since : 20100609
	//	(C)TOK

	//	notes : get an element

	ELLSLLIST* list = (ELLSLLIST* ) lt ;

	if ( 0 == list ) return 0 ;
	if ( 0 == list->next ) return 0 ;

	return list->next->element ;

}

int EllSlListIteratorNext ( int lt ) {

	//	author : Jelo Wang
	//	since : 20100609
	//	(C)TOK

	//	notes : iterator next

	ELLSLLIST* list = (ELLSLLIST* ) lt ;

	if ( 0 == list ) return 0 ;
	if ( 0 == list->next ) return 0 ;

	list->next = list->next->next ; 
	
	return 1 ;

}

int EllSlGetString ( int file , int position , char* name ) {
	
	//	author : Jelo Wang
	//	since : 20100624
	//	(C)TOK

	//	notes : get string from file
	
	int walker = 0 ;
	char swap ;

	if( !file ) return 0 ;

	EllHalFileSeek ( file , position , ELLHAL_SEEK_HEAD ) ;

# ifdef MTK_ELL

	for(  ; ; walker++ ) {
	
		EllHalFileRead ( file , &swap , 1 , 1 ) ;

		if( 0x20 == swap || 0 == swap ) break ;
		
		name[walker] = swap ;
	
	}

# else

	for(  ; !EllHalFileEnd ( file ) ; walker++ ) {

		EllHalFileRead ( file , &swap , 1 , 1 ) ;

		if( 0x20 == swap || 0 == swap ) break ;
		
		name[walker] = swap ;

	}

# endif

	name[walker] = 0 ;

	return 1 ;

}



