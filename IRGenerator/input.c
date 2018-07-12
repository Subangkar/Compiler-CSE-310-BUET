int y;
int a[5];

int main()
{
	int x,z;
  a[2] = 0;
  a[3] = 1;
  x=0+1;
  a[x]=4+(17%12);
  a[x] = (a[x] >= a[3]) && (a[2]<a[x]); // x && 
  x = a[x];
  println(x);

	return 0;
}