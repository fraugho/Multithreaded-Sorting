#include <chrono>
#include <iostream>
#include <random>
#include "algorithms.h"


int main() {
    const int n = 10000; // Size of the array to be sorted
    const int num_trials = 100; // Number of trials to average
    int num_threads = 2; // Number of threads for multi-threaded sorting

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(1, n);

    double time_bubble_sort = 0.0, time_multi_bubble_sort = 0.0;
    double time_heap_sort = 0.0, time_multi_heap_sort = 0.0;
    double time_quick_sort = 0.0, time_multi_quick_sort = 0.0;
    double time_merge_sort = 0.0, time_multi_merge_sort;

    for (int trial = 0; trial < num_trials; ++trial) {
        int arr1[n], arr2[n], arr3[n], arr4[n], arr5[n], arr6[n], arr7[n], arr8[n];
        for (int i = 0; i < n; i++) {
            int num = distrib(gen);
            arr1[i] = arr2[i] = arr3[i] = arr4[i] = arr5[i] = arr6[i] = arr7[i] = arr8[i] = num;
        }

        auto start = std::chrono::high_resolution_clock::now();
        bubble_sort(arr1, 0, n);
        auto end = std::chrono::high_resolution_clock::now();
        time_bubble_sort += std::chrono::duration<double>(end - start).count();

        start = std::chrono::high_resolution_clock::now();
        multi_threaded_bubble_sort(arr2, n, num_threads);
        //multi_threaded_bubble_sort(arr2, n, 4);
        end = std::chrono::high_resolution_clock::now();
        time_multi_bubble_sort += std::chrono::duration<double>(end - start).count();

        start = std::chrono::high_resolution_clock::now();
        heapSort(arr3, n);
        end = std::chrono::high_resolution_clock::now();
        time_heap_sort += std::chrono::duration<double>(end - start).count();

        start = std::chrono::high_resolution_clock::now();
        heapSortParallel(arr4, n);
        end = std::chrono::high_resolution_clock::now();
        time_multi_heap_sort += std::chrono::duration<double>(end - start).count();

        start = std::chrono::high_resolution_clock::now();
        iterativeQuickSort(arr5, n);
        end = std::chrono::high_resolution_clock::now();
        time_quick_sort += std::chrono::duration<double>(end - start).count();

        start = std::chrono::high_resolution_clock::now();
        multi_threaded_quick_sort(arr6, n, num_threads);
        //multi_threaded_quick_sort(arr6, n, 4);
        end = std::chrono::high_resolution_clock::now();
        time_multi_quick_sort += std::chrono::duration<double>(end - start).count();

        start = std::chrono::high_resolution_clock::now();
        mergeSort(arr7, 0, n);
        end = std::chrono::high_resolution_clock::now();
        time_merge_sort += std::chrono::duration<double>(end - start).count();

        start = std::chrono::high_resolution_clock::now();
        mergeSortParallel(arr8, 0, n);
        end = std::chrono::high_resolution_clock::now();
        time_multi_merge_sort += std::chrono::duration<double>(end - start).count();
    }

    std::cout << "Average Non-multi-threaded Bubble Sort Time: " << time_bubble_sort / num_trials << " seconds.\n";
    std::cout << "Average Multi-threaded Bubble Sort Time: " << time_multi_bubble_sort / num_trials << " seconds.\n";
    std::cout << "Average Non-multi-threaded Heap Sort Time: " << time_heap_sort / num_trials << " seconds.\n";
    std::cout << "Average Multi-threaded Heap Sort Time: " << time_multi_heap_sort / num_trials << " seconds.\n";
    std::cout << "Average Non-multi-threaded Quick Sort Time: " << time_quick_sort / num_trials << " seconds.\n";
    std::cout << "Average Multi-threaded Quick Sort Time: " << time_multi_quick_sort / num_trials << " seconds.\n";
    std::cout << "Average Non-multi-threaded Merge Sort Time: " << time_merge_sort / num_trials << " seconds.\n";
    std::cout << "Average Multi-threaded Merge Sort Time: " << time_multi_merge_sort / num_trials << " seconds.\n";

    return 0;
}
/*
int main() {
    const int n = 10000; // Size of the array to be sorted
    int num_threads = 2; // Number of threads for multi-threaded sorting

    // Generating a random array
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(1, n);

    int arr1[n], arr2[n], arr3[n], arr4[n], arr5[n], arr6[n];
    for (int i = 0; i < n; i++) {
        int num = distrib(gen);
        arr1[i] = arr2[i] = arr3[i] = arr4[i] = arr5[i] = arr6[i] = num;
    }

    // Non-multi-threaded Bubble Sort
    auto start = std::chrono::high_resolution_clock::now();
    bubble_sort(arr1, 0, n);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Non-multi-threaded Bubble Sort took " << elapsed.count() << " seconds.\n";

    // Multi-threaded Bubble Sort
    start = std::chrono::high_resolution_clock::now();
    multi_threaded_bubble_sort(arr2, n, num_threads);
    end = std::chrono::high_resolution_clock::now();
    elapsed = end - start;
    std::cout << "Multi-threaded Bubble Sort took " << elapsed.count() << " seconds.\n";

    // Non-multi-threaded Heap Sort
    start = std::chrono::high_resolution_clock::now();
    heapSort(arr3, n);
    end = std::chrono::high_resolution_clock::now();
    elapsed = end - start;
    std::cout << "Non-multi-threaded Heap Sort took " << elapsed.count() << " seconds.\n";

    // Multi-threaded Heap Sort
    start = std::chrono::high_resolution_clock::now();
    heapSortParallel(arr4, n);
    end = std::chrono::high_resolution_clock::now();
    elapsed = end - start;
    std::cout << "Multi-threaded Heap Sort took " << elapsed.count() << " seconds.\n";

    // Non-multi-threaded Quick Sort
    start = std::chrono::high_resolution_clock::now();
    iterativeQuickSort(arr5, n);
    end = std::chrono::high_resolution_clock::now();
    elapsed = end - start;
    std::cout << "Non-multi-threaded Quick Sort took " << elapsed.count() << " seconds.\n";

    // Multi-threaded Quick Sort
    start = std::chrono::high_resolution_clock::now();
    multi_threaded_quick_sort(arr6, n, num_threads);
    end = std::chrono::high_resolution_clock::now();
    elapsed = end - start;
    std::cout << "Multi-threaded Quick Sort took " << elapsed.count() << " seconds.\n";

    return 0;
}
*/

