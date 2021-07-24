#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
// On definit un nouveau type dat
struct date
{
	int j;
	int m;
	int a;
};
typedef struct date date;
//Definition des structures
struct article
{
    long c_article;
    char *designation;
    int classe;
    long quantite_en_stock;
    float prix;
};
typedef struct article ARTICLE;

struct client
{
    long c_client;
    char *nom;
    float taux;
};
typedef struct client CLIENT;

struct achat
{
    long c_client;
    long c_article;
    int nb;
    date date_achat;

};
typedef struct achat ACHAT;


//D�finition des listes cha�n�es
struct l_article
{
    ARTICLE d;
    struct l_article *next;
};
typedef struct l_article L_ARTICLE;

struct l_client
{
    CLIENT d;
    struct l_client *next;
};
typedef struct l_client L_CLIENT;

struct l_achat
{
    ACHAT d;
    struct l_achat *next;
};
typedef struct l_achat L_ACHAT;

//les fonctions  d'insertion
void charge_fichier_article(ARTICLE *nouveau,char *s)
{
    long c_article,quantite_en_stock;
    char designation[30];
    int classe;
    float prix;
    sscanf(s,"%d:%[^:]:%d:%d:%f",&c_article,designation,&classe,&quantite_en_stock,&prix);
    nouveau->designation=(char*)malloc(30*sizeof(char));
    memcpy(nouveau->designation,designation,30*sizeof(char));
    nouveau->c_article=c_article;
    nouveau->quantite_en_stock=quantite_en_stock;
    nouveau->classe=classe;
    nouveau->prix=prix;

}
void charge_liste_article(FILE *fp_art,L_ARTICLE**first)
{
    char ligne [50];
    ARTICLE *nouveau;
    nouveau= (ARTICLE *) malloc(sizeof(ARTICLE));
    L_ARTICLE *p;
    fgets(ligne,50,fp_art);
    charge_fichier_article(nouveau,ligne);
    (*first)->d=*nouveau;
    (*first)->next=NULL;
    while(fgets(ligne,50,fp_art))
    {

           p = (L_ARTICLE *) malloc(sizeof(L_ARTICLE));
           charge_fichier_article(nouveau,ligne);
           p->d=*nouveau;
           p->next= *first;
           *first = p;
    }
}



void charge_fichier_client(CLIENT *nouveau,char *s)
{
    long c_client;
    char nom[30];
    float taux;
    sscanf(s,"%d:%[^:]:%f",&c_client,nom,&taux);
    nouveau->nom=(char*)malloc(30*sizeof(char));
    memcpy(nouveau->nom,nom,30*sizeof(char));
    nouveau->c_client=c_client;
    nouveau->taux=taux;

}
void charge_liste_client(FILE *fp_cli,L_CLIENT **first)
{
    char ligne [41];
    CLIENT *nouveau;
    nouveau= (CLIENT *) malloc(sizeof(CLIENT));
    L_CLIENT *p;
    fgets(ligne,41,fp_cli);
    charge_fichier_client(nouveau,ligne);
    (*first)->d=*nouveau;
    (*first)->next=NULL;
    while(fgets(ligne,41,fp_cli))
    {

           p = (L_CLIENT *) malloc(sizeof(L_CLIENT));
           charge_fichier_client(nouveau,ligne);
           p->d=*nouveau;
           p->next= *first;
           *first = p;
    }
}

void charge_fichier_achat(ACHAT *nouveau,char *s)
{
    long c_client,c_article;
    char test[5];
    char s_date[10];
    char *a;
    int nb;
    sscanf(s,"%l4d:%l4d:%d:%s",&c_client,&c_article,&nb,s_date);
    a=(char*)malloc(10*sizeof(char));
    memcpy(a,s_date,10*sizeof(char));
    sscanf(a,"%2d",&(nouveau->date_achat).j);
    sscanf(a+3,"%2d",&(nouveau->date_achat).m);
    sscanf(a+6,"%4d",&(nouveau->date_achat).a);
    nouveau->c_client=c_client;
    nouveau->c_article=c_article;
    nouveau->nb=nb;


}
void charge_liste_achat(FILE *fp_ach,L_ACHAT**first)
{
    char ligne [41];
    ACHAT *nouveau;
    L_ACHAT *p;
    nouveau= (ACHAT *) malloc(sizeof(ACHAT));
    fgets(ligne,41,fp_ach);
    charge_fichier_achat(nouveau,ligne);
    (*first)->d=*nouveau;
    (*first)->next=NULL;
    while(fgets(ligne,41,fp_ach))
    {
           nouveau= (ACHAT *) malloc(sizeof(ACHAT));
           p = (L_ACHAT *) malloc(sizeof(L_ACHAT));
           charge_fichier_achat(nouveau,ligne);
           p->d=*nouveau;
           p->next= *first;
           *first = p;
    }
    /*p=*first;
    while(p != NULL){
        printf("%ld %ld %d %s",p->d.c_article,p->d.c_client,p->d.nb,p->d.date_achat);
        p=p->next;
    }*/
}
//la quantité achetée d’un article donné sur une période définie
long compare_date(date d1,date d2)
{
  return((d1.a*10000L+d1.m*100+d1.j)-(d2.a*10000L+d2.m*100+d2.j));
}

