#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CARTI 100
#define MAX_IMPRUMUT 10

#define RESET   "\033[0m"
#define ROSU     "\033[31m"
#define VERDE   "\033[32m"
#define GALBEN  "\033[33m"
#define ALBASTRU    "\033[34m"
#define CYAN    "\033[36m"
#define ALB "\033[1m\033[37m"

typedef struct {
    char titlu[101];
    char autor[101];
    char tip[51];
    char locatie[101];
    int disponibilitate;
} Carte;

Carte Biblioteca[MAX_CARTI];
int nrCarti;

void stergeEcran() {
    system("cls");
}

void citireDinFisier(char path[])
{
    FILE *filePointer = fopen(path, "r");
    if (filePointer == NULL)
    {
        printf(ROSU "Fisierul nu poate fi accesat.\n" RESET);
        return;
    }

    nrCarti = 0;
    int idx;
    while (fscanf(filePointer, "%d | %100[^|] | %100[^|] | %50[^|] | %100[^|] | %d\n",
                  &idx,
                  Biblioteca[nrCarti].titlu,
                  Biblioteca[nrCarti].autor,
                  Biblioteca[nrCarti].tip,
                  Biblioteca[nrCarti].locatie,
                  &Biblioteca[nrCarti].disponibilitate) == 6)
    {
        nrCarti++;
    }

    fclose(filePointer);
}

void adaugaCarte(char path[])
{
    stergeEcran();

    if (nrCarti >= MAX_CARTI)
    {
        printf(ROSU "Nu se mai pot adauga carti!\n" RESET);
        getchar();
        return;
    }

    printf("===== " CYAN "Adauga Carte" RESET " =====\n");

    printf(CYAN "Titlu: " RESET);
    fgets(Biblioteca[nrCarti].titlu, sizeof(Biblioteca[nrCarti].titlu), stdin);
    Biblioteca[nrCarti].titlu[strcspn(Biblioteca[nrCarti].titlu, "\n")] = 0;

    printf(CYAN "Autor: " RESET);
    fgets(Biblioteca[nrCarti].autor, sizeof(Biblioteca[nrCarti].autor), stdin);
    Biblioteca[nrCarti].autor[strcspn(Biblioteca[nrCarti].autor, "\n")] = 0;

    printf(CYAN "Tip: " RESET);
    fgets(Biblioteca[nrCarti].tip, sizeof(Biblioteca[nrCarti].tip), stdin);
    Biblioteca[nrCarti].tip[strcspn(Biblioteca[nrCarti].tip, "\n")] = 0;

    printf(CYAN "Locatie: " RESET);
    fgets(Biblioteca[nrCarti].locatie, sizeof(Biblioteca[nrCarti].locatie), stdin);
    Biblioteca[nrCarti].locatie[strcspn(Biblioteca[nrCarti].locatie, "\n")] = 0;

    Biblioteca[nrCarti].disponibilitate = 1;

    FILE *filePointer = fopen(path, "a");
    if (filePointer == NULL)
    {
        printf(ROSU "Fisierul nu poate fi accesat!\n" RESET);
        return;
    }

    fprintf(filePointer, "%d | %s | %s | %s | %s | %d\n", nrCarti + 1,
            Biblioteca[nrCarti].titlu,
            Biblioteca[nrCarti].autor,
            Biblioteca[nrCarti].tip,
            Biblioteca[nrCarti].locatie,
            Biblioteca[nrCarti].disponibilitate);

    fclose(filePointer);

    nrCarti++;

    printf(VERDE "Carte adaugata cu succes!\n" RESET);
    getchar();
}


void listeazaCarti(char path[])
{
    stergeEcran();

    FILE *filePointer;
    filePointer=fopen(path,"r");

    if (filePointer==NULL)
    {
        printf(ROSU "Fisierul nu poate fi accesat." RESET);
        return;
    }

    else
    {
        char linie[100];

        printf("=============== " CYAN "Lista Cartilor" RESET " ===============\n");

        while(fgets(linie,100,filePointer))
        {
            if(linie[0]!='\n')
            {
                linie[strcspn(linie,"\n")]='\0';
                printf("%s\n",linie);
            }
        }
        fclose(filePointer);
        printf("Apasati " CYAN "Enter" RESET " pentru a continua...");
        getchar();
    }
}

