#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <ctype.h>

#define TEXT_INPUT_LENGTH 1000
#define WORDS_NUMBER 1000

typedef struct Sentence  {
        int initialLineIndex;
        int finalLineIndex;
        int initialColIndex;
        int finalColIndex;
        int lettersNumber;
} Sentence;

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

Sentence getLongestSentence (char text[paragraphQnt][TEXT_INPUT_LENGTH])
{
    Sentence oneSentence;
    Sentence longestSentence;

    oneSentence.lettersNumber = 0;
    longestSentence.lettersNumber = 0;
    for (int i = 0; i < paragraphQnt; i++)
    {
        oneSentence.initialLineIndex = i;
        oneSentence.initialColIndex = 0;
        for (int j = 0; text[i][j] != '\n';j++)
        {
            if (isalpha(text[i][j]))
                oneSentence.lettersNumber++;
            
            if (text[i][j] == '.')
            {
                if (oneSentence.lettersNumber > longestSentence.lettersNumber)
                {
                    longestSentence.initialLineIndex = oneSentence.initialLineIndex;
                    longestSentence.finalLineIndex = i;
                    longestSentence.initialColIndex = oneSentence.initialColIndex;
                    longestSentence.finalColIndex = j;
                    longestSentence.lettersNumber = oneSentence.lettersNumber;
                }

                oneSentence.lettersNumber = 0;
                oneSentence.initialColIndex = j;
            }
        }
    }

    return longestSentence;
}

int getPolarity (char text[paragraphQnt][TEXT_INPUT_LENGTH], int initialLineIndex, int initialColIndex, int finalLineIndex, int finalColIndex)
{
    char partialWord[TEXT_INPUT_LENGTH];
    int positiveWordCount = 0;
    int negativeWordCount = 0;

    int i = initialLineIndex;
    int j = initialColIndex;
  
    for (; i <= finalLineIndex;)
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
        
        if (text[i][j] == '\n')
        {
            i++;
            j = 0;
        }

        if (i == finalLineIndex && j == finalColIndex)
            break;

        j++;
    }
        
    if ((positiveWordCount - negativeWordCount) > 3)
        return 1;
    
    if (positiveWordCount > 0 && negativeWordCount == 0)
        return 1;
    
    if ((negativeWordCount - positiveWordCount) > 3)
        return -1;
    
    if (positiveWordCount == 0 && negativeWordCount > 0)
        return -1;
    
    if (positiveWordCount > 0 && negativeWordCount > 0 && abs(positiveWordCount - negativeWordCount) <= 3)
        return -1;
}



// Falta observacao sobre sinal a mais sobre palavra do dicionario
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

    // getTextPolarity(text, 1, 1);
    // getLongestSentence(text);
    //Sentence longestSentence = getLongestSentence(text);
    //getPolarity(text, longestSentence.initialLineIndex, longestSentence.initialColIndex, longestSentence.finalLineIndex, longestSentence.finalColIndex);

    getPolarity(text, 0, 0, 0, TEXT_INPUT_LENGTH);

    return 0;
}