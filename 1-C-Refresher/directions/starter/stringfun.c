#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>


#define BUFFER_SZ 50

//prototypes
void usage(char *);
void print_buff(char *, int);
int  setup_buff(char *, char *, int);

//prototypes for functions to handle required functionality
int  count_words(char *, int, int);
//add additional prototypes here


int setup_buff(char *buff, char *user_str, int len) {
    int i = 0, j = 0; // i tracks input string, j tracks buffer
    bool in_whitespace = true; // Flag to track consecutive spaces

    // Process the input string
    while (*(user_str + i) != '\0') {
        if (j >= len) return -1; // Buffer overflow error

        char current = *(user_str + i);

        if (current == ' ' || current == '\t') {
            if (!in_whitespace) { // Add only one space
                *(buff + j++) = ' ';
                in_whitespace = true;
            }
        } else {
            *(buff + j++) = current; // Copy non-space character
            in_whitespace = false; // Reset whitespace flag
        }
        i++;
    }

    // Remove trailing space, if any
    if (j > 0 && *(buff + j - 1) == ' ') {
        j--;
    }

    // Fill the remaining buffer with dots
    while (j < len) {
        *(buff + j++) = '.';
    }

    return i; // Return length of the cleaned input
}

 


void print_buff(char *buff, int len){
    printf("Buffer:  [");
    for (int i=0; i<len; i++){
        putchar(*(buff+i));
    }
    printf("]\n");
}

void usage(char *exename){
    printf("usage: %s [-h|c|r|w|x] \"string\" [other args]\n", exename);

}

int count_words(char *buff, int len, int str_len) {
    int word_count = 0;      // Initialize the word count
    bool in_word = false;    // to track if we are inside a word
    char *ptr = buff;        // Pointer to traverse the buffer

    for (int i = 0; i < str_len; i++) { // Traverse only up to the length of the user string
        if (*ptr != ' ') {              // Non-space character
            if (!in_word) {             // Start of a new word
                word_count++;           // Increment word count
                in_word = true;            // Set in_word flag
            }
        } else {                        // Space character
            in_word = false;                // Mark end of the current word
        }
        ptr++;                          // Move to the next character
    }

    return word_count;                  // Return total word count
}


//ADD OTHER HELPER FUNCTIONS HERE FOR OTHER REQUIRED PROGRAM OPTIONS

int search_and_replace(char *buff, int buff_len, int str_len, char *search, char *replace) {
    char *search_start = buff;      // Pointer to the current search position in the buffer
    int search_len = 0, replace_len = 0;

    // Calculate the lengths of the search and replace strings 
    while (*(search + search_len) != '\0') search_len++;
    while (*(replace + replace_len) != '\0') replace_len++;

    // Traverse the buffer to find the search string
    while (search_start <= buff + str_len - search_len) {  // Ensure space for the search string
        if (memcmp(search_start, search, search_len) == 0) {  // Match found
            // Check if replacing would cause buffer overflow
            if (str_len - search_len + replace_len > buff_len) {
                return -1;  // Buffer overflow error
            }

            // Shift characters to the right if the replace string is longer
            if (replace_len > search_len) {
                for (char *shift = buff + str_len; shift >= search_start + search_len; shift--) {
                    *(shift + (replace_len - search_len)) = *shift;
                }
            }

            // Replace the search string with the replace string
            for (int i = 0; i < replace_len; i++) {
                *(search_start + i) = *(replace + i);
            }

            // Adjust string length
            str_len = str_len - search_len + replace_len;

            // Fill remaining buffer with dots
            for (int i = str_len; i < buff_len; i++) {
                *(buff + i) = '.';
            }

            return 1;  // Replacement successful
        }

        search_start++;  // Move to the next character in the buffer
    }


    return 0;  // Search string not found
}


