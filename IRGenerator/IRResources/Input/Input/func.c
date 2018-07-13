int f(int a){
    return 2*a;
    a=9;
}

int g(int a, int b){
    int x;
    x=f(a)+a+b;
    return x;
}

int main(){
    int a,b;
    a=1;
    b=2;
    a=f(a);
    println(a);
    b=g(1,2);
    println(b);
    return 0;
}
