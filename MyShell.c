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
  Refs:
  Anytime chatgpt is mentioned chatGPT refers to the first link and chatGPT 2 refers to the second conversation.
  chatGPT: https://chatgpt.com/share/6719ab0e-7080-8004-b93f-a654bc7b3121
  chatGPT 2: https://chatgpt.com/share/671dde24-b7cc-8004-8fed-63eac01c3653
  Hello world in C: https://www.youtube.com/watch?v=9VE7p-he4fA&t=272s
  Setting up GCC: https://www.youtube.com/watch?v=o8XEzZBjQTs
  chatgpt for setting up GCC: https://chatgpt.com/share/6719c276-1fb4-8004-b99a-3a6d7afeb838
  ScanF for input https://www.w3schools.com/c/c_user_input.php
  fgets documentation: https://en.cppreference.com/w/c/io/fgets
  null char: https://stackoverflow.com/questions/1296843/what-is-the-difference-between-null-0-and-0
  Modulus calculator: https://www.calculatorsoup.com/calculators/math/modulo-calculator.php
  Used this video for malloc and realloc https://www.youtube.com/watch?v=j1jo8QQT7zw

  ChatGPT also assisted heavily in debugging and memory management issuse.

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
  int wordCount = args[256][256];
  // Make sure that it doesn't try to execute commands that don't exist. ChatGPT 2 said I needed a null termination.
  args[wordCount + 1] = NULL;
  // An exit flag.
  int exiting = 0;
  char* exitSignal = (char*)malloc(sizeof(char) * 5);
  exitSignal = args[0];
  int failedExecute = 0;
  int rc = fork();

  if (rc < 0) {
    fprintf(stderr, " could not fork.\n");
    exit(0);
  }
  else if (rc == 0) {
    for (int n = 0; args[n] != NULL; n++) {
      failedExecute = execvp(args[n], args);
    }
    // I had put this in the else statement and chatGPT 2 helped me to
      // debug and move it here.
    if (failedExecute == -1 && strcmp(exitSignal, "exit") != 0) {

      printf("error executing command: No such file or directory\n");
    }
    else if (strcmp(exitSignal, "exit") == 0) {
      exiting = 1;
    }
  }
  else {
    // Ensures that the parent process pauses before returning to the parent process from the executing child.
    int wc = wait(NULL);
    exiting = 1;
    return 0;
  }

  // FREE ARGS
  for (int n = 0; n <= 256; n++) {
    free(args[n]);
  }
  free(args);

  // I needed a way to show the pwd, chatGPT 2 suggested this.
  char wd[1024];
  if (exiting == 1 && getcwd(wd, sizeof(wd)) != NULL) {
    printf("exiting");
    printf("%s", wd);
  }
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

  // I needed a way to take user input that wasn't constrained to a specfic type of data. 
  // Scanf() with %s would not include single characters
  // and %c would not include full strings, ChatGPT suggested this function fgets()
  fgets(curArg, sizeof(curArg), stdin);
  // I initalize 257 to meet the minimum of 255 arguments and to leave space for metadata to store the number of words.
  char** args = (char**)malloc(sizeof(char**) * 257);
  for (int n = 0; n <= 256; n++) {
    *args = (char*)malloc(sizeof(char*) * 257);
  }
  args = tokenizeLineOfInput(curArg);
  execute(args);
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
    char** args = parse();

    if (args[0] != NULL) {
      if (strcmp(*args, "exit") == 0) {
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

/**
   @brief A helper function that tokenizes the user input and counts the words storing the result as a number into the array near the end at
  256x256.
   @param input user input.
   @return 2D array of arguments.
 */
char** tokenizeLineOfInput(char* input) {

  // Debugged with chatgpt.
  char** args = (char**)malloc(257 * sizeof(char*));
  for (int n = 0; n <= 256; n++) {
    args[n] = (char*)malloc(257 * sizeof(char));
  }
  int wordI = 0;
  int charI = 0;
  for (int n = 0; n < strlen(input); n++) {
    // I needed a way to have a delimiter to parse commands by checking sections of whitespace, chatGPT suggested isspace()
    if (!isspace(*(input + n))) {
      // I used chatGPT 2 to debug and it suggested using
      // another iterator for the characters and resetting it
      // to 0 for each word which I have done a few lines below.
      args[wordI][charI++] = input[n];
      // ChatGPT 2 suggested I null terminate the words.
      args[wordI][charI] = '\0';
    }
    else {
      charI = 0;
      if (n < strlen(input) - 1) {
        wordI++;
      }
    }
  }
  // Storing word count into the arguments.
  // This is a temporary fix until I figure out dynamic
  // resizing and reallocating.
  args[256][256] = wordI;
  return args;
}

// UNFINISHED
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