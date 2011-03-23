
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

# ifndef __ELLHAL
# define __ELLHAL

typedef enum {

	//	author : Jelo Wang
	//	since : 20100703
	//	(C)TOK	

	//	seek
	//	opening of read only
	//	opening of create and write
	
	ELLHAL_SEEK_HEAD ,
	ELLHAL_READ_OPEN ,
	ELLHAL_CREW_OPEN ,

} ELLHAL_FILE ;

//# define MEMORY_MONITOR_ENABLE

extern void* EllNormalloc ( long int length , char* file , int line )  ;

# define EllMalloc(length) EllNormalloc(length,__FILE__,__LINE__)

typedef struct MMN {

	//	author : Jelo Wang
	//	since : 20100417
	//	(C)TOK
	
	char* file ;
	int line ;
	int length ;
	int address ;
	
	struct MMN* head ;
	struct MMN* next ;
	
} MEMORY_MONITOR ;

# endif

extern int EllFree ( void* buffer ) ;
extern int EllFreeEx ( void** buffer ) ;
extern int EllHalMemoryLeaked () ;
extern int EllHalFileOpen ( char* path , int flag ) ;
extern int EllHalFileSeek( int file , int offset , int direct ) ;
extern int EllHalFileRead ( int file , void* buffer , int size , int counter ) ;
extern int EllHalFileWrite ( int file , void* buffer , int size , int counter ) ;
extern int EllHalFileEnd ( int file ) ;
extern int EllHalFileLength( int file ) ;
extern int EllHalFileClose ( int file ) ;
extern void EllMemcpy ( void* target_memory , void* source_memory , int length ) ;
