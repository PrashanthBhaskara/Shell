#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
char error_message[30] = "An error has occurred\n"; //error message specified

void myPrint(char *msg)
{
    write(STDOUT_FILENO, msg, strlen(msg));
}
//int  *stat;


int execute(char* command, int numwords)
{
	

	// myPrint("made it to command");
	int flag = 0;
	if(strstr(command, ">") !=NULL)
	{
		// myPrint("\n");
//		myPrint("Additionally flag goes off");
		flag = 1;
	}
	if(flag)
	{
	}
	
	char* sad = strdup(command);
	char* cmd = strdup(command);

//	myPrint(command);I
	//char delimit[] = {' ', '\n', '\t'};
	char* newparse[514];
	int argnums = 0;
	char *cur = strtok(sad, " \n\t");
	if(cur == NULL)
	{
		//myPrint("HERE");
		return 1;
	}
    while(cur!= NULL)
    {


    	newparse[argnums] = cur; //loading in tokens into arg
    	cur = strtok(NULL, " \n\t");//tokenizing again in order to reinput args
    	argnums++;
   
    }
    //myPrint("here too \n");
    if(argnums ==0)
    {
    	return 1;
    }
    newparse[argnums] = NULL; //new parse has each word in the command
    int rflag;

    //check to see if > or >+ exist in any of these commands
    int redirection = 0;
    int advredirection = 0;
   // myPrint("HERE\n");

    for(int i = 0; i < argnums; i++)
    {
    	for(int j = 0; j < strlen(newparse[i]); j++)
    	{
    		//myPrint(newparse[i]);
    		if(newparse[i][j] == '>')
    		{
    			if(i == 0)
    			{
    				rflag = 1;
    			}
    			if(newparse[i][j+1] == '+')
    			{
    				// myPrint("HERE2\n");
    		
    				advredirection++;
    			}	

    			else
    			{
    				
    			
    				redirection++;
    			}
    		}
    	}	
	}
	//myPrint("HERE2\n");

		//checking to see if > is part of the first command of either cd, exit, or pwd



	if(rflag)
	{
	if (strstr(newparse[0], "cd") !=0|| strcmp(newparse[0], "exit")!=0 || strstr(newparse[0], "pwd") !=0)
	{

			myPrint(error_message);
		
			return 1; 
	}
}




    if((redirection>1 || advredirection>1) || (redirection>=1 && advredirection >=1)) //both cannot be present in one command and both cant be here more than once
    {
//	    myPrint("here what\n");
    	myPrint(error_message);
    	
    	return 1;
    }

   // if(redirection>)

    if((redirection ==0 && advredirection ==0) && flag == 0) //first we will do built ins
    { 

    	//the first arg in the new command will be one of the built ins, as they are the onlu place to show up
    	//we use newparse[0] to check for all built ins and if we cant find any then we move to executing
    	//cd
    	if(strcmp(newparse[0], "cd") == 0)
    	{

    		    for(int i = 0; i < argnums; i++)
    {
    	if(strcmp(newparse[i], ">") == 0)
    	{
    		myPrint(error_message);
    		return 1;
    	}
    }
//		myPrint("we at least know cd is seen here\n");


    		if (argnums >2)
    		
		{
//			myPrint("hello?");
    			myPrint(error_message);
    		
    			return 1;
    		}
    		else if (argnums == 2)
    		{
//			myPrint("HERE\n");
    			if(chdir(newparse[1]) != 0)// || strcmp(newparse[1], ">") == 0 )
    			{
//				 myPrint("DARN");
    				myPrint(error_message);
    				
    				return 1;
    			}
			return 1;
    		}
    		else
    		{
    
    			chdir(getenv("HOME"));
    			return 1;
    		}

    	}
    	else if(strcmp(newparse[0], "pwd") == 0)


    	{

  for(int i = 0; i < argnums; i++)
    {
    	if(strcmp(newparse[i], ">") == 0)
    	{
    		myPrint(error_message);
    		return 1;
    	}
    }
    		if(argnums>1)
    		{
//			myPrint("pwd error noo\n");
    			myPrint(error_message);
    			
    			return 1;
    		}
    		else
    		{
			//myPrint("flag2\n");
    			
    			char wd[514];
    			myPrint( getcwd(wd, 514));
    			//myPrint("\n");
    			myPrint("\n");  
    			return 1;  
    						
    		}
    	}
    	else if(strcmp(newparse[0], "exit") == 0) 
    	{

   for(int i = 0; i < argnums; i++)
    {
    	if(strcmp(newparse[i], ">") == 0)
    	{
    		myPrint(error_message);
    		return 1;
    	}
    }
    		if(argnums>1)
    		{
    			myPrint(error_message);
    			
    			return 1;
    		}
    		else
    		{
 
    			exit(0);
   

    		}
    	}
    }
  
    pid_t pid= fork(); //initially, having separate fork processes didnt work so i put them all together to make it easier
    if(pid ==0)
    {

    	if(redirection ==1)
    	{

    		char* file;
 			int argnums = 0;
 			char* args[514];
 		//	char de[] = {' ', '\n', '\t', '>'};
			char *cur = strtok(cmd, " \n\t>");
   		 	while(cur!= NULL)
    		{
    			// myPrint(cur);
    			// myPrint("\n");
    			args[argnums] = cur; //loading iIn tokens into arg
    			cur = strtok(NULL, " \n\t>");//tokenizing again in order to reinput args
    			argnums++;
    		}
   			file = args[argnums-1];
   			if(file == NULL)
   			{
   			 myPrint(error_message);
   			 // myPrint("HERE\n");	
   			 	return 1;
   			}
    		args[argnums-1] = NULL;
			// myPrint("We are here now\n");
    	    int fd;
  

//Check
    	//	fd = open(file, O_WRONLY | O_EXCL | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
   //  		myPrint("FD OPENED\n");
			// myPrint(file);

			if ((fd = open(file, O_WRONLY | O_EXCL | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR)) <= 0)
			{
				// myPrint("HERE1\n");
				myPrint(error_message); 
				exit(0); 
			} 
			else 
			{
				if (dup2(fd, STDOUT_FILENO) < 0)
				{
					myPrint(error_message); 
					exit(0);
				}
			}	
			close(fd);
			if (execvp(args[0], args) <0) 
			{
				// myPrint("HERE3\n");
				myPrint(error_message);
				exit(0);
			}
			exit(0);  //get rid of	exit(0);
		}


    			
    	


    	else if (advredirection == 1)
    	{
  //  		myPrint("HERE2\n");
    		char* file;
 			int argnums = 0;
 			char* args[514];
 		//	char de[] = {' ', '\n', '\t', '>', '+'};
			char *cur = strtok(cmd, " \n\t>+");
   		 	while(cur!= NULL)
    		{
    //			myPrint(cur);
    //			myPrint("\n");
    			args[argnums] = cur; //loading in tokens into arg
    			cur = strtok(NULL, " \n\t>+");//tokenizing again in order to reinput args
    			argnums++;
    		}

   			file = args[argnums-1];
   			if(file == NULL)
   			{
   //				myPrint("errorfound\n");
   			 myPrint(error_message);
   			 // myPrint("HERE\n");	
   			 	return 1;
   			}
    		args[argnums-1] = NULL;
    		int fd = 0;
    		//since we have o check if file already exiss we can just open it and see if it works


    		FILE* f = fopen(file, "r");
    	if(!f) //the file has not been created, in which case we just copy and paste from normal redirection
    	{


			if ((fd = open(file, O_WRONLY | O_EXCL | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR)) <= 0)
			{
				// myPrint("HERE1\n");
				myPrint(error_message); 
				exit(0); 
			} 
			else 
			{
				if (dup2(fd, STDOUT_FILENO) < 0)
				{
					// myPrint("HERE2\n");
					myPrint(error_message); 
					exit(0);
				}
			}
		//	write(fd,"e", 1);	
			close(fd);
			if (execvp(args[0], args) <0) 
			{
				// myPrint("HERE3\n");
				myPrint(error_message);
				exit(0);
			}
			exit(0);  //get rid of	exit(0);
		}
    		else//if the file already exists we need to make a child process 

    		{
    			char* tmp = "temp";



    pid_t p2 = fork();
   if(p2 == 0) //child
    {
		int f;

		if( (f =  (open(tmp, O_EXCL | O_RDWR | O_CREAT, S_IRUSR | O_TRUNC, 0777) )) <=0)
	{
		myPrint(error_message); 
		exit(0); 
		
	} 
	else if (dup2(f, STDOUT_FILENO) < 0)
	{
	
			myPrint(error_message); 
		close(f);
	}
			if (execvp(args[0], args) <0) 
			{
				// myPrint("HERE3\n");
				myPrint(error_message);
				exit(0);
			}
			exit(0);  //get rid of	exit(0);
		}
		else
		{
			wait(NULL);
		}


//code on what to do

	}
}







    	else
    	{
    		if (execvp(newparse[0], newparse) !=0) 
			{
      			myPrint(error_message);
      			exit(0);
  			}
    		exit(0);
 		}
 	}

   	else
    {
    	wait(NULL);
    }
    return 1;
}



    	



