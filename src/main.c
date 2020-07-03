#include <argtable3.h>


struct arg_lit *help, *version;

void setup_args();

void setup_args(const char *version_string) {
    help = arg_litn(NULL, "help", 0, 1, "displays help menu");
    version = arg_litn(NULL, "version", 0, 1, version_string);
}

int main(void) {
    setup_args("0.0.1");
    printf("%i\n", help->count);
    printf("%s\n", help->hdr.glossary);
    printf("%s\n", version->hdr.glossary);
}