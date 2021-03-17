#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <libgen.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>
#include <readline/readline.h>
#include <readline/history.h>


int help(char **arguments);
int version(char **arguments);
int shell_exit(char **arguments);
int cp(char **arguments);
int mv(char **arguments);
int _dirname(char **arguments);

char *shell_commands[] = {
  "cp",
  "mv",
  "dirname",
  "help",
  "version",
  "exit"
};

int (*shell_functions[]) (char **) = {
  &cp,
  &mv,
  &_dirname,
  &help,
  &version,
  &shell_exit
};

int cp(char **arguments)
{
	
	if(strcmp("-i",arguments[1])==0)
		{
			int no_of_args = 0;
			int offset = 0;
			char **aruments_cpy = arguments;
			while(*(arguments+offset)!='\0')
			{
				++no_of_args;
				++offset;
			}
			if(no_of_args != 4)
			{
				fprintf(stderr, "cp: usage cp -i <file> <file>\n");
				return 1;
			}
			printf("cp: overwrite '%s'? y/n ", arguments[3]);
			char input_option;
			scanf("%c", &input_option);
			if(input_option=='y')
			{
				char character;
				FILE *source_file, *target_file;

				source_file = fopen(arguments[2], "r");

   				if (source_file == NULL)
   				{
    				fprintf(stderr,"cp: File couldnt be open to read\n");
    				return 1;
   				}

   				target_file = fopen(arguments[3], "w");
 
   				if (target_file == NULL)
   				{
      				fclose(source_file);
      				fprintf(stderr,"cp: File couldnt be open to write\n");
      				return 1;
   				}

   				while ((character = fgetc(source_file)) != EOF)
    				fputc(character, target_file);

    			fclose(target_file);

   				fclose(source_file);
   				
   				return 1;
			}
			else
				return 1;
			
		}
	else if(strcmp("-r",arguments[1])==0)
		{
			int no_of_args = 0;
			int offset = 0;
			char **aruments_cpy = arguments;
			while(*(arguments+offset)!='\0')
			{
				++no_of_args;
				++offset;
			}
			if(no_of_args != 4)
			{
				fprintf(stderr,"cp: usage cp -r <arg> <arg>\n");
				return 1;
			}
			DIR *source_dir;
			DIR *target_dir;
			struct dirent *init;

			struct stat target_dir_info;

			if(lstat(arguments[3],&target_dir_info) != 0)
			{
				if(errno == ENOENT)
					mkdir(arguments[3],0700);
				if((target_dir = opendir(arguments[3])) == NULL){
					fprintf(stderr,"cp: Cannot open target directory.\n");
					return 1;
				}
			}

			if(S_ISDIR(target_dir_info.st_mode))
				target_dir = opendir(arguments[3]);
			if((source_dir = opendir(arguments[2])) == NULL){
				fprintf(stderr,"cp: Cannot open source directory.\n");
				return 1;
			}
			else
			{
				struct stat entry_check;
				while((init = readdir(source_dir)) != NULL)
				{
					lstat(init->d_name,&entry_check);
					if(S_ISDIR(entry_check.st_mode))
						continue;
					else if(S_ISREG(entry_check.st_mode))
					{
						char* source_filepath = (char*)malloc(sizeof(char));
						sprintf(source_filepath,"%s/%s",arguments[2],init->d_name);
						
						char* destination_filepath = (char*)malloc(sizeof(char));
						sprintf(destination_filepath,"%s/%s",arguments[3],init->d_name);

						if(creat(destination_filepath,0644) == -1)
						{
							fprintf(stderr,"cp: Error creating file.\n");
							return 1;
						}

						FILE *source_file, *target_file;
						char character;

						source_file = fopen(source_filepath, "r");

		   				if (source_file == NULL)
		   					{
		    				fprintf(stderr,"cp: File couldnt be open to read\n");
		    				return 1;
		   					}

		   				target_file = fopen(destination_filepath, "w");

		   				if (target_file == NULL)
		   				{
		      				fclose(source_file);
		      				fprintf(stderr,"cp: File couldnt be open to write\n");
		      				return 1;
		   				}

		   				while ((character = fgetc(source_file)) != EOF)
		    				fputc(character, target_file);

		    			fclose(target_file);

		   				fclose(source_file);
					}
				}
				closedir(source_dir);
				closedir(target_dir);
			}
			return 1;
		}
	else if(strcmp("-t",arguments[1])==0)
		{
			int no_of_args1= 0;
			int offset1 = 0;
			char **aruments_cpy1 = arguments;
			while(*(arguments+offset1)!='\0')
			{
				++no_of_args1;
				++offset1;
			}
			if(no_of_args1 < 4)
			{
				fprintf(stderr,"cp: usage cp -t <folder> <file>...\n");
				return 1;
			}
			int source_files = 0;
			int offset = 0;
			char **aruments_cpy = arguments;
			while(*(arguments+offset)!='\0')
			{
				++source_files;
				++offset;
			}


			struct stat directory_info;
			char* directory_name = arguments[2];

			if(stat(directory_name,&directory_info) == -1)
			{
				printf("cp: Directory does not exist. -- Creating directory...\n");
				mkdir(directory_name,0700);
			}

			for(int i=3;i<source_files;i++)
			{
				char* filename = arguments[i];
				char* filepath = (char*)malloc(sizeof(char));

				sprintf(filepath,"%s/%s",directory_name,filename);


				if(creat(filepath,0644) == -1)
				{
					fprintf(stderr,"cp: Error creating file.\n");
					return 1;
				}

				FILE *source_file, *target_file;
				char character;

				source_file = fopen(filename, "r");

   				if (source_file == NULL)
   					{
    				fprintf(stderr,"cp: File couldnt be open to read\n");
    				return 1;
   					}

   				target_file = fopen(filepath, "w");

   				if (target_file == NULL)
   				{
      				fclose(source_file);
      				fprintf(stderr,"cp: File couldnt be open to write\n");
      				return 1;
   				}

   				while ((character = fgetc(source_file)) != EOF)
    				fputc(character, target_file);

    			fclose(target_file);

   				fclose(source_file);

			}

			return 1;

		}
	else if(strcmp("-v",arguments[1])==0)
		{
			int no_of_args1= 0;
			int offset1 = 0;
			char **aruments_cpy1 = arguments;
			while(*(arguments+offset1)!='\0')
			{
				++no_of_args1;
				++offset1;
			}
			if(no_of_args1 != 4)
			{
				fprintf(stderr,"cp: usage cp -v <file> <file>\n");
				return 1;
			}

			printf("%s is being copied into %s\n", arguments[2],arguments[3]);
			FILE *source_file, *target_file;
			char character;

			source_file = fopen(arguments[2], "r");

   			if (source_file == NULL)
   			{
    			fprintf(stderr,"cp: File couldnt be open to read\n");
    			return 1;
   			}

   			target_file = fopen(arguments[3], "w");

   			if (target_file == NULL)
   			{
      			fclose(source_file);
      			fprintf(stderr,"cp: File couldnt be open to write\n");
      			return 1;
   			}

   			while ((character = fgetc(source_file)) != EOF)
    			fputc(character, target_file);

    		fclose(target_file);

   			fclose(source_file);
   			return 1;
		}
	else
	{
		int no_of_args1= 0;
		int offset1 = 0;
		char **aruments_cpy1 = arguments;
		while(*(arguments+offset1)!='\0')
		{
			++no_of_args1;
			++offset1;
		}
		if(no_of_args1 != 3 || (no_of_args1==1))
		{
			fprintf(stderr,"cp: usage cp <arg> <arg>\n");
			return 1;
		}

		FILE *source_file, *target_file;
		char character;

		source_file = fopen(arguments[1], "r");

   		if (source_file == NULL)
   		{
    		fprintf(stderr,"cp: File couldnt be open to read\n");
    		return 1;
   		}

   		target_file = fopen(arguments[2], "w");

   		if (target_file == NULL)
   		{
      		fclose(source_file);
      		fprintf(stderr,"cp: File couldnt be open to write\n");
      		return 1;
   		}

   		while ((character = fgetc(source_file)) != EOF)
    		fputc(character, target_file);

    	fclose(target_file);

   		fclose(source_file);
   		return 1;
	}
	return 1;
}

