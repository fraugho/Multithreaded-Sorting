#include <iostream>
#include <thread>
#include <chrono> //remember kids never ever write your own time libraries
#include <random>
#include <climits>
#include <algorithm>
#include <fstream>
#include "algorithms.h"

int main() {
    const int n = 10000; // Size of the array to be sorted
    const int num_trials = 10; // Number of trials to average
    int num_threads = 2; // Number of threads for multi-threaded sorting

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(1, n);

    double time_bubble_sort = 0.0, time_multi_bubble_sort = 0.0;
    double time_heap_sort = 0.0, time_multi_heap_sort = 0.0;
    double time_cpp_sort = 0.0, time_multi_cpp_sort = 0.0;
    double time_merge_sort = 0.0, time_multi_merge_sort;

    for (int trial = 0; trial < num_trials; ++trial) {
        int arr1[n], arr2[n], arr3[n], arr4[n], arr5[n], arr6[n], arr7[n], arr8[n];
        for (int i = 0; i < n; i++) {
            int num = distrib(gen);
            arr1[i] = arr2[i] = arr3[i] = arr4[i] = arr5[i] = arr6[i] = arr7[i] = arr8[i] = num;
        }
        time_bubble_sort += multiThread(bubSort,arr1,10000, false);
        time_multi_bubble_sort += multiThread(bubSort,arr2,10000,true);
        
        time_heap_sort += multiThread(heapSort,arr2,10000,false);
        time_multi_heap_sort += multiThread(heapSort,arr2,10000,true);

        time_cpp_sort += multiThread(cppSort,arr2,10000,false);
        time_multi_cpp_sort += multiThread(cppSort,arr2,10000,true);

        time_merge_sort += multiThread(mergeSort,arr2,10000,false);
        time_multi_merge_sort += multiThread(mergeSort,arr2,10000,true);
        
    }

    std::cout << "Average Non-multi-threaded Bubble Sort Time: " << time_bubble_sort / num_trials << " seconds.\n";
    std::cout << "Average Multi-threaded Bubble Sort Time: " << time_multi_bubble_sort / num_trials << " seconds.\n";
    std::cout << "Average Non-multi-threaded Heap Sort Time: " << time_heap_sort / num_trials << " seconds.\n";
    std::cout << "Average Multi-threaded Heap Sort Time: " << time_multi_heap_sort / num_trials << " seconds.\n";
    std::cout << "Average Non-multi-threaded C++ Sort Time: " << time_cpp_sort / num_trials << " seconds.\n";
    std::cout << "Average Multi-threaded C++ Sort Time: " << time_multi_cpp_sort / num_trials << " seconds.\n";
    std::cout << "Average Non-multi-threaded Merge Sort Time: " << time_merge_sort / num_trials << " seconds.\n";
    std::cout << "Average Multi-threaded Merge Sort Time: " << time_multi_merge_sort / num_trials << " seconds.\n";

    return 0;
}
