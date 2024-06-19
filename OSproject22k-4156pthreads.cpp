#include <iostream>
#include <cstdlib>
#include <pthread.h>
#include <climits>
#include <cstring>
#include <unistd.h>
#include <vector>
#include <algorithm>
#include <chrono>

using namespace std;
#define LEN 10000
#define PT 10
#define NUM_THREADS 10
int a[LEN];

void init(int a[], int l = LEN)
{
    memset(a, INT32_MAX, sizeof(int) * l);
    for (int i = 0; i < l; ++i)
    {
        int temp;
        do
        {
            temp = rand() % l;
        } while (a[temp] != INT32_MAX && a[temp] != -1);
        a[temp] = i;
    }
}

// rabin karp start tick

#define d 256

void search(const string &pat, const string &txt, int prime, vector<int> &result, int start, int end)
{
    int pat_l = pat.length();
    int txt_l = txt.length();
    int i, j;
    int p_h = 0;
    int window_h = 0;
    int basichash = 1;

    for (i = 0; i < pat_l; i++)
    {
        p_h = (d * p_h + pat[i]) % prime;
        window_h = (d * window_h + txt[i]) % prime;
    }

    for (i = 0; i < pat_l - 1; i++)
        basichash = (basichash * d) % prime;

    for (i = start; i <= end; i++)
    {
        if (p_h == window_h)
        {
            for (j = 0; j < pat_l; j++)
            {
                if (txt[i + j] != pat[j])
                    break;
            }

            if (j == pat_l)
                result.push_back(i);
        }

        if (i < end)
        {
            window_h = (d * (window_h - txt[i] * basichash) + txt[i + pat_l]) % prime;

            if (window_h < 0)
                window_h = (window_h + prime);
        }
    }
}
struct rbthreadsargs
{
    string pat;
    string txt;
    int prime;
    vector<int> *result;
    int start;
    int end;
};
void *search_thread(void *arg)
{

    rbthreadsargs *args = reinterpret_cast<rbthreadsargs *>(arg);
    search(args->pat, args->txt, args->prime, *(args->result), args->start, args->end);

    return nullptr;
}

vector<int> rbp(const string &pat, const string &txt, int prime)
{
    int pat_l = pat.length();
    int txt_l = txt.length();
    vector<int> result;
    pthread_t threads[PT];
    vector<rbthreadsargs> threadargs(PT);

    int chunk_size = txt_l / PT;
    int remainder = txt_l % PT;
    int start = 0;
    int end = chunk_size - 1;

    for (int i = 0; i < PT; ++i)
    {
        if (i == PT - 1)
            end += remainder;
        threadargs[i] = {pat, txt, prime, &result, start, end};
        pthread_create(&threads[i], NULL, search_thread, &threadargs[i]);
        start = end + 1;
        end += chunk_size;
    }

    for (int i = 0; i < PT; ++i)
    {
        pthread_join(threads[i], NULL);
    }

    return result;
}

// rabin karp

// heap sort start

struct hpsortargs
{
    int *array;
    int size;
    int i;
    int len;
};

pthread_mutex_t mutex;

void compareAndSwap(int *array, int i, int j)
{
    pthread_mutex_lock(&mutex);
    swap(array[i], array[j]);
    pthread_mutex_unlock(&mutex);
}

void heapify(int *array, int i, int len)
{
    int child;
    int temp = array[i];
    while (2 * i + 1 < len)
    {
        child = 2 * i + 1;
        if (child < len - 1 && array[child] < array[child + 1])
        {
            ++child;
        }
        if (temp < array[child])
        {
            compareAndSwap(array, i, child);
        }
        else
        {
            break;
        }
        i = child;
    }
    array[i] = temp;
}

void *parallelheapify(void *arg)
{
    hpsortargs *args = (hpsortargs *)arg;
    int *array = args->array;
    int i = args->i;
    int len = args->len;

    heapify(array, i, len);

    return NULL;
}

void parallelHeapConstruction(int *array, int len)
{
    pthread_t threads[len];
    hpsortargs threadargs[len];

    for (int i = len / 2 - 1; i >= 0; --i)
    {
        threadargs[i].array = array;
        threadargs[i].i = i;
        threadargs[i].len = len;
        pthread_create(&threads[i], NULL, parallelheapify, (void *)&threadargs[i]);
    }

    for (int i = 0; i < len / 2; ++i)
    {
        pthread_join(threads[i], NULL);
    }
}

void parallelHeapSort(int *array, int len)
{
    parallelHeapConstruction(array, len);

    for (int i = len - 1; i > 0; --i)
    {
        compareAndSwap(array, 0, i);
        heapify(array, 0, i);
    }
}

