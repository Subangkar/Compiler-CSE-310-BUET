int y;
int a[5];

int main()
{
	int x,z;
  x=1;
  a[x]=9;
  int i;
  i = 0;
  while( i < 6)
  {
    a[x]++;
    i++;
  }
   
  x = a[x];
  println(x);
  println(x);

	return 0;
}