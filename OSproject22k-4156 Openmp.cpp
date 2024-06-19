#include <omp.h>
#include <iostream>
#include <unistd.h>
#include <math.h>
#include <string.h>
#include <limits>
#include <stdint.h>
#include <ctime>
#include <stack>
#include <cstdlib>
using namespace std;


#define LEN 10000
void init(int a[],int l=LEN)
{
    memset(a, INT32_MAX, sizeof(int) * l);
#pragma omp parallel for
    for (int i = 0; i < l; ++i)
    {
        int temp;
        do
        {
            temp = rand() % l;
        } while (a[temp] != INT32_MAX && a[temp] != -1);
        a[temp] = i;
    }

    cout << "unsorted array\n";
    for (int i = 0; i < l; ++i)
    {
        cout << a[i] << endl;
        usleep(10);
    }
}
// maergesort start tick
void merge(int array[], int low, int mid, int high)
{
    int sub_arr1_len = mid - low + 1;
    int sub_arr2_len = high - mid;
    int left[sub_arr1_len];
    int right[sub_arr2_len];
    for (int i = 0; i < sub_arr1_len; i++)
        left[i] = array[low + i];
    for (int j = 0; j < sub_arr2_len; j++)
        right[j] = array[mid + 1 + j];

    int i = 0, j = 0, k = low;

    while (i < sub_arr1_len && j < sub_arr2_len)
    {
        if (left[i] <= right[j])
            array[k++] = left[i++];
        else
            array[k++] = right[j++];
    }

    while (i < sub_arr1_len)
        array[k++] = left[i++];
    while (j < sub_arr2_len)
        array[k++] = right[j++];

    
}

void merge_sort(int array[], int low, int high)
{
    if (low < high)
    {
        int mid = low + (high - low) / 2;

#pragma omp parallel sections
        {
#pragma omp section
            {
                merge_sort(array, low, mid);
            }

#pragma omp section
            {
                merge_sort(array, mid + 1, high);
            }
        }

        merge(array, low, mid, high);
    }
}
// maergesortend

// rabinkarpstart tick
const int serlen = 256;
const int prime = INT32_MAX;
int rbhash(const string &str, int n)
{
    int basichash = 0;

    for (int i = 0; i < n; i++)
    {
        basichash = (basichash * serlen + str[i]) % prime;
    }
    return basichash;
}

bool check(const string &txt, const string &pat, int i)
{
    int j = 0;
    cout << txt.substr(i, pat.length());
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

    int pat_hash = rbhash(pat, m);

    int *precomputedhashvalues = new int[n - m + 1];
    int chunk = ((n - m + 1) / omp_get_num_threads());
    cout << chunk << endl;
    memset(precomputedhashvalues, 0, chunk * sizeof(int));
#pragma omp parallel for schedule(static)
    for (int i = 0; i <= n - m; ++i)
    {
        int temp(0);
        temp = rbhash(txt.substr(i, m), m);
        if (temp < 0)
        {
            temp += prime;
        }
        precomputedhashvalues[i] = temp;
    }

#pragma omp parallel for schedule(static)
    for (int i = 0; i <= n - m; i++)
    {
        if (pat_hash == precomputedhashvalues[i])
        {
            if (check(txt, pat, i))
            {
                cout << "Pattern found at index " << i << endl;
                
            }
        }
    }
}
// rabinkarp end

// bitonic sort start

void compareExchange(int *array, int i, int j, bool direction)
{
    if ((array[i] > array[j]) == direction)
    {
        int temp = array[i];
        array[i] = array[j];
        array[j] = temp;
    }
}

void bitonicMerge(int *array, int start_in, int arr_len, bool direction)
{
    if (arr_len > 1)
    {
        int mid = arr_len / 2;
#pragma omp parallel for
        for (int i = start_in; i < start_in + mid; ++i)
        {
            compareExchange(array, i, i + mid, direction);
        }
        bitonicMerge(array, start_in, mid, direction);
        bitonicMerge(array, start_in + mid, mid, direction);
    }
}

void bitonicSort(int *array, int start_in, int arr_len, bool direction)
{
    if (arr_len > 1)
    {
        int mid = arr_len / 2;
        bitonicSort(array, start_in, mid, true);
        bitonicSort(array, start_in + mid, mid, false);
        bitonicMerge(array, start_in, arr_len, direction);
    }
}

