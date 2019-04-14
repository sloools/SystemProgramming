#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <pwd.h>

#define SIZE 100

void access_perm(char *perm, mode_t mode);

int main(int argc, char *argv[]){
	DIR *dp;
	char *ptr;
	struct stat statbuf;
	struct dirent *dent;
	struct passwd *pwd;
	char pathname[100];
	char perm[11];

	if(argc < 2 ){
		fprintf(stderr, "no directory name is provided\n");
		exit(1);
	}
	else if(argc == 2){

		if(chdir(argv[1]) < 0){
			perror("chdir failed");
			exit(1);
		}
		else {
			if(access(argv[1], F_OK == -1)){
				perror("access error!\n");
				exit(1);
				}
			if(stat(argv[1], &statbuf) < 0){
				perror("stat error\n");
				exit(1);
			}
			if(!S_ISDIR(statbuf.st_mode)){
				fprintf(stderr, "%s is not a directory\n", argv[1]);
				exit(1);
			}
			if((dp = opendir(argv[1])) == NULL){
				perror("opendir error!\n");
				exit(1);
			}

			ptr = getcwd(NULL, SIZE);
			printf("Current Working Directory : %s\n", ptr);
			
			printf("Lists of Directory(%s) : \n", argv[1]);
			while((dent = readdir(dp)) != NULL){

				sprintf(pathname, "%s/%s", argv[1], dent->d_name);
				if(stat(pathname, &statbuf) < 0){
					perror("stat erorr!\n");
					exit(1);
				}
				access_perm(perm, statbuf.st_mode);
				pwd = getpwuid(statbuf.st_uid);
				printf("%s %d %d %ld %s\n", perm, pwd->pw_uid, pwd->pw_gid, dent->d_ino, dent->d_name);
			}
			closedir(dp);
			exit(0);
			

		       	/*system("pwd"); *execute command*/
	}
}
}
void access_perm(char *perm, mode_t mode){
	int i;
	char permchar[] = "rwx";

	memset(perm, '-', 10);
	perm[10] = '\0';

	if(S_ISDIR(mode)) perm[0] = 'd';
	else if(S_ISCHR(mode)) perm[0] = 'c';
	else if(S_ISBLK(mode)) perm[0] = 'b';
	else if(S_ISFIFO(mode)) perm[0] = 'p';
	else if(S_ISLNK(mode)) perm[0] = 'l';


	for(i = 0; i < 9; i++){
		if((mode << i) & 0x100)
			perm[i+1] = permchar[i%3];
	}
}


