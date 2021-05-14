#define SIZE 6
#include <stdio.h>

void selection(int index[], int length){
    int small;
    int temp = 0;
    int array_num = 0;

    for(int i = 0; i<length; i++){
        small = index[i];
        array_num = i;

        for(int j = i; j<length; j++)
            if(index[j] < small ){
                small = index[j];
                array_num = j;
            }

        temp = index[i];
        index[i] = index[array_num];
        index[array_num] = temp;
    }
}

int main(){
    int index[SIZE] = {5, 3, 8, 1, 2, 7};
    selection(index, SIZE);

    for(int i = 0; i<SIZE; i++){
        printf("%d\n", index[i]);
    }
}

