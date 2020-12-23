#include <stdio.h>
#include <stdarg.h>
#include "plain.h"
#include "lex.h"

int legal_lookahead(int first_arg, ...) {
    va_list args;
    int tok;
    int lookaheads[MAXFIRST], *p = lookaheads, *current;
    int error_printed = 0;
    int rval = 0;

    va_start(args, first_arg);

    if(!first_arg) {
        if(match(EOI)) {
            rval = 1;
        }
    } else {
        *p = first_arg;
        p++;
        while((tok = va_arg(args, int)) && p < &lookaheads[MAXFIRST]) {
            *p = tok;
            p++;
        }
        while(!match(SYNCH)) {
            for(current = lookaheads; current < p; current++) {
                if(match(*current)) {
                    rval = 1;
                    va_end(args);
                    return rval;
                }
            }
            if(!error_printed) {
                fprintf(stderr, "Line %d: Syntax error\n", yylineno);
                error_printed = 1;
            }
            advance();
        }
    }
    
    va_end(args);
    return rval;
}

void statements() {
    expression();
    if(match(SEMI)) {
        advance();
    } else {
        fprintf(stderr, "%d: Inserting missing semicolon\n", yylineno);
    }

    if(!match(EOI)) {
        statements();
    }
}

void expression() {
    term();
    expr_prime();
}

void expr_prime() {
    if(match(PLUS)) {
        advance();
        term();
        expr_prime();
    }
}

void term() {
    factor();
    term_prime();
}

void term_prime() {
    if(match(TIMES)) {
        advance();
        factor();
        term_prime();
    }
}

void factor() {
    if(match(NUM_OR_ID)) {
        advance();
    } else if(match(LP)) {
        advance();
        expression();
        if(match(RP)) {
            advance();
        } else {
            fprintf(stderr, "%d: Mismatched parenthesis\n", yylineno);
        }
    } else {
        fprintf(stderr, "%d: Number or identifier expected\n", yylineno);
    }
}