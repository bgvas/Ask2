/***************************************************************************//**
  @file     Ask2.c
  @authors  Basilis Georgoulas A.M 2118147 : & Ioannis Skoumpas A.M : 2118063
*******************************************************************************/

#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>



//  Decleration for cd build-in command
int cd_command(char **args);


//  List of builtin commands, followed by their corresponding functions.
char *builtin_str[] = {
  "cd"
};

int (*builtin_func[]) (char **) = {
  &cd_command
};

int num_builtins() {
  return sizeof(builtin_str) / sizeof(char *);
}


//Bultin command: change directory.

int cd_command(char **args)
{
  if (args[1] == NULL) {
    fprintf(stderr, "uth: expected argument to \"cd\"\n");
  } else {
    if (chdir(args[1]) != 0) {
      perror("uth");
    }
  }
  return 1;
}


/**
  @brief Launch a program and wait for it to terminate.
  @param args Null terminated list of arguments (including program).
  @return Always returns 1, to continue execution.
 */
int launch(char **args)
{
  pid_t pid, wpid; // proccess-ID type variables(int)
  int status;

  pid = fork();
  if (pid == 0) { // If creation of Child process, succeed 
    if (execvp(args[0], args) == -1) {
      perror("uthsh");
    }
    exit(EXIT_FAILURE);
  } else if (pid < 0) { // if creation of child process , failed
    perror("uthsh");
  } else {
    // Parent process
    do { //WUNTRACED return if a child has stopped
      wpid = waitpid(pid, &status, WUNTRACED); //Suspends the calling process until a child process ends or is stopped
    } while (!WIFEXITED(status) && !WIFSIGNALED(status)); //determines whether the child and the parent process ended normally.
  }

  return 1;
}


/**
   -Execute shell built-in or launch program.
   -args Null terminated list of arguments.
   -1 if the shell should continue running, 0 if it should terminate
 */
int execute(char **args)
{
  int i = 0;

  if (args[0] == NULL) { // An empty command was entered.
    printf("\n");
    return EOF;
  }else if(strcmp(args[0], builtin_str[i]) == 0) {
      return (*builtin_func[i])(args);
    }
  return launch(args);
}


#define RL_BUFSIZE 255

// Read a line of input from stdin.
char *read_line(void){
  int bufsize = RL_BUFSIZE;
  int position = 0;
  char *buffer = malloc(sizeof(char) * bufsize);
  int c;

  if (!buffer) {
    fprintf(stderr, "uthsh: allocation error\n");
    exit(EXIT_FAILURE);
  }
  while (1) {
    
    c = getchar();  // Read a character
 
    // If we hit EOF, replace it with a null character and return.
    if (c == EOF || c == '\n') {
      buffer[position] = '\0';
      return buffer;
    } else {
      buffer[position] = c;
    }
    position++;

    // If we have exceeded the buffer, reallocate.
    if (position >= bufsize) {
      bufsize += RL_BUFSIZE;
      buffer = realloc(buffer, bufsize);
      if (!buffer) {
        fprintf(stderr, "uthsh: allocation error\n");
        exit(EXIT_FAILURE);
      }
    }
  }
}


#define RL_BUFSIZE 255

// read a command without arguments
char *read_command(void)
{
  int bufsize = RL_BUFSIZE;
  int position = 0;
  char *buffer = malloc(sizeof(char) * bufsize);
  int c;

  if (!buffer) {
    fprintf(stderr, "uthsh: allocation error\n");
    exit(EXIT_FAILURE);
  }

  while (1) {
    
    c = getchar(); // Read a character

	// If character is space then return
    if (c == '\n' || c == 32) {
      buffer[position] = '\0';
      return buffer;
    } else {
      buffer[position] = c;
    }
    position++;
  }
}



#define TOK_BUFSIZE 64
#define TOK_DELIM " \t\r\n\a"
/**
   -Split a line into tokens (very naively).
   -Null-terminated array of tokens.
 */
char **split_line(char *line)
{
  int bufsize = TOK_BUFSIZE, position = 0;
  char **tokens = malloc(bufsize * sizeof(char));
  char *token = malloc(bufsize * sizeof(char));

  if (!tokens) {
    fprintf(stderr, "uthsh: allocation error\n");
    exit(EXIT_FAILURE);
  }

  token = strtok(line, TOK_DELIM);
  while (token != NULL) {
    tokens[position] = token;
    position++;

    if (position >= bufsize) {
      bufsize += TOK_BUFSIZE;
      tokens = realloc(tokens, bufsize * sizeof(char*));
      if (!tokens) {
        fprintf(stderr, "uthsh: allocation error\n");
        exit(EXIT_FAILURE);
      }
    }

    token = strtok(NULL, TOK_DELIM);
  }
  tokens[position] = NULL;
  return tokens;
}



// Loop getting input and executing it.
void loop(void)
{
  char *line;
  char **args;
  int status;

  do {
    printf("uthsh1> ");
    line = read_command();
    args = split_line(line);
    status = execute(args);

    free(line);
    free(args);
  } while (status != EOF);

do {
    printf("uthsh2> ");
    line = read_line();
    args = split_line(line);
    status = execute(args);

    free(line);
    free(args);
  } while (status != EOF);
}


// The Main Function
int main(int argc, char **argv)
{
  loop();
  return EXIT_SUCCESS;
}

