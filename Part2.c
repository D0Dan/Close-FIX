#include<stdio.h>
#include<string.h>

#include"FieldNameList.h"

// structure to store FIX instruction encoding 
typedef struct FIX_component{
	int tag[64];
	char value[64][128];
	int num_tags;
} FIX_component;

//structure to store Field names, Messagenames
//typedef struct {
//    int tag;
//    char name[64];
//} field_name_t;

extern field_name_t field_name_list[];

extern field_name_t message_name_list[];

//return a LowerCase ascii if its uppercase, if not the original
//char to_lower_case(char n) {
//	if(n <= 90 && n >= 65) {
//		return (n + 32);
//	}else {
//		return n;
//	}
//}

//compare field name to get key value for message
int key_from_name(char* field_name_unknown, char type) {
	if (type == 't') {
		for (int i = 0; field_name_list[i].name != '0' || field_name_list[i].tag != 0; i++) {
			if (strcmp(field_name_list[i].name, field_name_unknown) == 0)
				return field_name_list[i].tag;
		}
		printf("error: field name unmached: %s\n", field_name_unknown);
	} else if (type == 'c') {
		for (int i = 0; message_name_list[i].name != 0 || message_name_list[i].tag != 0; i++) {
			if (strcmp(message_name_list[i].name, field_name_unknown) == 0)
				return message_name_list[i].tag;
		}
		printf("error: message name unmached: %s\n", field_name_unknown);
	}
	printf("error: unreachebale in key_from_name!: invalid type");
	return 0;
}

//print out the FIX message pair by pair
void read_FIX_component(FIX_component message) {
	for (int i = 0; i < message.num_tags; i++) {
		printf(" %i = %s \n", message.tag[i], message.value[i]);
	}
	return;
}

//convert string into Fix message components
void input_components(FIX_component* message, char* input, int input_flag) {

	//add version decalration
	message->tag[0] = 8;
	strcpy(message->value[0], "FIX.4.2");	

	//add message type
	char message_tmp[32];
	memcpy(message_tmp, input, input_flag);
	printf("message: %s \n", message_tmp);
	message->tag[2] = 35;
	strcpy(message->value[2], key_from_name(message_tmp, 'm'));

	
	if(input_flag == 0) {
		return; // empty message, nothing to add
	}
		
	//add message content
	//sycle through input intil '=' or ','
	int type_flag = 0; // 0 for tag, 1 for value
	char input_tmp[32] = {};
	int index_tmp = 0;
	for(int i = input_flag + 1; input[i - 1] != ')'; i++ ) {
		if (input[i] == '=') { //load chunk as tag
			message->tag[message->num_tags] = key_from_name(input_tmp, 't');

			memset(input_tmp, 0, index_tmp);
			index_tmp = 0;
			type_flag = 1;
			continue;
		}
		if (input[i] == ',' || input[i] == ')') { //load chunk as value
			strcpy(message->value[message->num_tags], input_tmp);
			message->num_tags ++;

			memset(input_tmp, 0, index_tmp);
			index_tmp = 0;
			type_flag = 0;
			continue;
		}
		if(input[i] == ' ') 
			continue;

		//add a charecter to chunk
		input_tmp[index_tmp] = input[i];			
		index_tmp ++;
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
	
	//find start and end of content in brackets 
	int input_flag = 0; // 0 for no input, # for '('
	for(int i = 0; i < 64; i ++) {
		if (input[i] == '('){ //start of function input value
			input_flag = i;
			break;
		}
	}

	FIX_component message;
	message.num_tags = 3;
	input_components(&message, input, input_flag);


	//split to indevidual message types
	switch(message.value[2] - '0') // [2] is message type
	{
		case '0':
			printf("heartbeat\n");

			break;
		case '1':
			printf("testrequest\n");

			break;
		case '2':
			printf("resendrequest\n");

			break;
		case '3':
			printf("reject\n");

			break;
		case '4':
			printf("sequencereset\n");

			break;
		case '5':
			printf("logout\n");

			break;
		case '6':
			printf("ioi\n");

			break;
		case '7':
			printf("advertisement\n");

			break;
		case '8':
			printf("executionreport\n");
			
			break;
		case '9':
			printf("ordercancelreject\n");

			break;
		case 'A':
			printf("logon\n");

			break;
		default:
			printf("ERORR: messaage bad :(");
			break;
	}

	//adding Checksum component
	message.tag[message.num_tags] = 10;
	strcpy(message.value[message.num_tags] , "Checksum");
	message.num_tags ++;
	//adding BodyLength component 
	message.tag[1] = 9;
	strcpy(message.value[1] , "BodyLength");
	read_FIX_component(message);
	return;
}
