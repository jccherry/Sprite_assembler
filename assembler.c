#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <assert.h>
#include "svec.h"
#include "map.h"


//input buffer for user to type into terminal
#define BUFFER_SIZE 256

map_str_t instruction_map;
map_str_t register_map;
map_int_t label_map;
svec* r_types;
svec* i_types;
svec* j_types;

//chomps off the endline
void chomp(char* string){
    string[strlen(string) - 1] = '\0';
}

//uses fgets to get stdin text into pointed buffer
//returns 0 if EOF is detected
//returns 1 if successful line read
int
read_line(char* buffer, int buffer_size)
{
    fgets(buffer, buffer_size, stdin);
    if (feof(stdin)) {
        puts("");
        return 0;
    }
    return 1;
}

//take a line of text and return a linked list of tokens from it
struct svec* 
tokenize(char* line)
{
    //maximum length that a token can be is the size of the buffer
    char token_buffer[BUFFER_SIZE];
    
    //keep track of the current token buffer size
    int current_buffer_size = 0;
    
    //keep track of number of tokens found
    int num_tokens_found = 0;
    
    //create a vector to store the tokens
    struct svec* found_tokens;
    found_tokens = make_svec();
    
    //iterate from 0 to null byte
    for(int ii = 0; ii < strlen(line)+1; ++ii) {
        
       
        if (line[ii] == '\t' || (line[ii] == ' ' && ii == 0)) {
            //ignore all tabs
            //printf("asaksjdlaksjd");
        } else if (line[ii] == ' ' || line[ii] == ':' || line[ii] == '.' || line[ii] == ',' || line[ii] == '(' || line[ii] == ')') { //check for spaces and commas and perens
            if (line[ii-1] == ' ' || line[ii-1] == '\t'|| line[ii-1] == ',' || line[ii-1] == '(' || line[ii-1] == ')' || line[ii-1] == '.' || line[ii-1] == ':') {
                //if previous character is space or tab or comma, ignore it
            } else {
                //otherwise, terminate buffer and track the found token
                token_buffer[current_buffer_size] = '\0';
                svec_push_back(found_tokens, token_buffer);
                num_tokens_found++;
                current_buffer_size = 0;
            }
        } else if (line[ii] == '\0') { //if null byte has been reached
            if (!(line[ii-1] == ' ' || line[ii-1] == '\t')) {
                //if the previous char isn't a space or tab, terminate buffer and track found token
                token_buffer[current_buffer_size] = '\0';
                svec_push_back(found_tokens, token_buffer);
                num_tokens_found++;
                current_buffer_size = 0;
                ii = strlen(line)+1;
            }
        } else { //if no special cases have arisen, keep adding to the token buffer
            token_buffer[current_buffer_size] = line[ii];
            current_buffer_size++; 
        }
    }
    return found_tokens;
}

