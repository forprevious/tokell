
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
# include "ellsl.h"
# include "ellhal.h"

ELL* ell = 0 ;

int EllDynamicPoolCreate () {

	//	author : WANG QUANWEI
	//	(C)TOK

	//	notes : Create an Excutable Linked Library

	ELL* nell = (ELL* ) EllMalloc ( sizeof(ELL) ) ;
	
	nell->ObjectList = EllSlListCreate () ;

	ell = nell ;
	
	return (int)nell ;

}

void EllDynamicPoolSetCurrent ( int ella ) {

	//	author : WANG QUANWEI
	//	(C)TOK

	ell = (ELL* ) ella ;

}

int EllDynamicPoolInsertApplication ( char* application ) {

	//	author : WANG QUANWEI
	//	(C)TOK

	//	notes : Insert an application to the DynamicPool

	int obcounter = 0 ;
	
	if ( !ell || !application ) return 0 ;

	obcounter = EllGetAllObjectFileFromDirectory ( ell->ObjectList , application ) ;
	
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

# if 0
	{
		int looper = 0 ;
		for ( looper = 0 ; looper <  obcounter ; looper ++ ) {

			ell->TextRel.elf32_rel[looper] = 0 ;
			ell->DataRel.elf32_rel[looper] = 0 ;
			
			ell->TextRela.elf32_rela[looper] = 0 ;
			ell->DataRela.elf32_rela[looper] = 0 ;
			
		}
		
	}
# endif 

	return (int) ell ;
		
}

int EllDynamicPoolCreateSymbols ( int obid , int totall ) {

	//	author : WANG QUANWEI
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

	//	author : WANG QUANWEI
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

	//	author : WANG QUANWEI
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

	//	author : WANG QUANWEI
	//	notes : Get a symbol
	//	(C)TOK

	//	|looper |length of SYMBOL TABLE |symbol1 | |symbol2 |...|symboln |
	//	for skip looper and totall we let info plus 2 here
	info = info + 2 ;
	return (Elf32_Sym* )((int)ell->Sym.elf32_sym[obid]+info*sizeof(Elf32_Sym)) ; ;
		
}

Elf32_Sym* EllDynamicPoolGlobalGetSymbol ( int obidborder , char* name ) {

	//	author : WANG QUANWEI
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

	//	author : WANG QUANWEI
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

	//	author : WANG QUANWEI
	//	(C)TOK

	int obidlooper = 0 ;

	if ( !ell ) return 0 ;
	
	for ( obidlooper = 0 ; obidlooper < ell->ObjTotall ; obidlooper ++ ) 
		EllFree ( ell->Sym.elf32_sym[obidlooper] ) ;

	EllFree ( ell->Sym.elf32_sym ) ;

	EllFreeEx ( (void**)&ell ) ; 

	return 1 ;
		
}

# include "windows.h"
int EllGetAllObjectFileFromDirectory ( int list , char* application ) {

	//	author : WANG QUANWEI
	//	notes : Get all obejct files from a directory and save it with a Linked-List
	//	(C)TOK

	int counter = 0 ;
	BOOL result = TRUE ;
	WIN32_FIND_DATA finfo ;
	HANDLE hDir ;

	char path [256] = { 0 } ;
	sprintf ( path , "e:\\ell\\%s\\obj\\*.ell" , application ) ;

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


int EllElfMapCheckHeader ( char* value ) {

	//	author : WANG QUANWEI
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

	//	author : WANG QUANWEI
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

	//	author : WANG QUANWEI
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

	//	author : WANG QUANWEI
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

	//	author : WANG QUANWEI
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
				
		if ( index == elf32_shdr->sh_flags  ) {
		
			return (int)((int)ell->Shdr.elf32_shdr[obid]+looper*sizeof(Elf32_Shdr)) ;

		}
	}

	return 0 ;

}

int EllElfMapNolSectGetLborder ( int obid ) {

	//	author : WANG QUANWEI
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

	//	author : WANG QUANWEI
	//	(C)TOK

	int obidlooper = 0 ;

	if ( !ell->Shdr.elf32_shdr ) return ;
	
	for ( obidlooper = 0 ; obidlooper < obidborder ; obidlooper ++ )  {

		if ( ell->Shdr.elf32_shdr[obidlooper]  ) EllFree ( ell->Shdr.elf32_shdr[obidlooper] ) ;
		
	}

	EllFree ( ell->Shdr.elf32_shdr ) ;
	
}


int EllElfMapRelocRelCreate ( Elf32_Rel** elf32_rel , int obid , int totall ) {

	//	author : WANG QUANWEI
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

	//	author : WANG QUANWEI
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

	//	author : WANG QUANWEI
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

	//	author : WANG QUANWEI
	//	(C)TOK

	int obidlooper = 0 ;

	if ( !elf32_rel ) return ;
	
	for ( obidlooper = 0 ; obidlooper < obidborder ; obidlooper ++ )  {

		if ( elf32_rel[obidlooper]  ) EllFree ( elf32_rel[obidlooper] ) ;

	}
 
	EllFree ( elf32_rel ) ;
	
}


# if 0
int EllElfMapRelocRelaCreate ( Elf32_Rel* reloctab , int totall , int* looper , int* length ) {


	//	author : WANG QUANWEI
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


	//	author : WANG QUANWEI
	//	(C)TOK

	if ( !reloctab ) return 0 ;

	if ( !reloctab ) return 0 ;
	if ( *looper >= length ) return 0 ;

	EllMemcpy ( (void*)((int)reloctab+(*looper)*sizeof(Elf32_Rela)) , buffer , sizeof(Elf32_Rela) ) ;
	*looper ++ ;
	
	return 1 ;

	
}
# endif

void EllDump ( char* path , void* buffer , int length ) {

	//	author : WANG QUANWEI
	//	(C)TOK

	int file = EllHalFileOpen ( path , ELLHAL_CREW_OPEN ) ;
	if ( !file ) return ;
	EllHalFileWrite ( file ,  buffer , 1 , length ) ;
	EllHalFileClose ( file ) ;
	
}

void EllLog ( const char* message , ... ) {

	//	author : WANG QUANWEI
	//	(C)TOK

	//	notes : log
	
	va_list ap ;   
	va_start ( ap , message ) ;   
	vprintf ( message , ap ) ;
	va_end ( ap ) ;

}

