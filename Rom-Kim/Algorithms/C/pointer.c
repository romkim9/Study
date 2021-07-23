#include <stdio.h>
int main(){
    int a = 10, b = 10, c = 10, d = 10;
    int tmp;
    int *e = &a, *f = &b, *g = &c, *h = &d;

    printf("a : %d / &a = : %d / e : %d / *e = : %d\n", a, &a, e, *e);
    tmp = *e++;

    printf("a : %d / &a = : %d / e : %d / *e = : %d/ tmp = %d\n", a, &a, e, *e, tmp);

    
    printf("b : %d / &b = : %d / f : %d / *f = : %d\n", b, &b, f, *f);
    tmp = (*f)++;
    printf("b : %d / &b = : %d / f : %d / *f = : %d / tmp = %d\n", b, &b, f, *f, tmp); 


    printf("c : %d / &c = : %d / g : %d / *g = : %d\n", c, &c, g, *g);
    tmp = *++g;
    printf("c : %d / &c = : %d / g : %d / *g = : %d / tmp = %d\n", c, &c, g, *g, tmp);

    printf("d : %d / &a = : %d / h : %d / *h = : %d\n", d, &d, h, *h);
    tmp = ++*h;
    printf("d : %d / &a = : %d / h : %d / *h = : %d / tmp = %d", d, &d, h, *h, tmp);

    return 0;
}