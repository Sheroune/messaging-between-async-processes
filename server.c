#include <stdio.h>
#include <sys/msg.h>

#define MAX_SIZE 16384

const int PROJECT_ID = 'E';

int main() {
	char buf[MAX_SIZE];

	system("touch /tmp/lab5");
	key_t ipckey = ftok("/tmp/lab5", PROJECT_ID);

	printf("[INFO] Creating message query\n");
	int mq_id = msgget(ipckey, IPC_CREAT | 0600);
	if (mq_id == -1) {
		printf("Error: cannot open message query");
		return -1;
   }

	struct {
		long type;
		char text[MAX_SIZE];
	} message;


	// 1st message
	printf("[INFO] Sending 1st message\n");
	FILE *dirs = popen("tree -da | head --lines=-2 | sed '1d'", "r");
   fread(buf, 1, sizeof(buf), dirs);
   pclose(dirs);

	message.type = 1;
   strcpy(&message.text, buf);
   msgsnd(mq_id, &message, strlen(message.text), 0);

	memset(buf, 0, MAX_SIZE);


	// 2nd message
	printf("[INFO] Sending 2nd message\n");

	//FILE *files = popen("expr $(ls -lR | grep \"^-\" | wc -l) - $(ls -l | grep \"^-\" | wc -l)", "r");
	//FILE *files = popen("ls */ -lR | grep \"^-\" | wc -l", "r");
	//FILE *files = popen("tree -da | tail -n 1 | cut -f 1 -d ' '", "r");

	// what have I done...
	FILE *files = popen("expr $(tree -uafi | tail -n 1 | cut -d ' ' -f 1,3 | tr ' ' + | bc) - $(tree -uafi -L 1 | tail -n 1 | cut -d ' ' -f 1,3 | tr ' ' + | bc)", "r");
   fread(buf, 1, sizeof(buf), files);
   pclose(files);

   message.type = 2;
   strcpy(&message.text, buf);
   msgsnd(mq_id, &message, strlen(message.text), 0);

	memset(buf, 0, MAX_SIZE);


	// 3rd message
	printf("[INFO] Sending 3rd message\n");
	FILE *hostname = popen("hostname", "r");
   fread(buf, 1, sizeof(buf), hostname);
   pclose(hostname);

	message.type = 3;
   strcpy(&message.text, buf);
   msgsnd(mq_id, &message, strlen(message.text), 0);

	memset(&message.text, 0, MAX_SIZE);

	// get message from 2nd client
	printf("[INFO] Waiting for message from the 2nd client\n");
	msgrcv(mq_id, &message, sizeof(message.text), 4, 0);
	printf("[INFO] Message received\n\n");

	// print message type and content
	printf("Message type: %ld\n", message.type);
	printf("Message content:\n%s\n", &message.text);

	// delete message query
	printf("[INFO] Deleting message query\n");
   msgctl(mq_id, IPC_RMID, NULL);

	return 0;
}