// bitonic sort end tick

// hyperquicksort start

int partition(int array[], int low, int high) {
    int pivot = array[high];
    int i = low - 1;

    for (int j = low; j <= high - 1; j++) {
        if (array[j] < pivot) {
            i++;
            swap(array[i], array[j]);
        }
    }
    swap(array[i + 1], array[high]);
    return i + 1;
}

void hyperquickSort(int array[], int low, int high) {
    if (low < high) {
        int mid = partition(array, low, high);

        #pragma omp parallel sections
        {
            #pragma omp section
            {
                hyperquickSort(array, low, mid - 1);
            }

            #pragma omp section
            {
                hyperquickSort(array, mid + 1, high);
            }
        }
    }
}

// hyperquicksort end

// heap sort start

void heapify(int* array, int i, int arr_lenght) {
    int child;
    int temp = array[i];
    while (2 * i + 1 < arr_lenght) {
        child = 2 * i + 1;
        if (child < arr_lenght - 1 && array[child] < array[child + 1]) {
            ++child;
        }
        if (temp < array[child]) {
            array[i] = array[child];
        } else {
            break;
        }
        i = child;
    }
    array[i] = temp;
}

void parallelHeapConstruction(int* array, int arr_lenght) {
    #pragma omp parallel for schedule(dynamic)
    for (int i = arr_lenght / 2 - 1; i >= 0; --i) {
        heapify(array, i, arr_lenght);
    }
}

void parallelHeapSort(int* array, int arr_lenght) {
    parallelHeapConstruction(array, arr_lenght);

    for (int i = arr_lenght - 1; i > 0; --i) {
        swap(array[0], array[i]);
        heapify(array, 0, i);
    }
}
    // heap sort end

