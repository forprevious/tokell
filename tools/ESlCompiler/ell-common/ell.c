
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
# include "ellsl.h"
# include "ellhal.h"

# ifdef MTK_ELL
	# include "FileManagerGProt.h"
	# include "Fs_type.h"
	# include "Fs_func.h"
# endif

ELL* ell = 0 ;

int EllDynamicPoolCreate () {

	//	author : Jelo Wang
	//	(C)TOK

	//	notes : Create an Excutable Linked Library

	ELL* nell = (ELL* ) EllMalloc ( sizeof(ELL) ) ;
	
	nell->ObjectList = EllSlListCreate () ;

	ell = nell ;
	
	return (int)nell ;

}

void EllDynamicPoolSetCurrent ( int ella ) {

	//	author : Jelo Wang
	//	(C)TOK

	ell = (ELL* ) ella ;

}

int EllDynamicPoolInsertApplication ( char* application ) {

	//	author : Jelo Wang
	//	(C)TOK

	//	notes : Insert an application to the DynamicPool

	int obcounter = 1 ;
	
	ell->ObjTotall = obcounter ;
	
	ell->ObjectBased = (int* ) EllMalloc ( sizeof(int)*ell->ObjTotall ) ;
	
	if ( !ell->ObjectBased ) {
		EllSlListDestroy ( ell->ObjectList ) ;
		return 0 ;
	}

	ell->Sym.elf32_sym = (Elf32_Sym ** ) EllMalloc ( sizeof(Elf32_Sym*)*obcounter ) ;
	ell->Shdr.elf32_shdr = (Elf32_Shdr ** ) EllMalloc ( sizeof(Elf32_Shdr*)*obcounter ) ;
	ell->TextRel.elf32_rel = (Elf32_Rel ** ) EllMalloc ( sizeof(Elf32_Rel*)*obcounter ) ;
	ell->DataRel.elf32_rel = (Elf32_Rel ** ) EllMalloc ( sizeof(Elf32_Rel*)*obcounter ) ;
	ell->TextRela.elf32_rela = (Elf32_Rela ** ) EllMalloc ( sizeof(Elf32_Rela*)*obcounter ) ;
	ell->DataRela.elf32_rela = (Elf32_Rela ** ) EllMalloc ( sizeof(Elf32_Rela*)*obcounter ) ;

	return (int) ell ;
		
}

int EllDynamicPoolCreateSymbols ( int obid , int totall ) {

	//	author : Jelo Wang
	//	notes : Create a Symbols pool
	//	(C)TOK

	int value = 0 ;
	
	if ( !ell || !totall || !ell->Sym.elf32_sym ) return 0 ;

	//	|looper |length of SYMBOL TABLE |symbol1 | |symbol2 |...|symboln |
	//	Get 2 additional unit for saving looper and totall of SYMBOLES
	ell->Sym.elf32_sym[obid] = (Elf32_Sym* ) EllMalloc ( sizeof(Elf32_Sym)*(totall+2) ) ;
	
	if ( !ell->Sym.elf32_sym[obid] ) return 0 ;

	//	looper
	value = 2 ;
	EllMemcpy ( (void*)((int)ell->Sym.elf32_sym[obid]+0*sizeof(Elf32_Sym)) , &value , sizeof(int) ) ;

	//	totall
	value = totall + 2 ;
	EllMemcpy ( (void*)((int)ell->Sym.elf32_sym[obid]+1*sizeof(Elf32_Sym)) , &value , sizeof(int) ) ;
	
	return 1 ;
	
}

