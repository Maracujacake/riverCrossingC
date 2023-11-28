#include "./boat.h"
#include "./main.h"
#include "./boat.c"

/*
struct pthread_board{
    Boat * boat; // ''barco em que vão embarcar''
    int isCaptain; // indica quem é o capitão
    int type; // tipo de ''pessoa'' a embarcar
};
*/

// apresentação do programa para o usuário
void apresentacaoInicial(){
    printf("Olá, esse projeto baseia-se no problema de River crossing (pg. 160)\n");
    printf("Existem varias pessoas de um lado do rio, dentre elas hackers e funcionarios da Microsoft, \nque precisam atravessar para o outro lado e existe apenas um barco. O barco so se movimenta se \nquatro pessoas estiverem nele. Entretanto, nao eh permitido que uma viagem seja feita se no barco \nestiverem presentes 3 hackers e 1 funcionario ou 3 funcionarios e 1 hacker. Qualquer outra combinacao eh permitida.\n\n");
    printf("Você pode terminar a execução do programa a qualquer momento pressionando as teclas Ctrl+C do teclado\n\n");
    printf("Dado a largada, apenas sera apresentado permutacoes que respeite as regras impostas! \n");
    sleep(15);
    system("clear");
}


// ''animação'' do barco no console
void apresentacao(){
    printf(" Vamos la                                                                    \n");
    printf(" _ _ _                                                                       \n");
    printf(" _|   |                                                                      \n");
    printf(" _|_ _|                                                                      \n");
    printf("      |                                                                      \n");
    printf("   _ _|_ _ _ _ _ _ __                                                        \n");
    printf("  |_             _ __|                                                       \n");
    printf("    |_ _ _ _ _ _|/                                                           \n");
}

void apresentacao1(){
    printf(" Vamos la                                                                    \n");
    printf("               _ _ _                                                         \n");
    printf("               _|   |                                                        \n");
    printf("               _|_ _|                                                        \n");
    printf("                |                                                            \n");
    printf("             _ _|_ _ _ _ _ _ __                                              \n");
    printf("            |_             _ __|                                             \n");
    printf("              |_ _ _ _ _ _|/                                                 \n");
}  

void apresentacao2(){
    printf(" Vamos la                                                                    \n");
    printf("                                  _ _ _                                     \n");
    printf("                                  _|   |                                    \n");
    printf("                                  _|_ _|                                    \n");
    printf("                                   |                                        \n");
    printf("                                _ _|_ _ _ _ _ _ __                          \n");
    printf("                               |_             _ __|                         \n");
    printf("                                |_ _ _ _ _ _|/                               \n");
}

void apresentacao3(){
    printf(" Vamos la                                                                    \n");
    printf("                                                     _ _ _                   \n");
    printf("                                                     _|   |                  \n");
    printf("                                                     _|_ _|                  \n");
    printf("                                                      |                      \n");
    printf("                                                   _ _|_ _ _ _ _ _ __        \n");
    printf("                                                  |_             _ __|       \n");
    printf("                                                    |_ _ _ _ _ _|/           \n");
}

void apresentacao4(){
    //printf("Entao, eh dada a largada!!\n");
    printf("Presentes no barco:\n\n");
}


int main(){
    system("clear"); // limpa tela do terminal
    apresentacaoInicial(); // exibe apresentação do programa

    // animação do barco conforme a viagem
    for (int p = 0; p < 2; p++)
    {
        apresentacao();
        sleep(1);
        system("clear");
        apresentacao1();
        sleep(1);
        system("clear");
        apresentacao2();
        sleep(1);
        system("clear");
        apresentacao3();
        sleep(1);
        system("clear");
    }
    apresentacao4();
    sleep(5);


    // tempo aleatório para sincronização
    srand(time(NULL));

    int isCaptain;
    Boat boat;
    pthread_t person;
    isCaptain = 0;

    // inicialização onde não há ninguém no barco e há 4 espaços vagos (padrão)
    boat.cap = 4;
    boat.n_hackers = 0;
    boat.n_serfs = 0;
    pthread_barrier_init(&(boat.barrier), NULL, 4);
    pthread_mutex_init(&(boat.mutex), NULL);
    pthread_mutex_lock(&(boat.mutex));

    sem_init(&boat.hackers_queue, 0, 0);        
    sem_init(&boat.serfs_queue, 0, 0);      

    struct pthread_board args;
    args.boat = &boat;
    args.isCaptain = isCaptain;

    // Criação de threads 
    while(1){
        sleep(1);
        int person_type = random() % 2; // (HACKER or SERF)
        args.type = person_type;
        pthread_create(&person,NULL, board,(void *)&args);
    }
    pthread_join(person, NULL);

    return 0;
} // endmain