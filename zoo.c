#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "mpi.h"

#define ZOO 1
#define OPEN 2
#define BREAK 3
#define CLOSED 4
#define CALM 5

#define NUMBER_OF_STRING 14

int number_of_aviaries;
int liczba_procesow;
int nr_procesu;

int number_of_workers = 3;
int food = 1100;
int tag=400;
int wyslij[2];
int odbierz[2];

MPI_Status mpi_status;

void Wyslij(int numer_a, int stan) //wyslij do zoo stan woliera
{
	wyslij[0]=numer_a;
	wyslij[1]=stan;
	MPI_Send(&wyslij, 2, MPI_INT, 0, tag, MPI_COMM_WORLD);
	sleep(2);
}

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
		if(status==2){
			printf("Aviary nr-%d is ready to work!\n", numer_a);
		}
		if(status==3){
			printf("Aviary nr-%d technical break.\n", numer_a);
		}
		if(status==4){ 
			printf("The zoo has a big problem, that is why it's closed!\n");
            exit(0);
		}
	} 
	printf("End program :)\n");
}

char *animal_name(char **animals){
    return animals[rand()%14];
}

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
		} else if(stan==2){
			printf("Aviary nr-%d working properly! %s aren't hungry!\n", nr_procesu, animal_name(animals));
			stan=CALM;
		} else if(stan==5){
			food-=rand()%100;
            printf("In aviary nr-%d %s eat. Food = %d \n", nr_procesu, animal_name(animals), food);
			if(food<=100){
				stan=BREAK;
				printf("In aviary nr-%d %s are hungry! Technical break.\n", nr_procesu, animal_name(animals));
				Wyslij(nr_procesu, stan);
			}else{
                stan=OPEN;
            }

        } else if(stan==3){
            printf("In aviary nr-%d technical break due to hunger of %s! The amount of food %d \n", nr_procesu, animal_name(animals), food);
           
            food+=rand()%200;
            if(food < 50){
                stan=CLOSED;
                Wyslij(nr_procesu, stan);
            } else {
                stan=OPEN;
                printf("In aviary nr-%d. The amount of food %d. Aviary is ready to work\n", nr_procesu, food);
                Wyslij(nr_procesu, stan);
            }
        } else if(stan==4){
            printf("In aviary nr-%d %s are in big trouble! Workers =%d The amount of food %d \n", nr_procesu, animal_name(animals), number_of_workers, food);
            Wyslij(nr_procesu, stan);
		}
    }
}

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


