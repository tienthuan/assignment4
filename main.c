#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "regions.h"

// this code should run to completion with the output shown
// you must think of additional cases of correct use and misuse for your testing
int main()
{
  Boolean test;
  long *test1;
  char *test2, *test3, *test4, *test5, *test6;
  char *fail;

  fail = ralloc(69420); //test before initiation
  assert(NULL == fail);
  
  test = rinit("hi", 999);//1000
  assert(test);
  test = rinit("there", 69); // 72
  assert(test);

  printf("Chosen: %s\n", rchosen()); // there
  
  test = rchoose("hi");
  assert(test);
  test1 = ralloc(sizeof(long) * 32);
  printf("Size: %d\n", rsize(test1)); // 128
  test2 = ralloc(256);
  assert(NULL != test2);
  fail = ralloc(768); //fail
  assert(NULL == fail);
  test3 = ralloc(616); // just enough
  assert(NULL != test3);
  test = rfree(test1);
  assert(test);
  test = rfree(test3);
  assert(test);
  fail = ralloc(616 + 128); //not anough contiguous
  assert(NULL == fail);
  test = rfree(test2);
  assert(test);
  test5 = ralloc(616 + 128); // fail again
  assert (NULL != test4);
  
  
  test = rchoose("there");
  assert(test);
  test6 = ralloc(999);
  assert(NULL != test5);
  printf("Size: %d\n", rsize(test5)); // 1000
  
  rdump();// hi there
  
  test = rfree(test5 + 69);
  assert(!test);
  test = rfree(test5);
  assert(test);
  
  rdestroy("hi");

  test = rchoose("hi");//test after deletion
  assert(test);
  fail = ralloc(1);
  assert(NULL == fail);
  rsize(fail);
  assert(fail);

  rdestroy("there");

  test = rchoose("hi");//test after deletion again
  assert(test);
  fail = ralloc(1);
  assert(NULL == fail);
  rsize(fail);
  assert(fail);

  rdump(); // nothing

  test = rinit("new",800);//test new insertion after deleting everything
  assert(test);
  rdump();

  fprintf(stderr,"\nEnd of processing.\n");

  return EXIT_SUCCESS;
}