/// Les bibliotheques standards necessaires pour ce programme
///***********************************************************

#include <stdio.h> // Entrees/sorties standard
#include <stdlib.h> // Fonctions de manipulation de la memoire et de chaines de caracteres
#include <string.h> // Fonctions de manipulation de chaines de caracteres
#include <inttypes.h> // Types de donnees entiers avec largeur specifiee
#include <stdbool.h> // Type de donnees booleen
#include <ctype.h> // manipulation des caracteres

/// Definition des constantes de ce programme
///*******************************************

#define MAX_SIZE 10000 // Taille maximale du tableau
#define __STDC_FORMAT_MACROC // Macro pour printf et scanf pour les types de donnees entiers avec largeur sp�cifi�e
#define MAX_VARIABLES 100 // Nombre maximum de variables dans le programme
#define MAX_TYPE_LENGTH 20 // Longueur maximale d'un type de donnees
#define MAX_NAME_LENGTH 20 // Longueur maximale d'un nom de variable
#define MAX_CONTENT_LENGTH 1000 //Longueur maximale d'un contenu
#define MAX_NUM_CONTENTS 100 //Nombre maximum de contenus
#define MAX_LINE_LENGTH 1000 // Définition de la longueur maximale d'une ligne dans un fichier
#define MAX_NESTING_DEPTH 100 // Définition de la profondeur maximale d'imbrication de blocs.Cela correspond au nombre maximal de blocs imbriqués que nous pouvons suivreavec notre pile lors de la recherche des blocs dans la fonction.


/// Tableau des types de donnees valides
///--------------------------------------

const char* valid_types[] = {"int", "float", "double", "char","void" ,"bool"};

/// definition des structures
///***************************

typedef struct Maillon* Pointeur_Maillon;
typedef struct Symbol_table * Pointeur_ST;

/// 1-Definition de structure de maillon de chaque liste lineaire chainee
///-----------------------------------------------------------------------

struct Maillon {
    char* nom;// Nom de la variable
    char* type;// type de la variable
    char* attribut;// Attribut de la variable
    Pointeur_Maillon suivant;// Pointeur vers le maillon suivant dans la liste lineaire chainee
};

/// 2-Definition de structure de la table des symboles
///----------------------------------------------------
struct Symbol_table
{
Pointeur_Maillon Maillon_G; // pointeur vers le d�but de la liste lineaire chainee de la portee globale
Pointeur_Maillon Maillon_F; // pointeur vers le d�but de la liste lineaire chainee des fonctions
Pointeur_Maillon Maillon_B; // pointeur vers le d�but de la liste lineaire chainee des blocs
};

/// LES MACHINES ABSTRAITES
///*************************

/// les machines abstraites_structure 1
///*************************************

//1---------------------------------------------------------------------------------------------------------
void Allouer_Maillon(Pointeur_Maillon* P)
{
*P = (Pointeur_Maillon) malloc(sizeof(struct Maillon));
(*P)->suivant=NULL;
}
//2---------------------------------------------------------------------------------------------------------
void Liberer_Maillon(Pointeur_Maillon P)
{
free(P);
}
//3---------------------------------------------------------------------------------------------------------
void aff_nom(Pointeur_Maillon P,char* nom)
{
P->nom = nom;
}
//4---------------------------------------------------------------------------------------------------------
void aff_type(Pointeur_Maillon P,char* type)
{
P->type = type;
}
//5---------------------------------------------------------------------------------------------------------
void aff_attribut(Pointeur_Maillon P,char* attribut)
{
P->attribut=attribut;
}
//6---------------------------------------------------------------------------------------------------------
Pointeur_Maillon suivant_Maillon(Pointeur_Maillon P)
{
return (P->suivant);
}
//7---------------------------------------------------------------------------------------------------------
void LinkLists(Pointeur_Maillon P , Pointeur_Maillon Q)
{
    Pointeur_Maillon current;
    current = P;
    while (current->suivant != NULL)
    {
        current = current->suivant;
    }
    current->suivant = Q;
}

///--------------------------------- FIN MACHINES ABSTRAITES DE STRUCTURE 1--------------------------------------


/// les machines abstraites_structure2
///************************************

