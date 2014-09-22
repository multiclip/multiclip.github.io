#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>

#define SIG_TEST 44

uid_t userID;

void receiveData(int n, siginfo_t *info, void *unused)
{
	printf("received value %i %i\n", info->si_int & 0xFF, info->si_int>>8);
	//system("notify-send -t 500 sigrecived");
	
	int code = info->si_int & 0xFF;
	int type = info->si_int>>8;
	
	char cmd[100];
	if(type==0)//COPY
		sprintf(cmd, "DISPLAY=:0 XAUTHORITY=~/.Xauthority xclip -o > /dev/multiclip/board%d", code);
	else //PASTE
		sprintf(cmd, "DISPLAY=:0 XAUTHORITY=~/.Xauthority xclip -selection c -i /dev/multiclip/board%d", code);
	
	//setuid(userID);
	system(cmd);
	//setuid(0);
}

int main ( int argc, char **argv )
{
	int configfd;
	char buf[10];
	pid_t pid, sid;
	struct sigaction sig;
	
	userID = 1000;
	
	setlogmask(LOG_UPTO(LOG_NOTICE));
    openlog("multiclipd", LOG_CONS | LOG_NDELAY | LOG_PERROR | LOG_PID, LOG_USER);

    syslog(LOG_INFO, "Entering mutliclipdaemon\n");
	
	
	pid = fork();
	
	if (pid < 0) { exit(EXIT_FAILURE); }
	if (pid > 0) { exit(EXIT_SUCCESS); }
	umask(0);
	sid = setsid();
    if (sid < 0) { exit(EXIT_FAILURE); }
    if ((chdir("/")) < 0) { exit(EXIT_FAILURE); }
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
    
    sig.sa_sigaction = receiveData;
	sig.sa_flags = SA_SIGINFO;
	sigaction(SIG_TEST, &sig, NULL);
    
    configfd = open("/dev/multiclip/board", O_WRONLY);
	sprintf(buf, "%i", getpid());
	write(configfd, buf, strlen(buf) + 1);
	
	while(1);
	
	return 0;
}



