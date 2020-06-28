#include <stdlib.h>
#include <assert.h>
#include "../../include/utils/random.h"

char *get_random_string(int stringLength) {
    // create an array of chars of length stringLength
    char word[stringLength];
    for (int i = 0; i < stringLength; i++) {
        word[i] = letters[get_random_number(0, 25)];
    }
    // allocate a chunk of memory sized to the word array
    // not sure why its showing an error in vscode???
    // https://stackoverflow.com/questions/50557000/complie-error-in-c-program-visual-studio-regarding-malloc-a-value-of-type-vo?rq=1
    char *retWord = malloc(sizeof(word));
    if (retWord == NULL) {
        return NULL;
    }
    // assign each char from word to its corresponding vlaue in retWord
    for (int i = 0; i < stringLength; i++) {
        retWord[i] = word[i];
    }
    // return a pointer to retWord
    // i dont entirely understand why compiler complains
    // if this is left out
    return (char *)retWord;
}

int get_random_number(int lower, int upper) {
    return (rand() % (upper - lower + 1)) + lower;
}