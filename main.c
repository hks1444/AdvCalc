#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>

char* parseAfterLeftStrip (int i, char *side, bool *error);

int main() {
    //yan yana char ve sayı durumunu handle'la
        //isalphanum yazan kısmı düzenle
    //error çözümlerini elinden geldiğince stack'e bırakma
    //sondaki parçalama kısmında operatorlarla ilgili sorun var

    //TODO ERRORS
    //açık parantez (stacke de salarsam benim kod döngüden çıkmaz)
    //EŞİTTİRSİZ DURUMLARDA KOD YOK, O YÜZDEN ŞİMDİLİK SAL
    //neyden sonra ne geleceğine göre yapmamız lazım sanırım
    //tokenleme kısmından sonra
    //string kısmında boşluk vs sorunları çöz
    //xor (a),c (bunu stack çözebilir)
    //=xor(a,c)
    //xor(a,b,c)x*y
    // > printlemeye bak
    //  = olunca sorun yok
    //boş inputta right==null kısmı çalışmamalı
    //='den sonra hiçbir şey yoksa error ver

    char line[256];
    while (fgets(line, sizeof(line), stdin)) {
        char *left;
        char *right;
        char *third;
        bool error = false;
        if (line == NULL) { //bu niye böyle anlamadım, hoca böyle yazdı
            break;
        } else {
            strtok(line, "%");
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
            //SOL TARAFI OKUMA VE HATA KONTROL ETME KISMI BURASI
            int i = 0;
            int length = strlen(left); //buna -1 koymak da gerekebilir, kafam karıştı
            while (left[i] == ' ') {
                i++;
                if (i == length - 1) {
                    printf("Error2!\n");
                    error = true;
                    break;
                }
            }
            if (error) {
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
                    printf("Error6!\n");
                    break;
                }
                po++;
            }
            if (error) {
                continue;
            }
            if (strcmp(variable, "xor") == 0 || strcmp(variable, "ls") == 0 || strcmp(variable, "rs") == 0 ||
                strcmp(variable, "lr") == 0 ||
                strcmp(variable, "rr") == 0 || strcmp(variable, "not") == 0) {
                printf("Error7!\n");
                continue;
            }
            if (error) {
                continue;
            }
            //BUNDAN SONRA SAĞ TARAFIN KODU GELECEK
            int k = 0;
            int length2 = strlen(right);
            while (right[k] == ' ') {
                k++;
                if (k == length2 - 1) { //burada length-1 vs durumlar da olabilir
                    printf("Error4!\n");
                    error = true;
                    break;
                }
            }
            if (error) {
                continue;
            }

            //burada ilk şey operator olamaz
            //buradaki atama nasıl olacak
            char *nsRight = parseAfterLeftStrip(k, right, &error);
            if (error) {
                printf("Error5!\n");
                continue;
            } else {
                //burada right'ı traverse edip tek tek bileşenlerine ayıracağız
                char *p = nsRight;
                char *array[256]; //YOK EDİLECEK
                int z = 0; //YOK EDİLECEK
                int type = 0; //0=start / 1=string / 2=number / 3=operator / 4=function
                //4'ten sonra parantez olduğunu zaten kesinlikle belirttik
                int length3 = strlen(nsRight);
                while (*p != '\n') {
                    char *item = malloc(length3 + 1 * sizeof(char));
                    item[0] = '\0';
                    int a = 0;
                    while (isalpha(*p)) {
                        if (type == 1 || type == 2) {
                            a=0;
                            error = true;
                            printf("Error19!\n");
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
                                printf("Error13!\n");
                                error = true;
                                break;
                            } else {
                                //burada hata kontrolü yapmamız gerekiyor ama şimdi yapacağım kontrol bunu başaramazsa yapacağım
                                //şu anki durumda parantez sayma olayının işe yarayabileceğine inanıyorum
                                //operator listesine ekleme yap
                                char *p2 = p;
                                p2++;
                                int parenthesis = 1;
                                int comma = 0;
                                while (parenthesis > 0) {
                                    //buraya bir yere string bittiyse ama parenthesis 0 değilse error ekle
                                    //eğer bu kodu parantez yerine fonksiyon için yaparsan garantili olmuş olur ama böyle de fena değil sanki
                                    // xor(a),c BUNU ÇÖZMEMİZ LAZIM
                                    // x = xor(a,ls(c)) BUNU DA
                                    if (*p2 == '(') {
                                        parenthesis++;
                                    } else if (*p2 == ',') {
                                        comma++;
                                    }
                                    if (comma > parenthesis) {
                                        printf("Error22!\n");
                                        error = true;
                                        break;
                                    }
                                    if (*p2 == ')') {
                                        parenthesis--;
                                        comma--;
                                    }
                                    p2++;
                                }
                            }
                        } else {
                            //o zaman variable
                            type = 1;
                            if (*p == '(') {
                                printf("Error14!\n");
                                error = true;
                                break;
                            } else {
                                //sayı listesine ekleme yap
                            }
                        }
                        if (error) {
                            continue;
                        }
                        array[z] = item;
                        z++;
                        continue;
                    }
                    //a = 0;
                    while (isdigit(*p)) {
                        if (type == 1 || type == 2) {
                            a=0;
                            error = true;
                            printf("Error15\n");
                            break;
                        }
                        item[a++] = *p;
                        p++;
                    }
                    if (a > 0) {
                        type = 2;
                        item[a] = '\0';
                        array[z] = item;
                        z++;
                        continue;
                    }
                    while (*p == '(') {
                        if (type == 1 || type == 2 || type == 6) {
                            a=0;
                            error = true;
                            printf("Error21!\n");
                            break;
                        }
                        item[a++] = *p;
                        p++;
                        break;
                    }
                    if (a>0) {
                        type = 5;
                        item[a] = '\0';
                        array[z] = item;
                        z++;
                        continue;
                    }
                    while (*p == ')') {
                        if (type == 0 || type == 3 || type == 5) {
                            a=0;
                            error = true;
                            printf("Error17!\n");
                            break;
                        }
                        item[a++] = *p;
                        p++;
                        break;
                    }
                    if (a>0) {
                        type = 6;
                        item[a] = '\0';
                        array[z] = item;
                        z++;
                        continue;
                    }
                    while (*p == '+' || *p == '*' || *p == '-' || *p == '&' || *p == '|' || *p == ',') {
                        if (type == 0 || type == 3) {
                            a=0;
                            error = true;
                            printf("Error20!\n");
                            break;
                        }
                        item[a++] = *p;
                        p++;
                    }
                    if (a==1) {
                        type = 3;
                        item[a] = '\0';
                        array[z] = item;
                        z++;
                        continue;
                    } else if (a!=1) {
                        printf("Error!16\n");
                        error =true;
                    }
                    if (error) {
                        continue;
                    }
                }
                for (int x = 0; x < 256; x++) {
                    printf("%s\n", array[x]);
                }
            }
        }


        if (right == NULL) {
            printf("no equal sign");
            //DİĞER BLOKTAKİ KOD BİTİNCE BURAYA DA GELECEK
            //!!! RIGHT VE ONUNLA ALAKALI HER ŞEYİ DEĞİŞTİRMEMİZ LAZIM

            //RHS'DE BURADA YAPTIĞIMIZ ŞEYİN AYNISI YAPILACAK
            //TEK FARK BURADA SONUÇ PRINT, ORADA ASSIGN

        }
    }
}
//boş string returnunu değerlendirmemiz lazım
//error da olabilir, düz devam da
char* parseAfterLeftStrip (int i, char *side, bool *error) { //buradaki error pointer vb. sebeplerden ötürü yanlış olabilir
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



/* variable token olmayacaksa burası
char variable[256];
int a = i;
for (; left[i] != ' '; i++) {
    variable[i-a] = left[i];
}
//variable name oluştur
variable[i-a] = '\0';
printf("%s", variable);
*/