int mv(char **arguments)
{

	if(strcmp("-i",arguments[1])==0)
		{
		int no_of_args1= 0;
		int offset1 = 0;
		char **aruments_cpy1 = arguments;
		while(*(arguments+offset1)!='\0')
		{
			++no_of_args1;
			++offset1;
		}
		if(no_of_args1 != 4)
		{
			fprintf(stderr,"mv: usage mv -i <arg> <arg>\n");
			return 1;
		}
		struct stat argument_info;

		stat(arguments[3],&argument_info);

		if(S_ISDIR(argument_info.st_mode))
		{
			char* filename = arguments[2];
			char* filepath = (char*)malloc(sizeof(char));

			sprintf(filepath,"%s/%s",arguments[3],filename);

			struct stat file_arg;
			if(stat(filepath,&file_arg)==0)
			{
				printf("mv: overwrite %s? y/n ", arguments[2]);
				char input_option;
				scanf("%c", &input_option);
				if(input_option=='y')
				{
	   				FILE *source_file, *target_file;
					char character;

					source_file = fopen(arguments[2], "r");

		   			if (source_file == NULL)
		   			{
		    			fprintf(stderr,"mv: File couldnt be open to read\n");
		    			return 1;
		   			}

		   			target_file = fopen(filepath, "w");

		   			if (target_file == NULL)
		   			{
		      			fclose(source_file);
		      			fprintf(stderr,"mv: File couldnt be open to write\n");
		      			return 1;
		   			}

			   		while ((character = fgetc(source_file)) != EOF)
			    		fputc(character, target_file);

			    	fclose(target_file);

			   		fclose(source_file);

			   		remove(arguments[2]);

			   		return 1;
				}
				else
					return 1;
			}
			else
			{
				FILE *source_file, *target_file;
				char character;

				source_file = fopen(arguments[2], "r");

	   			if (source_file == NULL)
	   			{
	    			fprintf(stderr,"mv: File couldnt be open to read\n");
	    			return 1;
	   			}

	   			target_file = fopen(filepath, "w");

	   			if (target_file == NULL)
	   			{
	      			fclose(source_file);
	      			fprintf(stderr,"mv: File couldnt be open to write\n");
	      			return 1;
	   			}

		   		while ((character = fgetc(source_file)) != EOF)
		    		fputc(character, target_file);

		    	fclose(target_file);

		   		fclose(source_file);

		   		remove(arguments[2]);

		   		return 1;
			}
		}
		else
		{
			int no_of_args1= 0;
			int offset1 = 0;
			char **aruments_cpy1 = arguments;
			while(*(arguments+offset1)!='\0')
			{
				++no_of_args1;
				++offset1;
			}
			if(no_of_args1 != 3)
			{
				fprintf(stderr,"mv: usage mv <arg> <arg>\n");
				return 1;
			}
			struct stat file_arg;
			if(stat(arguments[3],&file_arg)==0)
			{
				printf("mv: overwrite %s? y/n ", arguments[3]);
				char input_option;
				scanf("%c", &input_option);
				if(input_option=='y')
				{
	   				FILE *source_file, *target_file;
					char character;

					source_file = fopen(arguments[2], "r");

		   			if (source_file == NULL)
		   			{
		    			fprintf(stderr,"mv: File couldnt be open to read\n");
		    			return 1;
		   			}

		   			target_file = fopen(arguments[3], "w");

		   			if (target_file == NULL)
		   			{
		      			fclose(source_file);
		      			fprintf(stderr,"mv: File couldnt be open to write\n");
		      			return 1;
		   			}

			   		while ((character = fgetc(source_file)) != EOF)
			    		fputc(character, target_file);

			    	fclose(target_file);

			   		fclose(source_file);

			   		remove(arguments[2]);

			   		return 1;	
				}
				else
					return 1;
			}
			else
			{
				FILE *source_file, *target_file;
				char character;

				source_file = fopen(arguments[2], "r");

	   			if (source_file == NULL)
	   			{
	    			fprintf(stderr,"mv: File couldnt be open to read\n");
	    			return 1;
	   			}

	   			target_file = fopen(arguments[3], "w");

	   			if (target_file == NULL)
	   			{
	      			fclose(source_file);
	      			fprintf(stderr,"mv: File couldnt be open to write\n");
	      			return 1;
	   			}

		   		while ((character = fgetc(source_file)) != EOF)
		    		fputc(character, target_file);

		    	fclose(target_file);

		   		fclose(source_file);

		   		remove(arguments[2]);

		   		return 1;	

			}		
		}
	}
	else if(strcmp("-t",arguments[1])==0)
		{
			int no_of_args1= 0;
			int offset1 = 0;
			char **aruments_cpy1 = arguments;
			while(*(arguments+offset1)!='\0')
			{
				++no_of_args1;
				++offset1;
			}
			if(no_of_args1 < 4)
			{
				fprintf(stderr,"mv: usage mv -t <folder> <file>...\n");
				return 1;
			}
			int source_files = 0;
			int offset = 0;
			char **aruments_cpy = arguments;
			while(*(arguments+offset)!='\0')
			{
				++source_files;
				++offset;
			}


			struct stat directory_info;
			char* directory_name = arguments[2];
			if(stat(directory_name,&directory_info) == -1)
			{
				fprintf(stderr,"mv: Directory does not exist.\n");
				return 1;
			}

			for(int i=3;i<source_files;i++)
			{
				char* filename = arguments[i];
				char* filepath = (char*)malloc(sizeof(char));

				sprintf(filepath,"%s/%s",directory_name,filename);


				if(creat(filepath,0644) == -1)
				{
					fprintf(stderr,"mv: Error creating file.\n");
					return 1;
				}

				FILE *source_file, *target_file;
				char character;

				source_file = fopen(filename, "r");

   				if (source_file == NULL)
   					{
    				fprintf(stderr,"mv: File couldnt be open to read\n");
    				return 1;
   					}

   				target_file = fopen(filepath, "w");

   				if (target_file == NULL)
   				{
      				fclose(source_file);
      				fprintf(stderr,"mv: File couldnt be open to write\n");
      				return 1;
   				}

   				while ((character = fgetc(source_file)) != EOF)
    				fputc(character, target_file);

    			fclose(target_file);

   				fclose(source_file);

   				remove(filename);

			}

			return 1;

		}
	else if(strcmp("-S",arguments[1])==0)
		{
			int no_of_args1= 0;
			int offset1 = 0;
			char **aruments_cpy1 = arguments;
			while(*(arguments+offset1)!='\0')
			{
				++no_of_args1;
				++offset1;
			}
			if(no_of_args1 != 5)
			{
				fprintf(stderr,"mv: usage mv -S <arg(file)> <arg(file/dir)> <suffix name(2nd arg)>\n");
				return 1;
			}
			struct stat argument_info;

			stat(arguments[3],&argument_info);

			if(S_ISDIR(argument_info.st_mode))
			{

				char* filename = arguments[2];
				char* filepath = (char*)malloc(sizeof(char));

				sprintf(filepath,"%s/%s",arguments[3],filename);

				struct stat file_info;

				if(stat(filepath,&file_info)==0)
				{
					char* backup_filename = filepath;
					char* backup_filepath = (char*)malloc(sizeof(char));

					sprintf(backup_filepath,"%s%s",backup_filename,arguments[4]);

					FILE *backup_source_file, *backup_target_file;
					char character;

					backup_source_file = fopen(backup_filename, "r");

		   			if (backup_source_file == NULL)
		   			{
		    			fprintf(stderr,"mv: File couldnt be open to read\n");
		    			return 1;
		   			}

		   			backup_target_file = fopen(backup_filepath, "w");

		   			if (backup_target_file == NULL)
		   			{
		      			fclose(backup_source_file);
		      			fprintf(stderr,"mv: File couldnt be open to write\n");
		      			return 1;
		   			}

			   		while ((character = fgetc(backup_source_file)) != EOF)
			    		fputc(character, backup_target_file);

			    	fclose(backup_target_file);

			   		fclose(backup_source_file);
				}

				FILE *source_file, *target_file;
				char character;

				source_file = fopen(arguments[2], "r");

	   			if (source_file == NULL)
	   			{
	    			fprintf(stderr,"mv: File couldnt be open to read\n");
	    			return 1;
	   			}

	   			target_file = fopen(filepath, "w");

	   			if (target_file == NULL)
	   			{
	      			fclose(source_file);
	      			fprintf(stderr,"mv: File couldnt be open to write\n");
	      			return 1;
	   			}

		   		while ((character = fgetc(source_file)) != EOF)
		    		fputc(character, target_file);

		    	fclose(target_file);

		   		fclose(source_file);

		   		remove(arguments[2]);

		   		return 1;

			}
			else
			{
				struct stat file_info;

				if(stat(arguments[3],&file_info)==0)
				{
					char* backup_filename = arguments[3];
					char* backup_filepath = (char*)malloc(sizeof(char));

					sprintf(backup_filepath,"%s%s",backup_filename,arguments[4]);

					FILE *backup_source_file, *backup_target_file;
					char character;

					backup_source_file = fopen(backup_filename, "r");

		   			if (backup_source_file == NULL)
		   			{
		    			fprintf(stderr,"mv: File couldnt be open to read\n");
		    			return 1;
		   			}

		   			backup_target_file = fopen(backup_filepath, "w");

		   			if (backup_target_file == NULL)
		   			{
		      			fclose(backup_source_file);
		      			fprintf(stderr,"mv: File couldnt be open to write\n");
		      			return 1;
		   			}

			   		while ((character = fgetc(backup_source_file)) != EOF)
			    		fputc(character, backup_target_file);

			    	fclose(backup_target_file);

			   		fclose(backup_source_file);
				}

				FILE *source_file, *target_file;
				char character;

				source_file = fopen(arguments[2], "r");

	   			if (source_file == NULL)
	   			{
	    			fprintf(stderr,"mv: File couldnt be open to read\n");
	    			return 1;
	   			}

	   			target_file = fopen(arguments[3], "w");

	   			if (target_file == NULL)
	   			{
	      			fclose(source_file);
	      			fprintf(stderr,"mv: File couldnt be open to write\n");
	      			return 1;
	   			}

		   		while ((character = fgetc(source_file)) != EOF)
		    		fputc(character, target_file);

		    	fclose(target_file);

		   		fclose(source_file);

		   		remove(arguments[2]);

		   		return 1;			
			}
		}
	else
	{
		int no_of_args1= 0;
		int offset1 = 0;
		char **aruments_cpy1 = arguments;
		while(*(arguments+offset1)!='\0')
		{
			++no_of_args1;
			++offset1;
		}
		if(no_of_args1 != 3 || (no_of_args1==1))
		{
			fprintf(stderr,"mv: usage mv <arg> <arg>\n");
			return 1;
		}
		struct stat argument_info;

		stat(arguments[2],&argument_info);

		if(S_ISDIR(argument_info.st_mode))
		{
			char* filename = arguments[1];
			char* filepath = (char*)malloc(sizeof(char));

			sprintf(filepath,"%s/%s",arguments[2],filename);

			FILE *source_file, *target_file;
			char character;

			source_file = fopen(arguments[1], "r");

   			if (source_file == NULL)
   			{
    			fprintf(stderr,"mv: File couldnt be open to read\n");
    			return 1;
   			}

   			target_file = fopen(filepath, "w");

   			if (target_file == NULL)
   			{
      			fclose(source_file);
      			fprintf(stderr,"mv: File couldnt be open to write\n");
      			return 1;
   			}

	   		while ((character = fgetc(source_file)) != EOF)
	    		fputc(character, target_file);

	    	fclose(target_file);

	   		fclose(source_file);

	   		remove(arguments[1]);

	   		return 1;

		}
		else
		{
			FILE *source_file, *target_file;
			char character;

			source_file = fopen(arguments[1], "r");

   			if (source_file == NULL)
   			{
    			fprintf(stderr,"mv: File couldnt be open to read\n");
    			return 1;
   			}

   			target_file = fopen(arguments[2], "w");

   			if (target_file == NULL)
   			{
      			fclose(source_file);
      			fprintf(stderr,"mv: File couldnt be open to write\n");
      			return 1;
   			}

	   		while ((character = fgetc(source_file)) != EOF)
	    		fputc(character, target_file);

	    	fclose(target_file);

	   		fclose(source_file);

	   		remove(arguments[1]);

	   		return 1;			
		}

	}
	return 1;
}

