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
Auteur principal : SADEQI Muhammad Yousuf
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
Auteur principal : SADEQI Muhammad Yousuf
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
Auteur principal : SADEQI Muhammad Yousuf
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
Auteur principal : SADEQI Muhammad Yousuf
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
Auteur principal : SADEQI Muhammad Yousuf
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
        nbOperations++; // comparaison i < n

        nbOperations += 3; // acces tab + comparaison  + access champ numBus 
        if (tab[i].numBus == numBus) {
            return i;  // trouvé
        }
        i++; 
        nbOperations+=2; // incrementation
    }
    nbOperations++; // verification boucle finale
    return -1;  // pas trouvé
}

/*
Rôle : Recherche et affiche un trajet à partir du numéro de bus donné.
Préconditions : tab != NULL
Paramètre : numBus (numéro bus), tab (tableau de trajets), n (nombre de trajets)
Postconditions : Affichage du trajet ou message d'erreur.
Sortie : Aucune (void)
Auteur principal : SADEQI Muhammad Yousuf
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
Auteur principal : SADEQI Muhammad Yousuf
*/
int comparerTrajets(const Trajet *t1, const Trajet *t2) {
    nbOperations+= 3; // appel function + 2 * acces champ villeDepart sans compter déréférencement de t1 et t2
    int cmp = comparerChaines(t1->villeDepart, t2->villeDepart); 

    nbOperations++; // comparaison
    if (cmp != 0) {
        return cmp;
    }
    nbOperations++; // appel function
    return comparerDates(t1, t2); 
    // ici on ne mets pas déréférencement car t1 et t2 sont déjà déréférencés dans comparerDates()
}


/*
Rôle : Fusionne deux sous-tableaux triés pour le tri fusion.
Préconditions : Indices valides, temp assez grand
Paramètre : tab (tableau principal), deb (indice début), mil (indice milieu), fin (indice fin), temp (tableau temporaire)
Postconditions : La partie tab[deb..fin] est triée.
Sortie : Aucune (void)
Auteur principal : SADEQI Muhammad Yousuf
*/
void fusion(Trajet tab[], int deb, int mil, int fin, Trajet temp[]) {
    int i = deb; 
    nbOperations++;
    int j = mil + 1; 
    nbOperations+=2; // affectation + addiction 
    int k = deb; 
    nbOperations++;

    while (i <= mil && j <= fin) {
        nbOperations += 3; // comparaison * 2 + ET logique
        
         
        nbOperations += 4; // access tab * 2 et 1 compairson + appel function = 4 opérations
        if (comparerTrajets(&tab[i], &tab[j]) <= 0) {
            temp[k] = tab[i]; 
            k++;
            i++;
            nbOperations += 7; // access tab * 2 + 1 affectation + (2 incrementations)*2 = 7 op 
        } else {
            temp[k] = tab[j]; 
            k++;
            j++;
            nbOperations += 7; // access tab * 2 + 1 affectation + (2 incrementations)*2 = 7 op 
        }
    }
    if (i <= mil) {
        nbOperations += 3; // verification boucle finale si j <= fin est faux
    }else{
        nbOperations += 2; // verification boucle finale si i <= mil est faux
    }

    while (i <= mil) {
        nbOperations++; // comparaison
        temp[k] = tab[i];
        k++;
        i++;
        nbOperations += 7; // access tab * 2 + 1 affectation + (2 incrementations)*2 = 7 op 
    }
    nbOperations++; // verification boucle finale

    while (j <= fin) {
        nbOperations++; // comparaison

        temp[k] = tab[j];
        k++;
        j++;
        nbOperations += 7; // access tab * 2 + 1 affectation + (2 incrementations)*2 = 7 op 
    }
    nbOperations++; // verification boucle finale
    
    nbOperations++; // affectationo i = deb 
    for (i = deb; i <= fin; i++) {
        nbOperations += 3; // test, incrementation
        tab[i] = temp[i]; 
        nbOperations += 3; // access tab * 2 + 1 affectation
    }
    nbOperations++; // verification boucle finale
}

