#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>

char* parseAfterLeftStrip (char *side, int space);
    //TODO ERRORS
    //x=3+ STACK
    //x=3+% STACK
int main() {
    bool error;
    bool equals;
    while (true) {
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
        int space = 0;
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
                space++;
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
            printf("Error0!\n");
            continue;
        }

        left = strtok(line, "=");
        right = strtok(NULL, "=");
        third = strtok(NULL, "=");
        if (third != NULL) {
            printf("Error1!\n");
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
                printf("Error18!\n");
            }
            if (variable2 != NULL) {
                printf("Error3!\n");
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
            while (right[k] == ' ') {
                k++;
                if (k == length2 - 1) {
                    error = true;
                    break;
                }
            }
            if (error) {
                printf("Error!\n");
                continue;
            }
            char *nsRight = parseAfterLeftStrip(right, space);
            if (error) {
                printf("Error5!\n");
                continue;
            } else {
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
                            } else if (type == 1) {
                                //! VARIABLE'I EKLENMESİ GEREKEN YERE EKLE
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
            }
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
            char *nsLeft = parseAfterLeftStrip(left, space);
            if (error) {
                printf("Error5!\n");
                continue;
            } else {
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
                            } else if (type == 1) {
                                //! VARIABLE'I EKLENMESİ GEREKEN YERE EKLE
                                //! İSTERSEN HASH'TEN DEĞER ÇEKİP SAYIYI ATACAĞIN YER BURAYI
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
            }
        }
    }
}
char* parseAfterLeftStrip (char *side, int space) {
    char *equation = malloc(strlen(side)-space);
    equation[0] = '\0';
    char *p = strtok(side, " ");
    while (p != NULL) {
        strcat(equation, p);
        p = strtok(NULL, " ");
    }
    return equation;
}
