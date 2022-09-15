#include <stdio.h>
#include <sys/msg.h>

#define PROJECT_ID 8841

int main() {
	char buf[8192];

	system("touch /tmp/lab5");
	key_t ipckey = ftok("/tmp/lab5", PROJECT_ID);

	int mq_id = msgget(ipckey, IPC_CREAT | 0600);
	if (mq_id == -1) {
		printf("Cannot open message query");
		return -1;
   }

	struct {
		long type;
		char text[8192];
	} message;


	// 1st message
	FILE *dirs = popen("tree -d | head --lines=-2 | sed '1d'", "r");
   fread(buf, 1, sizeof(buf), dirs);
   pclose(dirs);

	message.type = 1;
   strcpy(&message.text, buf);
   msgsnd(mq_id, &message, strlen(message.text), 0);

	memset(buf, 0, 8192);


	// 2nd message
	FILE *files = popen("expr $(ls -lR | grep \"^-\" | wc -l) - $(ls -l | grep \"^-\" | wc -l)", "r");
   fread(buf, 1, sizeof(buf), files);
   pclose(files);

   message.type = 2;
   strcpy(&message.text, buf);
   msgsnd(mq_id, &message, strlen(message.text), 0);

	memset(buf, 0, 8192);


	// 3rd message
	FILE *hostname = popen("hostname", "r");
   fread(buf, 1, sizeof(buf), hostname);
   pclose(hostname);

	message.type = 3;
   strcpy(&message.text, buf);
   msgsnd(mq_id, &message, strlen(message.text), 0);

	// ???
	//printf("Deleting messages query\n");
   //msgctl(mq_id, IPC_RMID, NULL);

	return 0;
}