/*
Rôle : Fonction récursive du tri fusion.
Préconditions : Indices valides
Paramètre : tab (tableau à trier), deb (indice début), fin (indice fin), temp (tableau temporaire)
Postconditions : Le sous-tableau est trié.
Sortie : Aucune (void)
Auteur principal : SADEQI Muhammad Yousuf
*/
void triFusionRec(Trajet tab[], int deb, int fin, Trajet temp[]) {
    nbOperations++; // comparaison
    if (deb < fin) {
        int mil = (deb + fin) / 2;
        nbOperations += 3; // addition + division + affectation
        triFusionRec(tab, deb, mil, temp);
        nbOperations++; // appel
        triFusionRec(tab, mil + 1, fin, temp); 
        nbOperations+= 2; // appel + addiction mil + 1 
        fusion(tab, deb, mil, fin, temp); 
        nbOperations++; // appel
    }
}

/*
Rôle : Trie les trajets par ville de départ puis date (Tri Fusion).
Préconditions : tab != NULL
Paramètre : tab (tableau de trajets), n (nombre de trajets)
Postconditions : Le tableau est trié.
Sortie : Aucune (void)
Auteur principal : SADEQI Muhammad Yousuf
*/
void trierTrajets(Trajet tab[], int n) {
    nbOperations++; // comparaison
    if (n <= 1) {
        return;
    }
    Trajet *temp = tempArray; 
    nbOperations++; // affectation sans compte deferencement
    triFusionRec(tab, 0, n - 1, temp);
    nbOperations += 2; // appel + substraction n - 1 
}

/*
Rôle : Trie et affiche les trajets.
Préconditions : tab != NULL
Paramètre : tab (tableau de trajets), n (nombre de trajets)
Postconditions : Le tableau est modifié (trié) et affiché.
Sortie : Aucune (void)
Auteur principal : SADEQI Muhammad Yousuf
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
Auteur principal : SADEQI Muhammad Yousuf
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
Auteur principal : SADEQI Muhammad Yousuf
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
void filtrerComplet(const Trajet tab[], int n, const char *vd, const char *va, int j, int m, int a) {
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
Auteur principal : SADEQI Muhammad Yousuf
*/
float chiffreAffaires(const Trajet *t) {
    float recettes = 0; 
    nbOperations++;
    int i = 0; 
    nbOperations++;

    while (i < t->nbPassagers) {
        nbOperations+= 2; // comparaison et access champ nbPassagers sans compte deferencement de t 
        
        nbOperations += 6; // incrementation + affectation + acces tab + acces champ prix + access champ passagers 
        recettes += t->passagers[i].prix; 
        
        i++; 
        nbOperations+=2;
    }
    nbOperations+= 2; // verification boucle finale comparison et access champ nbPassagers

    // Horaires manquants
    nbOperations += 5; // comparaisons + OU logique + access champ heureDepart + access champ heureArrivee  
    if (t->heureDepart == -1 || t->heureArrivee == -1) {
        return CA_INVALIDE; 
    }
    
    nbOperations += 7; // affectation + acces champ heureDepart * 2 + division + multiplication + % + addiction
    int minDepart = (t->heureDepart / 100) * 60 + (t->heureDepart % 100); 
    
    nbOperations += 7; // affectation + acces champ heureArrivee * 2 + division + multiplication + % + addiction
    int minArrivee = (t->heureArrivee / 100) * 60 + (t->heureArrivee % 100); 

    nbOperations++; // comp
    if (minArrivee < minDepart) {
        minArrivee += 24 * 60; 
        nbOperations += 3; // arith + affect
    }

    // float duree = ...
    nbOperations += 3; // affectation + substraction + division
    float duree = (float)(minArrivee - minDepart) / 60.0f; 
    
    nbOperations++ ;// affectation 
    int heuresEntamees = (int)duree; // cast = op? negligible usually or +1
    
    nbOperations++; // comparison 
    if (duree > (float)heuresEntamees) { 
        heuresEntamees++; 
        nbOperations+=2;
    }
    
    nbOperations++; // comparison
    if (heuresEntamees < 1) {
        heuresEntamees = 1; 
        nbOperations++; 
    }

    // float cout = ...
    nbOperations += 3; // * + affectation + addition
    float cout = 100.0f + 50.0f * heuresEntamees; 

    // recettes - cout
    nbOperations++; // -
    return recettes - cout;
}

