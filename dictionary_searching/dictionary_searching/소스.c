#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define wordsize 20
#define formsize 10
#define des 1100

#pragma warning(disable:4013)
#pragma warning(disable:4996)

typedef struct dataset
{
	char word_name[wordsize];
	char word_form[formsize];
	char word_des[des];
} Data;

Data text[200000];

int check_token(char a[])
{
	int i = 0;

	while (a[i] != '\0')
	{
		if (a[i] == '(')
			return 1;
		i++;
	}

	return 2;
}

int get_dic(char a[])
{
	char buffer[des], deli[2] = " ", *token = NULL;
	int file_state, size = 0, i = 0, val = 0, val1, val2 = 0;
	FILE *dic_f = NULL;

	dic_f = fopen(a, "r");

	if (dic_f == NULL)
		return -1;

	while (fgets(buffer, sizeof(buffer), dic_f) != NULL)
	{
		val = 0, val1 = 0;

		token = strtok(buffer, deli);

		if (strcmp(token, "\n") == 0 )
			continue;

		while (token)
		{
			val2 = check_token(token);

			if (val == 0 && val2 == 2)
			{
				if (val1 > 0)
					strcat(text[i].word_name, " ");
				strcat(text[i].word_name, token);
				val1++;
			}
			else if (val2 == 1)
			{
				strcpy(text[i].word_form, token);
				val++;
			}
			else if (val >= 1)
			{
				strcat(text[i].word_des, token);
				strcat(text[i].word_des, " ");
			}
			token = strtok(NULL, deli);
		}
		i++;
	}

	file_state = fclose(dic_f);

	if (file_state == EOF)
		puts("파일 닫기 오류");

	return i;
}

void fix_arr(char *a)
{
	int i, j;

	for (i = 0; i < wordsize; i++)
	{
		if (a[i] == '-' || a[i] == '\'')
		{
			for (j = i; j < wordsize - 1; j++)
				a[j] = a[j + 1];
		}
	}
	a[0] = toupper(a[0]);
	a[wordsize - 1] = '\n';
}


int find(char a[], int n, int m, int *c)
{
	char b[wordsize] = { 0 };
	int val, sum = 0;

	sum = (n + m) / 2;
	strcpy(b, text[sum].word_name);
	fix_arr(b);
	
	val = strcmp(b, a);

	if (m - n == 1)
	{
		if (val == 0)
		{
			c[0] = 0;
			return n;
		}
		else
		{
			strcpy(b, text[m].word_name);
			fix_arr(b);

			if (strcmp(b, a) == 0)
			{
				c[0] = 0;
				return m;
			}
			else
			{
				c[0] = 1;
				return n;
			}
		}
	}

	if (val == 0)
	{
		c[0] = 0;
		return sum;
	}
	else if (val == -1)
	{
		return find(a, sum, m, c);
	}
	else if (val == 1)
	{
		return find(a, n, sum, c);
	}
	else
	{
		c[0] = 2;
		return -1;
	}
}

void find_same(int val, int *a, const int c[])
{
	int i, j, be = 0, la = 0;

	if (c[0] == 0)
	{
		for (i = 1; i <= val; i++)
		{
			if (strcmp(text[val].word_name, text[val - i].word_name) == 0)
				be++;
			else
				break;
		}

		for (j = 1; j <= val; j++)
		{
			if (strcmp(text[val].word_name, text[val + j].word_name) == 0)
				la++;
			else
				break;
		}

		a[0] = be;
		a[1] = la;
	}
	else if (c[0] == 1)
	{
		a[0] = 0;
		a[1] = 1;
	}
}

void print(int val, int *a, int c[])
{
	int i, val2;
	val2 = a[1] + a[0] + 1;

	if (c[0] == 2)
		printf("가장 근접한 단어가 없습니다.");

	if (c[0] == 0)
	{
		if (val2 == 1)
			printf("find %ditem\n", val2);
		else if (val2 > 1)
			printf("find %d items\n", val2);

		for (i = val - a[0]; i <= val + a[1]; i++)
			printf("%s\t%s\t%s\n", text[i].word_name, text[i].word_form, text[i].word_des);
	}
	else
	{
		printf("Not found\n");
		for (i = val - a[0]; i <= val + a[1]; i++)
		{
			if (i == val + 1)
				printf("- - -\n");
			printf("%s\t%s\n", text[i].word_name, text[i].word_form);
		}
	}
	

}

int main()
{
	char a[wordsize] = { 0 }, opcode[30] = { 0 }, *token = NULL, op[] = " ";
	int size = 0, val, i = 0, j = 0, be_la[2] = { 0 }, c[1] = { 0 }, val1 = 0;

	while (val1 == 0)
	{
		printf("전자사전입니다. 읽을 파일을 입력하세요 ex) read dict.txt: ");
		gets(opcode);

		token = strtok(opcode, op);
		if (strcmp(token, "read") == 0)
		{
			token = strtok(NULL, op);
			size = get_dic(token) - 1;
			if (size == -2)
			{
				printf("파일 열기 오류입니다. 파일 이름을 확인해주세요");
				continue;
			}
		}
		val1++;
	}
	
	while (1)
	{
		printf("찾고자 하는 단어를 입력해주세요 ex) find yesterday\n사전의 단어 수를 나타내는 명령어는 size\n종료는 exit입니다.\n");
		gets(opcode);

		if (strcmp(opcode, "size") == 0)
			printf("%d\n", size);
		else if (strcmp(opcode, "exit") == 0)
			exit(1);
		else
		{
			if (strcmp(opcode, "") == 0)
			{
				printf("명령어를 확인해주세요\n");
				continue;
			}

			token = strtok(opcode, op);

			if (strcmp(token, "find") == 0)
			{
				token = strtok(NULL, op);
				strcpy(a, token);
				token = strtok(NULL, op);
				while (token != NULL)
				{
					strcat(a, " ");
					strcat(a, token);
					token = strtok(NULL, op);
				}
				fix_arr(a);
				val = find(a, 0, size, c);
				find_same(val, be_la, c);
				print(val, be_la, c);
			}
		}

	}
	return 0;
}