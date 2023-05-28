#include<stdio.h>
#include<omp.h>

int main(){
    int arr[] = {64,99,12,53,72};
    int n = sizeof(arr)/sizeof(arr[0]);
    int i,j;
    int first;
    printf("Before Sort:\n");
    for(int i=0;i<n;i++){
        printf("%d ",arr[i]);
    }
    for(i=0;i<n-1;i++){
        first = i%2;
        #pragma omp parallel for shared(arr,n,first)
        for(j=first;j<n-1;j++){
            if(arr[j]>arr[j+1]){
                int temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
        }
    }


    printf("\n");
    printf("After Sorting:\n");
    for(int i=0;i<n;i++){
        printf("%d ",arr[i]);
    }
}