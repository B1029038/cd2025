#include <stdio.h>
#include <stdlib.h>

typedef struct node {
    char word;
    int num;
    struct node* next;
} Node;

int main() {
    FILE *fptr = fopen("main.c", "r");
    if (fptr == NULL) {
        printf("Could not open file \n");
        return 1;
    } else {
        printf("File opened successfully\n");
    }

    Node* head = NULL;
    Node* current = NULL;

    char c;
    while ((c = fgetc(fptr)) != EOF) {
        int found = 0;
        current = head;
        while (current != NULL) {
            if (current->word == c) {
                current->num++;
                found = 1;
                break;
            }
            current = current->next;
        }

        if (!found) {
            Node* newNode = (Node*)malloc(sizeof(Node));
            newNode->word = c;
            newNode->num = 1;
            newNode->next = NULL;

            if (head == NULL) {
                head = newNode;
            } else {
                current = head;
                while (current->next != NULL) {
                    current = current->next;
                }
                current->next = newNode;
            }
        }
    }

    fclose(fptr);

    current = head;
    while (current != NULL) {
        printf("Character: '%c', Count: %d\n", current->word, current->num);
        current = current->next;
    }

    current = head;
    while (current != NULL) {
        Node* temp = current;
        current = current->next;
        free(temp);
    }

    return 0;
}