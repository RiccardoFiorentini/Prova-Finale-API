#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define MAXARR 20
#define MAXCHAR 78

//RB-BST funzioni e struttura
//nodo dell'albero
struct nodo{
    char compat;
    char* parola; //puntatore alla parola
    struct nodo* dx;
    struct nodo* sx;
};

//inserimento elementi
void BSTInsert(struct nodo **radice, char *inser, short int len){
    //creo nuovo nodo
    struct nodo *z = (struct nodo*)malloc(sizeof(struct nodo));
    z->parola = (char*)malloc(sizeof(char)*(len+1));
    memcpy(z->parola, inser, len);
    z->parola[len] = '\0';
    z->dx = NULL;
    z->sx = NULL;
    z->compat = 't';
    //caso albero vuoto:
    if(*radice == NULL){
        (*radice) = z;
    }else{
    //se albero non vuoto
        struct nodo *tmp = (*radice);
        struct nodo *prec = NULL;
        while(tmp!=NULL){
            if(memcmp(z->parola, tmp->parola, len)<0){
                prec = tmp;
                tmp = tmp->sx;
            }else{
                prec = tmp;
                tmp = tmp->dx;
            }
        }
        if(memcmp(prec->parola, z->parola, len)>0){
            prec->sx = z;
        }else{
            prec->dx = z;
        }
    }
}

void BSTInsert1(struct nodo **radice, char *inser, short int len){  
    //creo nuovo nodo
    struct nodo *z = (struct nodo*)malloc(sizeof(struct nodo));
    z->parola = (char*)malloc(sizeof(char)*(len+1));
    memcpy(z->parola, inser, len);
    z->parola[len] = '\0';
    z->dx = NULL;
    z->sx = NULL;
    z->compat = 'f';
  
    //caso albero vuoto:
    if(*radice == NULL){
        (*radice) = z;
    }else{
    //se albero non vuoto
        struct nodo *tmp = (*radice);
        struct nodo *prec = NULL;
        while(tmp!=NULL){
            if(memcmp(z->parola, tmp->parola, len)<0){
                prec = tmp;
                tmp = tmp->sx;
            }else{
                prec = tmp;
                tmp = tmp->dx;
            }
        }
        if(memcmp(prec->parola, z->parola, len)>0){
            prec->sx = z;
        }else{
            prec->dx = z;
        }
    }
}

//restituisce 1 se la parola è presente 0 altrimenti
short int IsPresent(char* str, struct nodo* root, short int len){
    int comp = 0;
    while(root!=NULL){
        comp = memcmp(str, root->parola, len);
        if(comp>0){
            root = root->dx;
        }else if(comp<0){
            root = root->sx;
        }else if(comp==0){
            return 1;
        }
    }
    return 0;
}

//equivalente di stampa lista, stampa tutti gli elementi preselozionati
void Stampa_Filtrate(struct nodo* root){
    if (root == NULL){
        return;
    }
    Stampa_Filtrate(root->sx);
    if(root->compat == 't'){
        printf("%s\n", root->parola);
    }
    Stampa_Filtrate(root->dx);
}

//struttura utile all'analisi delle ricorrenze e della posizione delle lettere preseti nella parola
struct InList{
    short int ricorr; //ricorrenze conosciute
    char IsEsact; //sono il totale delle ricorrenze? v = vero sono esatto, f = non sono esatti
    char *NotPres; //punatore ad array di char di len k che segnala dove non è presente la lettera p = presente, n = non presente
};

