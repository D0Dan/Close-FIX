#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/stat.h>

#include"field_name_list.h"

// struct to store FIX tag - value pairs
typedef struct {
	int tag;
	char value[64];
} field_t;

// structure to store FIX instruction encoding 
typedef struct {
	field_t fields[64];
	int num_fields;
} message_t;

int read_file_to_string(const char* file_name, char *buffer, int buffer_size) {

	FILE input_file = fopen(filed_name, "r"); 

	//getting file information
	struct stat file_stats;
	
	if (stat(input_file, &file_stats) == -1) {
		perror("Error reading file");
		return 1;
	}
	
	//check if buffer allocated is big enough
	if (file_stats.st_size > buffer_size) {
		perror("File too bit for buffer space allocated");
		return 1;
	}
	
	//inputing contents to buffer
	int bytes_read  = fread(buffer, file_stats.st_size, 1 ,input_file);

	fclose(input_file);
	return bytes_read;
}


//compare message types to get message key
char message_type_from_name(char* message_name_unknown) {
	for (int i = 0; message_name_list[i].tag != 0; i++) {
		if (strcmp(message_name_list[i].name, message_name_unknown) == 0)
			return message_name_list[i].tag;
	}
	printf("error: message name unmached: %s\n", message_name_unknown);
	return '0';
}
//compare field name to get key value for message
int key_from_name(char* field_name_unknown) {
	for (int i = 0; field_name_list[i].name != 0; i++) {
		if (strcmp(field_name_list[i].name, field_name_unknown) == 0)
			return field_name_list[i].tag;
	}
	printf("error: field name unmached: %s\n", field_name_unknown);
	return 0;
}

//print out the FIX message pair by pair
void read_FIX_component(message_t message) {
	for (int i = 0; i < message.num_fields; i++) {
		printf(" %i = %s \n", message.fields[i].tag, message.fields[i].value);
	}
	return;
}

//convert string into Fix message components
void input_components(message_t* message, char* input) {
	
		//find message type
	//locator pointer
	char *message_p = input;
	
	//cycle until end of message declaration
	while (*message_p != '\0' && *message_p != '(')
		message_p++;
	//if p != ( error TODO
	
	char message_type[32];
	strncpy(message_type, input, (message_p - input));

	//input message type field
	message->fields[2].tag = 8;
	message->fields[2].value[0] = message_type_from_name(message_type);

		//find message contents
	//sycle through input until '=' or ','
	//int type_flag = 0; // 0 for tag, 1 for value // for error handeling TODO
	char tmp[32];
	while (*message_p != '\0' && *message_p != ')') {
		if (*message_p == '=') { //load chunk as tag
			message->fields[message->num_fields].tag = key_from_name(tmp);

			memset(tmp, 0, strlen(tmp));
			message_p++;
			continue;
		}
		if (*message_p == ',') { //load chunk as value
			strcpy(message->fields[message->num_fields].value, tmp);
			message->num_fields ++;

			memset(tmp, 0, strlen(tmp));
			message_p++;
			continue;
		}
		if(*message_p == ' ') 
			message_p++;
			continue;

		//advence pointer
		strncat(tmp, message_p, 1);
		message_p ++;
	}
}

void main(int argc, char** argv){

	//Read data from Input.txt into input
	char filename[] = "input.txt";
	int input_size = 360;
	char input[360];

	// read file contents into input, return lenght of contents
	int message_length = read_file_to_string(filename, input, input_size);

	message_t message;
	input_components(&message, input);

	//split to indevidual message types //TODO
	switch(message.fields[2].value[0]) // [2] is message type
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

	return;
}
