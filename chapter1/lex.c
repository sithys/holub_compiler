#include "lex.h"
#include <stdio.h>
#include <ctype.h>

#define INPUT_BUFFER_SIZE 128

char *yytext = "";
int yyleng = 0;
int yylineno = 0;

int lex() {
   static char input_buffer[INPUT_BUFFER_SIZE];
   char *current;

   current = yytext + yyleng;

   while(1) {
      while(!*current) {
         current = input_buffer;
         if(!fgets(input_buffer, INPUT_BUFFER_SIZE, stdin)) {
            *current = '\0';
            return EOI;
         }
         ++yylineno;
         while(isspace(*current)) {
            ++current;
         }
      }
      for( ; *current; ++current) {
         yytext = current;
         yyleng = 1;
         switch(*current) {
            case EOF: return EOI;
            case ';': return SEMI;
            case '+': return PLUS;
            case '*': return TIMES;
            case '(': return LP;
            case ')': return RP;
            case '\n':
            case '\t':
            case ' ': break;
            default:
               if(!isalnum(*current)) {
                  fprintf(stderr, "Ignoring illegal input <%c>\n", *current);
               } else {
                  while(isalnum(*current)) {
                     ++current;
                  }
                  yyleng = current - yytext;
                  return NUM_OR_ID;
               }
               break;
         }
      }
   }
}

static int Lookahead = -1;

void advance() {
   Lookahead = lex();
}

int match(int token) {
   if(Lookahead == -1) {
      advance();
   }
   return token == Lookahead;
}
