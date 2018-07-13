// int y;
// int a[5];
int a,b;
int add1()
{
    return a+b;
}
int sub1()
{
    return a-b;
}

int val(){
    int x;
    x=1;
    a=10;
    b=5;
    if(x==3) return add1();
    else return sub1();
}

int main(){
    int a,b,i;
    b=0;
    for(i=0;i<4;i++){
        a=3;
        while(a--){
            b++;
        }
    }
    int x;
    x=val();
    println(a);
    println(b);
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