#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 100
#define MAX_PROBE 10

typedef struct node {
    char* key;
    int value;
} node;

node* hashtable[TABLE_SIZE];

// hash function using quadratic probing
int hash(char* key, int i) {
    int hashval = 0;
    for (int j = 0; j < strlen(key); j++) {
        hashval += key[j];
    }
    return (hashval + i*i) % TABLE_SIZE;
}

// insert a key-value pair into the hash table
void insert(char* key, int value) {
    int i = 0;
    int index = hash(key, i);
    while (hashtable[index] != NULL) {
        i++;
        if (i > MAX_PROBE) {
            printf("Error: Maximum probing limit reached\n");
            return;
        }
        index = hash(key, i);
    }
    node* new = (node*) malloc(sizeof(node));
    new->key = key;
    new->value = value;
    hashtable[index] = new;
}

// get the value associated with the given key from the hash table
int get(char* key) {
    int i = 0;
    int index = hash(key, i);
    while (hashtable[index] != NULL) {
        if (strcmp(hashtable[index]->key, key) == 0) {
            return hashtable[index]->value;
        }
        i++;
        if (i > MAX_PROBE) {
            printf("Error: Maximum probing limit reached\n");
            return 0;
        }
        index = hash(key, i);
    }
    return 0;
}

// print the hash table
void print_table() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        if (hashtable[i] == NULL) {
            printf("%d: NULL\n", i);
        } else {
            printf("%d: %s=%d\n", i, hashtable[i]->key, hashtable[i]->value);
        }
    }
}

int main() {
    insert("apple", 3);
    insert("banana", 5);
    insert("cherry", 2);
    insert("date", 1);
    insert("orange",1);
    printf("apple=%d\n", get("apple"));
    printf("orange=%d\n",get("ordfcdrange"));
    printf("banana=%d\n", get("banana"));
    printf("cherry=%d\n", get("cherry"));
    printf("date=%d\n", get("date"));
    printf("grape=%d\n", get("grape"));
    // print_table();
    return 0;
}
