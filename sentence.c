#include <stdlib.h>
#include <string.h>
#include <assert.h>
typedef struct WORD Word;
struct WORD{
    Word * next;
    char * content;
};
typedef struct SENTENCE Sentence;
struct SENTENCE {
    Word * top;
    int length;
};

void validateSentence(Sentence *s)
{
	assert(s != NULL);
	assert(s->top != NULL);
	assert( s->top->content != NULL );
}
//length of 0 actually means there is 1 element in it
Sentence *create() {
    Sentence *sentence = malloc(sizeof(Sentence));
    sentence->top = NULL;
    sentence->length = -1;
    return sentence;
}
int insert(Sentence *s, char *word, int pos) {
    int output = -1;
    validateSentence(s);
    if(NULL == word){
        return -1;
    }
    if(NULL == s){
        return -1;
    }
    if(pos > s->length || pos < -s->length){
        return -1;
    }
    if(pos >= 0){
        Word *temp = s->top;
        Word *word = malloc(sizeof(word));
        word->content = word;
        word->next = NULL;
        for(int i = 0; i < pos; i ++){
            output++;
            temp = temp->next;
        }
        temp->next = word;
        s->length++;
        return output;
    }
    else{
        int toBeInserted = s->length - (-pos);
        Word *temp = s->top;
        Word *word = malloc(sizeof(word));
        word->content = word;
        word->next = NULL;
        for(int i = 0; i < toBeInserted; i ++){
            output++;
            temp = temp->next;
        }
        temp->next = word;
        s->length++;
        return output;
    }
}

int to_string(Sentence *s, char string[], int max) {
    validateSentence(s);
    if(NULL == string){
        return -1;
    }
    Word *temp = s->top;
    int output = 0;
    assert(temp != NULL);
    while(NULL != temp->next && max >= 1){
        max -= strlen(temp->content);
        strcat(string,temp->content);
        strcat(string," ");
        output += strlen(temp->content) + 1;// +1 for the space
        temp = temp->next;
    }
    strcat(string,"\0");
    return output;
}

int clength(Sentence *s) {
    validateSentence(s);
    if(NULL == s){
        return -1;
    }
    Word * temp = s->top;
    int output = 0;
    while(NULL != temp->next){
        output += strlen(temp->content);
        temp = temp -> next;
    }
    return output;
}

void destroy(Sentence *s) {
    validateSentence(s);
    Word *temp = s->top;
    while(NULL != temp->next){
        free(temp);
        temp = temp->next;
    }
    s->length = NULL;
    s->top = NULL;
    s = NULL;
    free(s);
}

int main()
{
  Sentence *test,*newTest;
  int test1;
  int test2, test3, test4, test5, test6, test7, test8, test9;
  int fail;
  char a[20], b[20];
  fail = insert(test,"hello",1); //test before initiation
  assert(-1 == fail);
  
  test = create();
  test1 = insert(test,"hello",0);
  assert(0 == test1);
  test2 = insert(test,"world",0);
  assert(0 == test2);//currently length in Sentence is 1;
  fail = insert(test,"lorem ipsum", 3);//technically test3;
  assert(-1 == fail);


  newTest = create();
  insert(newTest,"f",0);
  insert(newTest,"e",0);
  insert(newTest,"d",0);
  insert(newTest,"c",0);
  insert(newTest,"b",0);
  insert(newTest,"a",0);
  test3 = insert(newTest,"g", -4);
  assert(2 == test3);
  test4 = insert(newTest,"h", -4);
  assert(3 == test4);
  test5 = insert(newTest,"i", -1);
  assert(8 == test5);
  fail = insert(newTest, NULL, 0);
  assert(-1 == NULL);

  test6 = to_string(test,a,9);
  assert(9 == test6);//copy max -1 characters + 1 space between hello n world
  printf("%s\n", a);// hello wor
  test7 = clength(test);
  assert(10 == test7);
  test8 = clength(newTest);
  assert(8 == test8);
  test9 = to_string(newTest,a,8);
  assert(16 == test9);//8 characters + 8 spaces
  printf("%s\n", b);

  destroy(test);
  destroy(newTest);

  fail = insert(newTest,"comp genius", 0);// try inserting after freeing
  assert(-1 == fail);
}
