#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

/*** Stack and HashMap**/
#define MAP_CAPACITY 128
#define stack_cap 256

typedef struct {
    char *name;
} Stack_Variable;
typedef struct {
    Stack_Variable *content;
    int size;
} Stack;
Stack *Stack_construct() {
    Stack *stack = malloc(sizeof(Stack));
    stack->content = calloc(stack_cap, sizeof(Stack_Variable));
    stack->size=0;
    return stack;
}

void push(Stack* stack,char* key){
    stack->content[stack->size++].name = key;
}
char* pop(Stack* stack){
    return stack->content[--(stack->size)].name;
}
char* peek(Stack* stack){
    char* ans = malloc(strlen(stack->content[(stack->size)-1].name)+1);
    ans = strcpy(ans,stack->content[(stack->size)-1].name);
    return ans;
}

typedef struct {
    long long value;
    char* name;
} Hash_Variable;

typedef struct {
    Hash_Variable *entries;
    int capacity;
} Map;

long long hash(char *key) {
    long long hash = 0;
    for(int i=0;i<strlen(key)-1;i++){
        hash = (hash << 2) + hash + key[i];
    }
    return hash;
}

Map *HashMap_construct() {
    Map *map = malloc(sizeof(Map));
    map->capacity = MAP_CAPACITY;
    map->entries = calloc(MAP_CAPACITY, sizeof(Hash_Variable));
    return map;
}
void put(Map *map, char *key, long long value) {
    long long index = hash(key) % map->capacity;
    while (map->entries[index].name != NULL) {
        if (strcmp(map->entries[index].name, key) == 0) {
            map->entries[index].value = value;
            return;
        }
        index = (index + 1) % map->capacity;
    }
    map->entries[index].value = value;
    map->entries[index].name = key;
}
long long get(Map *map, char *key) {
    long long index = hash(key) % map->capacity;
    while (map->entries[index].name != NULL) {
        if (strcmp(map->entries[index].name, key) == 0) {
            return map->entries[index].value;
        }
        index = (index + 1) % map->capacity;
    }
    put(map,key,0);
    return 0;
}
/*****/
/**** Helper Functions ************************/
int isInt(char* key){
    int var = 0;
    for(int i=0;i< strlen(key);i++){
        if(isdigit(key[i])==0){
            if(i==0 && '-' == key[i]&& strlen(key)>1){
                continue;
            }else{
                var = 1;
            }
        }
    }
    return var;
}
int isVar(char* key){
    int var = 0;
    for(int i=0;i< strlen(key);i++){
        if(isalpha(key[i])==0){
            var = 1;
        }
    }
    return var;
}
int is_Func(char* item){
    if (strcmp(item, "xor") == 0 || strcmp(item, "not") == 0 || strcmp(item, "lr") == 0 || strcmp(item, "rr") == 0 || strcmp(item, "rs") == 0 || strcmp(item, "ls") == 0){
        return 0;
    }
    return 1;
}
int precedence(char* key){
    if (strcmp(key, "not") == 0){
        return 6;
    }else if (strcmp(key, "*") == 0){
        return 5;
    }else if (strcmp(key, "+") == 0|| strcmp(key,"-")==0){
        return 4;
    }else if(strcmp(key, "ls") == 0|| strcmp(key,"rs")==0){
        return 3;
    }else if(strcmp(key, "&") == 0){
        return 2;
    }else if(strcmp(key, "xor") == 0){
        return 1;
    }else{
        return 0;
    }
}
int is_left_Parenthesis(char* key){
    return strcmp(key,"(");
}
int is_right_Parenthesis(char* key){
    return strcmp(key,")");
}
int push_to_stack(Stack* stack_operator,Stack* output,char* key){
    // 0 means no error 1 means error
    if(is_Func(key)==0){
        while(stack_operator->size>0){
            if(precedence(peek(stack_operator))>= precedence(key)){
                if(strcmp(peek(stack_operator),"(")!=0){
                    push(output,pop(stack_operator));
                }else{
                    push(stack_operator,key);
                    break;
                }
            }else{
                push(stack_operator,key);
                break;
            }
        }
        return 0;
    }else if(is_left_Parenthesis(key)==0){
        push(stack_operator,key);
        return 0;
    }else if(is_right_Parenthesis(key)==0){
        int error = 1;
        while(stack_operator->size>0){
            char* temp= pop(stack_operator);
            if(is_left_Parenthesis(temp)!=0){
                push(output,temp);
            }else{
                error=0;
                break;
            }
        }
        return error;
    }else if(isVar(key)==0|| isInt(key)==0){
        push(output,key);
        return 0;
    } else{//operator
        while(stack_operator->size>0){
            if(strcmp(peek(stack_operator),"(")==0){
                break;
            }
            if(precedence(peek(stack_operator))>= precedence(key)){
                    push(output,pop(stack_operator));
            }else{
                break;
            }
        }
        push(stack_operator,key);
        return 0;
    }
}

