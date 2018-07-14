// int y;
// int a[5];
int a,b;
int l;
int sum(int a)
{   
    if(!a) return 0;
    return (a)+sum(a-1);
}


int main(){
    int a,b,i;
    // l=10;
    // a=15;
    // b=21;
    int x;
    x=sum(5);
    println(x);
    return 0;
}




// int main()
// {
// 	int x,z;
//   x=0;
//   // a[x]=9;
//   // int i;
//   // i = 0;
//   y=3;
//   while( y--)
//   {
//     // a[x]++;
//     // i++;
//     x++;
//   }
   
//   // x = a[x];
//   println(x);
//   println(x);

// 	return 0;
// }