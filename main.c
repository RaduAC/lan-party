#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Player{
    char* prenume;
    char* nume;
    int puncte;
}Player;

typedef struct Team{
    char* nume_echipa;
    Player* player;
    int nrPlayers;
    float score;
}Team;

typedef struct Node{
    Team* team;
    struct Node* next;
}Node;

typedef struct StackNode{
    Team* val;
    struct StackNode* next;
}StackNode;

typedef struct QueueNode{
    Team* team1;
    Team* team2;
    struct QueueNode* next;
}QueueNode;

typedef struct Queue{
    QueueNode* front;
    QueueNode* rear;
}Queue;

typedef struct TreeNode{
    Team* team;
    struct TreeNode* left;
    struct TreeNode* right;
}TreeNode;

void addTeam(Node** head, Team* team){
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->team = team;
    newNode->next = *head;
    *head = newNode;
}

int eliminareMINIM(Node** head, int deelim, int nrTeams){
    for(int i = 0; i < deelim; i++){
        Node* curent = *head;
        Node* minNode = *head;
        Node* prevMin = NULL;
        Node* prev = NULL;
        while(curent != NULL){
            if(curent->team->score < minNode->team->score){
                minNode = curent;
                prevMin = prev;
            }
            prev = curent;
            curent = curent->next;
        }
        if(prevMin == NULL)
            *head = (*head)->next;
        else
            prevMin->next = minNode->next;
        free(minNode->team->nume_echipa);
        free(minNode->team->player);
        free(minNode->team);
        free(minNode);
        nrTeams--;
    }
    return nrTeams;
}

void printList(Node* head, FILE* fout){
    Node* curent = head;
    while(curent != NULL){
        fprintf(fout, "%s\n", curent->team->nume_echipa);
        curent = curent->next;
    }
}

int isEmptyStack(StackNode* top){
    return top == NULL;
}

void push(StackNode** top, Team* v){
    StackNode* newNode = (StackNode*)malloc(sizeof(StackNode));
    newNode->val = v;
    newNode->next = *top;
    *top = newNode;
}

Team* pop(StackNode** top){
    if(isEmptyStack(*top)) return NULL;
    StackNode* temp = *top;
    Team* aux = temp->val;
    *top = (*top)->next;
    free(temp);
    return aux;
}

void deleteStack(StackNode** top){
    StackNode* temp;
    while(*top != NULL){
        temp = *top;
        *top = (*top)->next;
        free(temp);
    }
}

int isEmptyQueue(Queue* coada){
    return coada->front == NULL;
}

void enQueue(Queue* coada, Team* t1, Team* t2){
    QueueNode* newNode = (QueueNode*)malloc(sizeof(QueueNode));
    newNode->team1 = t1;
    newNode->team2 = t2;
    newNode->next = NULL;
    if(coada->rear == NULL) coada->rear = newNode;
    else{
        coada->rear->next = newNode;
        coada->rear = newNode;
    }
    if(coada->front == NULL) coada->front = coada->rear;
}

QueueNode* deQueue(Queue* coada){
    if(isEmptyQueue(coada)) return NULL;
    QueueNode* aux = coada->front;
    coada->front = coada->front->next;
    if(coada->front == NULL) coada->rear = NULL;
    return aux;
}

Team* Match(Team* t2, Team* t1){
    Team* castigator;
    if(t1->score < t2->score)
        castigator = t2;
    else
        castigator = t1;

    for(int k = 0; k < castigator->nrPlayers; k++)
        castigator->player[k].puncte++;
    int total_score = 0;
    for(int k = 0; k < castigator->nrPlayers; k++)
        total_score += castigator->player[k].puncte;
    castigator->score = (float)total_score / castigator->nrPlayers;
    return castigator;
}

void printRunda(Queue* coada, FILE* fout, int runda){
    fprintf(fout, "--- ROUND NO:%d\n", runda);
    QueueNode* curent = coada->front;
    while(curent != NULL){
        fprintf(fout, "%-33s-  %31s\n", curent->team1->nume_echipa, curent->team2->nume_echipa);
        curent = curent->next;
    }
    fprintf(fout, "\n");
}

void printCastigatoriRunda(StackNode* stiva, FILE* fout, int runda){
    fprintf(fout, "WINNERS OF ROUND NO:%d\n", runda);
    StackNode* curent = stiva;
    while(curent != NULL){
        fprintf(fout, "%-34s-  %.2f\n", curent->val->nume_echipa, curent->val->score);
        curent = curent->next;
    }
}