int _dirname(char **arguments)
{
	int buffer_size = 64;
	char *dirname = arguments[1];
	char **name_components = malloc(buffer_size * sizeof(char*));
	int name_position = 0;

	if (!name_components) {
    	fprintf(stderr, "Allocation error\n");
    	return 1;
  }

	char *dirname_split = strtok(dirname, "/");

	while(dirname_split!=NULL)
	{
		name_components[name_position] = dirname_split;
		name_position++;
		if(name_position >= buffer_size)
		{
     		buffer_size =  buffer_size + 64;
     		name_components = realloc(name_components, buffer_size * sizeof(char*));
      		if (!name_components) {
       			fprintf(stderr, "Allocation error\n");
        		return 1;
      		}
    	}
    	dirname_split = strtok(NULL, "/");
	}

	name_components[name_position] = NULL;

	if(name_position!=1){
		for(int i =0; i<name_position-1; i++)
			printf("/%s", name_components[i]);
		printf("\n");
	}
	else
		printf(".\n");
	return 1;
}

int help(char **arguments)
{
  printf("The following commands are created in this shell :\n");

  for (int i = 0; i < sizeof(shell_commands) / sizeof(char *); i++) {
    printf("  %s\n", shell_commands[i]);
  }

  return 1;
}

int version(char **arguments)
{
	printf("Final version of the shell. \n Created by Monea Sebastian\n");
	return 1;
}

