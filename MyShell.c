#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
char** tokenizeLineOfInput(char* input);
char* resize(char* input, int* argSize);
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
int execute(char** args)
{


  // Storage of word count metadata.
  // printf("%s", args[0]);
  int wordCount = args[256][256];
  args[wordCount + 1] = NULL;
  printf("%d", args[256][256])
    ;
  // // FROM SLIDES
  printf("pid:%d\n", (int)getpid());
  int rc = fork();
  if (rc < 0) {
    fprintf(stderr, " could not fork.\n");
    // MAY NEED TO FREE ARGS here too.
    // for (int n = 0; n <= 256; n++) {
    //   free(args[n]);
    // }
    // free(args);
    exit(1);
  }
  else if (rc == 0) {
    printf("Fork successful, child pid:%d\n", (int)getpid());
    for (int n = 0; args[n] != NULL; n++) {
      printf("%d", n);
      printf("%s", args[n]);
      execvp(args[n], args);
    }
    printf("Running command: %s\n", args[0]);

  }
  else {
    int wc = wait(NULL);
    printf("Parent pid: %d of child %d wc %d\n", (int)getpid(), rc, wc);
    return 0;
  }

  // FREE ARGS
  for (int n = 0; n <= 256; n++) {
    free(args[n]);
  }
  free(args);
  exit(1);
  return 0;
}


/**
  @brief gets the input from the prompt and splits it into tokens. Prepares the arguments for execvp
  @return returns char** args to be used by execvp
 */
char** parse(void)
{
  // *argumentCount = 0;
  char curArg[10000];

  // I needed a way to take user input that wasn't constrained to a specfic type of data. Scanf() with %s would not include single characters
  // and %c would not include full strings, ChatGPT suggested this function fgets()
  fgets(curArg, sizeof(curArg), stdin);
  char** args = (char**)malloc(sizeof(char**) * 257);
  for (int n = 0; n <= 256; n++) {
    *args = (char*)malloc(sizeof(char*) * 257);
  }
  args = tokenizeLineOfInput(curArg);
  printf("%d", execute(args));
  // if (execute(args) == 1) {
  //   exit(EXIT_SUCCESS);
  // }
  // printf("%s", tokenizeLineOfInput(curArg)[0]);
  return args;
}


/**
   @brief Main function should run infinitely until terminated manually using CTRL+C or typing in the exit command
   It should call the parse() and execute() functions
   @param argc Argument count.
   @param argv Argument vector.
   @return status code
 */
int main(int argc, char** argv) {
  while (1) {
    printf("MyShell> ");
    // I needed a way to check the current character to account for newlines, ChatGPT suggested using getchar()
    char curChar = getchar();
    if (curChar == '\n') {
      // argumentCount = 0;

      continue;
    }
    else {
      // I needed a way to reverse iteration of getchar() so ungetc() puts it back onto the stream 
      // to ensure fgets() has the whole line of input, ChatGPT suggested this function ungetc()
      ungetc(curChar, stdin);
    }
    // I needed a way to compare two strings and didn't want to manually compare them, ChatGPT suggested using strcmp()
    // printf("%s", *parse());
    char** args = parse();

    printf("%s", *args);
    if (args[0] != NULL) {

      if (strcmp(*args, "exit") == 0) {
        // break;
        for (int n = 0; n <= 256; n++) {
          free(args[n]);
        }
        free(args);
        return EXIT_SUCCESS;
      }
    }
  }
  return EXIT_SUCCESS;
}

// 2 Used chatGPT to determine how to use argumentCount correctly as a pointer.
char** tokenizeLineOfInput(char* input) {

  // printf("%s", input);

// Fixed with chatgpt.
  char** args = (char**)malloc(257 * sizeof(char*));
  for (int n = 0; n <= 256; n++) {
    args[n] = (char*)malloc(257 * sizeof(char));
  }
  int wordI = 0;
  int charI = 0;
  for (int n = 0; n < strlen(input); n++) {
    // I needed a way to have a delimiter to parse commands by checking sections of whitespace, chatGPT suggested isspace()
    // printf("%c", *(input + n));
    if (!isspace(*(input + n))) {
      // I used chatGPT 2 to debug and it suggested using
      // another iterator for the characters and resetting it
      // to 0 for each word which I have done a few lines below.
      args[wordI][charI++] = input[n];
      // ChatGPT 2 suggested I null terminate the words.
      args[wordI][charI] = '\0';
      // printf("%c", input[n]);
    }
    else {
      charI = 0;
      // args[wordI] = (char*)realloc(args, sizeof(args) + sizeof(char) * 256);
      if (n < strlen(input) - 1) {

        wordI++;
      }
      // ChatGPT 2 said I need to null terminate for execute to work.

    }
  }
  // Storing word count into the arguments.
  // This is a temporary fix until I figure out dynamic
  // resizing and reallocating.
  args[256][256] = wordI;
  return args;
}

// This resize function is intended for resizing
// the storage allocated for a string.
// However I also need to dynamically resize for the number of words.
char* resize(char* input, int* argSize) {
  // printf("%s", input);
  for (int n = 0; n < strlen(input); n++) {
    // printf("%c", *(input + n));
    if (n >= *argSize - 1) {
      *argSize += 10;
      input = (char*)realloc(input, *argSize);
    }
  }
  return input;
}