int main(int argc, char *argv[]) 
{
	if(argc > 2)
	{myPrint(error_message);
		exit(2); }

	int length = 1006; //specified  by p4?

//	char* cmags [1000]; //arbitrarilly long list of string indices for parsing
    char cmd_buff[length];
    char *pinput;
    FILE *bfile;
    if(argc ==2)
    {
    		bfile = fopen(argv[1], "r");

    	    if(bfile == NULL)
    		{
    			//myPrint(error_message);
    			//printf("error\n");
    			exit(0);
    		}
    }


    while (1) 
    {


    	if(argc ==1) // interactive
    	{
    		myPrint("myshell> ");
    		pinput = fgets(cmd_buff, 1006, stdin);
        	if (!pinput) 
        	{
        		
        		//printf("error\n");
        	//	printf("error\n");
           		exit(0);
        	}

        		//int counter = 0;
		          if(strlen(cmd_buff) >= 514)
		          {
		          	// for(int i = strlen(cmd_buff)-1; i > 0; i--)
		          	// {
		          	// 	if(cmd_buff[i] != ' ')
		          	// 	{
		          	// 		counter = i;
		          	// 		break;
		          	// 	}
		          	// }
		          	// char msg[counter+1];
		          	// for(int i = 0; i <=counter; i++)
		          	// {
		          	// 	msg[i] = cmd_buff[i];
		          	// }
		          	// msg[counter+1] = '\0';

		          	
                
                        myPrint(cmd_buff);

			                        myPrint("\n");
                        myPrint(error_message);
                        continue;
                }

    	}



    	else if(argc ==2) //batch, havent accounted for not equaling 0 or 1 yet
    	{




    		pinput = fgets(cmd_buff, 1006, bfile);
        	if (!pinput) 
        	{
        		
        		//printf("error\n");
           		exit(0);
        	}
 if(strlen(cmd_buff) >= 514)
 {

                        myPrint(cmd_buff);

			                        myPrint("\n");
                        myPrint(error_message);
                        continue;

			// cmd_buff[strlen(cmd_buff)-1] = '\0';
			// //Check for blank line 
			// char* checkline = strdup(cmd_buff); 
			// char* stripped = strtok(checkline, " \n\t");
			// if (stripped == NULL)
			// {
			// 	continue; 
			// }
			// //myPrint(cmd_buff); 
			// myPrint("\n"); 
			// if (strlen(cmd_buff) >= 514) 
			// {
   //     	 		myPrint(error_message);
   //      		continue;
   //    		}
     }	



                




		char* mop = strdup(pinput);
		char* star;
		star = strtok(mop, " \n\t\r")	;
		
			if(star == NULL)
			{
				//myPrint("True");
			}
			else
			{
				myPrint(pinput);

			}

    	}

	char* cmags[1006];

    	
    	//tokenize input according to linitations referenced in p4 (more needed)
    	char *cur;
    	cur = strtok(pinput, ";\n"); //delimit); //tokenizing to input string, another tokenization needed
    	int argnums= 0;
    	while(cur!= NULL)
    	{
    
    		cmags[argnums] = cur; //loading in tokens into arg
    		cur = strtok(NULL, ";\n");// delimit);//tokenizing again in order to reinput args
    		argnums++;
    	}
    	cmags[argnums] = NULL; //ending array, so cmags now has our args as per the delimiter

    	for(int i = 0; i < argnums; i++)
    	{

		   
    	 execute(cmags[i], argnums);
        }



    }
}







