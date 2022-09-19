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
	int st = msgrcv(mq_id, &message, sizeof(message.text), 1, 0);

	printf("Message type: %ld\n", message.type);
	printf("Message content:\n%s", &message.text);

	char *command;
   int s = asprintf(&command, "du -b */");
   if(s < 0) {
      printf("Error: cannot print\n");
   }


	char buf[8192];
	FILE *size = popen("du -b */", "r");
   fread(buf, 1, sizeof(buf), size);
   pclose(size);

	message.type = 4;
   strcpy(&message.text, buf);
	printf("\n[INFO] Sending message\n");
   msgsnd(mq_id, &message, strlen(message.text), 0);

/*
	printf("\nSize:\tDirectory:\n");

	int code;
   // parallel process
   if(fork() == 0) {
        execl("/bin/sh", "sh", "-c", command, (char*)0);
   }
   else {
      // wait until child process die
      wait(&code);
   }
*/
   return 0;
}
