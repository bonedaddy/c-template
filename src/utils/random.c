#include <stdlib.h>
#include <assert.h>
#include "../../include/utils/random.h"
#include "../../include/utils/errors.h"
#include "../../include/utils/returns.h"

cg_return *get_random_string(int stringLength) {
    // allocate a chunk of memory sized to stringLength
    // not sure why its showing an error in vscode???
    // https://stackoverflow.com/questions/50557000/complie-error-in-c-program-visual-studio-regarding-malloc-a-value-of-type-vo?rq=1
    char *retWord = malloc(stringLength);
    if (retWord == NULL) {
        return new_cg_return(NULL, new_cg_error("failed to malloc memory"));
    }
    // assign each char from word to its corresponding vlaue in retWord
    for (int i = 0; i < stringLength; i++) {
        retWord[i] = letters[get_random_number(0, 25)];
    }
    return new_cg_return(retWord, NULL);
}

int get_random_number(int lower, int upper) {
    return (rand() % (upper - lower + 1)) + lower;
}