int shell_exit(char **arguments)
{
  return 0;
}

int launch_command(char **arguments)
{
  pid_t process_pid, wait_pid;
  int process_status;

  if((process_pid = fork())<0)
  	perror("Error forking");
  
  if (process_pid == 0) {
    if (execvp(arguments[0], arguments) == -1) {
      perror("Error executing command");
    }
    exit(EXIT_FAILURE);
  } 

  do {
      wait_pid = waitpid(process_pid, &process_status, WUNTRACED);
  } while (!WIFEXITED(process_status) && !WIFSIGNALED(process_status));
  
  return 1;
}

int execute_commands(char **arguments)
{
	if (arguments[0] == NULL) {
    	return 1;
  	}

  	for (int i = 0; i < sizeof(shell_commands) / sizeof(char *); i++) {
    	if (strcmp(arguments[0], shell_commands[i]) == 0) {
      		return (*shell_functions[i])(arguments);
    	}
  	}

	return launch_command(arguments);
}

char **shell_line_parser(char *cmd_line)
{
  int buffer_size = 64;
  char *line_token;
  int token_position = 0;
  char **commands = malloc(buffer_size * sizeof(char*));

  if (!commands) {
    fprintf(stderr, "Allocation error\n");
    exit(EXIT_FAILURE);
  }

  line_token = strtok(cmd_line, " \t\r\n\a");
  while (line_token != NULL) {
    commands[token_position] = line_token;
    token_position++;

    if (token_position >= buffer_size) {
      buffer_size =  buffer_size + 64;
      commands = realloc(commands, buffer_size * sizeof(char*));
      if (!commands) {
        fprintf(stderr, "Allocation error\n");
        exit(EXIT_FAILURE);
      }
    }

    line_token = strtok(NULL, " \t\r\n\a");
  }
  commands[token_position] = NULL;
  return commands;
}