int quantite_achat(L_ACHAT *first,long code,date deb,date fin)
{
     L_ACHAT *p;
     int quantite_achat=0;
     for(p=first; p!=NULL; p=p->next)
     {
         if( p->d.c_article==code && compare_date(p->d.date_achat,deb)>=0 && compare_date(fin,p->d.date_achat)>=0 )
                  quantite_achat+=p->d.nb;
     }
     return(quantite_achat);
}
//lire date
void lire_date(date *d)
{
 char s[20],c;
 scanf("%s",s);
 sscanf(s,"%2d%c%2d%c%4d",&d->j,&c,&d->m,&c,&d->a);
}
// fonction open files
int open_files(FILE **fp_art,FILE **fp_cli,FILE **fp_ach)
{
 if (!((*fp_art)=fopen("article.txt","r")))
   {
    printf("le fichier article.txt n'existe pas") ;
    return (0) ;
   }
if (!((*fp_cli)=fopen("client.txt","r")))
   {
    printf("le fichier client.txt n'existe pas") ;
    return (0) ;
   }
if (!( (*fp_ach) = fopen("achat.txt","r")))
   {
    printf("le fichier Fournisseur.txt n'existe pas") ;
    return (0) ;
   }
return(1) ;
}
//affichage des listes
void viewarticle (L_ARTICLE *first)
{
    L_ARTICLE *p;
    for (p=first;p!=NULL;p=p->next)
        printf("\n%d:%s:%d:%d:%0.3f\n",p->d.c_article,p->d.designation,p->d.classe,p->d.quantite_en_stock,p->d.prix);
}
//
void viewclient (L_CLIENT *first)
{
    L_CLIENT *p;
    for (p=first;p!=NULL;p=p->next)
        printf("\n%d:%s:%f\n",p->d.c_client,p->d.nom,p->d.taux);
}
void viewachat (L_ACHAT *first)
{
    L_ACHAT *p;
    for (p=first;p!=NULL;p=p->next)
        printf("\n%d:%d:%d:%d/%d/%d\n",p->d.c_client,p->d.c_article,p->d.nb,p->d.date_achat.j,p->d.date_achat.m,p->d.date_achat.a);
}
//afficher, pour chaque classe de produits, le montant des achats réalisés sur une période donnée.

struct l_montant
{
    int classe;
    float montant;
};
typedef struct l_montant LM;
struct l_montant_pclasse
{
    LM montant_pclasse;
    struct l_montant_pclasse *next;
};
typedef struct l_montant_pclasse LMC;


void lire_classe_montant(LMC *first,L_ARTICLE *first_art, L_ACHAT *first_ach, date deb, date fin)
{
    LMC *m;
    LMC *ma = NULL;
    L_ARTICLE *p;
    L_ACHAT *e;
    float montant ;
    for (int i=9;i>0;i--){
        m=(LMC*)malloc(sizeof(LMC));
        m->montant_pclasse.classe=i;
        m->montant_pclasse.montant=0;
        m->next = ma;
        ma=m;
    }


    for(p=first_art; p!=NULL; p=p->next)
    {   montant=0;
        for(e=first_ach; e!=NULL; e=e->next)
        {
            if(p->d.c_article == e->d.c_article)
                montant += p->d.prix * e->d.nb;

        }
        m=ma;
        while(m && montant > 0){
            if (m->montant_pclasse.classe == p->d.classe)
                m->montant_pclasse.montant += montant;

            m = m->next;

        }
    }
    m=ma;
    while(m){
        printf("\n%d %f\n",m->montant_pclasse.classe,m->montant_pclasse.montant);
        m=m->next;
    }
}



//afficher pour chaque client le montant de ses achats, l’affichage doit se faire de manière triée (tri décroissant sur le montant).


struct l_montant_client
{
    long code_c;
    float montant;
};
typedef struct l_montant_client LC;
struct l_montant_pclient
{
    LC montant_pclient;
    struct l_montant_pclient *next;
};
typedef struct l_montant_pclient L_MONTANT_PCLIENT;

