#include <iostream>

#include "engine.h"

using namespace std;

int main()
{

//  uint32_t size = 10;
//  char* str = (char*) malloc(4 + 10);
//  memset(str, 0, 14);


//  memcpy(str, &size, sizeof(uint32_t));

//  char* p = str + sizeof(uint32_t);

//  char data[] = "123456789";

//  memcpy(str + 4, "123456789", sizeof(data));

//  printf(":%s\n", (char*) &str);

  char* s = "hello?";

  SizedScope<uint32_t> *scp = new SizedScope<uint32_t>(s);

  char* stream = (char*) malloc(sizeof(scp));
  memcpy(stream, scp, sizeof(scp));


  printf("\n%s:%d\n", redeemVal<uint32_t, char*>(stream), sizeof (scp));





  return 0;
}
