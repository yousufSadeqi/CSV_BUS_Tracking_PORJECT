#include <stdio.h>
#include <stdlib.h>
#include "function.h"
#include "utils.h"


Trajet lireTrajet(const char *ligne) {
    Trajet t;
    int i = 0;

    // numéro bus
    t.numBus = lireEntier(ligne, &i);
    i++; 

    // ville départ
    lireChamp(ligne, &i, t.villeDepart);

    // ville arrivée
    lireChamp(ligne, &i, t.villeArrivee);

    // date JJ/MM/AAAA
    t.jour = lireEntier(ligne, &i); i++;
    t.mois = lireEntier(ligne, &i); i++;
    t.annee = lireEntier(ligne, &i); i++; 

    // horaire départ
    t.heureDepart = lireEntier(ligne, &i);
    i++; 

    t.heureArrivee = lireEntier(ligne, &i);
    
    t.nbPassagers = 0;

    while (ligne[i] == ',' && t.nbPassagers < MAX_PASSAGERS) {
        i++; 
        if (ligne[i] == '\0' || ligne[i] == '\n') break;

        t.passagers[t.nbPassagers].id = lireEntier(ligne, &i);
        
        if (ligne[i] == ':') i++; 
        
        int k = 0;
        while (ligne[i] != ':' && ligne[i] != '\0') {
            t.passagers[t.nbPassagers].nom[k++] = ligne[i++];
        }
        t.passagers[t.nbPassagers].nom[k] = '\0';

        if (ligne[i] == ':') i++; 

        t.passagers[t.nbPassagers].prix = lireReel(ligne, &i);
        
        t.nbPassagers++;
    }

    return t;
}

int chargerTrajets(const char *nom, Trajet tab[], int max) {
    FILE *f = fopen(nom, "r");
    if (!f) return 0;

    char ligne[4096];
    int n = 0;

    while (fgets(ligne, sizeof(ligne), f) && n < max) {
        // enlever \n
        int len = 0;
        while (ligne[len] != '\0') {
            if (ligne[len] == '\n') ligne[len] = '\0';
            len++;
        }

        tab[n] = lireTrajet(ligne);
        n++;
    }

    fclose(f);
    return n;
}


void afficherTrajet(const Trajet *t) {
    printf(
        "Bus %d : %-20s -> %-20s | %02d/%02d/%04d | %04d -> %04d | Passagers: %d\n",
        t->numBus,
        t->villeDepart,
        t->villeArrivee,
        t->jour, t->mois, t->annee,
        t->heureDepart,
        t->heureArrivee,
        t->nbPassagers
    );
}

void afficherTrajetComplet(const Trajet *t) {
    printf(
        "Bus %d : %s -> %s | %02d/%02d/%04d | %04d -> %04d\n",
        t->numBus,
        t->villeDepart,
        t->villeArrivee,
        t->jour, t->mois, t->annee,
        t->heureDepart,
        t->heureArrivee
    );
    
    // Display passengers if any
    if (t->nbPassagers > 0) {
        printf("  Passagers (%d):\n", t->nbPassagers);
        printf("  %-4s %-30s %s\n", "ID", "Nom", "Prix");
        printf("  %s\n", "------------------------------------------------------------");
        
        int i = 0;
        while (i < t->nbPassagers) {
            printf("  %-4d %-30s %.2f EUR\n", 
                   t->passagers[i].id, 
                   t->passagers[i].nom, 
                   t->passagers[i].prix);
            i++;
        }
    } else {
        printf("  Aucun passager\n");
    }
}


void afficherTous(const Trajet tab[], int n) {
    for (int i = 0; i < n; i++) {
        printf("[%d] ", i + 1);
        afficherTrajetComplet(&tab[i]);
        printf("\n");
    }
}


// fin de la question 1


int trouverTrajet(int numBus, const Trajet tab[], int n) {
    int i = 0;
    while (i < n) {
        if (tab[i].numBus == numBus) {
            return i;  // trouvé
        }
        i++;
    }
    return -1;  // pas trouvé
}

void afficherTrajetDepuisNumBus(int numBus, const Trajet tab[], int n) {
    int index = trouverTrajet(numBus, tab, n);

    if (index == -1) {
        printf("Aucun trajet avec le bus %d.\n", numBus);
    } else {
        afficherTrajet(&tab[index]);
    }
}


// question 3






int comparerTrajets(const Trajet *t1, const Trajet *t2) {
    int cmp = comparerChaines(t1->villeDepart, t2->villeDepart);

    if (cmp != 0) return cmp;

    return comparerDates(t1, t2);
}


void fusion(Trajet tab[], int deb, int mil, int fin, Trajet temp[]) {
    int i = deb;
    int j = mil + 1;
    int k = deb;

    while (i <= mil && j <= fin) {
        if (comparerTrajets(&tab[i], &tab[j]) <= 0) {
            temp[k++] = tab[i++];
        } else {
            temp[k++] = tab[j++];
        }
    }

    while (i <= mil) {
        temp[k++] = tab[i++];
    }

    while (j <= fin) {
        temp[k++] = tab[j++];
    }

    for (i = deb; i <= fin; i++) {
        tab[i] = temp[i];
    }
}