//1---------------------------------------------------------------------------------------------------------
void Allouer_ST(Pointeur_ST* P)
{
*P=(Pointeur_ST) malloc(sizeof(struct Symbol_table));
(*P)->Maillon_G=NULL;
(*P)->Maillon_F=NULL;
(*P)->Maillon_B=NULL;
}
//2---------------------------------------------------------------------------------------------------------
void Liberer_ST(Pointeur_ST P)
{
free(P);
}
//3---------------------------------------------------------------------------------------------------------
void aff_LG(Pointeur_ST P ,Pointeur_Maillon Q)
{
P->Maillon_G=Q;
}
//4---------------------------------------------------------------------------------------------------------
void aff_LF(Pointeur_ST P ,Pointeur_Maillon Q)
{
P->Maillon_F=Q;
}
//5---------------------------------------------------------------------------------------------------------
void aff_LB(Pointeur_ST P ,Pointeur_Maillon Q)
{
P->Maillon_B=Q;
}
///--------------------------------- FIN MACHINES ABSTRAITES DE STRUCTURE 2--------------------------------------


///LES FONCTIONS ET LES PROCEDURES NECESSAIRES
///********************************************

///1---------------------------------------------------------------------------------
void affichage_entete ()
{

        printf("\n   |==========================================================================================================================================================================================================|\n");
        printf("   |                                                                                    Ecole National Superieur d'Informatique                                                                               |\n");
        printf("   |                                                                          TP 01 Algorithmique et Structures des Donnees Dynamiques                                                           2022/2023    |\n");
        printf("   |                                                                                                                                                                                                          |\n");
        printf("   |     Realise par : SAIDI SELMA           Section:D        Groupe:11                                                                                                                                       |\n");
        printf("   |==========================================================================================================================================================================================================|\n");
        printf("   |                                                                                                                                                                                                          |\n");
        printf("   |                                                                       Generation des tables des symboles a partir d'un code source C                                                                     |\n");
        printf("   |                                                                                                                                                                                                          |\n");
        printf("   |==========================================================================================================================================================================================================|\n\n");

}
///2----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
char* extract_content_globale(char* filename, char* content)
{
    FILE *fp;
    int brace_level = 0, paren_level = 0;
    content[0] = '\0';

    fp = fopen(filename, "r");
    if (fp == NULL)
    {
        printf("Impossible d'ouvrir le fichier.\n");
        return NULL;
    }

    int prev_ch = 0, ch;
    while ((ch = fgetc(fp)) != EOF)
    {
        if (ch == '{')
        {
            brace_level++;
        } else if (ch == '}')
        {
            brace_level--;
        } else if (ch == '(')
        {
            paren_level++;
            strncat(content, "( ", 2);
            while ((ch = fgetc(fp)) != EOF && ch != ')')
            {
                // Ignorer le contenu de la parenthèse
            }
            strncat(content, ") ", 2);
            paren_level--;
            prev_ch = ch; // mettre à jour le caractère précédent
        } else if (ch == ')')
        {
            paren_level--;
            strncat(content, ") ", 2);
            prev_ch = ch; // mettre à jour le caractère précédent
        } else if (brace_level == 0 && paren_level == 0 && ch != '\n')
        {
            if (prev_ch == ')' && ch == 'i')
            {
                strncat(content, " ", 1);
            }
            strncat(content, (char*)&ch, 1);
            prev_ch = ch; // mettre à jour le caractère précédent
        }
    }

    fclose(fp);

    // Nettoyer le contenu
    char new_content[strlen(content)];
    new_content[0] = '\0';
    int j = 0;
    int space_flag = 0;
    for (int i = 0; i < strlen(content); i++)
    {
        if (isspace(content[i]))
        {
            if (!space_flag && i > 0 && content[i-1] != '(' && content[i+1] != ')')
            {
                new_content[j] = ' ';
                j++;
            }
            space_flag = 1;
        }
       else
        {
            new_content[j] = content[i];
            j++;
            space_flag = 0;
        }
    }
    new_content[j] = '\0';

    strcpy(content, new_content);

    return content;
}
///3-------------------------------------------------------------------------------------------------------------
char* extract_content_bloc(char* filename)
{
    FILE *fp;
    char* content = (char*) malloc(sizeof(char) * MAX_SIZE);
    content[0] = '\0';

    fp = fopen(filename, "r");
    if (fp == NULL)
    {
        printf("Impossible d'ouvrir le fichier.\n");
        return NULL;
    }
char line[1024];
    int in_function = 0, in_bracket = 0, depth = 0;
while (fgets(line, sizeof(line), fp))
{
    int len = strlen(line);
    for (int i = 0; i < len; i++)
    {
    if (in_function && line[i] == '{')
    {
            in_bracket++;
            depth++;
            if (in_bracket == 1)
            {
                depth = 1;
            }
        }
        else if (in_function && line[i] == '}')
        {
            in_bracket--;
            if (in_bracket == 0)
            {
                in_function = 0;
            }
        }
        else if (!in_function)
        {
            if (line[i] == '(')
            {
                while (line[i] != ')')
                {
                    i++;
                }
            } else if (isalpha(line[i]))
            {
                int j = i + 1;
                while (j < len && (isalnum(line[j]) || line[j] == '_'))
                {
                    j++;
                }
                if (j < len && line[j] == '(')
                {
                    in_function = 1;
                }
            }
        }
         else if (in_function && in_bracket > 0 && depth > 1)
        {
            strncat(content, &line[i], 1);
        }
    }
}
   int i, j;
    for (i = 0,j = 0; i < strlen(content); i++)
    {
        if (!isspace(content[i]) || (i > 0 && !isspace(content[i-1])))
        {
            content[j++] = content[i];
        }
    }
    content[j] = '\0'; // ajouter le caractère de fin de chaîne


    fclose(fp);

    return content;
}
///4--------------------------------------------------------------------------------------------------------
int Recherche(Pointeur_ST table, char* nom_variable, char* type_variable, char* attribut_variable)
{
    Pointeur_Maillon maillon_actuel;

    // Recherche dans la liste de portée globale
    maillon_actuel = table->Maillon_G;
    while (maillon_actuel != NULL)
    {
        if (strcmp(maillon_actuel->nom, nom_variable) == 0
            && strcmp(maillon_actuel->type, type_variable) == 0
            && strcmp(maillon_actuel->attribut, attribut_variable) == 0)
        {
            return 1; // la variable existe
        }
        maillon_actuel = maillon_actuel->suivant;
    }

    // Recherche dans la liste de fonctions
    maillon_actuel = table->Maillon_F;
    while (maillon_actuel != NULL)
    {
        if (strcmp(maillon_actuel->nom, nom_variable) == 0
            && strcmp(maillon_actuel->type, type_variable) == 0
            && strcmp(maillon_actuel->attribut, attribut_variable) == 0)
        {
            return 1; // la variable existe
        }
        maillon_actuel = maillon_actuel->suivant;
    }

    // Recherche dans la liste de blocs
    maillon_actuel = table->Maillon_B;
    while (maillon_actuel != NULL)
    {
        if (strcmp(maillon_actuel->nom, nom_variable) == 0
            && strcmp(maillon_actuel->type, type_variable) == 0
            && strcmp(maillon_actuel->attribut, attribut_variable) == 0)
        {
            return 1; // la variable existe
        }
        maillon_actuel = maillon_actuel->suivant;
    }

    // La variable n'a pas été trouvée dans la table des symboles
    return 0;
}
///5--------------------------------------------------------------------------------------------------------
char* extract_function_parametre(char* function_name, char* file_name)
{
    FILE* fp = fopen(file_name, "r");
    if (fp == NULL)
    {
        printf("Impossible d'ouvrir le fichier %s\n", file_name);
        return NULL;
    }

    char line[MAX_SIZE];
    char content[MAX_SIZE];
    bool found_function = false;
    bool inside_function = false;
    int brace_level = 0;
    int content_index = 0;

    while (fgets(line,MAX_SIZE, fp) != NULL)
    {
        if (!found_function && strstr(line, function_name) != NULL)
        {
            found_function = true;
            inside_function = true;
        }

        if (found_function && !inside_function)
        {
            break;
        }

        if (inside_function)
        {
            for (int i = 0; i < strlen(line); i++)
            {
                if (line[i] == '(')
                {
                    brace_level++;
                }
                else if (line[i] == ')' && brace_level == 1)
                {
                    inside_function = false;
                    content[content_index] = '\0';
                    break;
                }
                else if (brace_level == 1)
                {
                    content[content_index] = line[i];
                    content_index++;
                }

                if (line[i] == '{' && brace_level == 1)
                {
                    content_index = 0;
                }
                else if (line[i] == '{')
                {
                    brace_level++;
                }
                else if (line[i] == '}')
                {
                    brace_level--;
                }
            }
        }
    }

    fclose(fp);

    if (found_function)
    {
        char new_content[1000];
        int j = 0;
        int space_flag = 0;

        for (int i = 0; i < strlen(content); i++)
            {
            if (isspace(content[i]))
            {
                if (!space_flag && i > 0 && content[i-1] != '(' && content[i+1] != ')')
                {
                    new_content[j] = ' ';
                    j++;
                }
                space_flag = 1;
            }
            else
            {
                new_content[j] = content[i];
                j++;
                space_flag = 0;
            }
        }

        new_content[j] = '\0';

        char* resultat = (char*) malloc(strlen(new_content) + 1);
        if (resultat != NULL)
        {
            strcpy(resultat, new_content);
        }
        return resultat;
    }
    else
    {
        printf("La fonction %s n'a pas été trouvée dans le fichier %s\n", function_name, file_name);
        return NULL;
    }
}
///6--------------------------------------------------------------------------------------------------------
void extract_function_first_brace(char* filename,char **new_contents, int *content_counter)
{
    FILE *fp = fopen(filename, "r");
    if (fp == NULL)
    {
        printf("Impossible d'ouvrir le fichier %s\n",filename);
        return;
    }

    // Lire le contenu entre toutes les paires d'accolades de niveau 1 suivant immédiatement une parenthèse fermante
    int brace_level = 0;
    char content[MAX_CONTENT_LENGTH];
    int i = 0;
    char ch;
    bool found_paren = false;
    while ((ch = fgetc(fp)) != EOF)
    {
        if (ch == '(')
        {
            found_paren = true;
        }
        else if (ch == '{' && brace_level == 0 && found_paren)
        {
            brace_level++;
            i = 0;
        }
        else if (ch == '{' && brace_level > 0)
        {
            brace_level++;
        }
        else if (ch == '}' && brace_level > 0)
        {
            brace_level--;
            if (brace_level == 0)
            {
                content[i] = '\0';
                // nettoyer la chaîne de caractères
                char new_content[MAX_CONTENT_LENGTH];
                int j = 0;  // index pour parcourir la nouvelle chaîne
                int space_flag = 0;  // indicateur pour le caractère précédent

                for (int i = 0; i < MAX_CONTENT_LENGTH; i++)
                {
                    if (isspace(content[i]))
                    {
                        if (!space_flag && i > 0 && content[i-1] != '(' && content[i+1] != ')')
                        {
                            new_content[j] = ' ';  // ajouter un seul espace si le caractère précédent n'était pas un espace et si ce n'est pas un espace entouré de parenthèses
                            j++;
                        }
                        space_flag = 1;  // mettre à jour l'indicateur
                    }
                    else
                    {
                        new_content[j] = content[i];  // copier le caractère à la nouvelle chaîne
                        j++;
                        space_flag = 0;  // mettre à jour l'indicateur
                    }
                }

                new_content[j] = '\0';  // ajouter un caractère nul à la fin de la nouvelle chaîne

                // ajouter le nouveau contenu au tableau de contenus
                new_contents[*content_counter] = malloc((strlen(new_content) + 1) * sizeof(char));
                strcpy(new_contents[*content_counter], new_content);

                (*content_counter)++;
            }
        }
        else if (brace_level == 1 && found_paren)
        {
            content[i++] = ch;
        }
    }

    fclose(fp);
}
///7------------------------------------------------------------------------------------------------------------------
void insertion_Symbol_table(Pointeur_ST table_symboles, char* nom, char* type, char* attribut, char portee)
{
    // Créer un nouveau maillon pour la variable
    Pointeur_Maillon nouveau_maillon ;
    Allouer_Maillon(&nouveau_maillon);
    aff_nom(nouveau_maillon,nom);
    aff_type(nouveau_maillon,type);
    aff_attribut(nouveau_maillon,attribut);

    // Ajouter le nouveau maillon à la liste correspondante selon la portée
    if (portee == 'G')
    {  // Portée globale
        if (table_symboles->Maillon_G == NULL)
        {  // Si la liste est vide
            table_symboles->Maillon_G = nouveau_maillon;
        } else
        {  // Si la liste n'est pas vide
            Pointeur_Maillon dernier_maillon = table_symboles->Maillon_G;
            while (dernier_maillon->suivant != NULL)
            {
                dernier_maillon = dernier_maillon->suivant;
            }
            dernier_maillon->suivant = nouveau_maillon ;

        }
    }
    else if (portee == 'F')
    {  // Portée des fonctions
        if (table_symboles->Maillon_F == NULL)
        {  // Si la liste est vide
            table_symboles->Maillon_F = nouveau_maillon;
        }
        else
        {  // Si la liste n'est pas vide
            Pointeur_Maillon dernier_maillon = table_symboles->Maillon_F;
            while (dernier_maillon->suivant != NULL)
            {
                dernier_maillon = dernier_maillon->suivant;
            }
            dernier_maillon->suivant = nouveau_maillon;
        }
     }
    else if (portee == 'B')
    {  // Portée des blocs
        if (table_symboles->Maillon_B == NULL)
        {  // Si la liste est vide
            table_symboles->Maillon_B = nouveau_maillon;
        }
        else
        {  // Si la liste n'est pas vide
            Pointeur_Maillon dernier_maillon = table_symboles->Maillon_B;
            while (dernier_maillon->suivant != NULL)
            {
                dernier_maillon = dernier_maillon->suivant;
            }
            dernier_maillon->suivant = nouveau_maillon;
        }
    }
}
///8-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int count_functions(const char* filename, char*** function_names)
{
    FILE* file = fopen(filename, "r");
    if (!file)
    {
        printf("Unable to open file: %s\n", filename);
        return 0;
    }

    int num_functions = 0;
    char* line = malloc(MAX_LINE_LENGTH * sizeof(char));
    while (fgets(line, MAX_LINE_LENGTH, file))
    {
        if (strstr(line, "void ") || strstr(line, "int ") || strstr(line, "float ") || strstr(line, "bool "))
        {
            char* name_start = strchr(line, ' ');
            char* name_end = strchr(line, '(');
            if (name_start && name_end)
            {
                int name_length = name_end - name_start - 1;
                char* name = malloc(name_length + 1);
                strncpy(name, name_start + 1, name_length);
                name[name_length] = '\0';
                *function_names = realloc(*function_names, (num_functions + 1) * sizeof(char*));
                (*function_names)[num_functions] = name;
                num_functions++;
            }
        }
    }

    fclose(file);
    free(line);

    return num_functions;
}
///9-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int count_blocks(FILE* fp, const char* func_name)
{
    char line[MAX_LINE_LENGTH];
    char* pos;
    int nesting_depth = 0;
    int block_count = 0;
    int stack[MAX_NESTING_DEPTH];
    int stack_top = -1;

    while (fgets(line, MAX_LINE_LENGTH, fp) != NULL)
    {
        // Recherche de la fonction spécifiée
        pos = strstr(line, func_name);
        if (pos != NULL && strncmp(pos + strlen(func_name), "(", 1) == 0)
        {
            // La fonction est trouvée, on commence à compter les blocs
            nesting_depth = 0;
            stack_top = -1;

            while (fgets(line, MAX_LINE_LENGTH, fp) != NULL)
            {
                for (int i = 0; i < strlen(line); i++)
                {
                    if (line[i] == '{')
                    {
                        nesting_depth++;
                        if (nesting_depth == 2)
                        {
                            stack_top++;
                            stack[stack_top] = 1;
                        } else if (nesting_depth > 2)
                        {
                            stack[stack_top]++;
                        }
                    }
                    else if (line[i] == '}')
                    {
                        nesting_depth--;
                        if (nesting_depth == 1)
                        {
                            block_count += stack[stack_top];
                            stack_top--;
                        }
                    }
                }
            }
            break;
        }
    }

    return block_count;
}
///9------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void afficher_table_symboles(Pointeur_ST table_symboles, char portee) {

    int i = 0;
    Pointeur_Maillon maillon_courant;


    if (portee == 'G')
    {
    maillon_courant = table_symboles->Maillon_G;
    printf("\n");
    printf("\t\t\t\t\t\t\t\t\t                              TABLE DES SYMBOLES                              \n");
    printf("\t\t\t\t\t\t\t\t\t                              ------------------                              \n");
    printf("\n");
    printf("\n");
    printf("\t\t\t\t\t\t\t\t+------------------------------------------------------------------------------+\n");
    printf("\t\t\t\t\t\t\t\t|                      Table des symboles : portee globale                     |\n");
    printf("\t\t\t\t\t\t\t\t|------------------------------------------------------------------------------|\n");
    printf("\t\t\t\t\t\t\t\t|      Type      |      Nom      |      Attribut      |         Adresse        |\n");
        while (maillon_courant != NULL)
        {
        printf("\t\t\t\t\t\t\t\t|----------------+---------------+--------------------+------------------------|\n");
        printf("\t\t\t\t\t\t\t\t|      %-7s   |       %-7s |      %-7s      |   %p     |\n", maillon_courant->nom, maillon_courant->type, maillon_courant->attribut, (void*)maillon_courant);
        maillon_courant = maillon_courant->suivant;
        }
        printf("\t\t\t\t\t\t\t\t--------------------------------------------------------------------------------\n");
    }
    if (portee == 'F')
    {
    maillon_courant = table_symboles->Maillon_F;
    printf("\t\t\t\t\t\t\t\t+------------------------------------------------------------------------------+\n");
    printf("\t\t\t\t\t\t\t\t|                      Table des symboles : fonctions                          |\n");
    printf("\t\t\t\t\t\t\t\t|------------------------------------------------------------------------------|\n");
    printf("\t\t\t\t\t\t\t\t|      Type      |      Nom      |      Attribut      |         Adresse        |\n");
    while (maillon_courant != NULL) {
    printf("\t\t\t\t\t\t\t\t|----------------+---------------+--------------------+------------------------|\n");
    printf("\t\t\t\t\t\t\t\t|      %-7s   |       %-7s |      %-7s      |   %p     |\n", maillon_courant->nom, maillon_courant->type, maillon_courant->attribut, (void*)maillon_courant);
    maillon_courant = maillon_courant->suivant;
    i++;
    }
    printf("\t\t\t\t\t\t\t\t--------------------------------------------------------------------------------                               \n");
    }
    if (portee == 'B')
    {
    maillon_courant = table_symboles->Maillon_B;
    int j = 0;
    int maillons_affiches = 0;
    const char* tab[] = {" ","\t\t\t","\t\t\t\t\t\t\t"};
    while (maillon_courant != NULL )
    {
    if (maillons_affiches % 2 == 0)
    {
    j++;
    printf("\t\t%s+------------------------------------------------------------------------------+\n",tab[j]);
    printf("\t\t%s|                  Table des symboles : bloc %d de la fonction %s                |\n",tab[j],j,"f");
    printf("\t\t%s|------------------------------------------------------------------------------|\n",tab[j]);
    printf("\t\t%s|      Type      |      Nom      |      Attribut      |         Adresse        |\n",tab[j]);
    }
    printf("\t\t%s|----------------+---------------+--------------------+------------------------|\n",tab[j]);
    printf("\t\t%s|      %-7s   |       %-7s |      %-7s      |   %p     |\n",tab[j], maillon_courant->nom, maillon_courant->type, maillon_courant->attribut, (void*)maillon_courant);
    maillon_courant = maillon_courant->suivant;
    maillons_affiches++;

    if (maillons_affiches % 2 == 0 || maillon_courant == NULL)
    {
        printf("\t\t%s+----------------+---------------+--------------------+------------------------+\n",tab[j]);
        printf("\n");
    }
    }
    }
}

