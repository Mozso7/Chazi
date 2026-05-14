#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define VERSION "1.1"

const char *c_template =
"#include <stdio.h>\n"
"\n"
"int main() {\n"
"    printf(\"Hello, World!\\n\");\n"
"    return 0;\n"
"}\n";

const char *java_template =
"public class Main {\n"
"    public static void main(String[] args) {\n"
"        System.out.println(\"Hello, World!\");\n"
"    }\n"
"}\n";

const char *python_template =
"print(\"Hello, World!\")\n";

const char *sh_template =
"#!/bin/bash\n"
"\n"
"echo \"Hello, World!\"\n";

void print_help(const char *program_name) {

    printf("Használat:\n");
    printf("  %s c\n", program_name);
    printf("  %s java\n", program_name);
    printf("  %s py\n", program_name);
    printf("  %s sh\n", program_name);

    printf("\n");

    printf("Kapcsolók:\n");
    printf("  -h, --help      Súgó megjelenítése\n");
    printf("  -v, --version   Verziószám megjelenítése\n");
    printf("  --stdout        Nem hoz létre fájlt, csak kiírja a forráskódot\n");
}

int file_exists(const char *filename) {

    FILE *file = fopen(filename, "r");

    if (file != NULL) {
        fclose(file);
        return 1;
    }

    return 0;
}

int write_file(const char *filename, const char *content, int executable) {

    if (file_exists(filename)) {

        fprintf(stderr, "Hiba: A fájl már létezik: %s\n", filename);
        return 1;
    }

    FILE *file = fopen(filename, "w");

    if (file == NULL) {

        fprintf(stderr, "Hiba fájl létrehozásakor: %s\n", strerror(errno));
        return 1;
    }

    fputs(content, file);
    fclose(file);

    if (executable) {

        char command[256];

        snprintf(command, sizeof(command), "chmod +x %s", filename);
        system(command);
    }

    printf("Fájl létrehozva: %s\n", filename);

    return 0;
}

int main(int argc, char *argv[]) {

    // Verzió ha nincs argumentum
    if (argc == 1) {

        printf("Verzió: %s\n", VERSION);
        return 0;
    }

    // Help
    if (strcmp(argv[1], "-h") == 0 ||
        strcmp(argv[1], "--help") == 0) {

        print_help(argv[0]);
        return 0;
    }

    // Verzió kapcsoló
    if (strcmp(argv[1], "-v") == 0 ||
        strcmp(argv[1], "--version") == 0) {

        printf("Verzió: %s\n", VERSION);
        return 0;
    }

    int stdout_mode = 0;

    // Kapcsolók
    for (int i = 1; i < argc; i++) {

        if (strcmp(argv[i], "--stdout") == 0) {
            stdout_mode = 1;
        }
    }

    const char *language = NULL;

    // Nyelv keresése
    for (int i = 1; i < argc; i++) {

        if (argv[i][0] == '-') {
            continue;
        }

        language = argv[i];
        break;
    }

    if (language == NULL) {

        fprintf(stderr, "Hiba: Nincs megadva nyelv.\n");
        return 1;
    }

    const char *filename = NULL;
    const char *content = NULL;
    int executable = 0;

    // C
    if (strcmp(language, "c") == 0) {

        filename = "main.c";
        content = c_template;
    }

    // Java
    else if (strcmp(language, "java") == 0) {

        filename = "Main.java";
        content = java_template;
    }

    // Python
    else if (strcmp(language, "py") == 0 ||
             strcmp(language, "python") == 0) {

        filename = "main.py";
        content = python_template;
    }

    // Bash
    else if (strcmp(language, "sh") == 0) {

        filename = "main.sh";
        content = sh_template;
        executable = 1;
    }

    else {

        fprintf(stderr, "Hiba: Ismeretlen nyelv: %s\n", language);
        return 1;
    }

    // stdout mód
    if (stdout_mode) {

        printf("%s", content);
        return 0;
    }

    // Fájl létrehozása
    return write_file(filename, content, executable);
}