/*
int main() {
    const int n = 10000; // Size of the array to be sorted
    int num_threads = 2; // Number of threads for multi-threaded sorting

    // Generating a random array
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(1, n);

    int arr1[n], arr2[n], arr3[n];
    for (int i = 0; i < n; i++) {
        arr1[i] = arr2[i] = arr3[i] = distrib(gen);
    }

    // Multi-threaded Bubble Sort
    auto start = std::chrono::high_resolution_clock::now();
    multi_threaded_bubble_sort(arr1, n, num_threads);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Multi-threaded Bubble Sort took " << elapsed.count() << " seconds.\n";

    // Multi-threaded Heap Sort
    start = std::chrono::high_resolution_clock::now();
    heapSortParallel(arr2, n);
    end = std::chrono::high_resolution_clock::now();
    elapsed = end - start;
    std::cout << "Multi-threaded Heap Sort took " << elapsed.count() << " seconds.\n";

    // Multi-threaded Quick Sort
    start = std::chrono::high_resolution_clock::now();
    multi_threaded_quick_sort(arr3, n, num_threads);
    end = std::chrono::high_resolution_clock::now();
    elapsed = end - start;
    std::cout << "Multi-threaded Quick Sort took " << elapsed.count() << " seconds.\n";

    return 0;
}
*/