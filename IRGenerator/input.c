// int y;
// int a[5];
int a,b;
int l;
int add1(int l)
{   
    int i,v;
    v=0;
    
    for(i = 0; l; i++)
    {
       v++;
       l = l/2;
    }
    
    return v;
}


int main(){
    int a,b,i;
    l=10;
    a=15;
    b=21;
    int x;
    x=add1(a);
    println(x);
    x=add1(b);
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