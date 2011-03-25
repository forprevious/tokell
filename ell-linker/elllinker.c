
/*

+	Executable Linking-Library 1.0.1.
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
# include "elllinker.h"

ELLLINKER EllLinker = { 0 , 0 } ;
ELLLINKERMEMORYPOOL EllLinkerMemoryPool = { 0 , 0 , 0 } ;

int EllLocalLinkerStatic ( int obid , int file , int ER_RO_RW_Rel , int ER_RO_ZI_Rel ) {

	//	author : Jelo Wang
	//	notes : link single object file
	//	(C)TOK

	int results = 1 ;
	//	as for why plus 2 here , check out ell.c
	int looper = 2 ;
	int lborder = EllElfMapNolSectGetLborder ( obid ) ;
	int gotsect = 0 ;
	
	Elf32_Shdr* elf32_shdr = { 0 } ;

	# ifdef ELL_DEBUG	
		EllLinkerMemoryPool.base = 0 ;
	# endif

	EllLinker.obidborder = 1 ;
	EllLinker.status = ELL_LOCAL_LINKER ;
	
	//	copy .text , .data , .rodata , .constdata , .bss to EllLinkerMemoryPool
	for ( looper = 2 ; looper < lborder ; gotsect = 0 , looper ++ ) {
	
		Elf32_Shdr* aelf32_shdr = (Elf32_Shdr* )((int)ell->Shdr.elf32_shdr[obid]+looper*sizeof(Elf32_Shdr)) ;
	
		if ( SHT_PROGBITS == aelf32_shdr->sh_type ) {

			gotsect = 1 ;
			
		} else if ( SHT_NOBITS == aelf32_shdr->sh_type ) {
		
			ELL_4BYTES_ALIGN ( EllLinkerMemoryPool.looper ) ;

			aelf32_shdr->sh_offset = EllLinkerMemoryPool.looper ;

			EllLinkerMemoryPool.looper = EllLinkerMemoryPool.looper + aelf32_shdr->sh_size ;
			
			gotsect = 0 ;
			
		}

		if ( gotsect ) {

			ELL_4BYTES_ALIGN ( EllLinkerMemoryPool.looper ) ;
		
			EllHalFileSeek ( file , aelf32_shdr->sh_offset , ELLHAL_SEEK_HEAD ) ;
			EllHalFileRead ( file , (void* )((int)EllLinkerMemoryPool.pool+EllLinkerMemoryPool.looper) , 1 , aelf32_shdr->sh_size ) ;
			
			aelf32_shdr->sh_offset = EllLinkerMemoryPool.looper ;
			EllLinkerMemoryPool.looper = EllLinkerMemoryPool.looper + aelf32_shdr->sh_size ;				
		
		}
		
	}

	if ( ER_RO_RW_Rel ) {
		elf32_shdr = (Elf32_Shdr* )EllElfMapNolSectGet ( obid , "ER_RW" ) ; 
		gotsect = elf32_shdr->sh_offset + EllLinkerMemoryPool.base ;
		EllMemcpy ( (void* )((int)EllLinkerMemoryPool.pool+ER_RO_RW_Rel) , &gotsect , sizeof(int) ) ;	
	}
	
	if ( ER_RO_ZI_Rel ) {
		elf32_shdr = (Elf32_Shdr* )EllElfMapNolSectGet ( obid , "ER_ZI" ) ; 
		gotsect = elf32_shdr->sh_offset + EllLinkerMemoryPool.base ;
		EllMemcpy ( (void* )((int)EllLinkerMemoryPool.pool+ER_RO_ZI_Rel) , &gotsect , sizeof(int) ) ;	
	}

	EllHalFileClose ( file ) ;
	
	return results ;

}

int EllLocalLinker ( int obid , int file ) {

	//	author : Jelo Wang
	//	notes : link single object file
	//	(C)TOK

	int results = 1 ;
	//	as for why plus 2 here , check out ell.c
	int looper = 2 ;
	int lborder = EllElfMapNolSectGetLborder ( obid ) ;
	int gotsect = 0 ;

	# ifdef ELL_DEBUG	
		EllLinkerMemoryPool.base = 0 ;
	# endif

	EllLinker.obidborder = 0 ;
	EllLinker.status = ELL_LOCAL_LINKER ;
	
	//	copy .text , .data , .rodata , .constdata , .bss to EllLinkerMemoryPool
	for ( looper = 2 ; looper < lborder ; gotsect = 0 , looper ++ ) {
	
		Elf32_Shdr* aelf32_shdr = (Elf32_Shdr* )((int)ell->Shdr.elf32_shdr[obid]+looper*sizeof(Elf32_Shdr)) ;
	
		if ( SHT_PROGBITS == aelf32_shdr->sh_type ) {

# if 0			
			if ( !strcmp ( ".text" , (char*)aelf32_shdr->sh_name ) ) {
				gotsect = 1 ;
			} else if ( !strcmp ( ".data" , (char*)aelf32_shdr->sh_name ) ) {
				gotsect = 1 ;
			} else if ( !strcmp ( ".rodata" , (char*)aelf32_shdr->sh_name ) ) {
				gotsect = 1 ;
			} else if ( !strcmp ( ".constdata" , (char*)aelf32_shdr->sh_name ) ) {
				gotsect = 1 ;
			} if ( !strcmp ( "_ell_text" , (char*)aelf32_shdr->sh_name ) ) {
				gotsect = 1 ;
			} 
# endif
			gotsect = 1 ;
			
		} else if ( SHT_NOBITS == aelf32_shdr->sh_type ) {
		
			if ( !strcmp ( ".bss" , (char*)aelf32_shdr->sh_name ) ) {	
				ELL_4BYTES_ALIGN ( EllLinkerMemoryPool.looper ) ;
				aelf32_shdr->sh_addr = EllLinkerMemoryPool.base + EllLinkerMemoryPool.looper ;
				//	It's very important informations about section-relocation bellow.
				//	Use sh_entsize saving the offset of section at EllLinkerMemoryPool		
				aelf32_shdr->sh_entsize =  EllLinkerMemoryPool.looper ; 	
				EllLinkerMemoryPool.looper = EllLinkerMemoryPool.looper + aelf32_shdr->sh_size ;
				continue ;

			} 
			
		}

		if ( gotsect ) {
			ELL_4BYTES_ALIGN ( EllLinkerMemoryPool.looper ) ;
			//	Absolute address of section 
			aelf32_shdr->sh_addr = EllLinkerMemoryPool.base + EllLinkerMemoryPool.looper ;
			//	It's very important informations about section-relocation bellow.
			//	Use sh_entsize saving the offset of section at EllLinkerMemoryPool		
			aelf32_shdr->sh_entsize =  EllLinkerMemoryPool.looper ; 					
			EllHalFileSeek ( file , aelf32_shdr->sh_offset , ELLHAL_SEEK_HEAD ) ;
			EllHalFileRead ( file , (void* )((int)EllLinkerMemoryPool.pool+EllLinkerMemoryPool.looper) , 1 , aelf32_shdr->sh_size ) ;
			EllLinkerMemoryPool.looper = EllLinkerMemoryPool.looper + aelf32_shdr->sh_size ;				
		}
		
	}

	//	As for why plus 2 here , check out ell.c
	looper = 2 ;
	lborder = EllElfMapRelocGetLborder ( ell->TextRel.elf32_rel , obid ) ;
	
	//	Resolve TextRel
 	if ( !EllReloc ( ell->TextRel.elf32_rel[obid] , EllTextReloc , looper , lborder , obid ) ) {
		EllHalFileClose ( file ) ;
		results = 0 ;
	}

	//	As for why plus 2 here , check out ell.c
	looper = 2 ;
	lborder = EllElfMapRelocGetLborder ( ell->DataRel.elf32_rel , obid ) ;
	
	//	Resolve DataRel
	if ( !EllReloc ( ell->DataRel.elf32_rel[obid] , EllDataReloc , looper , lborder , obid ) ) {
		EllHalFileClose ( file ) ;
		results = 0 ;
	}

	EllHalFileClose ( file ) ;

	return results ;

}

int EllGlobalLinker ( int obidborder ) {

	//	author : Jelo Wang
	//	notes : link multi-object files
	//	(C)TOK
	
	int results = 1 ;
	int obilooper = 0 ;

	EllLinker.obidborder = obidborder ;
	EllLinker.status = ELL_GLOBAL_LINKER ;
	
	for ( obilooper = 0 ; obilooper < EllLinker.obidborder ; obilooper ++ ) {

		int looper = 0 ;
		int lborder = 0 ;

		//	As for why plus 2 here , check out ell.c
		looper = 2 ;
		lborder = EllElfMapRelocGetLborder ( ell->TextRel.elf32_rel , obilooper ) ;
			
		//	Resolve textreloctab
		if ( !EllReloc ( ell->TextRel.elf32_rel[obilooper] , EllTextReloc , looper , lborder , obilooper ) ) {
			results = 0 ;
		}

		//	As for why plus 2 here , check out ell.c
		looper = 2 ;
		lborder = EllElfMapRelocGetLborder ( ell->DataRel.elf32_rel , obilooper ) ;
			
		//	Resolve textreloctab
		if ( !EllReloc ( ell->DataRel.elf32_rel[obilooper] , EllDataReloc , looper , lborder , obilooper ) ) {
			results = 0 ;
		}
		
	}

	return results ; 
	
}

static int EllReloc ( Elf32_Rel* reloctab , int (*EllRelocKernal) ( Elf32_Rel* elf32_rel , Elf32_Sym* elf32_sym , int ) , int looper , int lborder , int obid ) {

	//	author : Jelo Wang
	//	notes : Relocation
	//	(C)TOK
	
	int results = 1 ;

	for ( ; looper < lborder ; looper ++ ) {
		
		Elf32_Sym* elf32_sym = 0 ;
		Elf32_Rel* elf32_rel = (Elf32_Rel* )((int)reloctab+looper*sizeof(Elf32_Rel)) ;
		
		//	avoid rerelocation
		if ( 0xffffffff == elf32_rel->r_info ) continue ;

		elf32_sym = EllDynamicPoolLocalGetSymbol ( obid , ELF32_R_SYM(elf32_rel->r_info) ) ;

		if ( ELL_LOCAL_LINKER == EllLinker.status ) {

			//	global symbol detected.
			if ( SHN_UNDEF == elf32_sym->st_shndx && STB_GLOBAL == ELF32_ST_BIND(elf32_sym->st_info) ) {
				EllLog ( "Ell Local Linker Warning -> external symbol : '%s' expected.\n",elf32_sym->st_name) ;
				continue ;
			} 

			EllLog ( "Ell Local Linker Warning -> symbol : '%s' is relocated.\n",elf32_sym->st_name) ;
			
		} else if ( ELL_GLOBAL_LINKER == EllLinker.status ) {

			char* name = (char*)elf32_sym->st_name ;

			elf32_sym = EllDynamicPoolGlobalGetSymbol ( EllLinker.obidborder , name ) ;
	
			if ( !elf32_sym ) {
				elf32_sym = EllDynamicPoolLocalGetSymbol ( obid , ELF32_R_SYM(elf32_rel->r_info) ) ;
				EllLog ( "Ell Global Linker Error -> external symbol : '%s' expected.\n",name) ;
				results = 0 ;
				continue ;
			} else {
				EllLog ( "Ell Global Linker Warning -> external symbol : '%s' detected.\n",elf32_sym->st_name) ;
			}

		}

		if ( !EllRelocKernal ( elf32_rel , elf32_sym , obid ) ) results = 0 ;
		//	avoid rerelocation
		elf32_rel->r_info = 0xffffffff ;
		
	}

	return results ;

}

static int EllTextReloc ( Elf32_Rel* elf32_rel , Elf32_Sym* elf32_sym , int obid ) {

	//	author : Jelo Wang
	//	notes : relocation of segment text
	//	(C)TOK

	int relca = 0 ;
	int value = 0 ;
	
	int high_branch_offset = 0 ;
	int low_branch_offset = 0 ;
	
	int thumb1h8bit = 0 ;
	int thumb1l8bit = 0 ;
	int thumb2h8bit = 0 ;
	int thumb2l8bit = 0 ;

	Elf32_Shdr* elf32_shdr_reffer = 0 ;	

	switch ( ELF32_R_TYPE(elf32_rel->r_info) ) {
		
		case R_ARM_NONE : return 1 ;
		case R_ARM_THM_PC22 :

			//	16位指令集跳转地址由两个11位地址合并而成
			//	For THUMB-Instruction set branch:
			//	to allow for a reasonably large offset to the target subroutine,the bl or blx instruction is automatically
			//	translated by the assembler into a sequence of two 16 bit thumb instruction:
					
			//	(1) the first thumb instruction has H == 10 and suppiles the high part of the branch offset.this instruction
			//		sets up the subroutine call and is shared between the bl and blx forms.
			//	(2) the second thumb instruction has H == 11 (for bl) or H == 01 (for blx),it supplies the low part of the
			//		branch offset and causes the subroutine call to take place.
			//	摘自 ARM Architecture Reference Manual 
				
			//	S - P + A
			relca = (signed int)( elf32_sym->st_value - elf32_rel->r_offset ) / 2 - 2 ;
			
			//	the high part of the branch offset
			high_branch_offset = (0x3ff800 & relca) >> 11 ;
		
			//	the first thumb instruction
			thumb1h8bit = 0xf0 | ( (0x700 & high_branch_offset) >> 8 ) ;
			thumb1l8bit = 0xff & high_branch_offset ;
			
			//	the low part of the branch offset
			low_branch_offset = 0x7ff & relca ;
			 
			//	the second thumb instruction
			thumb2h8bit = 0xf8 | ( (0x700 & low_branch_offset) >> 8 ) ;  
			thumb2l8bit = 0xff & low_branch_offset ;  
		
			EllLinkerMemoryPool.pool[elf32_rel->r_offset+3] = thumb2h8bit ;   
			EllLinkerMemoryPool.pool[elf32_rel->r_offset+2] = thumb2l8bit ;	  
		
			EllLinkerMemoryPool.pool[elf32_rel->r_offset+1] = thumb1h8bit ;
			EllLinkerMemoryPool.pool[elf32_rel->r_offset] = thumb1l8bit ;
		
			high_branch_offset = 0 ;
			low_branch_offset = 0 ;
		
			return 1 ;

		case R_ARM_PC24 :
			
			//	对应ARM 32位指令: B、BL、BLX 
				
			//	To caculate the correct value of signed_immed_24,the linker must:
			//	1.	Form the base address for this branch instruction,This is the address of the instruction,plus 8. 
			//		in other words,this base address is equal to the PC value used by the instruction. 
			//	2.	Substract the base address from the target address to form a byte offset.This offset is always even,because all ARM
			//		instruction are word-aligned and all Thumb instruction are halfword-aligned.
			//	3.	If the byte offset is outside the range -33554432 to +33554430,use an alternative code-generation strategy or produce
			//		an error as appropriate.
			//	4.	Otherwise,set the signed_immed_24 field of the instruction to bits[25:2] of the byte offset,and the H bit of the instruction
			//		to bit[1] of the byte offset.
			//	摘自 ARM Architecture Reference Manual 

			relca = (signed int)( elf32_sym->st_value - elf32_rel->r_offset ) / 4 - 2 ;

			if ( 33554430 < relca ) {
				EllLog ( "EllReloc -> Error : offset is outside the range -33554432 to +33554430\n") ;
			} else if ( -33554432 > relca ) {
				EllLog ( "EllReloc -> Error : offset is outside the range -33554432 to +33554430\n") ;
			}
 		
			EllMemcpy ( (void*)((int)EllLinkerMemoryPool.pool+elf32_rel->r_offset) , &relca , 3 ) ;				
				 
			return 1 ;		

		case R_ARM_ABS32:

//			EllLog ( "name : %s\n" , elf32_sym->st_name ) ;
//			EllLog ( "value : %d\n" , elf32_sym->st_value ) ;
//			EllLog ( "size : %d\n" , elf32_sym->st_size ) ;
//			EllLog ( "info : %d\n" , elf32_sym->st_info ) ;
//			EllLog ( "other : %d\n" , elf32_sym->st_other ) ;
//			EllLog ( "shndx : %d\n" , elf32_sym->st_shndx ) ;		

			//	compiler puts some additional value in the object-files which is relocation-value for linker.
			//	linker using it for caculate symbol address in its section.
			value = (int)EllLinkerMemoryPool.pool [elf32_rel->r_offset] ;

			//	caculate absolute address of a symbol
			//	address = st_value + sh_addr + index * 4
			//	index of symbo in section , index holds with elf32_sym->st_other		
			
			elf32_shdr_reffer = (Elf32_Shdr* )EllElfMapNolSectGetWithIndex ( obid , elf32_sym->st_shndx ) ;	

			if ( !elf32_shdr_reffer ) return 0 ;

			elf32_sym->st_value = elf32_shdr_reffer->sh_addr + value ; 
			
			EllMemcpy( (void*)((int)EllLinkerMemoryPool.pool+elf32_rel->r_offset) , &elf32_sym->st_value , 4 ) ;
				
			return 1 ;
				
		default :
			EllLog ( "EllReloc -> Error : unrecognized ELF32_R_TYPE : %d\n" , ELF32_R_TYPE(elf32_rel->r_info) ) ;
	}
	
	return 0 ;


}

static int EllDataReloc ( Elf32_Rel* elf32_rel , Elf32_Sym* elf32_sym , int obid ) {

	//	author : Jelo Wang
	//	notes : relocation of segment data
	//	(C)TOK

	int relca = 0 ;
	int value = 0 ;
	
	Elf32_Shdr* elf32_shdr_entry = 0 ;
	Elf32_Shdr* elf32_shdr_reffer = 0 ;	

	switch ( ELF32_R_TYPE(elf32_rel->r_info) ) {
			
		case R_ARM_NONE : return 1 ;
		case R_ARM_ABS32:

//			EllLog ( "name : %s\n" , elf32_sym->st_name ) ;
//			EllLog ( "value : %d\n" , elf32_sym->st_value ) ;
//			EllLog ( "size : %d\n" , elf32_sym->st_size ) ;
//			EllLog ( "info : %d\n" , elf32_sym->st_info ) ;
//			EllLog ( "other : %d\n" , elf32_sym->st_other ) ;
//			EllLog ( "shndx : %d\n" , elf32_sym->st_shndx ) ;		

			//	Get .data elf32_shdr
			elf32_shdr_entry = (Elf32_Shdr* )EllElfMapNolSectGet ( obid , ".data" ) ;			

			if ( !elf32_shdr_entry ) return 0 ;
			
			{
				value = (int)EllLinkerMemoryPool.pool[elf32_shdr_entry->sh_entsize+elf32_rel->r_offset] ;
			}

			elf32_shdr_reffer = (Elf32_Shdr* )EllElfMapNolSectGetWithIndex ( obid , elf32_sym->st_shndx ) ;	

			if ( !elf32_shdr_reffer ) return 0 ;
			//	caculate absolute address of a symbol
			//	address = st_value + sh_addr 
			elf32_sym->st_value = elf32_shdr_reffer->sh_addr + value ; 

			EllMemcpy( (void*)((int)EllLinkerMemoryPool.pool+elf32_shdr_entry->sh_entsize+elf32_rel->r_offset) , &elf32_sym->st_value , 4 ) ;
				
			return 1 ;
				
		default :
			EllLog ( "EllReloc -> Error : unrecognized ELF32_R_TYPE : %d\n" , ELF32_R_TYPE(elf32_rel->r_info) ) ;
			
	}
	
	return 0 ;


}