int EllDynamicPoolInsertSymbol ( int obid , void* buffer , char* name , int index ) {

	//	author : Jelo Wang
	//	notes : Insert a Symbol to the application
	//	(C)TOK

	int looper = 0 ;
	int totall = 0 ;	
	Elf32_Sym* elf32_sym = 0 ;

	if ( !ell->Sym.elf32_sym[obid] ) return 0 ;

	//	|looper |totall length of SYMBOL TABLE |symbol1| |symbol2 |...|symboln|
	//	get looper and totall length of SYMBOL TABLE from ell->Sym.elf32_sym[obid]
	elf32_sym = (Elf32_Sym*)((int)ell->Sym.elf32_sym[obid]+0*sizeof(Elf32_Sym)) ;
	looper = elf32_sym->st_name ;
	elf32_sym = (Elf32_Sym*)((int)ell->Sym.elf32_sym[obid]+1*sizeof(Elf32_Sym)) ;
	totall = elf32_sym->st_name ;

	if ( looper >= totall ) return 0 ;
		
	elf32_sym = (Elf32_Sym* ) buffer ;
	elf32_sym->st_name = (Elf32_Word ) EllMalloc ( strlen(name) + 1 ) ;

	if ( elf32_sym->st_name ) strcpy ( (char*)elf32_sym->st_name , (char* )name ) ;	

	//	we let st_other for saving the index at symtab
	elf32_sym->st_other = index ;
	//	recaculate the based-offset of a symbol
	elf32_sym->st_value = elf32_sym->st_value + ell->ObjectBased[obid] ;

	EllMemcpy ( (void*)((int)ell->Sym.elf32_sym[obid]+looper*sizeof(Elf32_Sym)) , buffer , sizeof(Elf32_Sym) ) ;
	
	looper ++ ;

	//	save looper
	EllMemcpy ( (void*)((int)ell->Sym.elf32_sym[obid]+0*sizeof(Elf32_Sym)) , &looper , sizeof(int) ) ;
	
	return 1 ;
		
}


int EllDynamicPoolGetLborder ( int obid ) {

	//	author : Jelo Wang
	//	(C)TOK

	int lborder = 0 ;
	Elf32_Sym* elf32_sym = 0 ;

	if ( !ell->Sym.elf32_sym[obid] ) return 0 ;

	//	|looper |totall length of SYMBOL TABLE |symbol1| |symbol2 |...|symboln|
	//	get looper and totall length of SYMBOL TABLE from ell->Sym.elf32_sym[obid]
	elf32_sym = (Elf32_Sym*)((int)ell->Sym.elf32_sym[obid]+0*sizeof(Elf32_Sym)) ;
	lborder = elf32_sym->st_name ;

	return lborder ;
	
	
}

Elf32_Sym* EllDynamicPoolLocalGetSymbol ( int obid , int info ) {

	//	author : Jelo Wang
	//	notes : Get a symbol
	//	(C)TOK

	//	|looper |length of SYMBOL TABLE |symbol1 | |symbol2 |...|symboln |
	//	for skip looper and totall we let info plus 2 here
	info = info + 2 ;
	return (Elf32_Sym* )((int)ell->Sym.elf32_sym[obid]+info*sizeof(Elf32_Sym)) ; ;
		
}

Elf32_Sym* EllDynamicPoolGlobalGetSymbol ( int obidborder , char* name ) {

	//	author : Jelo Wang
	//	notes : Get a global symbol on global scope
	//	(C)TOK

	int obidlooper = 0 ;
	
	for ( obidlooper = 0 ; obidlooper < obidborder ; obidlooper ++ )  {

		int looper = 2 ;
		//	walkthrough symbols
		int lborder = EllDynamicPoolGetLborder ( obidlooper ) ;

		for ( looper = 2 ; looper < lborder ; looper ++ ) {

			Elf32_Sym* elf32_sym = 0 ;
			elf32_sym = (Elf32_Sym* )((int)ell->Sym.elf32_sym[obidlooper]+looper*sizeof(Elf32_Sym)) ;
						
			if ( elf32_sym->st_name && elf32_sym->st_shndx ) 
				if ( !strcmp ( (char*)elf32_sym->st_name , name ) ) return elf32_sym ;
		
		}
		
	}

	return 0 ;

		
}


