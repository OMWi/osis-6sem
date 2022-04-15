#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define outputFileName "output.txt"
#define inputFileName "input.txt"
#define bannedFileName "banned.txt"

size_t readBannedWords(char*** words) {
    size_t amount = 0;
    FILE* fileIn = fopen(bannedFileName, "r"
    );
    char* line = NULL; size_t len = 0;
    while(getline(&line, &len, fileIn) != -1) {
        amount++;
    }
    *words = malloc(amount * sizeof(char*));
    rewind(fileIn);
    for (size_t index = 0; index < amount; index++) {
        (*words)[index] = NULL;
        getline(&(*words)[index], &len, fileIn);
        (*words)[index][strcspn((*words)[index], "\n")] = 0;
    }
    free(line);
    fclose(fileIn);
    return amount;
}

void filterInput(size_t wordsNumber, char** bannedWords) {
    for (size_t index = 0; index < wordsNumber; index++) {
        printf("%s", bannedWords[index]);
    }

    FILE* inputFile = fopen(inputFileName, "r");
    FILE* outputFile = fopen(outputFileName, "w");
    char* line = NULL; size_t len = 0;
    while(getline(&line, &len, inputFile) != -1) {
        char* word = strtok(line, " ");
        while (word) {
            bool isBanned = false;
            for (size_t index = 0; index < wordsNumber; index++) {
                char* substr = strstr(word, bannedWords[index]);
                if (substr) {
                    isBanned = true;
                    break;
                }                
            }
            char* temp = word;
            word = strtok(NULL, " ");
            if (!isBanned) {
                if (word) {
                    fprintf(outputFile, "%s ", temp);
                }
                else {
                    fprintf(outputFile, "%s", temp);
                }
            }
        }     
    }
    free(line);
    fclose(inputFile);
    fclose(outputFile);
}

int main() {    
    char** bannedWords = NULL;
    size_t wordsNumber = readBannedWords(&bannedWords);
    filterInput(wordsNumber, bannedWords);
}