#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <unistd.h>
#include <getopt.h>

#define VERSION "1.2"
#define VERSION_DATE "7.1.2026"
#define ENG_LENGHT  26

bool isNumber(char *str);
void Help(void);
void About(void);

char* ToNum(const char *str);
char* ToAlpha(char* str);

int main(int argc, char **argv) {
    
    int opt;
    bool rawOutput = false;

    static struct option long_options[] = {
        {"help",   no_argument,       0, 'h'}, // --help
        {"info",   no_argument,       0, 'i'}, // --info
        {"raw",    no_argument,       0, 'r'}, // --raw
        {"to-alpha", required_argument, 0, 'a'}, // --to-alpha <number>
        {"to-num", required_argument, 0, 'n'},   // --to-num <ANS string>
        {0, 0, 0, 0} // ukončenie
    };

    if(argc == 1) {
        fprintf(stderr, "Invalid syntax, try -h for help\n");
        return 1;
    }

    int optionIndex = 0;
    while((opt = getopt_long(argc, argv, "hirn:a:", long_options, &optionIndex)) != -1) {
        switch(opt) {
            case 'h':
                Help();
                break;
            case 'i':
                About();
                break;
            case 'r':
                rawOutput = true;
                break;
            case 'n': {
                if(!optarg) {
                    fprintf(stderr, "Error: -n requires a value\n");
                    return 1;
                }

                char *result = ToNum(optarg);
                if(!result) {
                    fprintf(stderr, "Memory allocation failed\n");
                    return 1;
                }

                if(rawOutput)
                    printf("%s", result);
                else
                    printf("-%c \"%s\" -> %s\n", opt, optarg, result);

                free(result);
                break;
            }
            case 'a': {
                if(!optarg) {
                    fprintf(stderr, "Error: -a requires a value\n");
                    return 1;
                }
                
                char *result = ToAlpha(optarg);
                if(!result) {
                    fprintf(stderr, "Memory allocation failed\n");
                    return 1;
                }
                
                if(rawOutput)
                    printf("%s", result);
                else
                    printf("-%c \"%s\" -> %s\n", opt, optarg, result);
                
                free(result);
                break;
            }
        }
    }

    putchar('\n');
    return 0;
}

bool isNumber(char *str) {
    if (*str == '\0') return 0; // empty string is not a number
    while(*str) {
        if (!isdigit(*str))
            return false;
        str++;
    }
    return true;
}

void Help(void) {
    putchar('\n');

    printf("Usage:\n");
    printf("  ans [options]\n");
    
    putchar('\n');

    printf("Options:\n");
    printf("  -h or --help                Show this help message\n");
    printf("  -i or --info                Show detailed program description\n");
    printf("  -r or --raw                 Output result only (raw output)\n");
    printf("  -a <number> or --to-alpha   Convert decimal number to ANS format\n");
    printf("  -n <string> or --to-num     Convert ANS string to decimal number\n");
    
    putchar('\n');
    
    printf("Notes:\n");
    printf("  - ANS strings must contain only alphabetic characters (A-Z)\n");
    printf("  - Decimal input must be non-negative\n");
    printf("  - Use shell redirection (>) to save output to a file\n");
}

void About(void) {
    putchar('\n');

    printf("ANS - Alphabetical Numeric System\n");
    printf("Version %s (%s)\n", VERSION, VERSION_DATE);
    printf("(c) v31ndev 2026\n");

    putchar('\n');

    printf("ANS is a custom positional numeral system based on\n");
    printf("alphabetic characters instead of digits.\n");

    putchar('\n');

    printf("Each letter represents a numeric value (A = 0, B = 1, ..., Z = 25),\n");
    printf("similar to how hexadecimal or other base-N systems work.\n");
    printf("For example:\n");
    printf("  \"-n ANS\" outputs \"356\"\n");
    printf("  \"-a 3.14159\" outputs \"D.UYP\"\n");

    putchar('\n');

    printf("The program allows conversion between:\n");
    printf("  - decimal numbers and ANS strings\n");
    printf("  - ANS strings and decimal numbers\n");

    putchar('\n');

    printf("This project is intended for educational and experimental purposes.\n");
}

