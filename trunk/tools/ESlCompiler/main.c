
# include "stdio.h"
# include "string.h"
# include "ellhal.h"
# include "ellloader.h"

//char file1 [ 256 ] = { 0 } ;
//char file2 [ 256 ] = { 0 } ;
char file1 [ 256 ] = { 0 } ;
char file2 [ 256 ] = { 0 } ;

static void help () {
	
	//	author : Jelo Wang
	//	since : 20110224
	//	(c)TOK	
 
	printf ( 	"\
\t - ESL Compiler 1.0.0 -\n\
\t - Í»¿Ç¿ªÔ´ Techniques of Knowledge -\n\n\
Format : \n\
\t ESlCompiler file1 file2\n\
Example :\n\
\t ESlCompiler test.o test.axf\n\
\n\
More :\n\
\t For further more info you can visit the offical site\n\
\t http://www.tok.cc \n\
" ) ;
	
}

int main ( int argc , char* argv [] ) {

	//	author : Jelo Wang
	//	since : 20110224
	//	(c)TOK	
	
	int file1open = 0 ;
	int file2open = 0 ;

	int infile = 0 ;
	int outfile = 0 ;
	int fileoffset = 0 ;
	
	if ( !argc || argc < 2 ) {

		help () ;
		return 0 ;
	
	}	

	strcpy ( file1 , argv[1] ) ;
	strcpy ( file2 , argv[2] ) ;		

	file1open = EllHalFileOpen ( file1 , ELLHAL_READ_OPEN ) ;
	file2open = EllHalFileOpen ( file2 , ELLHAL_READW_OPEN ) ;

	if ( !file1open ) {
		printf ( "Can not find the file %s\n" , file1 ) ;
		return 0 ;
	}

	if ( !file2open ) {
		printf ( "Can not find the file %s\n" , file2 ) ;
		return 0 ;
	}
	
	EllInstall ( file1open , file2open ) ;	
	EllHalFileClose ( file1open  ) ;
	EllHalFileClose ( file2open  ) ;
	
	strcat ( file1 , ".esl" ) ;

	//	a linked ELF file
	infile = EllHalFileOpen ( file2 , ELLHAL_READ_OPEN ) ;
	//	ESL file
	outfile = EllHalFileOpen ( file1 , ELLHAL_CREW_OPEN ) ;
	
	while ( !EllHalFileEnd ( infile ) ) {
		
		int value = 0 ;
		EllHalFileRead ( infile , &value , sizeof(int) , 1 ) ;
		EllHalFileWrite ( outfile , &value , sizeof(int) , 1 ) ;

	}

	EllHalFileClose ( infile ) ;
	EllHalFileClose ( outfile ) ;

	printf ( "'%s' generated.\n" , file1 ) ;
	
	return 1 ;

}