int EllDynamicPoolGetSymbolEntry ( char* name ) {

	//	author : Jelo Wang
	//	notes : Get a func symbol or objective symbol
	//	(C)TOK

	int obidlooper = 0 ;
	
	if ( !ell ) return 0 ;

	for ( obidlooper = 0 ; obidlooper < ell->ObjTotall  ; obidlooper ++ )  {

		int looper = 2 ;
		//	walkthrough symbols
		int lborder = EllDynamicPoolGetLborder ( obidlooper ) ;

		for ( looper = 2 ; looper < lborder ; looper ++ ) {

			Elf32_Sym* elf32_sym = 0 ;
			elf32_sym = (Elf32_Sym* )((int)ell->Sym.elf32_sym[obidlooper]+looper*sizeof(Elf32_Sym)) ;
						
			if ( STT_FUNC == ELF32_ST_BIND(elf32_sym->st_info) || STT_OBJECT == ELF32_ST_BIND(elf32_sym->st_info) ) {
				if ( !strcmp ( (char*)elf32_sym->st_name , name ) && elf32_sym->st_shndx ) 
					return elf32_sym->st_value ;
			}
		
		}
		
	}

	return -1 ;

		
}

int EllDynamicPoolDestroy () {

	//	author : Jelo Wang
	//	(C)TOK

	int counter = 0 ;
	int obidlooper = 0 ;

	if ( !ell ) return 0 ;

	//	release symbol table
	for ( obidlooper = 0 ; obidlooper < ell->ObjTotall ; obidlooper ++ ) {

		int totall_symbol = 0 ;
		Elf32_Sym* elf32_sym = (Elf32_Sym*)((int)ell->Sym.elf32_sym[obidlooper]+1*sizeof(Elf32_Sym)) ;

		totall_symbol = elf32_sym->st_name ;

		for ( counter = 2 ; counter < totall_symbol ; counter ++ ) {
			elf32_sym = (Elf32_Sym*)((int)ell->Sym.elf32_sym[obidlooper]+counter*sizeof(Elf32_Sym)) ;
			if ( elf32_sym->st_name ) {
				EllFree ( (void*) elf32_sym->st_name ) ;
			}			
		}
		EllFree ( (void*) ell->Sym.elf32_sym[obidlooper] ) ;
				
	}

	EllFree ( (void*) ell->Sym.elf32_sym ) ;

	EllFreeEx ( (void**)&ell ) ; 

	return 1 ;
		
}

# ifdef MTK_ELL

# if 0

char* ell_root = "%c:\\ExeutableLibraryLink\\applications\\%s\\object\\*";
char* ell_current_directory = 0;