/* 
controlla le parole che vengono inserite per indovinare e aggiorna i parametri sui quali si basa lo stampa filtrate,
infine restituisce 1 se la parola è stata indovinata 0 altrimenti.
*/
short int Check_Word(char* NotIn, struct InList** LetPres, char* RightPos, char str[], char ref[], short int len, short int* RefRec){
    
    if(memcmp(str, ref, len)==0){   //controllo se ha indovinato
        return 1;
    }else{
        short int i;

        //copio parola in input nell'output
        char* output = malloc(sizeof(char)*(len+1));
        for(i = 0; i < len; i++){
            output[i] = str[i];
        }
        output[len] = '\0';
        //copio parola di riferimento nel copy
        char* copy = malloc(sizeof(char)*len);
        for(i = 0; i < len; i++){
            copy[i] = ref[i];
        }
        
        //controllo le lettere giuste nella posizione giusta e per ognuna di esse costruisco la struttura per valutarne
        //le ricorrenze se non è già stata fatta
        for(i = 0; i<len; i++){
            if(copy[i] == output[i]){
                output[i] = '+';                                            //riscrittura dell'output
                copy[i] = '*';                                              //elimino caratteri già valutati
                RightPos[i] = 't';                                          //aggiorno array delle lettere indovinate e ne nella posizione giusta
            }
        }

        //controllo posizione sbagliata ma lettera giusta e lettera sbagliata o ridondante
        short int counter[MAXCHAR] = {0};        //tiene il conto delle ricorrenze per ogni lettera

        //conto le ricorrenze delle lettere che non sono già state considerate prima
        for(i = 0; i < len; i++){
           if(copy[i] != '*'){
                counter[copy[i] - '-'] = counter[copy[i] - '-'] + 1;
           }           
        }

        free(copy);             //libero la memoria poichè non uso più copy da qui in poi

        //per ogni lettera non acora considerata nella parola presa in input vedo se ci sono ancora ricorreze da considerare
        //ed essendo nella posizione sbagliata le ricorrenze rimaste o non essendoci proprio nella parola mi permettono di aggiornare gli ultimi parametri utili
        //e di concludere la scrittura dell'outpupt.
        for(i = 0; i < len; i++){
           if(output[i] != '+'){
                if(counter[output[i] - '-'] != 0){
                    if(counter[output[i] - '-'] > 0 && counter[output[i] - '-'] -1 > 0){
                        LetPres[output[i] - '-']->NotPres[i] = 'n';
                        counter[output[i] - '-'] = counter[output[i] - '-'] -1;
                        output[i] = '|';
                    } else if(counter[output[i] - '-'] > 0 && counter[output[i] - '-'] -1 == 0){
                        LetPres[output[i] - '-']->NotPres[i] = 'n';
                        counter[output[i] - '-'] = counter[output[i] - '-'] -2;
                        output[i] = '|';
                    }else if(counter[output[i] - '-'] == -1){
                        LetPres[output[i] - '-']->NotPres[i] = 'n';
                        output[i] = '/';
                    }
                    
                    
                }else{
                    output[i] = '/';
                }
           }           
        }

        printf("%s\n", output);
        free(output);           //libero la memoria poichè non uso più copy da qui in poi

        //controllo ricorrenze lettere ed effettiva esistenza
        memset(counter, 0, sizeof(short int)*MAXCHAR);

        //conto tutte le ricorrenze
        for(i = 0; i < len; i++){
           counter[str[i] - '-'] = counter[str[i] - '-'] + 1;           
        }

        //per ogni lettera, verifico se deve essere presente, se le sue ricorrenze sono più o meno di quelle note e se so già
        //l'esatta occorrenza di tale lettera, in tale modo aggiorno la struttura dati apposita al calcolo delle ricorreze.
        for(i=0; i<MAXCHAR; i++){
            if(RefRec[i]!=0){
                if(counter[i] > LetPres[i]->ricorr && counter[i]>RefRec[i]){
                    LetPres[i]->ricorr = RefRec[i];
                    LetPres[i]->IsEsact = 't';
                }else if(counter[i] > LetPres[i]->ricorr && counter[i]<=RefRec[i]){
                    LetPres[i]->ricorr = counter[i];
                }
            }else if(RefRec[i] == 0 && counter[i]!=0){
                NotIn[i] = 't';
            }
        } 
        return 2;
    }
}

