#include <stdio.h>

typedef struct {
    char data[4096];
    int key;
} item;

item array[] = {
    {"bill", 3},
    {"neil", 4},
    {"john", 2},
    {"rick", 5},
    {"alex", 1},
};

void sort(item *a, int n) {
    int op=1, j, i = n;

    while(i>0 && op!=0)
    {
        op=0; i--;

        for(j=0; j <= i; j++)
        {
            if(a[j+1].key < a[j].key){
                item aux = a[j];
                a[j]=a[j+1];
                a[j+1]=aux;      

                ++op;
            }
        }
    }
}

int main() {
    int i;
    sort(array,5);
    for(i = 0; i < 5; i++)
        printf("array[%d] = {%s, %d}\n",
                i, array[i].data, array[i].key);
    return 0;
}
