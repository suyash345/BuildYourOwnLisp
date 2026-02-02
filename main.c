#include <stdio.h>
#include <stdlib.h>
#include "mpc.h"

/* If WINDOWS */
#ifdef _WIN32
#include <string.h>

static char buffer[2048];

/* Fake readline function */
char* readline(char* prompt) {
  fputs(prompt, stdout);
  fgets(buffer, 2048, stdin);
  char* cpy = malloc(strlen(buffer)+1);
  strcpy(cpy, buffer);
  cpy[strlen(cpy)-1] = '\0';
  return cpy;
}

/* Fake add_history function */
void add_history(char* unused) {}

/* ELSE */
#else
#include <editline/readline.h>
#endif

int main(int argc, char** argv) {
  mpc_parser_t* num = mpc_new("num");
  mpc_parser_t* op = mpc_new("op");
  mpc_parser_t* ex = mpc_new("ex");
  mpc_parser_t* lispy = mpc_new("lispy");

mpca_lang(MPCA_LANG_DEFAULT,
  "                                       \
    num   : /-?[0-9]+/ ;                  \
    op : '+' | '-' | '*' | '/' ;          \
    ex     : <num> | '(' <op> <ex>+ ')' ; \
    lispy    : /^/ <op> <ex>+ /$/ ;       \
  ",
  num,op, ex, lispy);

  puts("Lispy Version Bob");
  puts("Press Ctrl+c to Exit\n");
  
  while(1) {
    char* input = readline("lispy> ");
    add_history(input);
    mpc_result_t result;
    if(mpc_parse("<stdin>",input,lispy,&result)) {
      // print the output that is read in.
      mpc_ast_print(result.output);
      mpc_ast_delete(result.output);
    } 
    else {
      // print the error otherwise
      mpc_err_print(result.error);
      mpc_err_delete(result.error);

    }


    free(input);
  }
  mpc_cleanup(4,num,op,ex,lispy);

  return 0;
}