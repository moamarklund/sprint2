#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h> //toupper
#include <string.h>


bool only_alpha(char str[]){
  int i = 0;
  while(str[i] != '\0'){
    if(isalpha(str[i]) == 0){
      const char exceptions[] = {' ',',','.'};
	//{'å', 'ä', 'ö', 'Å', 'Ä', 'Ö', ' ', ',', '.'};
      bool is_exception = false;
      for(int n=0; n<sizeof(exceptions); n++){
	if(str[i] == exceptions[n])
	  is_exception = true;
      }
      if(!is_exception)
	return false;
    }
    i++;
  }
  return true;
}

char ask_question_char(char message[]){
  bool have_answer = false;
  char input;
  while(!have_answer){
    printf("%s", message);
    input = toupper(getchar());
    while(getchar() != '\n');
    if(isalpha(input) != 0)
      have_answer = true;
    else
      printf("felaktigt svar, ange ett tecken.\n");
  }
  return input;
}

int ask_question_int(char message[]){
  bool have_answer = false;
  char buffer[sizeof(int)*8+1];
  while(!have_answer){
    printf("%s", message);
    scanf("%s", buffer);
    if(atoi(buffer) != 0)
      have_answer = true;
    else
      printf("felaktigt svar, ange en siffra.\n");
  }
  while(getchar() != '\n');
  return atoi(buffer);
}

void ask_question_string(char message[], char return_message[]){
  bool have_answer = false;
  while(!have_answer){
    printf("%s", message);
    scanf("%s", return_message);
    if(only_alpha(return_message)){
      have_answer = true;
    } else {
      printf("felaktigt svar, tillåtna tecken: a...ö , .\n");
    }
    while(getchar() != '\n');
  }
}
