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
	msgrcv(mq_id, &message, sizeof(message.text), 1, 0);

	printf("Message type: %ld\n", message.type);
	printf("Message content:\n%s", &message.text);

	// send message to the server
	FILE *size = popen("du -b | head -n -1", "r");
   fread(buf, 1, sizeof(buf), size);
   pclose(size);

	message.type = 4;
   strcpy(&message.text, buf);
	printf("\n[INFO] Sending message\n");
   msgsnd(mq_id, &message, strlen(message.text), 0);

   return 0;
}
