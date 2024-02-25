#include<stdio.h>

// structure to store FIX instruction encoding 
typedef union Touple {
	int key;
	char* value;
}touple;

touple Add(int key, char value[]){
	touple New;
	New.key = key;
	New.value = value;
	return New;
}

void main(int argc, char** argv){
	//Read data from Input.txt into input [max 20 charecters]
	char filename[] = "input.txt";
	FILE *inputFile;
	char input[20];

	inputFile = fopen("input.txt", "r");
	fscanf(inputFile, "%s", &input);
	printf("Read input: %s", input);

	//distinguish charecter from key-word
	//Logon(34=56) -> Logon [key] 34=56 [value]
	//Part1 -> assuming empty values !!!restriction!!!
	
	touple keyValue[5];
	char BeginString[] = "FIX.4.2"; 
	keyValue[1] = Add(8, BeginString);

	printf("|%d=%s|", keyValue[1].key, keyValue[1].value);
	return;
}
