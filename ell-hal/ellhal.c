
/*

+	Executable Linking-Library 1.0.2.
+	Architecture : ARMv6

+	'Executable Linking-Library' is a Dynamic Linking solution for closed runing environment.
+	The project lunched by Jelo Wang since 2010 from 'Techniques of Knowledge' community. 

+	You can redistribute it and/or modify it under the terms of the gnu general public version 3 of 
+	the license as published by the free software foundation.this program is distributed in the hope 
+	that it will be useful,but without any warranty.without even the implied warranty of merchantability 
+	or fitness for a particular purpose.																					
																												
+	(C)	ͻ�ǿ�ԴTechniques of Knowledge
+		an open source community since 2008
+		Community : http://www.tok.cc
+		Contact Us : jelo.wang@gmail.com

+		����֧�֡�������չ��ƽ̨�����ӭ��������ϵ��
+		���ǽ�Ϊ���ṩ�г��ģ�ǿ���ķ���

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
	# define EllFREE(buffer) //med_free_ext_mem_ext(buffer,__FILE__,__LINE__)
# else
	# define EllMALLOC malloc
	# define EllFREE free
# endif

# define MEMORY_MONITOR_ENABLE
# ifdef MEMORY_MONITOR_ENABLE
# define MMT_MASK_LENGTH 1024
unsigned char* mmt_mask = 0 ;
MEMORY_MONITOR mem_monitor = { 0 , 0 , 0 , 0 , 0 , 0 } ;
int MMTInit () {

	//	author : Jelo Wang
	//	since : 20100418
	//	(C)TOK
	
	mem_monitor.file = 0 ;
	mem_monitor.line = 0 ;
	mem_monitor.length = 0 ;
	mem_monitor.address = 0 ;	
	mem_monitor.head = 0 ;		
	mem_monitor.next = 0 ;

	return 1 ;
		
}

void MMTCreateMask ()
{

	//	author : Jelo Wang
	//	since : 20110705
	//	(C)TOK

	int looper = 0 ;
	
	mmt_mask = (unsigned char* ) EllMALLOC ( MMT_MASK_LENGTH ) ;

	for ( looper = 0 ; looper < MMT_MASK_LENGTH ; looper ++ )
	{		mmt_mask [ looper ] = looper ;
	}

}

void MMTAdd ( char* file , int line , int length , int address ) {
	
	//	author : Jelo Wang
	//	since : 20100418
	//	(C)TOK

	MEMORY_MONITOR* newmem = 0 ;

	newmem	= (MEMORY_MONITOR* ) EllMALLOC ( sizeof(MEMORY_MONITOR) ) ;

	if ( 0 == mmt_mask ) MMTCreateMask () ;

	memcpy ( (void*)((unsigned int)address+length) , mmt_mask , MMT_MASK_LENGTH ) ;

	if ( file ) {
		newmem->file = (char* ) EllMALLOC ( strlen ( file ) + 1 ) ;
		strcpy ( newmem->file , file ) ;
	}

	newmem->line = line ; 
	newmem->length = length ;
	newmem->address = address ;
	newmem->head = 0 ;
	newmem->next = 0 ;
 
	if ( 0 == mem_monitor.head ) {
		mem_monitor.head = newmem ;
		mem_monitor.next = newmem ;
	} else {
		mem_monitor.next->next = newmem ;
		mem_monitor.next = newmem ;
	}

}

void MMTDestroy () {
	
	//	author : Jelo Wang
	//	since : 20100418
	//	(C)TOK

	MEMORY_MONITOR* walker = 0 ;
	
	for ( walker = mem_monitor.head ; walker ; ) {
		mem_monitor.next = walker->next ;
		if ( walker->file ) EllFREE ( walker->file ) ;
		EllFREE ( walker ) ;
		walker = mem_monitor.next ;
	}
	
}

void MMTCheckOverflow () {

	//	author : Jelo
	//	since : 2011.4.10
	//	(C)TOK
	
	//	notes : ʶ������ڴ��

	MEMORY_MONITOR* looper = 0 ;
	unsigned char* mask = 0 ;

	mask = (unsigned char* ) EllMALLOC ( MMT_MASK_LENGTH ) ;

	for ( looper = mem_monitor.head ; looper ; looper=looper->next ) 
	{

		int counter = 0 ;
		
		memcpy ( mask , (void*)((unsigned int )looper->address+looper->length) , MMT_MASK_LENGTH ) ;

		for ( counter = 0 ; counter < MMT_MASK_LENGTH ; counter ++ )
		{			if ( mask [ counter ] != mmt_mask [ counter ] )
			{
				EllLog ( "!!!!! M : %x , In : '%s' , At line : '%d' - overflowed\n" , looper->address , looper->file , looper->line ) ;	
				EllFREE ( mask ) ;
				return ;
			}
		}		

	}

	EllFREE ( mask ) ;
	
}

int MMTCheckLeaked () {

	//	author : Jelo Wang
	//	since : 20091129

		
	int totall = 0 ;
	int leakedtimes = 0 ;
	
	MEMORY_MONITOR* walker = mem_monitor.head ;
	
	for ( ;walker;walker=walker->next) {

		EllLog ("!!! M : %x , S : %d , In '%s' , At line : '%d' - leaked\n",
			walker->address,\
			walker->length,\
			walker->file,\
			walker->line\
		) ;\
	
		totall = totall + walker->length ;

		leakedtimes ++ ;
		
	}

	MMTDestroy () ;

	EllLog ( "Leaked Totall : %d Bytes\n" , totall ) ;
	EllLog ( "Leaked Times : %d\n" , leakedtimes ) ;

	return 0 ;
}

void MMTFree ( int address ) {
	
	//	author : Jelo Wang
	//	since : 20100418
	//	(C)TOK

	MEMORY_MONITOR* walker = mem_monitor.head ;
	MEMORY_MONITOR* pre = &mem_monitor ;
	
	if ( !address ) return ;

	for ( ; walker ; ) {
		
		if ( address == walker->address ) {
			if ( walker == mem_monitor.head ) {
				mem_monitor.head = walker->next ;
				if ( walker->file ) EllFREE ( walker->file ) ;
				EllFREE ( walker ) ;
				return ;
			} else {
				pre->next = walker->next ;
				if ( walker == mem_monitor.next ) {
					mem_monitor.next = pre ;
				}
				if ( walker->file ) EllFREE ( walker->file ) ;
				EllFREE ( walker ) ;
				return ;
			}
			
		}

		pre = walker ;
		walker = walker->next ;
			
	}
	
}



# endif

void* EllNormalloc ( long int length , char* file , int line ) {

	//	author : Jelo Wang
	//	notes : malloc
	//	since : 20090809
	
	void* buffer = 0 ;

	# ifdef MEMORY_MONITOR_ENABLE
		buffer = (void*) EllMALLOC ( length + MMT_MASK_LENGTH ) ;
		memset ( buffer , 0 , length + MMT_MASK_LENGTH ) ;
		MMTAdd ( file , line , length , (int)buffer ) ;
		MMTCheckOverflow () ;
	# else
		buffer = (void*) EllMALLOC ( length ) ;
		if ( buffer ) memset ( buffer , 0 , length ) ;
	# endif
	
	return buffer ;

}

int EllFree ( void* buffer ) {

	//	author : Jelo Wang
	//	notes : free
	//	since : 20090809

	# ifdef MEMORY_MONITOR_ENABLE
		MMTCheckOverflow () ; 
		MMTFree ( (int)buffer ) ; 
	# endif

# ifdef MTK_ELL
	//EllFREE ( buffer ) ;
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
		MMTCheckOverflow () ; 
		MMTFree ( (int)*buffer ) ; 
	# endif

# ifdef MTK_ELL
	//EllFREE ( *buffer ) ;
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