char* ToNum(const char *str) {
    int len = strlen(str);
    if(len < 1) return NULL;

    int buffsize = len * 3 + 10;
    char *response = malloc(buffsize);
    long result = 0;
    bool floatMode = false;
    char floatSign = '\0';

    int floatIndex = len;

    for(int i = 0; i < strlen(str); i++) {
        if(str[i] == ',' || str[i] == '.') {
            floatMode = true;
            floatSign = str[i];
            floatIndex = i;
            break;
        }
    }

    // NUMBER BEFORE FLOATING SIGN
    for(int i = 0; i < floatIndex; i++) {
        if(!isalpha((int)str[i])) {
            fprintf(stderr, "Provided argument (\"%s\") is not a proper ANS format.\n", str);
            free(response);
            return NULL;
        }

        int value = toupper(str[i]) - 'A';
        long power = 1;
        for(int j = 0; j < floatIndex - i - 1; j++)
            power *= ENG_LENGHT;

        result += value * power;
    }

    snprintf(response, buffsize, "%ld", result);

    // NUMBER AFTER FLOATING SIGN
    long fractional = 0;
    if(floatMode) {
        for(int i = floatIndex + 1; i < len; i++) {
            if(!isalpha((int)str[i])) {
                fprintf(stderr, "Provided argument (\"%s\") is not in a proper ANS format.\n", str);
                free(response);
                return NULL;
            }

            int value = toupper(str[i]) - 'A';
            long power = 1;
            for(int j = 0; j < len - i - 1; j++)
                power *= ENG_LENGHT;

            fractional += value * power;
        }
        snprintf(response, buffsize, "%ld%c%ld", result, floatSign, fractional);
    }

    return response;
}

char* ToAlpha(char* str) {
    int len = strlen(str);
    if(len < 1) return NULL;

    int buffsize = len * 3 + 10;
    char *result = malloc(buffsize);

    bool fracMode = false;
    char fracSign = '\0';
    int fracIndex = len;

    /* CHECK, IF THE NUMBER HAS A FRACTION */
    for(int i = 0; i < len; i++) {
        if(str[i] == ',' || str[i] == '.') {
            fracMode = true;
            fracSign = str[i];
            fracIndex = i;
        }
    }

    /* INTEGER PART */

    // COPYING INTEGER PART (STR) FROM PARSED ARGUMENT
    char *intNumStr = malloc(fracIndex + 1);
    for(int i = 0; i < fracIndex; i++)
        intNumStr[i] = str[i];
    intNumStr[fracIndex] = '\0';
    
    // CHECK, IF THE INTEGER PART A NUM
    if(!isNumber(intNumStr)) {
        fprintf(stderr, "Integer part (\"%s\") of the provided argument (\"%s\") is not a number.\n", intNumStr, str);
        free(intNumStr);
        return NULL;
    }

    // CONVERT INT PART (STR) TO INT
    int intNum = atoi(intNumStr);
    free(intNumStr);

    int maxLen = (int)(log(intNum) / log(ENG_LENGHT)) + 1;
    char *intResult = malloc(maxLen + 1);
    intResult[maxLen] = '\0';
    int intResIndex = maxLen - 1;

    // MAIN LOGIC
    if(intNum == 0) {
        intResult[0] = 'A';
        intResult[1] = '\0';
    }
    else {
        do {
            long value = intNum % ENG_LENGHT;
            intResult[intResIndex--] = value + 'A';
            intNum /= ENG_LENGHT;
        } while(intNum > 0);
    }

    /* FRACTIONAL PART */
    if(fracMode) {
        // COPYING FRACTIONAL PART (STR) FROM PARSED ARGUMENT
        char *fracNumStr = malloc(len - fracIndex + 1);
        for(int i = 0; i < len - fracIndex; i++)
            fracNumStr[i] = str[i + fracIndex + 1];
        fracNumStr[len - fracIndex] = '\0';

        // CHECK, IF THE FRACTIONAL PART IS A NUM
        if(!isNumber(fracNumStr)) {
            fprintf(stderr, "Fractional part (\"%s\") of the provided argument (\"%s\") is not a number.\n", fracNumStr, str);
            free(fracNumStr);
            return NULL;
        }

        // CONVERT FRAC PART (STR) TO INT
        int fracNum = atoi(fracNumStr);
        free(fracNumStr);

        maxLen = (int)(log(fracNum) / log(ENG_LENGHT)) + 1;
        char *fracResult = malloc(maxLen + 1);
        fracResult[maxLen] = '\0';
        int fracResIndex = maxLen - 1;

        if(fracNum == 0) {
            fracResult[0] = 'A';
            fracResult[1] = '\0';
        }
        else {
            do {
                long value = fracNum % ENG_LENGHT;
                fracResult[fracResIndex--] = value + 'A';
                fracNum /= ENG_LENGHT;
            } while(fracNum > 0);
        }

        snprintf(result, buffsize, "%s%c%s", intResult, fracSign, fracResult);
        free(fracResult);
    }
    
    if(!fracMode)
        snprintf(result, buffsize, "%s", intResult);

    free(intResult);
    return result;
}