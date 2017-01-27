#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

typedef struct Stack_double
{
	double data;
	struct Stack_double *next;
} StackOfDouble;

typedef struct Stack_char
{
	char data;
	struct Stack_char *next;
} StackOfChar;

void pushDouble(StackOfDouble **stack, double value)
{
	StackOfDouble *tempStack = malloc(sizeof(StackOfDouble));
	if (tempStack != NULL)
	{
		tempStack -> next = *stack;
		tempStack -> data = value;
		*stack = tempStack;
	}
}

void pushChar(StackOfChar **stack, char value)
{
	StackOfChar *tempStack = malloc(sizeof(StackOfDouble));
	if (tempStack != NULL)
	{
		tempStack -> next = *stack;
		tempStack -> data = value;
		*stack = tempStack;
	}
}

double popDouble(StackOfDouble **stack)
{
	if (*stack != NULL)
	{
		double result;
		StackOfDouble *out = *stack;
		*stack = (*stack) -> next;
		result = out -> data;
		free(out);
		return result;
	}
	return 0;
}

char popChar(StackOfChar **stack)
{
	if (*stack != NULL)
	{
		char result;
		StackOfChar *out = *stack;
		*stack = (*stack) -> next;
		result = out -> data;
		free(out);
		return result;
	}
	return 0;
}

double peekDouble(const StackOfDouble *stack)
{
	if (stack != NULL) return stack -> data;
	return 0;
}

int peekChar(const StackOfChar *stack)
{
	if (stack != NULL) return stack -> data;
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

int isOperation(char op)
{
	if ((op == '+') || (op == '-') || (op == '/') || (op == '*') || (op == '^')) return 1;
	return 0;
}

int getPriority(char op)
{
	switch (op)
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
	StackOfChar *stackOfOperations = NULL;
	StackOfDouble *stackOfValues = NULL;
	char symbol;

	// ПЕРЕВОД В ОПЗ

	fscanf(input, "%c", &symbol);
	while (!feof(input))
	{
		if (symbol == '\n') break;
		if (symbol == ' ')
		{
			fscanf(input, "%c", &symbol);
			continue;
		}
		int isUnaryMinus = 0;
		if (symbol == '-')
		{
			if (ftell(input) == 1) isUnaryMinus = 1;
			else
			{
				char tmp;
				fseek(input, -1, SEEK_CUR);
				fscanf(input, "%c", &tmp);
				if ((tmp == '(') || (isOperation(tmp))) isUnaryMinus = 1;
			}
		}
		if (isDigit(symbol) || isUnaryMinus) fprintf(poland, "%c", symbol);
		else
		{
			fprintf(poland, " ");
			if (isOperation(symbol))
			{
				while (!((stackOfOperations == NULL) || (getPriority(symbol) > getPriority(peekChar(stackOfOperations)))))
					fprintf(poland, "%c ", popChar(&stackOfOperations));

				if ((stackOfOperations == NULL) || (getPriority(symbol) > getPriority(peekChar(stackOfOperations))))
					pushChar(&stackOfOperations, symbol);
			}
			else if (symbol == '(')
			{
				pushChar(&stackOfOperations, symbol);
			}
			else
			{
				while (peekChar(stackOfOperations) != '(')
				{
					fprintf(poland, "%c ", popChar(&stackOfOperations));
				}
				popChar(&stackOfOperations);
			}
		}
		fscanf(input, "%c", &symbol);
	}
	if (stackOfOperations != NULL) fprintf(poland, " ");
	while (stackOfOperations != NULL)
	{
		fprintf(poland, "%c ", popChar(&stackOfOperations));
	}
	fclose(poland);

	// ВЫЧИСЛЕНИЕ ОПЗ

	poland = fopen("poland.txt", "r");

	fscanf(poland, "%c", &symbol);

	while (!feof(poland))
	{
		if (symbol == ' ')
		{
			fscanf(poland, "%c", &symbol);
			continue;
		}
		double value = 0;
		double val1, val2;
		int isUnaryMinus = 0;
		if (ftell(poland) == 1) isUnaryMinus = 1;
		else
		{
			char tmp;
			fscanf(poland, "%c", &tmp);
			fseek(poland, -1, SEEK_CUR);
			if (isDigit(tmp)) isUnaryMinus = 1;
		}
		if (isDigit(symbol) || isUnaryMinus)
		{
			fseek(poland, -1, SEEK_CUR);
			fscanf(poland, "%lf", &value);
			pushDouble(&stackOfValues, value);
		}
		else
		{
			val1 = popDouble(&stackOfValues);
			val2 = popDouble(&stackOfValues);
			switch (symbol)
			{
			case '+': value = val2 + val1; break;
			case '-': value = val2 - val1; break;
			case '*': value = val2 * val1; break;
			case '/': value = val2 / val1; break;
			case '^': value = pow(val2, val1); break;
			}
			pushDouble(&stackOfValues, value);
		}
		fscanf(poland, "%c", &symbol);
	}
	fprintf(output, "%.15lE", popDouble(&stackOfValues));

	fclose(input);
	fclose(output);
	return 0;
}
