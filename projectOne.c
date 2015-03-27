/*
 * Class: CS 446
 * Assignment: Homework One
 * File: projectOne.c
 * Date: 9/17/14
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

/*
 * Name: readInFile
 * 
 * Input Parameters: file to open, character array to store info
 * 
 * Outputs Returnted: integer for file open success 
 * 
 * Process: this function opens the file provided in the command line if
 * it is found and returns a ( 1 ) for successful file open or a ( 0 ) 
 * otherwise.
 * 
 */
int readInFile( FILE *, char [] );

/*
 * Name: countNumberOfRows
 * 
 * Input Parameters: character array that has family names, integer array
 * to store the indexes of rows
 * 
 * Outputs Returnted: integer with the number of rows
 * 
 * Process: this function takes in the family array which contains all of
 * the names in the family. the function runs through the array and gets
 * the index of where ever there is a new row.  The index of the row is
 * stored into the rows array. each element in the rows array holds the 
 * index of each row in the family array. the index of the rows is later
 * used to go from row to row easily.  
 * 
 */
int countNumberOfRows( char [ ], int [ ] );

/*
 * Name: checkIfMarried
 * 
 * Input Parameters: character array that has family names, character of
 * the name to check if they married, the integer array with index of rows,
 * integer with the number of rows
 * 
 * Outputs Returnted: integer with the status of marriage, index of the 
 * person is returned if they are married, if they are not married then
 * a ( -1 ) is returned.
 * 
 * Process: this function takes the family array and loops through each
 * row to check to see if a person is married. the name that being checked
 * is used to compare the first element of each row. if the name being 
 * checked is found on one of the first elements of a row then the index
 * at where it was found is returned. this index is used later to find
 * number of kids.  if after looping through all of the rows the name is 
 * not found at the first element in any row then a ( -1 ) is returned 
 * to state that the person is not married.
 * 
 */ 
int checkIfMarried( char [ ], char, int [ ], int );

/*
 * Name: getNumberOfKids
 * 
 * Input Parameters: character array that has family names, integer of
 * the index of the person who is to be checked to number of kids.
 * 
 * Outputs Returnted: integer containing the number of kids
 * 
 * Process: this function gets the index of the person to be checked 
 * and increments the index until it gets to the spot where the number 
 * of kids is listed. the function converts that character into an 
 * integer and returns it.
 * 
 */
int getNumberOfKids( char[ ], int );

/*
 * Name: generateFamilyTree
 * 
 * Input Parameters: character array with family names, character with
 * the name of current person, integer for the current generation, integer
 * array with row indexes, integer for number of rows.
 * 
 * Outputs Returnted: none
 * 
 * Process: this function checks to see if the current person is married.
 * if they are not married then their name is printed with appropirate 
 * number of tabs for their generation. however, if they are married then
 * the current person & their spouse names are printed. after that the 
 * current person is checked to see if they have any children. if they 
 * have no children then nothing is done. if they do have children then
 * the generation is incremented & a for loop is used to loop through 
 * all the children. in the for loop a fork is created with the child
 * process recursively calling the generateFamilyTree function with the
 * first childs name. the parent process waits for the child process to
 * finish. also inside the for loop the index of the child is incremented
 * in order to go to the next child to be recursively called.
 * 
 */
void generateFamilyTree( char [ ], char, int, int [ ], int );

int main(int argc, char **argv)
{
	FILE *fin = fopen( argv[ 1 ], "r" ); // open file
	char family[ 256 ]; // contains family names ( info from file )
	int rows[ 256 ]; // contains indexes of each row
	int rowCount; // contains number of rows 
	int fileStatus; // contains status of file ( open properly or not )
	
	// open file & get fileStatus
	fileStatus = readInFile( fin, family );
	
	// if fileStatus good continue with the rest of the program
	if( fileStatus == 1 )
	{
		// get the number of rows 
		rowCount = countNumberOfRows( family, rows );
	
		// generate the family tree
		generateFamilyTree( family, family[ 0 ], 0, rows, rowCount );
	}
	
	return 0;
}

