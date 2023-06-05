#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <stdio.h>
using namespace std;

struct Files {
    int id, size;
};

struct Pendrive {
    int sum = 0, amount = 0;
    Files* pack;
};

int compareSizes(Files a, Files b) {

    if (a.size == b.size)
        return 0;

    if (a.size < b.size)
        return 1;
    return -1;

}

int compareIds(Files a, Files b) {

    if (a.id == b.id)
        return 0;

    if (a.id > b.id)
        return 1;
    return -1;

}

int partition(Files* array, int left, int right, int (*compare)(Files, Files)) {

    Files pivot = array[left];

    int left_p = left - 1;
    int right_p = right + 1;


    while (1) {

        while (compare(array[++left_p], pivot) > 0) {}

        while (compare(array[--right_p], pivot) < 0) {}

        if (left_p < right_p) {
            Files tmp = array[left_p];
            array[left_p] = array[right_p];
            array[right_p] = tmp;
        }
        else {
            return right_p;
        }
    }


}

void sort(Files* array, int left, int right, int (*compare)(Files, Files)) {

    if (left < right) {
        int q = partition(array, left, right, compare);
        sort(array, left, q, compare);
        sort(array, q + 1, right, compare);
    }


}


int main()
{
    int n, p;

    scanf("%i%i", &n, &p);


    Files* files = new Files[n];

    for (int i = 0; i < n; i++) {

        scanf("%i%i", &files[i].id, &files[i].size);

    }

    sort(files, 0, n - 1, &compareSizes);

    Pendrive pendrives[2];
    pendrives[0].pack = new Files[n];
    pendrives[1].pack = new Files[n];

    for (int i = 0, j = 0; i < n && j < 2; i++) {

        if (files[i].size + pendrives[j].sum <= p) {

            pendrives[j].sum += files[i].size;
            pendrives[j].amount++;

            pendrives[j].pack[pendrives[j].amount-1] = files[i];
        }
        else {
            j++;
            i--;
        }

    }

    delete[] files;

    sort(pendrives[0].pack, 0, pendrives[0].amount - 1, &compareIds);
    sort(pendrives[1].pack, 0, pendrives[1].amount - 1, &compareIds);

    printf("%i\n", pendrives[0].amount + pendrives[1].amount);

    for (int i = 0; i < 2; i++) {
        printf("%i:", i + 1);
        for (int j = 0; j < pendrives[i].amount; j++) {
            printf(" %i", pendrives[i].pack[j].id);
        }
        printf("\n");
    }

}
