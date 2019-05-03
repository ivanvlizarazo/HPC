#include <vector>
#include <iostream>
#include <stdlib.h>     /* srand, rand */
#include <time.h> 
#include <chrono> 

using namespace std;
using namespace std::chrono; 

typedef vector <int> row;
typedef vector < vector <int> > mat;

mat operator * (mat &A, mat &B){
    int size = A.size();
    mat answer(size, vector<int>(size, 0));

    for(int i = 0; i < size; i++){
        for(int j = 0; j < size; j++){
            for(int k = 0; k < size; k++){
                answer[i][j] += A[i][k] * B[j][k];
            }
        }
    }
    return answer;
}

void print(mat &matrix){
    for(int i = 0; i < matrix.size(); i++){
        for(int j = 0; j < matrix.size(); j++){
            cout<< matrix[i][j] << "  ";
        }
        cout<<"\n";
    }
}

void generate(mat &matrix){
    //srand (time(NULL));
    for(int i = 0; i < matrix.size(); i++){
        for(int j = 0; j < matrix.size(); j++){
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
    mat A(n,row(n)), B(n, row(n)), C(n,row(n));
    mat trans(n,row(n));
    generate(A);
    generate(B);
    transponer(B,trans);
    auto start = high_resolution_clock::now();
    C = A * trans;
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start); 
    
    /*
    cout<<'A'<<endl;
    print(A);
    cout<<'B'<<endl;
    print(B);
    cout<<'C'<<endl;
    print(C);
    */
    cout <<duration.count() <<",";
    return 0;
}