long long postfix(Stack *stack,int* hks_error){
    char* temp = pop(stack);
    long long ans;
    if(stack->size<0){
        *hks_error = 1;
        return 0;
    }else if(isInt(temp)==0){
        ans = atoll(temp);
        return ans;
    }else if(strcmp("not",temp)==0){
        ans =  ~(postfix(stack,hks_error));
        return ans;
    }else if(strcmp("*",temp)==0){
        ans = postfix(stack,hks_error)*postfix(stack,hks_error);
        return ans;
    }else if(strcmp("-",temp)==0){
        long long a = postfix(stack,hks_error);
        long long b = postfix(stack,hks_error);
        ans = b-a;
        return ans;
    }else if(strcmp("+",temp)==0){
        ans= postfix(stack,hks_error)+postfix(stack,hks_error);
        return ans;
    }else if(strcmp("ls",temp)==0){
        long long a = postfix(stack,hks_error);
        long long b = postfix(stack,hks_error);
        ans= b << a;
        return ans;
    }else if(strcmp("rs",temp)==0){
        long long a = postfix(stack,hks_error);
        long long b = postfix(stack,hks_error);
        ans= b >> a;
        return ans;
    }else if(strcmp("lr",temp)==0){
        long long a = postfix(stack,hks_error);
        long long b = postfix(stack,hks_error);
        ans=((b << a)|(b >> (64 - a)));
        return ans;
    }else if(strcmp("rr",temp)==0){
        long long a = postfix(stack,hks_error);
        long long b = postfix(stack,hks_error);
        ans=((b >> a)|(b << (64 - a)));
        return ans;
    }else if(strcmp("&",temp)==0){
        ans=(postfix(stack,hks_error)&postfix(stack,hks_error));
        return ans;
    }else if(strcmp("xor",temp)==0){
        ans=(postfix(stack,hks_error)^postfix(stack,hks_error));
        return ans;
    }else if(strcmp("|",temp)==0){
        ans=(postfix(stack,hks_error)|postfix(stack,hks_error));
        return ans;
    } else{
        return 0;
    }
}
/*****************************************/
char* parseAfterLeftStrip (char *side);
    //TODO ERRORS
    //x=3+ STACK
    //x=3+% STACK
