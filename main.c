#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* product_list[30];
int price_list[30];
unsigned max_name_length = 0;

void Menu();
void AddProduct();
void Display();
void Delete();
void Search();
char *ReadLine(size_t, FILE*);
int first_available(int);
int first_occupied(int);
int amount();
char *to_lower(char*);
char *String(char*, int);

int main(int argc, char** argv) {
    int i;
    FILE *ffile;
    char *line = NULL;
    size_t len = 15;
    ssize_t read;
    char *token;
    int list_index;
    char *spl[3];
    int count = 0;
    int pindex;
    char *name;
    // product_list = (char **)malloc(30 * sizeof(char *)); // Unfortunately this is too complicated for the implementation of this program
    for (i = 0; i < 30; i++)
        product_list[i] = NULL;
    ffile = fopen("list.txt", "r");
    if (ffile != NULL) {
        //read = getline(&line, &len, ffile);
        read = ReadLine(len, ffile);
        while (read != -1) {
            if (product_list[1] == NULL)
                printf("NULL\n");
            else
                printf("%s\n", product_list[1]);
            //if (line [ strlen(line) - 1 ] == '\n')
            //  line [ strlen(line) - 1 ] = '\0';
            //token = strtok(line, "|");
            token = String(line, 0);
            while (token != NULL) {
                spl[count++] = token;
                //token = strtok(NULL, "|");
                token = String(line, 1);
            }
            if (count == 3) {
                count = 0;
                pindex = atoi(spl[0]) - 1;
                name = spl[1];
                if ((unsigned) strlen(name) > max_name_length)
                    max_name_length = (unsigned) strlen(name);
                price_list[pindex] = atoi(spl[2]);
                product_list[pindex] = name;
            }
            if (product_list[1] == NULL)
                printf("NULL\n");
            else
                printf("%s\n", product_list[1]);
            //read = getline(&line, &len, ffile);
            read = ReadLine(len, ffile);
            printf("done read");
            if (product_list[1] == NULL)
                printf("NULL\n");
            else
                printf("%s\n", product_list[1]);
        }
    }
    fclose(ffile);

    Menu();
    return (EXIT_SUCCESS);
}

char *String(char *str, int did) {
    char *token;
    if (did == 1) {
        token = strtok(NULL, "|");
    } else {
        did = 1;
        token = strtok(str, "|");
    }
    return token;
}

/////////////////////////////////////////////////////////////////////////////////////
// Below are unnecessary for bug fixing process and just for testing purposes only //
/////////////////////////////////////////////////////////////////////////////////////

void Menu() {
    char *input;
    // Add full menu
    printf("=====================================\n");
    //printf("Welcome to TTHN_5C13_ManageProduct.\n");
    //printf("Enter a command in the menu to start.\n");
    //printf("add - Add a product.");
    //printf("display - Show all products");
    //printf("delete - Delete a product.");
    //printf("=====================================\n");

    //while(getchar()!='\n');
    input = ReadLine(100, stdin);
    if (strcmp(input, "add") == 0) {
        free(input);
        AddProduct();
    } else if (strcmp(input, "display") == 0) {
        free(input);
        Display();
    } else if (strcmp(input, "delete") == 0) {
        free(input);
        Delete();
    } else if (strcmp(input, "search") == 0) {
        free(input);
        Search();
    }
}

void AddProduct() {
    char *name;
    int price, slot;
    slot = first_available(0);
    if (slot == -1) {
        printf("There is no available slot left. Use command 'delete' to delete some slots first.\n");
        Menu();
        return;
    }
    printf("Enter the product name:\n");
    name = ReadLine(255, stdin);
    if ((unsigned) strlen(name) > max_name_length)
        max_name_length = (unsigned) strlen(name);
    printf("Enter product's price in VND:\n");
    scanf("%d", &price);
    while (getchar() != '\n');
    if (price < 0) {
        printf("Product price can not be a negative number.");
        free(name);
        Menu();
        return;
    }
    cell_edit(slot, name, price);
    printf("Product added.\n");
    Menu();
}