int main() {
    affichage_entete();
char filename[256];
    printf("Veuillez entrer le nom de votre fichier code:\n");
    scanf("%s", filename);
    Pointeur_ST table_symboles;
    Allouer_ST(&table_symboles);
    char content[MAX_SIZE]; // TAILLE_MAX est la taille maximale que peut avoir votre contenu
    char *new_content;
    new_content = extract_content_globale(filename, content);
    char* mot;
   char delim[] = " \n,;( (";
   char* mots[100];
   mot = strtok(new_content, delim);
   int i = 0;
   while (mot != NULL)
   {
    if (strlen(mot) > 0)
    {
        mots[i++] = mot;
    }
    mot = strtok(NULL, delim);
   }

// parcours des mots et création de la liste
Pointeur_Maillon tete= NULL;
int j=0;
while (j < i)
{
    char* attribut;
    char* type;
    char* nom;
    if (j+2 < i && mots[j+2] != NULL && !isalpha(mots[j+2]) && (strstr(mots[j+2],")") != NULL))
    {
        attribut = "fonction";
        type = mots[j];
        nom = mots[j+1];
        j++;
    } else
    {
        attribut = "variable";
        type = mots[j];
        nom = mots[j+1];
    }

    if (strlen(type) == 1 && type[0] == ')')
    {
        j += 2;
        continue;
    }

    if (mots[j+2] != NULL && strlen(mots[j+2]) == 1 && (strstr(mots[j+2],")") == NULL) )
    {
        attribut = "variable";
        char* nom2 = mots[j+2];
        j++;
        insertion_Symbol_table(table_symboles,type,nom2,"variable",'G');

    }
    insertion_Symbol_table(table_symboles,type,nom,attribut,'G');
    j = j+2;
}
    afficher_table_symboles(table_symboles,'G');
    char** function_names = NULL;
    int num_functions = count_functions(filename,&function_names);
    for(int i = 0;i < num_functions ; i++)
    {
        char* resultat = extract_function_parametre(function_names[i],filename);
        if (resultat != NULL)
        {
             char delim[] = " \n,;)";
             char* token = strtok(resultat, delim);
             char* tokens[3] = { NULL };
             int h = 0;
             while (token != NULL && h < 3)
             {
             tokens[h++] = token;
             token = strtok(NULL, delim);
             }
            if ((strcmp(tokens[0],"int") == 0) || (strcmp(tokens[0],"float") == 0) || (strcmp(tokens[0],"bool") == 0))
            {
           char* type = tokens[0];
           char* nom1 = tokens[1];
           if (tokens[2] != NULL && strlen(tokens[2]) == 1)
           {
           char* nom2 = tokens[2];
           insertion_Symbol_table(table_symboles,type,nom2,"variable",'F');
           }
           insertion_Symbol_table(table_symboles,type,nom1,"variable",'F');
           }
        }

    }

    char ** n_content = malloc (10 * sizeof(char *));
    int content_counter = 0;
    extract_function_first_brace(filename,n_content,&content_counter);
    for (i= 0;i<content_counter;i++){{
    char* result = n_content[i];
        if (result != NULL) {
             char delim[] = " \n,;)";
             char* token = strtok(result, delim);
             char* tokens[3] = { NULL };
             int h = 0;
             while (token != NULL && h < 3) {
             tokens[h++] = token;
             token = strtok(NULL, delim);
              }
            if ((strcmp(tokens[0],"int") == 0) || (strcmp(tokens[0],"float") == 0) || (strcmp(tokens[0],"bool") == 0)){
           char* type = tokens[0];
           char* nom1 = tokens[1];
           if (tokens[2] != NULL && strlen(tokens[2]) == 1){
           char* nom2 = tokens[2];
           insertion_Symbol_table(table_symboles,type,nom2,"variable",'F');


 }
           insertion_Symbol_table(table_symboles,type,nom1,"variable",'F');


        }}}}
afficher_table_symboles(table_symboles,'F');
char *contenu = extract_content_bloc(filename);
char *delimiter = ",; ";
char *token = strtok(contenu, delimiter);
char current_type[50] = "";
while (token != NULL) {
 //   printf("%s\n", token);
    if (strcmp(token, "int") == 0 || strcmp(token, "float") == 0 || strcmp(token, "char") == 0) {
        strcpy(current_type, token);
        token = strtok(NULL, delimiter); // Avancer au prochain token
    } else if (strcmp(current_type, "") != 0 && strcmp(token, current_type) != 0) {
        insertion_Symbol_table(table_symboles, current_type, token, "variable", 'B');
        token = strtok(NULL, delimiter); // Avancer au prochain token
    } else {
        token = strtok(NULL, delimiter); // Avancer au prochain token
    }
}
        afficher_table_symboles(table_symboles,'B');

        // Libération de la mémoire allouée pour la table des symboles
        free(table_symboles);
    return 0;
}