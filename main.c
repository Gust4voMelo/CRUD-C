#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CONTATOS 1000

typedef enum {
    TELA_SAIR = -1,
    TELA_PRINCIPAL,
    TELA_CADASTRAR, 
    TELA_DESCADASTRAR,
    TELA_LISTAR_TODOS,
} Telas;  

typedef struct{
    char nome[128];
    char telefone[64];
} Contato;

typedef struct{
    Telas menu;
    int totalContatos;
    Contato contatos[MAX_CONTATOS];
} AgendaTelefonica;

int main()
{
        AgendaTelefonica agenda;
        //zera todos os bytes de agenda
        memset(&agenda, 0x0, sizeof(agenda));
        
        //menu da agenda
        agenda.menu = TELA_PRINCIPAL;
        
        
    FILE * memoria = fopen("contatos.bin", "rb");
    if (memoria == NULL){
        printf("Erro ao abrir arquivo de memoria de contatos.\n");
        printf("Você deve criar o arquivo 'contatos.bin' na primeira execução\n");
        return 1;
    }
    
    size_t lidos = fread(agenda.contatos, sizeof(Contato), MAX_CONTATOS, memoria);
    printf("Lidos %zd contatos \n", lidos);
    //faz totalContatos conter a quantidade de contatos lidos
    agenda.totalContatos = (int)lidos;
    fclose(memoria);
        
        while (agenda.menu != TELA_SAIR){
            switch (agenda.menu){
                
                case TELA_PRINCIPAL:
                {
                    printf("----------------------------------------\n");
                    printf("\tAGENDA TELEFONICA:\n");
                    printf("----------------------------------------\n");
                    int opcao = -1;
                    do
                    {
                        printf("Escolha uma opcão: \n");
                        printf("\tdigite 1 para cadastrar contato\n");
                        printf("\tdigite 2 para remover contato\n");
                        printf("\tdigite 3 para listar todos os contatos\n");
                        printf("\tdigite 0 para sair do programa\n");
                        scanf("%d", &opcao);
                        int c = 0; //Solução para esvaziamento do buffer
                        while((c = getchar()) != '\n' && c != EOF) { }
                    } while (opcao < 0 || opcao > 3);
                    
                    if (opcao == 0){
                        agenda.menu = TELA_SAIR;
                        printf("Programa finalizado com sucesso!");
                    }
                    
                    if (opcao == 1){
                        agenda.menu = TELA_CADASTRAR;
                        if (agenda.totalContatos >= MAX_CONTATOS){
                            printf("\tAgenda está lotada, não é possivel adiconar novos contatos");
                            agenda.menu = TELA_PRINCIPAL;
                        }
                    }
                    
                    if (opcao == 2){
                        agenda.menu = TELA_DESCADASTRAR;
                        if (agenda.totalContatos == 0){
                            printf("\tNão há nenhum contao cadastrado para remover\n");
                            agenda.menu = TELA_PRINCIPAL;
                        }
                    }
                    
                    if (opcao ==3){
                        agenda.menu = TELA_LISTAR_TODOS;
                        if (agenda.totalContatos == 0){
                            printf("\tAgenda não tem nenhum contato para listar\n");
                            agenda.menu = TELA_PRINCIPAL;
                        }
                    }
                    
                }
                break;
                
                case TELA_CADASTRAR:
                {
                    printf("----------------------------------------\n");
                    printf("\tTELA DE CADASTRO:\n");
                    printf("----------------------------------------\n");
                    Contato novo;
                    char opcao = 's';
                    printf("Você deseja inserir um novo contato? Enter para continuar\n");
                    int c = 0;
                    while ((c = getchar()) != '\n' && c != EOF) { }
                    
                    do{
                        memset(&novo, 0x0, sizeof(novo));
                        printf("Digite um novo nome para o contato: \n");
                        fgets(novo.nome, 128, stdin);
                        int id = 0;
                        while (novo.nome[id] != '\0' && novo.nome[id] != '\n'){
                            id++;
                        }
                        novo.nome[id] = '\0';
                        printf("Digite um novo telefone para o contato: \n");
                        fgets(novo.telefone, 64, stdin);
                        id = 0;
                        while (novo.telefone[id] != '\0' && novo.telefone[id] != 'n'){
                            id++;
                        }
                        novo.telefone[id] = '\0';
                        printf("Tem certeza que quer adicionar este contato? [s/n]\n");
                        opcao = getchar();
                        int c = 0;
                        while ((c = getchar()) != '\n' && c != EOF) { }
                    } while(opcao == 'n' || opcao == 'N');
                    
                    agenda.contatos[agenda.totalContatos] = novo;
                    agenda.totalContatos++;
                    memoria = fopen("contatos.bin", "w+b");
                    if (memoria == NULL){
                        printf("Erro ao abrir contatos.bin\n");
                        return 0;
                    }
                    
                    size_t salvos = fwrite(agenda.contatos, sizeof(Contato), agenda.totalContatos, memoria);
                    if (salvos != (size_t)agenda.totalContatos){
                        printf("Erro ao salvar contatos na memoria contatos.bin\n");
                        return 1;
                    }
                    fclose(memoria);
                    
                    agenda.menu = TELA_PRINCIPAL;
                }
                break;
                
                case TELA_DESCADASTRAR:
                {
                    printf("----------------------------------------\n");
                    printf("\tTELA DE DESCADASTRAR:\n");
                    printf("----------------------------------------\n");
                    int i;
                    
                    for (i = 0; i < agenda.totalContatos; i++){
                    printf("Contato %d\n", i);
                    printf("\tNome: %s\n", agenda.contatos[i].nome);
                    printf("\tTelefone: %s\n", agenda.contatos[i].telefone);
                    printf("----------------------------------------\n");
                    }
                    
                    int indice = -1;
                    do{
                        printf("Digite um indice de contato para descadastrar: de 0 até 1000 ");
                        scanf("%d", &indice);
                        
                        int c = 0;
                        while ((c = getchar()) != '\n' && c != EOF) { }
                    } while (indice < 0 || indice >= agenda.totalContatos);
                    
                    for (i = indice; i < agenda.totalContatos - 1; i++){
                        agenda.contatos[i] = agenda.contatos[i + 1];
                    }
                    agenda.totalContatos--;
                    
                    memoria = fopen("contatos.bin", "w+b");
                    if (memoria == NULL){
                        printf("Erro ao abrir contatos.bin\n");
                        return 0;
                    }
                    size_t salvos = fwrite (agenda.contatos, sizeof(Contato), agenda.totalContatos, memoria);
                    if (salvos != (size_t)agenda.totalContatos){
                        printf("Erro ao salvar contatos na memoria contatos.bin\n");
                        return 1;
                    }
                    fclose(memoria);
                    
                    agenda.menu = TELA_PRINCIPAL;
                }
                break;
                
                case TELA_LISTAR_TODOS:
                {
                    printf("----------------------------------------\n");
                    printf("\tLISTANDO CONTATOS:\n");
                    printf("----------------------------------------\n");
                    int i;
                    for (i=0; i < agenda.totalContatos; i++){
                        printf("Contato %d\n", i);
                        printf("\tNome: %s\n", agenda.contatos[i].nome);
                        printf("\tTelefone: %s\n", agenda.contatos[i].telefone);
                        printf("----------------------------------------\n");
                    }
                    agenda.menu = TELA_PRINCIPAL;
                }
                break;
                
                default:
                    agenda.menu = TELA_PRINCIPAL;
                    break;
            }
        }
    return 0;
}