void Display() {
    int i, amt;
    unsigned j;
    if ((amt = amount()) == 0) {
        printf("There is no product currently.\n");
        Menu();
        return;
    }

    printf("ID   NAME");
    for (i = 1; i < max_name_length; i++)
        printf(" ");
    printf("PRICE\n");

    for (i = 0; i < 30; i++) {
        if (product_list[i] == NULL)
            continue;
        if (i + 1 < 10) printf("0");
        printf("%d   ", i + 1); // Printing product ID (= array index + 1)
        printf("%s   ", product_list[i]);
        for (j = 1; j <= max_name_length - (unsigned) strlen(product_list[i]); j++)
            printf(" ");
        printf("%d", price_list[i]);
        printf("\n");
    }
    Menu();
}

void Delete() {
    int id, j;
    int gap, occupied, num_gaps;
    char opt;
    printf("Please enter product ID to perform deletion:\n");
    scanf("%d", &id);
    while (getchar() != '\n');
    if (product_list[--id] == NULL)
        printf("No product found. Double check your product ID.\n");
    else {
        product_list[id] = NULL;
        printf("Product deleted.\nDo you want to update product IDs (will remove gaps between IDs): (y/n)\n");
        if ((opt = getchar()) == 'y') {
            printf("All product IDs updated.\n");
            while (first_occupied(first_available(0) - 1) != -1) {
                gap = first_available(0) - 1;
                occupied = first_occupied(gap) - 1;
                num_gaps = occupied - gap;
                memmove(product_list + gap, product_list + occupied, 30 - occupied);
                memmove(price_list + gap, price_list + occupied, 30 - occupied);
                for (j = 30 - num_gaps; j < 30; j++)
                    product_list[j] = NULL;
            }
        } else {
            printf("No update made.\n");
        }
        while (getchar() != '\n');
    }
    Menu();
}

void Search() {
    char *key;
    int found = 0;
    int i, j, k;
    key = malloc(51 * sizeof (int));
    printf("Enter search keyword:\n");
    fgets(key, 50, stdin);
    if (key[strlen(key) - 1] == '\n')
        key[strlen(key) - 1] = '\0';
    for (i = 0; i < 30; i++) {
        if (product_list[i] == NULL)
            continue;
        if (strstr(product_list[i], key) != NULL) {
            if (found == 0) {
                found = 1;
                printf("Match found.\n");
                printf("ID   NAME");
                for (k = 1; k < max_name_length; k++)
                    printf(" ");
                printf("PRICE\n");
            }
            if (i + 1 < 10) printf("0");
            printf("%d   ", i + 1); // Printing product ID (= array index + 1)
            printf("%s   ", product_list[i]);
            for (j = 1; j <= max_name_length - (unsigned) strlen(product_list[i]); j++)
                printf(" ");
            printf("%d", price_list[i]);
            printf("\n");
        }
    }
    if (found == 0)
        printf("No match found.\n");
    Menu();
}

/*********************/
/* EXTERNAL FUNCTION */
/*********************/

// Resemblance of method Console.ReadLine() from C#
// USAGE: This function can virtually read everything from user input, no matter the length of it
char *ReadLine(size_t size, FILE *file) {
    char *str;
    int ch;
    size_t len = 0;
    str = (char*) malloc(sizeof (char)*size);
    if (str == NULL) {
        printf("\nUnable to allocate memory.\n");
        free(str);
        return NULL;
    }
    while ((ch = fgetc(file)) != '\n') {
        str[len++] = ch;
        if (len == size) {
            str = (char*) realloc(str, sizeof (char)*(size += 16));
            if (str == NULL) {
                printf("\nUnable to allocate memory.\n");
                free(str);
                return NULL;
            }
        }
    }
    str[len++] = '\0';

    return (char*) realloc(str, sizeof (char)*len);
}

int first_available(int count) { // Start from 0
    //while (cell_match(count++, NULL) != -1 && count!=30);
    //return count; // id

    while (count <= 29 && product_list[count++] != NULL) {
        if (count == 30)
            return -1;
    }
    return count;
}

int first_occupied(int count) {
    //while(cell_match(count++, NULL)==-1 && count!=30);
    //return count;

    while (count <= 29 && product_list[count++] == NULL) {
        if (count == 30)
            return -1; // Means that there is no occupied slot left
    }
    return count;
}

int amount() {
    int i, count = 0;
    for (i = 0; i < 30; i++)
        if (product_list[i] != NULL)
            count++;
    return count;
}

// Doesn't work
char *to_lower(char *str) {
    int i;
    unsigned length = strlen(str);
    char *result = (char*) malloc(length * sizeof(char));
    for (i = 0; i<length; i++)
        result[i] = tolower(str[i]);
    return result;
}
