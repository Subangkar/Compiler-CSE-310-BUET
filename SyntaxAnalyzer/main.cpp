#include <iostream>
#include "DataStructure.h"
#include "DataStructure.h"


char *indent(char *input, const char *pad)
{
	int lenpad = strlen(pad);
	int inlen = strlen(input);
	char *output = (char*)malloc(inlen+lenpad*90); //Here I'm praying +lenpad*90 is enough
	int indent = 0;
	int i = 0;
	int j = 0;
	int ndx;
	int ondx;
	char current = 'a';
	int n;

	for(ndx=ondx=0; ndx<inlen; ndx++){
		current = input[ndx];
		if(current == '{') indent++;
		if(current == '\n'){
			output[ondx++] = '\n';
//			n = ondx;
			n = ndx+1;
			while(input[n] != '\n' && input[n] != '\0'){ //Trying to check if the line to come has a curly bracket.
				if(input[n] == '}') //If it does, don't indent that line anymore.
					indent--;
				n++;
			}
			for(j=0; j<indent; j++){
				for(i=0; i<lenpad; i++){
					output[ondx++] = pad[i];
				}
			}
		}
		else{
			output[ondx++] = current;
		}
	}
//	free(input);
	output[ondx] = '\0';
	return output;
}


int main() {

	cout << indent("int main(){int x ,y ,z ; y =8 ; int d ; d =x +y ; z =(x ==y ) ; } ","    ")<<endl;

	return 0;
}