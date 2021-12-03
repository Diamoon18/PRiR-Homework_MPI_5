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
4)Function Zoo -> decides how many aviaries there will be; takes the status of aviaries (MPI_Recv)
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
### ``` matrix.c ``` - task 2 - generation of a matrix from other matrix
#### 
