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
            //sanırım eşittir yoksa burada hata veriyor ama hiiç emin değilim
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
            //burada değişkenle ilgili işimiz bitti, RHS kodunu artık buraya mı çekeriz orasını bilmiyorum
            int i = 0;
            int length = strlen(left); //buna -1 koymak da gerekebilir, kafam karıştı
            while (left[i] == ' ') {
                i++;
                if (i == length-1) {
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
            if (strcmp(variable,"xor")==0||strcmp(variable,"ls")==0||strcmp(variable,"rs")==0||strcmp(variable,"lr")==0||
                    strcmp(variable,"rr")==0||strcmp(variable,"not")==0) {
                printf("Error7!\n");
                continue;
            }
            if (error) {
                continue;
            }
        }
        if (right == NULL) {
            //RHS İÇİN YAPTIĞIMIZ ŞEYİN AYNISI YAPILACAK
            //TEK FARK BURADA PRINT, ORADA ASSIGN
            bool isDigit = true;
            for (int j=0; j<strlen(variable)-1; j++) {
                if (!(isdigit(variable[j]))) {
                    isDigit = false;
                }
            }
            if (isDigit) {
                printf("%s\n", variable);
                continue;
            } else {
                //değerini print
                continue;
            }
        } else {
            //assignment case
        }

        //bundan sonrası stack kısımları


        //char boşluk char sekansında error
        //sembol, reserved word ve sayı varsa error
        //BAŞKA CASE VAR MI DÜŞÜN





        //RHS
        int k = 0;
        int length2 = strlen(right);
        while (right[k] == ' ') {
            k++;
            if (k==length2-1) { //burada length-1 vs durumlar da olabilir
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
            int z=0;
            int length3 = strlen(nsRight);
            while (*p != '\n') {
                char *item = malloc(length3+1 * sizeof(char));
                item[0] = '\0';
                int a=0;
                while (isalpha(*p)) {
                    item[a++] = *p;
                    p++;
                }
                if (a>0) {
                    item[a] = '\0';
                    if (strcmp(item,"xor") == 0) {
                        char *p2 = p;
                        p2++;
                        if (strcmp(p2,"(") != 0) {
                            printf("Error13!\n");
                            error = true;
                            break;
                        } else {
                            //operator listesine ekleme yap
                        }
                    } else if (strcmp(item,"not") == 0){
                        char *p2 = p;
                        p2++;
                        if (strcmp(p2,"(") != 0) {
                            printf("Error9!\n");
                            error = true;
                            break;
                        } else {
                            //operator listesine ekleme yap
                        }
                    } else if (strcmp(item,"lr") == 0) {
                        char *p2 = p;
                        p2++;
                        if (strcmp(p2,"(") != 0) {
                            printf("Error10!\n");
                            error = true;
                            break;
                        } else {
                            //operator listesine ekleme yap
                        }
                    } else if (strcmp(item,"rr") == 0){
                        char *p2 = p;
                        p2++;
                        if (strcmp(p2,"(") != 0) {
                            printf("Error11!\n");
                            error = true;
                            break;
                        } else {
                            //operator listesine ekleme yap
                        }
                    }
                    else if (strcmp(item,"rs") == 0){
                        char *p2 = p;
                        p2++;
                        if (strcmp(p2,"(") != 0) {
                            printf("Error12!\n");
                            error = true;
                            break;
                        } else {
                            //operator listesine ekleme yap
                        }
                    }
                    else if (strcmp(item,"ls") == 0){
                        char *p2 = p;
                        p2++;
                        if (strcmp(p2,"(") != 0) {
                            printf("Error8!\n");
                            error = true;
                            break;
                        } else {
                        //operator listesine ekleme yap
                        }
                    } else {
                        //o zaman variable
                        char *p2 = p;
                        p2++;
                        if (strcmp(p2,"(") == 0) {
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
                }
                a=0;
                while (isdigit(*p)) {
                    item[a++] = *p;
                    p++;
                }
                if (a>0) {
                    item[a] = '\0';
                    array[z] = item;
                    z++;
                    //sonra operator olmalı
                    //sayıyı gideceği yere gönder
                }
                else if (*p == '+' || *p == '*' || *p == '-' || *p == '&' || *p == '|' || *p == '(' || *p == ')') {
                    printf("operator");
                    p++;
                    //tek ifade zaten
                    //sonra yine operator varsa error (büyük ihtimalle string kısmında yapacağız)
                    //burada tam olarak gideceği yere gönderme hareketinde bulunamayız
                    //shunting-yard'a göre yapacağız bunu
                    //sonra char veya sayı olmalı
                    //gideceği yere gönder
                }
                 if (*p == ',') {
                     printf("virgul");
                     p++;
                     //error galiba
                     //xor için ayrı komutlar olacak, o yüzden buraya virgül gelemez, ama sen yine de bunun yaşanmamasını sağla
                 }
            }
            //for (int x=0; x<256; x++) {
            //    printf("%s\n", array[x]);
            //}
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
