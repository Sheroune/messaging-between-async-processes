#include <stdio.h>
#include <sys/msg.h>

#define PROJECT_ID 8841

int main() {
	char buf[1024];
	
	system("touch /tmp/lab5");
	key_t ipckey = ftok("/tmp/lab5", PROJECT_ID);
	
	int mq_id = msgget(ipckey, IPC_CREAT | 0600);
    if (mq_id == -1) {
        printf("Cannot open message queue");
        return -1;
    }
	
	struct {
		long type;
		char text[8192];
	} message;
	
	
	FILE *hostname = popen("hostname", "r");
    fread(buf, 1, sizeof(buf), hostname);
    pclose(hostname);
    
	message.type = 3;
    strcpy(&message.text, buf);
    msgsnd(mq_id, &message, strlen(message.text), 0);
	
	
	return 0;
}

