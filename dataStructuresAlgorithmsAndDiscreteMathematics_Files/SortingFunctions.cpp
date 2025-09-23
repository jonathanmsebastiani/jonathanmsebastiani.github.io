#include <iostream>

using namespace std;

//  bubble, insertion, merge, quick, and shell. You also need to
// know the best, worst, and average performance of each of the sorting methods
// (time and memory)

// Conclusion:
// Bubble Sort         worst and average O(n^2) / best O(n)                   In-place
// Insertion Sort      worst and average O(n^2) / best O(n)                   In-place
// Shell Sort          worst O(n^2) / average O(n^3/2) / best O(nlogn)        In-place
// Merge Sort          worst, average, and best O(n log n)                    n
// Quick Sort          worst O(n^2) / average  and best O(n log n)            n

/*
* Bubble Sort: (Moves the index, swapping once if needed)
* Worst case: O(n^2)
* Best case:  O(n)
* In place in memory
*
*/

/** Sorts the items in an array into ascending order.
 * Goes from front to back of array, swapping elements once if needed
 * and then moving onto the next index

@pre None.
@post theArray is sorted into ascending order; n is unchanged.
@param theArray, The given array. @param n The size of theArray. */
template<typename ItemType>
void bubbleSort(ItemType theArray[], int n)
{
    bool sorted = false;
    // False when swaps occur
    int pass = 1;
    while (!sorted && (pass < n))
    {
        // At this point, theArray[n+1-pass..n-1] is sorted
        // and all of its entries are › the entries in theArray[0..n-pass]
        sorted = true;
        // Assume sorted
        for (int index = 0; index < n - pass; index++)
        {
            // At this point, all entries in theArray[0..index-1]
            // are <= theArray[index]
            int nextIndex = index + 1;
            if (theArray[index] > theArray[nextIndex])
            {
                // Exchange entries
                swap(theArray[index], theArray[nextIndex]);
                sorted = false; 
            }
        }
        pass++;
    }
}

template<typename ItemType>
void swap(ItemType a, ItemType b)
{
    ItemType c = a;
    a = b;
    b = c;
}

template<typename T>
void swap(T &a, T &b)
{
  T holder = b;
  b = a;
  a = holder;
}

template<typename T>
void swap(T *a, T *b)
{
  T holder = *b;
  *b = *a;
  *a = holder;
}
/*
* Insertion Sort (Moves an element until sorted, then moves to next element)
* Worst Case: O(n^2)
* Best Case: O(n)
*/

// Description
/*
Insertion sort is a simple sorting algorithm that builds 
the final sorted array (or list) one item at a time by comparisons.
Holds onto the same item until it is sorted within the front(sorted) part
of the array
*/
void insertionSort(int a[], int n)
{
    for (int i = 1; i < n; i++)
    {
        int v = a[i];
        int j = i - 1;
        while (j >= 0 && a[j] > v)
        {
            a[j + 1] = a[j];
            j--;
        }
        a[j + 1] = v;
    }
}

/**
 * Insertion Sort:
 * Worst and Best: O(n * logn)
 * 
 * Space Complexity: O(n)
*/

// Description
/*
Merge sort is a recursive algorithm that continuously splits 
the array in half until it cannot be further divided i.e., the 
array has only one element left (an array with one element is 
always sorted). Then the sorted subarrays are merged into one 
sorted array.
*/
template <typename ItemType>
void mergeSort(ItemType theArray[], int first, int last)
{
    if (first < last)
    {
        // Sort each half
        int mid = first + (last - first) / 2; // Index of midpoint
        // Sort left half theArray[first..mid]
        mergeSort(theArray, first, mid);
        // Sort right half theArray[mid+1..last]
        mergeSort(theArray, mid + 1, last);
        // Merge the two halves
        merge(theArray, first, mid, last);
    } // end if
} // end mergeSort

/**
 * Quick Sort
 * Worst Case: O(n^2)
 * Best: O(n * logn)
 * 
 * Space Complexity: O(n)
*/

// Description
/*
Quicksort is a divide-and-conquer algorithm. It works by selecting 
a 'pivot' element from the array and partitioning the other 
elements into two sub-arrays, according to whether they 
are less than or greater than the pivot. 

Uses combination of recursion and swaping
*/

void quickSort(int array[], int low, int high) {
  if (low < high) {
      
    // find the pivot element such that
    // elements smaller than pivot are on left of pivot
    // elements greater than pivot are on righ of pivot
    int pi = partition(array, low, high);

    // recursive call on the left of pivot
    quickSort(array, low, pi - 1);

    // recursive call on the right of pivot
    quickSort(array, pi + 1, high);
  }
}

// function to swap elements with ptrs
void swap(int *a, int *b) {
  int t = *a;
  *a = *b;
  *b = t;
}

// function to print the array
void printArray(int array[], int size) {
  int i;
  for (i = 0; i < size; i++)
    cout << array[i] << " ";
  cout << endl;
}

// function to rearrange array (find the partition point)
int partition(int array[], int low, int high) {
    
  // select the rightmost element as pivot
  int pivot = array[high];
  
  // pointer for greater element
  int i = (low - 1);

  // traverse each element of the array
  // compare them with the pivot
  for (int j = low; j < high; j++) {
    if (array[j] <= pivot) {
        
      // if element smaller than pivot is found
      // swap it with the greater element pointed by i
      i++;
      
      // swap element at i with element at j
      swap(&array[i], &array[j]);
    }
  }
  
  // swap pivot with the greater element at i
  swap(&array[i + 1], &array[high]);
  
  // return the partition point
  return (i + 1);
}

/**
 * Shell Sort
 * Worst Case: O(n^2)
 * Best Case: O(n^(3/2))
*/
// Description
/*
Using a gap, we insertion sort the elements at the gap, then move one
and insertion sort all the elements using the gap at the new starting point
*/
int shellSort(int arr[], int n) 
{ 
    // Start with a big gap, then reduce the gap 
    for (int gap = n/2; gap > 0; gap /= 2) 
    { 
        // Do a gapped insertion sort for this gap size. 
        // The first gap elements a[0..gap-1] are already in gapped order 
        // keep adding one more element until the entire array is 
        // gap sorted  
        for (int i = gap; i < n; i += 1) 
        { 
            // add a[i] to the elements that have been gap sorted 
            // save a[i] in temp and make a hole at position i 
            int temp = arr[i]; 
  
            // shift earlier gap-sorted elements up until the correct  
            // location for a[i] is found 
            int j;             
            for (j = i; j >= gap && arr[j - gap] > temp; j -= gap) 
                arr[j] = arr[j - gap]; 
              
            //  put temp (the original a[i]) in its correct location 
            arr[j] = temp; 
        } 
    } 
    return 0; 
}
