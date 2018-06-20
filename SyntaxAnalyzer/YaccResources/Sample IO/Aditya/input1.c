int x,y,z; float a;

void foo();

int var(int a,int b){
	return a + b;
}

void foo(){
	x=2;
	y=4;
}

int main(){

	int a[2],b,c; float d;
	foo();
	a[var(2,3)]=4;
	a[1]=2;
	a[2]=3;
	a[4+5]=6;
	a[7<8]=9;
	a[b%c]=x;
	b=(2%3 * 4 +5);
	d=2.3*4;
	b=var(2,3);
	return 0;

}