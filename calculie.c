#include <stdio.h>
#include <string.h>
#include <ctype.h>

float values[100];
char ops[100];
int valTop, opTop;

// -------- LOGIN ----------
int login()
{
    char u[20], p[20];
    printf("Username: ");
    scanf("%s", u);
    printf("Password: ");
    scanf("%s", p);
    return strcmp(u, "admin") == 0 && strcmp(p, "calc123") == 0;
}

// -------- STACK LOGIC ----------
void reset()
{
    valTop = -1;
    opTop = -1;
}
void pushVal(float v) { values[++valTop] = v; }
float popVal() { return values[valTop--]; }
void pushOp(char o) { ops[++opTop] = o; }
char popOp() { return ops[opTop--]; }

int prec(char o)
{
    if (o == '+' || o == '-')
        return 1;
    if (o == '*' || o == '/')
        return 2;
    return 0;
}

float apply(float a, float b, char o)
{
    if (o == '+')
        return a + b;
    if (o == '-')
        return a - b;
    if (o == '*')
        return a * b;
    if (o == '/')
        return b != 0 ? a / b : 0;
    return 0;
}

float eval(char exp[])
{
    reset();
    for (int i = 0; exp[i]; i++)
    {
        if (isdigit(exp[i]))
        {
            float n = 0;
            while (isdigit(exp[i]))
            {
                n = n * 10 + (exp[i] - '0');
                i++;
            }
            pushVal(n);
            i--;
        }
        else if (exp[i] == '(')
            pushOp(exp[i]);
        else if (exp[i] == ')')
        {
            while (ops[opTop] != '(')
            {
                float b = popVal(), a = popVal();
                pushVal(apply(a, b, popOp()));
            }
            popOp();
        }
        else
        {
            while (opTop >= 0 && prec(ops[opTop]) >= prec(exp[i]))
            {
                float b = popVal(), a = popVal();
                pushVal(apply(a, b, popOp()));
            }
            pushOp(exp[i]);
        }
    }
    while (opTop >= 0)
    {
        float b = popVal(), a = popVal();
        pushVal(apply(a, b, popOp()));
    }
    return popVal();
}

int main()
{
    char exp[100];
    FILE *fp;

    if (!login())
    {
        printf("Access Denied\n");
        return 0;
    }

    printf("Enter expression: ");
    scanf("%s", exp);

    float result = eval(exp);
    printf("Result = %.2f\n", result);

    fp = fopen("history.txt", "a");
    fprintf(fp, "%s = %.2f\n", exp, result);
    fclose(fp);

    return 0;
}
