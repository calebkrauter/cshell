#include <stdio.h>
#include <string.h>
/***************************************************************************
  @file         MyShell.c
  @author       Juhua Hu
  @author       Caleb Krauter

*******************************************************************************/




/**
DONOT change the existing function definitions. You can add functions, if necessary.
*/



/**
  @brief Fork a child to execute the command using execvp. The parent should wait for the child to terminate
  @param args Null terminated list of arguments (including program).
  @return returns 1, to continue execution and 0 to terminate the MyShell prompt.
 */
int execute(char **args)
{

}


/**
  @brief gets the input from the prompt and splits it into tokens. Prepares the arguments for execvp
  @return returns char** args to be used by execvp
 */
char** parse(void)
{

}


/**
   @brief Main function should run infinitely until terminated manually using CTRL+C or typing in the exit command
   It should call the parse() and execute() functions
   @param argc Argument count.
   @param argv Argument vector.
   @return status code
 */
int main(int argc, char **argv) {
  int EXIT_SUCCESS = 1;
  char input[250];
  const char *exit1 = "exit";
  const char cont = '\0';

 while (1) {
  printf("MyShell> ");
  // ChatGPT reccomended using getchar()
  char curChar = getchar();
  if (curChar == '\n') {
      
    continue; 
  } else {
    // ChatGPT reccomended this function ungetc()
    ungetc(curChar, stdin);
  }
  // ChatGPT reccomended this function fgets()
  fgets(input, sizeof(input), stdin);
  // printf("MyShell> %s=%s\n", input, exit1);
  // ChatGPT reccomended using strcmp()
    if (strcmp(input, "exit\n") == 0) {
      EXIT_SUCCESS = 0;
      break;
    }
  }

  return EXIT_SUCCESS;
}
