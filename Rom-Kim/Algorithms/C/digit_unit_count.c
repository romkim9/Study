#include <stdio.h>

int main(){
    int tmp = 11;
    int n = 100;

    for(int i = 10; i <= 100000; i++){
        if(i == n){
            tmp += n;
            n *= 10;
            printf("\n");
        }

        if(i % tmp == 0) printf("%d \t", i);

    }

    return 0;
}