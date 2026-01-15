#include <stdio.h>
#include <stdlib.h>
#include "function.h"
#include "utils.h"

/*
Rôle : Parse une ligne CSV pour créer une structure Trajet.
Préconditions : ligne != NULL, ligne bien formée
Paramètre : ligne (chaîne complète du CSV)
Postconditions : Aucune.
Sortie :
  - Structure Trajet remplie avec les données lues
Auteur principal : Yousuf
*/
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
    t.jour = lireEntier(ligne, &i); 
    i++;
    t.mois = lireEntier(ligne, &i);
    i++;
    t.annee = lireEntier(ligne, &i);
    i++; 

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

/*
Rôle : Charge les trajets depuis un fichier CSV dans un tableau.
Préconditions : nom != NULL, tab != NULL, max > 0
Paramètre : nom (nom du fichier), tab (tableau destination), max (taille maximale du tableau)
Postconditions : Le tableau tab est rempli jusqu'à max éléments.
Sortie :
  - Nombre de trajets chargés
  - 0 si échec d'ouverture fichier
Auteur principal : Yousuf
*/
int chargerTrajets(const char *nom, Trajet tab[], int max) {
    FILE *f = fopen(nom, "r");
    if (!f) {
        return 0;
    }

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


/*
Rôle : Affiche les informations principales d'un trajet (sans passagers).
Préconditions : t != NULL
Paramètre : t (trajet à afficher)
Postconditions : Affichage sur la sortie standard.
Sortie : Aucune (void)
Auteur principal : Yousuf
*/
void afficherTrajet(const Trajet *t) {
    printf("Bus %d : %-20s -> %-20s | %02d/%02d/%04d | ",
        t->numBus,
        t->villeDepart,
        t->villeArrivee,
        t->jour, t->mois, t->annee);
    
    if (t->heureDepart == -1) {
        printf("Non renseigné");
    } else {
        printf("%04d", t->heureDepart);
    }
    
    printf(" -> ");
    
    if (t->heureArrivee == -1) {
        printf("Non renseigné");
    } else {
        printf("%04d", t->heureArrivee);
    }
    
    printf(" | Passagers: %d\n", t->nbPassagers);
}

/*
Rôle : Affiche un trajet avec la liste détaillée de ses passagers.
Préconditions : t != NULL
Paramètre : t (trajet à afficher)
Postconditions : Affichage sur la sortie standard.
Sortie : Aucune (void)
Auteur principal : Yousuf
*/
void afficherTrajetComplet(const Trajet *t) {
    printf("Bus %d : %s -> %s | %02d/%02d/%04d | ",
        t->numBus,
        t->villeDepart,
        t->villeArrivee,
        t->jour, t->mois, t->annee);
    
    if (t->heureDepart == -1) {
        printf("Non renseigné");
    } else {
        printf("%04d", t->heureDepart);
    }
    
    printf(" -> ");
    
    if (t->heureArrivee == -1) {
        printf("Non renseigné");
    } else {
        printf("%04d", t->heureArrivee);
    }
    
    printf("\n");
    
    if (t->nbPassagers > 0) {
        printf("  Passagers (%d):\n", t->nbPassagers);
        printf("  %-4s %-30s %s\n", "ID", "Nom", "Prix");
        printf("  %s\n", "------------------------------------------------------------");
        
        int i = 0;
        while (i < t->nbPassagers) {
            printf("  %-4d ", t->passagers[i].id);
            
            // Nom manquant
            if (t->passagers[i].nom[0] == '\0') {
                printf("%-30s ", "Non renseigné");
            } else {
                printf("%-30s ", t->passagers[i].nom);
            }
            
            // Prix manquant
            if (t->passagers[i].prix < 0) {
                printf("Non renseigné\n");
            } else {
                printf("%.2f EUR\n", t->passagers[i].prix);
            }
            
            i++;
        }
    } else {
        printf("  Aucun passager\n");
    }
}


/*
Rôle : Affiche la liste complète des trajets chargés.
Préconditions : tab != NULL, n >= 0
Paramètre : tab (tableau de trajets), n (nombre de trajets)
Postconditions : Affichage sur la sortie standard.
Sortie : Aucune (void)
Auteur principal : Yousuf
*/
void afficherTous(const Trajet tab[], int n) {
    for (int i = 0; i < n; i++) {
        printf("[%d] ", i + 1);
        afficherTrajetComplet(&tab[i]);
        printf("\n");
    }
}


// fin de la question 1


/*
Rôle : Recherche l'index d'un trajet par son numéro de bus.
Préconditions : tab != NULL, n >= 0
Paramètre : numBus (numéro à chercher), tab (tableau de trajets), n (nombre de trajets)
Postconditions : Aucune.
Sortie :
  - Index du trajet si trouvé
  - -1 si non trouvé
Auteur principal : Amdjed LOUCIF
*/
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

/*
Rôle : Recherche et affiche un trajet à partir du numéro de bus donné.
Préconditions : tab != NULL
Paramètre : numBus (numéro bus), tab (tableau de trajets), n (nombre de trajets)
Postconditions : Affichage du trajet ou message d'erreur.
Sortie : Aucune (void)
Auteur principal : Amdjed LOUCIF
*/
void afficherTrajetDepuisNumBus(int numBus, const Trajet tab[], int n) {
    int index = trouverTrajet(numBus, tab, n);

    if (index == -1) {
        printf("Aucun trajet avec le bus %d.\n", numBus);
    } else {
        afficherTrajet(&tab[index]);
    }
}

// question 3

/*
Rôle : Compare deux trajets (ville départ puis date).
Préconditions : t1 != NULL, t2 != NULL
Paramètre : t1 (premier trajet), t2 (deuxième trajet)
Postconditions : Aucune.
Sortie :
  - -1 si t1 < t2
  - 1 si t1 > t2
  - 0 si t1 == t2
Auteur principal : Yousuf
*/
int comparerTrajets(const Trajet *t1, const Trajet *t2) {
    int cmp = comparerChaines(t1->villeDepart, t2->villeDepart);

    if (cmp != 0) {
        return cmp;
    }

    return comparerDates(t1, t2);
}


/*
Rôle : Fusionne deux sous-tableaux triés pour le tri fusion.
Préconditions : Indices valides, temp assez grand
Paramètre : tab (tableau principal), deb (indice début), mil (indice milieu), fin (indice fin), temp (tableau temporaire)
Postconditions : La partie tab[deb..fin] est triée.
Sortie : Aucune (void)
Auteur principal : Yousuf
*/
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

/*
Rôle : Fonction récursive du tri fusion.
Préconditions : Indices valides
Paramètre : tab (tableau à trier), deb (indice début), fin (indice fin), temp (tableau temporaire)
Postconditions : Le sous-tableau est trié.
Sortie : Aucune (void)
Auteur principal : Yousuf
*/
void triFusionRec(Trajet tab[], int deb, int fin, Trajet temp[]) {
    if (deb < fin) {
        int mil = (deb + fin) / 2;
        triFusionRec(tab, deb, mil, temp);
        triFusionRec(tab, mil + 1, fin, temp);
        fusion(tab, deb, mil, fin, temp);
    }
}


/*
Rôle : Trie les trajets par ville de départ puis date (Tri Fusion).
Préconditions : tab != NULL
Paramètre : tab (tableau de trajets), n (nombre de trajets)
Postconditions : Le tableau est trié.
Sortie : Aucune (void)
Auteur principal : Yousuf
*/
void trierTrajets(Trajet tab[], int n) {
    if (n <= 1) {
        return;
    }

    Trajet *temp = tempArray;        

    triFusionRec(tab, 0, n - 1, temp);
}

/*
Rôle : Trie et affiche les trajets.
Préconditions : tab != NULL
Paramètre : tab (tableau de trajets), n (nombre de trajets)
Postconditions : Le tableau est modifié (trié) et affiché.
Sortie : Aucune (void)
Auteur principal : Yousuf
*/
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
/*
Rôle : Ajoute un passager à un trajet.
Préconditions : t != NULL, nom != NULL
Paramètre : t (trajet cible), nom (nom du passager), prix (prix du billet)
Postconditions : Passager ajouté si place disponible.
Sortie :
  - 1 si succès
  - 0 si échec (bus complet)
Auteur principal : Yousuf
*/
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


/*
Rôle : Modifie les informations d'un passager existant.
Préconditions : t != NULL
Paramètre : t (trajet contenant le passager), id (ID du passager), nouveauNom (nouveau nom, ou NULL), nouveauPrix (nouveau prix, ou -1)
Postconditions : Passager modifié si trouvé.
Sortie : Aucune (void)
Auteur principal : Yousuf
*/
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

/*
Rôle : Sauvegarde tous les trajets dans un fichier CSV.
Préconditions : nom != NULL, tab != NULL
Paramètre : nom (nom du fichier), tab (tableau de trajets), n (nombre de trajets)
Postconditions : Fichier créé ou écrasé avec les données.
Sortie : Aucune (void)
Auteur principal : Ricardo
*/
void sauvegarderTrajets(const char *nom, Trajet tab[], int n) {
    FILE *f = fopen(nom, "w");
    if (!f) {
        return;
    }

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

/*
Rôle : Détermine si un trajet arrive le lendemain (heure arrivée < heure départ).
Préconditions : t != NULL
Paramètre : t (trajet)
Postconditions : Aucune.
Sortie :
  - 1 si le trajet arrive le lendemain
  - 0 sinon
Auteur principal : Amdjed LOUCIF
*/
int arriveLeLendemain(const Trajet *t) {
    return t->heureArrivee < t->heureDepart;
}

/*
Rôle : Affiche les trajets correspondant à une ville de départ et une date.
Préconditions : tab != NULL, ville != NULL
Paramètre : tab (tableau de trajets), n (nombre de trajets), ville (ville départ recherchée), j, m, a (jour, mois, année)
Postconditions : Affichage des trajets trouvés.
Sortie : Aucune (void)
Auteur principal : Amdjed LOUCIF
*/
void filtrerVilleDate(const Trajet tab[], int n, const char *ville, int j, int m, int a) {
    int trouve = 0;
    int i = 0;
    while (i < n) {
        if (comparerChaines(tab[i].villeDepart, ville) == 0 &&
            tab[i].jour == j && tab[i].mois == m && tab[i].annee == a) {
            afficherTrajet(&tab[i]);
            trouve = 1;
        }
        i = i + 1;
    }
    
    if (trouve == 0) {
        printf("Aucun trajet trouvé pour %s le %02d/%02d/%04d\n", ville, j, m, a);
    }
}

// Question 8

/*
Rôle : Filtre les trajets par ville de départ, ville d'arrivée et date.
Préconditions : tab != NULL, vd != NULL, va != NULL
Paramètre : tab (tableau de trajets), n (nombre de trajets), vd (ville départ), va (ville arrivée), j, m, a (date)
Postconditions : Affichage des trajets trouvés.
Sortie : Aucune (void)
Auteur principal : Ricardo
*/
void filtrerComplet(const Trajet tab[], int n,
                    const char *vd, const char *va,
                    int j, int m, int a) {
    int trouve = 0;
    int i = 0;
    while (i < n) {
        if (comparerChaines(tab[i].villeDepart, vd) == 0 &&
            comparerChaines(tab[i].villeArrivee, va) == 0 &&
            tab[i].jour == j && tab[i].mois == m && tab[i].annee == a) {
            afficherTrajet(&tab[i]);
            trouve = 1;
        }
        i++;
    }
    
    if (trouve == 0) {
        printf("Aucun trajet trouvé de %s vers %s le %02d/%02d/%04d\n", vd, va, j, m, a);
    }
}

// Question 9

/*
Rôle : Calcule le chiffre d'affaires d'un trajet.
Préconditions : t != NULL, 0 <= t->nbPassagers <= MAX_PASSAGERS
Paramètre : t (trajet valide)
Postconditions : Aucun effet de bord.
Sortie :
  - CA du trajet si calculable
  - CA_INVALIDE si horaires manquants
Auteur principal : Yousuf
*/
float chiffreAffaires(const Trajet *t) {
    float recettes = 0;
    int i = 0;

    while (i < t->nbPassagers) {
        recettes += t->passagers[i].prix;
        i++;
    }

    // Horaires manquants : calcul impossible
    if (t->heureDepart == -1 || t->heureArrivee == -1) {
        return CA_INVALIDE;  // Sentinel value for invalid calculation
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
    
    if (heuresEntamees < 1) {
        heuresEntamees = 1;
    }

    float cout = 100.0f + 50.0f * heuresEntamees;

    return recettes - cout;
}


/*
Rôle : Compare le CA de deux trajets (pour tri décroissant).
Préconditions : a != NULL, b != NULL
Paramètre : a (premier trajet), b (deuxième trajet)
Postconditions : Aucune.
Sortie :
  - 1 si CA a < CA b (a doit aller après b)
  - -1 si CA a > CA b (a doit aller avant b)
  - 0 si égaux
Auteur principal : Yousuf
*/
int comparerCA(const void *a, const void *b) {
    const Trajet *t1 = (const Trajet *)a;
    const Trajet *t2 = (const Trajet *)b;
    
    float ca1 = chiffreAffaires(t1);
    float ca2 = chiffreAffaires(t2);
    
    if (ca1 < ca2) {
        return 1;
    }
    if (ca1 > ca2) {
        return -1;
    }
    return 0;
}


/*
Rôle : Fonction de partition pour le tri rapide (selon CA).
Préconditions : Indices valides
Paramètre : tab (tableau de trajets), deb (indice début), fin (indice fin)
Postconditions : Le pivot est placé à sa position finale.
Sortie :
  - Index du pivot
Auteur principal : Yousuf
*/
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

/*
Rôle : Implémentation récursive du tri rapide.
Préconditions : Indices valides
Paramètre : tab (tableau), deb (début), fin (fin)
Postconditions : Sous-tableau trié.
Sortie : Aucune (void)
Auteur principal : Yousuf
*/
void triRapideRec(Trajet tab[], int deb, int fin) {
    if (deb < fin) {
        int pi = partition(tab, deb, fin);
        triRapideRec(tab, deb, pi - 1);
        triRapideRec(tab, pi + 1, fin);
    }
}

/*
Rôle : Trie les trajets par chiffre d'affaires et les affiche.
Préconditions : tab != NULL
Paramètre : tab (tableau de trajets), n (nombre de trajets)
Postconditions : Tableau trié et affiché.
Sortie : Aucune (void)
Auteur principal : Yousuf
*/
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

/*
Rôle : Supprime les trajets déficitaires et redistribue leurs passagers.
Préconditions : tab != NULL, n != NULL
Paramètre : tab (tableau de trajets), n (pointeur vers nombre de trajets, sera mis à jour)
Postconditions :
  - Les trajets déficitaires sont supprimés
  - Le nombre de trajets (*n) est mis à jour
  - Les passagers sont redistribués vers d'autres trajets si possible
Sortie :
  - Gain total généré par l'opération (différence CA après/avant)
Auteur principal : Yousuf
*/
int supprimerTrajetsDeficitaires(Trajet tab[], int *n) {
    float caAvant = 0;
    for(int i=0; i<*n; i++) {
        caAvant += chiffreAffaires(&tab[i]);
    }

    int estDeficitaire[MAX_TRAJETS];
    for(int i=0; i<*n; i++) {
        if (chiffreAffaires(&tab[i]) < 0) {
            estDeficitaire[i] = 1;
        } else {
            estDeficitaire[i] = 0;
        }
    }

    for(int i=0; i<*n; i++) {
        if (estDeficitaire[i]) {
            // Trier passagers par prix decroissant
            qsort(tab[i].passagers, tab[i].nbPassagers, sizeof(Passager), comparePassagersPrixDesc);

            for (int p=0; p < tab[i].nbPassagers; p++) {
                Passager *pass = &tab[i].passagers[p];

                // Ignorer si deja deplace
                if (pass->prix < 0) {
                    continue;
                }

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

                    // Trajets identiques : meme depart et arrivee
                    if (comparerChaines(tab[j].villeDepart, tab[i].villeDepart) == 0 &&
                        comparerChaines(tab[j].villeArrivee, tab[i].villeArrivee) == 0) {

                        // Proximite temporelle
                        long t1 = unifieDateHeure(&tab[i]);
                        long t2 = unifieDateHeure(&tab[j]);
                        long diff = (t1 > t2) ? (t1 - t2) : (t2 - t1);

                        // Choisir le trajet le plus proche
                        if (meilleurIdx == -1 || diff < meilleurDiff) {
                            meilleurDiff = diff;
                            meilleurIdx = j;
                        }
                    }
                }

                // Deplacer le passager si trouve
                if (meilleurIdx != -1) {
                    ajouterPassager(&tab[meilleurIdx], pass->nom, pass->prix);
                    // Marquer comme deplace
                    pass->prix = -1;
                }
            }
        }
    }

    // supprime les bus deficients 
    int newN = 0;
    for (int i=0; i<*n; i++) {
        if (!estDeficitaire[i]) {
            tab[newN] = tab[i];
            newN++;
        }
    }
    *n = newN;

    float caApres = 0;
    for(int i=0; i<*n; i++) {
        caApres += chiffreAffaires(&tab[i]);
    }

    return (int)(caApres - caAvant);
}



// Printf

/*
Rôle : Affiche le menu principal de l'application.
Préconditions : Aucune
Paramètre : Aucun
Postconditions : Menu affiché sur la sortie standard.
Sortie : Aucune (void)
Auteur principal : Yousuf
*/
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