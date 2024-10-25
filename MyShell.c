#include <stdio.h>
#include <string.h>
#include <ctype.h>
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
  int *argumentCount = 0;
  const char *command = getenv("MY_ARG");

 while (1) {
  printf("MyShell> ");
  
  // printf("%s", argv[0]);
  // I needed a way to check the current character to account for newlines, ChatGPT suggested using getchar()
  char curChar = getchar();
  if (curChar == '\n') {
    // argumentCount = 0;
    
    continue;
  } else {
    // I needed a way to reverse iteration of getchar() so ungetc() puts it back onto the stream 
    // to ensure fgets() has the whole line of input, ChatGPT suggested this function ungetc()
    ungetc(curChar, stdin);
  }
  // I needed a way to take user input that wasn't constrained to a specfic type of data. Scanf() with %s would not include single characters
  // and %c would not include full strings, ChatGPT suggested this function fgets()
  fgets(input, sizeof(input), stdin);
  
  countArguments(&input, &argumentCount);
  // printf("MyShell> %s=%s\n", input, exit1);
  if (strstr(input, ""))
  // I needed a way to compare two strings and didn't want to manually compare them, ChatGPT suggested using strcmp()
    if (strcmp(input, "exit\n") == 0) {
      EXIT_SUCCESS = 0;
      break;
    }
      // printf("%d", argumentCount);
  }

  return EXIT_SUCCESS;
}

// 2 Used chatGPT to determine how to use argumentCount correctly as a pointer.
void countArguments(char *input, int *argumentCount) {
  *argumentCount = 0;
  for (int n = 0; n < strlen(input); n++) {
    // printf("%d", n);
    if (!isspace(input[0]) && n == 0) {
      *argumentCount = *argumentCount + 1;
      
    } else if (n < strlen(input) - 1 && isspace(input[n]) && !isspace(input[n + 1])) {
      *argumentCount = *argumentCount + 1;
      
    }
  }
    // printf("%d", *argumentCount);
}