void imprumutaCarte(char path[])
{
    stergeEcran();

    citireDinFisier(path);

    printf("========== " CYAN "Imprumuta Carte" RESET "==========\n");
    listeazaCarti(path);

    int index;
    printf("Introduceti " ALBASTRU "numarul cartii" RESET " pe care doriti sa o " GALBEN "imprumutati" RESET ": ");
    scanf("%d", &index);
    getchar();
    index--;

    if (index >= 0 && index < nrCarti && Biblioteca[index].disponibilitate == 1)
    {
        Biblioteca[index].disponibilitate = 0;

        FILE *tempfile = fopen("temp.txt", "w");
        if (tempfile == NULL)
        {
            printf(ROSU "Eroare la crearea fisierului temporar.\n" RESET);
            return;
        }

        for (int i = 0; i < nrCarti; i++)
        {
            fprintf(tempfile, "%d | %s | %s | %s | %s | %d\n",
                    i + 1,
                    Biblioteca[i].titlu,
                    Biblioteca[i].autor,
                    Biblioteca[i].tip,
                    Biblioteca[i].locatie,
                    Biblioteca[i].disponibilitate);
        }

        fclose(tempfile);

        remove(path);
        rename("temp.txt", path);

        printf("Ati " GALBEN "imprumutat" RESET " cartea pentru " ALBASTRU "%d zile" RESET ".\n", MAX_IMPRUMUT);
    }
    else
    {
        printf(ROSU "Carte indisponibila sau index invalid!\n" RESET);
    }

    getchar();
}

void returneazaCarte(char path[])
{
    stergeEcran();

    citireDinFisier(path);

    printf("========== " CYAN "Returneaza Carte" RESET " ==========n");
    listeazaCarti(path);

    int index;
    printf("Introduceti " ALBASTRU "numarul cartii" RESET " pe care doriti sa o " GALBEN "returnati" RESET ": ");
    scanf("%d", &index);
    getchar();
    index--;

    if (index >= 0 && index < nrCarti && Biblioteca[index].disponibilitate == 0)
    {
        Biblioteca[index].disponibilitate = 1;

        FILE *tempfile = fopen("temp.txt", "w");
        if (tempfile == NULL)
        {
            printf(ROSU "Eroare la crearea fisierului temporar.\n"RESET);
            return;
        }

        for (int i = 0; i < nrCarti; i++)
        {
            fprintf(tempfile, "%d | %s | %s | %s | %s | %d\n",
                    i + 1,
                    Biblioteca[i].titlu,
                    Biblioteca[i].autor,
                    Biblioteca[i].tip,
                    Biblioteca[i].locatie,
                    Biblioteca[i].disponibilitate);
        }

        fclose(tempfile);

        remove(path);
        rename("temp.txt", path);

        printf(VERDE "Carte returnata cu succes!\n" RESET);
    }
    else
    {
        printf(ROSU "Aceasta carte nu era imprumutata sau index invalid!\n" RESET);
    }

    getchar();
}


void meniu(char path[])
{
    citireDinFisier(path);
    int alege;
    do {
        stergeEcran();
        printf("===== " CYAN "Meniu Biblioteca" RESET " =====\n");
        printf("1. " GALBEN "Adauga Carte\n" RESET);
        printf("2. " GALBEN "Afiseaza Carti\n" RESET);
        printf("3. " GALBEN "Imprumuta Carte\n" RESET);
        printf("4. " GALBEN "Returneaza Carte\n" RESET);
        printf("5. " GALBEN "Iesire\n" RESET);
        printf(CYAN "Alegeti o optiune: " RESET);
        scanf("%d", &alege);
        getchar();
        switch (alege)
        {
            case 1: adaugaCarte(path); break;
            case 2: listeazaCarti(path); break;
            case 3: imprumutaCarte(path); break;
            case 4: returneazaCarte(path); break;
            case 5: printf(VERDE "Iesire...\n" RESET); break;
            default: printf(ROSU "Optiune invalida!\n" RESET);
        }
    } while (alege != 5);
}

int main()
{
    char path[]="Raft.txt";
    meniu(path);
}