int ell_io_layer_load_all_object_file ( int list , char* application  ) {

	//author: WANG QUANWEI
	//notes : 取目录中所有文件的绝对路径（WIN32版本）
	//since : 20090702
	//(c)MET 

	FS_HANDLE object_file_handle;
	FS_DOSDirEntry directory_entry;
 
	int long times = 0;
	int space_length_needs = 0;
	char* buffer = 0;
	char* asicbuffer;
	unsigned short filenamebuffer[120] ;

	char* ell_current_directory_back_up = 0;
	char* unicode_directorySpecification = 0;
	unsigned short* directorySpecification = 0;
 
	directorySpecification = (unsigned short*) EllMalloc ( (strlen(ell_root) + 2) * 2 * sizeof(unsigned short) ) ;
	unicode_directorySpecification = (char*) EllMalloc ( (strlen(ell_root) + 2) * 2 * sizeof(unsigned short) ) ;
	ell_current_directory_back_up = (char*) EllMalloc ( (strlen(ell_root) + 64) * 2 ) ;
	ell_current_directory = (char*) EllMalloc ( (strlen(ell_root) + 64) * 2 ) ;
	buffer = (char*) EllMalloc( (strlen(ell_root) + 2) * 2 ) ;
	asicbuffer = (char* ) EllMalloc ( 64 );
	  
	sprintf(ell_current_directory,ell_root,FS_GetDrive(FS_DRIVE_V_REMOVABLE, 1, FS_NO_ALT_DRIVE),application) ;

	EllLog("ell_current_directory %s",ell_current_directory);
	//	directorySpecification 用来寻找文件
	//	directorySpecification  = d:\\ExecutableLibraryLink\\applications\\gtkings\\object\\*
	strcpy((char*)directorySpecification,ell_current_directory) ;	
 
	EllLog("directorySpecification %s",directorySpecification);

	//	buffer  = d:\\ExecutableLibraryLink\\applications\\gtkings\\object\\%s 
	strcpy(buffer,ell_current_directory) ;	
	buffer[strlen(buffer)-1] = '%';
	buffer[strlen(buffer)] = 's';
	buffer[strlen(buffer)+1] = 0;
 
	//	ell_current_directory = d:\\ExecutableLibraryLink\\applications\\gtkings\\object\\	
	ell_current_directory[strlen(ell_current_directory)-1] = 0 ;

	//	全角字符转换
	EllAsciiToUnicode (unicode_directorySpecification , ell_current_directory ) ;
	EllWStrcpy ( (signed char*)ell_current_directory , (signed char*)unicode_directorySpecification ) ;
	EllWStrcpy ( (signed char*)ell_current_directory_back_up , (signed char*)ell_current_directory ) ;

	//	全角字符转换	
	EllAsciiToUnicode ( unicode_directorySpecification , (char*)directorySpecification ) ;	
	EllWStrcpy ( (signed char*)directorySpecification , (signed char*)unicode_directorySpecification ) ;

	object_file_handle = FS_FindFirst ( directorySpecification , 0 , 0 , &directory_entry , filenamebuffer , 120 ) ;

	if( 0 > object_file_handle ) {		
	  FS_Close ( object_file_handle ) ;
	  return 0; 	  
	}
	
	while( FS_NO_ERROR == FS_FindNext(object_file_handle,&directory_entry,filenamebuffer , 120 ) ) {

		if( !(FS_ATTR_DIR & directory_entry . Attributes) ) {

			EllUnicodeToAscii((char *)asicbuffer,(char *)filenamebuffer); 
			
			sprintf(ell_current_directory,buffer,asicbuffer) ; 
		 
			EllAsciiToUnicode (unicode_directorySpecification , ell_current_directory ) ;
			EllWStrcpy ( (signed char*)ell_current_directory , (signed char*)unicode_directorySpecification ) ;	
	     
			space_length_needs += ell_memory_space (ell_current_directory) ;

			EXECUTABLE_LIBRARY_add_object( execute_library , ell_current_directory );
			
		} 
	
	}
 
	FS_Close ( object_file_handle ) ;
	
	EllWStrcpy ( (signed char*) ell_current_directory , (signed char*) ell_current_directory_back_up ) ;
	
# if 0
	EllFree ( ell_current_directory_back_up ) ;
	EllFree ( unicode_directorySpecification ) ;
	EllFree ( directorySpecification ) ;
	EllFree ( ell_current_directory ) ;	
	EllFree ( asicbuffer );
	EllFree ( buffer ) ;	
# endif

	return 1;



}  

# endif

int EllGetAllObjectFileFromDirectory ( int list , char* application ) {

	//	author : Jelo Wang
	//	notes : Get all obejct files from a directory and save it with a Linked-List
	//	(C)TOK

	int counter = 0 ;

	char path [256] = { 0 } ;
		
	EllSlListInsert ( list , (int)"e:\\ell\\ellapp.ell" ) ;
		
	counter ++ ;
	
	return counter ;
	
}
#else
# include "windows.h"
int EllGetAllObjectFileFromDirectory ( int list , char* application ) {

	//	author : Jelo Wang
	//	notes : Get all obejct files from a directory and save it with a Linked-List
	//	(C)TOK

	int counter = 0 ;
	BOOL result = TRUE ;
	WIN32_FIND_DATA finfo ;
	HANDLE hDir ;

	char path [256] = { 0 } ;
	sprintf ( path , "e:\\ell\\%s\\obj\\*.esl" , application ) ;

	hDir = FindFirstFile( path , &finfo ) ;
	if (hDir == INVALID_HANDLE_VALUE) return 0 ;

	while (result)  {

		char* string = 0 ;
		sprintf ( path , "e:\\ell\\%s\\obj\\%s" , application , finfo.cFileName ) ;
		string = (char* ) EllMalloc ( strlen(path) + 1 ) ;
		strcpy ( string , path ) ;
		
		EllSlListInsert ( list , (int)string ) ;
		
		result = FindNextFile(hDir, &finfo);

		counter ++ ;

	} 

	FindClose(hDir);

	return counter ;
	
}
#endif

