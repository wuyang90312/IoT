#include <stdio.h>
#include <string.h>

int deliminater(char token, int num, char* target, int* ptr)
{
	int i, length, loop=0;
	length = strlen(target);
	
	memset(ptr, 0x00, num*sizeof(int));

	for(i=0; i < num; i++)
		printf("%d\n", ptr[i]);
	
	i=0;
	while(loop<length)
	{
		if(target[loop] == token)
		{
			ptr[i] = loop;
			i++;
		}
		loop++;
	}

	if(i==num)
		return 1;

	return 0;
}

void main()
{
	char target[128] = "NAME=BCRLovs&PASS=23456&APIK=DSKLFJSDKLFJ&TIME=60";
	char tmp[64];
	int ptr[3],i;

	if(!deliminater('&', 3, target, ptr))
		printf("NULL\n");
		
	for(i=0; i < 3; i++)
		printf("%d\n", ptr[i]);
	
	memset(tmp, 0x00, sizeof(tmp));
	memcpy(tmp, target, ptr[0]);
	printf("%d  %s\n",ptr[0], tmp);
	memset(tmp, 0x00, sizeof(tmp));
	memcpy(tmp, target+ptr[0]+1, ptr[1]-ptr[0]);
	printf("%d  %s\n",ptr[1], tmp);
	memset(tmp, 0x00, sizeof(tmp));
	memcpy(tmp, target+ptr[1]+1, ptr[2]-ptr[1]);
	printf("%d  %s\n",ptr[2], tmp);
	memset(tmp, 0x00, sizeof(tmp));
	memcpy(tmp, target+ptr[2]+1, strlen(target)-ptr[1]-1);
	printf("[%s]\n", tmp);
}