void lire_client_montant(L_MONTANT_PCLIENT *first,L_CLIENT *first_cli,L_ARTICLE *first_art, L_ACHAT *first_ach)
{
    L_MONTANT_PCLIENT *ma= NULL;
    L_MONTANT_PCLIENT *m;
    L_MONTANT_PCLIENT *current;
    L_CLIENT *c;
    L_ARTICLE *p;
    L_ACHAT *e;

    for (c=first_cli; c != NULL; c=c->next)
    {

        m=(L_MONTANT_PCLIENT*)malloc(sizeof(L_MONTANT_PCLIENT));
        m->montant_pclient.code_c = c->d.c_client;
        m->montant_pclient.montant=0;
        m->next=NULL;
            for(e=first_ach; e!=NULL; e=e->next)
            {
                if(e->d.c_client == c->d.c_client){
                    for(p=first_art; p->d.c_article != e->d.c_article && p != NULL; p=p->next){}
                m->montant_pclient.montant += p->d.prix * e->d.nb;
                }

            }
            m->montant_pclient.montant -= m->montant_pclient.montant * (c->d.taux/100);
        //Ajout au liste ma en mode trieé
    if (ma == NULL || ma->montant_pclient.montant <= m->montant_pclient.montant)
    {
        m->next = ma;
        ma = m;
    }
    else
    {
        current = ma;
        while (current->next!=NULL &&
               current->next->montant_pclient.montant < m->montant_pclient.montant)
        {
            current = current->next;
        }
        m->next = current->next;
        current->next = m;
    }

    }

    m=ma;
    while(m)
    {
        printf("%d %f\n",m->montant_pclient.code_c,m->montant_pclient.montant);
        m=m->next;
    }


}


//programme pricipale



int main()
{
    L_ARTICLE *l_article;
    L_CLIENT  *l_client;
    L_ACHAT   *l_achat;
    LMC *m;
    L_MONTANT_PCLIENT *mpc;
    int q;
    date deb,fin;
    long code;
    char quit[1]="N";
    int choix;
    char chainecode[100];
    FILE *fp_art, *fp_cli, *fp_ach;
    open_files(&fp_art,&fp_cli,&fp_ach);
    l_article = (L_ARTICLE *) malloc(sizeof(L_ARTICLE));
    l_client = (L_CLIENT *) malloc(sizeof(L_CLIENT));
    l_achat = (L_ACHAT *) malloc(sizeof(L_ACHAT));
    m=(LMC*)malloc(sizeof(LMC));
    mpc=(L_MONTANT_PCLIENT*)malloc(sizeof(L_MONTANT_PCLIENT));
    charge_liste_article(fp_art,&l_article);
    charge_liste_client(fp_cli,&l_client);
    charge_liste_achat(fp_ach,&l_achat);
    while(strcmp(quit,"O") != 0)
    {
        printf("%s\n"," _______________________________________________________________________ " );
        printf("%s\n","|                                                                       |" );
        printf("%s\n","|  1 - afficher la liste des articles                                   |" );
        printf("%s\n","|  2 - afficher la liste des clients                                    |" );
        printf("%s\n","|  3 - afficher la liste des achats                                     |" );
        printf("%s\n","|  4 - determiner la qte achete dun article sur  une periode            |" );
        printf("%s\n","|  5 - afficher pour chaque classe  le montant des achatssur une periode|" );
        printf("%s\n","|  6 - afficher pour chaque client le montant de ses achats triee       |" );
        printf("%s\n","|_______________________________________________________________________|" );
        printf("\n%s", "Entrer Votre Choix : ");
        scanf("%d",&choix);
        if(choix == 1)
        {
           viewarticle(l_article);
        }
        else if(choix == 2)
        {
             viewclient(l_client);

        }
        else if(choix == 3)
        {
             viewachat(l_achat);
        }
        else if(choix == 4)
        {

            printf("\nDonnez la periode:  ");
            printf("Donnez une date debut: ");
            lire_date(&deb);
            printf("\nDonnez une date fin: ");
            lire_date(&fin);
            printf("\nDonnez le code d'article dont vous souhaitez determiner la quantite achetee: ");
            scanf("%s",chainecode);
            sscanf(chainecode,"%4d",&code);
            q=quantite_achat(l_achat,code,deb,fin);
            printf("la quantite est %d\n",q);
        }
        else if(choix == 5)
        {
            printf("\nDonnez la periode:  ");
            printf("Donnez une date debut: ");
            lire_date(&deb);
            printf("\nDonnez une date fin: ");
            lire_date(&fin);
            lire_classe_montant(m,l_article,l_achat,deb,fin);
        }
        else if(choix == 6)
        {
            lire_client_montant(mpc,l_client,l_article,l_achat);
        }
        printf("Voulez-vous quittez (O/N) : ");
        scanf("%s",quit);
    }
    return 0;
}










