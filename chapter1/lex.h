#define EOI 0
#define SEMI 1
#define PLUS 2
#define TIMES 3
#define LP 4
#define RP 5
#define NUM_OR_ID 6

extern char *yytext;
extern int yyleng;
extern int yylineno;

int lex(void);
void advance(void);
int match(int);