//controlla se la parola considerata è compatibile o meno con le informazioni apprese fino a quel momento.
short int Check_Compat(char* NotIn, struct InList** LetPres, char* RightPos, char* str, char* ref, short int len){
    //controllo lettere giuste nel posto giusto;
    short int i;
    short int j;
    for(i=0; i<len; i++){
        if(RightPos[i] == 't' && ref[i]!=str[i]){ //so che lettera va in i ma la parola non la possiede in quella posizione
            return 0;
        }
    }

    //controllo se ci sono lettere che non dovrebbero
    for(i=0; i<len; i++){
        if(NotIn[str[i]-'-'] == 't'){
            return 0;
        }
    }

    //controllo posizione e ricorrenze per ogni lettera
    short int ric = 0;
    for(i=0; i<MAXCHAR; i++){
        ric = LetPres[i]->ricorr;
        if(ric != 0){
            for(j=0; j<len; j++){
                if(str[j] == i + '-'){
                    if(LetPres[i]->NotPres[j] == 'n'){
                        return 0;
                    }else{
                        ric--;
                    }
                }
            }
            if(ric>0){                                          //se ho meno ricorrenze di quelle che mi aspetto
                return 0;
            }else if((ric<0) && (LetPres[i]->IsEsact == 't')){      //se ho piu ricorrenze di quelle che mi aspetto ma quelle che mi aspetto so essere esatte
                return 0;
            }
        }
    }
    return 1;
}

//funzione ricorsiva derivata dall'inordervisit utile a percorre tutto l'albero in ordine alfabetico
//e creare una lista ordinata di parole che potrebbero essere la parola vincente
//la funzione aggiorna anche un contatore utile a sapere quanti elementi si trovano all'interno della lista
void filtraInorder(char* NotIn, struct InList** LetPres, char* RightPos, char* ref, short int len, struct nodo *root, int* counted){
    if (root == NULL){
        return;
    }
    filtraInorder(NotIn, LetPres, RightPos, ref, len, root->sx, counted);
    if(Check_Compat(NotIn, LetPres, RightPos, root->parola, ref, len) == 0){
        root->compat = 'f';
        (*counted) = (*counted) - 1;
    }
    filtraInorder(NotIn, LetPres, RightPos, ref, len, root->dx, counted);
}

