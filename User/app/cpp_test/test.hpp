#ifndef __TEST_HPP__
#define __TEST_HPP__

class testcls
{
private:
  int v;
public:
  static testcls& Inst(void);//µ¥ÀýÄ£Ê½
  
  void show(void);
};


#endif