void triFusionRec(Trajet tab[], int deb, int fin, Trajet temp[]) {
    if (deb < fin) {
        int mil = (deb + fin) / 2;
        triFusionRec(tab, deb, mil, temp);
        triFusionRec(tab, mil + 1, fin, temp);
        fusion(tab, deb, mil, fin, temp);
    }
}

void trierTrajets(Trajet tab[], int n) {
    if (n <= 1) return;

    Trajet *temp = (Trajet *)malloc(n * sizeof(Trajet));
    if (temp == NULL) {
        printf("Erreur allocation memoire pour le tri fusion.\n");
        return;
    }

    triFusionRec(tab, 0, n - 1, temp);

    free(temp);
}

void afficherTrajetsTries(Trajet tab[], int n) {
    trierTrajets(tab, n);

    printf("l'affichage de tous les trajets triés par ville de départ et par date de départ\n");

    int i = 0;
    while (i < n) {
        afficherTrajet(&tab[i]);
        i++;
    }
}

// question 4
int ajouterPassager(Trajet *t, const char *nom, float prix) {
    if (t->nbPassagers >= MAX_PASSAGERS)
        return 0;

    int id = t->nbPassagers + 1;
    t->passagers[t->nbPassagers].id = id;

    int i = 0;
    while (nom[i] != '\0' && i < 49) { 
        t->passagers[t->nbPassagers].nom[i] = nom[i];
        i++;
    }
    t->passagers[t->nbPassagers].nom[i] = '\0';

    t->passagers[t->nbPassagers].prix = prix;
    t->nbPassagers++;

    return 1;
}



// Quesiton 5


void modifierPassager(Trajet *t, int id, const char *nouveauNom, float nouveauPrix) {
    int i = 0;

    while (i < t->nbPassagers) {
        if (t->passagers[i].id == id) {

            if (nouveauNom != NULL) {
                int j = 0;
                while (nouveauNom[j] != '\0' && j < 49) { 
                    t->passagers[i].nom[j] = nouveauNom[j];
                    j++;
                }
                t->passagers[i].nom[j] = '\0';
            }

            if (nouveauPrix >= 0) {
                t->passagers[i].prix = nouveauPrix;
            }

            return;
        }
        i++;
    }
}


// Quesiton 6

void sauvegarderTrajets(const char *nom, Trajet tab[], int n) {
    FILE *f = fopen(nom, "w");
    if (!f) return;

    int i = 0;
    while (i < n) {
        fprintf(f, "%d,%s,%s,%02d/%02d/%04d,%04d,%04d",
                tab[i].numBus,
                tab[i].villeDepart,
                tab[i].villeArrivee,
                tab[i].jour, tab[i].mois, tab[i].annee,
                tab[i].heureDepart,
                tab[i].heureArrivee);

        int j = 0;
        while (j < tab[i].nbPassagers) {
            fprintf(f, ",%d:%s:%.2f",
                    tab[i].passagers[j].id,
                    tab[i].passagers[j].nom,
                    tab[i].passagers[j].prix);
            j++;
        }

        fprintf(f, "\n");
        i++;
    }

    fclose(f);
}


//  Question 7


int arriveLeLendemain(const Trajet *t) {
    return t->heureArrivee < t->heureDepart;
}

void filtrerVilleDate(const Trajet tab[], int n, const char *ville, int j, int m, int a) {
    int i = 0;
    while (i < n) {
        if (comparerChaines(tab[i].villeDepart, ville) == 0 &&
            tab[i].jour == j && tab[i].mois == m && tab[i].annee == a) {
            afficherTrajet(&tab[i]);
        }
        i++;
    }
}

// Question 8

void filtrerComplet(const Trajet tab[], int n,
                    const char *vd, const char *va,
                    int j, int m, int a) {
    int i = 0;
    while (i < n) {
        if (comparerChaines(tab[i].villeDepart, vd) == 0 &&
            comparerChaines(tab[i].villeArrivee, va) == 0 &&
            tab[i].jour == j && tab[i].mois == m && tab[i].annee == a) {
            afficherTrajet(&tab[i]);
        }
        i++;
    }
}

// Question 9

float chiffreAffaires(const Trajet *t) {
    float recettes = 0;
    int i = 0;

    while (i < t->nbPassagers) {
        recettes += t->passagers[i].prix;
        i++;
    }


    int minDepart = (t->heureDepart / 100) * 60 + (t->heureDepart % 100);
    int minArrivee = (t->heureArrivee / 100) * 60 + (t->heureArrivee % 100);

    if (minArrivee < minDepart) {
        minArrivee += 24 * 60;
    }

    float duree = (float)(minArrivee - minDepart) / 60.0f;
    int heuresEntamees = (int)duree; 
    if (duree > (float)heuresEntamees) { 
        heuresEntamees++; 
    }
    
    if (heuresEntamees < 1) heuresEntamees = 1;

    float cout = 100.0f + 50.0f * heuresEntamees;

    return recettes - cout;
}


