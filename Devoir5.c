#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

int n;
sem_t* sem;

void *Thread(void* arg)
{
    int* i=(int*)arg;
    while(1)
    {
        sem_wait(&sem[*i]);
        printf("Execution du Thread%d\n",*i+1);
        sem_post(&sem[(*i+1)%n]);
    }
    pthread_exit(NULL);
}

int main(int argc,char* argv[])
{
    if(argc<2)
    {
        perror("Nombre d'argument insuffisant\n");
        return 0;
    }
    n=atoi(argv[1]);
    int resultat;

    sem=malloc(n*sizeof(sem_t));
    resultat=sem_init(&sem[0],0,1);
    if(resultat==-1)
    {
        printf("Erreur de creation du semaphore 1\n");
        return 0;
    }
    for(int i=1;i<n;i++)
    {
        resultat=sem_init(&sem[i],0,0);
        if(resultat==-1)
        {
            printf("Erreur de creation du semaphore %d\n",i+1);
            return 0;
        }
    }

    pthread_t th[n];
    for(int i=0;i<n;i++)
    {
        int* j=malloc(sizeof(int));
        *j=i;
        resultat=pthread_create(&th[i],NULL,Thread,j);
        if(resultat==-1)
        {
            printf("Erreur de creation du Thread %d\n",i+1);
            return 0;
        }
    }

    for(int i=0;i<n;i++)
       pthread_join(th[i],NULL);

    return 0;
}