int EllElfMapCheckHeader ( char* value ) {

	//	author : Jelo Wang
	//	(C)TOK

	char elf_header_legal_value [7] = {

		ELFMAG0 , 'E' , 'L' , 'F' ,
		ELFCLASS32 , ELFCLASS32 ,
		ELFCLASS32

	} ;
	
	int looper = 0 ;
	
	for(  ; looper < 7  ; looper ++ )
		if( *(value+looper) != elf_header_legal_value[looper] )
			return 0 ;

	return 1 ;

}


int EllElfMapNolSectCreate ( int obid , int totall ) {

	//	author : Jelo Wang
	//	(C)TOK

	int value = 0 ;

	if ( !totall || !ell  ) return 0 ;

	//	|looper |totall length of SECTIONS |section1| |section2 |...|sectionn |
	//	Get 2 additional unit for saving looper and totall of SECTIONS
	ell->Shdr.elf32_shdr[obid] = (Elf32_Shdr* ) EllMalloc ( sizeof(Elf32_Shdr) * (totall+2) ) ;

	if ( !ell->Shdr.elf32_shdr[obid] ) return 0 ;
	
	//	looper
	value = 2 ;
	EllMemcpy ( (void*)((int)ell->Shdr.elf32_shdr[obid]+0*sizeof(Elf32_Shdr)) , &value , sizeof(int) ) ;

	//	totall
	value = totall + 2 ;
	EllMemcpy ( (void*)((int)ell->Shdr.elf32_shdr[obid]+1*sizeof(Elf32_Shdr)) , &value , sizeof(int) ) ;
	

	return 1 ;
	
}

int EllElfMapNolSectInsert ( int obid , int index , void* buffer , const char* name ) {

	//	author : Jelo Wang
	//	(C)TOK

	int looper = 0 ;
	int totall = 0 ;
	Elf32_Shdr* elf32_shdr = 0 ;
	
	if ( !ell->Shdr.elf32_shdr[obid] ) return 0 ; 

	//	|looper |totall length of SECTIONS |section1| |section2 |...|sectionn |
	//	get looper and totall length of SECTIONS from ell->Shdr.elf32_shdr[obid]
	elf32_shdr = (Elf32_Shdr*)((int)ell->Shdr.elf32_shdr[obid]+0*sizeof(Elf32_Shdr)) ;
	looper = elf32_shdr->sh_name ;
	elf32_shdr = (Elf32_Shdr*)((int)ell->Shdr.elf32_shdr[obid]+1*sizeof(Elf32_Shdr)) ;
	totall = elf32_shdr->sh_name ;

	if ( looper >= totall ) return 0 ;

	elf32_shdr = (Elf32_Shdr* ) buffer ;
	elf32_shdr->sh_name = (Elf32_Word) name ;
	
	//	we use sh_flags saving index of section
	elf32_shdr->sh_flags = index ;
	
	EllMemcpy ( (void*)((int)ell->Shdr.elf32_shdr[obid]+looper*sizeof(Elf32_Shdr)) , buffer , sizeof(Elf32_Shdr) ) ;

	looper ++ ;

	//	save looper
	EllMemcpy ( (void*)((int)ell->Shdr.elf32_shdr[obid]+0*sizeof(Elf32_Shdr)) , &looper , sizeof(int) ) ;
	
	return 1 ;

}

