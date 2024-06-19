#include <omp.h>
#include <iostream>
#include <windows.h>
#include <math.h>
#include<string.h>
using namespace std;
//maergesort start
#define LEN 100000
void init(int a[])
{
#pragma omp parallel for schedule(static)
    for (int i = 0; i < 100000; ++i)
    {
        int temp = NULL;
          do
          {
            temp = rand() % 100000;
          } while (a[temp] != NULL);
          a[temp] = i;
    }
for (int i = 0; i < 100000; ++i)
{
    cout << i << endl;
}

}
#define THRESHOLD 2

void merge(int arr[], int low, int mid, int high) {
    int n1 = mid - low + 1;
    int n2 = high - mid;

    int* left = new int[n1];int  *right=new int[n2];

    
    for (int i = 0; i < n1; i++)
        left[i] = arr[low + i];
    for (int j = 0; j < n2; j++)
        right[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = low;
   
    while (i < n1 && j < n2) {
        if (left[i] <= right[j])
            arr[k] = left[i++];
        else
            arr[k] = right[j++];
        k++;
    }

    
    while (i < n1)
        arr[k++] = left[i++];
    while (j < n2)
        arr[k++] = right[j++];
}

void merge_sort(int arr[], int low, int high) {
    if (low < high) {
        int mid = low + (high - low) / 2;

#pragma omp task firstprivate(arr, low, mid)
        {
            merge_sort(arr, low, mid);
        }

#pragma omp task firstprivate(arr, mid + 1, high)
        {
            merge_sort(arr, mid + 1, high);
        }

#pragma omp taskwait  // Wait for both sorting tasks to finish

        merge(arr, low, mid, high);
    }
}
//maergesortend
//rabinkarpstart
const int serlen = 256;
const int prime =INT_MAX;
int rbhash(const string &str, int n)
{
    int p = 0;
   
    for (int i = 0; i < n; i++)
    {
        p = (p * serlen + str[i]) % prime;
    }
    return p;
}

bool check(const string &txt, const string &pat, int i)
{
    int j = 0;cout<<txt.substr(i,pat.length());
    while (j < pat.size())
    {
        if (txt[i + j] != pat[j])
        {
            return false;
        }
        j++;
    }
    return true;
}

void search(const string &txt, const string &pat)
{
    int n = txt.size();
    int m = pat.size();

    int p = rbhash(pat, m);

    int* precomputedhashvalues = new int[n - m + 1]
        ; int chunk = ((n - m + 1) / omp_get_num_threads()); cout << chunk<<endl;
    memset(precomputedhashvalues, 0,chunk*sizeof(int));
#pragma omp parallel for schedule(static)
    for (int i = 0; i <= n - m; ++i)
    {
            int temp(0);
            temp = rbhash(txt.substr(i , m), m);
            if (temp < 0)
            {
                temp += prime;
            }
            precomputedhashvalues[i] = temp;
        
    }

#pragma omp parallel for schedule(static)
    for (int i = 0; i <= n - m; i++)
    {
        if (p == precomputedhashvalues[i])
        {
            if (check(txt, pat, i))
            {
                cout << "Pattern found at index " << i << endl;
                //exit(EXIT_SUCCESS);
            }
        }
    }
}
//rabinkarp end
int main()
{
    srand(time(0));
    /*omp_set_num_threads(2);
    string text = "popo se mein darta si";
    string ser = "mein";
    search(text, ser);
    cout<<"exiting...\n";*/
    int array[100000] = {NULL};
    init(array);
    

        merge_sort(array, 0,LEN- 1);
    

    std::cout << "Sorted array: \n";
    for (int i = 0; i < LEN; i++)
        std::cout << array[i] << " ";
    std::cout << std::endl;
    return 0;
}