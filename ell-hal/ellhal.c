
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

# include "ell.h"
# include "ellhal.h"

# ifdef MTK_ELL
	# include "FileManagerGProt.h"
	# include "Fs_type.h"
	# include "Fs_func.h"
# endif

# ifdef MTK_ELL
	extern void *med_alloc_ext_mem_ext ( int size , char* file_p , long line_p ) ;
	extern void med_free_ext_mem_ext ( void **pointer , char* file_p , long line_p ) ;
	# define EllMALLOC(size) med_alloc_ext_mem_ext(size,__FILE__,__LINE__)
	# define EllFREE(buffer) med_free_ext_mem_ext(buffer,__FILE__,__LINE__)
# else
	# define EllMALLOC malloc
	# define EllFREE free
# endif

# define MEMORY_MONITOR_ENABLE
# ifdef MEMORY_MONITOR_ENABLE
MEMORY_MONITOR mem_monitor = { 0 , 0 , 0 , 0 , 0 , 0 } ;
int MemoryMonitorInit ( MEMORY_MONITOR* mem_monitor ) {

	//	author : Jelo Wang
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
	
	//	author : Jelo Wang
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
	newmem->head = mem_monitor->next ;
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
	
	//	author : Jelo Wang
	//	since : 20100418
	//	(C)TOK

	MEMORY_MONITOR* walker = mem_monitor->head ;

	if ( !address ) return ;

	for ( ;walker && walker->address != address ;walker = walker->next ) ;

	if ( walker ) {

		if ( walker->head ) 
		walker->head->next = walker->next ; 
		
		if ( walker->next ) 
		walker->next->head = walker->head ;
		
	# ifdef MTK_ELL
	//	EllFREE ( (void*) &walker ) ;
	# else
	//	EllFREE ( walker ) ;
	# endif
	}
	
}

void MemoryMonitorDestroy ( MEMORY_MONITOR* mem_monitor ) {
	
	//	author : Jelo Wang
	//	since : 20100418
	//	(C)TOK

	MEMORY_MONITOR* walker = 0 ;
	
	if ( !mem_monitor ) return ;

	for ( walker = mem_monitor->head ; walker ; ) {

		mem_monitor->next = walker->next ;
		
		if ( walker->file ) {

			# ifdef MTK_ELL
				EllFREE ( (void*) &walker->file ) ;
			# else
				EllFREE ( walker->file ) ;
			# endif

		}

		# ifdef MTK_ELL
			EllFREE ( (void*) &walker ) ;
		# else
			EllFREE ( walker ) ;
		# endif

		walker = mem_monitor->next ;

	}
	
}

int EllHalMemoryLeaked () {

	//	author : Jelo Wang
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
		EllLog ( "memory leaked %d bytes totall.\n" , totall ) ;

	# endif

	return 0 ;
}

# endif

void* EllNormalloc ( long int length , char* file , int line ) {

	//	author : Jelo Wang
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

	//	author : Jelo Wang
	//	notes : free
	//	since : 20090809

	# ifdef MEMORY_MONITOR_ENABLE
		MemoryMonitorFree ( &mem_monitor , (int)buffer ) ;
	# endif

# ifdef MTK_ELL
		EllFREE ( (void**)&buffer ) ;
# else
		EllFREE ( buffer ) ;
# endif

	return 1 ;

}

int EllFreeEx ( void** buffer ) {

	//	author : Jelo Wang
	//	notes : free
	//	since : 20090809

	# ifdef MEMORY_MONITOR_ENABLE
		MemoryMonitorFree ( &mem_monitor , (int)*buffer ) ;
	# endif

# ifdef MTK_ELL
	EllFREE ( buffer ) ;
# else
	EllFREE ( *buffer ) ;
# endif

	*buffer = 0 ;

	return 1 ;

}

int EllHalFileOpen ( char* path , int flag ) {

	//	author : Jelo Wang
	//	notes : fopen
	//	since : 20090809

# ifdef MTK_ELL

	char unpath [ 256 ] = {0} ;

	EllAsciiToUnicode ( unpath , path ) ;
	
	switch ( flag ) {

		case ELLHAL_READ_OPEN : return (int) FS_Open ( (unsigned short* )unpath , FS_READ_ONLY ) ; 
		case ELLHAL_CREW_OPEN : return (int) FS_Open ( (unsigned short* )unpath , FS_CREATE_ALWAYS ) ; 		
   
	}

# else

	switch ( flag ) {

		case ELLHAL_READ_OPEN : return (int) fopen ( path , "rb" ) ; 
		case ELLHAL_CREW_OPEN : return (int) fopen ( path , "wb" ) ; 		

	}

# endif

	return 0 ;
	
}

int EllHalFileSeek ( int file , int offset , int direct ) {

	//	author : Jelo Wang
	//	notes : fseek
	//	since : 20090809

	switch ( direct ) {

	# ifdef MTK_ELL
		case ELLHAL_SEEK_HEAD : return FS_Seek( file , offset , FS_FILE_BEGIN ) ;
	# else
		case ELLHAL_SEEK_HEAD : return fseek ( (FILE*)file , offset , direct ) ;
	# endif
	
	}

	return 0 ;	

}

int EllHalFileRead ( int file , void* buffer , int size , int counter ) {

	//	author : Jelo Wang
	//	notes : fread
	//	since : 20090809

# ifdef MTK_ELL
	unsigned int read = 0 ;
	return FS_Read( file , buffer , counter*size , &read );
# else
	return fread ( buffer , size , counter , (FILE*)file ) ;
# endif

}

int EllHalFileWrite ( int file , void* buffer , int size , int counter ) {
	
	//	author : Jelo Wang
	//	notes : fwrite
	//	since : 20090809

# ifdef MTK_ELL
	unsigned int write = 0 ;
	return FS_Write( file , buffer , counter*size , &write );
# else
	return fwrite ( buffer , size , counter , (FILE*)file ) ;
# endif

}

int EllHalFileEnd ( int file ) {
	
	//	author : Jelo Wang
	//	notes : fwrite
	//	since : 20090809

# ifdef MTK_ELL
	unsigned int position = 0;
	unsigned int length = 0;
	FS_GetFilePosition ( file , &position ) ; 
	FS_GetFileSize ( file , &length ) ; 
	return position >= length ;
# else
	return feof ( (FILE*)file ) ;
# endif

}

int EllHalFileLength ( int file ) {

	//	author : Jelo Wang
	//	notes : fseek
	//	since : 20090809

# ifdef MTK_ELL	

	unsigned int length = 0;

	FS_GetFileSize ( file , &length ) ; 
	
	return length ;
	
# else
	
	int length = 0;

	EllHalFileSeek ( file , 0 , ELLHAL_SEEK_HEAD ) ;

	while( !EllHalFileEnd (file) ) {

		char swap = 0 ;

		EllHalFileRead ( file , &swap , 1 , 1 ) ;

		length ++ ;

	}

	EllHalFileSeek ( file , 0 , ELLHAL_SEEK_HEAD ) ;
	
	return length ;

# endif

}

int EllHalFileClose ( int file ) {
	
	//	author : Jelo Wang
	//	notes : fclose
	//	since : 20090809

# ifdef MTK_ELL
	return FS_Close ( file ) ;
# else
	return fclose ( (FILE*)file ) ;
# endif

}

void EllMemcpy ( void* target_memory , void* source_memory , int length ) {
	
	//	author : Jelo Wang
	//	since : 20100107

	memcpy ( target_memory , source_memory , length ) ; 

}

