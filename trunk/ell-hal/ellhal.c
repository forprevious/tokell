
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

# include "ell.h"
# include "ellhal.h"

# define EllMALLOC malloc
# define EllFREE free

# ifdef MEMORY_MONITOR_ENABLE
MEMORY_MONITOR mem_monitor = { 0 , 0 , 0 , 0 , 0 , 0 } ;
int MemoryMonitorInit ( MEMORY_MONITOR* mem_monitor ) {

	//	author : WANG QUANWEI
	//	since : 20100418
	//	(C)TOK

	if ( !mem_monitor ) return 0 ; 
	
	mem_monitor->file = 0 ;
	mem_monitor->line = 0 ;
	mem_monitor->length = 0 ;
	mem_monitor->address = 0 ;	
	mem_monitor->head = 0 ;		
	mem_monitor->next = 0 ;

	return (int)mem_monitor ;
		
}

void MemoryMonitorAdd ( MEMORY_MONITOR* mem_monitor , char* file , int line , int length , int address ) {
	
	//	author : WANG QUANWEI
	//	since : 20100418
	//	(C)TOK

	MEMORY_MONITOR* newmem = 0 ;
	
	if ( !mem_monitor ) return ;

	newmem	= (MEMORY_MONITOR* ) EllMALLOC ( sizeof(MEMORY_MONITOR) ) ;

	if ( file ) {
		newmem->file = (char* ) EllMALLOC ( strlen ( file ) + 1 ) ;
		strcpy ( newmem->file , file ) ;
	}

	newmem->line = line ; 
	newmem->length = length ;
	newmem->address = address ;
	newmem->head = 0 ;
	newmem->next = 0 ;

	if ( 0 == mem_monitor->head ) {
		mem_monitor->head = newmem ;
		mem_monitor->next = newmem ;
	} else {
		mem_monitor->next->next = newmem ;
		mem_monitor->next = newmem ;
	}

}

void MemoryMonitorFree ( MEMORY_MONITOR* mem_monitor , int address ) {
	
	//	author : WANG QUANWEI
	//	since : 20100418
	//	(C)TOK

	MEMORY_MONITOR* walker = mem_monitor->head ;
	MEMORY_MONITOR* pre = mem_monitor ;
	
	if ( !address ) return ;

	for ( ;walker && walker->address != address ;pre = walker , walker=walker->next) ;

	if ( walker ) {
		
		pre->next = walker->next ; 		
		if ( 0 == pre->next ) mem_monitor->next = pre ;
		EllFREE ( walker ) ;
		
	}
	
}

void MemoryMonitorDestroy ( MEMORY_MONITOR* mem_monitor ) {
	
	//	author : WANG QUANWEI
	//	since : 20100418
	//	(C)TOK

	MEMORY_MONITOR* walker = 0 ;
	
	if ( !mem_monitor ) return ;

	for ( walker = mem_monitor->head ; walker ; ) {
		mem_monitor->next = walker->next ;
		if ( walker->file ) EllFREE ( walker->file ) ;
		EllFREE ( walker ) ;
		walker = mem_monitor->next ;
	}
	
}

int EllHalMemoryLeaked () {

	//	author : WANG QUANWEI
	//	since : 20091129

	# ifdef MEMORY_MONITOR_ENABLE
		
		int totall = 0 ;
		MEMORY_MONITOR* walker = mem_monitor.head ;
	
		for ( ;walker;walker=walker->next) {

			EllLog ("address:%x,length:%d,file:'%s',line:%d - leaked\n",
				walker->address,\
				walker->length,\
				walker->file,\
				walker->line\
			) ;\

			
			totall = totall + walker->length ;
			
		}

		MemoryMonitorDestroy ( &mem_monitor ) ;

		//	EllLog ("%1.3f kb memory is leaked.\n",(float)(totall/1024)) ;
		EllLog ("%d memory is leaked.\n",totall) ;

	# endif

	return 0 ;
}

# endif

void* EllNormalloc ( long int length , char* file , int line ) {

	//	author : WANG QUANWEI
	//	notes : malloc
	//	since : 20090809
	
	void* buffer = 0 ;

	if ( !length ) 
		return 0 ;
	
	buffer = (void*) EllMALLOC ( length ) ;

	if ( buffer ) memset ( buffer , 0 , length ) ;

	# ifdef MEMORY_MONITOR_ENABLE
		MemoryMonitorAdd ( &mem_monitor , file , line , length , (int)buffer ) ;
	# endif
	
	return buffer ;

}

int EllFree ( void* buffer ) {

	//	author : WANG QUANWEI
	//	notes : free
	//	since : 20090809

	# ifdef MEMORY_MONITOR_ENABLE
		MemoryMonitorFree ( &mem_monitor , (int)buffer ) ;
	# endif

 	EllFREE ( buffer ) ;

	return 1 ;

}


int EllFreeEx ( void** buffer ) {

	//	author : WANG QUANWEI
	//	notes : free
	//	since : 20090809

	# ifdef MEMORY_MONITOR_ENABLE
		MemoryMonitorFree ( &mem_monitor , (int)*buffer ) ;
	# endif

 	EllFREE ( *buffer ) ;

	*buffer = 0 ;

	return 1 ;

}

int EllHalFileOpen ( char* path , int flag ) {

	//	author : WANG QUANWEI
	//	notes : fopen
	//	since : 20090809


	switch ( flag ) {

		case ELLHAL_READ_OPEN : return (int) fopen ( path , "rb" ) ; 
		case ELLHAL_CREW_OPEN : return (int) fopen ( path , "wb" ) ; 		

	}

	return 0 ;
	
}

int EllHalFileSeek ( int file , int offset , int direct ) {

	//	author : WANG QUANWEI
	//	notes : fseek
	//	since : 20090809

	switch ( direct ) {
		
		case ELLHAL_SEEK_HEAD : return fseek ( (FILE*)file , offset , direct ) ;

	}

	return 0 ;	

}

int EllHalFileRead ( int file , void* buffer , int size , int counter ) {

	//	author : WANG QUANWEI
	//	notes : fread
	//	since : 20090809

	return fread ( buffer , size , counter , (FILE*)file ) ;

}

int EllHalFileWrite ( int file , void* buffer , int size , int counter ) {
	
	//	author : WANG QUANWEI
	//	notes : fwrite
	//	since : 20090809

	return fwrite ( buffer , size , counter , (FILE*)file ) ;

}

int EllHalFileEnd ( int file ) {
	
	//	author : WANG QUANWEI
	//	notes : fwrite
	//	since : 20090809

	return feof ( (FILE*)file ) ;

}

int EllHalFileLength ( int file ) {

	//	author : WANG QUANWEI
	//	notes : fseek
	//	since : 20090809
	
	int length = 0;

	EllHalFileSeek ( file , 0 , ELLHAL_SEEK_HEAD ) ;

	while( !EllHalFileEnd (file) ) {

		char swap = 0 ;

		EllHalFileRead ( file , &swap , 1 , 1 ) ;

		length ++ ;

	}

	EllHalFileSeek ( file , 0 , ELLHAL_SEEK_HEAD ) ;
	
	return length ;

}

int EllHalFileClose ( int file ) {
	
	//	author : WANG QUANWEI
	//	notes : fclose
	//	since : 20090809
	
	return fclose ( (FILE*)file ) ;

}

void EllMemcpy ( void* target_memory , void* source_memory , int length ) {
	
	//	author : WANG QUANWEI
	//	since : 20100107

	memcpy ( target_memory , source_memory , length ) ; 

}

