#include <stdio.h>
#include "lex.h"

int main() {
   int i;
   for(i = 0; i < 5; i++) {
      printf("Token %d %d\n", i, lex());
   }
   return 0;
}
