# PRiR-Homework_MPI_5
### ``` zoo.c ``` - task 1 - simulation system for the zoo with MPI
#### Very similar to the program from lab 1
Zoo with aviaries and various animals.\
They may or may not be hungry.\
People work in these aviaries, that is, they watch the animals.\
If there is not enough food, the worker can order supplies.\
If the animals are very hungry and there are not enough supplies ordered, the zoo coulbe be closed.\
1) Definition of zoo states.
```c
#define ZOO 1
#define OPEN 2
#define BREAK 3
#define CLOSED 4
#define CALM 5
...
```
2) Help function -> Send to the zoo aviary state
```c
void Wyslij(int numer_a, int stan) //wyslij do zoo stan woliera
{
	wyslij[0]=numer_a;
	wyslij[1]=stan;
	MPI_Send(&wyslij, 2, MPI_INT, 0, tag, MPI_COMM_WORLD);
	sleep(2);
}
```
3) Function Aviaries -> sets the status of the process to ZOO, and depending on the amount of food and the number of employees, whether to open the zoo or not (stan=OPEN)
Also it sends the state to the Zoo function
```c
void Aviaries(char **animals) {
	int stan, i;
	stan=ZOO;
    //char *animal = animal_name(animals);
	while(1){
		if(stan==1){
			if(food >= 1000 && number_of_workers > 1){
				stan=OPEN;
				printf("There is enougth food for %s and disasters are not found for aviary nr- %d\n", animal_name(animals), nr_procesu);
				Wyslij(nr_procesu, stan);
			}else{
                stan=BREAK;
				Wyslij(nr_procesu, stan);
			}
      ...
```
4) Function Zoo -> decides how many aviaries there will be; takes the status of aviaries (MPI_Recv)
```c
void Zoo(int liczba_procesow) {
	int numer_a, status;
	number_of_aviaries = liczba_procesow-1;
	printf("Hello! Here is the zoo work simulation system! \n");
	if(rand()%2==1){
		printf("What a beautiful weather! We invite you to zoo!\n");
	} else {
		printf("Unfortunately, the weather is not very good, but we inite you to the zoo!\n");
	}
	printf("We wish you a pleasant time!\n \n \n");
	printf("We have %d avaries.\n", number_of_aviaries);
	sleep(2);

	while(1){
		MPI_Recv(&odbierz,2,MPI_INT,MPI_ANY_SOURCE,tag,MPI_COMM_WORLD, &mpi_status);
		numer_a = odbierz[0];
		status = odbierz[1];
		if(status==1){
			printf("Aviary nr-%d is okay! There is enougth food for animals.\n", numer_a);
		}
    ...
```
5) MPI initialization; 
6) The first process is responsible for the zoo; others processes for aviaries
```c
int main(int argc, char** argv) {
    
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&nr_procesu);
	MPI_Comm_size(MPI_COMM_WORLD,&liczba_procesow);
    
    char *animals[NUMBER_OF_STRING] = 
    {
        "Spiders", "Flamingo", "Owls", "Sparrows", "Horses",
        "Lions", "Gazelle", "Rabbits", "Hawks", "Snakes",
        "Cockatoos", "Wolfs", "Foxes", "Butterflies"
    };
	
	if(nr_procesu == 0){
		Zoo(liczba_procesow);
	}else{
        	srand(time(NULL));
		Aviaries(animals);
    }
	MPI_Finalize();
	return 0;
}
```
#### Results
![zoo1](https://user-images.githubusercontent.com/72127610/144656936-1292ea5a-f1fe-4d82-a380-b3d678ace582.jpg)
![zoo2](https://user-images.githubusercontent.com/72127610/144656960-98dbcc4f-05a1-43ae-bc68-5fd3b17ffa08.jpg)
![zoo3](https://user-images.githubusercontent.com/72127610/144656977-8341d20c-a40c-4b6a-9c25-ef8baaede9b4.jpg)
### ``` matrix.c ``` - task 2 - generation of a matrix from other matrix witn MPI
Each process generates a square matrix with the size NxN (where N - number of pocesses), where all elements are zeros except the diagonal.\
Fill in the matrix elements diagonally by the process number.\
Each process sends array of elements diagonally to the zero process.\
In zero process -> overwrite the table sent by i-process in the i-th line in matrix.\
Finally, process number-0 prints it's final matrix, where each element should be it's row number.
1) MPI Initialization
```c
MPI_Init(&argc, &argv);
MPI_Comm_size(MPI_COMM_WORLD, &size);
MPI_Comm_rank(MPI_COMM_WORLD, &nr_proc);
...
```
2) Matrix initialization
```c
 int matrix[size][size];

for (int i = 0; i < size; i++){
	for (int j = 0; j < size; j++){
		if (i==j) 
		    matrix[i][j] = nr_proc;
		else 
		    matrix[i][j] = 0;
	}
}
```
3) Print process nr-0 matrix
```c
if (nr_proc == 0) {
       printf("Process nr-0 -> matrix before communication:\n");
       for (int i = 0; i < size; i++) {
           for (int j = 0; j < size; j++){
               printf("%d ", matrix[i][j]);
           }
           printf("\n");
       }
       printf("\n"); 
}
```
4) Diagonal datatype vector creation; Commits a datatype.
```c
 MPI_Type_vector(size, 1, size+1, MPI_INT, &diag);
 MPI_Type_commit(&diag);
```
5) Process nr-0 collects all diagonals from other processes and stores them in a row corresponding to the sending process.
```c
MPI_Gather(matrix, 1, diag, matrix, size, MPI_INT, 0, MPI_COMM_WORLD);
```
6) Print process nr-0 result_matrix (each element should be its row number)
```c
if (nr_proc == 0) {
       printf("Process nr-0 -> matrix after communication:\n");
       for (int i = 0; i < size; i++) {
           for (int j = 0; j < size; j++){
                printf("%d ", matrix[i][j]);
           }
           printf("\n");
       }
       printf("\n");
}
```
7) Free the datatype; Finish work in MPI
```c
MPI_Type_free(&diag);
MPI_Finalize();
```
![matrix](https://user-images.githubusercontent.com/72127610/144657086-feb56770-24f0-43bf-9583-2da06cfddea1.jpg)