int execute_pipe(char **lcommand, char **rcommand)
{
	int pipe_file_descriptor[2];
	pid_t pid1,pid2;
	if(pipe(pipe_file_descriptor) == -1)
	{
		printf("Error creating pipe.\n");
		return 1;
	}

	pid1 = fork();
	if(pid1 == 0)
	{
		dup2(pipe_file_descriptor[1],1);
		close(pipe_file_descriptor[0]);
		close(pipe_file_descriptor[1]);
		execute_commands(lcommand);
	}
	else
	{
		pid2 = fork();
		if(pid2 == 0)
		{
			dup2(pipe_file_descriptor[0],0);
			close(pipe_file_descriptor[0]);
			close(pipe_file_descriptor[1]);
			execute_commands(rcommand);	
			return 1;
		}
		else
		{
			return 1;
		}
	}

	return 1;
}

int execute_redirection(char **leftpart,char **rightpart,int redirection_type)
{
	int file_descriptor0,file_descriptor1;
	pid_t pid;

	pid = fork();

	if((pid = fork())== -1)
		fprintf(stderr, "Error forking.\n");
	else if (pid == 0)
	{
		if(redirection_type == 1) //1 for redirection out
		{
			int file_descriptor0;
			if((file_descriptor0 = open(rightpart[0],O_RDONLY,0)) < 0)
			{
				fprintf(stderr, "Error opening file.\n");
				return 1;
			}
			dup2(file_descriptor0,STDIN_FILENO);
			close(file_descriptor0);
		}

		if(redirection_type == 0) //0 for redirection in
		{
			int file_descriptor1;
			if((file_descriptor1 = creat(leftpart[0],0644)) < 0)
			{
				fprintf(stderr, "Error writing to file.\n");
				return 1;
			}

			dup2(file_descriptor1, STDOUT_FILENO);
			close(file_descriptor1);

		}

		if(redirection_type == 1)
		{
			execute_commands(leftpart);
		}else if (redirection_type == 0)
		{
			execute_commands(rightpart);
		}

	}

}

