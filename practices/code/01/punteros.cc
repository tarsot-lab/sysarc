int a = 0x1234, b = 0x5678;
int *pa, *pb;

int main()
{
  int c = 0x8765, d = 0x4321;
  int *pc, *pd;
  
  pa = &a;
  pb = &b;
  
  a = 0;
  *pb = 1;
  
  pc = &c;
  pd = &d;
  
  c = 2;
  *pd = 3;

  return *pa + *pb + *pc + *pd;
}