int main(int argc, char *argv[]){

    char *buff;             //placehoder for the internal buffer
    char *input_string;     //holds the string provided by the user on cmd line
    char opt;               //used to capture user option from cmd line
    int  rc;                //used for return codes
    int  user_str_len;      //length of user supplied string

    //TODO:  #1. WHY IS THIS SAFE, aka what if arv[1] does not exist?
    // To ensure there is at least one argument provided (besides the program name) and the first argument is a valid option flag starting with -
    if ((argc < 2) || (*argv[1] != '-')){
        usage(argv[0]);
        exit(1);
    }

    opt = (char)*(argv[1]+1);   //get the option flag

    //handle the help flag and then exit normally
    if (opt == 'h'){
        usage(argv[0]);
        exit(0);
    }

    //WE NOW WILL HANDLE THE REQUIRED OPERATIONS

    //TODO:  #2 Document the purpose of the if statement below
    // To ensure that the required input string is provided. 
    // If only the flag is supplied without the corresponding string, the program displays the usage message and exits.
    if (argc < 3){
        usage(argv[0]);
        exit(1);
    }

    input_string = argv[2]; //capture the user input string

    //TODO:  #3 Allocate space for the buffer using malloc and
    //          handle error if malloc fails by exiting with a 
    //          return code of 99
    buff = (char *)malloc(BUFFER_SZ);
    if (buff == NULL) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        exit(99);
    }



    user_str_len = setup_buff(buff, input_string, BUFFER_SZ);     //see todos
    if (user_str_len < 0){
        printf("Error setting up buffer, error = %d", user_str_len);
        exit(2);
    }

    switch (opt){
        case 'c':
            rc = count_words(buff, BUFFER_SZ, user_str_len);  //you need to implement
            if (rc < 0){
                printf("Error counting words, rc = %d", rc);
                exit(2);
            }
            printf("Word Count: %d\n", rc);
            break;

        //TODO:  #5 Implement the other cases for 'r' and 'w' by extending
        //       the case statement options
        default:
            usage(argv[0]);
            exit(1);

        case 'r': {
            char *start = buff;                             // Points to the first character of the string
            char *end = buff + user_str_len - 1;            // Points to the last character of the string

            // Reverse the characters in the buffer in place
            while (start < end) {
                char temp = *start;       
                *start++ = *end;            
                *end-- = temp;              
            }
            
            break;
        }

        case 'w': 
            printf("Word Print\n----------\n");

            int total_words = count_words(buff, BUFFER_SZ, user_str_len); // Count total words
            int length = 0;              // Track the length of the current word
            int word_number = 1;         // Word numbering
            char *ptr = buff;            // Pointer to traverse the buffer

            printf("%d. ", word_number);

            for (int i = 0; i < user_str_len; i++) {
                if (*ptr != ' ') {       // Non-space character
                    putchar(*ptr);       // Print character
                    length++;            // Increment word length
                } else if (length > 0) { // End of a word
                    printf("(%d)\n", length);  // Print word length
                    if (word_number < total_words) {
                        printf("%d. ", ++word_number); // Print the next word number
                    }
                    length = 0;          // Reset word length
                }
                ptr++;
            }

            if (length > 0) {            // Handle the last word (if not followed by a space)
                printf("(%d)\n", length);
            }

            printf("\nNumber of words returned: %d\n", total_words);
            break;

        case 'x': 
            // Check if the correct number of arguments are provided (argc == 5)
            if (argc != 5) {
                printf("Error: Incorrect number of arguments for -x option.\n");
                usage(argv[0]);  // Display usage message
                exit(1);         // Exit with error code 1
            }

            // Capture the additional arguments
            char *search_str = argv[3];   // Word to search for
            char *replace_str = argv[4];  // Word to replace it with

            // Call a helper function to handle the search and replace
            rc = search_and_replace(buff, BUFFER_SZ, user_str_len, search_str, replace_str);
            if (rc == -1) {
                printf("Error: Buffer overflow during replacement.\n");
                exit(3);
            } else if (rc == 0) {
                printf("Error: Search string not found.\n");
                exit(3);
            }

            break;


        }

    //TODO:  #6 Dont forget to free your buffer before exiting

    print_buff(buff,BUFFER_SZ);

    if (buff != NULL) {
        free(buff);
        buff = NULL;
    }
    exit(0);
}

//TODO:  #7  Notice all of the helper functions provided in the 
//          starter take both the buffer as well as the length.  Why
//          do you think providing both the pointer and the length
//          is a good practice, after all we know from main() that 
//          the buff variable will have exactly 50 bytes?
//  
//          Even though the buffer is guaranteed to have 50 bytes, 
//          providing the length ensures safety and clarity when dealing 
//          with memory boundaries so we can avoid potential buffer 
//          overflows or missteps when modifying or accessing the buffer.
