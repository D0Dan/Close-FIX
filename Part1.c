#include<stdio.h>

//TODO ->
//error handeling
//migrate to Patr1.h?


// structure to store FIX instruction encoding 
//struct pair {
//	int key;
//	char value[64];
//};

void main(int argc, char** argv){

	//Read data from Input.txt into input
	char filename[] = "input.txt";
	FILE *inputFile;
	char input[128];

	inputFile = fopen("input.txt", "r");
	fscanf(inputFile, "%s", &input);
	printf("Read input: %s \n", input);
	fclose(inputFile);

	//distinguish charecter from key-word
	//Logon(34=56) -> Logon [key] 34=56 [value]
	//Part1 -> assuming empty values !!!restriction!!!
	
	//find the sum of letters in assci of mesag
	//find start and end of content in brackets 
	int messageType = 0;
	int inputFlag = 0;
	int inputFlagClose = 0;
	for(int i = 0; i < 64; i ++) {
		if (input[i] == '('){
			inputFlag = i;
			for (int j = i; j < 64; j ++) {
				if (input[j] == ')')
					inputFlagClose = j + 1;
			}
			break;
		}
		messageType += input[i];
	}

	//split to indevidual message types
	switch(messageType)
	{
		case 944:
			printf("heartbeat");
			break;
		case 1225:
			printf("testrequest");
			break;
		case 1418:
			printf("resendrequest");
			break;
		case 637:
			printf("reject");
			break;
		case 1404:
			printf("sequencereset");
			break;
		case 666:
			printf("logout");
			break;
		case 321:
			printf("ioi");
			break;
		case 1403:
			printf("advertisement");
			break;
		case 1648:
			printf("executionreport");
			break;
		case 1791:
			printf("ordercancelreject");
			break;
		case 511:
			printf("logon");
			if (inputFlag == 0 || inputFlagClose == 0) {
				printf("no message content");
			}
			else {
				//TODO print in range native in C , input[12;32]?
				for (int i = inputFlag; i < inputFlagClose; i++)
					printf("%c", input[i]);
			}
			break;
		default:
			printf("ERORR: messaage bad :(");
			break;
	}
	
	printf("\n");
	return;
}
