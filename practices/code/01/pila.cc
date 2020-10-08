int fa(int a)
{
  return a + 1;
}

int fb(int b)
{
  return fa(b) * 2;
}

int fc(int c)
{
  return fb(c + 1) / fa(c - 1);
}

int main()
{
  return fc(3);
}
