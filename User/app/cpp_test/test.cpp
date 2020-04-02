#include "test.h"
#include "test.hpp"
#include "testadd.h"
#include "stdio.h"

extern int add(int a, int b);

void testcls::show(void)
{
  int v;
  v = add(this->v,this->v);;
  printf("hello cpp,%d\r\n",v);
}

testcls& testcls::Inst(void)
{
  static testcls a;
  
  a.v++;
  return a;
}

void testcls_show(void)//����c����
{
  testcls::Inst().show();
}