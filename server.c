


#include "hw2.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <dirent.h>
#include <time.h>


long lookup(char *filename) {
	int status;
	struct stat statbuf;

	if (stat(filename, &statbuf) == -1) {
		return -1;
	}

	return (long)statbuf.st_size;

}

void logging(char* logs)
{
	FILE *log;
  log = fopen("server.log","a");
  struct tm *ptr_time;
  time_t time_raw_format;
  time(&time_raw_format);
  ptr_time = localtime(&time_raw_format);
  printf("[%d/%d %d:%d:%d]	", ptr_time->tm_mon+1,ptr_time->tm_mday,ptr_time->tm_hour, ptr_time->tm_min, ptr_time->tm_sec);
  printf("%s\n", logs);
  fprintf(log, "[%d/%d %d:%d:%d]	", ptr_time->tm_mon+1,ptr_time->tm_mday,ptr_time->tm_hour, ptr_time->tm_min, ptr_time->tm_sec);
  fprintf(log, "%s\n", logs);
  fclose(log);
}


int *
create_1_svc(char* line,int len,char* fname ,  struct svc_req *rqstp)
{
	static int  result;
           FILE* log;
         
	log= fopen("server.log","a");

	if (lookup(fname) > -1) {
		result=0;
		if(len<0){
			
			logging(line);
			printf("Please specify the number of bytes to read from file %s\n",fname );
			fprintf(log, "Please speficy the number of bytes to read from file %s\n", fname );
		}
		fclose(log);
		return &result;
		}

		logging(line);
		FILE *fp = fopen(fname, "w+");
		if (fp == NULL) {
			result=-1;
			return &result;
		}
		fclose(fp);
		result= 1;
		printf("File %s has been created \n", fname);

		fprintf(log, "File %s has been created \n", fname);
	           fclose(log);
	return &result;
}

int *
delete_1_svc(char* line,char *filename,  struct svc_req *rqstp)
{
	static int  result;
          logging(line);
           FILE *log;
  log = fopen("server.log","a");
	if (lookup(filename) < 0) {
		    result=-2;
		    printf("File %s doesn't exist to remove\n", filename);
	   	    fprintf(log, "File %s doesn't exist to remove\n", filename);
	               fclose(log);
			return &result;
		}
			result= remove(filename);
		
	
	printf("removed file %s \n", filename);
           fprintf(log,"removed file %s \n", filename);
	fclose(log);
	

	return &result;
}

int *
write_1_svc(char* line,char *data, int length, char *filename,  struct svc_req *rqstp)
{
	static int  result;
	logging(line);
	 FILE *log;
            log = fopen("server.log","a");
	FILE *fp = fopen(filename, "w+");
		if (fp == NULL) {
			result=-1;
			return &result;
		}
		int size = fwrite(data, sizeof(char), length, fp);
		result= size;
		fclose(fp);



	printf("% d bytes of data were written to file %s\n", result, filename);
	fprintf(log,"% d bytes of data were written to file %s\n", result, filename);
	fclose(log);
		
		

	return &result;
}

long *
find_1_svc(char* line,char *filename,  struct svc_req *rqstp)
{
	logging(line);
	 FILE *log;
            log = fopen("server.log","a");
	 static long  result;

	result=lookup(filename);
           if(result<0){


                  printf("Could not find %s\n", filename);
                  fprintf(log, "Could not find %s\n", filename );
          }
          else{
                     printf("File %s has %ld bytes\n", filename, result);
                     fprintf(log,"File %s has %ld bytes\n", filename, result );
          }
          fclose(log);

	return &result;
}

filenames *
list_1_svc(char* line,struct svc_req *rqstp)
{
          logging(line);
          FILE *log;
          log = fopen("server.log","a");
	DIR *pdir;
		struct dirent *ent;
		filenames *tmp;
		static filenames *lst;

		pdir = opendir("."); /*assuming the simple file system is located in the current directory */
		if (pdir == NULL) {
			return NULL;
		}

		lst = (filenames *) NULL;
		while((ent = readdir(pdir)) != NULL) {
			tmp = (filenames *) malloc(sizeof(filenames));
			tmp->name = (char *) malloc(sizeof(ent->d_name)+1);
			strcpy (tmp->name, ent->d_name);
			tmp->next = lst;
			lst = tmp;


			printf("%s \n", lst->name);
		fprintf(log, "%s \n", lst->name); 
		}
		fclose(log);
		closedir(pdir);


	return lst;
}

int*
read_1_svc(char* line,char *data, int length, char *filename,  struct svc_req *rqstp)
{
	static int result;
	FILE *log;
           log = fopen("server.log","a");
    
          logging(line);
	FILE *fp = fopen(filename, "r");
		if (fp == NULL) {
			result = -1;
			return &result;
		}
		int size = fread(data, sizeof(char), length, fp);



		fclose(fp);
		fclose(log);
                     result=size;
                     return &result;
}

fdata* 
data_1_svc(char* line,char* data,int length,char*filename, struct svc_req *rqstp)
{

fdata* tmp;
FILE* log;
log=fopen("server.log","a");  

FILE *fp;
  fp = fopen(filename,"r");
  tmp = (fdata *)malloc(sizeof(filenames));


	tmp->text = (char *) malloc(length);
	fread(data, sizeof(char), length, fp);
           printf("data read: %.*s\n",length, data);





	fprintf(log,"data read: %.*s \n",length, data); 	
	fclose(log);
	strcpy(tmp->text,data);
           fclose(fp);	
	return(tmp);
}