int comparerCA(const void *a, const void *b) {
    const Trajet *t1 = (const Trajet *)a;
    const Trajet *t2 = (const Trajet *)b;
    
    float ca1 = chiffreAffaires(t1);
    float ca2 = chiffreAffaires(t2);
    
    if (ca1 < ca2) return 1;
    if (ca1 > ca2) return -1;
    return 0;
}


int partition(Trajet tab[], int deb, int fin) {
    float pivotCA = chiffreAffaires(&tab[fin]);
    int i = deb - 1;

    for (int j = deb; j < fin; j++) {
        if (chiffreAffaires(&tab[j]) >= pivotCA) {
            i++;
            Trajet temp = tab[i];
            tab[i] = tab[j];
            tab[j] = temp;
        }
    }

    Trajet temp = tab[i + 1];
    tab[i + 1] = tab[fin];
    tab[fin] = temp;

    return i + 1;
}

void triRapideRec(Trajet tab[], int deb, int fin) {
    if (deb < fin) {
        int pi = partition(tab, deb, fin);
        triRapideRec(tab, deb, pi - 1);
        triRapideRec(tab, pi + 1, fin);
    }
}

void afficherTriParCA(Trajet tab[], int n) {
    // Tri Rapide
    triRapideRec(tab, 0, n - 1);
    
    printf(" TRAJETS TRIES PAR CHIFFRE D'AFFAIRES \n");
    for (int i = 0; i < n; i++) {
        float ca = chiffreAffaires(&tab[i]);
        printf("Bus %d (%s->%s) : %.2f Euros\n", tab[i].numBus, tab[i].villeDepart, tab[i].villeArrivee, ca);
    }
}


// Attention: qsort demande int (*)(const void*, const void*)





int supprimerTrajetsDeficitaires(Trajet tab[], int *n) {
    float caAvant = 0;
    for(int i=0; i<*n; i++) caAvant += chiffreAffaires(&tab[i]);

    int estDeficitaire[MAX_TRAJETS]; // 1 si ou, 0 si non
    for(int i=0; i<*n; i++) {
        if (chiffreAffaires(&tab[i]) < 0) estDeficitaire[i] = 1;
        else estDeficitaire[i] = 0;
    }

    for(int i=0; i<*n; i++) {
        if (estDeficitaire[i]) {
            qsort(tab[i].passagers, tab[i].nbPassagers, sizeof(Passager), comparePassagersPrixDesc);

            for (int p=0; p < tab[i].nbPassagers; p++) {
                Passager *pass = &tab[i].passagers[p];
                
                int meilleurIdx = -1;
                long meilleurDiff = -1;
                
                for (int j=0; j<*n; j++) {
                    if (i == j) {
                        continue;
                    } 
                    if (estDeficitaire[j]) {
                        continue;
                    } 
                    if (tab[j].nbPassagers >= MAX_PASSAGERS) {
                        continue;
                    }

                    // "Trajets identiques" : meme ville depart et arrivee
                    if (comparerChaines(tab[j].villeDepart, tab[i].villeDepart) == 0 &&
                        comparerChaines(tab[j].villeArrivee, tab[i].villeArrivee) == 0) {
                        
                        // Calcul proximité temporelle
                        long t1 = unifieDateHeure(&tab[i]);
                        long t2 = unifieDateHeure(&tab[j]);
                        long diff = (t1 > t2) ? (t1 - t2) : (t2 - t1);

                        if (meilleurIdx == -1 || diff < meilleurDiff) {
                            meilleurDiff = diff;
                            meilleurIdx = j;
                        }
                    }
                }

                // Si trouvé, on déplace
                if (meilleurIdx != -1) {
                    ajouterPassager(&tab[meilleurIdx], pass->nom, pass->prix);
                }
            }
        }
    }

    int newN = 0;
    for (int i=0; i<*n; i++) {
        if (!estDeficitaire[i]) {
            tab[newN] = tab[i];
            newN++;
        }
    }
    *n = newN;

    float caApres = 0;
    for(int i=0; i<*n; i++) caApres += chiffreAffaires(&tab[i]);

    return (int)(caApres - caAvant); // Gain généré
}



// Printf

void afficherMenu() {
    printf("\n===== MENU =====\n");
    printf("1 - Afficher tous les trajets\n");
    printf("2 - Afficher un trajet par numero de bus\n");
    printf("3 - Afficher les trajets tries (ville + date)\n");
    printf("4 - Ajouter un passager a un bus\n");
    printf("5 - Modifier un passager\n");
    printf("6 - Sauvegarder les donnees\n");
    printf("7 - Filtrer trajets (ville/date/lendemain)\n");
    printf("8 - Filtre complet (ville depart + arrivee + date)\n");
    printf("9 - Afficher chiffre d'affaires des trajets\n");
    printf("10 - Supprimer trajets deficitaires (et redistribuer)\n");
    printf("Autre - Quitter\n");
    printf("Votre choix : ");
}