#include <iostream>
#include <vector>

int partition(std::vector<int> &arr, int low, int high)
{
    int base = arr[high];
    int i = low - 1;
    for (int j = low; j < high; j++)
    {
        if (arr[j] < base)
            std::swap(arr[++i], arr[j]);
    }
    std::swap(arr[i + 1], arr[high]);
    return i + 1;
}

void quickSort(std::vector<int> &arr, int low, int high)
{
    if (low < high)
    {
        int idx = partition(arr, low, high);
        quickSort(arr, low, idx - 1);
        quickSort(arr, idx + 1, high);
    }
}

int main()
{
    std::vector<int> arr = {10, 7, 8, 9, 1, 5};

    std::cout << "before: ";
    for (auto it : arr)
        std::cout << it << " ";
    std::cout << std::endl;
    quickSort(arr, 0, arr.size() - 1);
    std::cout << "after : ";
    for (auto it : arr)
        std::cout << it << " ";
    std::cout << std::endl;
    return 0;
}