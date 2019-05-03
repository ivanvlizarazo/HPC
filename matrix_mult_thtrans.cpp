#include <vector>
#include <iostream>
#include <stdlib.h> /* srand, rand */
#include <time.h>
#include <thread>
#include <chrono> 

using namespace std;
using namespace std::chrono; 


typedef vector<int> row;
typedef vector<vector<int>> mat;

void multiply(mat &result, const int THREADS_NUMBER, const int thread_number,
              mat &m1, mat &m2)
{
    // Calculate workload
    const int MATRIX_SIZE = m1.size();
    const int n_elements = (MATRIX_SIZE * MATRIX_SIZE);
    const int n_operations = n_elements / THREADS_NUMBER;
    const int rest_operations = n_elements % THREADS_NUMBER;

    int start_op, end_op;
    
    if (thread_number == 0)
    {
        // First thread does more job
        start_op = n_operations * thread_number;
        end_op = (n_operations * (thread_number + 1)) + rest_operations;
    }
    else
    {
        start_op = n_operations * thread_number + rest_operations;
        end_op = (n_operations * (thread_number + 1)) + rest_operations;
    }
    
    for (int op = start_op; op < end_op; ++op)
    {
        const int row = op % MATRIX_SIZE;
        const int col = op / MATRIX_SIZE;
        
        for (int i = 0; i < MATRIX_SIZE; ++i)
        {
            result[row][col] += m1[row][i] * m2[col][i];
            //cout<<&result[row][col]<<endl;
        }
    }
}

void print(mat &matrix)
{
    
    for (int i = 0; i < matrix.size(); i++)
    {
        for (int j = 0; j < matrix.size(); j++)
        {
            cout << matrix[i][j] << "  ";
        }
        cout << "\n";
    }
}

void generate(mat &matrix)
{
    //srand(time(NULL));
    for (int i = 0; i < matrix.size(); i++)
    {
        for (int j = 0; j < matrix.size(); j++)
        {
            matrix[i][j] = rand() % 10;
        }
    }
}

void transponer(mat B, mat &T){

        for(int i = 0; i < B.size(); i++)
            for(int j = 0; j < B.size(); j++)
                T[i][j] = B[j][i];
}

int main(int argc, char const *argv[])
{
    int n = atoi(argv[1]);
    int THREADS_NUMBER = atoi(argv[1]) ;
    
    // Generate both matrix1 and matrix2 randomly
    mat matrix1(n, row(n)), matrix2(n, row(n));
    // Initialize result matrix with 0
    mat result(n, row(n, 0));
    mat trans(n,row(n));
    generate(matrix1);
    generate(matrix2);
    transponer(matrix2,trans);

    auto start = high_resolution_clock::now();
    // Create an array of threads
    thread threads[THREADS_NUMBER];

    for (int i = 0; i < THREADS_NUMBER; ++i)
    {
        /* Initialize each thread with the function responsible 
        of multiplying only a part of the matrices */
        threads[i] = thread(multiply, ref(result), THREADS_NUMBER, i, ref(matrix1), ref(trans));
    }

    for (int i = 0; i < THREADS_NUMBER; ++i)
    {
        // Wait until each thead has finished
        threads[i].join();
    }

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start); 

    
   /*cout << "Matrix 1" << endl;
    print(matrix1);
    cout << "Matrix 2" << endl;
    print(matrix2);
    cout << "Result" << endl;
    print(result);*/
    
    cout <<duration.count() <<",";
    return 0;
}