void populate_maps() 
{
    map_set(&instruction_map, "add", "000000");
    map_set(&instruction_map, "sub", "000001");
    map_set(&instruction_map, "mul", "000010");
    map_set(&instruction_map, "div", "000011");
    map_set(&instruction_map, "mod", "000100");
    map_set(&instruction_map, "and", "000101");
    map_set(&instruction_map, "or", "000110");
    map_set(&instruction_map, "addi", "000111");
    map_set(&instruction_map, "subi", "001000");
    map_set(&instruction_map, "muli", "001001");
    map_set(&instruction_map, "divi", "001010");
    map_set(&instruction_map, "modi", "001011");
    map_set(&instruction_map, "andi", "001100");
    map_set(&instruction_map, "ori", "001101");
    map_set(&instruction_map, "load", "001110");
    map_set(&instruction_map, "store", "001111");
    map_set(&instruction_map, "mov", "010000");
    map_set(&instruction_map, "movi", "010001");
    map_set(&instruction_map, "sra", "010010");
    map_set(&instruction_map, "sll", "010011");
    map_set(&instruction_map, "srai", "010100");
    map_set(&instruction_map, "slli", "010101");
    map_set(&instruction_map, "inv", "010110");
    map_set(&instruction_map, "invi", "010111");
    map_set(&instruction_map, "j", "011000");
    map_set(&instruction_map, "je", "011001");
    map_set(&instruction_map, "jne", "011010");
    map_set(&instruction_map, "jl", "011011");
    map_set(&instruction_map, "jle", "011100");
    map_set(&instruction_map, "jg", "011101");
    map_set(&instruction_map, "jge", "011110");
    map_set(&instruction_map, "wvb", "011111");
    
    map_set(&register_map, "%rsp", "00000");
    map_set(&register_map, "%r1", "00001");
    map_set(&register_map, "%r2", "00010");
    map_set(&register_map, "%r3", "00011");
    map_set(&register_map, "%r4", "00100");
    map_set(&register_map, "%r5", "00101");
    map_set(&register_map, "%r6", "00110");
    map_set(&register_map, "%r7", "00111");
    map_set(&register_map, "%r8", "01000");
    map_set(&register_map, "%r9", "01001");
    map_set(&register_map, "%r10", "01010");
    map_set(&register_map, "%r11", "01011");
    map_set(&register_map, "%r12", "01100");
    map_set(&register_map, "%r13", "01101");
    map_set(&register_map, "%r14", "01110");
    map_set(&register_map, "%r15", "01111");
    map_set(&register_map, "%r16", "10000");
    map_set(&register_map, "%r17", "10001");
    map_set(&register_map, "%r18", "10010");
    map_set(&register_map, "%r19", "10011");
    map_set(&register_map, "%r20", "10100");
    map_set(&register_map, "%r21", "10101");
    map_set(&register_map, "%r22", "10110");
    map_set(&register_map, "%r23", "10111");
    map_set(&register_map, "%r24", "11000");
    map_set(&register_map, "%r25", "11001");
    map_set(&register_map, "%r26", "11010");
    map_set(&register_map, "%rcp", "11011");
    map_set(&register_map, "%rpp", "11100");
    map_set(&register_map, "%rbg", "11101");
    map_set(&register_map, "%rfg", "11110");
    map_set(&register_map, "%rov", "11111");
    
    //r type instructions
    svec_push_back(r_types, "add");
    svec_push_back(r_types, "sub");
    svec_push_back(r_types, "mul");
    svec_push_back(r_types, "div");
    svec_push_back(r_types, "and");
    svec_push_back(r_types, "or");
    svec_push_back(r_types, "sll");
    svec_push_back(r_types, "sra");
    svec_push_back(r_types, "mov");
    svec_push_back(r_types, "inv");
    
    //i type (immediate) instructions
    svec_push_back(i_types, "addi");
    svec_push_back(i_types, "subi");
    svec_push_back(i_types, "muli");
    svec_push_back(i_types, "divi");
    svec_push_back(i_types, "andi");
    svec_push_back(i_types, "ori");
    svec_push_back(i_types, "slli");
    svec_push_back(i_types, "srai");
    svec_push_back(i_types, "movi");
    svec_push_back(i_types, "inv");
    
    //j type (jump) instructions
    svec_push_back(j_types, "je");
    svec_push_back(j_types, "jne");
    svec_push_back(j_types, "jl");
    svec_push_back(j_types, "jle");
    svec_push_back(j_types, "jg");
    svec_push_back(j_types, "jge");

    map_set(&label_map, "start", 320);
    //printf("%d\n\n", *map_get(&label_map, "start"));
    
}

void reverseString(char* str) 
{ 
    int l, i; 
    char *begin_ptr, *end_ptr, ch; 
  
    // Get the length of the string 
    l = strlen(str); 
  
    // Set the begin_ptr and end_ptr 
    // initially to start of string 
    begin_ptr = str; 
    end_ptr = str; 
  
    // Move the end_ptr to the last character 
    for (i = 0; i < l - 1; i++) 
        end_ptr++; 
  
    // Swap the char from start and end 
    // index using begin_ptr and end_ptr 
    for (i = 0; i < l / 2; i++) { 
  
        // swap character 
        ch = *end_ptr; 
        *end_ptr = *begin_ptr; 
        *begin_ptr = ch; 
  
        // update pointers positions 
        begin_ptr++; 
        end_ptr--; 
    } 
} 


char *decimal_to_binary(int n)
{
   int c, d, count;
   char *pointer;
   
   count = 0;
   pointer = (char*)malloc(16+1);
   
   if (pointer == NULL)
      exit(EXIT_FAILURE);
     
   for (c = 15 ; c >= 0 ; c--)
   {
      d = n >> c;
     
      if (d & 1)
         *(pointer+count) = 1 + '0';
      else
         *(pointer+count) = 0 + '0';
     
      count++;
   }
   *(pointer+count) = '\0';
   
   return  pointer;
}


char* gen_instr_i(char* immediate) {
    for(int i = 0; i < strlen(immediate); ++i) {
            immediate[i] = immediate[i+1];
        }
        
        int immediate_int = atoi(immediate);
        
        char* immediate_bit_vector = decimal_to_binary(immediate_int);
        
        return immediate_bit_vector;

}

