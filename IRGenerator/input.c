// int y;
// int a[5];

// #define println(x) cout <<x<<endl
// #include <bits/stdc++.h>
// using namespace std;

int a,b;
int l;
int sumf(int a,int b)
{   
    int t;
    if(!a) return 0;
    return (a+b)+sumf(a-1,b-1);
}

int fact(int x)
{
    if(!x) return 1;
    return x*fact(x-1);
}

int subf(int a,int b)
{   
    ;
    return (a)+(-b);
}

int main(){
    int a,b,i;
    int y[3];
    l=10;
    a=15;
    b=21;
    y[2]=4;
    int x;
    // a=y[2];
    x=a;
    // 7&&8;
    x=a;
    a=x;
    x=a;
    x=y[2];
    x=fact(7);
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