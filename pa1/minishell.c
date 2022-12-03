#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <grp.h>
#include <pwd.h>

int ls(char *dir_path, char *option);
int head(char *file_path, char *line);
int tail(char *file_path, char *line);
int mv(char *file_path1, char* file_path2);
int mk_dir(char *dir_path);
int rm_dir(char *dir_path);

#define MAX_CMD_LEN 200
void permissionFile(struct stat *check ,char * permission){
	if(S_ISDIR(check -> st_mode)) 
		permission[0]='d';
	else if(S_ISLNK(check -> st_mode))
		permission[0]='l';
	else if(S_ISCHR(check -> st_mode))
		permission[0]='c';
	else if(S_ISBLK(check -> st_mode))
		permission[0]='b';
	else if(S_ISSOCK(check -> st_mode))
		permission[0]='s';
	else if(S_ISFIFO(check -> st_mode))
		permission[0]='P';
	else
		permission[0]='-';

	if(check->st_mode & S_IRUSR)
		permission[1]='r';
	else
		permission[1]='-';

	if(check->st_mode & S_IWUSR)
		permission[2]='w';
	else
		permission[2]='-';

	if(check->st_mode & S_IXUSR)
		permission[3]='x';
	else if(check->st_mode & S_ISUID)
		permission[3]='s';
	else
		permission[3]='-';

	if(check->st_mode & S_IRGRP)
		permission[4]='r';
	else
		permission[4]='-';
	if(check->st_mode & S_IWGRP)
		permission[5]='w';
	else
		permission[5]='-';

	if(check->st_mode & S_IXGRP)
		permission[6]='x';
	else if(check->st_mode & S_ISGID)
		permission[6]='s';
	else
		permission[6]='-'; 

	if(check->st_mode & S_IROTH)
		permission[7]='r'; 
	else 
		permission[7]='-';

	if(check->st_mode & S_IWOTH)
		permission[8]='w';
	else
		permission[8]='-';

	if(check->st_mode & S_IXOTH) 
	{
		if(check->st_mode & S_ISVTX)
			permission[9]='t';
		else
			permission[9]='x';
	}
	else
	{
		if(check->st_mode & S_ISVTX)
			permission[9]='T';
		else
			permission[9]='-';
	}	

	permission[10]='\0';	
}

int main(){
	while(1){
		char error1[]="ERROR: The command is executed abnormally\n";
		char error2[]="ERROR: Invalid commands\n";
		int i = 0;
		int cmdret;
		char cmd[MAX_CMD_LEN];
		char *argument[10];
		size_t size;	
			/* Input commands */
		size = read(STDIN_FILENO,cmd,200);
		cmd[size-1]='\0';
			/* ============== */
			/* Tokenize commands (Use strtok function) */
		char *ptr= strtok(cmd," ");
		while(ptr != NULL){
			argument[i++]= ptr;
			ptr = strtok(NULL," ");
		}
		argument[i] = NULL;
			/* ======================================= */
			/* Command */
		if (strcmp("ls", argument[0]) == 0){
			cmdret = ls(argument[1], argument[2]);	
		}
		else if (strcmp("head", argument[0]) == 0){
			cmdret = head(argument[3], argument[2]);
		}
		else if (strcmp("tail", argument[0]) == 0){
			cmdret = tail(argument[3], argument[2]);
		}
		else if (strcmp("mv", argument[0]) == 0){
			cmdret = mv(argument[1], argument[2]);	
		}
		else if (strcmp("mkdir", argument[0]) == 0){
			cmdret = mk_dir(argument[1]);	
		}
		else if (strcmp("rmdir", argument[0]) == 0){
			cmdret = rm_dir(argument[1]);	
		}
		else if (strcmp("quit", argument[0]) == 0){
			break;
		}
		else{
			/* Print "ERROR: Invalid commands" */
			write(STDOUT_FILENO,error2,sizeof(error2));
            continue;
		}
		if (cmdret == -1){
			write(STDOUT_FILENO,error1,sizeof(error1));
            /* Print "ERROR: The command is executed abnormally" */
		}
	}
	return 0;
}
void changeNum(int num, char *data){
    int tmp = num;
    int cnt = 0;    
    char temp;
    while(1){
		if(tmp == 0){
			break;
		}
        tmp = tmp / 10;
        cnt = cnt + 1;
    }

	for(int i = 0; i < cnt; i++){
		data[i] =(char)(num %10+48);
		num= num / 10;
	}
  	for (int a = 0; a < cnt / 2; a++) {
    	temp = data[a];
    	data[a] = data[(cnt - 1) - a];
    	data[(cnt - 1) - a] = temp;
  	}

	data[cnt]='\0'; 	
}