// heap sort end

// merge sort
struct msargs
{
    int *array;
    int start;
    int end;
};

void merge(int array[], int low, int mid, int high)
{
    int Left_S = mid - low + 1;
    int Right_S = high - mid;

    int *left=new int[Left_S], *right=new int[Right_S];

    for (int i = 0; i < Left_S; ++i)
        left[i] = array[low + i];
    for (int j = 0; j < Right_S; ++j)
        right[j] = array[mid + 1 + j];

    int i = 0, j = 0, k = low;
    while (i < Left_S && j < Right_S)
    {
        if (left[i] <= right[j])
            array[k++] = left[i++];
        else
            array[k++] = right[j++];
    }

    while (i < Left_S)
        array[k++] = left[i++];

    while (j < Right_S)
        array[k++] = right[j++];
}

void merge_sort(int array[], int low, int high)
{
    if (low < high)
    {
        int mid = low + (high - low) / 2;
        merge_sort(array, low, mid);
        merge_sort(array, mid + 1, high);
        merge(array, low, mid, high);
    }
}

void *merge_sort_thread(void *arg)
{
    struct msargs *args = reinterpret_cast<msargs *>(arg);
    int *array = args->array;
    int low = args->start;
    int high = args->end;
    merge_sort(array, low, high);
    return nullptr;
}

void parallel_merge_sort(int array[], int size)
{
    pthread_t threads[PT];
    struct msargs thread_args[PT];

    for (int i = 0; i < PT; ++i)
    {
        thread_args[i].array = array;
        thread_args[i].start = i * (size / PT);
        thread_args[i].end = (i + 1) * (size / PT) - 1;
        pthread_create(&threads[i], NULL, merge_sort_thread, &thread_args[i]);
    }

    for (int i = 0; i < PT; ++i)
    {
        pthread_join(threads[i], NULL);
    }

    for (int i = 1; i < PT; ++i)
    {
        merge(array, 0, i * (size / PT) - 1, (i + 1) * (size / PT) - 1);
    }
}
// merge sort end//tick

// bitonic sort start
struct btsargs {
    int* arr;
    int low;
    int high;
    bool direction;
};

void swap(int& a, int& b) {
    int temp = a;
    a = b;
    b = temp;
}

void comparator(int arr[], int i, int j, bool direction) {
    if ((arr[i] > arr[j] && direction) || (arr[i] < arr[j] && !direction))
        swap(arr[i], arr[j]);
}

void bitonicMerge(int arr[], int low, int count, bool direction) {
    if (count > 1) {
        int mid = count / 2;
        for (int i = low; i < low + mid; ++i)
            comparator(arr, i, i + mid, direction);
        bitonicMerge(arr, low, mid, direction);
        bitonicMerge(arr, low + mid, mid, direction);
    }
}

void bitonicsort(int arr[], int low, int count, bool direction) {
    if (count > 1) {
        int mid = count / 2;
        bitonicsort(arr, low, mid, true);
        bitonicsort(arr, low + mid, mid, false);
        bitonicMerge(arr, low, count, direction);
    }
}

void* bitonicsortworker(void* arg) {
    btsargs* args = (btsargs*)arg;
    bitonicsort(args->arr, args->low, args->high - args->low + 1, args->direction);
    return NULL;
}

void bparallelitonicsort(int arr[], int size) {
    pthread_t threads[PT];
    btsargs thread_args[PT]; 

    for (int i = 0; i < PT; ++i) {
        thread_args[i].arr = arr;
        thread_args[i].low = i * (size / PT);
        thread_args[i].high = (i + 1) * (size / PT) - 1;
        thread_args[i].direction = (i < PT / 2) ? true : false; // Assign direction to each half of threads
        pthread_create(&threads[i], NULL, bitonicsortworker, (void*)&thread_args[i]);
    }

    for (int i = 0; i < PT; ++i) {
        pthread_join(threads[i], NULL);
    }

    for (int step = size ; step > 0; step /= 2) {
        for (int i = 0; i < size; i += step) {
            bitonicMerge(arr, i, step, true);
        }
    }
}
// bitonic sort end

// hyperwuick sort start

typedef struct hyperquicksortparameters
{
    int *array;
    int low;
    int high;
    int threadnum;
} hpa;

void swap(int *a, int *b)
{
    int t = *a;
    *a = *b;
    *b = t;
}

int partition(int *array, int low, int high, int pivot)
{
    int p_v = array[pivot];
    swap(&array[pivot], &array[high]);
    int smallest = low;

    for (int i = low; i < high; i++)
    {
        if (array[i] <= p_v)
        {
            swap(&array[i], &array[smallest]);
            smallest++;
        }
    }
    swap(&array[smallest], &array[high]);
    return smallest;
}

