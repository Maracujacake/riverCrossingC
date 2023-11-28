#include "./boat.h"
#include "./main.h"


struct pthread_board{
    Boat * boat; // barco
    int isCaptain; // indicador do capitão
    int type; // tipo de viajante
};

  void row_boat(pthread_mutex_t * mutex, int * is_captain, sem_t * queue, const int cap){
    int i;
    printf("Soltar o barco!!\n");
    sleep(5);

    printf("Liberar espaços, saiam do barco seus nerds!!\n\n"); 
    sleep(5); 
    pthread_mutex_unlock(mutex);
    for(i=0;i<cap;i++) sem_post(queue);
                          
    *is_captain = 0;
};


// Print de quantas pessoas há no barco
void print_boat_fleet(const Boat boat){
    printf("h: %d, s: %d\n", boat.n_hackers, boat.n_serfs);
}

// função que deve ser chamada para garantir que nunca haverá:
// 3 serfs e 1 hacker OU
// 3 hackers e 1 serf

void * board(void *a){
    struct pthread_board * args = (struct pthread_board *)a;
        
    if(args->type == SERF){
        args->boat->n_serfs++; // serf
        sem_post(&args->boat->serfs_queue); // sinaliza que há um serf no barco
        // última thread (pessoa) a entrar no barco executa (capacidade maxima do barco)
        if(args->boat->n_serfs == args->boat->cap){
            print_boat_fleet(*args->boat);
            pthread_barrier_wait(&(args->boat->barrier));    //sinal

            args->isCaptain = 1;

            // Barco pode ser liberado
            pthread_mutex_lock(&args->boat->mutex);
            row_boat(&args->boat->mutex, &args->isCaptain, &args->boat->serfs_queue, args->boat->cap);

            // Barco vazio (pós-viagem)
            args->boat->n_serfs = 0;
            // verifica se o numero de threads (pessoas) é igual (capacidade total do barco = 4)
        }else if (args->boat->n_hackers == args->boat->cap/2 && args->boat->n_serfs == args->boat->cap/2){
            print_boat_fleet(*args->boat);
            pthread_barrier_wait(&(args->boat->barrier)); // sinal
            args->isCaptain = 1;

            // Barco pode ser liberado
            pthread_mutex_lock(&args->boat->mutex);
            row_boat(&args->boat->mutex, &args->isCaptain, &args->boat->serfs_queue, args->boat->cap);

            // Barco vazio (pós-viagem)
            args->boat->n_hackers = 0;
            args->boat->n_serfs = 0;
        }else{
            if(args->boat->n_hackers + args->boat->n_serfs >= args->boat->cap){
                // verifica se ultrapassou a capacidade do barco
                args->boat->n_serfs--;
            }else{
                print_boat_fleet(*args->boat);
                pthread_barrier_wait(&(args->boat->barrier));
                pthread_mutex_unlock(&(args->boat->mutex));
            }
        }
        sem_wait(&args->boat->serfs_queue);

        // codigo simetrico para caso seja um hacker           
    }else{
        args->boat->n_hackers++; // hacker
        sem_post(&args->boat->hackers_queue);
        if(args->boat->n_hackers == args->boat->cap){
            print_boat_fleet(*args->boat);

            pthread_barrier_wait(&(args->boat->barrier));
            args->isCaptain = 1;

            
            pthread_mutex_lock(&args->boat->mutex);
            row_boat(&args->boat->mutex, &args->isCaptain, &args->boat->hackers_queue, args->boat->cap);

            args->boat->n_hackers = 0;
        }else if (args->boat->n_hackers == args->boat->cap/2 && args->boat->n_serfs == args->boat->cap/2){
            print_boat_fleet(*args->boat);

            pthread_barrier_wait(&(args->boat->barrier));
            args->isCaptain= 1;

            pthread_mutex_lock(&args->boat->mutex);
            row_boat(&args->boat->mutex, &args->isCaptain,&args->boat->hackers_queue, args->boat->cap);

            args->boat->n_serfs = 0;
            args->boat->n_hackers = 0;
        }else{
            if(args->boat->n_hackers + args->boat->n_serfs >= args->boat->cap){
                args->boat->n_hackers--;
            }else{
                print_boat_fleet(*args->boat);
                pthread_barrier_wait(&(args->boat->barrier));
                pthread_mutex_unlock(&(args->boat->mutex));
            }
        }
        sem_wait(&args->boat->hackers_queue);             
        // }
    }

}; 