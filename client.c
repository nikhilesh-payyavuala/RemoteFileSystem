#include "hw2.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <dirent.h>
#include <time.h>

#define MAXCOMMANDLEN 10 /* maximum length of command is 9 */
#define MAXFILENAME 20   /* maximum length of file name is 19 */
#define MAXSTRLEN 1000

#define CAT "cat"
#define WRITEFILE "write"
#define FINDFILE "find"
#define DELETEFILE "rm"
#define LISTFILE "ls"


int main(int argc,char* argv[])
{
	CLIENT* cl;
	char* host;
	char operation[MAXCOMMANDLEN+1];
	char fname[MAXFILENAME+1];
	char line[40];
	char string[MAXSTRLEN + 1];
	int len;
	int* res;
	char *data;
	long* fsize;
	filenames* lst;
	fdata* fdata;


	if(argc<2)
	{
		printf ("usage: %s server_host\n", argv[0]);
		exit (1);
	}

	else if(argc==2)
	{
		printf("Type a command, format : ./client <host> <command>\n");
		exit(1);
	}
	else
	{
		host=(char*)malloc(sizeof(char)*20);
		                   host=argv[1];
		cl = clnt_create (host, HW2PROG, CLISERV, "tcp");
			if (cl == NULL) {
				clnt_pcreateerror (host);
				exit (1);
			}

                   printf("Host is %s\n",host);

                   strcpy(operation,argv[2]);
                   strcpy(line,argv[2]);
                   printf("Operation is %s \n",operation);
                   if(argc==3 ){

                	   //printf("%s\n",operation);
                        if ((strcmp(operation, CAT) 	== 0) 	|| 
			    (strcmp(operation, WRITEFILE) 	== 0) 	||	 
			    (strcmp(operation, FINDFILE)   	== 0) 	||
			    (strcmp(operation, DELETEFILE) 	== 0) ) {
				printf ("usage: %s operation [filename] [string | length]\n", argv[0]);
				exit(1);
			        }
                   }
                   else if(argc==4){
                	   strcpy(fname,argv[3]);
                	   strcat(line," ");
                	   strcat(line,argv[3]);
                	   printf("File name is %s\n",fname);
                   }
                   else if(argc>4)
                   {
                   	   strcat(line," ");
                	   strcat(line,argv[3]);
                	   strcat(line," ");
                	   strcat(line,argv[4]);
                	   strcpy(fname,argv[3]);
                	   strcpy (string, argv[4]);
                	   			len = atoi (string);

                	   			int i = 5;
                	   			while (i   < argc)
                	   			{  
                	   				strcat(line," ");
                	                                            strcat(line,argv[i]);
                	   				strcat(string, " ");
                	   				strcat(string, argv[i]);
                	   				i ++;
                                 }
                	   			printf("File name is %s\n",fname);
                	   			printf("String is %s\n",string);

	                 }
           }
	if(strcmp(operation,CAT)==0)
	{

		res = create_1(line,len,fname , cl);
			if (res == (int *) NULL) {
				clnt_perror (cl, "call failed");
			}

		if (*res == 0) {
							if (len <= 0) {
								printf("please specify the number of bytes to read from file %s\n", fname);
								clnt_destroy(cl);
								exit(1);
							}
				

                                            			data = malloc(len);
							fsize = read_1(line,data, len, fname,cl);
				
								if (*fsize <0) {
									printf("Error in opening:\n");
								} else {
									fdata=data_1(line,data,len,fname,cl);
									printf("Contents are %.*s\n",len ,(fdata->text));
								}

		             }

		else if(*res == -1)
		{
						printf("Error in opening %s\n", fname);
		}
		else
		{
						printf("File %s has been created \n", fname);
		}

	}
	else if(strcmp(operation, FINDFILE) == 0)
	{

				if ((fsize = find_1(line,fname,cl)) < 0) {
					printf("Could not find %s\n", fname);
				} else {
					printf("File %s has %ld bytes\n", fname, *fsize);
				}
	}

		else if (strcmp(operation, DELETEFILE) == 0)
		{
					res = delete_1(line,fname,cl);
					if (res== (int *) NULL) {
							clnt_perror (cl, "call failed");
						}

					if (*res == 0) {
						printf("File %s has been deleted\n", fname);
					} else if (*res == -2) {
						printf("File %s does not exist\n", fname);
					} else {
						printf("Error in deleting %s\n", fname);
					}
		}
		else if (strcmp(operation,LISTFILE) == 0)
		{
			lst = list_1(line,cl);
						if (lst == NULL) {
							printf("Error in listing:\n");
						} else {
							for (; lst != (filenames *) NULL; lst = lst->next) {
								printf("%s\n", lst->name);
							}
						}
		}
		else if (strcmp(operation, WRITEFILE) == 0) {
					if (argc <= 3) {
						printf("Please speficy data to write to file %s\n", fname);
						exit(1);
					}
					len = strlen(string);
		                	fsize = write_1(line,string, len, fname,cl);
		                	if (*fsize == (int *) NULL) {
		                			clnt_perror (cl, "call failed");
		                		}
		                	if (*fsize < 0) {
		                        	printf("Error in writing:\n");
		                	} else {
						printf("%ld bytes of data were written to file %s\n", *fsize, fname);
		                	}
		        	}
		else if (strcmp(operation, "exit") == 0) {
					printf("exiting...\n");
					exit(0);
				}

}






