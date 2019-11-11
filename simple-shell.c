//Drew Bies
//simple-shell.c
//CPSC 346 02

/* this program is an example of a simple shell.
	the command history prints up to 9 of the recent commands
	!! executes the last used command
	!n executes the nth last command
	exit will exit the program
	^d will exit the program
*/

#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>


#define MAX_LINE 80 	// 80 chars per line, per command, should be enough
#define MAX_COMMANDS 9 	// size of history

char history[MAX_COMMANDS][MAX_LINE]; 	//the array used to store history commands.
char display_history [MAX_COMMANDS][MAX_LINE];
/*the array used for "printf" to display history nicely. Remove special characters like "\n" or "\0"*/

int command_count = 0;

// Add the most recent command to the history
void addToHistory(char inputBuffer[])
{
	int i = 0, length = strlen(inputBuffer); // serpate indexing for display_buffer
	
	command_count++;
	
	strcpy(history[(command_count - 1) % MAX_COMMANDS], inputBuffer);

	while(i < length)
	{
		if(inputBuffer[i] != '\n' && inputBuffer[i] != '\0')// no special chars for display_buffer
			display_history[(command_count - 1) % MAX_COMMANDS][i] = inputBuffer[i];
			
		i++;
	}
	display_history[(command_count - 1) % MAX_COMMANDS][i] = '\0'; // overwrite the rest of the chars
	
	return;
}

/**
 * The setup function below will not return any value, but it will just: read
 * in the next command line; separate it into distinct arguments (using blanks as
 * delimiters), and set the args array entries to point to the beginning of what
 * will become null-terminated, C-style strings.
 */

int setup(char inputBuffer[], char *args[],int *background)
{
    int length,		// # of characters in the command line
	i,				// loop index for accessing inputBuffer array
	n,				// index of requested command number
	numOfArgs = 0;	// number of arguements
	
	
    // read what the user enters on the command line 
	do
	{
		printf("osh>");
		fflush(stdout);
		length = read(STDIN_FILENO,inputBuffer,MAX_LINE);
	}
	while (inputBuffer[0] == '\n'); // swallow newline characters


    if (length == 0)
        exit(0);	// ^d was entered, end of user command stream

    if (length < 0)
	{
		perror("error reading the command");
		exit(-1); // terminate with error code of -1
    }

	// Check if they are using history
	if (inputBuffer[0] == '!')
	{
		if(inputBuffer[1] == '!') // !! execute most recent command
			n = 1;
		else if(isdigit(inputBuffer[1]))
			n = inputBuffer[1] - '0' ; // execute the nth command
		
		if(n > command_count || n < 1 || command_count == 0) 
			perror("No such command in history.\n");
		else 
			strcpy(inputBuffer, history[(command_count - n) % MAX_COMMANDS]); // copy desired command to inputBuffer
	}
	
	//Parse the contents of inputBuffer
    for (i = 0; i < length; i++)
	{
		/* examine every character in the inputBuffer */
		
        switch (inputBuffer[i])
		{
			case '\n': // should be the final char examined
				//fill in your code here, set up the last item args[x] ==NULL;
				/* no more arguments to this command */
				args[numOfArgs] = NULL;
			case ' ':
			case '\t': // argument separators
				//fill in your code here, set up args
				inputBuffer[i] = '\0';
				break;
	    	default: // some other character
				
				 //fill in your code here,
				if(inputBuffer[i - 1] == '\0' && inputBuffer[i] != '&')
					args[numOfArgs++] = &inputBuffer[i];
				
				if(inputBuffer[i] == '&')
					*background = 1;
				
		} // end of switch
	} // end of for
	
	return 1;
} // end of setup routine


int main(void)
{
	char inputBuffer[MAX_LINE]; 	// buffer to hold the command entered
	int background;             	// equals 1 if a command is followed by '&'
	char *args[MAX_LINE/2 + 1];	    // command line (of 80) has max of 40 arguments
	pid_t child;            		// process id of the child process

	//define your local variables here, at the beginning of your program.

	int shouldrun = 1;

	

    while(shouldrun) // Program terminates normally inside setup
	{
		background = 0;

		shouldrun = setup(inputBuffer, args, &background);       // get next command
		
		//int i;
		//for(i = 0; args[i] != NULL; i++)
			//printf("args: %d\t%s\n", i, args[i]);

		// fill in your code here Part I
		/* if the user typed in "exit", the shell program will return (or terminate).
		* Call strncmp(str1,str1,count). The function call will return 0 if str1 == str2.
		* "count" is the number of characters we use to compare.
		*/
		if(strncmp(inputBuffer, "exit", 4) == 0)
			exit(0);
		
		

		// fill in your code here Part II
		/* if the user typed in "history", the shell program will display the history commands.
		* you will use "printf" to print the display_history
		* after you display all the array, this command is done.
		* Your program should go to read again, which means calling the "setup" function.
		*/
		
		if(strncmp(inputBuffer, "history", 7) == 0)
		{
			if(command_count > 0)
			{
				int i, // for loop 
				n,		// number of commands to print
				index = 0; // helper for indexing array
				
				if(command_count > MAX_COMMANDS)
					n = MAX_COMMANDS;
				else
					n = command_count;
				
				for(i = n; i > 0; i--)
				{
					printf("%d\t%s\n", i, display_history[(command_count + index) % n]);
					index++;
				}
			}
			else
				perror("No commands in history.\n");
			continue;
		}
		else
			addToHistory(inputBuffer); // add the command to the history

		if (shouldrun)
		{
			/* creates a duplicate process! */
			/* pid<0  error
			*  pid == 0, it is the child process. use the system call execvp(args[0],args);
			*  pid > 0, it is the parent. Here you need consider it is foreground or background
			*/
			
			if((child = fork()) == 0) // child
				execvp(args[0], args);			
			else if(child > 0) // parent
				if(!background) while(child != wait(NULL)); // wait for child if background is true
			else // error
			{
				perror("Could not create new process.\n");
				return -1;
			}

		}
    }

	return 0;
} // end of main