void quickSort(int *array, int low, int high)
{
    if (low < high)
    {
        int pivot_pos = low + (high - low) / 2;
        pivot_pos = partition(array, low, high, pivot_pos);
        quickSort(array, low, pivot_pos - 1);
        quickSort(array, pivot_pos + 1, high);
    }
}

void hyperquicksort(int *array, int low, int high, int threadnum);

void *worker_hyperquicksort(void *initialValues)
{
    hpa *parameters = reinterpret_cast<hpa *>(initialValues);
    hyperquicksort(parameters->array, parameters->low, parameters->high, parameters->threadnum);
    return NULL;
}

void hyperquicksort(int *array, int low, int high, int threadnum)
{
    if (low < high)
    {

        int pivot_pos = low + (high - low) / 2;
        pivot_pos = partition(array, low, high, pivot_pos);
        pthread_t thread;

        if (threadnum > 0)
        {
            hpa thread_param = {array, low, pivot_pos - 1, threadnum};
            int result;
            result = pthread_create(&thread, NULL, worker_hyperquicksort, &thread_param);

            hyperquicksort(array, pivot_pos + 1, high, threadnum);

            pthread_join(thread, NULL);
        }
        else
        {

            quickSort(array, low, pivot_pos - 1);
            quickSort(array, pivot_pos + 1, high);
        }
    }
}

