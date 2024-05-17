#include <stdio.h>
#include <string.h>
#include <strings.h>

#define TEXT_INPUT_LENGTH 1000
#define WORDS_NUMBER 1000

int paragraphQnt;
char positiveWordList[WORDS_NUMBER][TEXT_INPUT_LENGTH];
char negativeWordList[WORDS_NUMBER][TEXT_INPUT_LENGTH];

void resetString (char oneString[])
{
    for (int i = 0; oneString[i] != '\0'; i++)
        oneString[i] = '\0';
}

void setWordList (char wordList[WORDS_NUMBER][TEXT_INPUT_LENGTH], char textInput[])
{
    char partialWord[TEXT_INPUT_LENGTH];

    int wordlistCount = 0;
    for (int i = 0; ; i++)
    {
        if (textInput[i] == ' ' || textInput[i] == '\n')
        {
            strcpy(wordList[wordlistCount++], partialWord);
            resetString(partialWord);

            if (textInput[i] == '\n')
                break;
        }
        else
            strncat(partialWord, &textInput[i], 1);
    }
}

int listContainsWord(char list[WORDS_NUMBER][TEXT_INPUT_LENGTH], char word[])
{
    for (int i = 0; list[i][0] != '\0'; i++)
        if (strcasecmp(list[i], word) == 0)
            return 1;
    
    return 0;
}

int getTextPolarity (char text[paragraphQnt][TEXT_INPUT_LENGTH])
{
    char partialWord[TEXT_INPUT_LENGTH];
    int positiveWordCount = 0;
    int negativeWordCount = 0;

    for (int i = 0; i < paragraphQnt; i++)
        for (int j = 0; text[i][j] != '\n'; j++)
        {
            if (text[i][j] == ' ' || text[i][j] == '.' || text[i][j] == ',')
            {
                if (listContainsWord(positiveWordList, partialWord))
                    positiveWordCount++;
                
                if (listContainsWord(negativeWordList, partialWord))
                    negativeWordCount++;
                
                resetString(partialWord);
            }
            else
                strncat(partialWord, &text[i][j], 1);
        }
    
    return 0;
}

int main ()
{
    scanf("%d\n", &paragraphQnt);
    char text[paragraphQnt][TEXT_INPUT_LENGTH];

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
    setWordList(positiveWordList, textInput);

    fgets(textInput, sizeof(textInput), stdin);
    setWordList(negativeWordList, textInput);


    getTextPolarity(text);

    return 0;
}