/*
Rôle : Fonction de partition pour le tri rapide (selon CA).
Préconditions : Indices valides
Paramètre : tab (tableau de trajets), deb (indice début), fin (indice fin)
Postconditions : Le pivot est placé à sa position finale.
Sortie :
  - Index du pivot
Auteur principal : SADEQI Muhammad Yousuf
*/
int partition(Trajet tab[], int deb, int fin) {
    float pivotCA = chiffreAffaires(&tab[fin]); 
    nbOperations+=3; // appel fonction + affectation + acces
    int i = deb - 1; 
    nbOperations+=2;

    nbOperations++ ;// affectation j = deb
    for (int j = deb; j < fin; j++) {
        nbOperations += 3; // test + increment
         
        nbOperations+=3 ;// appel fonction + acces + comp
        if (chiffreAffaires(&tab[j]) >= pivotCA) {
            i++; 
            nbOperations+=2;

            Trajet temp = tab[i];
            tab[i] = tab[j];
            tab[j] = temp;

            nbOperations += 7; // acces * 4 + affectation * 3
        }
    }
    nbOperations++; // verification boucle finale

    Trajet temp = tab[i + 1];
    tab[i + 1] = tab[fin];
    tab[fin] = temp;
    nbOperations += 9; // acces * 4 + affectation * 3 + (addiction i + 1) * 2

    nbOperations++; // return
    return i + 1; 
}

/*
Rôle : Implémentation récursive du tri rapide.
Préconditions : Indices valides
Paramètre : tab (tableau), deb (début), fin (fin)
Postconditions : Sous-tableau trié.
Sortie : Aucune (void)
Auteur principal : SADEQI Muhammad Yousuf
*/
void triRapideRec(Trajet tab[], int deb, int fin) {
    nbOperations++; // comparison 
    if (deb < fin) {
        int pi = partition(tab, deb, fin);
        nbOperations+=2; // affectation + appel fonction
        triRapideRec(tab, deb, pi - 1); 
        nbOperations+=2; // appel fonction + substraction pi - 1 
        triRapideRec(tab, pi + 1, fin); 
        nbOperations+=2; // appel fonction + addition pi + 1
    }
}

/*
Rôle : Trie les trajets par chiffre d'affaires et les affiche.
Préconditions : tab != NULL
Paramètre : tab (tableau de trajets), n (nombre de trajets)
Postconditions : Tableau trié et affiché.
Sortie : Aucune (void)
Auteur principal : SADEQI Muhammad Yousuf
*/
void afficherTriParCA(Trajet tab[], int n) {
    // Tri Rapide
    triRapideRec(tab, 0, n - 1);

    printf("\n=== TRI PAR CHIFFRE D'AFFAIRES (Decroissant) ===\n");

    for (int i = 0; i < n; i++) {
        float ca = chiffreAffaires(&tab[i]);
        
        // Affichage du CA
        if (ca == CA_INVALIDE) {
            printf("[CA:     N/A     ] ");
        } else {
            printf("[CA: %9.2f €] ", ca);
        }

        // Réutilisation de la fonction existante pour l'affichage du trajet
        afficherTrajet(&tab[i]);
    }
}


/*
Rôle : Fonction de partition pour le tri rapide des passagers (par prix décroissant).
Préconditions : Indices valides
Paramètre : tab (tableau de passagers), deb (indice début), fin (indice fin)
Postconditions : Le pivot est placé à sa position finale.
Sortie :
  - Index du pivot
Auteur principal : SADEQI Muhammad Yousuf
*/
int partitionPassagers(Passager tab[], int deb, int fin) {
    float pivotPrix = tab[fin].prix;
    nbOperations+=2; // acces + affectation
    int i = deb - 1;
    nbOperations+=2; // substraction + affectation

    nbOperations++; // affectation j = deb
    for (int j = deb; j < fin; j++) {
        nbOperations += 3; // test + increment
         
        nbOperations+=3; // acces tab + comp + access champ prix  
        if (tab[j].prix >= pivotPrix) {
            i++; 
            nbOperations+=2;

            Passager temp = tab[i];
            tab[i] = tab[j];
            tab[j] = temp;

            nbOperations += 7; // acces * 4 + affectation * 3
        }
    }
    nbOperations++; // verification boucle finale

    Passager temp = tab[i + 1];
    tab[i + 1] = tab[fin];
    tab[fin] = temp;
    nbOperations += 9; // acces * 4 + affectation * 3 + (addition i + 1) * 2

    nbOperations++; // return
    return i + 1; 
}