int main()
{
    srand(time(0));
    omp_set_num_threads(10);
    int array[LEN];
    double times[5]={0.0};
    // mergesort driver start
    cout<<"merge sort to be used index is 1(useful when printing times)\n";
    init(array);

    double startt = omp_get_wtime();
    merge_sort(array, 0, LEN - 1);
    double endt=omp_get_wtime();
    times[0]=endt-startt;

    cout << "\ntime taken by merge sort with "<<omp_get_num_threads()<<"is "<< times[0] <<"seconds"<< endl;
    cout << "\nsorted array \n";
    for (int i = 0; i < LEN; i++)
        cout << array[i] << " ";
    cout << endl;
    // mergesort driver end

    // rabinkarp driver start
    cout << "\nRabinKarp to be used index is 2(useful when printing times)\n";
    string text = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Proin lacinia ex ac libero dignissim, nec vulputate tellus malesuada. Vivamus maximus, massa sed accumsan hendrerit, leo lorem facilisis est, sed volutpat arcu purus ut nibh. Praesent vel eleifend justo. In sed congue neque. Aenean lobortis consectetur ipsum quis volutpat. Sed pellentesque metus id eleifend semper. Cras nec purus urna. Sed tempus eleifend elit, elementum fermentum libero mollis a. Suspendisse commodo eleifend ornare. Duis enim magna, fermentum quis blandit eu, condimentum nec ex. Praesent neque neque, porttitor sed posuere id, dictum et ligula. Aliquam malesuada eleifend orci non tincidunt. Ut id tellus a leo rutrum convallis et et nisi. Morbi massa lectus, tristique a massa elementum, dignissim sollicitudin ipsum.Curabitur sagittis feugiat arcu ut tristique. Praesent augue magna, ultrices vel mauris quis, congue tristique justo. Maecenas consequat sapien eu felis lacinia molestie. Morbi sollicitudin justo nec nisi congue luctus. Vestibulum id tellus laoreet, iaculis arcu ut, ultricies diam. Donec varius nec ipsum sit amet porttitor. Integer aliquam risus nec arcu suscipit, ac feugiat quam finibus.Nullam mattis eros est. Fusce felis ex, gravida ut ex ultrices, commodo volutpat lorem. Aliquam scelerisque diam ac maximus rhoncus. Pellentesque habitant morbi tristique senectus et netus et malesuada fames ac turpis egestas. Mauris eget lacus eu ipsum dignissim facilisis. Phasellus quis gravida dui, non mollis metus. Maecenas porta augue sed arcu dapibus euismod.Ut posuere, felis sed mollis mollis, ante velit semper quam, a vulputate felis risus eu ante. In ullamcorper augue vel iaculis fringilla. Quisque dignissim ex massa, ac feugiat augue lacinia sit amet. Phasellus fermentum turpis nec quam pulvinar luctus. Nulla ut justo non ante placerat consequat. Pellentesque sed risus ipsum. Nunc imperdiet lorem ut leo viverra faucibus. Donec a magna porttitor, porta erat sit amet, ultrices risus. Integer et velit efficitur, rutrum eros vel, finibus ante. Nulla nec ligula arcu. Sed non blandit tortor. Nam at aliquet mi, mollis convallis arcu. Nullam pellentesque, metus ut feugiat ullamcorper, tellus sapien convallis dui, ut blandit ligula urna non est.Vestibulum ante ipsum primis in faucibus orci luctus et ultrices posuere cubilia curae; Morbi a nisi a ligula hendrerit consequat a ac sem. Vestibulum ante ipsum primis in faucibus orci luctus et ultrices posuere cubilia curae; Nunc id lacus a ante pellentesque placerat ut et odio. Vestibulum consequat rhoncus risus quis commodo. Etiam lacinia, tellus mattis dapibus dapibus, tellus magna dapibus elit, quis luctus ex turpis sit amet mauris. Pellentesque erat metus, lacinia quis gravida ut, facilisis non ipsum. Morbi dignissim sapien nec orci ornare commodo. Morbi odio massa, finibus a quam eu, tincidunt auctor ex. Etiam pulvinar mollis ex nec gravida. Nunc imperdiet odio ut molestie auctor. Sed malesuada lorem id enim ullamcorper, a tincidunt lacus bibendum. ";
    string ser = "massa";
    startt=omp_get_wtime();
    #define N 10
    for (int i=0;i<N;++i)
    {text+=text;}
    search(text, ser);
    endt=omp_get_wtime();
    times[1]=endt-startt;

    cout<<"\nrabin karp took "<<times[1]<<"seconds with"<<omp_get_num_threads()<<" threads\n";
    //sleep(3);
    // rabinkarp driver end

    // bitonic sort driver start
    cout << "\nbitonic sort to be used index is 3(useful when printing times)\n";
    //omp_set_dynamic(0);
    int bitoniclen=10240;
    init(array,bitoniclen);
     bool direction = true;
     startt=omp_get_wtime();
     bitonicSort(array, 0, bitoniclen, direction);
     endt=omp_get_wtime();
     times[2]=endt-startt;
     cout<<"\nbitonicsort took "<<times[2]<<"seconds with "<<omp_get_num_threads()<<" threads\n";
     
     //sleep(3);
     cout << "\nSorted array: ";
     for (int i = 0; i < bitoniclen; ++i) 
     {
         cout << array[i] << " ";
     }
     cout << endl;
     //sleep(3);
    // note change len to 2^something it doesn't work otherwise
    //  bitonic sort driver end

    // hyperquicksort driver start
    cout<<"hyperquick sort to be used index is 4\n";
    init(array);
     startt=omp_get_wtime();
     hyperquickSort(array, 0, LEN- 1);
     endt=omp_get_wtime();
     times[3]=endt-startt;

     cout<<"\nhyperquicksort took "<<times[3]<<"seconds with "<<omp_get_num_threads()<<"threads\n";
     //sleep(3);
     cout << "\nsorted array \n";
     for (int i = 0; i < LEN; ++i) {
         cout << array[i] << " ";
     }
     cout << endl;
    //sleep(3);
    // hyperquicksort driver end

    // heapsort driver start

    // omp_set_dynamic(0);
    cout<<"heapsort to be used index is 5\n";
    init(array);
    startt=omp_get_wtime();
    parallelHeapSort(array, LEN);
    endt=omp_get_wtime();
    times[4]=endt-startt;
    cout<<"\nheapsort took" <<times[4]<<"seconds with "<<omp_get_num_threads()<<" threads\n";
    //sleep(2);
    cout << "\nsorted array\n";
    for (int i = 0; i < LEN; ++i)
    {
        cout << array[i] << " ";
    }
    cout <<endl;
    //sleep(3);
    cout<<"\ntime taken by each algo using openmp \n";
    

    // heapsort driver end tick
    for(int i=0;i<5;++i)
    cout<<"algo no"<<i+1<<"took"<<times[i]<<endl;
}

