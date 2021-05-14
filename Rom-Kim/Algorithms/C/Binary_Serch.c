#include <stdio.h>

int Binary_Serch(int index[], int length, int key){
    int low = 0;
    int high = length-1;
    int mid;

    while(low<=high){
        mid = (low + high) / 2;
    
        if(index[mid] == key) 
            return key;
        
        else if(key > index[mid])
            low = mid+1;
        
        else 
            high = mid-1;
    }
    return -1;
}

int main(){
    int index[16] = {2, 6, 11, 13, 18, 20, 22, 27, 29, 30, 34, 38, 41, 42, 45, 47};
    int key;
    int result;
    
    printf("key 값을 입력해주세요 : ");
    scanf("%d", &key);

    result = Binary_Serch(index, sizeof(index)/4,key);

    printf("%d\n", result);

    return 0;
}