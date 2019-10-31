//Arquivo: simula-vm.c
//Nome: Matheus Vinicius da Silva de Figueiredo
//Curso: Ciencia da Computacao - UFRJ
//Turma: 2019.2
//Descricao: Implementação dos algoritmos FIFO, LRU e Optimal para substituicao de paginas numa memoria virtual
//Entrada: O programa recebe o numero de quadros disponiveis como parametro e le um arquivo referencias.txt
//         que contem a sequencia de referencias as paginas (uma referencia por linha)
//Saida: O numero de faltas de pagina para cada algoritmo

#include <stdio.h>

#define MAXENTRIES 1000000
#define MAXPAGES 4096

int n;
int pageAmount;
int pages[MAXPAGES];
int entries[MAXENTRIES];
int i,j,k;
int pageFaults[3];

//Inicializa o vetor de paginas
void Start()
{
    for(i = 0; i < pageAmount; i++)
    {
        pages[i] = -2;
    }
}

//Procura por um valor nas paginas, retornando -1 se nao encontrar ou a pagina que contem o valor
int FindPage(int value)
{
    int page = -1;
    for(j = 0; j < pageAmount; j++)
    {
        if(pages[j] == value)
        {
            page = j;
            break;
        }
    }
    return page;
}

//First In First Out: Usa o vetor de paginas como uma pilha, inserindo novo valor no topo
void FIFO()
{
    Start();
    pageFaults[0] = 0;
    for(i = 0; i < n; i++)
    {
        if(FindPage(entries[i]) == -1)
        {
            for(k = 0; k < pageAmount-1; k++)
            {
                pages[k] = pages[k+1];
            }
            pages[k] = entries[i];
            pageFaults[0]++;
        }
    }
}

//Least Recently Used: Usa um vetor auxiliar, que armazena a linha da entrada em que a pagina foi modificada pela ultima vez
//                     e insere um valor novo na pagina que foi modificada faz mais tempo
void LRU()
{
    Start();
    pageFaults[1] = 0;

    //Vetor auxiliar que armazena ultimo uso
    int order[MAXPAGES]; 
    for(i = 0; i < n; i++)
    {
        if(FindPage(entries[i]) == -1)
        {
            for(j = 0; j < pageAmount; j++)
            {
                //Acha linha em que a pagina foi modificada pela ultima vez
                int p = pages[j];
                int found = 0;
                for(k = i-1; k >= 0; k--)
                {
                    if(p == entries[k])
                    {
                        order[j] = k;
                        found = 1;
                        break;
                    }
                    else
                        found = 0;
                }
                if(!found)
                    order[j] = -1;
            }
            //Escolhe a pagina que foi modificada faz mais tempo e a atribui o novo valor
            int smallest = MAXENTRIES+1;
            int smallestPage;
            for(j = 0; j < pageAmount; j++)
            {
                if(order[j] < smallest)
                {
                    smallest = order[j];
                    smallestPage = j;
                }
            }
            pages[smallestPage] = entries[i];
            pageFaults[1]++;
        }
    }
}

//Optimal: Baseado na lista de modificacoes, escolhe qual das paginas contem valor que demorara mais para ser modificado
//         e substitui o que ficara mais tempo sem ser usado
void OPT()
{
    Start();
    pageFaults[2] = 0;

    //Vetor que armazena proximo uso do valor atual da pagina
    int nextUse[MAXPAGES];
    for(i = 0; i < n; i++)
    {
        if(FindPage(entries[i]) == -1)
        {
            for(j = 0; j < pageAmount; j++)
            {
                int page = pages[j];
                int found = 0;
                for (k = i; k < n; k++)
                {
                    if(page == entries[k])
                    {
                        nextUse[j] = k;
                        found = 1;
                        break;
                    }
                }
                if(!found)
                    nextUse[j] = MAXENTRIES+1;
            }
            //Determina qual pagina tem valor que demorara mais para ser usado e o substitui
            int max = -MAXENTRIES-1;
            int furthestPage;
            for(j = 0; j < pageAmount; j++)
            {
                if(nextUse[j] > max)
                {
                    max = nextUse[j];
                    furthestPage = j;
                }
            }
            pages[furthestPage] = entries[i];
            pageFaults[2]++;
        }
    }
}

int main(int argc, char *argv[])
{
    pageAmount = atoi(argv[1]); //Recebe o numero de paginas dos argumentos de compilacao
    if(pageAmount < 1 || pageAmount > MAXPAGES) //Um numero valido de paginas deve ser usado
        return(1);

    n = 0; //Contador de entradas no arquivo .txt
    while ((!feof(stdin)) && (n <= MAXENTRIES) ) //Percorre o arquivo .txt usado de entrada ate o fim ou ate chegar no numero maximo de entradas
    {
        scanf ("%d\n", &entries[n]) ; //Salva o inteiro lido no vetor de entradas
        n++;
    }

    //Chama todos algoritmos de substituicao de pagina
    FIFO();
    LRU();
    OPT();

    printf("%5d quadros, %7d refs: FIFO: %5d PFs, LRU: %5d PFs, OPT: %5d PFs\n", pageAmount, n, pageFaults[0], pageFaults[1], pageFaults[2]);
}