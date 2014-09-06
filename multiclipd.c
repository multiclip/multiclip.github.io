#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>

#define SIG_TEST 44

void receiveData(int n, siginfo_t *info, void *unused)
{
	printf("received value %i %i\n", info->si_int & 0xFF, info->si_int>>8);
	//system("notify-send -t 500 sigrecived");
	
	int code = info->si_int & 0xFF;
	int type = info->si_int>>8;
	
	char cmd[100];
	if(type==0)//COPY
		sprintf(cmd, "xclip -o > /dev/multiclip/board%d", code);
	else //PASTE
		sprintf(cmd, "xclip -selection c -i /dev/multiclip/board%d", code);
	system(cmd);
}

int main ( int argc, char **argv )
{
	int configfd;
	char buf[10];
	
	struct sigaction sig;
	sig.sa_sigaction = receiveData;
	sig.sa_flags = SA_SIGINFO;
	sigaction(SIG_TEST, &sig, NULL);

	configfd = open("/dev/multiclip/board", O_WRONLY);
	sprintf(buf, "%i", getpid());
	write(configfd, buf, strlen(buf) + 1);
	
	while(1);
	
	return 0;
}



