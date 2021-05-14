#include <stdio.h>
#include <stdlib.h>
#include <time.h>


void random_input(int array[], int length){
    srand(time(0));
    for(int i = 0; i < 20; i++){
        array[i] = rand() % 100 + 1;
        // printf("%d\n", array[i]);
    }
}

void Bubble(int array[], int length){
    int temp = 0;
    for(int i = length - 1; i >= 0 ; i--)
        for(int j = 0; j < i ; j++)
            if(array[j] > array[j+1]){  // > : 오름차순 / < : 내림차순 /
                temp = array[j];
                array[j] = array[j+1];
                array[j+1] = temp;
            }
        
    
}

void print(int array[], int length){
    for(int i = 0; i < length; i++)
        printf("%d  ",array[i]);
}

int main(){
    int num[20];

    random_input(num, sizeof(num) / 4);
    Bubble(num, sizeof(num) / 4);
    print(num, sizeof(num) / 4);

    return 0;
}