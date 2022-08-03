//Дана последовательность целых чисел a1...an и натуральное число k, такое что для любых i, j: если j >= i + k, то a[i] <= a[j].
//Требуется отсортировать последовательность.
//Последовательность может быть очень длинной. Время работы O(n * log(k)). Доп. память O(k). Использовать слияние.
#include <iostream>
#include <cstring>
void Merge(int *array, int const first_len, int const second_len) {
//    создаем массив размера k и перекладываем из него элементы в наш массив
    int buffer_array [second_len];
    memcpy(buffer_array, array+first_len, sizeof(int)*(second_len));
    int i = first_len -  1  , j = second_len - 1;
    int end = first_len + second_len -1 ;
    while (j >= 0) {
        if (i >= 0 && array[i] > buffer_array[j]) {
            array[end] = array[i];
            i--;
        } else {
            array[end] = buffer_array[j];
            j--;
        }
        end--;
    }
}
void MergeSort(int * unsorted_arr, int begin_ind, int end_ind) {
    if (begin_ind < end_ind) {
        //вызываем сортировки для обеих частей
        MergeSort(unsorted_arr, begin_ind, (begin_ind+end_ind)/2);
        MergeSort(unsorted_arr, (begin_ind+end_ind)/2+1, end_ind);
        // мерджим части
        Merge(unsorted_arr + begin_ind, 1+(begin_ind+end_ind)/2 - begin_ind, end_ind - (begin_ind+end_ind)/2);
    }
};
int main() {
    int n, k;
    std::cin >> n >> k;
    int * numbers = new int [n];
    for (int number = 0, i = 0; i < n; ++i) {
        std::cin >> number;
        numbers[i] = number;
    }

    MergeSort(numbers,0, k-1);
    for (int i = k; i < n; i += k) {
        MergeSort(numbers,i, std::min(i+ k-1, n-1));
        Merge(numbers + i - k, k, std::min(k,n-i));
    }

    for (int i = 0; i < n; ++i) {
        std::cout << numbers[i] << ' ';
    }
    delete [] numbers;
    return 0;
}
