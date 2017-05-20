#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

int updateFile(char *fileName,char **fileId){
	pid_t pid;
	int fd[2];
	int pip;
	pip=pipe(fd);
		
		pid=fork();
		if(-1==pid){
			printf("进程创建失败");
			return 1;
		}else if(0==pid){
			close(fd[0]);
			dup2(fd[1],STDOUT_FILENO);
			printf("开始长传文件:\n",fileName);
			execlp("fdfs_upload_file","fdfs_upload_file","/etc/fdfs/client.conf",fileName,NULL);
		}else if(0<pid){
			close(fd[1]);
			char *val=malloc(128);
			read(fd[0],val,128);
			*fileId=val;
			free(val);
			val=NULL;
		}
		return 0;
}

int main(int argc,char *argv[]){
	
	if(argc<2){
		printf("请输入要上传的文件\n");
		exit(1);
	}
	char *fileId;
	int flag=updateFile(argv[1],&fileId);
	if(1==flag){
		printf("文件上传失败");
		return 1;	
	}
	printf("文件上传成功,ID:%s",fileId);
	
	return 0;	
}