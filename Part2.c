#include<stdio.h>
#include<string.h>

//FIX tag defaults
#define MAX_FIX_TAGS 64
#define MAX_VALUE_LENGTH 128
#define MAX_MESSAGE_LENGTH 16

//TODO ->
//migrate to Patr1.h?

// structure to store FIX instruction encoding 
struct FIXComponent {
	int tag[MAX_FIX_TAGS];
	char value[MAX_FIX_TAGS][MAX_VALUE_LENGTH];
	int numTags = 0;
};

//return a LowerCase ascii if its uppercase, if not the original
char ToLowerCase(char n) {
	if(n <= 90) {
		return (n + 32);
	}else {
		return n;
	}
}

//compare field name to get key value for message
//5000 unique message types,  have to do someting unique 
int KeyFromName(char* fieldName) {
	return 1; //TODO
}

//print out the FIX message pair by pair
void ReadFixComponent(FIXComponent message) {
	for (int i = 0, i < numTags, i++) {
		printf(" %i = %c \n", message.tag[i], message.value[i]);
	}
	return;
}

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
	//Part2 -> assuming one input ignore it, print defaults for message !!!restriction
	
	//find the sum of letters in assci of mesage
	//find start and end of content in brackets 
	int messageType = 0;
	int inputFlag = 0; // 0 for no input, # for '('
	int inputFlagClose = 0; // 0 of rno input, # for ')'
	for(int i = 0; i < 64; i ++) {

		if (input[i] == '('){ //start of function input value
			inputFlag = i;
			for (int j = i; j < 64; j ++) {
				if (input[j] == ')')
					inputFlagClose = j + 1;
			}
			break;
		}

		messageType += ToLowerCase(input[i]); //adding ascii
	}

	FIXComponent message;


	//split to indevidual message types
	switch(messageType)
	{
		case 944: // HeartBeat 
			printf("heartbeat\n");
			message.tag[message.numTags] = 8;
			strcpy(message.value[message.numTags], '0');
			message.numTags += 1;
			break;
		case 1225:
			printf("testrequest\n");
			message.tag[message.numTags] = 8;
			strcpy(message.value[message.numTags], '1');
			message.numTags += 1;
			break;
		case 1418:
			printf("resendrequest\n");
			message.tag[message.numTags] = 8;
			strcpy(message.value[message.numTags], '2');
			message.numTags += 1;
			break;
		case 637:
			printf("reject\n");
			message.tag[message.numTags] = 8;
			strcpy(message.value[message.numTags], '3');
			message.numTags += 1;
			break;
		case 1404:
			printf("sequencereset\n");
			message.tag[message.numTags] = 8;
			strcpy(message.value[message.numTags], '4');
			message.numTags += 1;
			break;
		case 666:
			printf("logout\n");
			message.tag[message.numTags] = 8;
			strcpy(message.value[message.numTags], '5');
			message.numTags += 1;
			break;
		case 321:
			printf("ioi\n");
			message.tag[message.numTags] = 8;
			strcpy(message.value[message.numTags], '6');
			message.numTags += 1;
			break;
		case 1403:
			printf("advertisement\n");
			message.tag[message.numTags] = 8;
			strcpy(message.value[message.numTags], '7');
			message.numTags += 1;
			break;
		case 1648:
			printf("executionreport\n");
			message.tag[message.numTags] = 8;
			strcpy(message.value[message.numTags], '8');
			message.numTags += 1;
			break;
		case 1791:
			printf("ordercancelreject\n");
			message.tag[message.numTags] = 8;
			strcpy(message.value[message.numTags], '9');
			message.numTags += 1;
			break;
		case 543:
			printf("logon\n"); // add logon value pair to message
			message.tag[message.numTags] = 8;
			strcpy(message.value[message.numTags], 'A');
			message.numTags += 1;

			if (inputFlag == 0 || inputFlagClose == 0) {
				break; // empty message, nothing to add
			}

			//add message content
			//sycle through input intil '=' or ','
			int TypeFlag = 0; // 0 for tag, 1 for value
			int inputFlagTmp = inputFlag;
			while(inputFlag < inputFlagClose) {
				if (input[inputFlagTmp] == '=')
					TypeFlag = 1;
				if (input[inputFlagTmp] == ',')
					TypeFlag = 0;

				inputFlagTmp += 1;
			}

			break;
		default:
			printf("ERORR: messaage bad :(");
			break;
	}
	
	ReadFIXMessage(message);
	return;
}
