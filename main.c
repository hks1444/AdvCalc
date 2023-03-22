#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>

char* parseAfterLeftStrip (int i, char *side, bool *error);

int main() {
    //TODO ERRORS
    //x=   (eşittirden sonra boşluk yok)
    //x=((5) STACK
    //rs=2
    //x=3+ STACK
    //x=(3+ STACK
    //x=3+% STACK
    //% kodunu değiştirdikten sonra tüm % caselerini dene
    char lineFull[256];
    bool error;
    //bool first = true;
    while (true) {
        char *left;
        char *right;
        char *third;
        char *line;
        error = false;
        printf("> ");
        char * deneme = "\0";

        if (fgets(lineFull, sizeof(lineFull), stdin) == NULL) { //bu niye böyle anlamadım, hoca böyle yazdı
            break;
        } else {
            line = strtok(lineFull, "%"); //burada line objesi değişmemiş olabilir, bunu kontrol et
            //printf("%s\n", line);
            left = strtok(line, "=");
            right = strtok(NULL, "=");
            third = strtok(NULL, "=");
            if (third != NULL) {
                printf("Error1!\n");
                continue;
            }
        }
        char *variable;
        char *variable2;
        if (right != NULL) {
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
            //sağdan strip durumunu çözmemiz lazım
            //bunu önceden yazmışım ama bir hata vermiyor şu anda
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
                error = true;
                continue;
            }
            if (error) {
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
            //buradaki atama nasıl olacak
            char *nsRight = parseAfterLeftStrip(k, right, &error);
            if (error) {
                printf("Error5!\n");
                continue;
            } else {
                char *p = nsRight;
                //char *array[256]; //YOK EDİLECEK
                //int z = 0; //YOK EDİLECEK
                int type = 0; //0=start / 1=string / 2=number / 3=operator / 4=function
                //4'ten sonra parantez olduğunu zaten kesinlikle belirttik
                int length3 = strlen(nsRight);
                while (*p != '\n' && *p != '\0') {
                    char *item = malloc(length3 + 1 * sizeof(char));
                    item[0] = '\0';
                    int a = 0;
                    while (isalpha(*p)) {
                        if (type == 1 || type == 2 || type == 6) {
                            a=0;
                            error = true;
                            break;
                        }
                        item[a++] = *p;
                        p++;
                    }
                    if (a > 0) {
                        item[a] = '\0';
                        if (strcmp(item, "xor") == 0 || strcmp(item, "not") == 0 || strcmp(item, "lr") == 0 || strcmp(item, "rr") == 0 || strcmp(item, "rs") == 0 || strcmp(item, "ls") == 0) {
                            type = 4;
                            if (*p != '(') {
                                error = true;
                                break;
                            } else {
                                char *p2 = p;
                                p2++;
                                int parenthesis = 1;
                                int function;
                                if (strcmp(item, "not") == 0) {
                                    function = 0;
                                } else {
                                    function = 1;
                                }
                                int comma = 0;
                                while (parenthesis > 0) {
                                    if (*p2 == '(') {
                                        parenthesis++;
                                        p2--;
                                        if (*p2 == 'r' || *p2 == 's') {
                                            function++;
                                        }
                                        p2++;
                                    } else if (*p2 == ',') {
                                        comma++;
                                    }
                                    if (comma > function) {
                                        error = true;
                                        break;
                                    }
                                    if (*p2 == ')') {
                                        parenthesis--;
                                    }
                                    p2++;
                                }
                                if (comma != function) {
                                    error = true;
                                }
                            }
                        } else {
                            //variable
                            type = 1;
                            if (*p == '(') {
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
                    while (isdigit(*p)) {
                        if (type == 1 || type == 2 || type == 6) {
                            a=0;
                            error = true;
                            break;
                        }
                        item[a++] = *p;
                        p++;
                    }
                    if (a > 0) {
                        type = 2;
                        item[a] = '\0';
                        //! SAYIYI EKLENMESİ GEREKEN YERE EKLE
                        continue;
                    }
                    while (*p == '(') {
                        if (type == 1 || type == 2 || type == 6) {
                            a=0;
                            error = true;
                            break;
                        }
                        item[a++] = *p;
                        p++;
                        break;
                    }
                    if (a>0) {
                        type = 5;
                        item[a] = '\0';
                        //! AÇIK PARANTEZİ EKLENMESİ GEREKEN YERE EKLE
                        continue;
                    }
                    while (*p == ')') {
                        if (type == 0 || type == 3 || type == 5) {
                            a=0;
                            error = true;
                            break;
                        }
                        item[a++] = *p;
                        p++;
                        break;
                    }
                    if (a>0) {
                        type = 6;
                        item[a] = '\0';
                        //! KAPALI PARANTEZ EKLENMESİ GEREKEN YERE EKLE
                        continue;
                    }
                    while (*p == '+' || *p == '*' || *p == '-' || *p == '&' || *p == '|' || *p == ',') {
                        if (type == 0 || type == 3 || type == 5) {
                            a=0;
                            error = true;
                            break;
                        }
                        item[a++] = *p;
                        p++;
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


        if (right == NULL) {
            //DİĞER BLOKTAKİ KOD BİTİNCE BURAYA DA GELECEK
            //! RIGHT VE ONUNLA ALAKALI HER ŞEYİ DEĞİŞTİRMEMİZ LAZIM

            //RHS'DE BURADA YAPTIĞIMIZ ŞEYİN AYNISI YAPILACAK
            //TEK FARK BURADA SONUÇ PRINT, ORADA ASSIGN
            //SOL TARAFI OKUMA VE HATA KONTROL ETME KISMI BURASI
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
            char *nsLeft = parseAfterLeftStrip(i, left, &error);
            if (error) {
                printf("Error5!\n");
                continue;
            } else {
                char *p = nsLeft;
                //char *array[256]; //YOK EDİLECEK
                //int z = 0; //YOK EDİLECEK
                int type = 0; //0=start / 1=string / 2=number / 3=operator / 4=function
                //4'ten sonra parantez olduğunu zaten kesinlikle belirttik
                int length3 = strlen(nsLeft);
                while (*p != '\0' && *p != '\n') {
                    char *item = malloc(length3 + 1 * sizeof(char));
                    item[0] = '\0';
                    int a = 0;
                    while (isalpha(*p)) {
                        if (type == 1 || type == 2 || type == 6) {
                            a = 0;
                            error = true;
                            break;
                        }
                        item[a++] = *p;
                        p++;
                    }
                    if (a > 0) {
                        item[a] = '\0';
                        if (strcmp(item, "xor") == 0 || strcmp(item, "not") == 0 || strcmp(item, "lr") == 0 ||
                            strcmp(item, "rr") == 0 || strcmp(item, "rs") == 0 || strcmp(item, "ls") == 0) {
                            type = 4;
                            if (*p != '(') {
                                //printf("Error13!\n");
                                error = true;
                                break;
                            } else {
                                //operator listesine ekleme yap
                                char *p2 = p;
                                p2++;
                                int parenthesis = 1;
                                int function;
                                if (strcmp(item, "not") == 0) {
                                    function = 0;
                                } else {
                                    function = 1;
                                }
                                int comma = 0;
                                while (parenthesis > 0) {
                                    if (*p2 == '(') {
                                        parenthesis++;
                                        p2--;
                                        if (*p2 == 'r' || *p2 == 's') {
                                            function++;
                                        }
                                        p2++;
                                    } else if (*p2 == ',') {
                                        comma++;
                                    }
                                    if (comma > function) {
                                        error = true;
                                        break;
                                    }
                                    if (*p2 == ')') {
                                        parenthesis--;
                                    }
                                    p2++;
                                }
                                if (comma != function) {
                                    error = true;
                                }
                            }
                        } else {
                            //variable
                            type = 1;
                            if (*p == '(') {
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
                    while (isdigit(*p)) {
                        if (type == 1 || type == 2 || type == 6) {
                            a = 0;
                            error = true;
                            break;
                        }
                        item[a++] = *p;
                        p++;
                    }
                    if (a > 0) {
                        type = 2;
                        item[a] = '\0';
                        //! SAYIYI EKLENMESİ GEREKEN YERE EKLE
                        continue;
                    }
                    while (*p == '(') {
                        if (type == 1 || type == 2 || type == 6) {
                            a = 0;
                            error = true;
                            break;
                        }
                        item[a++] = *p;
                        p++;
                        break;
                    }
                    if (a > 0) {
                        type = 5;
                        item[a] = '\0';
                        //! AÇIK PARANTEZİ EKLENMESİ GEREKEN YERE EKLE
                        continue;
                    }
                    while (*p == ')') {
                        if (type == 0 || type == 3 || type == 5) {
                            a = 0;
                            error = true;
                            break;
                        }
                        item[a++] = *p;
                        p++;
                        break;
                    }
                    if (a > 0) {
                        type = 6;
                        item[a] = '\0';
                        //! KAPALI PARANTEZ EKLENMESİ GEREKEN YERE EKLE
                        continue;
                    }
                    while (*p == '+' || *p == '*' || *p == '-' || *p == '&' || *p == '|' || *p == ',') {
                        if (type == 0 || type == 3 || type == 5) {
                            a = 0;
                            error = true;
                            break;
                        }
                        item[a++] = *p;
                        p++;
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
//boş string returnunu değerlendirmemiz lazım
//error da olabilir, düz devam da
char* parseAfterLeftStrip (int i, char *side, bool *error) {
    int len = strlen(side);
    bool ch = false;
    bool sp = false;
    int space = 0;
    for (; i < len - 1; i++) {
        if (isalnum(side[i])) {
            if (ch && sp) {
                *error = true;
                return "";
            }
            ch = true;
        } else if (side[i] == ' ') {
            if (ch) {
                sp = true;
            }
            space++;
        } else if (side[i] == '+' || side[i] == '*' || side[i] == '-' || side[i] == '&' || side[i] == '|' ||
                   side[i] == ',' || side[i] == '(' || side[i] == ')') {
            ch = false;
            sp = false;
        } else {
            *error = true;
            return "";
        }
    }
    char *equation = malloc(strlen(side)-space); //+1 falan olabilir
    equation[0] = '\0';
    char *p = strtok(side, " ");
    while (p != NULL) {
        strcat(equation, p);
        p = strtok(NULL, " ");
    }
    return equation;
}
