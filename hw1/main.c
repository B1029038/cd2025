#include <stdio.h>
#include <string.h>

int is_letter(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

int is_digit(char c) {
    return c >= '0' && c <= '9';
}

int is_valid_identifier_char(char c) {
    return is_letter(c) || is_digit(c) || c == '_';
}

void print_token(const char* value, const char* token_type) {
    printf("%s: %s\n", value, token_type);
}

const char* match_keyword(const char* word) {
    if (strcmp(word, "int") == 0) return "TYPE_TOKEN";
    if (strcmp(word, "main") == 0) return "MAIN_TOKEN";
    if (strcmp(word, "if") == 0) return "IF_TOKEN";
    if (strcmp(word, "else") == 0) return "ELSE_TOKEN";
    if (strcmp(word, "while") == 0) return "WHILE_TOKEN";
    return NULL;
}

void scan_identifier(FILE *fptr, char first) {
    char buffer[256];
    int i = 0;
    buffer[i++] = first;

    char c;
    while ((c = fgetc(fptr)) != EOF && is_valid_identifier_char(c)) {
        buffer[i++] = c;
    }
    buffer[i] = '\0';
    if (c != EOF) ungetc(c, fptr);

    const char* token = match_keyword(buffer);
    if (token) print_token(buffer, token);
    else print_token(buffer, "ID_TOKEN");
}

void scan_number(FILE *fptr, char first) {
    char buffer[256];
    int i = 0;
    buffer[i++] = first;

    char c;
    while ((c = fgetc(fptr)) != EOF && is_digit(c)) {
        buffer[i] = c;
        i++;
    }
    buffer[i] = '\0';
    if (c != EOF) ungetc(c, fptr);

    print_token(buffer, "LITERAL_TOKEN");
}

void scan_operator(FILE *fptr, char first) {
    char second = fgetc(fptr);
    char buffer[3] = {first, '\0', '\0'};

    if ((first == '=' && second == '=') || 
        (first == '<' && second == '=') || 
        (first == '>' && second == '=')) {
        buffer[1] = second;
    } else {
        if (second != EOF) ungetc(second, fptr);
    }

    if (strcmp(buffer, "==") == 0) print_token(buffer, "EQUAL_TOKEN");
    else if (strcmp(buffer, ">=") == 0) print_token(buffer, "GREATEREQUAL_TOKEN");
    else if (strcmp(buffer, "<=") == 0) print_token(buffer, "LESSEQUAL_TOKEN");
    else if (strcmp(buffer, "=") == 0) print_token(buffer, "ASSIGN_TOKEN");
    else if (strcmp(buffer, ">") == 0) print_token(buffer, "GREATER_TOKEN");
    else if (strcmp(buffer, "<") == 0) print_token(buffer, "LESS_TOKEN");
    else if (strcmp(buffer, "+") == 0) print_token(buffer, "PLUS_TOKEN");
    else if (strcmp(buffer, "-") == 0) print_token(buffer, "MINUS_TOKEN");
    else if (strcmp(buffer, "(") == 0) print_token(buffer, "LEFTPAREN_TOKEN");
    else if (strcmp(buffer, ")") == 0) print_token(buffer, "REFTPAREN_TOKEN");
    else if (strcmp(buffer, "{") == 0) print_token(buffer, "LEFTBRACE_TOKEN");
    else if (strcmp(buffer, "}") == 0) print_token(buffer, "REFTBRACE_TOKEN");
    else if (strcmp(buffer, ";") == 0) print_token(buffer, "SEMICOLON_TOKEN");
}

int main() {
    FILE *fptr = fopen("test.c", "r");
    if (!fptr) {
        printf("Could not open file.\n");
        return 1;
    }

    char c;
    while ((c = fgetc(fptr)) != EOF) {
        if (c == ' ' || c == '\n' || c == '\t') {
            continue;
        } else if (is_letter(c) || c == '_') {
            scan_identifier(fptr, c);
        } else if (is_digit(c)) {
            scan_number(fptr, c);
        } else {
            scan_operator(fptr, c);
        }
    }

    fclose(fptr);
    return 0;
}
