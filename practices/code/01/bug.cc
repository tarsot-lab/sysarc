int main()
{
	const int N = 1000;

	int a = 0, b = 1, c = 2, d = 3, e = 4, f = 5, g = 6;

	for (int i = N; i > 0; --i)
	{
		a = b + c - i;
		b = c - d * i;
		c = d * e / i;
		d = e / (f % i);
		e = f % (g + i);
	}

	return a + b + c + d + e + f + g;
}