int main() {
    bool error;
    bool equals;
    Map* HashMap = HashMap_construct();
    while (true) {
        Stack* Operator = Stack_construct();
        Stack* Output = Stack_construct();
        Stack* Funcs = Stack_construct();
        char *left;
        char *right;
        char *third;
        char line[256];
        error = false;
        equals = false;
        printf("> ");
        if (fgets(line, sizeof(line), stdin) == NULL) {
            break;
        }
        char *p = line;
        int par = 0;
        bool ch = false;
        bool sp = false;
        bool nu = false;
        // in this loop we traverse through the input
        while (*p != '\n') {
            if (*p == '%') {
                // we put \n when we see a comments so the program will ignore it
                // the reason why we chose \n instead of \0 is, inputs without comments end with \n too
                *p = '\n';
                break;
            } else if (*p == '=') {
                // when we see equals sign we approve that the input is an assignment input, this boolean will be used later
                // other booleans will be explained later
                equals = true;
                ch = false;
                sp = false;
                nu = false;
                char *p2 = p;
                p2++;
                if (*p2 == '=') {
                    // if there are two equal signs side by side we raise error
                    error = true;
                    break;
                }
            } else if (*p == '(') {
                // we keep the count of open parentheses here
                ch = false;
                sp = false;
                nu = false;
                par++;
            } else if (*p == ')') {
                // we increase the number of open parentheses
                ch = false;
                sp = false;
                nu = false;
                par--;
            } else if (isalpha(*p)) {
                if ((ch && sp) || (ch && nu)) {
                    error = true;
                    break;
                }
                ch = true;
            } else if (isdigit(*p)) {
                if ((nu && sp) || (nu && ch)) {
                    error = true;
                    break;
                }
                nu = true;
            } else if (*p == ' ') {
                if (ch || nu) {
                    sp = true;
                }
            }
            else if (*p == '+' || *p == '*' || *p == '-' || *p == '&' || *p == '|' || *p == ',') {
                ch = false;
                sp = false;
                nu = false;
            } else { //legal olmayan karakterler buraya düşecek
                error = true;
                break;
            }
                p++;
        }
        if (par != 0 || error) {
            printf("Error!\n");
            continue;
        }

        left = strtok(line, "=");
        right = strtok(NULL, "=");
        third = strtok(NULL, "=");
        if (third != NULL) {
            printf("Error!\n");
            continue;
        }
        //}
        char *variable;
        char *variable2;

        if (right != NULL && strcmp(right, "\n") != 0 && strcmp(right, " ") != 0) {
            //! BU IF'İN İÇİNDE BULDUĞUMUZ DEĞER variable DEĞİŞKENİNE HASHLENECEK
            int i = 0;
            int length = strlen(left);
            while (left[i] == ' ') {
                i++;
                if (i == length - 1) {
                    error = true;
                    break;
                }
            }
            if (error) {
                printf("Error!\n");
                continue;
            }
            variable = strtok(left, " ");
            variable2 = strtok(NULL, " ");
            if (variable == NULL) {
                printf("Error!\n");
            }
            if (variable2 != NULL) {
                printf("Error!\n");
                continue;
            }
            char *po = variable;
            while (*po != '\0') {
                if (!isalpha(*po)) {
                    error = true;
                    break;
                }
                po++;
            }
            if (strcmp(variable, "xor") == 0 || strcmp(variable, "ls") == 0 || strcmp(variable, "rs") == 0 ||
                strcmp(variable, "lr") == 0 || strcmp(variable, "rr") == 0 || strcmp(variable, "not") == 0) {
                printf("Error!\n");
                continue;
            }
            //BUNDAN SONRA SAĞ TARAFIN KODU GELECEK
            int k = 0;
            int length2 = strlen(right);
            char *p8 = right;
            char *r = malloc(length2*sizeof(char));
            while (*p8 != '\n') {
                r[k] = *p8;
                k++;
                p8++;
            }
            r[k] = '\0';
            if (strtok(r, " ") == NULL) {
                error = true;
            }
            if (error) {
                printf("Error!\n");
                continue;
            }
            char *nsRight = parseAfterLeftStrip(right);
                char *p3 = nsRight;
                int type = 0; //0=start / 1=string / 2=number / 3=operator / 4=function
                //4'ten sonra parantez olduğunu zaten kesinlikle belirttik
                int length3 = strlen(nsRight);
                while (*p3 != '\n' && *p3 != '\0') {
                    char *item = malloc(length3 + 1 * sizeof(char));
                    item[0] = '\0';
                    int a = 0;
                    while (isalpha(*p3)) {
                        if (type == 1 || type == 2 || type == 6) {
                            a=0;
                            error = true;
                            break;
                        }
                        item[a++] = *p3;
                        p3++;
                    }
                    if (a > 0) {
                        item[a] = '\0';
                        if (strcmp(item, "xor") == 0 || strcmp(item, "not") == 0 || strcmp(item, "lr") == 0 || strcmp(item, "rr") == 0 || strcmp(item, "rs") == 0 || strcmp(item, "ls") == 0) {
                            type = 4;
                            if (*p3 != '(') {
                                error = true;
                                break;
                            } else {
                                char *p4 = p3;
                                p4++;
                                int parenthesis = 1;
                                int function;
                                if (strcmp(item, "not") == 0) {
                                    function = 0;
                                } else {
                                    function = 1;
                                }
                                int comma = 0;
                                while (parenthesis > 0) {
                                    if (*p4 == '(') {
                                        parenthesis++;
                                        p4--;
                                        if (*p4 == 'r' || *p4 == 's') {
                                            function++;
                                        }
                                        p4++;
                                    } else if (*p4 == ',') {
                                        comma++;
                                    }
                                    if (comma > function) {
                                        error = true;
                                        break;
                                    }
                                    if (*p4 == ')') {
                                        parenthesis--;
                                    }
                                    p4++;
                                }
                                if (comma != function) {
                                    error = true;
                                }
                            }
                        } else {
                            //variable
                            type = 1;
                            if (*p3 == '(') {
                                error = true;
                                break;
                            }
                        }
                        if (error) {
                            break;
                        } else {
                            if (type == 4) {
                               //! FONKSİYONU EKLENMESİ GEREKEN YERE EKLE
                                push(Funcs,item);
                            } else if (type == 1) {
                                //! VARIABLE'I EKLENMESİ GEREKEN YERE EKLE
                                long long var = get(HashMap,item);
                                char val[256];
                                sprintf(val, "%lld", var);
                                char *aaaaa = malloc(sizeof val);
                                aaaaa = strcpy(aaaaa, val);
                                push_to_stack(Operator,Output,aaaaa);
                            }
                        }
                        continue;
                    }
                    while (isdigit(*p3)) {
                        if (type == 1 || type == 2 || type == 6) {
                            a=0;
                            error = true;
                            break;
                        }
                        item[a++] = *p3;
                        p3++;
                    }
                    if (a > 0) {
                        type = 2;
                        item[a] = '\0';
                        //! SAYIYI EKLENMESİ GEREKEN YERE EKLE
                        push_to_stack(Operator,Output,item);
                        continue;
                    }
                    while (*p3 == '(') {
                        if (type == 1 || type == 2 || type == 6) {
                            a=0;
                            error = true;
                            break;
                        }
                        item[a++] = *p3;
                        p3++;
                        break;
                    }
                    if (a>0) {
                        type = 5;
                        item[a] = '\0';
                        //! AÇIK PARANTEZİ EKLENMESİ GEREKEN YERE EKLE
                        if(Funcs->size>0){
                            if(strcmp(peek(Funcs),"not")==0){
                                push(Operator, pop(Funcs));
                            }
                        }
                        push_to_stack(Operator,Output,item);
                        continue;
                    }
                    while (*p3 == ')') {
                        if (type == 0 || type == 3 || type == 5) {
                            a=0;
                            error = true;
                            break;
                        }
                        item[a++] = *p3;
                        p3++;
                        break;
                    }
                    if (a>0) {
                        type = 6;
                        item[a] = '\0';
                        //! KAPALI PARANTEZ EKLENMESİ GEREKEN YERE EKLE
                        push_to_stack(Operator,Output,item);
                        continue;
                    }
                    while (*p3 == '+' || *p3 == '*' || *p3 == '-' || *p3 == '&' || *p3 == '|' || *p3 == ',') {
                        if (type == 0 || type == 3 || type == 5) {
                            a=0;
                            error = true;
                            break;
                        }
                        item[a++] = *p3;
                        p3++;
                    }
                    if (a==1) {
                        type = 3;
                        item[a] = '\0';
                        //! DİĞER OPERATÖRLER EKLENMESİ GEREKEN YERE EKLE
                        if(strcmp(item,",")!=0) {
                            push_to_stack(Operator, Output, item);
                        }else{
                            push_to_stack(Operator, Output, pop(Funcs));
                        }
                        continue;
                    } else if (a!=1) {
                        error =true;
                    }
                    if (error) {
                        break;
                    }
                }
                if (error) {
                    printf("Error!\n");
                    continue;
                }
            /*************************************************************/
            int error_hasan_kerem = 0;
            while (Operator->size>0){
                if(is_left_Parenthesis(peek(Operator))==0){
                    error_hasan_kerem = 1;
                    break;
                }else{
                    push(Output, pop(Operator));
                }
            }
            if(error_hasan_kerem==1){
                printf("Error!\n");
                continue;
            }
            /*******/
            int hks_error = 0;
            if(Output->size==0){
                continue;
            }
            long long ans = postfix(Output,&hks_error);
            if(hks_error==1){
                printf("Error!\n");
                continue;
            }
            char* temp_name = malloc(sizeof  variable);
            strcpy(temp_name,variable);
            put(HashMap,temp_name,ans);



            /**************************************************************/
        }
        else {
            if (equals) {
                printf("Error!\n");
                continue;
            }
            //RHS'DE BURADA YAPTIĞIMIZ ŞEYİN AYNISI YAPILACAK
            //TEK FARK BURADA SONUÇ PRINT, ORADA ASSIGN
            int i = 0;
            int length = strlen(left);
            while (left[i] == ' ') {
                i++;
                if (i == length - 1) {
                    error = true;
                    break;
                }
            }
            if (error) {
                //burada print yapmayacağız
                continue;
            }
            //BUNDAN SONRA SAĞ TARAFIN KODU GELECEK
            char *nsLeft = parseAfterLeftStrip(left);
                char *p5 = nsLeft;
                int type = 0; //0=start / 1=string / 2=number / 3=operator / 4=function
                //4'ten sonra parantez olduğunu zaten kesinlikle belirttik
                int length3 = strlen(nsLeft);
                while (*p5 != '\0' && *p5 != '\n') {
                    char *item = malloc(length3 + 1 * sizeof(char));
                    item[0] = '\0';
                    int a = 0;
                    while (isalpha(*p5)) {
                        if (type == 1 || type == 2 || type == 6) {
                            a = 0;
                            error = true;
                            break;
                        }
                        item[a++] = *p5;
                        p5++;
                    }
                    if (a > 0) {
                        item[a] = '\0';
                        if (strcmp(item, "xor") == 0 || strcmp(item, "not") == 0 || strcmp(item, "lr") == 0 ||
                            strcmp(item, "rr") == 0 || strcmp(item, "rs") == 0 || strcmp(item, "ls") == 0) {
                            type = 4;
                            if (*p5 != '(') {
                                error = true;
                                break;
                            } else {
                                char *p6 = p5;
                                p6++;
                                int parenthesis = 1;
                                int function;
                                if (strcmp(item, "not") == 0) {
                                    function = 0;
                                } else {
                                    function = 1;
                                }
                                int comma = 0;
                                while (parenthesis > 0) {
                                    if (*p6 == '(') {
                                        parenthesis++;
                                        p6--;
                                        if (*p6 == 'r' || *p6 == 's') {
                                            function++;
                                        }
                                        p6++;
                                    } else if (*p6 == ',') {
                                        comma++;
                                    }
                                    if (comma > function) {
                                        error = true;
                                        break;
                                    }
                                    if (*p6 == ')') {
                                        parenthesis--;
                                    }
                                    p6++;
                                }
                                if (comma != function) {
                                    error = true;
                                }
                            }
                        } else {
                            type = 1;
                            if (*p5 == '(') {
                                error = true;
                                break;
                            }
                        }
                        if (error) {
                            break;
                        }  else {
                            if (type == 4) {
                                //! FONKSİYONU EKLENMESİ GEREKEN YERE EKLE
                                push(Funcs,item);
                            } else if (type == 1) {
                                //! VARIABLE'I EKLENMESİ GEREKEN YERE EKLE
                                //! İSTERSEN HASH'TEN DEĞER ÇEKİP SAYIYI ATACAĞIN YER BURAYI
                                long long var = get(HashMap,item);
                                char val[256];
                                sprintf(val, "%lld", var);
                                char *aaaaa = malloc(sizeof val);
                                aaaaa = strcpy(aaaaa, val);
                                push_to_stack(Operator,Output,aaaaa);
                            }
                        }
                        continue;
                    }
                    while (isdigit(*p5)) {
                        if (type == 1 || type == 2 || type == 6) {
                            a = 0;
                            error = true;
                            break;
                        }
                        item[a++] = *p5;
                        p5++;
                    }
                    if (a > 0) {
                        type = 2;
                        item[a] = '\0';
                        //! SAYIYI EKLENMESİ GEREKEN YERE EKLE
                        push_to_stack(Operator,Output,item);
                        continue;
                    }
                    while (*p5 == '(') {
                        if (type == 1 || type == 2 || type == 6) {
                            a = 0;
                            error = true;
                            break;
                        }
                        item[a++] = *p5;
                        p5++;
                        break;
                    }
                    if (a > 0) {
                        type = 5;
                        item[a] = '\0';
                        //! AÇIK PARANTEZİ EKLENMESİ GEREKEN YERE EKLE
                        if(Funcs->size>0){
                            if(strcmp(peek(Funcs),"not")==0){
                                push(Operator, pop(Funcs));
                            }
                        }
                        push_to_stack(Operator,Output,item);
                        continue;
                    }
                    while (*p5 == ')') {
                        if (type == 0 || type == 3 || type == 5) {
                            a = 0;
                            error = true;
                            break;
                        }
                        item[a++] = *p5;
                        p5++;
                        break;
                    }
                    if (a > 0) {
                        type = 6;
                        item[a] = '\0';
                        //! KAPALI PARANTEZ EKLENMESİ GEREKEN YERE EKLE
                        push_to_stack(Operator,Output,item);
                        continue;
                    }
                    while (*p5 == '+' || *p5 == '*' || *p5 == '-' || *p5 == '&' || *p5 == '|' || *p5 == ',') {
                        if (type == 0 || type == 3 || type == 5) {
                            a = 0;
                            error = true;
                            break;
                        }
                        item[a++] = *p5;
                        p5++;
                    }
                    if (a == 1) {
                        type = 3;
                        item[a] = '\0';
                        //! DİĞER OPERATÖRLER EKLENMESİ GEREKEN YERE EKLE
                        if(strcmp(item,",")!=0) {
                            push_to_stack(Operator, Output, item);
                        }else{
                            push_to_stack(Operator, Output, pop(Funcs));
                        }
                        continue;
                    } else if (a != 1) {
                        error = true;
                    }
                    if (error) {
                        break;
                    }
                }
                if (error) {
                    printf("Error!\n");
                    continue;
                }

            /*************************************************************/
            int error_hasan_kerem = 0;
            while (Operator->size>0){
                if(is_left_Parenthesis(peek(Operator))==0){
                    error_hasan_kerem = 1;
                    break;
                }else{
                    push(Output, pop(Operator));
                }
            }
            if(error_hasan_kerem==1){
                printf("Error!\n");
                continue;
            }
            /*******/
            int hks_error = 0;
            if(Output->size==0){
                continue;
            }
            long long ans = postfix(Output,&hks_error);
            if(hks_error==1){
                printf("Error!\n");
                continue;
            }
            printf("%lld\n",ans);
            /*******/
            /**************************************************************/

        }
    }
}
char* parseAfterLeftStrip (char *side) {
    char *equation = malloc(strlen(side));
    equation[0] = '\0';
    char *p = strtok(side, " ");
    while (p != NULL) {
        strcat(equation, p);
        p = strtok(NULL, " ");
    }
    return equation;
}
