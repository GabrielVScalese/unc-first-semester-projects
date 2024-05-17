#include <stdio.h>
#include <string.h>

#define TEXT_INPUT_LENGTH 1000
#define NUMBER_LINES 1000
#define WORDS_NUMBER 1000

void resetString (char oneString[])
{
    for (int i = 0; oneString[i] != '\0'; i++)
        oneString[i] = '\0';
}

void setWordList (char wordList[WORDS_NUMBER][TEXT_INPUT_LENGTH], char textInput[])
{
    char partialWord[TEXT_INPUT_LENGTH];
    int partialWordCount = 0;

    int wordlistCount = 0;
    for (int i = 0; ; i++)
    {
        if (textInput[i] == ' ' || textInput[i] == '\n')
        {
            strcpy(wordList[wordlistCount++], partialWord);
            resetString(partialWord);
            partialWordCount = 0;

            if (textInput[i] == '\n')
                break;
        }
        else
            partialWord[partialWordCount++] = textInput[i];
    }
}

int main ()
{
    int paragraphQnt;
    scanf("%d\n", &paragraphQnt);

    char text[NUMBER_LINES][TEXT_INPUT_LENGTH];

    // Leitura do texto
    for (int i = 0; i < paragraphQnt; i++)
    {
        char textInput[TEXT_INPUT_LENGTH];
        fgets(textInput, sizeof(textInput), stdin);
        strcpy(text[i], textInput);
    }

    // Leitura das palavras positivas e negativas
    char textInput[TEXT_INPUT_LENGTH];
    fgets(textInput, sizeof(textInput), stdin);

    char positiveWordList[WORDS_NUMBER][TEXT_INPUT_LENGTH];
    setWordList(positiveWordList, textInput);

    fgets(textInput, sizeof(textInput), stdin);
    char negativeWordList[WORDS_NUMBER][TEXT_INPUT_LENGTH];
    setWordList(negativeWordList, textInput);

    return 0;
}