void resetInorder(struct nodo *root){
    if (root == NULL){
        return;
    }
    resetInorder(root->sx);
    root->compat = 't';
    resetInorder(root->dx);
}
int main(){
    //variabile ignorata iserita solo per la compilazione
    //e usata per concludere ilprogramma alla fine
    short int outScanf = 0;

    int numListaProb = 0;           //numero parole compatibili con ciò che ho imparato
    int TotPar = 0;
    //radice albero
    struct nodo* root = NULL;
    
    //lunghezza parole input
    short int k = 0;
    
    //array di caratteri per emulare un array booleano e occupare meno spazio rispetto l'utilizzo deli int
    //è t = vero per ogni carattere non presente nella parola che ho scoperto, f = false atrimenti
    char NotIn[MAXCHAR] = {'f'};

    //salvo effettive ricorrenze di ogni lettera della parola da indovinare
    short int RefRecourr[MAXCHAR] = {0};

    //costruzione vocabolario
    outScanf = scanf("%hd", &k);    //lunghezza parole inserite
    char RightPos[k];   //array per salvare che lettere ho indovinato nella posizione giusta t = true se nella giusta posizione f = false altrimenti
    for(short int i = 0; i<k; i++){
        RightPos[i] = 'f';
    }

    //array di puntatori ad InList, NULL se non ancora utilizzati altrimenti puntati, permette di trovare velocemente il carattere da verificare
    struct InList* LetterePresenti[MAXCHAR];
    for(short int i = 0; i<MAXCHAR; i++){
        LetterePresenti[i] = malloc(sizeof(struct InList));
        LetterePresenti[i]->IsEsact = 'f';
        LetterePresenti[i]->ricorr = 0;
        LetterePresenti[i]->NotPres = malloc(sizeof(char)*k);
        for(short int j=0; j<k; j++){
            LetterePresenti[i]->NotPres[j] = 'p';
        }
    }

    //prendo in input le parole del vocabolario controllando sempre la prima lettera, se c'è un più il comando è per forza "+nuova_partita"
    /*int len = 17;
    if(len < k){
        len = k;
    }*/
    char StrIn[MAXARR+1];
    outScanf = scanf("%s", StrIn);
    while(StrIn[0] != '+'){
        BSTInsert(&root, StrIn, k);
        numListaProb++;
        TotPar++;
        outScanf = scanf("%s", StrIn);
    }
    if(StrIn[1]== 'i'){
        outScanf = scanf("%s", StrIn);
        while(StrIn[0] != '+'){
            BSTInsert(&root, StrIn, k);
            numListaProb++;
            TotPar++;
            outScanf = scanf("%s", StrIn);
        }
        outScanf = scanf("%s", StrIn);
    }

//inizio partita:
    
    char Reference[k+1];            //parola di riferimento
    short int try = 0;                    //numero tentaivi
    short int IsCorrect = 0;              //variabile in cui salvo se ho indovinato o meno la parola
    short int NuovaPartita = 1;           //variabile in cui salvo se voglio o meno cominciare una nuova partita
    short int i;
    
    while(NuovaPartita!=0){
        
        //prendo in input la parola da indovinare e mi salvo le effettive ricorrenze di ogni lettera presente
        outScanf = scanf("%s", Reference);
        for(i=0; i<k; i++){
            RefRecourr[Reference[i] - '-'] = RefRecourr[Reference[i] - '-'] + 1; 
        }

        //prendo in input il numeri effettivo di tentativi
        outScanf = scanf("%hd", &try);
        short int vittoria = 0;
        for(i=0; i<try; i++){
            //prendo le parole da testare oppure i comandi controllando solo le lettere utili
            outScanf = scanf("%s", StrIn);

            if(StrIn[0] == '+'){
                if(StrIn[1] == 's'){    //comando stampa filtrate
                    Stampa_Filtrate(root);
                }else{                  //comando inserisci inizio
                    outScanf = scanf("%s", StrIn);
                    while(StrIn[0] != '+'){
                        if(Check_Compat(NotIn, LetterePresenti, RightPos, StrIn, Reference, k) == 1){
                            BSTInsert(&root, StrIn, k);
                            numListaProb = numListaProb + 1;
                            TotPar++;
                        }else{
                            BSTInsert1(&root, StrIn, k);
                            TotPar++;
                        }
                        outScanf = scanf("%s", StrIn);
                    }
                }
                i--;
            }else{
                if(IsPresent(StrIn, root, k)){
                    IsCorrect = Check_Word(NotIn, LetterePresenti, RightPos, StrIn, Reference, k, RefRecourr);
                    if(IsCorrect == 1){
                        printf("%s\n", "ok");
                        vittoria = 1;
                        i = try;
                    }else{
                        filtraInorder(NotIn, LetterePresenti, RightPos, Reference, k, root, &numListaProb);
                        printf("%d\n", numListaProb);
                    }
                }else{
                    printf("%s\n", "not_exists");
                    i--;
                }
            }
        }
        if(vittoria == 0){
            printf("%s\n", "ko");
        }
        outScanf = scanf("%s", StrIn);
        if(StrIn[1] == 'i'){ //aggiungo parole
            outScanf = scanf("%s", StrIn);
            while(StrIn[0] != '+'){
                BSTInsert(&root, StrIn, k);
                TotPar++;
                outScanf = scanf("%s", StrIn);
            }
            outScanf = scanf("%s", StrIn);
            outScanf++;
            if(StrIn[1] == 'n'){
            //resetto tutti i parametri per la nuova partita
                try = 0;
                numListaProb = TotPar;
                IsCorrect = 0;
                memset(RightPos, 'f', k);
                memset(NotIn, 'f', MAXCHAR);
                for(i = 0; i<MAXCHAR; i++){
                    for(short int j=0; j<k; j++){
                        LetterePresenti[i]->NotPres[j] = 'p';
                    }
                    LetterePresenti[i]->IsEsact = 'f';
                    LetterePresenti[i]->ricorr = 0;
                }
                resetInorder(root);
                memset(RefRecourr, 0, MAXCHAR*(sizeof(short int)));
            }else{
                NuovaPartita = 0;
            }
        }else if(StrIn[1] == 'n'){
//resetto tutti i parametri per la nuova partita
            //resetto tutti i parametri per la nuova partita
                try = 0;
                numListaProb = TotPar;
                IsCorrect = 0;
                memset(RightPos, 'f', k);
                memset(NotIn, 'f', MAXCHAR);
                for(i = 0; i<MAXCHAR; i++){
                    for(short int j=0; j<k; j++){
                        LetterePresenti[i]->NotPres[j] = 'p';
                    }
                    LetterePresenti[i]->IsEsact = 'f';
                    LetterePresenti[i]->ricorr = 0;
                }
                resetInorder(root);
                memset(RefRecourr, 0, MAXCHAR*(sizeof(short int)));
        }else{
            NuovaPartita = 0;
        }
    }
}