int EllElfMapNolSectGet ( int obid , char* name ) {

	//	author : Jelo Wang
	//	(C)TOK

	int looper = 0 ;
	int lborder = 0 ;
	Elf32_Shdr* elf32_shdr = 0 ;

	if ( !ell->Shdr.elf32_shdr[obid] ) return 0 ; 

	//	|looper |totall length of SECTIONS |section1| |section2 |...|sectionn |
	//	skip looper and totall length of SECTIONS we let looper = 2 here

	//	get looper and totall length of SECTIONS from ell->Shdr.elf32_shdr[obid]
	elf32_shdr = (Elf32_Shdr*)((int)ell->Shdr.elf32_shdr[obid]+0*sizeof(Elf32_Shdr)) ;
	lborder = elf32_shdr->sh_name ;
	
	for ( looper = 2 ; looper < lborder ; looper ++ ) {
		
		elf32_shdr = (Elf32_Shdr* )((int)ell->Shdr.elf32_shdr[obid]+looper*sizeof(Elf32_Shdr)) ;

		if ( !elf32_shdr->sh_name ) continue ;

		if ( !strcmp ( (char*)elf32_shdr->sh_name , name ) ) {
		
			return (int)((int)ell->Shdr.elf32_shdr[obid]+looper*sizeof(Elf32_Shdr)) ;

		}
	}

	return 0 ;

}


int EllElfMapNolSectGetWithIndex ( int obid , int index ) {

	//	author : Jelo Wang
	//	(C)TOK

	int looper = 0 ;
	int lborder = 0 ;
	Elf32_Shdr* elf32_shdr = 0 ;

	if ( !ell->Shdr.elf32_shdr[obid] ) return 0 ; 

	//	|looper |totall length of SECTIONS |section1| |section2 |...|sectionn |
	//	skip looper and totall length of SECTIONS we let looper = 2 here

	//	get looper and totall length of SECTIONS from ell->Shdr.elf32_shdr[obid]
	elf32_shdr = (Elf32_Shdr*)((int)ell->Shdr.elf32_shdr[obid]+0*sizeof(Elf32_Shdr)) ;
	lborder = elf32_shdr->sh_name ;
	
	for ( looper = 2 ; looper < lborder ; looper ++ ) {
		
		elf32_shdr = (Elf32_Shdr* )((int)ell->Shdr.elf32_shdr[obid]+looper*sizeof(Elf32_Shdr)) ;
				
		if ( index == (int) elf32_shdr->sh_flags  ) {
		
			return (int)((int)ell->Shdr.elf32_shdr[obid]+looper*sizeof(Elf32_Shdr)) ;

		}
	}

	return 0 ;

}

int EllElfMapNolSectGetLborder ( int obid ) {

	//	author : Jelo Wang
	//	(C)TOK

	int lborder = 0 ;
	Elf32_Shdr* elf32_shdr = 0 ;
	
	if ( !ell->Shdr.elf32_shdr[obid] ) return 0 ; 

	//	|looper |totall length of SECTIONS |section1| |section2 |...|sectionn |
	//	get looper and totall length of SECTIONS from ell->Shdr.elf32_shdr[obid]
	elf32_shdr = (Elf32_Shdr*)((int)ell->Shdr.elf32_shdr[obid]+0*sizeof(Elf32_Shdr)) ;
	lborder = elf32_shdr->sh_name ;

	return lborder ;
	
	
}


void EllElfMapNolSectDestroy ( int obidborder ) {

	//	author : Jelo Wang
	//	(C)TOK

	int obidlooper = 0 ;

	if ( !ell->Shdr.elf32_shdr ) return ;
	
	for ( obidlooper = 0 ; obidlooper < obidborder ; obidlooper ++ )  {

		if ( ell->Shdr.elf32_shdr[obidlooper]  ) EllFree ( ell->Shdr.elf32_shdr[obidlooper] ) ;
		
	}

	EllFree ( ell->Shdr.elf32_shdr ) ;
	
}