int ls(char *dir_path, char *option){
	DIR *dirptr = NULL;
	struct stat buf;
	struct dirent *direntp;
	struct group * grp;
	struct passwd * pwd;
	char linkSize[10];
	char fileSize[10];
	char permission[10];
	char total[500];
	struct stat check;
	int kind;
	char ls_error[]="ERROR: invalid path\n";
	if(dir_path[0] == '~'){
		DIR *dirstr = NULL;
		struct stat buffer;
		struct dirent *dirensp;
		struct passwd * pwdd;		
		dir_path[0] = '/';
		dirstr=opendir(".");
		dirensp = readdir(dirstr);
		lstat(dirensp->d_name, &buffer);
		pwdd=getpwuid(buffer.st_uid);
		strcpy(total,pwdd->pw_dir);
		strcat(total,dir_path);
		closedir(dirstr); 
	}
	else{
		strcpy(total,dir_path);
	}
	stat (total, &check);
	kind = check.st_mode & S_IFMT;
	if(kind != S_IFDIR ){
		write(STDOUT_FILENO,ls_error,sizeof(ls_error));
		return -1;
	}	
	dirptr=opendir(total);
	if (dirptr == NULL){ 
		return -1;
	}
	else{
		while (direntp = readdir(dirptr)){
			lstat(direntp->d_name, &buf);
			pwd=getpwuid(buf.st_uid);
			grp=getgrgid(buf.st_gid);
			
			if(option == NULL ){
				if('.'!=direntp -> d_name[0]){
					write(STDOUT_FILENO,direntp->d_name,strlen(direntp->d_name));
					write(STDOUT_FILENO,"\n",1);	
				} 			
			}
			else{
				if(strcmp("-al", option) == 0){
					permissionFile(&buf,permission);
					changeNum(buf.st_nlink,linkSize);char head_error[]="ERROR: invalid path\n";
					write(STDOUT_FILENO,permission,strlen(permission));
					write(STDOUT_FILENO," ",1);
					write(STDOUT_FILENO,linkSize,strlen(linkSize));
					write(STDOUT_FILENO," ",1);
					write(STDOUT_FILENO,pwd->pw_name,strlen(pwd->pw_name));
					write(STDOUT_FILENO," ",1);
					write(STDOUT_FILENO,grp->gr_name,strlen(grp->gr_name));
					write(STDOUT_FILENO," ",1);
					write(STDOUT_FILENO,fileSize,strlen(fileSize)+1);
					write(STDOUT_FILENO," ",1);
					write(STDOUT_FILENO,direntp->d_name,strlen(direntp->d_name));
					write(STDOUT_FILENO,"\n",1);									
				}
				else{
					return -1;
				}
			}
		}
		closedir(dirptr); 
	} 
}

int head(char *file_path, char *line){
	struct stat check;
	int kind;
	char head_error[]="ERROR: invalid path\n";
	stat (file_path, &check);

	kind = check.st_mode & S_IFMT;
	if(kind == S_IFDIR ){
		write(STDOUT_FILENO,head_error,sizeof(head_error));
		return -1;
	}

	char buffer[10];
	int fd,fw,n=0;
	if((fd = open(file_path, O_RDONLY,0744)) < 0){
		return -1;
	}
	else{
		while(fw=read(fd,buffer,1) > 0){
			if(buffer[0]=='\n'){
				n++;
			}
			if( n == atoi(line)){
				break;
			}
			write(STDOUT_FILENO,buffer,fw);
		}
		close(fd);
	}
	write(STDOUT_FILENO,"\n",1);
}

int tail(char *file_path, char *line){
	struct stat check;
	int kind;
	char tail_error[] ="ERROR: invalid path\n";
	stat (file_path, &check);

	kind = check.st_mode & S_IFMT;
	if(kind == S_IFDIR ){
		write(STDOUT_FILENO,tail_error,sizeof(tail_error));
		return -1;
	}

	int fd,fw,n = 0;
	char buffer[10];
	int total=1;
	int need=0;
	if((fd = open(file_path, O_RDONLY,0744)) < 0){
		return -1;
	}
	else{
		while(fw=read(fd,buffer,1) > 0){
			if(buffer[0]=='\n'){
				total++;
			}
		}
		need=total- atoi(line);
		lseek(fd,-1,SEEK_END);
		read(fd,buffer,1);
		if(buffer[0] == '\n'){
			need=need-1;
		}
		lseek(fd,0,SEEK_SET);
		while(fw=read(fd,buffer,1) > 0){
			if(buffer[0]=='\n'){
				n++;
				if(n == need){
					continue;
				}
			}
			if(n >= need){
				write(STDOUT_FILENO,buffer,fw);
			}
		}
		close(fd);
	}	
	write(STDOUT_FILENO,"\n",1);
}

int mv(char *file_path1, char *file_path2){
	struct stat check,check2;
	int kind,kind2;

	stat (file_path1, &check);
	stat (file_path2, &check2);

	char mv_error[]="ERROR: invalid path\n";
	kind = check.st_mode & S_IFMT;
	kind2 = check2.st_mode & S_IFMT;

	if(kind == S_IFDIR ){
		write(STDOUT_FILENO,mv_error,sizeof(mv_error));
		return -1;
	}	
	int fd_in,fd_out,fr_in;
	char buffer[10];
	if((fd_in = open(file_path1, O_RDONLY,0744)) < 0){
		write(STDOUT_FILENO,mv_error,sizeof(mv_error));
		return -1;
	}
	else{
		if((fd_out= open(file_path2, O_CREAT | O_WRONLY | O_RDONLY,0755)) < 0){
			write(STDOUT_FILENO,mv_error,sizeof(mv_error));
			return -1;
		}	
		else{
			while(fr_in=read(fd_in,buffer,1) > 0){
				write(fd_out,buffer,fr_in);
			}
		}	
	}
	close(fd_in);
	close(fd_out);
	int del_file= unlink(file_path1);
	return del_file;
}

int mk_dir(char *dir_path){
	char mk_error[]="ERROR: invalid path\n";
	int result = mkdir(dir_path,0755);
	if(result == -1){
		write(STDOUT_FILENO,mk_error,sizeof(mk_error));
	}
	return result;
}

int rm_dir(char *dir_path){
	char rm_error[]="rmdir: failed to remove dir_path: Directory is not empty\n";
	int result = rmdir(dir_path);
	if(result == -1){
		write(STDOUT_FILENO,rm_error,sizeof(rm_error));
	}
	return result;
}
