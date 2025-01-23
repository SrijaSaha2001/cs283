#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define BUFFER_SZ 50

//prototypes
void usage(char *);
void print_buff(char *, int);
int  setup_buff(char *, char *, int);
int finalCheck = 0;
//prototypes for functions to handle required functionality
int  count_words(char *, int);
//add additional prototypes here


int setup_buff(char *buff, char *user_str, int len){
    //TODO: #4:  Implement the setup buff as per the directions
    int j = 0;
    bool space = true;
    int i = 0;
    while(user_str[i] != '\0') { //itereating until null character appears
        if((space == true) || (user_str[i] != ' ')) { //check to make sure only valid characters and non-duplicate whitespaces are copied
            buff[j] = user_str[i];
            j++;
        }
        if(user_str[i] == ' ') { //keeping a check to avoid duplicated whitespaces
            space = false;
        }
        else {
            space = true;
        }
        i++;
    }
    for(int i = j; i < len; i++) {
        *(buff + i) = '.'; //adding dots until BUFFER_SZ limit is reached
    }
     if(j > len) {
        printf("error: Provided input string is too long\n");
        exit(3);
        return -1;
    }
    return j; //for now just so the code compiles. 
}

void print_buff(char *buff, int len){
    printf("Buffer: ");
    for (int i=0; i<len; i++){
        putchar(*(buff+i));
    }
    putchar('\n');
}

void usage(char *exename){
    printf("usage: %s [-h|c|r|w|x] \"string\" [other args]\n", exename);
}

int count_words(char *buff, int len){
    int count = 0;
    bool foundFirstWord = false;
    for(int i = 0; i < len; i++) {
        if((i <= (len - 1)) && ((*(buff + i + 1) == '.') || *(buff + i + 1) == '\0')) { //counting the last word that might not have a white space
            count = count + 1;
            break;
        }
        if((*(buff + i) == ' ') && (foundFirstWord == true)) { //counting words seperated by whitespace
            count = count + 1;
        }
        if(*(buff + i) != ' ') { //keeping check to ensure first word is counted since it might not be preceded by white space
            foundFirstWord = true;
        }
    }
    return count;
}

int length_of_words(char *buff, int len) {
    int c = 0;
    int word = 1;
    printf("Word Print\n");
    printf("-----------\n");
    int i = 0;
    bool check = false;
    bool firstWord = false;
    buff[len + 1] = ' ';
    while((buff[i] != '\0')) {
        if(buff[i] == '.') { //if a dot character is found, it means we have exhausted letters in our string
            break;
        }
        if(buff[i] != ' ') { //if a string starts with a whitespace characters, this takes care of making sure we start counting from an actual word
            firstWord = true;
        }
        if(firstWord == true) {
            if(c == 0) {
                printf("%d. ", word); //printing numbered bullet points
            }
            if(*(buff + i) == ' ') {  
                printf(" (%d)\n", c); //printing out letters in a word when whitespace seperation is encountered
                c = 0;
                word = word + 1;
                check = true;
            }   
            else {
                printf("%c", buff[i]);
                c = c + 1;
                check = false;
            }
        }
        i++;
    }
    if(check == false) { //special case if last word is not followed by a whitespace
            printf(" (%d)\n", c);
            c = 0;
        }
    return 0;
}

int reverse(char *buff, int len) {
    printf("Reversed string: ");
    int c = 0;
    for(int i = (len - 1); i >= 0; i--) {
        if(buff[i] != '.') {
            putchar(buff[i]);
            c++;
        }
    }
    printf("\n");
    return 0;
}

//ADD OTHER HELPER FUNCTIONS HERE FOR OTHER REQUIRED PROGRAM OPTIONS

int main(int argc, char *argv[]){

    char *buff;             //placehoder for the internal buffer
    char *input_string;     //holds the string provided by the user on cmd line
    char opt;               //used to capture user option from cmd line
    int  rc;                //used for return codes
    int  user_str_len;      //length of user supplied string

    //TODO:  #1. WHY IS THIS SAFE, aka what if arv[1] does not exist?
    //      PLACE A COMMENT BLOCK HERE EXPLAINING
    //If argv[1] doesn't exist and we try accessing it through our code, we will get an error and the code will terminate.
    //Therefore, it is best to have a check in place that makes sure we have the minimum required amount of arguments.
    if ((argc < 2) || (*argv[1] != '-')){
        usage(argv[0]);
        exit(1);
    }

    opt = (char)*(argv[1]+1);   //get the option flag

    //handle the help flag and then exit normally
    if (opt == 'h'){
        usage(argv[0]);
        printf("-h     prints help about the program\n");
        printf("-c     counts of the number of words in the sample string\n");
        printf("-r     reverses the characters (in place) in sample string\n");
        printf("-w     prints the individual words and their length in the sample string\n");
        exit(0);
    }

    //WE NOW WILL HANDLE THE REQUIRED OPERATIONS

    //TODO:  #2 Document the purpose of the if statement below
    //      PLACE A COMMENT BLOCK HERE EXPLAINING
    //If we have less than 3 arguments, that means there is one essential parameter left out. THis if statement prints out the 
    //details of how to use the function so that the user knows what parameters to pass onto the function.
    if (argc < 3){
        usage(argv[0]);
        //setup_buff(buff, buff, user_str_len);
        exit(1);
    }
    input_string = argv[2]; //capture the user input string
    //TODO:  #3 Allocate space for the buffer using malloc and
    //          handle error if malloc fails by exiting with a 
    //          return code of 99
    // CODE GOES HERE FOR #3
    buff = malloc(BUFFER_SZ);
    user_str_len = setup_buff(buff, input_string, BUFFER_SZ);     //see todos
    if (user_str_len < 0){
        printf("Error setting up buffer, error = %d\n", user_str_len);
        exit(2);
    }
    int val = -1;
    switch (opt){
        case 'c':
            rc = count_words(buff, BUFFER_SZ);  //you need to implement
            val = rc;
            if (rc < 0){
                printf("Error counting words, rc = %d\n", rc);
                exit(2);
            }
            printf("Word Count: %d\n", rc);
            break;

        //TODO:  #5 Implement the other cases for 'r' and 'w' by extending
        //       the case statement options
        
        case 'w':
            val = length_of_words(buff, BUFFER_SZ);
            break;
        case 'r':
            val = reverse(buff, BUFFER_SZ);
            break;
        default:
            usage(argv[0]);
            exit(1);
        
    }
    if(val != -1) {//checking if all functions ran successfully
            print_buff(buff, BUFFER_SZ);
        }
    //TODO:  #6 Dont forget to free your buffer before exiting
    free(buff);
    
    exit(0);
}

//TODO:  #7  Notice all of the helper functions provided in the 
//          starter take both the buffer as well as the length.  Why
//          do you think providing both the pointer and the length
//          is a good practice, after all we know from main() that 
//          the buff variable will have exactly 50 bytes?
//  
//          PLACE YOUR ANSWER HERE
//Instead of hard coding the values and setting everything to 50 in the functions, passing on the length makes sure that in case
//we need to make any changes to the acceptable length value, we can simply make one change in the declaration of the variable
//instead of changing the value in every single place. Additionally, a pointer that is passed on can be at any character of the string.
//We can verify that we are at the right starting point of a string easily if we have the total length of it.