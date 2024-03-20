#include<stdio.h>

//TODO ->
//error handeling
//migrate to Patr1.h?


// structure to store FIX instruction encoding 
//struct pair {
//	int key;
//	char value[64];
//};

//temp function for debug
void printInRange(char* input, int startFlag, int endFlag){
	for (int i = startFlag; i >= endFlag; i++)
		printf("%c", input[i]);
	return;
}


void main(int argc, char** argv){

	//Read data from Input.txt into input
	char filename[] = "input.txt";
	FILE *inputFile;
	char input[128];

	inputFile = fopen("input.txt", "r");
	fscanf(inputFile, "%s", &input);
	printf("Read input: %s", input);
	fclose(inputFile);

	//distinguish charecter from key-word
	//Logon(34=56) -> Logon [key] 34=56 [value]
	//Part1 -> assuming empty values !!!restriction!!!
	
	//find the sum of letters in assci of mesag
	//find start and end of content in brackets 
	int messageType = 0;
	int inputFlag = 0;
	int inputFlagClose = 0;
	for(int i = 0; input[i] =! '\0'; i ++) {
		if (input[i] == '('){
			inputFlag = i;
			messageType += input[i];
			for (int j = i; input[j] =! '\0'; j ++)
				if (input[j] == ')')
					inputFlagClose = j;
			break;
		}
		messageType += input[i];
	}

	//split to indevidual message types
	switch(messageType)
	{
		case 944:
			printf("heartbeat");
		case 1225:
			printf("testrequest");
		case 1418:
			printf("resendrequest");
		case 637:
			printf("reject");
		case 1404:
			printf("sequencereset");
		case 666:
			printf("logout");
		case 321:
			printf("ioi");
		case 1403:
			printf("advertisement");
		case 1648:
			printf("executionreport");
		case 1791:
			printf("ordercancelreject");
		case 543:
			printf("logon");
			if (inputFlag == 0 || inputFlagClose == 0)
				printf("no message content");
			else
				//TODO print in range native in C , input[12;32]?
				printInRange(input, inputFlag, inputFlagClose);
		default:
			printf("ERORR: messaage bad :(");
	}
	
	return;
}
