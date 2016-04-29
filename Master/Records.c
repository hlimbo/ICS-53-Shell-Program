#define BUFFER_SIZE 80
#define RECORD_SIZE 100
#define NAME_SIZE 20
#define ADDRESS_SIZE 40
#define PHONE_SIZE 20
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Record
{
	char name[NAME_SIZE];
	char address[ADDRESS_SIZE];
	char phone[PHONE_SIZE];
}sweet;

//returns the number of records and number of characters in the file.
//useful for debugging.
void recordStats(char* filename, int* recordCount,int* charCount)
{
	char c_ptr;
	FILE* someFile = fopen(filename,"r");
	*charCount = 0;
	*recordCount = 1;

	if(someFile == NULL)
	{
		*recordCount = -1;
		*charCount = -1;
		printf("File not found\n");
		return;
	}

	while(!feof(someFile))
	{
		++(*charCount);
		c_ptr = fgetc(someFile);
		if(c_ptr == '\n')
			++(*recordCount);
	}

	if(*charCount == 0)
		*recordCount = 0;

	fclose(someFile);
}

int read(char* filename,struct Record* records)
{
	int i = 0;
	char buf[BUFFER_SIZE];
	char nameBuf[NAME_SIZE];
	char addressBuf[ADDRESS_SIZE];
	char phoneBuf[PHONE_SIZE];
	const char delim[2] = "\t";
	char* token = NULL;
	unsigned long length;
	FILE* file_source = fopen(filename,"r");
	
	if(file_source == NULL)
	{
		printf("File not found\n");
		return 0;
	}

	fseek(file_source,0,SEEK_END);
	length = (unsigned long)ftell(file_source);
	if(length <= 0)
	{
		printf("File %s is empty\n",filename);
		fclose(file_source);
		return 0;
	}

	fseek(file_source,0,SEEK_SET);
	while(!feof(file_source))
	{
		if(i >= RECORD_SIZE)
		{
			printf("Warning...can only read 100 records in memory\n");
			fclose(file_source);
			return i;
		}

		fgets(buf,BUFFER_SIZE,file_source);
		token = strtok(buf,delim);
		strncpy(nameBuf,token,NAME_SIZE);
		token = strtok(NULL,delim);
		strncpy(addressBuf,token,ADDRESS_SIZE);
		token = strtok(NULL,delim);
		strncpy(phoneBuf,token,PHONE_SIZE);

		if(phoneBuf[strlen(phoneBuf) - 1] == '\n')
			phoneBuf[strlen(phoneBuf) -1] = '\0';

		strncpy(records[i].name,nameBuf,NAME_SIZE);
		strncpy(records[i].address,addressBuf,ADDRESS_SIZE);
		strncpy(records[i++].phone,phoneBuf,PHONE_SIZE);
		token = NULL;
	
	}

	strncat(records[i - 1].phone,"\n",PHONE_SIZE);
	fclose(file_source);
	return i;
}

void write(char* filename,struct Record* records,int size)
{
	int i;
	FILE* file_source = fopen(filename,"w");

	if(size <= 0)
	{
		printf("There are no records to write from memory\n");
	}
	else
	{
		//printf("number of records: %d\n",size);
		for(i = 0;i < size;++i)
		{
			fprintf(file_source,"%s%c",records[i].name,'\t');
			fprintf(file_source,"%s%c",records[i].address,'\t');
			if(records[i].phone[strlen(records[i].phone) - 1] == '\n')
				records[i].phone[strlen(records[i].phone) - 1] = '\0';
			fprintf(file_source,"%s",records[i].phone);
			if(i + 1 == size)
				break;
			else
				fprintf(file_source,"%c",'\n');
	
		}
	}


	fclose(file_source);
}


void print(struct Record* records,int recCount)
{
	int i;
	for(i = 0;i < recCount;++i)
		printf("%d %s\t%s\t%s\n",i + 1,records[i].name,records[i].address,records[i].phone);
}

void deleteRecord(int index,struct Record* records,int* recCount)
{
	int i;
	if(index > *recCount || index < 0)
	{
		printf("index %d is out of bounds\n",index);
		return;
	}

	if(*recCount <= 0)
	{
		printf("there are number %d records stored in memory\n",*recCount);
		return;
	}

	for(i = index - 1;i < *recCount;++i)
	{
		records[i] = records[i + 1];
	}
	

	--(*recCount);

}

void quit()
{
	printf("quitting program..\n");
}
int main(int argc,char* argv[])
{
	struct Record records[RECORD_SIZE];
	int index;
	int recCount = 0;

	char cmd[60];
	char action[10];
	char arg1[50];

	char* token;
	char* c_ptr;
	const char delim[3] = " \t";

	const char* readCmd = "read";
	const char* writeCmd = "write";
	const char* printCmd = "print";
	const char* deleteCmd = "delete";
	const char* quitCmd = "quit";

	int lineCount = 0;
	int charCount = 0;

	//printf("Record Keeper Program:\n\nCommands:\n");
	//printf("1. read\n2. write\n3. print\n4. delete\n5. quit\n");

	while(strcmp(cmd,quitCmd) != 0)
	{
		printf("\nEnter a command: ");
		fgets(cmd,60,stdin);
	
		//remove \n character from read command.
		cmd[strlen(cmd) -1] = 0;
		
		//parse command here. ~ change to scanf
		token = strtok(cmd,delim);
		if(token == NULL)
		{
			printf("Invalid comnand\n");
			continue;
		}
		strncpy(action,token,10);
		token = strtok(NULL,delim);
		if(token != NULL)
			strncpy(arg1,token,50);

		if(strcmp(action,readCmd) == 0)
		{
			recCount = read(arg1,records);
		}
		else if(strcmp(action,writeCmd) == 0)
		{
			write(arg1,records,recCount);
		}
		else if(strcmp(action,printCmd) == 0)
		{
			print(records,recCount);
		}
		else if(strcmp(action,deleteCmd) == 0)
		{
			index = atoi(arg1);
			deleteRecord(index,records,&recCount);
		}
		else if(strcmp(action,quitCmd) == 0)
		{
			quit();
			break;
		}
		else
			printf("Not a supported command: %s\n",action);

		token = NULL;
	}

	return 0;
}

