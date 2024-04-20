#include<stdio.h>
#include<string.h>

#include"FieldNameList.h"

// structure to store FIX instruction encoding 
typedef struct FIXComponent{
	int tag[64];
	char value[64][128];
	int numTags;
} FIXComponent;

//return a LowerCase ascii if its uppercase, if not the original
char ToLowerCase(char n) {
	if(n <= 90) {
		return (n + 32);
	}else {
		return n;
	}
}

//compare field name to get key value for message
int KeyFromName(char* fieldNameUnknown) {
	for (int i = 0; i < 6051; i++) {
		if (strcmp(FieldNameList[i].fieldName, fieldNameUnknown) == 0)
			return FieldNameList[i].tag;
	}
	printf("error: field name unmached: %s\n", fieldNameUnknown);
	return 0;
}

//print out the FIX message pair by pair
void ReadFIXComponent(FIXComponent message) {
	for (int i = 0; i < message.numTags; i++) {
		printf(" %i = %s \n", message.tag[i], message.value[i]);
	}
	return;
}

//convert string into Fix message components
void InputComponents(FIXComponent* message, char* input, int inputFlag) {
	if(inputFlag == 0) {
		return; // empty message, nothing to add
	}
		
	//add message content
	//sycle through input intil '=' or ','
	int TypeFlag = 0; // 0 for tag, 1 for value
	char inputTmp[32] = {};
	int indexTmp = 0;
	for(int i = inputFlag + 1; input[i - 1] != ')'; i++ ) {
		if (input[i] == '=') { //load chunk as tag
			message->tag[message->numTags] = KeyFromName(inputTmp);

			memset(inputTmp, 0, indexTmp);
			indexTmp = 0;
			TypeFlag = 1;
			continue;
		}
		if (input[i] == ',' || input[i] == ')') { //load chunk as value
			strcpy(message->value[message->numTags], inputTmp);
			message->numTags ++;

			memset(inputTmp, 0, indexTmp);
			indexTmp = 0;
			TypeFlag = 0;
			continue;
		}
		if(input[i] == ' ') 
			continue;

		//add a charecter to chunk
		inputTmp[indexTmp] = input[i];			
		indexTmp ++;
	}
}

void main(int argc, char** argv){

	//Read data from Input.txt into input
	char filename[] = "input.txt";
	FILE *inputFile;
	char input[360];

	inputFile = fopen("input.txt", "r");
	fgets(input, 360, inputFile);
	printf("Read input: %s \n", input);
	fclose(inputFile);

	//distinguish charecter from key-word
	//Logon(34=56) -> Logon [key] 34=56 [value]
	//Part2 -> assuming one input ignore it, print defaults for message !!!restriction
	
	//find the sum of letters in assci of mesage
	//find start and end of content in brackets 
	int messageType = 0;
	int inputFlag = 0; // 0 for no input, # for '('
	for(int i = 0; i < 64; i ++) {
		if (input[i] == '('){ //start of function input value
			inputFlag = i;
			break;
		}

		messageType += ToLowerCase(input[i]); //adding ascii
	}

	FIXComponent message;
	message.tag[0] = 8;
	strcpy(message.value[0], "FIX.4.2");
	message.numTags = 3;

	//split to indevidual message types
	switch(messageType)
	{
		case 944: // HeartBeat 
			printf("heartbeat\n");
			message.tag[2] = 35;
			strcpy(message.value[2], "0");

			InputComponents(&message, input, inputFlag);

			break;
		case 1225:
			printf("testrequest\n");
			message.tag[2] = 35;
			strcpy(message.value[2], "1");

			InputComponents(&message, input, inputFlag);

			break;
		case 1418:
			printf("resendrequest\n");
			message.tag[2] = 35;
			strcpy(message.value[2], "2");

			InputComponents(&message, input, inputFlag);

			break;
		case 637:
			printf("reject\n");
			message.tag[2] = 35;
			strcpy(message.value[2], "3");

			InputComponents(&message, input, inputFlag);

			break;
		case 1404:
			printf("sequencereset\n");
			message.tag[2] = 35;
			strcpy(message.value[2], "4");

			InputComponents(&message, input, inputFlag);

			break;
		case 666:
			printf("logout\n");
			message.tag[2] = 35;
			strcpy(message.value[2], "5");

			InputComponents(&message, input, inputFlag);

			break;
		case 321:
			printf("ioi\n");
			message.tag[2] = 35;
			strcpy(message.value[2], "6");

			InputComponents(&message, input, inputFlag);

			break;
		case 1403:
			printf("advertisement\n");
			message.tag[2] = 35;
			strcpy(message.value[2], "7");

			InputComponents(&message, input, inputFlag);

			break;
		case 1648:
			printf("executionreport\n");
			message.tag[2] = 35;
			strcpy(message.value[2], "8");

			InputComponents(&message, input, inputFlag);
			
			break;
		case 1791:
			printf("ordercancelreject\n");
			message.tag[2] = 35;
			strcpy(message.value[2], "9");

			InputComponents(&message, input, inputFlag);

			break;
		case 543:
			printf("logon\n"); // add logon value pair to message
			message.tag[2] = 35;
			strcpy(message.value[2], "A");

			InputComponents(&message, input, inputFlag);

			break;
		default:
			printf("ERORR: messaage bad :(");
			break;
	}

	//adding Checksum component
	message.tag[message.numTags] = 10;
	strcpy(message.value[message.numTags] , "Checksum");
	message.numTags ++;
	//adding BodyLength component 
	message.tag[1] = 9;
	strcpy(message.value[1] , "BodyLength");
	ReadFIXComponent(message);
	return;
}
