#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

typedef struct stack_int
{
	double data;
	struct stack_int *next;
} stack_i;

typedef struct stack_char
{
	char data;
	struct stack_char *next;
} stack_c;

void push(stack_i **ps, double x)
{
	stack_i *tmp = malloc(sizeof(stack_i));
	if (tmp != NULL)
	{
		tmp -> next = *ps;
		tmp -> data = x;
		*ps = tmp;
	}
}

void push_char(stack_c **ps, char x)
{
	stack_c *tmp = malloc(sizeof(stack_i));
	if (tmp != NULL)
	{
		tmp -> next = *ps;
		tmp -> data = x;
		*ps = tmp;
	}
}

double pop(stack_i **ps)
{
	if (*ps != NULL)
	{
		double result;
		stack_i *out = *ps;
		*ps = (*ps) -> next;
		result = out -> data;
		free(out);
		return result;
	}
	return 0;
}

char pop_char(stack_c **ps)
{
	if (*ps != NULL)
	{
		char result;
		stack_c *out = *ps;
		*ps = (*ps) -> next;
		result = out -> data;
		free(out);
		return result;
	}
	return 0;
}

double peek(const stack_i *ps)
{
	if (ps != NULL) return ps -> data;
	return 0;
}

int peek_char(const stack_c *ps)
{
	if (ps != NULL) return ps -> data;
	return 0;
}

int isDigit(char digit)
{
	if 		((digit == '0') || (digit == '1') || (digit == '2') || (digit == '3') ||
	         (digit == '4') || (digit == '5') || (digit == '6') || (digit == '7') ||
	         (digit == '8') || (digit == '9') || (digit == ',') || (digit == '.'))
	{
		return 1;
	}
	return 0;
}

int isOp(char op)
{
	if ((op == '+') || (op == '-') || (op == '/') || (op == '*') || (op == '^')) return 1;
	return 0;
}

int getPriority(char c)
{
	switch (c)
	{
	case '(': return 1;
	case '+': return 2;
	case '-': return 2;
	case '*': return 3;
	case '/': return 3;
	case '^': return 4;
	}
	return 0;
}

int main()
{
	FILE *input = NULL;
	FILE *output = NULL;
	FILE *poland = NULL;
	input = fopen("input.txt", "r");
	output = fopen("output.txt", "w");
	poland = fopen("poland.txt", "w");
	stack_c *opstack = NULL;
	stack_i *valstack = NULL;
	char c;
	fscanf(input, "%c", &c);

	// ПЕРЕВОД В ОПЗ

	while (!feof(input))
	{
		if (c == '\n') break;
		if (c == ' ')
		{
			fscanf(input, "%c", &c);
			continue;
		}
		int Neg = 0;
		if (c == '-')
		{
			if (ftell(input) == 1) Neg = 1;
			else
			{
				char tmp;
				fseek(input, -1, SEEK_CUR);
				fscanf(input, "%c", &tmp);
				if ((tmp == '(') || (isOp(tmp))) Neg = 1;
			}
		}
		if (isDigit(c) || Neg) fprintf(poland, "%c", c);
		else
		{
			fprintf(poland, " ");
			if (isOp(c))
			{
				while (!((opstack == NULL) || (getPriority(c) > getPriority(peek_char(opstack)))))
					fprintf(poland, "%c ", pop_char(&opstack));

				if ((opstack == NULL) || (getPriority(c) > getPriority(peek_char(opstack))))
					push_char(&opstack, c);
			}
			else if (c == '(')
			{
				push_char(&opstack, c);
			}
			else
			{
				while (peek_char(opstack) != '(')
				{
					fprintf(poland, "%c ", pop_char(&opstack));
				}
				pop_char(&opstack);
			}
		}
		fscanf(input, "%c", &c);
	}
	if (opstack != NULL) fprintf(poland, " ");
	while (opstack != NULL)
	{
		fprintf(poland, "%c ", pop_char(&opstack));
	}
	fclose(poland);

	// ВЫЧИСЛЕНИЕ ОПЗ

	poland = fopen("poland.txt", "r");

	fscanf(poland, "%c", &c);

	while (!feof(poland))
	{
		if (c == ' ')
		{
			fscanf(poland, "%c", &c);
			continue;
		}
		double value = 0;
		double val1, val2;
		int Neg = 0;
		if (ftell(poland) == 1) Neg = 1;
		else
		{
			char tmp;
			fscanf(poland, "%c", &tmp);
			fseek(poland, -1, SEEK_CUR);
			if (isDigit(tmp)) Neg = 1;
		}
		if (isDigit(c) || Neg)
		{
			fseek(poland, -1, SEEK_CUR);
			fscanf(poland, "%lf", &value);
			push(&valstack, value);
		}
		else
		{
			val1 = pop(&valstack);
			val2 = pop(&valstack);
			switch (c)
			{
			case '+': value = val2 + val1; break;
			case '-': value = val2 - val1; break;
			case '*': value = val2 * val1; break;
			case '/': value = val2 / val1; break;
			case '^': value = pow(val2, val1); break;
			}
			push(&valstack, value);
		}
		fscanf(poland, "%c", &c);
	}
	fprintf(output, "%.15lE", pop(&valstack));

	fclose(input);
	fclose(output);
	return 0;
}
