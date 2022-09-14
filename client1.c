#include <stdio.h>
#include <sys/msg.h>

#define PROJECT_ID 8841

int main() {
	
	key_t ipckey;

    while(1) {
        ipckey = ftok("/tmp/lab5", PROJECT_ID);
        
		if(ipckey == -1) {
            printf("No messages\n");
            sleep(1);
        }
		else	break;
    }
	
	//get message query id
	int mq_id = msgget(ipckey, 0);
    if (mq_id == -1) {
        printf("Cannot open message query\n");
        exit(1);
    }
	
	struct
    {
        long type;
        char text[8192];
    } message;
	

	int st = msgrcv(mq_id, &message, sizeof(message.text), 2, 0);
	
	char *command;
	
	printf("Message content:\n%s", &message.text);
	

	
	return 0;
}