void Turneu(Node* head, FILE* fout, Node** top8){
    Queue coada;
    coada.front = coada.rear = NULL;
    StackNode* stiva = NULL;

    Node* curent = head;
    while(curent != NULL && curent->next != NULL){
        enQueue(&coada, curent->team, curent->next->team);
        curent = curent->next->next;
    }

    int runda = 1;
    while(!isEmptyQueue(&coada)){
        fprintf(fout, "\n");
        printRunda(&coada, fout, runda);
        while(!isEmptyQueue(&coada)){
            QueueNode* meci = deQueue(&coada);
            Team* castigator = Match(meci->team1, meci->team2);
            push(&stiva, castigator);
            free(meci);
        }
        printCastigatoriRunda(stiva, fout, runda);
        int nrCastigatori = 0;
        StackNode* temp = stiva;
        while(temp != NULL){
            nrCastigatori++;
            temp = temp->next;
        }

    Team** castigatori = (Team**)malloc(nrCastigatori * sizeof(Team*));
    for(int i = 0; i < nrCastigatori; i++)
        castigatori[i] = pop(&stiva);

    if(nrCastigatori == 8)
        for(int i = 0; i < 8; i++){
            Team* copie = (Team*)malloc(sizeof(Team));
            copie->nume_echipa = strdup(castigatori[i]->nume_echipa);
            copie->score = castigatori[i]->score;
            copie->nrPlayers = castigatori[i]->nrPlayers;
            copie->player = NULL;
            addTeam(top8, copie);
        }

    for(int i = 0; i + 1 < nrCastigatori; i += 2)
        enQueue(&coada, castigatori[i], castigatori[i+1]);

    free(castigatori);
    runda++;
    }
}

TreeNode* insert(TreeNode* root, Team* team){
    if(root == NULL){
        TreeNode* newNode = (TreeNode*)malloc(sizeof(TreeNode));
        newNode->team = team;
        newNode->left = newNode->right = NULL;
        return newNode;
    }
    if(team->score > root->team->score)
        root->right = insert(root->right, team);
    else if(team->score < root->team->score)
        root->left = insert(root->left, team);
    else{
        if(strcmp(team->nume_echipa, root->team->nume_echipa) > 0)
            root->right = insert(root->right, team);
        else
            root->left = insert(root->left, team);
    }
    return root;
}

void printTree(TreeNode* root, FILE* fout){
    if(root == NULL) return;
    printTree(root->right, fout);
    fprintf(fout, "%-34s-  %.2f\n", root->team->nume_echipa, root->team->score);
    printTree(root->left, fout);
}

int main(int argc, char *argv[]){
    Node* head = NULL;

    FILE* fin = fopen(argv[2], "r");
    FILE* fout = fopen(argv[3], "w");
    if(fin == NULL){ printf("Eroare la deschiderea fisierului!\n"); return 1; }
    if(fout == NULL){ printf("Eroare la deschiderea fisierului de iesire!\n"); return 1; }

    FILE* finC = fopen(argv[1], "r");
    int a, b, c, d;
    fscanf(finC, "%d %d %d %d", &a, &b, &c, &d);
    fclose(finC);

    if(a == 1){
        int nrTeams, nrPlayers, scor_echipa = 0, i, j;
        fscanf(fin, "%d", &nrTeams);
        for(i = 0; i < nrTeams; i++){
            scor_echipa = 0;
            Team* team = (Team*)malloc(sizeof(Team));
            fscanf(fin, "%d", &nrPlayers);
            team->nrPlayers = nrPlayers;
            team->player = (Player*)malloc(nrPlayers * sizeof(Player));

            char buffer[100];
            fgets(buffer, 100, fin);
            buffer[strcspn(buffer, "\n")] = 0;
            int len = strlen(buffer);
            while(len > 0 && buffer[len-1] == ' ')
                buffer[--len] = 0;
            team->nume_echipa = strdup(buffer + 1);

            for(j = 0; j < nrPlayers; j++){
                char nume[50], prenume[50];
                int pct;
                fscanf(fin, "%s %s %d", nume, prenume, &pct);
                team->player[j].nume = strdup(nume);
                team->player[j].prenume = strdup(prenume);
                team->player[j].puncte = pct;
                scor_echipa = scor_echipa + pct;
            }
            team->score = (float)scor_echipa / nrPlayers;
            addTeam(&head, team);
        }

        int putere = 1;
        while(putere * 2 <= nrTeams){
            putere = putere * 2;
        }
        int deelim = nrTeams - putere;
        nrTeams = eliminareMINIM(&head, deelim, nrTeams);
        printList(head, fout);
        Node* top8 = NULL;
        if(b == 1)
            Turneu(head, fout, &top8);
        if(c == 1){
            TreeNode* root = NULL;
            Node* curent = top8;
            while(curent != NULL){
                root = insert(root, curent->team);
                curent = curent->next;
            }
            fprintf(fout, "\nTOP 8 TEAMS:\n");
            printTree(root, fout);
        }
    }
    else{
        printf("Eroare la citirea din c.in!\n");
        return 1;
    }
    fclose(fin);
    fclose(fout);
    return 0;
}