// hyperquicksort end tick
int main()
{
    srand(time(0));
    double times[5]={0.0};

    // merge sort driver start
    cout << "mergesort start\n";
    //sleep(2);
    init(a);
    cout << "unsorted :\n";
    for (int i = 0; i < LEN; ++i)
    {
        cout << a[i] << endl;
    }
    //sleep(2);
    auto start = chrono::high_resolution_clock::now();
    parallel_merge_sort(a, LEN);
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed_seconds = end - start;
    times[0]=elapsed_seconds.count();

    cout << "\nmergesort took" << elapsed_seconds.count() << "seconds" << "using" << PT << " threads\n";
    cout << "sorted :\n";
    //sleep(2);
    for (int i = 0; i < LEN; ++i)
    {
        cout << a[i] << endl;
    }
    //sleep(2);

    // merge sort driver end

    // rabin karp driver start
    #define N 10
    cout << "rabin karp start\n";
    string txt = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Proin lacinia ex ac libero dignissim, nec vulputate tellus malesuada. Vivamus maximus, massa sed accumsan hendrerit, leo lorem facilisis est, sed volutpat arcu purus ut nibh. Praesent vel eleifend justo. In sed congue neque. Aenean lobortis consectetur ipsum quis volutpat. Sed pellentesque metus id eleifend semper. Cras nec purus urna. Sed tempus eleifend elit, elementum fermentum libero mollis a. Suspendisse commodo eleifend ornare. Duis enim magna, fermentum quis blandit eu, condimentum nec ex. Praesent neque neque, porttitor sed posuere id, dictum et ligula. Aliquam malesuada eleifend orci non tincidunt. Ut id tellus a leo rutrum convallis et et nisi. Morbi massa lectus, tristique a massa elementum, dignissim sollicitudin ipsum.Curabitur sagittis feugiat arcu ut tristique. Praesent augue magna, ultrices vel mauris quis, congue tristique justo. Maecenas consequat sapien eu felis lacinia molestie. Morbi sollicitudin justo nec nisi congue luctus. Vestibulum id tellus laoreet, iaculis arcu ut, ultricies diam. Donec varius nec ipsum sit amet porttitor. Integer aliquam risus nec arcu suscipit, ac feugiat quam finibus.Nullam mattis eros est. Fusce felis ex, gravida ut ex ultrices, commodo volutpat lorem. Aliquam scelerisque diam ac maximus rhoncus. Pellentesque habitant morbi tristique senectus et netus et malesuada fames ac turpis egestas. Mauris eget lacus eu ipsum dignissim facilisis. Phasellus quis gravida dui, non mollis metus. Maecenas porta augue sed arcu dapibus euismod.Ut posuere, felis sed mollis mollis, ante velit semper quam, a vulputate felis risus eu ante. In ullamcorper augue vel iaculis fringilla. Quisque dignissim ex massa, ac feugiat augue lacinia sit amet. Phasellus fermentum turpis nec quam pulvinar luctus. Nulla ut justo non ante placerat consequat. Pellentesque sed risus ipsum. Nunc imperdiet lorem ut leo viverra faucibus. Donec a magna porttitor, porta erat sit amet, ultrices risus. Integer et velit efficitur, rutrum eros vel, finibus ante. Nulla nec ligula arcu. Sed non blandit tortor. Nam at aliquet mi, mollis convallis arcu. Nullam pellentesque, metus ut feugiat ullamcorper, tellus sapien convallis dui, ut blandit ligula urna non est.Vestibulum ante ipsum primis in faucibus orci luctus et ultrices posuere cubilia curae; Morbi a nisi a ligula hendrerit consequat a ac sem. Vestibulum ante ipsum primis in faucibus orci luctus et ultrices posuere cubilia curae; Nunc id lacus a ante pellentesque placerat ut et odio. Vestibulum consequat rhoncus risus quis commodo. Etiam lacinia, tellus mattis dapibus dapibus, tellus magna dapibus elit, quis luctus ex turpis sit amet mauris. Pellentesque erat metus, lacinia quis gravida ut, facilisis non ipsum. Morbi dignissim sapien nec orci ornare commodo. Morbi odio massa, finibus a quam eu, tincidunt auctor ex. Etiam pulvinar mollis ex nec gravida. Nunc imperdiet odio ut molestie auctor. Sed malesuada lorem id enim ullamcorper, a tincidunt lacus bibendum. ";
    for(int i=0;i<N;++i)
    txt+=txt;
    string pat = "massa";
    int prime = 101;

    start = chrono::high_resolution_clock::now();
    vector<int> result = rbp(pat, txt, prime);

    end = chrono::high_resolution_clock::now();

    elapsed_seconds = end - start;
    cout << "\nrabin karp took " << elapsed_seconds.count() << " seconds using" << PT << " threads\n";
    times[1]=elapsed_seconds.count();
    cout << "occurrences at indexes:- ";
    for (int i = 0; i < result.size(); i++)
    {
        cout << result[i] << " ";
    }
    cout << endl;
    //sleep(3);
    // rabin karp driver end

    // bitonic sort driver start
    cout << "bitonic sort start\n";
    //sleep(2);
    int bitonicl = 10240;
    int bitonicarr[bitonicl];
    init(bitonicarr, bitonicl);
    cout << "unsorted :\n";
    for (int i = 0; i < bitonicl; ++i)
    {
        cout << bitonicarr[i] << endl;
    }
    //sleep(2);

    start = chrono::high_resolution_clock::now();

    bparallelitonicsort(bitonicarr,bitonicl);
    end = chrono::high_resolution_clock::now();
    elapsed_seconds = end - start;
        times[2]=elapsed_seconds.count();

    cout << "\nbitonic sort took" << elapsed_seconds.count() << "seconds" << "using" << PT << " threads\n";
    //sleep(2);
    cout << "sorted\n:";
    for (int i = 0; i < bitonicl; ++i)
    {
        cout << bitonicarr[i] << endl;
    }
    //sleep(2);


    // hyperquicksort driver start
    cout << "hyperquicksort start\n";
    //sleep(2);
    init(a);
    cout << "unsorted :\n";
    for (int i = 0; i < LEN; ++i)
    {
        cout << a[i] << endl;
    }
    //sleep(2);
    start = chrono::high_resolution_clock::now();
    hyperquicksort(a, 0, LEN - 1, PT);
    end = chrono::high_resolution_clock::now();
    elapsed_seconds = end - start;
        times[3]=elapsed_seconds.count();

    cout << "\nhyperquicksort took" << elapsed_seconds.count() << "seconds" << "using" << PT << " threads\n";
    //sleep(2);
    for (int i = 0; i < LEN; ++i)
    {
        cout << a[i] << endl;
    }
    //sleep(2);
    // hyperquicksort driver end
    
    // bitonic sort driver end

    // heap sort driver start
    cout << "heap sort start\n";
    //sleep(2);
    init(a);
    cout << "unsorted \n";
    for (int i = 0; i < LEN; ++i)
    {
        cout << a[i] << endl;
    }
    //sleep(2);
    pthread_mutex_init(&mutex, NULL);
    start = std::chrono::high_resolution_clock::now();
    parallelHeapSort(a, LEN);
    end = std::chrono::high_resolution_clock::now();
    elapsed_seconds = end - start;
    times[4]=elapsed_seconds.count();

    std::cout << "\nheap sort took" << elapsed_seconds.count() << "seconds" << "using" << PT << " threads\n";
    //sleep(2);
    pthread_mutex_destroy(&mutex);

    cout << "\nsorted array\n";
    for (int i = 0; i < LEN; ++i)
    {
        cout << a[i] << endl;
    }

    // heap sort driver end
    for(int i=0;i<5;++i)
    {
        cout<<times[i]<<endl;
    }
    return 0;
}