void printBin(svec* line)
{
    char* opcode = "000000";
    char* reg1 = "00000";
    char* reg2 = "00000";
    char* instr_i = "0000000000000000";
    char* instruction = svec_get(line, 0);
    opcode = *map_get(&instruction_map, instruction);

    if (svec_contains(r_types, instruction)!=-1) {

        reg2 = *map_get(&register_map, svec_get(line, 1));
        reg1 = *map_get(&register_map, svec_get(line, 2));
    } else if (svec_contains(i_types, instruction)!=-1) {
        reg2 = "00000";
        reg1 = *map_get(&register_map, svec_get(line, 2));
        
        char* immediate = svec_get(line, 1);
        instr_i = gen_instr_i(immediate);
        
        
    } else if (svec_contains(j_types, instruction)!=-1) {
        reg2 = *map_get(&register_map, svec_get(line, 1));
        reg1 = *map_get(&register_map, svec_get(line, 2));
        
        char* immediate = svec_get(line, 3);
        instr_i = decimal_to_binary(*map_get(&label_map, immediate));
        
       
    } else if (strcmp(instruction, "j") == 0) {
        char* immediate = svec_get(line, 1);
        instr_i = decimal_to_binary(*map_get(&label_map, immediate));
    } else if (strcmp(instruction, "load") == 0) {
        char* immediate = svec_get(line, 1);
        instr_i = decimal_to_binary(atoi(immediate));
        reg1 = *map_get(&register_map, svec_get(line, 2));
        reg2 = *map_get(&register_map, svec_get(line, 3));
    } else if (strcmp(instruction, "store") == 0) {
        char* immediate = svec_get(line, 2);
        instr_i = decimal_to_binary(atoi(immediate));
        //printf("instr_i: %d\n", instr_i);
        reg1 = *map_get(&register_map, svec_get(line, 3));
        reg2 = *map_get(&register_map, svec_get(line, 1));
    }
    
    printf("%s%s%s%.16s", opcode, reg1, reg2, instr_i);

}


int
main(int argc, char* argv[])
{
    //initialize the global instruction hashmap
    map_init(&instruction_map);
    map_init(&register_map);
    map_init(&label_map);
    r_types = make_svec();
    i_types = make_svec();
    j_types = make_svec();
    populate_maps();
    
    //print_svec(r_types);
    //printf("%d", svec_contains(r_types, "add"));
    
    
    //character buffer
    char line[BUFFER_SIZE];
    
    //read line return
    int read_state;
    
    
    if (argc != 3) {
        puts("Usage: ./assembler program.asm -flags");
        puts("flags: -o == output only");
        puts("flags: -f == debut mode");
    } else {
        //if a shell script has been passed as an argument to the program
        
        //do the same stuff, but just read from the file using fgets instead
        char line[BUFFER_SIZE];
        char* flags = argv[2];
        FILE* file1 = fopen(argv[1], "r");
        int lineNum = 0;
        while (fgets(line, sizeof line, file1) != NULL) {
            
            
            chomp(line);
            if (line[0] != '#' && line[0] != 0 && line[0] != ';') {
                
                svec* tokens = tokenize(line);
              
                
                if (line[0] == '.') {
                    int length = strlen(line);
                    char label[length-1];
                    for(int ii = 0; ii < length-1; ++ii) {
                    
                        label[ii] = line[ii+1];
                        
                    }
                    label[length-2] = 0;
                    //printf("label:%s  line num: %d\n", label, lineNum); 
                    map_set(&label_map, label, lineNum);
                } else if(strlen(line) >= 3 && tokens->size > 0){
                    //printf("%d: %s\n", lineNum, line);
                    lineNum++;
                }
                
                
            }
            
            

        }
        
        //printf("%d\n\n", *map_get(&label_map, "start"));
        //printf("%d\n\n", *map_get(&label_map, "label3"));
        // int test = *map_get(&label_map, " label");
        //printf("%d\n",test);
    
        fclose(file1);
        FILE* file = fopen(argv[1], "r");
        int fileLineNum = 1;
        while (fgets(line, sizeof line, file) != NULL) {
            
            
            chomp(line);
            if (line[0] != '#' && line[0] != '\n' && line[0] != '0' && line[0] != ';' && line[0] != '.') {
                
                svec* tokens = tokenize(line);
                /*
                printf("line:%s|\n", line);
                printf("tokens:");
                print_svec(tokens);
                printf("strlen:%d\n",strlen(line));
                */
                if(strlen(line) >= 3 && tokens->size > 0) {
                    char* first_token = strdup(svec_get(tokens,0));
                    //printf("first token:%s|\n",first_token);
                    //print_svec(tokens);
                    if (strcmp(flags, "-d")==0){
                        printf("%d: %s\n", fileLineNum, line);
                    } else if (strcmp(flags, "-o")==0) {
                        printBin(tokens);
                        printf("\n");
                    }
                        
                    
                }
                
                
                
                
                //char* instruction = svec_get(tokens,0);
                //printf("%s ", *map_get(&instruction_map, instruction));
                
                
            }
            
            fileLineNum++;
        }
        fclose(file);
    }
    
    return 0;
}


