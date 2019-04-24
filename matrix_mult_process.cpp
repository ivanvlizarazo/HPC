#include <vector>
#include <iostream>
#include <stdlib.h> /* srand, rand */
#include <time.h>
#include <chrono> 
#include <bits/stdc++.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/mman.h>

using namespace std;
using namespace std::chrono; 


//typedef vector<int> row;
//typedef vector<vector<int>> mat;
int  **mt1, **mt2, **mt3;

void print(int ** matrix, int tam)
{
    for (int i = 0; i < tam; i++)
    {
        for (int j = 0; j < tam; j++)
        {
            cout << matrix[i][j] << "  ";
        
        }
        cout << "\n";
    }
}


void multiply(int **mt3, const int PROCESS_NUMBER, const int fork_number,
              int **mt1, int **mt2 )
{
    
    //cout<<"fork_number: "<<fork_number<<endl;
    // Calculate workload
    const int MATRIX_SIZE = PROCESS_NUMBER;
    const int n_elements = (MATRIX_SIZE * MATRIX_SIZE);
    const int n_operations = n_elements / PROCESS_NUMBER;
    const int rest_operations = n_elements % PROCESS_NUMBER;

    int start_op, end_op;
    //cout<<endl<<endl<<"ref recibida: "<< &result <<endl<<endl;

    if (fork_number == 0)
    {
        // First thread does more job
        start_op = n_operations * fork_number;
        end_op = (n_operations * (fork_number + 1)) + rest_operations;
    }
    else
    {
        start_op = n_operations * fork_number + rest_operations;
        end_op = (n_operations * (fork_number + 1)) + rest_operations;
    }
    
    for (int op = start_op; op < end_op; ++op)
    {   
        
        const int row = op % MATRIX_SIZE;
        const int col = op / MATRIX_SIZE;
        
        for (int i = 0; i < MATRIX_SIZE; ++i)
        {
            mt3[row][col] += mt1[row][i] * mt2[i][col];
            //cout<<&result[row][col]<<endl;
        }
    }
    
}

void generate(int ** matrix, int tam)
{
    //srand(time(NULL));
    for (int i = 0; i < tam; i++)
    {
        for (int j = 0; j < tam; j++)
        {
            matrix[i][j] = rand() % 10;
        }
    }
}

int main(int argc, char const *argv[])
{
    
    int i;
    int PROCESS_NUMBER = atoi(argv[1]);
    pid_t pid;
    int parent_id = getpid();

    /*
    // Generate both matrix1 and matrix2 randomly
    mat matrix1(n, row(n)), matrix2(n, row(n));
    // Initialize result matrix with 0
    mat result(n, row(n, 0));*/
    mt1=(int **)mmap(NULL,sizeof(int *)*PROCESS_NUMBER, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0); //Reservar memoria para filas
	mt2=(int **)mmap(NULL,sizeof(int *)*PROCESS_NUMBER, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0); //Reservar memoria para filas
	mt3=(int **)mmap(NULL,sizeof(int *)*PROCESS_NUMBER, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0); //Reservar memoria para filas

    for (int i=0; i<PROCESS_NUMBER; i++){
			mt1[i]=(int *)mmap(NULL,sizeof(int)*PROCESS_NUMBER, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0); //Reservar memoria para filas
			mt2[i]=(int *)mmap(NULL,sizeof(int)*PROCESS_NUMBER, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0); //Reservar memoria para filas
			mt3[i]=(int *)mmap(NULL,sizeof(int)*PROCESS_NUMBER, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0); //Reservar memoria para filas
		}
    generate(mt1,PROCESS_NUMBER);
    generate(mt2,PROCESS_NUMBER);

    auto start = high_resolution_clock::now();
    

    for (i=0; i<PROCESS_NUMBER; i++) {
		pid = fork();				// se hace fork()
		
		if (pid==0) {				// si el proceso se crea bien, terminamos el ciclo for
            break;
		}
		else if (pid==-1) {			// si hay error, se aborta la operación
			perror("ERROR al hacer fork()");
			exit(1);
			break;
		}
	}

    if (pid==0) {			// Lógica del Hijo
		multiply(mt3, PROCESS_NUMBER, i, mt1, mt2);
        //cout<<"proceso: "<<i<<endl;
        //print(mt3,PROCESS_NUMBER);
		exit(0);
	}
	else {					// Lógica del Padre
		for (int j=0; j<PROCESS_NUMBER; j++) {		// esperamos a que todos los hijos terminen (código mejorado)
    		if ((pid = wait(NULL)) >= 0) {
    			//cout << "Proceso " << pid << " terminado" << endl;
    		}
    	}

		/*cout << "Soy el padre " << getpid() << endl;
        cout << "Matrix 1" << endl;
        print(mt1,PROCESS_NUMBER);
        cout << "Matrix 2" << endl;
        print(mt2,PROCESS_NUMBER);
        cout << "Result" << endl;
        print(mt3,PROCESS_NUMBER);*/
        
	}

    


    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start); 

    
    
    cout <<duration.count() <<",";
    return 0;
}
