int x,y,z;
float a;

void hello(){
	int b;
	return 3; 			//Error at line : 7. Return type doesn't match
}

void hello(); 			//Error at line : 9. hello Declared multiple times

int foo1(int a);

float foo1(int a){
	y=b; 				// Error at line: 14. Undeclared variable b ** Error at line: 14. Type mismatched
	return 3.4; 		//Error at line : 16. Return type doesn't match
}

float foo2(int a,int b){
	return 0; 			// Error at line : 20. Return type doesn't match
}

int nondec(int a);

int main(){
	int x;
	int a[2],b,c; float d;
	a=5; 				//Error at line: 27. Trying to access array like normal variable!
	b[2]=3; 			//Error at line: 28. b not an Array
	c=sort(2,3); 		//Error at line: 29. Undeclared function sort Error at line: 29. Type mismatched
	d=a[2]; 			//Error at line: 30. Type mismatched
	a[2.5]=9; 			//Error at line: 31. Not-Integer Array Index used
	b=4.5%3; 			//Error at ine: 32. Both operands on modulus should be Integer ** Error at line: 32. Type mismatched
	b=2%3.5; 			//Error at ine: 33. Both operands on modulus should be Integer
	
	if(2<3){
		d=x + y%4; 		//Error at line: 36. Type mismatched
	}

	a[hello()]=4; 		//Error at line: 39. Can't call Void function for assignment or expression
	b=hello(); 			// Error at line: 40. Can't call Void function for assignment or expression
	b=foo2(2,3,4); 		//Error at line 41, Parameter number doesn't match! ** Error at line: 41. Type mismatched
	b=foo2(2,3.5); 		//Error at line 42. Parameter type doesn't match ** Error at line: 42. Type mismatched
	b=nondec(1); 		//Error at line :43. Function nondec not defined
	b=x;
	return 0;
}