int EllElfMapRelocRelCreate ( Elf32_Rel** elf32_rel , int obid , int totall ) {

	//	author : Jelo Wang
	//	(C)TOK

	int value = 0 ;

	if ( !totall || !ell  ) return 0 ;

	//	|looper |totall length of REL |rel1| |rel2 |...|reln |
	//	Get 2 additional unit for saving looper and totall of REL
	elf32_rel[obid] = (Elf32_Rel* ) EllMalloc ( sizeof(Elf32_Rel) * (totall*12) ) ;

	if ( !elf32_rel[obid] ) return 0 ;
	
	//	looper
	value = 2 ;
	EllMemcpy ( (void*)((int)elf32_rel[obid]+0*sizeof(Elf32_Rel)) , &value , sizeof(int) ) ;

	//	totall
	value = totall + 2 ;
	EllMemcpy ( (void*)((int)elf32_rel[obid]+1*sizeof(Elf32_Rel)) , &value , sizeof(int) ) ;
	

	return 1 ;
	
	
}


int EllElfMapRelocRelInsert ( Elf32_Rel** elf32_rel , int obid , void* buffer ) {

	//	author : Jelo Wang
	//	(C)TOK

	int looper = 0 ;
	int totall = 0 ;
	Elf32_Rel* rel = 0 ;
	
	if ( !elf32_rel[obid] ) return 0 ; 

	//	|looper |totall length of REL |section1| |section2 |...|sectionn |
	//	get looper and totall length of REL from elf32_rel[obid]
	rel = (Elf32_Rel*)((int)elf32_rel[obid]+0*sizeof(Elf32_Rel)) ;
	looper = rel->r_offset ;
	rel = (Elf32_Rel*)((int)elf32_rel[obid]+1*sizeof(Elf32_Rel)) ;
	totall = rel->r_offset ;

	if ( looper >= totall ) return 0 ;
	
	//	recaculate the based-offset of a symbol
	rel = (Elf32_Rel* ) buffer ;
	rel->r_offset = rel->r_offset + ell->ObjectBased[obid] ;

	EllMemcpy ( (void*)((int)elf32_rel[obid]+looper*sizeof(Elf32_Rel)) , buffer , sizeof(Elf32_Rel) ) ;

	looper ++ ;

	//	save looper
	EllMemcpy ( (void*)((int)elf32_rel[obid]+0*sizeof(Elf32_Rel)) , &looper , sizeof(int) ) ;
	
	return 1 ;
	
}

int EllElfMapRelocGetLborder ( Elf32_Rel** elf32_rel , int obid ) {

	//	author : Jelo Wang
	//	(C)TOK

	int lborder = 0 ;
	Elf32_Rel* rel = 0 ;

	if ( !elf32_rel[obid] ) return 0 ; 

	//	|looper |totall length of REL |section1| |section2 |...|sectionn |
	//	get looper and totall length of REL from elf32_rel[obid]
	rel = (Elf32_Rel*)((int)elf32_rel[obid]+1*sizeof(Elf32_Rel)) ;
 	lborder = rel->r_offset ;


	return lborder ;
	
}

void EllElfMapRelocDestroy ( Elf32_Rel** elf32_rel , int obidborder ) {

	//	author : Jelo Wang
	//	(C)TOK

	int obidlooper = 0 ;

	if ( !elf32_rel ) return ;
	
	for ( obidlooper = 0 ; obidlooper < obidborder ; obidlooper ++ )  {

		if ( elf32_rel[obidlooper]  ) EllFree ( elf32_rel[obidlooper] ) ;

	}
 
	EllFree ( elf32_rel ) ;
	
}

void EllElfMapRelocRelaDestroy ( Elf32_Rela** elf32_rela , int obidborder ) {

	//	author : Jelo Wang
	//	(C)TOK

	int obidlooper = 0 ;

	if ( !elf32_rela ) return ;
	
	for ( obidlooper = 0 ; obidlooper < obidborder ; obidlooper ++ )  {

		if ( elf32_rela[obidlooper]  ) EllFree ( elf32_rela[obidlooper] ) ;

	}
 
	EllFree ( elf32_rela ) ;
	
}

