int fib(int i)
{
	if (i < 2)
		return i;
	else
		return fib(i - 1) + fib(i - 2);
}

int main()
{
	return fib(5);
}
