#include <stdio.h>
#include <sys/msg.h>

#define PROJECT_ID 8841

int main() {
	
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
	
	return 0;
}

