#include <stdio.h>
#include <sys/msg.h>

#define PROJECT_ID 8841

int main() {

	key_t ipckey;

	while(1) {
		ipckey = ftok("/tmp/lab5", PROJECT_ID);

		if(ipckey == -1) {
			printf("Error: no messages\n");
			sleep(1);
		}
		else {
			break;
		}
	}

	//get message query id
	int mq_id = msgget(ipckey, 0);
	if(mq_id == -1) {
		printf("Error: cannot open message query\n");
		exit(1);
   }

	struct {
        long type;
        char text[8192];
    } message;

	printf("[INFO] Reading message...\n\n");
	int st = msgrcv(mq_id, &message, sizeof(message.text), 2, 0);

	printf("Message type: %ld\n", message.type);
	printf("Message content: %s", &message.text);

	char *command;
	int s = asprintf(&command, "ls */ -lR | grep \"^-\" | tr -s \" \" \"\t\" | cut -f 3,9");
   if(s < 0) {
		printf("Error: cannot print\n");
	}

	printf("\n[INFO] Printing names of the owners of the received files\n");
	printf("\nOwner:\tFile:\n");

	int code;
	if(fork() == 0) {
   	execl("/bin/sh", "sh", "-c", command, (char *) 0);
   }
	else {
   	// wait until child process die
		wait(&code);
   }


	// "ls */ -lR | grep "^-" | tr -s " " "\t" | cut -f 3,9"

	// with directories
	// "ls */ -lR | grep "^-\|:$" | tr -s " " "\t""

	return 0;
}