int readInFile( FILE *fin, char array[] )
{
	// if file not found output error
	if( fin == 0 )
	{
		printf( "ERROR!\n" );
	}
	// else open the file
	else
	{		
		char element; // contains character from file
		int index = 0; // index of the family array
		
		// while the end of file not reached continue to read in characters
		while( ( element = fgetc( fin ) ) != EOF )
		{
			// if the character is not a space then load into array 
			if( element != ' ' )
			{
				array[ index ] = element;
				index++;
			}
		}
		
		// place null character at the end of the array
		array[ index ] = '\0';
			
		// close the file
		fclose( fin );	
		
		// return file status good
		return 1;
	}
	
	// return file status bad
	return 0;
}

int countNumberOfRows( char array[ ], int rows[ ]  )
{
	int index = 0; // index used in traversing family array
	int rowCount = 0; // index of the row array
	
	// set first element in the row array equal to the first row index
	rows[ rowCount ] = 0;
	
	// increment to next element for row array
	rowCount++;
	
	// while not all the rows are found
	while( array[ index ] != '\0' )
	{
		// if an end line is found then the next character is the start of a new line
		if( array[ index ] == '\n' )
		{
			// store the index into the row array
			rows[ rowCount ] = ++index;
			
			// go to next element in row array
			rowCount++;
		}
		
		// go to next character in family array
		index++;
	}
	
	// don't count the last row ( extra row with no data )
	return --rowCount;
}

int checkIfMarried( char family[ ], char name, int rows[ ], int numberOfRows )
{
	int i; // used to loop through rows
	
	// loop through the rows to find if person is married
	for( i = 0; i < numberOfRows; i++ )
	{
		// use the indexes of each row to go directly to each row in the
		// family array. once at the first element of the row check to
		// see if that element is equal to the person given.
		if( family[ rows[ i ] ] == name )
		{
			// return the index of the row is person is married.
			return rows[ i ];
		}
	}
	
	// if person not married return ( -1 )
	return -1;
}

int getNumberOfKids( char family[ ], int indexOfPerson )
{
	int numberOfKids; // used to convert character to integer
	
	// get the number of kids in the family array and convert to integer
	numberOfKids = ( family[ ++indexOfPerson ] - '0' );
	
	// return the number of kids as an integer
	return numberOfKids;
}

void generateFamilyTree( char family[ ], char name, int generation, int rows[ ], int numberOfRows )
{	
	int indexOfPersonFound; // used to check if person married 
	int tabs; // number tabs to loop through
	int numberOfKids; // number of kids of the current person
	int i; // loop through number of kids
	int PID; // process i.d.
	int indexOfChild; // index of the child
	
	// check to see if the current person is married
	indexOfPersonFound = checkIfMarried( family, name, rows, numberOfRows );
	
	// if the current person is married the print name with spouse & check
	// to see if they have any children
	if( indexOfPersonFound != -1 )
	{
		// print appropriate tabs for number of generations
		for( tabs = 0; tabs < generation; tabs++ )
		{
			printf( "\t" );
		} 		
		
		// print current person with their spouse
		printf( "%c", family[ indexOfPersonFound ] );
		printf( "-%c\n", family[ ++indexOfPersonFound ] );
		
		// get the number of children
		numberOfKids = getNumberOfKids( family, indexOfPersonFound );
		
		// if the current person has any kids then loop through
		// kids and call generateFamilyTree with each of the children
		if( numberOfKids > 0 )
		{
			// increment the generation
			generation++;
			
			// get the index of the first child
			indexOfChild = indexOfPersonFound + 2;
			
			// loop through all of the children
			for( i = 0; i < numberOfKids; i++ )
			{
				// fork to create new processes
				PID = fork();
				
				// child process
				if( PID == 0 )
				{
					// call generate family tree with a child
					generateFamilyTree( family, family[ indexOfChild ], 
										generation, rows, numberOfRows );
					return;
				}
				// parent process
				else
				{
					// wait for child process to be done
					waitpid( PID, NULL, 0);
				}
				
				// increment to the next child
				indexOfChild++;
			}
		}
	}
	// current person not married so just print their name
	else
	{	
		// print appropriate tabs for number of generations
		for( tabs = 0; tabs < generation; tabs++ )
		{
			printf( "\t" );
		} 
		
		// print the name of the current person.
		printf( "%c\n", name );
	}	
}