int main(int argc, char *argv[])
{
	int exit_status; 

	do {
		char **commands;

    	printf("$> ");
    
    	char *shell_line = NULL;
  		ssize_t line_buffersize = 0; 
  	
  		if (getline(&shell_line, &line_buffersize, stdin) == -1){
    
    	if (feof(stdin)) {
      	exit(EXIT_SUCCESS);  
    	} 
    	else{
      	perror("Error reading line");
      	exit(EXIT_FAILURE);
   			}
		}

		//add_history(shell_line);
  
    	commands = shell_line_parser(shell_line);

    	char **commands_copy = commands;
    	char **left_command_pipe = malloc(64 * sizeof(char*));
    	char **right_command_pipe = malloc(64 * sizeof(char*));

    	char **left_command_redirin = malloc(64 * sizeof(char*));
    	char **right_command_redirin = malloc(64 * sizeof(char*));

    	char **left_command_redirout = malloc(64 * sizeof(char*));
    	char **right_command_redirout = malloc(64 * sizeof(char*));

    	int no_of_commands=0;
    	int offset = 0;
		while(*(commands+offset)!='\0')
		{
			++no_of_commands;
			++offset;
		}

		bool pipe_found = false;
		int pipe_found_at;

		int redirin_found_at;
		int redirout_found_at;

		bool redirection_in = false;
		bool redirection_out = false;

		for(int i=0;i<no_of_commands;i++)
		{
			if(strcmp("|",commands_copy[i])==0){
				pipe_found=true;
				pipe_found_at = i+1;
			}
			if(pipe_found == false && strcmp("|",commands_copy[i]))
				left_command_pipe[i] = commands_copy[i];
			else if((pipe_found == true) && strcmp("|",commands_copy[i])) 
				right_command_pipe[i-pipe_found_at] = commands_copy[i];
		}

		for(int i=0;i<no_of_commands;i++)
		{
			if(pipe_found==true)
				break;

			if(strcmp("<",commands_copy[i])==0){
				redirection_in=true;
				redirin_found_at = i+1;
			}
			if(redirection_in == false && strcmp("<",commands_copy[i]))
				left_command_redirin[i] = commands_copy[i];
			else if((redirection_in == true) && strcmp("<",commands_copy[i])) 
				right_command_redirin[i-redirin_found_at] = commands_copy[i];
		}

		for(int i=0;i<no_of_commands;i++)
		{
			if(pipe_found==true || redirection_in == true)
				break;

			if(strcmp(">",commands_copy[i])==0){
				redirection_out=true;
				redirout_found_at = i+1;
			}
			if(redirection_out == false && strcmp(">",commands_copy[i]))
				left_command_redirout[i] = commands_copy[i];
			else if((redirection_out == true) && strcmp(">",commands_copy[i])) 
				right_command_redirout[i-redirout_found_at] = commands_copy[i];
		}

		if(pipe_found == true)
		{
			exit_status = execute_pipe(left_command_pipe,right_command_pipe);
			exit_status = 1;
		}
		else if(redirection_in==true)
		{
			exit_status = execute_redirection(left_command_redirin,right_command_redirin,0); //0 for redirection in
		}
		else if(redirection_out==true)
		{
			exit_status = execute_redirection(left_command_redirout,right_command_redirout,1); //1 for redirection out
		}
		else{
			exit_status = execute_commands(commands);
		}

		free(left_command_pipe);
		free(right_command_pipe);

		free(left_command_redirin);
		free(right_command_redirin);
		
		free(left_command_redirout);
		free(right_command_redirout);


	}while(exit_status);

	return 0;
}