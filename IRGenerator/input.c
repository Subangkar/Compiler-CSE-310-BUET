int y;
int a[5];

int main()
{
	int x,z;
  a[2] = 9;
  a[3] = 0;
  x=1;
  a[x]=9;
  x = (a[x] || a[3]) && a[2]; // x && 
  println(x);

	return 0;
}