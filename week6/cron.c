#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <errno.h>
#include <limits.h>
#include <signal.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <syslog.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>

int main(void)
{
	unsigned int pid;
	time_t t;
	struct tm *tm;	
	int fd;
	char *argv[3];
	char buf[512];
	int fd0, fd1, fd2;

	pid = fork();
	
	if(pid == -1) return -1;
	if(pid != 0)
		exit(0);
	if(setsid() < 0)
		exit(0);
	if(chdir("/") < 0)
		exit(0);

	umask(0);

	close(0);
	close(1);
	close(2);

	fd0 = open("/dev/null", O_RDWR);
	fd1 = open("/dev/null", O_RDWR);
	fd2 = open("/dev/null", O_RDWR);
	t = time(NULL);
	tm = localtime(&t);

	while (1)
	{
		fd = open("/crontab", O_RDONLY);
        t = time(NULL);
        tm = localtime(&t);
        buf[0] = '\0';

        int count, i = 0, j= 0;

        while(count=read(fd,buf+i,1) > 0){
            i++;
        }
        char *token;
        char *pos = buf;

        while ((token = strtok_r(pos, " ", &pos))){
            argv[j]=token;
            j++;
        }

		int hour = atoi(argv[1]);
		int minute = atoi(argv[0]);

		if( hour == tm->tm_hour || strcmp(argv[1],"*") == 0){
			if( minute == tm->tm_min || strcmp(argv[0],"*") == 0){
				if(fork()==0){
					execl("/bin/sh", "/bin/sh", "-c", argv[2], (char*) NULL);
				}
			}
		}	
		sleep(60 - tm->tm_sec % 60);
	}

	return 0;
}