# if 0
int EllElfMapRelocRelaCreate ( Elf32_Rel* reloctab , int totall , int* looper , int* length ) {


	//	author : Jelo Wang
	//	(C)TOK

	if ( !totall || !reloctab ) return 0 ;
	
	reloctab = (Elf32_Rela* ) EllMalloc ( sizeof(Elf32_Rela) ) ;

	if ( !reloctab ) return 0 ;
	
	reloctab = (Elf32_Rela* ) EllMalloc ( sizeof(Elf32_Rela)*totall ) ;

	if ( !reloctab) return 0 ;
	
	*looper = 0 ;
	*length = 0 ;
		
	return reloctab ;

	
}


int EllElfMapRelocRelaInsert ( Elf32_Rela* reloctab , void* buffer , int* looper , int length ) {


	//	author : Jelo Wang
	//	(C)TOK

	if ( !reloctab ) return 0 ;

	if ( !reloctab ) return 0 ;
	if ( *looper >= length ) return 0 ;

	EllMemcpy ( (void*)((int)reloctab+(*looper)*sizeof(Elf32_Rela)) , buffer , sizeof(Elf32_Rela) ) ;
	*looper ++ ;
	
	return 1 ;

	
}
# endif

char* EllWStrcpy ( signed char* strDestination , const signed char* strSource ) {

	//	author: Jelo Wang
	//	since : 20110323
	//	(C) TOK
 
    int count = 1;
    signed char* temp = strDestination;

    if ( !strSource ) {
		
        if ( strDestination ) {

            *(strDestination + count - 1) = '\0';
            *(strDestination + count) = '\0';
			
        }
		
        return (char* ) temp;

    }

    if ( !strDestination || !strSource ) {
        return 0;
    }
	
    while (!((*(strSource + count) == 0) && (*(strSource + count - 1) == 0))) {

        *(strDestination + count - 1) = *(strSource + count - 1);
        *(strDestination + count) = *(strSource + count);
        count += 2;
		
    }

    *(strDestination + count - 1) = '\0';
    *(strDestination + count) = '\0';

    return (char* ) temp;
	
}


int EllUnicodeToAscii ( unsigned char* pOutBuffer , signed char* pInBuffer ) {

	//	author: Jelo Wang
	//	since : 20110323
	//	(C) TOK
	
    int count = 0;

    while (!((*pInBuffer == 0) && (*(pInBuffer + 1) == 0))) {
        *pOutBuffer = *(pInBuffer);
        pInBuffer += 2;
        pOutBuffer++;
        count++;
    }

    *pOutBuffer = 0;
	
    return count;
	
}

void EllAsciiToUnicode ( char *outbuffer , char *inbuffer ) {

	//	author: Jelo Wang
	//	since : 20090625
	//	(C) TOK

	
	//(1) 参数：输出串 outbuffer unicode
	//(2) 参数：输入串 inbuffer asiic

    signed short count = -1 ;

    while (*inbuffer != '\0') {

        outbuffer[++count] = *inbuffer ;
        outbuffer[++count] = 0 ;
        inbuffer++ ;

    }

    outbuffer[++count] = '\0' ;
    outbuffer[++count] = '\0' ;
	
}

void EllDump ( char* path , void* buffer , int length ) {

	//	author : Jelo Wang
	//	(C)TOK

	int file = EllHalFileOpen ( path , ELLHAL_CREW_OPEN ) ;
	if ( !file ) return ;
	EllHalFileWrite ( file ,  buffer , 1 , length ) ;
	EllHalFileClose ( file ) ;
	
}

void EllLog ( const char* message , ... ) {

	//	author : Jelo Wang
	//	(C)TOK

	//	notes : log
	
	static int times = 0 ;
	static int file = -1 ;  
	unsigned int write = 0 ;

	char* path = "e:\\elllog.txt";
	
	char str [1024] ;
		
	va_list args;
	va_start ( args , message ) ;
	vsprintf ( str , message , args ) ;
	va_end (args) ;
	
	# ifdef WIN32
		printf ( str ) ;
	# endif

	if ( 0 == times ) {
		file = EllHalFileOpen ( path , ELLHAL_CREW_OPEN ) ;
		times ++ ;	
	}
	
	if ( !file ) return ;
	
	EllHalFileWrite ( file , str , 1 , strlen(str) ) ;

	return ;
			
}

