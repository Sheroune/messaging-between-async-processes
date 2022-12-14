#include <stdio.h>
#include <sys/msg.h>

#define MAX_SIZE 16384

const int PROJECT_ID = 'E';

int main() {
	char buf[MAX_SIZE];

	// get ipc key
	key_t ipckey = ftok("/tmp/lab5", PROJECT_ID);
	if(ipckey == -1) {
		printf("Error: no messages\n");
		return -1;
	}

	// get message query id
	int mq_id = msgget(ipckey, 0);
	if(mq_id == -1) {
		printf("Error: cannot open message query\n");
		exit(1);
   }

	struct {
        long type;
        char text[MAX_SIZE];
    } message;

	printf("[INFO] Reading message...\n\n");
	msgrcv(mq_id, &message, sizeof(message.text), 2, 0);

	printf("Message type: %ld\n", message.type);
	printf("Message content: %s", &message.text);

	// commands which can be used to find owners
	// "ls */ -lR | grep "^-" | tr -s " " "\t" | cut -f 3,9"
	// with directories
	// "ls */ -lR | grep "^-\|:$" | tr -s " " "\t""
	// find */ -type f -printf \"%u\t%p\n\"

	FILE *owners = popen("tree -uafi | egrep '/[A-Za-z0-9.]*/'", "r");
	fread(buf, 1, sizeof(buf), owners);
	pclose(owners);

	printf("\n[INFO] Printing names of the owners of the received files\n");
	printf("\nOwner:\t\tFile:\n");
	printf("%s", &buf);

	return 0;
}