/*
Rôle : Implémentation récursive du tri rapide pour passagers.
Préconditions : Indices valides
Paramètre : tab (tableau passagers), deb (début), fin (fin)
Postconditions : Sous-tableau trié par prix décroissant.
Sortie : Aucune (void)
Auteur principal : SADEQI Muhammad Yousuf
*/
void triRapidePassagersRec(Passager tab[], int deb, int fin) {
    nbOperations++; // comparison 
    if (deb < fin) {
        int pi = partitionPassagers(tab, deb, fin);
        nbOperations+=2; // affectation + appel fonction
        triRapidePassagersRec(tab, deb, pi - 1); 
        nbOperations+=2; // appel fonction + substraction pi - 1 
        triRapidePassagersRec(tab, pi + 1, fin); 
        nbOperations+=2; // appel fonction + addition pi + 1
    }
}

/*
Rôle : Trie un tableau de passagers par prix décroissant (Tri Rapide).
Préconditions : tab != NULL, n >= 0
Paramètre : tab (tableau de passagers), n (nombre de passagers)
Postconditions : Le tableau est trié par prix décroissant.
Sortie : Aucune (void)
Auteur principal : SADEQI Muhammad Yousuf
*/
void triRapidePassagers(Passager tab[], int n) {
    nbOperations++; // comparison n <= 1
    if (n <= 1) {
        return;
    }
    triRapidePassagersRec(tab, 0, n - 1);
    nbOperations += 2; // appel + substraction n - 1
}


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
Auteur principal : SADEQI Muhammad Yousuf
*/
int supprimerTrajetsDeficitaires(Trajet tab[], int *n) {
    float caAvant = 0;
    nbOperations+=2; // affectation caAvant = 0 et i = 0 
    for(int i=0; i<*n; i++) {
        nbOperations+=3; // increment i, comparison et sans compte le dereferencement de n  
        float ca = chiffreAffaires(&tab[i]);
        nbOperations+=3; // appel fonction + acces + affectation 

        nbOperations++; // comparaison
        if (ca != CA_INVALIDE) {
            caAvant += ca;
            nbOperations+=2; // addition + affectation
        }
    }

    int estDeficitaire[MAX_TRAJETS]; // on compte ca 

    nbOperations++ ;// affectation  i = 0 
    for(int i=0; i<*n; i++) {
        nbOperations += 3; // increment i, comparison et sans compte le dereferencement de n 
        float ca = chiffreAffaires(&tab[i]);
        nbOperations+=3; // appel fonction + acces + affectation 

        nbOperations+=3 ;// comparaison * 2 + operation logique
        if (ca < 0 && ca != CA_INVALIDE) {
            estDeficitaire[i] = 1;
            nbOperations+= 2; // affectation + acces tab
        } else {
            estDeficitaire[i] = 0;
            nbOperations+= 2; // affectation + acces tab
        }
    }

    nbOperations++ ;// affectation  i = 0 
    for(int i=0; i<*n; i++) {
        nbOperations += 3; // increment i, comparison et sans compte le dereferencement de n 
        if (estDeficitaire[i]) {
            // Trier passagers par prix decroissant
            triRapidePassagers(tab[i].passagers, tab[i].nbPassagers);
            nbOperations+=3; // appel fonction + 2 acces (tab[i].passagers, tab[i].nbPassagers)

            nbOperations++ ;// affectation  i = 0 
            for (int p=0; p < tab[i].nbPassagers; p++) {
                nbOperations+=5; // increment p, comparison, acces tab, access nbPassagers
                
                nbOperations+=4; // acces de tab et passagers + affectation + access champe passagers = 4 
                Passager *pass = &tab[i].passagers[p];

                // Ignorer si deja deplace
                if (pass->prix < 0) {
                    continue;
                }

                nbOperations+=3; // affectation  meileurIdX, meilleurDiff et j = 0 
                int meilleurIdx = -1;
                long meilleurDiff = -1;

                for (int j=0; j<*n; j++) {
                    nbOperations+=3; // increment j, comparison et sans compte le dereferencement de n 

                    nbOperations++ ; // comparaison 
                    if (i == j) {
                        continue;
                    }

                    nbOperations++ ; // access tab 
                    if (estDeficitaire[j]) {
                        continue;
                    }
                    
                    nbOperations+=3 ; // comparaison + acces tab + access nbPassagers 
                    if (tab[j].nbPassagers >= MAX_PASSAGERS) {
                        continue;
                    }

                    // Trajets identiques : meme depart et arrivee
                    nbOperations+=13 ; // acces tab * 4 + comparaison * 2 + appel fonction * 2 + operateur logique + 4 access champ villeDepart et villeArrive
                    if (comparerChaines(tab[j].villeDepart, tab[i].villeDepart) == 0 &&
                        comparerChaines(tab[j].villeArrivee, tab[i].villeArrivee) == 0) {

                        // Proximite temporelle
                        long t1 = unifieDateHeure(&tab[i]);  
                        long t2 = unifieDateHeure(&tab[j]);

                        nbOperations+=6; // acces tab * 2 + appel fonction * 2 + affectation * 2 
                        
                        long diff; 
                        // long diff = (t1 > t2) ? (t1 - t2) : (t2 - t1); la meme chose mais pour mieux comprends on fais sa
                        if(t1 > t2){
                            diff = t1 - t2;
                        }else{
                            diff = t2 - t1;
                        }
                        nbOperations+=3 ; // affectation + subration + comparison

                        // Choisir le trajet le plus proche
                        nbOperations+=3 ; // comparison * 2 + operateur logique  
                        if (meilleurIdx == -1 || diff < meilleurDiff) {
                            meilleurDiff = diff;
                            meilleurIdx = j;
                            nbOperations+=2 ; // affectation * 2
                        }
                    }
                }
                nbOperations++ ; // validation finale de j < *n sans compte dereferencement de n 
                // Deplacer le passager si trouve
                if (meilleurIdx != -1) {
                    ajouterPassager(&tab[meilleurIdx], pass->nom, pass->prix);
                    // Marquer comme deplace
                    pass->prix = -1;
                    nbOperations+=5 ; // affectation + access + accès à un champ * 3 + appel fonction sans compte dereferencement de pass
                }
            }
        }
    }

    // supprime les bus deficients 
    nbOperations+=2 ; // affectation de newN plus affectation de i = 0 
    int newN = 0;
    for (int i=0; i<*n; i++) {
        nbOperations+=5 ; // comparison + increment + ! + access tab = 5 
        if (!estDeficitaire[i]) {
            tab[newN] = tab[i];
            newN++;
            nbOperations+=5 ; // affectation + increment + access tab * 2 = 5 
        }
    }
    *n = newN;
    nbOperations++ ; // affectation de n = newN sans coompter dereferencement de n 

    nbOperations+=2 ; // affectation de caApres = 0 + increment i = 0 
    float caApres = 0;
    for(int i=0; i<*n; i++) {
        nbOperations+=3 ; // increment i, comparison et sans compte le dereferencement de n 
        float ca = chiffreAffaires(&tab[i]);
        nbOperations+=3 ; // acces tab + affectation de ca + appel 

        nbOperations++ ; // comparaison 
        if (ca != CA_INVALIDE) {
            caApres += ca;
            nbOperations+=2 ; // increment = 2
        }
    }
    nbOperations++ ; // validation finale de i < *n sans compte dereferencement de n 

    nbOperations++; // substraction caApres - caAvant 
    return (int)(caApres - caAvant);
}

// Printf

/*
Rôle : Affiche le menu principal de l'application.
Préconditions : Aucune
Paramètre : Aucun
Postconditions : Menu affiché sur la sortie standard.
Sortie : Aucune (void)
Auteur principal : SADEQI Muhammad Yousuf
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