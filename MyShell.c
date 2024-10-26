#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
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

  printf("%d", args[256][256]);
  // // FROM SLIDES
  // printf("hello world (pid:%d)\n", (int)getpid());
  // int rc = fork();
  // if (rc < 0) {
  //   // fork failed; exit
  //   fprintf(stderr, "fork failed\n");
  //   exit(1);
  // }
  // else if (rc == 0) {
  //   // child (new process)
  //   printf("hello, I am child (pid:%d)\n", (int)getpid());
  //   char* myargs[3];
  //   myargs[0] = strdup("wc"); // program: "wc" (word count)
  //   myargs[1] = strdup("p3.c"); // argument: file to count
  //   myargs[2] = NULL; // marks end of array
  //   execvp(myargs[0], myargs); // runs word count
  //   printf("this shouldn't print out");
  // }
  // else {
  //   // parent goes down this path (original process)
  //   int wc = wait(NULL);
  //   printf("hello, I am parent of %d (wc:%d) (pid:%d)\n",
  //          rc, wc, (int)getpid());
  // }
  return 0;
}


/**
  @brief gets the input from the prompt and splits it into tokens. Prepares the arguments for execvp
  @return returns char** args to be used by execvp
 */
char** parse(void)
{
  // int* argumentCount = (int*)malloc(sizeof(int));
  // *argumentCount = 0;
  char curArg[10000];

  // I needed a way to take user input that wasn't constrained to a specfic type of data. Scanf() with %s would not include single characters
  // and %c would not include full strings, ChatGPT suggested this function fgets()
  fgets(curArg, sizeof(curArg), stdin);
  char** args = tokenizeLineOfInput(curArg);

  execute(args);
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
  // int EXIT_SUCCESS = 1;
  // Using 11 because I want ten characters plus the \0 character allowed.
  int defaultArgSize = 11;
  int* argSize = (int*)malloc(sizeof(int));
  *argSize = defaultArgSize;
  // ONLY USE IF RESIZING DYNAMICALLY. TODO FIX DYNAMIC RESIZING.
  // char* input = (char*)malloc(sizeof(char) * (*argSize));
  const char* exit1 = "exit";
  const char cont = '\0';

  while (1) {
    printf("MyShell> ");

    // printf("%s", argv[0]);
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

    // printf("%d", sizeof(input));
    // printf("%d", sizeof("aaa"));

    // This seems to cause a segmentation fault.
    // fgets(resize(input, &argSize), sizeof(resize(input, &argSize)), stdin);

    // printf("MyShell> %s=%s\n", input, exit1);
    // if (strstr(input, ""))
      // I needed a way to compare two strings and didn't want to manually compare them, ChatGPT suggested using strcmp()
    // printf("%s", *parse());
    // printf("%s", parse());
    if (strcmp(*parse(), "exit") == 0) {
      // EXIT_SUCCESS = 0;
      break;
    }
    // execute(parse());
    // printf("%d", argumentCount);
  }
  // free(argSize);
  // free(input);
  // free(argumentCount);
  // return EXIT_SUCCESS;
  return 0;
}

// 2 Used chatGPT to determine how to use argumentCount correctly as a pointer.
char** tokenizeLineOfInput(char* input) {

  // printf("%s", input);

// Fixed with chatgpt.
  char** args = (char**)malloc(1 * sizeof(char*));
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
      // args[wordI][charI + 1] = '\0';
      // printf("%c", input[n]);
    }
    else {
      charI = 0;
      // args[wordI] = (char*)realloc(args, sizeof(args) + sizeof(char) * 256);
      wordI++;
    }
  }
  args[256][256] = wordI;
  // FOR TESTING.
  // for (int n = 0; n < 4; n++) {

  //   printf("%s", args[n]);
  // }
  return args;
}

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