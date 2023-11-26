#pragma once
#include <iostream>
#include <stack>
#include <thread>
#include <vector>
#include <algorithm>

void heapify(int arr[], int n, int i);
int partition(int arr[], int low, int high);

void swap(int* a, int* b) {
    int t = *a;
    *a = *b;
    *b = t;
}

void bubble_sort(int array[], int start, int end) {
    int temp;
    for (int i = start; i < end; i++) {
        for (int j = start; j < end - (i - start) - 1; j++) {
            if (array[j] > array[j + 1]) {
                temp = array[j];
                array[j] = array[j+1];
                array[j+1] = temp;
            }
        }
    }
}

void multi_threaded_bubble_sort(int array[], int size, int num_threads) {
    std::vector<std::thread> threads;
    int chunk_size = size / num_threads;

    for (int i = 0; i < num_threads; ++i) {
        int start = i * chunk_size;
        int end = (i + 1) * chunk_size;

        if (i == num_threads - 1) {
            end = size;
        }

        threads.emplace_back(bubble_sort, array, start, end);
    }

    for (auto &thread : threads) {
        thread.join();
    }

    bubble_sort(array, 0, size);
}

void merge(int arr[], int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    std::vector<int> L(n1), R(n2);

    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;

    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

void mergeSort(int arr[], int left, int right) {
    if (left >= right)
        return;

    int mid = left + (right - left) / 2;
    mergeSort(arr, left, mid);
    mergeSort(arr, mid + 1, right);
    merge(arr, left, mid, right);
}

void mergeSortParallel(int arr[], int left, int right) {
    if (left >= right) return;

    int mid = left + (right - left) / 2;

    std::thread t1(mergeSort, arr, left, mid);
    std::thread t2(mergeSort, arr, mid + 1, right);

    t1.join();
    t2.join();

    merge(arr, left, mid, right);
}

void heapify(int arr[], int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && arr[left] > arr[largest])
        largest = left;

    if (right < n && arr[right] > arr[largest])
        largest = right;

    if (largest != i) {
        swap(&arr[i], &arr[largest]);
        heapify(arr, n, largest);
    }
}

void buildMaxHeap(int arr[], int n) {
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify(arr, n, i);
    }
}

void heapSort(int arr[], int n) {
    buildMaxHeap(arr, n);

    for (int i = n - 1; i > 0; i--) {
        swap(&arr[0], &arr[i]);
        heapify(arr, i, 0);
    }
}

void buildMaxHeapSegment(int arr[], int n, int start, int end) {
    for (int i = end; i >= start; i--) {
        heapify(arr, n, i);
    }
}

void buildMaxHeapParallel(int arr[], int n) {
    int mid = n / 4;

    std::thread t1(buildMaxHeapSegment, arr, n, 0, mid);
    std::thread t2(buildMaxHeapSegment, arr, n, mid + 1, n / 2 - 1);

    t1.join();
    t2.join();

    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify(arr, n, i);
    }
}

void heapSortParallel(int arr[], int n) {
    buildMaxHeapParallel(arr, n);

    for (int i = n - 1; i > 0; i--) {
        swap(&arr[0], &arr[i]);
        heapify(arr, i, 0);
    }
}

int partition(int arr[], int low, int high) {
    int pivot = arr[high];
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

void iterativeQuickSort(int arr[], int n) {
    std::stack<int> s;
    s.push(0);
    s.push(n - 1);

    while (!s.empty()) {
        int h = s.top();
        s.pop();
        int l = s.top();
        s.pop();

        int p = partition(arr, l, h);

        if (p - 1 > l) {
            s.push(l);
            s.push(p - 1);
        }
        if (p + 1 < h) {
            s.push(p + 1);
            s.push(h);
        }
    }
}

void quicksort_thread(int arr[], int start, int end);
void final_pass_quicksort(int arr[], int n, int num_threads) {
    int chunk_size = n / num_threads;
    std::vector<int> boundary_indices;

    for (int i = 1; i < num_threads; ++i) {
        int boundary = i * chunk_size;
        if (arr[boundary - 1] > arr[boundary]) {
            boundary_indices.push_back(boundary);
        }
    }

    for (int boundary : boundary_indices) {
        int start = boundary - 1;
        while (start > 0 && arr[start - 1] > arr[start]) {
            --start;
        }

        int end = boundary;
        while (end < n - 1 && arr[end] > arr[end + 1]) {
            ++end;
        }

        quicksort_thread(arr, start, end);
    }
}

void quicksort_thread(int arr[], int start, int end) {
    std::stack<int> s;
    s.push(start);
    s.push(end);

    while (!s.empty()) {
        int h = s.top();
        s.pop();
        int l = s.top();
        s.pop();

        int p = partition(arr, l, h);

        if (p - 1 > l) {
            s.push(l);
            s.push(p - 1);
        }
        if (p + 1 < h) {
            s.push(p + 1);
            s.push(h);
        }
    }
}

void multi_threaded_quick_sort(int arr[], int n, int num_threads) {
    std::vector<std::thread> threads;
    int chunk_size = n / num_threads;

    for (int i = 0; i < num_threads; ++i) {
        int start = i * chunk_size;
        int end = (i + 1) * chunk_size - 1;

        if (i == num_threads - 1) {
            end = n - 1;
        }

        threads.emplace_back(quicksort_thread, arr, start, end);
    }

    for (auto &thread : threads) {
        thread.join();
    }

    final_pass_quicksort(arr, n, num_threads);
}

void printArray(int arr[], int n) {
    for (int i = 0; i < n; ++i)
        std::cout << arr[i] << " ";
    std::cout << "\n";
}
