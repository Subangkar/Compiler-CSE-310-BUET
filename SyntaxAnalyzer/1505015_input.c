int func(int a){
	return a;
}

float f(){
	return 1.0;
}

void g(){
	return 0;
}

int h(int a);

int main(){
	int a[2],c,i,j ; 
	float c;  // multiple declaration
	i=2.3;    //type mismatch, error or warning,
	a[2.5]=1; //array index not integer	
	j=2%3.7;  //not integer on modulus
	j=2%f();  //not integer on modulus
	func(a);	//type mismatch
	j=g();
	j=2%(3.5<4); // type conversion of RELOP
	b=8;		//undeclared variable
	a=4;		//no index on array
	i[3]=1;		//index on not array
	func();		//function call with inappropraite number of params
	i();		//function call on non-function
	return 0;	
}

int h(float a){
	return 0;
}