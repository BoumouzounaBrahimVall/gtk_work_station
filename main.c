#include <gtk/gtk.h>
#include <string.h>
#include<stdlib.h>
#define maxTitre 30 //taille max d'un titre
#define maxPath 100 // taille max d'un chemin


/*
 * structure de dimention d'un widget
 */
typedef struct
{
    gint height; //longueur
    gint width; //largeur
}dimension;
/*
 * structure des coordonnees
 */
typedef struct
{
    gint x; //abscisse
    gint y;//ordonne
}coordonnees;
/*
 * structure de positionnement
 * soit avoir des coordonnees soit une position
 */
typedef union
{
    coordonnees cord;//les coordonnees
    gchar pos;       //position
}POS;

/*
 * structure du couleur
 */
typedef struct
{
 gchar*color;
 gfloat opacity;
}color_widg;

/*
 * structure d'un fenetre
 */
typedef struct
{
    GtkWidget *window;   //la fenetre
    gchar *titre;       //titre de la fenetre
    gchar *icon;       //chemin de l'icone de la fenetre
    dimension  dim;   //dimention de la fenetre
    POS place;       //l'emplacement de la fenetre
    gint resizable; // redimensionnement de la fenetre (0 la 1 ah )
    color_widg *color; //couleur de fenetre
    gint bordure_taille; // la taille de la bordure
}Fenetre;
/*
 * structure d'un widget fils
 */
typedef struct
{
    GtkWidget *elem;       //l'element lui meme
    gchar *titre;         //titre du widget
    GtkWidget *conteneur;//le pere conteneur du widget
}widg;



/******************************************************************/
 /// test d'allocation
void test_allocation(void *pointeur, char*nom)
{
    if(!pointeur) {
        printf("erreur d'allocation du %s\n", nom);
        exit(-1);
    }
}




/**********************************fonction du couleur***********************************/

void add_bgcolor(GtkWidget*widget,gchar*couleur,gfloat opc)

{

    if(couleur==NULL) return;

    GdkRGBA color;
    gdk_rgba_parse(&color,couleur);
    color.alpha=opc;
    gtk_widget_override_background_color(widget,GTK_STATE_FLAG_NORMAL,&color);
}


coordonnees walo()
{
    coordonnees c;
    c.x=0;
    c.y=0;
    return c;
}
dimension walo2()
{
    dimension d;
    d.width=0;
    d.height=0;
    return d;
}

///-----------------------------------WINDOW-----------------------------------------------------------------------


/*************************************************************
  La fonction : init_window
  -Entrees :  titre de la fenêtre
      hauteur de notre fenêtre
      largeur de notre fenêtre
      position par defaults de la fenêtre :
        A = GTK_WIN_POS_NONE
        B = GTK_WIN_POS_CENTER
        C = GTK_WIN_POS_MOUSE
        D = GTK_WIN_POS_CENTER_ALWAYS
        E = GTK_WIN_POS_CENTER_ON_PARENT
                Ou bien
        position dans l'axe des x
        position dans l'axe des y
      Possibilité de changer la taille:
        0=NON
        1=OUI
      chemin de l'icon de la fenetre
  -Sortie: La structure WindowObjet apres initialisation
  -Description: Cette fonction permet d'allouer de l'espace memoire pour notre variable de type
                   Fenetre et de l'initaliser
*************************************************************/
Fenetre* init_window(gchar* titre,POS P,dimension D, gint res, gchar*path,color_widg *couleur,gint border)
{
    Fenetre* Window = NULL;
    //Allocation memoire
    Window = (Fenetre*)malloc(sizeof(Fenetre));
    // Test d'allocation
    test_allocation(Window, "fenetre");

    if(titre)
    {  //Allocation de la memoire pour le titre
        Window->titre = (gchar *) malloc(maxTitre*sizeof(gchar));

        test_allocation(Window->titre, "titre fenetre");
        strcpy(Window->titre, titre);//donner le titre a notre fenetre
    }
    if(path)
    {//Allocation de la memoire pour le chemin de l'icon
        Window->icon = (gchar *) malloc(maxPath*sizeof(gchar));
        test_allocation(Window->icon, "Icon fenetre");
        strcpy(Window->icon, path);//donner le titre a notre fenetre
    }
    else Window->icon=NULL;
    Window->dim.height = D.height; // initialisation de la hauteur
    Window->dim.width = D.width; // initialisation de la largeur
    Window->resizable = res; //initialisation du resizable
    Window->place=P;
    Window->color=(color_widg*)malloc(sizeof(color_widg));
    test_allocation(Window->color,"allocation_couleur");
    if(couleur){
        Window->color=couleur;
        }
        else{
            (Window->color)=NULL;
        }

    Window->bordure_taille=border;
    return (Fenetre*)Window; //Retourner la structure
}//fin
//positionnement de la fenetre
/*************************************************************
 La fonction : pos_Window
 Entree :Un pointeur vers la structure Fenetre
           initialisée
 Sortie :  Un pointeur vers la variable W (Fenetre)
 Description : Cette fonction permet d'affecter un type de
    positionement de la fenetre
*************************************************************/
Fenetre *pos_Window(Fenetre*W)
{
    switch(W->place.pos)
    {
        case 'A' : gtk_window_set_position(GTK_WINDOW(W->window),GTK_WIN_POS_NONE);
            break ;
            // Au centre de l'ecran
        case 'B' : gtk_window_set_position(GTK_WINDOW(W->window),GTK_WIN_POS_CENTER);
            break;
            // La position du curseur
        case 'C' : gtk_window_set_position(GTK_WINDOW(W->window),GTK_WIN_POS_MOUSE);
            break;
            // Toujours au centre
        case 'D' : gtk_window_set_position(GTK_WINDOW(W->window),GTK_WIN_POS_CENTER_ALWAYS);
            break;
            // Au centre par rapport a l'élement parent
        case 'E' : gtk_window_set_position(GTK_WINDOW(W->window),GTK_WIN_POS_CENTER_ON_PARENT);
            break;
    }
    return (Fenetre*)W;
}//fin
/*************************************************************
 La fonction : creer_Window
 Entree :Un pointeur vers la structure Fenetre
           initialisée
 Sortie :  Un pointeur vers la variable W (Fenetre)
 Description : Cette fonction permet de créer une fenêtre en
Fct des paramètres récupérés de la structure
                 Fenetre
*************************************************************/
Fenetre* creer_Window(Fenetre *W)
{

    GdkPixbuf *pixbuf;//pointeur sur le peffeur
    //Creation de la fenetre
    W->window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    if(W->titre)//affectation du titre
        gtk_window_set_title(GTK_WINDOW(W->window),W->titre);
    if((W->dim.height) && (W->dim.width))//affectation de la dimension
        gtk_widget_set_size_request(W->window,(W->dim.width),(W->dim.height));
    //si le positionement est spesifier
    if((W->place.pos>='A')&&(W->place.pos<='E'))
        W=pos_Window(W);
        //sinon affecter les coordonnees de positionment
    else gtk_window_move(GTK_WINDOW(W->window),W->place.cord.x,W->place.cord.y);
    //Si res = 1, redimensionnement = VRAI
    if(W->resizable)
        gtk_window_set_resizable(GTK_WINDOW(W->window),TRUE);
    else
        gtk_window_set_resizable(GTK_WINDOW(W->window),FALSE);
    //ajouter l'icon de la fenetre apartire du beffeur
    if(W->icon) {
        pixbuf = gdk_pixbuf_new_from_file(W->icon, NULL);
        gtk_window_set_icon(GTK_WINDOW(W->window), pixbuf);
    }

    if((W->color))
   add_bgcolor(W->window,W->color->color,W->color->opacity);

   gtk_container_set_border_width(W->window,W->bordure_taille);

    return (Fenetre*)W;
}//fin



///----------------------------------- BOX -----------------------------------------------------------------------
///****************************************************************************************************************
///************************************************************************************************************
///************************************************************************************************************

/*
 * structure du widget GTK_BOX
 * Expend: n'est utile que si la GtkBox en question n'est pas définie comme homogène (homogeneous=FALSE lors de la création).
 * Dans ce cas, tous les widgets qui auront été insérés avec la valeur expand=TRUE se partageront tout l'espace libre de la GtkBox
 * (les widgets avec expand=FALSE n'utiliseront que l'espace qui leur est nécessaire).
 */
typedef struct
{
    widg box;        // le box
    gchar type;    // type horizontal ou vertical
    gint homogene; // l'homogénéité des fils du box ( true or false)
    gint spacing; // nombre de pixles entre les fils du box
    gint bordure_taille; //taille de bordure dans le box
    color_widg *color; //couleur du box
    dimension D;
    coordonnees C;
}MonBox;

/*************************************************************
  La fonction : Init_box
  -Entrees :  pere du box ou conteneur
      titre du box
      type du box :
        V = GTK_ORIENTATION_VERTICAL
        H = GTK_ORIENTATION_HORIZONTAL
      homogeniete:
        0=NON
        1=OUI
      l'espace entre les elements du box
  -Sortie: La structure MonBox apres initialisation
  -Description: Cette fonction permet d'allouer de l'espace memoire pour notre variable de type
                   MonBox et de l'initaliser
*************************************************************/
MonBox*Init_box(GtkWidget*pere,gchar*titre,gchar type,gint homo,dimension dim,
                gint spacing,gint border,color_widg *couleur,coordonnees cord)
{
    MonBox*B=NULL;//declaration du pointeur de type MonBox
    //allocation de la memoire de B
    B=(MonBox*) malloc(sizeof (MonBox));
    //verifier l'allocation
    test_allocation(B,"box");
    if(titre)//titre no vide
    {//allocation de la memoire du titre du box
        B->box.titre = (gchar *) malloc(100*sizeof(titre));
        //verifier l'allocation
        test_allocation(B->box.titre,"titre Box");
        strcpy(B->box.titre, titre);//affecter le titre
    }
    else B->box.titre=NULL;
    B->type=type;//affecter le type
    if(pere) {B->box.conteneur=pere;}//affecter le conteneur
    else B->box.conteneur=NULL;
    B->spacing=spacing;//affecter l'espacement
    B->homogene=homo;//affecter l'omogeite
    B->bordure_taille=border;
  B->color=(color_widg*)malloc(sizeof(color_widg));
    test_allocation(B->color,"allocation_couleur");
    if(couleur){
        B->color=couleur;
        }
        else{
            (B->color)=NULL;
        }
        B->D=dim;
        B->C=cord;
    return ((MonBox*)B);

}//fin
//creer box
/*************************************************************
 La fonction : creer_Box
 Entree :Un pointeur vers la structure MonBox
           initialisée
 Sortie :  Un pointeur vers la variable B (MonBox)
 Description : Cette fonction permet de créer une Box en
Fonction des paramètres récupérés de la structure MonBox
*************************************************************/
MonBox*creer_Box(MonBox*B)
{
    //si le box est vertical
    if(B->type=='v'|| B->type=='V')
        B->box.elem= gtk_box_new(GTK_ORIENTATION_VERTICAL,B->spacing);
    //sinon si c'est horizontal
    else if(B->type=='h'|| B->type=='H')
        B->box.elem= gtk_box_new(GTK_ORIENTATION_HORIZONTAL,B->spacing);
    //sinon c'est une erreur
    else{printf("error type box");exit(-1);}
    //affecter l'omogeniete
    gtk_box_set_homogeneous(GTK_BOX(B->box.elem),B->homogene);
    //affecter le titre
    gtk_widget_set_name(B->box.elem,B->box.titre);
    if(B->box.conteneur)
    gtk_container_add(GTK_CONTAINER(B->box.conteneur),B->box.elem);
    gtk_container_set_border_width(B->box.elem,B->bordure_taille);
    if(B->color)
    add_bgcolor(B->box.elem,B->color->color,B->color->opacity);
    gtk_widget_set_size_request(B->box.elem,B->D.width,B->D.height);
    return ((MonBox*)B);
}//fin
//ajouter debut
/*************************************************************
 La fonction : ajouter_Box
 Entree :Un pointeur vers la structure MonBox
           initialisée et cree
         un pointeur sur un element (fils) qui sera
           inserer dans le box
         un boolean (fill)   : (1) le fils peut remplir son espace complet, 0 non
         un boolean  (expend): (1) le fils peut s'etendre dans le box, 0 non
         un entiere (padding): qui rerpesente le rembourrage du fils
         un entiere (posF): (1) on ajoute le fils au debut du box, sinon a la fin
 Sortie :  Un pointeur vers la variable B (MonBox)
 Description : Cette fonction permet d'ajouter une widget dans un box

*************************************************************/
MonBox *ajouter_Box(MonBox*B,GtkWidget*fils,gboolean fill,gboolean expend,int padding,int posF)
{
    if(posF==1)//ajouter l'element au debut du box
        gtk_box_pack_start(GTK_BOX(B->box.elem),fils,expend,fill,padding);
    //ajouter l'element a la fin du box
    else gtk_box_pack_end(GTK_BOX(B->box.elem),fils,expend,fill,padding);
    return ((MonBox*)B);
}//fin



///************************************************GRID*************************************************///
///**********************************************************************************************///
///**********************************************************************************************///
///**********************************************************************************************///



typedef struct
{
    widg Grid;
    coordonnees C;
     dimension D;
     color_widg *couleur;
}Mongrid;


/*************************************************************
 La fonction : init_creer_Grid
 -Entrées : titre du Grid
 conteneur du Grid
 -Sortie : La structure widg (grid) après initialisation et création
 -Description : Cette fonction permet d’allouer de l’espace mémoire pour
 notre variable de type widg (grid), l’initialisé et le créé
*************************************************************/
widg *init_creer_Grid(gchar*titre,GtkWidget*conteneur,coordonnees cord,dimension dim,color_widg*couleur)
{
 //pointeur sur un grid, allocation de la mémoire
 Mongrid *grid=(Mongrid *) malloc(sizeof(Mongrid)) ;
 //vérifier l’allocation
 test_allocation(grid, "grid") ;
 if(titre){// affecter un titre a notre grid s’il existe
 grid->Grid.titre=(gchar*) malloc(maxTitre*sizeof ((titre))) ;
 test_allocation(grid, "titre grid") ;
 gtk_widget_set_name(grid->Grid.elem,titre) ;

 }
 else grid->Grid.titre=NULL;
 grid->Grid.elem=gtk_grid_new() ;//créer un grid
 if(conteneur)
 grid->Grid.conteneur=conteneur ;//l’affecter son conteneur

 else grid->Grid.conteneur=NULL;
 grid->D=dim;
 grid->C=cord;
 //ajouter le grid dans son conteneur
 if(grid->Grid.conteneur)
 gtk_container_add(GTK_CONTAINER(grid->Grid.conteneur),grid->Grid.elem) ;
 gtk_widget_set_size_request(grid->Grid.elem,grid->D.width,grid->D.height);
 if(couleur)
    grid->couleur=couleur;
 else grid->couleur=NULL;
 if(grid->couleur)
    add_bgcolor(grid->Grid.elem,grid->couleur->color,grid->couleur->opacity);
 return (Mongrid*)grid ;
}//fin






/*************************************************************
La fonction : ajouter_Grid_pos
Entree :Un pointeur vers la structure widg
 initialisée
 un pointeur sur un élément(fils) a ajouté dans le grid
 coordonnées du fils
 dimension du fils
Sortie : Un pointeur vers la variable grid (widg)
Description : Cette fonction permet l’ajout d’un élément (widget)
dans un grid à l’aide de ses coordonnées
*************************************************************/
widg *ajouter_Grid_pos(widg *grid,GtkWidget*fils,coordonnees filsCrd,
 dimension filsDim)
{
 //attacher le fils au grid
 gtk_grid_attach(GTK_GRID(grid->elem),fils,filsCrd.x,
 filsCrd.y,filsDim.width,filsDim.height) ;
 return (widg*)grid ;
}//fin



/*************************************************************
La fonction : ajouter_Grid_cote_de
Entree :Un pointeur vers la structure widg
 initialisée
 un pointeur sur un élément(fils) a ajouté dans le grid
 un pointeur sur un fils du grid (déjà ajouter dans le grid)
 un positionnement (side) :
 { GTK_POS_LEFT,(à gauche)
 GTK_POS_RIGHT,(a droit)
 GTK_POS_TOP, (en haut)
 GTK_POS_BOTTOM (en bas) }
 dimension du fils
Sortie : Un pointeur vers la variable grid (widg) après l’ajout
Description : Cette fonction permet l’ajout d’un élément (fils)
dans un grid a cote d’un autre élément dans le grid (frère) selon
un positionnement donne (side).
si (frère n’est pas dans le grid (NULL)) l’élément sera ajouter au début du
grid
*************************************************************/
widg *ajouter_Grid_cote_de(widg *grid,GtkWidget*fils,
 GtkWidget *frere,GtkPositionType side,dimension filsDim)
{
 gtk_grid_attach_next_to(GTK_GRID(grid->elem),fils,frere,side,filsDim.width,filsDim.height) ;
 return (widg*)grid ;
}









///-------------------------------------BUTTON-----------------------------------------------------------------------------------
///****************************************************************************************************************
///************************************************************************************************************
///************************************************************************************************************
/*
 * structure d'un boutton
 */
typedef struct
{
    widg button;        // le boutton
    GtkWidget * pere;  // pour les bouttons radios
    gchar *url_im;     // path pour image
    gchar *url_ic ;     // path pour icon
    gint type;       //radio/check/nomale

    dimension dim; //dimension du bouton
    gint mnemo;//la creation du button(mnemonic/label)
    color_widg *color; //couleur de button
    coordonnees C;
}MonBouton;

/**********************************************************
La fonction : init_button
-Entrees : mere : la mere du button
          url: chemin de l'icone du boutton
          C les coordonnes du bouton
          D la dimention du bouton
          tit:le titre du button
          conte:le coteneur du bouton
          type:le type du bouton (normal,radio,
                                  check...)
                mnemo:(cration du bouton
                       soit avec label ou birn
                       avec mnemoniac)
-Sortie : B : Objet bouton
-Description: Cette fonction permet d'allouer de l'espace pour
 la structure de type ButtonObject, d'initialiser
 ses champs et de la retourner .
***********************************************************/

MonBouton *init_Button(GtkWidget *mere, gchar *url1,gchar *url2,
                       dimension D,gchar*tit, GtkWidget*conte,gint type,
                        gint mnemo,color_widg *couleur,coordonnees cord)
{
    MonBouton*B;
    //allocation de la memoire pour le bouton
    B=(MonBouton*)malloc(sizeof(MonBouton));
    test_allocation(B,"bouton");
    if(tit){
        B->button.titre= (gchar*)malloc(maxTitre*sizeof(gchar));
        test_allocation( B->button.titre,"titre");
        strcpy( B->button.titre,tit);
    }
    else B->button.titre=NULL;

    //allocation de la memoire pour l'url 1
        B->url_im=(gchar*)malloc(100*sizeof(gchar));
        test_allocation(B->url_im,"url bouton");
        //initialisation de l url 1
        if(url1)
        strcpy(B->url_im,url1);
        else B->url_im=NULL;

        //allocation de la memoire pour l'url 2
        B->url_ic=(gchar*)malloc(100*sizeof(gchar));
        test_allocation(B->url_ic,"url bouton");
        //initialisation de l url
        if(url2)
        strcpy(B->url_ic,url2);
        else B->url_ic=NULL;

if(conte)
    B->button.conteneur=conte;
    else B->button.conteneur=NULL;

    B->dim=D;//initialisation du dimention
    if(mere)
    B->pere=mere;//initialisation du widget pere
    else B->pere=NULL;
    B->type=type;//initialisation du type de bouton
    B->mnemo=mnemo;
   B->color=(color_widg*)malloc(sizeof(color_widg));
    test_allocation(B->color,"allocation_couleur");
    if(couleur){
        B->color=couleur;
        }
        else{
            (B->color)=NULL;
        }
        B->C=cord;

    // return le bouton
    return ((MonBouton*)B);

}

/**********************************************************
La fonction : creer_normal_button
-Entrees : B : objet bouton initialise
-Sortie : B : Objet bouton cree
-Description:Cette fonction permet la creation d'un boutton
normal en fct des parametres recuperes de la structure
 ButtonObject initialisee.
***********************************************************/
MonBouton *creer_normal_Bouton(MonBouton *B)

{

    if(B->button.titre)
    {
        switch(B->mnemo)
        {   // creation du bouton with label
            case 0: B->button.elem=gtk_button_new_with_label(B->button.titre);
                break;
                // creation du bouton with mnemonic
            case 1: B->button.elem=gtk_button_new_with_mnemonic(B->button.titre);
                break;
        }
        // donner un nom au bouton
        gtk_widget_set_name(B->button.elem,B->button.titre);
    }
// creation du bouton sans titre
    else B->button.elem = gtk_button_new();

//retourner le bouton
    return ((MonBouton*)B);
}

/**********************************************************
La fonction : creer_radio_button
-Entrees : B : objet bouton initialise
-Sortie : B : Objet bouton cree
-Description:Cette fonction permet la creation d'un boutton
radio en fct des parametres recuperes de la structure
 ButtonObject initialisee.
***********************************************************/
MonBouton *creer_radio_Bouton(MonBouton *B)
{
    if(B->button.titre)//boutton avec titre
    {
        switch(B->mnemo)//bouton avec mnemonic
        {
            case 0:// creation du bouton with label
                if(B->pere)//si le bouton a un pere
                {//affecter le bouton au group du bouton pere
                    B->button.elem=gtk_radio_button_new_with_label(
                     gtk_radio_button_get_group(GTK_RADIO_BUTTON(B->pere)),B->button.titre);
                }
                else//sinon
                    B->button.elem=gtk_radio_button_new_with_label(NULL,B->button.titre);
                break;
            case 1:// creation du bouton avec mnemonic
                if(B->pere)//si le bouton a un pere
                    B->button.elem=gtk_radio_button_new_with_mnemonic(
                            gtk_radio_button_get_group(GTK_RADIO_BUTTON(B->pere)),B->button.titre);
                    //sinon
                else B->button.elem=gtk_radio_button_new_with_mnemonic(NULL,B->button.titre);
                break;
        }//fin switch
        // donner un nom au bouton
        gtk_widget_set_name(B->button.elem,B->button.titre);
    }
        //creation du bouton sans titre
    else B->button.elem = gtk_button_new();
    return ((MonBouton*)B);
}

/**********************************************************
La fonction : creer_check_button
-Entrees : B : objet bouton initialise
-Sortie : B : Objet bouton cree
-Description:Cette fonction permet la creation d'un boutton
check en fct des parametres recuperes de la structure
 ButtonObject initialisee.
***********************************************************/
MonBouton *creer_check_Bouton(MonBouton *B)
{
    if(B->button.titre)
    {
        switch(B->mnemo)
        {
            case 0:// creation du bouton with label
                B->button.elem=gtk_check_button_new_with_label(B->button.titre);break;
            case 1:// creation du bouton with mnemonic
                B->button.elem=gtk_check_button_new_with_mnemonic(B->button.titre);break;
        }//fin switch
        //donner un nom au bouton
        gtk_widget_set_name(B->button.elem,B->button.titre);
    }//fin if
        // creation du bouton sans titre
    else B->button.elem = gtk_button_new();
    return ((MonBouton*)B);
}

/**********************************************************
La fonction : creer_toggle_button
-Entrees : B : objet bouton initialise
-Sortie : B : Objet bouton cree
-Description:Cette fonction permet la creation d'un boutton
toggle en fct des parametres recuperes de la structure
 ButtonObject initialisee.
***********************************************************/

MonBouton *creer_toggle_Bouton(MonBouton *B)
{
    if(B->button.titre)
    {
        switch(B->mnemo)
        {
            case 0:// creation du bouton with label
                B->button.elem=gtk_toggle_button_new_with_label(B->button.titre);break;
            case 1:// creation du bouton with mnemonic
                B->button.elem=gtk_toggle_button_new_with_mnemonic(B->button.titre);break;
        }//fin switch
        gtk_widget_set_name(B->button.elem,B->button.titre);
    }
        //creation du bouton sans titre
    else B->button.elem = gtk_button_new();
    return ((MonBouton*)B);
}




/*******  Ajouter image to button *******/
void ajouter_image_button(gchar* url,MonBouton *B)
{
if(!url) return;

  GtkWidget* image_button;
    image_button=gtk_image_new_from_file(url);
gtk_button_set_image(GTK_BUTTON(B->button.elem),image_button);
gtk_button_set_always_show_image(GTK_BUTTON(B->button.elem),TRUE);

}


 /*******  Ajouter icon to button *******/
void ajouter_icon_button(gchar* url,MonBouton *B)
{
    if(!url) return;
    GtkWidget *image_button;
 GdkPixbuf* pixt=gdk_pixbuf_new_from_file_at_size(B->url_ic,50,50,NULL);
        image_button=gtk_image_new_from_pixbuf(pixt);
 gtk_button_set_image(GTK_BUTTON(B->button.elem),image_button);

 gtk_button_set_always_show_image(GTK_BUTTON(B->button.elem),1);


}















/**********************************************************
La fonction : creer_button
-Entrees : B : objet bouton initialise
-Sortie : B : Objet bouton cree
-Description:Cette fonction permet la creation d'un boutton
 en fct des parametres recuperes de la structure
 ButtonObject initialisee.
***********************************************************/


MonBouton *creer_Bouton(MonBouton *B)
{
     GdkPixbuf *pixbuf;
    switch(B->type)
    {
        case 1: B=creer_normal_Bouton(B) ;  break;    /* normal button */
        case 2: B=creer_radio_Bouton(B);break;         /* radio button */
        case 3: B= creer_check_Bouton(B); break;    /* check button*/
        case 4: B= creer_toggle_Bouton(B); break;    /* toggle button*/
    }
    /// ajout de couleur
    if(B->color)
    add_bgcolor(B->button.elem,B->color->color,B->color->opacity);

  if(B->button.conteneur)  gtk_container_add(B->button.conteneur,B->button.elem);

    if(B->url_ic)
        ajouter_icon_button(B->url_ic,B);

     if(B->url_im)
        ajouter_image_button(B->url_im,B);
            gtk_widget_set_size_request(B->button.elem,B->dim.width,B->dim.height);


    return ((MonBouton*)B);
}


void Afficher_widget(GtkWidget *F)
{
  gtk_widget_show_all(GTK_WINDOW(F));

}






///******************************************SPIN BUTTON*************************************///
///*****************************************************************************************///
///*****************************************************************************************///

typedef struct
{
    GtkWidget *SpinButton;//SpinButton
    gdouble min;//la valeur minimale
    gdouble max;//la valeur maximale
    gdouble step;//le pas
    gdouble value;//valeur par defaut
    dimension D;
    gint wrap;//0:FALSE , 1:TRUE
    gint digits;//nombre de chiffres en partie décimale
    gint numeric;//0:FALSE(tapez de caracteres en generale) , 1:TRUE(juste les chiffres)
    gint arrondissement;//0:FALSE , 1:TRUE
    color_widg *bgcolor;//background color
    coordonnees C;
    GtkWidget *conteneur;
}MonSpin;


/*********************Macros ********************************/

/**
    fonction: init_spin_button
    entreés :
               gdouble val_min   :indique la valeur minimale du spin button
               gdouble val_max   :indique la valeur maximale du spin button
               gdouble pas       :indique le pas d'incrémentation ou décrémentation
               gdouble val_defaut:indique la valeur par défaut du spin button
dimension:     gint height       :indique la longueur du spin button
               gint width        :indique la largeur du spinbutton
               gint wrap         :indique la possibilité de revient à la limite opposée lorsque la limite
                                  supérieure ou inférieure de la place est dépassée(1:TRUE,0:FALSE)
               gint digits       :indique le nombre de chiffres en partie décimale
               gint numeric      :indique possibilité d'écrire juste les chiffres (1:TRUE),ou Ecrire tous caractères(0:FALSE)
               gint arrondis     :indique possibilité d'arrondissement(1:TRUE,0:FALSE)
    sorties :
               SpinButton bien initialisé
    Description:
                Cette fonction permet d'initialiser spinbutton et l'affecte des caractéristique
*/
MonSpin * init_spin_button(gdouble val_min    , gdouble val_max , gdouble pas,
                                    gdouble val_defaut ,dimension dim, gint wrap ,
                                    gint digits ,gint numeric  , gint arrondis,color_widg *bgcolor,
                                    coordonnees cord,GtkWidget*conte)
{

    MonSpin *Sb = NULL;
    //allocation mémoire
    Sb =(MonSpin*)malloc(sizeof(MonSpin));

    //test d'allocation
    test_allocation(Sb,"spin");
    //allocation réussi
    //Initialisation
    Sb->min = val_min ;
    Sb->max =val_max ;
    Sb->step = pas;
    Sb->D=dim;

    Sb->wrap = wrap;
    Sb->digits = digits;
    Sb->numeric = numeric;
    Sb->arrondissement =arrondis;
    //Sb->bgcolor->color =NULL;
    //color
    if(bgcolor)
    {
        Sb->bgcolor= (color_widg*)malloc(sizeof (color_widg));
        strcpy(Sb->bgcolor , bgcolor);
    }
    else Sb->bgcolor=NULL;


   Sb->C=cord;
   if(conte) Sb->conteneur=conte;
   else Sb->conteneur=NULL;

    return ((MonSpin*)Sb);
}

/*************************Création********************/
/**
    fonction:   create_spin_button
    entreés :
                SpinButtonObjet *Sb:indique notre spinbutton object à créer
    sorties :
                SpinButton bien crée
    desciprion:
                cette fonction sert a créer spinbutton en utilisant des fonctions de base sur ses caractéristiques
*/
void create_spin_button(MonSpin* Sb )

{
    //L'intervalle du SpinButton avec le pas
    if((Sb->max) >= (Sb->min))
    {

        Sb->SpinButton =gtk_spin_button_new_with_range(Sb->min,Sb->max,Sb->step);
        //Valeur par defaut
        gtk_spin_button_set_value( GTK_SPIN_BUTTON(Sb->SpinButton) ,Sb->value);

        //largeur et hauteur
        if( (Sb->D.height >0)&&(Sb->D.width > 0) ){

            gtk_widget_set_size_request(GTK_WIDGET(Sb->SpinButton),Sb->D.width ,Sb->D.height);
        }
        else
            printf("Height & Width should be positivs\n");

        //digits
        if(Sb->digits >=0 ){
            gtk_spin_button_set_digits(GTK_SPIN_BUTTON(Sb->SpinButton) , Sb->digits);
        }
        else
            printf("\nDigits should be positive\n");

        //Wrap
        if(Sb->wrap == 1){
            gtk_spin_button_set_wrap (GTK_SPIN_BUTTON(Sb->SpinButton) , TRUE);
        }

        //Numeric
        if(!(Sb->numeric)){
            gtk_spin_button_set_numeric(GTK_SPIN_BUTTON(Sb->SpinButton) , FALSE);
            }

        //Arrondissement
        if(Sb->arrondissement == 1){
            gtk_spin_button_set_snap_to_ticks(GTK_SPIN_BUTTON(Sb->SpinButton) , TRUE);
        }

        //color
        if(Sb->bgcolor)
        add_bgcolor(GTK_WIDGET(Sb->SpinButton) ,Sb->bgcolor->color,Sb->bgcolor->opacity);

    }

    else
    {
        printf("\nIntervalle du SpinButton n'est pas correct\n");
        exit(0);
    }
    if(Sb->conteneur)
        gtk_container_add(Sb->conteneur,Sb->SpinButton);



}
















///********************************************************SCALE************************************************///
///**************************************************************************************************************///
///*************************************************************************************************************///



typedef struct
{
    widg scale;             // objet scale
    GtkOrientation type;    // type de scale : H ou V
    GtkPositionType posDig;// le position dans laquelle la valeur actuelle est afficher
    gdouble Min;          // valeur minimale
    gdouble Max;         //valeur maximale
    gdouble step;       //le pas de deplacement
    gint domDig;        // domain des valeur (1:reels,0:entiers)
    coordonnees C;

}MonScale;



/*************************************************************
  La fonction : init_scale
  -Entrees :  pere ou conteneur du scale
      titre du scale
      type du scale :
         GTK_ORIENTATION_VERTICAL
         GTK_ORIENTATION_HORIZONTAL
      un positionnement de la valeur du Scale (posD):
           { GTK_POS_LEFT,(à gauche)
           GTK_POS_RIGHT,(a droit)
           GTK_POS_TOP, (en haut)
           GTK_POS_BOTTOM (en bas) }
      un entier (domD): determinant le domain des pas soit des reels (1)
           ou des entiers (0)
  -Sortie: La structure MonScale apres initialisation
  -Description: Cette fonction permet d'allouer de l'espace memoire pour notre variable de type
                   MonScale et de l'initaliser
*************************************************************/
MonScale *init_scale(GtkWidget*pere,gchar*titre,GtkOrientation type,
                     GtkPositionType posD,gdouble min,gdouble max,gdouble step,gint domD,
                     coordonnees cord)
{
    //declaration et allocation de la memoire pour un pointeur de type MonScale
    MonScale* S=(MonScale*) malloc(sizeof (MonScale));
    test_allocation(S,"scale");//verifier l'allocation
    if(titre)//affectation du titre s'il existe
    {
        S->scale.titre=(gchar*) malloc(maxTitre*sizeof (gchar));
        test_allocation(S->scale.titre,"titre scale");
        strcpy(S->scale.titre,titre);
    }
    if(pere)
    S->scale.conteneur=pere;//affectation du conteneur
    else S->scale.conteneur=NULL;
    S->type=type;//affectation du type de scale
    S->posDig=posD;//affectation du positionnement de la valeur du Scale
    S->Max=max;//valeur maximale
    S->Min=min;//valeur minimale
    S->step=step;//pas de variation
    S->domDig=domD;//domain des valeurs
    S->C=cord;
    return ((MonScale*) S);
}//fin
/*************************************************************
 La fonction : creer_scale
 Entree :Un pointeur vers la structure MonScale
           initialisée
 Sortie :  Un pointeur vers la variable S apres la creation
 Description : Cette fonction permet de créer un Scale en
Fonction des paramètres récupérés de la structure MonScale
*************************************************************/
MonScale *creer_scale(MonScale* S)
{   //creation du scale
    S->scale.elem=gtk_scale_new_with_range(S->type,S->Min,S->Max,S->step);
    //determiner le domain des valeurs du Scale
    gtk_scale_set_digits(GTK_SCALE(S->scale.elem),S->domDig);
    //determiner le positionnement du valeur par rapport au scale
    gtk_scale_set_value_pos (GTK_SCALE (S->scale.elem),S->posDig);
    //affectation du titre au scale s'il existe
    if(S->scale.titre) gtk_widget_set_name(S->scale.elem,S->scale.titre);
    if(S->scale.conteneur)
        gtk_container_add(S->scale.conteneur,S->scale.elem);

    return ((MonScale*) S);
}//fin
















/*******************************************FIXED********************************************/
///****************************************************************************************************************
///************************************************************************************************************
///************************************************************************************************************


/**   Structure du fixed   **/

typedef struct
{
 widg fix;

 dimension d;
 color_widg *couleur;
 coordonnees C;
}Monfixed;


Monfixed *init_Fixed(dimension D,gchar*titre,GtkWidget*conten,color_widg *couleur,coordonnees cord)
{
    Monfixed*F=NULL;

    //allocation de la memoire de F
    F=(Monfixed*) malloc(sizeof (Monfixed));
    //verifier l'allocation
    test_allocation(F,"Monfixed");
    if(titre)//titre no vide
    {//allocation de la memoire du titre du box
        F->fix.titre = (gchar *) malloc(100*sizeof(titre));
        //verifier l'allocation
        test_allocation(F->fix.titre,"titre Box");
        strcpy(F->fix.titre, titre);//affecter le titre
    }
    else F->fix.titre=NULL;
    if(conten) {F->fix.conteneur=conten;}//affecter le conteneur
    else F->fix.conteneur=NULL;
    F->couleur=(color_widg*)malloc(sizeof(color_widg));
    test_allocation(F->couleur,"allocation_couleur");
    if(couleur){
        F->couleur=couleur;
        }
        else{
            F->couleur=NULL;
        }
        F->d=D;
        F->C=cord;
    return((Monfixed*)F);

}


Monfixed *creer_Fixed(Monfixed* F)
{
     F->fix.elem= gtk_fixed_new();
     if(F->fix.conteneur)
         gtk_container_add(GTK_CONTAINER(F->fix.conteneur),F->fix.elem);
     if(F->couleur)
     {
           add_bgcolor(F->fix.elem,F->couleur->color,F->couleur->opacity);
     }
     gtk_widget_set_size_request(F->fix.elem,F->d.width,F->d.height);
     return((Monfixed*)F);

}




Monfixed *ajouter_Fixed(Monfixed *F,GtkWidget *fils,coordonnees C)
{
    gtk_fixed_put(GTK_FIXED(F->fix.elem),fils,C.x,C.y);
    return((Monfixed*)F);
}















/**  Les macros du menu   **/



///---------------------------------------------------------------------------------------------------------------------------
///-------------------------------------------    Menu   -----------------------------------------------------------------------------
///-------------------------------------------------------------------------------------------------------------------------


typedef struct
{
    GtkWidget *barre; ///la barre de menu elle-même
    GtkWidget *menu; ///la partie déroulante qui contient les différentes éléments
    GtkWidget *ptr_menu;///c'est sur ce widget que l'utilisateur clique pour lancer une action
    gchar* titre;
}MonMenu;




/// Structure de position d'un élement menu
typedef struct
{
/// Emplacement
   int x1;
   int x2;
 /// Niveau d'emplacement
   int y1;
   int y2;
}place_elem;



/// structure barre menu
typedef struct
{
    GtkWidget* barre;
    dimension dim;
    color_widg * coleur;

}bar_menu;


///**************************************************************************************/
/// Ajouter couleur

MonMenu* color_menu(MonMenu* M,int choix,color_widg* C)
{
  if(C)
  {
  switch(choix)
  {
      case 1 : add_bgcolor(M->ptr_menu,C->color,C->opacity);   break; /// colorer la tete du menu
      case 2 : add_bgcolor(M->menu,C->color,C->opacity);break; ///colorer les élements du menu
      case 3 :add_bgcolor(M->ptr_menu,C->color,C->opacity); /// les deux
              add_bgcolor(M->menu,C->color,C->opacity); break;
  }
  }
  return (MonMenu*) M;
}
/********************************************************************************/
///Creer barre menu
bar_menu* Creer_bar_menu(dimension d,color_widg * couleur)
{
    bar_menu* MaBar=NULL;
    MaBar=( bar_menu*)malloc(sizeof(bar_menu));
    test_allocation(MaBar,"barre menu");
    MaBar->barre=gtk_menu_bar_new();
    gtk_widget_set_size_request(MaBar->barre,d.width,d.height);
    if(couleur) add_bgcolor(MaBar->barre,couleur->color,couleur->opacity);


return (bar_menu*) MaBar;
}

/********************************************************************************/
/// Ajouter  elem barre
GtkWidget * Ajouter_elm_bar(bar_menu* bar , gchar* label)
{
    GtkWidget * elem;
    elem=gtk_menu_item_new_with_label(label);
    gtk_container_add(GTK_MENU_BAR(bar->barre),elem);
 return (GtkWidget *) elem;
}



/**************************************************************************/
///initialiser menu
 MonMenu* init_menu(gchar*label)
{
    MonMenu* menu;
    //allocation de la memoire pour le menu
   menu=(MonMenu*)malloc(sizeof(MonMenu));
    test_allocation(menu,"menu");
    if(label){
        menu->titre= (gchar*)malloc(30*sizeof(gchar));
        test_allocation( menu->titre,"titre menu");
        strcpy(menu->titre,label);
             }

return (MonMenu*) menu;
}

/******************************************************************************/
/// creer menu
MonMenu* Creer_Menu(MonMenu* monMenu)
{
    monMenu->barre=gtk_menu_bar_new();
    monMenu->menu=gtk_menu_new();
    monMenu->ptr_menu=gtk_menu_item_new_with_label(monMenu->titre);
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(monMenu->ptr_menu),monMenu->menu); /// attacher
    gtk_menu_shell_append(GTK_MENU_BAR(monMenu->barre), monMenu->ptr_menu); /// insertion du menu dans la barre

    return (MonMenu*) monMenu;
}
/*************************************************************************************/
/// creer un menu dans une barre spécifique
MonMenu* Creer_Menu_barre(bar_menu* bar,MonMenu* monMenu)
{
    monMenu->barre=bar->barre;
    monMenu->menu=gtk_menu_new();
    monMenu->ptr_menu=gtk_menu_item_new_with_label(monMenu->titre);
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(monMenu->ptr_menu),monMenu->menu); /// attacher
    gtk_menu_shell_append(GTK_MENU_BAR(bar->barre), monMenu->ptr_menu); /// insertion du menu dans la barr

 return (MonMenu*) monMenu;

}
/**************************************************************************************/
/// ajouter un element menu

GtkWidget * Ajouter_elem(MonMenu* monMenu,gchar* label,color_widg * couleur,place_elem* pos )
{
 GtkWidget *ptr_item;
ptr_item= gtk_menu_item_new_with_label(label);
if(!pos) gtk_menu_shell_append(GTK_MENU_SHELL(monMenu->menu),ptr_item);
else gtk_menu_attach (GTK_MENU(monMenu->menu),ptr_item,pos->x1,pos->x2,pos->y1,pos->y2);
if(couleur) add_bgcolor(ptr_item,couleur->color,couleur->opacity);
return ptr_item;
}

/************************************************************************************************/
/// Ajouter un sous-menu
MonMenu* Ajouter_sous_menu(MonMenu* menu_principale,MonMenu* Sous_Menu,place_elem* pos)
{
   Sous_Menu->menu=gtk_menu_new();
   Sous_Menu->ptr_menu=gtk_menu_item_new_with_label(Sous_Menu->titre);
   gtk_menu_item_set_submenu(GTK_MENU_ITEM(Sous_Menu->ptr_menu),Sous_Menu->menu);
   if(pos) gtk_menu_attach(GTK_MENU(menu_principale->menu),Sous_Menu->ptr_menu,pos->x1,pos->x2,pos->y1,pos->y2);
    else gtk_menu_shell_append(GTK_MENU_SHELL(menu_principale->menu), Sous_Menu->ptr_menu ); /// insertion du sous menu dans le menu principale

return (MonMenu*) Sous_Menu;
}







/*****************************************************************************************************

                                            GTK_ENTRY
**************************************************************/
///****************************************************************************************************************
///************************************************************************************************************
///************************************************************************************************************






///structure de gtk entry

typedef struct

{

    GtkWidget *entry; //widget d'une entree.
    dimension dim; //dimension du zone d entree
    gboolean visible; //le texte est visible ou pas.
    gboolean editable; //on peut saisir par clavier.
    gboolean frame;//on peut ajouter frame au gtk_entry
    const char* text_to_set; //le texte a saisir.
    const char* text_to_get;//le texte a recuperer.
    gint placeholder;/*0:pas de place holder
                       1:avec place holder*/
    gchar *titre;
    color_widg *couleur;
    coordonnees C;
    GtkWidget *conteneur;
}MonEntry;

/**********************************************************
La fonction : init_entry
-Entrees : titre:le titre du label
 texte : le texte du label
 conten : le conteneur du label
 coordonnee: les coordonnees du label
 dimention : la dimention du label
-Sortie : L : Objet label
-Description: Cette fonction permet d'allouer de l'espace pour
 la structure de type Monlabel, d'initialiser
 ses champs et de la retourner .
***********************************************************/
MonEntry *init_entry(dimension D,GtkWidget*cont
                       , color_widg*couleur)

{

    MonEntry *E=NULL;

    E=(MonEntry*)malloc(sizeof(MonEntry));

    test_allocation(E,"entry");



    E->dim=D;

    E->visible=TRUE;
    E->editable=TRUE;

//    E->text_to_get=(gchar*)malloc(300*sizeof(gchar));
//
//   test_allocation(E,"entry text get");

//    if(txt_get) strcpy(E->text_to_get,txt_get);
//    else E->text_to_get=NULL;

//    E->text_to_set=(gchar*)malloc(100*sizeof(gchar));
//
//    test_allocation(E,"entry text set");
//
//    if(txt_set) strcpy(E->text_to_set,txt_set);
//    else E->text_to_set=NULL;

    E->titre=(gchar*)malloc(30*sizeof(gchar));

    test_allocation(E,"entry titre");

     strcpy(E->titre,"Mon_Entry");


   if(couleur)
    E->couleur=couleur;
   else E->couleur=NULL;
E->C=walo();
E->conteneur=cont;
    return ((MonEntry*)E);



}





/**********************************************************
La fonction : creer_Entry
-Entrees : L : objet entry initialisé
-Sortie : L : Objet label créé
-Description:Cette fonction permet la creation d'un label
 en fct des paramètres récuperés de la structure
 Monlabel initialisée.
***********************************************************/


MonEntry *creer_entry(MonEntry *E)

{
    MonBox*box;
    box=Init_box(E->conteneur,"inter",'H',0,E->dim,0,0,NULL,E->C);
     box=creer_Box(box);
    E->entry=gtk_entry_new();

//    if(E->text_to_set)
//    gtk_entry_set_text(GTK_ENTRY(E->entry),E->text_to_set);

//    gtk_entry_get_text(GTK_ENTRY(E->entry));

//    gtk_entry_get_visibility(GTK_ENTRY(E->entry));
gtk_entry_set_visibility (E->entry,E->visible);

gtk_entry_set_has_frame(E->entry,TRUE);
//gtk_entry_set_editable(GTK_EDITABLE(E->entry),E->editable);

    gtk_widget_set_size_request(E->entry,E->dim.width,E->dim.height);
   if(E->titre)
    gtk_widget_set_name(E->entry,E->titre);
    E->entry=gtk_search_entry_new();
        gtk_entry_set_placeholder_text (E->entry,"Ecrire qlq chose");
        if(E->couleur)
            add_bgcolor(E->entry,E->couleur->color,E->couleur->opacity);
            if(E->conteneur){
                gtk_container_add(box->box.elem,E->entry);

                gtk_container_add(E->conteneur,box->box.elem);

            }

    return ((MonEntry*)E);

}





///**********************************************LABEL******************************************************************
///************************************************************************************************************
///************************************************************************************************************



typedef struct

{

    widg label; // objet label

    gchar * texte;//texte du label

    dimension dim; //dimension de la zone texte
    coordonnees C;
}Monlabel;





/**********************************************************
La fonction : init_label
-Entrees : titre:le titre du label
 texte : le texte du label
 conten : le conteneur du label
 coordonnee: les coordonnees du label
 dimention : la dimention du label
-Sortie : L : Objet label
-Description: Cette fonction permet d'allouer de l'espace pour
 la structure de type Monlabel, d'initialiser
 ses champs et de la retourner .
***********************************************************/

Monlabel *init_label(gchar *tit,gchar *texte,GtkWidget *conten,dimension D,coordonnees cord)

{

    Monlabel* L;
    //allocation de la memoire pour l element

    L=(Monlabel*)malloc(sizeof(Monlabel));

    test_allocation(L,"lable");


        //allocation de la memoire pour le titre
        L->label.titre= (gchar*)malloc(30*sizeof(gchar));

        test_allocation( L->label.titre,"titre lable");
       //initialisation du titre
       if(tit)
        strcpy(L->label.titre,tit);
        else L->label.titre=NULL;

if(conten)
    L->label.conteneur=conten;
else  L->label.conteneur=NULL;

L->texte=(gchar*)malloc(300*sizeof(gchar));
test_allocation(L->texte,"text label");
if(texte)
    strcpy(L->texte,texte);
else L->texte=NULL;

L->dim=D;
L->C=cord;
return((Monlabel*)L);

}






/**********************************************************
La fonction : creer_label
-Entrees : L : objet label initialisé
-Sortie : L : Objet label créé
-Description:Cette fonction permet la creation d'un label
 en fct des paramètres récuperés de la structure
 Monlabel initialisée.
***********************************************************/






Monlabel *creer_label(Monlabel *L)

{

   //creation d un  label avec un texte
    L->label.elem=gtk_label_new(L->texte);
    //donner un nom au label
    if(L->label.titre) gtk_widget_set_name(L->label.elem,L->label.titre);

    gtk_widget_set_size_request(L->label.elem,L->dim.width,L->dim.height);
    if(L->label.conteneur)
         gtk_container_add(GTK_CONTAINER(L->label.conteneur),L->label.elem);

    return ((Monlabel*)L);



}




/********************************Dialogue********************************/
///****************************************************************************************************************
///************************************************************************************************************
///************************************************************************************************************

///                 structure du dialgue


typedef struct
{
 GtkWidget *dialog;//dialog
 GtkWidget *content_area;//espace de la fenetre dialogue ou on peut ajouter des widgets
 GtkWindow *tran;//transitoire de la fenetre dialogue
 gchar *titre;//Le titre de la fenetre dialogue
 gchar *icon;//icon de la fenetre dialogue

 gint modal;//1 s'il est modal:(dialogue s'affiche une seule fois),0 sinon
 dimension dim;//dimention de la fenetre dialogue
 color_widg *couleur;
}MonDialog;


///****************************************initialiser dialog************************************************************************
///************************************************************************************************************
///************************************************************************************************************


MonDialog *init_dialog(GtkWindow *parent , gchar* titre ,gchar* icon ,gint modal,
 dimension dim,color_widg *couleur)
{

 MonDialog *D;
 D =(MonDialog*)malloc(sizeof(MonDialog));
 //verifier d'allocation
 test_allocation(D,"fenetre dialogue");
 //titre


 D->titre = (gchar*)malloc(maxTitre*sizeof(gchar));
 test_allocation(D->titre,"titre dialog");

 if(titre) strcpy(D->titre , titre);
 else D->titre=NULL;
 //icon





     if(icon)
    {//Allocation de la memoire pour le chemin de l'icon
        D->icon = (gchar *) malloc(100*sizeof(gchar));
        test_allocation(D->icon, "Icon fenetre");
        strcpy(D->icon, icon);//donner le titre a notre fenetre
    }
    else D->icon=NULL;

 //transient
 D->tran = parent ;
 //modal
 D->modal = modal;
 //height and width
 D->dim=dim;
 //position

 if(parent)
    D->tran=parent;
 else D->tran=NULL;
 if(couleur)
    D->couleur=couleur;
 else D->couleur=NULL;

 return((MonDialog*)D);
}//fin



///**************************************creer dialog**********************************************************
///************************************************************************************************************
///************************************************************************************************************
///************************************************************************************************************



void creer_dialog(MonDialog *D)
{
 //creer un dialog
 D->dialog=gtk_dialog_new();
 //affecter le titre
 if(D->titre)
 gtk_window_set_title( GTK_WINDOW(D->dialog) ,D->titre);
 //affecter l'icon
 if(D->icon)
 gtk_window_set_icon_from_file(GTK_WINDOW(D->dialog),D->icon ,NULL);
 //initialiser content-area

 D->content_area = gtk_dialog_get_content_area(GTK_DIALOG(D->dialog));
 //transient

 gtk_window_set_transient_for(GTK_WINDOW (D->dialog ) , GTK_WINDOW ( D->tran));
 //affecter la dimension
 gtk_widget_set_size_request ( GTK_WIDGET(D->dialog) , D->dim.width ,D->dim.height);
 //si la fenêtre dialogue est modal activer la modalité
 if(D->modal == 1)
 gtk_window_set_modal(GTK_WINDOW(D->dialog) , TRUE);
 else gtk_window_set_modal(GTK_WINDOW(D->dialog) , FALSE);
 //affecter le positionnement
if(D->couleur)
    add_bgcolor(D->dialog,D->couleur->color,D->couleur->opacity);
 if(D->tran)
 gtk_container_add(D->tran,D->dialog);
}//fin





/**
GTK_RESPONSE_NONE = -1 : La boîte de dialogue a été détruite par
le gestionnaire de fenêtres ou détruite par programme avec gtk_widget_destroy().
Ceci est également renvoyé si un widget de réponsen'a pas d'identifiant de réponse
défini.
GTK_RESPONSE_REJECT = -2 :Cet identifiant n'est pas associé aux boutons des
boîtes de dialogue intégrées, mais vous êtes libre de l'utiliser vous-même.
GTK_RESPONSE_ACCEPT = -3 :Cet identifiant n'est pas associé aux boutons
des boîtes de dialogue intégrées, mais vous êtes libre de l'utiliser vous-même.
GTK_RESPONSE_DELETE_EVENT = -4 :Chaque boîte de dialogue est automatiquement
connectée au signal d'événement de suppression. Pendant que gtk_dialog_run()
est en cours d'exécution, cet identifiant sera renvoyé et l'événement
de suppression ne pourra plus détruire la fenêtre comme d'habitude.
GTK_RESPONSE_OK = -5 : un bouton GTK_STOCK_OK a été cliqué dans
une boîte de dialogue intégrée. Vous êtes libre d'utiliser
ce bouton ou l'un des suivants dans vos propres boîtes de dialogue.
GTK_RESPONSE_CANCEL = -6 :
un bouton GTK_STOCK_CANCEL a été cliqué dans une boîte de dialogue intégrée.
GTK_RESPONSE_CLOSE = -7 :
un bouton GTK_STOCK_CLOSE a été cliqué dans une boîte de dialogue intégrée.
GTK_RESPONSE_YES = -8 :
un bouton GTK_STOCK_YES a été cliqué dans une boîte de dialogue intégrée.
GTK_RESPONSE_NO = -9 :
un bouton GTK_STOCK_NO a été cliqué dans une boîte de dialogue intégrée.
GTK_RESPONSE_APPLY = -10 :
un bouton GTK_STOCK_APPLY a été cliqué dans une boîte de dialogue intégrée.
GTK_RESPONSE_HELP = -11 :
un bouton GTK_STOCK_HELP a été cliqué dans une boîte de dialogue intégrée



**/



///** Add a button to dialog  **///

void ajouter_button_dialog(MonDialog *D,gchar *buttonlabel,gint idbutton)
{
    gtk_dialog_add_button(D->dialog,buttonlabel,idbutton);
}

///** Ajouter un element to  content_area  **///

void ajouter_content_area(MonDialog *D,GtkWidget *elem)
{
    gtk_box_pack_start(D->content_area,elem,0,0,0);
}


///***************COMBOBOX********************///
///*******************************************************************
///*******************************************************************
///*******************************************************************





//-------------------- HE WE GO --------------------
typedef struct {
    widg combo;
    dimension dim;
    coordonnees Crd;
    gint type;
}MonComboBox;
//INNITIALISATION
MonComboBox*init_comboBox(GtkWidget*cont,gchar*titre,dimension dim,coordonnees Crd,gint type)
{
    MonComboBox *Comb=(MonComboBox*)malloc(sizeof (MonComboBox));
    test_allocation(Comb,"combo");
    Comb->combo.titre=NULL;
    Comb->combo.conteneur=NULL;
    if(titre)
    {
        Comb->combo.titre=(gchar*) malloc(maxTitre*sizeof (gchar));
        test_allocation(Comb->combo.titre,"titre combo");
        strcpy(Comb->combo.titre,titre);
    }
    Comb->dim=dim;
    Comb->Crd=Crd;
    Comb->type=type;//1 un combo des textes suelement, sinon un combo des textes avec entry
    if(cont) Comb->combo.conteneur=cont;
    return (MonComboBox*)Comb;
}
//CREATION
MonComboBox*creer_comboBox(MonComboBox*comb)
{
    if(comb->type==1)comb->combo.elem=gtk_combo_box_text_new ();
    else comb->combo.elem=gtk_combo_box_text_new_with_entry ();
    if(comb->combo.conteneur)
        gtk_container_add (GTK_CONTAINER (comb->combo.conteneur), comb->combo.elem);
    if(comb->combo.titre)  gtk_widget_set_name(comb->combo.elem,comb->combo.titre);
    return (MonComboBox*)comb;
}
//AJOUTER UN TEXTE
MonComboBox*ajouter_string_comboBox(MonComboBox*comb,gchar*element)
{
    if(element)
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(comb->combo.elem),element);
    return (MonComboBox*)comb;
}
//ajouter un nombre (example les jours du mois)
MonComboBox*ajouter_number_comboBox(MonComboBox*comb,gdouble num)
{
    gchar element[10];
    sprintf(element, "%.0f", num);
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(comb->combo.elem),element);
    return (MonComboBox*)comb;
}

















///*****************************************Scroll bare*************************************************///
///**********************************************************************************************
///************************************************************************************************
///****************************************************************************************************


typedef struct {
    GtkWidget *swin;
    GtkWidget *cont;//generalement un window mais peut etre un box
    GtkWidget *contFils;// grid generalement ou fixed
    gint borderWidth;
    dimension dim;
}MonScroll;
/*****************************TREEEEES IMPORTANT****************************
 remember dim of contFils bigger than swin and swin dim equal or less than cont
 ****************************FIN D'IMPORTANCE****************************/






///initialisation*********************************************



MonScroll *init_Scroll(GtkWidget *cont,  GtkWidget *contFils,gint brW,dimension dim)
{
    MonScroll *scroll= (MonScroll*)malloc(sizeof (MonScroll));
    test_allocation(scroll,"scorll");
    //initialisation des donnees par defaut
    scroll->cont=NULL;
    scroll->contFils=NULL;
    scroll->borderWidth=0;
    scroll->dim.height=scroll->dim.width=100;
    //verifications des entrees et affectation
    if(cont) scroll->cont=cont;
    if(contFils) scroll->contFils=contFils;
    if(brW>0) scroll->borderWidth=brW;
    if(dim.width>100) scroll->dim.width= dim.width;
    if(dim.height>100) scroll->dim.height= dim.height;
    return (MonScroll *)scroll;
}



///creation**********************************************



MonScroll *creer_Scroll(MonScroll *scroll)
{
    //creation du scroll
    scroll->swin=gtk_scrolled_window_new (NULL, NULL);
    if(scroll->cont)
    {   // affectation du scroll au widget parent
        gtk_container_add (GTK_CONTAINER (scroll->cont), scroll->swin);
    }
    //affectation du taille de la bordure
    gtk_container_set_border_width (GTK_CONTAINER (scroll->swin), scroll->borderWidth);
    if(scroll->contFils)
    {   //affectation du widget conteneur fils
        gtk_container_add(GTK_CONTAINER(scroll->swin), scroll->contFils);
    }
    //affectation de la taille du scroll
    gtk_widget_set_size_request (scroll->swin, scroll->dim.width, scroll->dim.height);
    return (MonScroll *)scroll;
}








typedef struct {
 widg frame; // le cadre
 GtkShadowType shadow_type;//le type de l’ombre
 float label_xalign; /*l’alignement x de l’étiquette le long du bord
 supérieur du widget*/
 float label_yalign; //l’alignement y de l’étiquette
 dimension dim; //dimension du cadre
 coordonnees C;
}MonFrame;





/*************************************************************
 La fonction : init_Frame
 -Entrees : titre du cadre
 Chtype: indique le type du contour du cadre
 GTK_SHADOW_NONE
 GTK_SHADOW_IN
 GTK_SHADOW_OUT
 GTK_SHADOW_ETCHED_IN
 GTK_SHADOW_ETCHED_OUT
 label_xalign: indique l’alignement de l’étiquette au bord supérieur.
 La valeur 0.0 représente l’alignement à gauche,
 la valeur 1.0 représente l’alignement à droite.
 label_yalign: indique l’alignement y de l’étiquette.
 La valeur 0.0 s’aligne sous le cadre,
la valeur 1.0 s’aligne au-dessus du cadre
 cont: pointeur sur le conteneur du cadre.
 dim: dimention de notre cadre {width,hieght}
 C: coordonnees de notre cadre
 -Sortie: La structure MonFrame apres initialisation
 -Description: Cette fonction permet d'allouer de l'espace memoire pour
 notre variable de type MonFrame et de l'initaliser
*************************************************************/
MonFrame *init_Frame(GtkWidget*cont,dimension dim,
 float xalign,float yalign,gchar*titre,GtkShadowType Chtype,coordonnees cord)
{
 MonFrame * F;
 //allocation de la memoire pour l element
 F=(MonFrame *)malloc(sizeof(MonFrame));
 test_allocation(F,"Frame");
 if(titre)
 {
 //allocation de la memoire pour le titre
 F->frame.titre= (gchar*)malloc(maxTitre*sizeof(gchar));
 test_allocation( F->frame.titre,"titre Frame");
 //initialisation du titre
 strcpy( F->frame.titre,titre);
 }
 F->shadow_type=Chtype;//initialisation du type de contour
 F->label_xalign=xalign;//initialisation de lalignement x de l'etiquette
 F->label_yalign=yalign;//initialisation de lalignement y de l'etiquette

 F->dim=dim;// initialisation du dimention
 F->frame.conteneur=cont;//initialisation du conteneur du cadre
 F->C=cord;
 return ((MonFrame *)F);
}//fin

/**********************************************************
La fonction : creer_Frame
-Entrees : L : objet MonFrame initialise
-Sortie : L : Objet MonFrame cree
-Description:Cette fonction permet la creation d'un cadre
en fonction des parametres recuperes de la structure
MonFrame initialisee.
***********************************************************/
MonFrame *creer_Frame(MonFrame *F)
{
 F->frame.elem = gtk_frame_new(F->frame.titre);//affecter le titre
 //affecter lalignement de l'etiquette
 gtk_frame_set_label_align(GTK_FRAME(F->frame.elem),F->label_xalign,F->label_yalign);
 //affecter le type de contour
 gtk_frame_set_shadow_type(GTK_FRAME(F->frame.elem),F->shadow_type);
 //affecter la dimension si elle positive (existe)
 if((F->dim.height)>0 && (F->dim.width)>0)
 gtk_widget_set_size_request(F->frame.elem,F->dim.width, F->dim.height);
 //mettre le cadre dans son conteneur
 if(F->frame.conteneur)
 gtk_container_add(GTK_CONTAINER(F->frame.conteneur),F->frame.elem);
 return((MonFrame *)F);
}//fin


///**************************************IMAGE**************************************///



/*
 * structure de l'image
 */
typedef struct
{
    widg image;         //l'objet image
    gchar *path;       //le chemin vers l'image
    coordonnees ImCrd;//position de l'image
}MonImage;



MonImage* init_image(gchar* url,gchar *titre,coordonnees C,GtkWidget *cont)
{
    //Allocation memoire
    MonImage* IM = NULL;
    IM = (MonImage*)malloc(sizeof(MonImage));
    IM->path = (gchar*)malloc(sizeof(strlen(url)));
    if(!(IM->path))
    {
        printf("erreur");
        exit(0);
    }
    //Affectation du chemin
    strcpy(IM->path,url);
    //Affectation de positions
    IM->ImCrd.x = C.x;
    IM->ImCrd.y = C.y;
    IM->image.titre = (gchar*)malloc(sizeof(strlen(titre)));
    strcpy(IM->image.titre,titre);
    if(cont)
    IM->image.conteneur=cont;
    else
    IM->image.conteneur=NULL;
    return (MonImage*)IM;
}
MonImage* creer_image(MonImage* M)
{
    //Creation de l'image à partir du chemin
    M->image.elem= gtk_image_new_from_file(M->path);
    if(M->image.titre) gtk_widget_set_name(M->image.elem,M->image.titre);
    if(M->image.conteneur)
    gtk_container_add(GTK_CONTAINER(M->image.conteneur),M->image.elem);
    return (MonImage*)M;
}




///****************************************TOOLBARRE**********************************************///
///**************************************************************************************************///
///**********************************************************************************************///

typedef struct toolbarre
{

    GtkWidget *toolbar ; // barre d’outil
    GtkToolItem *toolitem; // les buttons de la barre
    gchar* icon; // l’icon d’apres le stock
    color_widg *color;
}Montool;

Montool * creat_toolbar(color_widg *color,GtkWidget *cont,dimension d){
  Montool *p;
  MonBox *B;
    p=(Montool*) malloc(sizeof (Montool));
    p->toolbar=gtk_toolbar_new();
    gtk_toolbar_set_style(GTK_TOOLBAR(p->toolbar), GTK_TOOLBAR_ICONS);
    B=Init_box(cont,"tool",'v',0,d,0,0,&color,walo());
    B=creer_Box(B);
    gtk_container_add(B->box.elem,p->toolbar);
    if(color)
        add_bgcolor(p->toolbar,color->color,color->opacity);
    return((Montool* )p);

}


Montool * creat_toolitem(Montool* p,gchar* icone,gint pos)
{
//création d'un toolitem avec une icone du stock
    p->toolitem= gtk_tool_button_new_from_stock(icone);
//inseret le toolitem dans le toolbar
    gtk_toolbar_insert(GTK_TOOLBAR(p->toolbar), p->toolitem,pos);
    return((Montool*)p);

}
















//int main(int argc, char *argv[])
//{
//    //initialisation du main
//gtk_init(&argc, &argv);
//color_widg coul1={"grey",11},coul2={"yellow",9},coul3={"red",9},coul4={"white",99};
//POS p;p.pos='C';
//coordonnees c1={1,1},c2={0,200},c3={40,200},c4={80,200},c5={30,300},c6={0,100};
//dimension d1={500,500},d2={300,300},d3={200,200},d4={100,200},d5={20,50},d6={10,10};
//Fenetre *F;
//F=init_window("window",p,d1,1,NULL,NULL,20);
//F=creer_Window(F);
//MonBox*box;
//box=Init_box(F->window,"box",'H',0,d1,20,10,&coul1,walo());
//box=creer_Box(box);
//Mongrid*grid;
//grid=init_creer_Grid(NULL,box->box.elem,walo(),d2,&coul2);
//Monfixed*fixed;
//fixed=init_Fixed(d3,"fixed",grid->Grid.elem,&coul3,walo());
//fixed=creer_Fixed(fixed);
//MonFrame*frame;
//frame=init_Frame(NULL,d4,0,0,"frame",GTK_SHADOW_IN,walo());
//frame=creer_Frame(frame);
//ajouter_Fixed(fixed,frame->frame.elem,c1);
//Monlabel*label;
//label=init_label("label","tajriba",fixed->fix.elem,d5,walo());
//label=creer_label(label);
//MonEntry*entry;
//entry=init_entry(d5,frame->frame.elem,NULL);
//entry=creer_entry(entry);
////MonMenu*menu,*soum;
////bar_menu*barre;
////barre=Creer_bar_menu(box->box.elem,NULL,walo());//creer la barre
////barre=positionner_bar(barre,c2);//positionneer la barre
////menu=init_menu("Monmenu",NULL);//initialiser menu
////soum=init_menu("Monsoumenu",NULL);//initialiser menu
////Creer_Menu_barre(barre->barre,menu);//ajouter le menu au barre
////Ajouter_elm_bar(barre,"elem1");//ajouter element simple au barre
////Ajouter_elem(menu,"souelem1",NULL,NULL);//ajouter element simple zau menu
//
//MonBouton *bouton1,*bouton2,*bouton3,*bouton4,*bouton5,*bouton6;
//
//bouton4=init_Button(NULL,NULL,NULL,d6,"Coronavirus",NULL,2,0,NULL,walo());
//bouton4=creer_Bouton(bouton4);
//bouton5=init_Button(bouton4->button.elem,NULL,NULL,d6,"Ebolavirus",NULL,2,0,NULL,walo());
//bouton5=creer_Bouton(bouton5);
//bouton6=init_Button(bouton4->button.elem,NULL,NULL,d6,"Rotavirus",NULL,2,0,NULL,walo());
//bouton6=creer_Bouton(bouton6);
//box=ajouter_Box(box,bouton4->button.elem,0,0,0,1);
//box=ajouter_Box(box,bouton5->button.elem,0,0,0,1);
//box=ajouter_Box(box,bouton6->button.elem,0,0,0,1);
//MonScale*scale;
//scale=init_scale(box->box.elem,"scale",GTK_ORIENTATION_VERTICAL,GTK_POS_TOP,0.0,10.0,1.0,0,c5);
//scale=creer_scale(scale);
//box=ajouter_Box(box,scale->scale.elem,0,0,0,1);
//MonSpin*spin;
//spin=init_spin_button(0.0,10.0,1.0,0.0,d6,1,1,1,1,NULL,walo(),NULL);
//create_spin_button(spin);
//fixed=ajouter_Fixed(fixed,spin->SpinButton,c6);
//
//Afficher_widget(F->window);
//
//
//gtk_main();
//}


int main(int argc, char *argv[])
{
    //initialisation du main
gtk_init(&argc, &argv);

dimension d1={800,1500},d2={40,500},d3={20,30},d4={40,400},d5={10,20},d6={6,20},d7={40,300},d8={500,15};
coordonnees c1={0,200},c2={0,100},c3={10,350},c4={10,450},c5={800,550},c6={800,650},c0={0,-480};
POS p;p.pos='C';



Fenetre*window=NULL;
window=init_window("monwindow",p,d1,0,NULL,NULL,20);
window=creer_Window(window);
bar_menu*barre=NULL;
barre=Creer_bar_menu(d8,NULL);

MonMenu*menu=NULL;
menu=init_menu("monmenu");
menu=Creer_Menu_barre(barre,menu);
menu->menu= Ajouter_elem(menu,"elem1",NULL,NULL);

Monfixed *fixed=NULL;
fixed=init_Fixed(d1,"monfixed",window->window,NULL,walo());
fixed=creer_Fixed(fixed);
fixed= ajouter_Fixed(fixed,barre->barre,c0);
MonFrame*frame=NULL;
frame=init_Frame(NULL,d2,10,10,"frame",GTK_SHADOW_IN,walo());
frame=creer_Frame(frame);
fixed= ajouter_Fixed(fixed,frame->frame.elem,c1);
fixed= ajouter_Fixed(fixed,barre->barre,c0);
MonBox*box=NULL;
box=Init_box(frame->frame.elem,"box",'H',0,d2,30,0,NULL,walo());
box=creer_Box(box);
MonBouton*b1,*b2,*b3,*b4;
b1=init_Button(NULL,NULL,NULL,d3,"Convivial",NULL,3,0,NULL,walo());
b2=init_Button(NULL,NULL,NULL,d3,"interessant",NULL,3,0,NULL,walo());
b3=init_Button(NULL,NULL,NULL,d3,"facile a utuliser",NULL,3,0,NULL,walo());
b4=init_Button(NULL,NULL,NULL,d3,"optimise le travail",NULL,3,0,NULL,walo());
b1=creer_Bouton(b1);
b2=creer_Bouton(b2);
b3=creer_Bouton(b3);
b4=creer_Bouton(b4);
box=ajouter_Box(box,b1->button.elem,0,0,20,1);
box=ajouter_Box(box,b2->button.elem,0,0,20,1);
box=ajouter_Box(box,b3->button.elem,0,0,20,1);
box=ajouter_Box(box,b4->button.elem,0,0,20,1);





MonFrame*frame2=NULL;
frame2=init_Frame(NULL,d4,10,10,"frame2",GTK_SHADOW_IN,walo());
frame2=creer_Frame(frame2);
fixed=ajouter_Fixed(fixed,frame2->frame.elem,c2);
//
MonBox*box2=NULL;
box2=Init_box(frame2->frame.elem,"box2",'H',0,d4,30,0,NULL,walo());
box2=creer_Box(box2);
MonBouton*b11,*b22,*b33,*b44;
b11=init_Button(NULL,NULL,NULL,d3,"faible",NULL,2,0,NULL,walo());
b11=creer_Bouton(b11);
b22=init_Button(b11->button.elem,NULL,NULL,d3,"moyen",NULL,2,0,NULL,walo());
b22=creer_Bouton(b22);
b33=init_Button(b11->button.elem,NULL,NULL,d3,"bien",NULL,2,0,NULL,walo());
b33=creer_Bouton(b33);
b44=init_Button(b11->button.elem,NULL,NULL,d3,"tres bien",NULL,2,0,NULL,walo());
b44=creer_Bouton(b44);
box2=ajouter_Box(box2,b11->button.elem,0,0,20,1);
box2=ajouter_Box(box2,b22->button.elem,0,0,20,1);
box2=ajouter_Box(box2,b33->button.elem,0,0,20,1);
box2=ajouter_Box(box2,b44->button.elem,0,0,20,1);



MonFrame*frame3=NULL;
frame3=init_Frame(NULL,d4,10,10,"frame3",GTK_SHADOW_IN,walo());
frame3=creer_Frame(frame3);
fixed=ajouter_Fixed(fixed,frame3->frame.elem,c3);
//
MonBox*box3=NULL;
box3=Init_box(frame3->frame.elem,"box3",'H',0,d4,30,0,NULL,walo());
box3=creer_Box(box3);

MonComboBox*combo1,*combo2,*combo3;
combo1=init_comboBox(box3->box.elem,"combo1",d6,walo(),1);
combo1=creer_comboBox(combo1);
for(int i=1;i<=30;i++)
combo1=ajouter_number_comboBox(combo1,i);

combo2=init_comboBox(box3->box.elem,"combo2",d6,walo(),1);
combo2=creer_comboBox(combo2);
for(int j=1;j<=12;j++)
combo2=ajouter_number_comboBox(combo2,j);
combo3=init_comboBox(box3->box.elem,"combo3",d6,walo(),1);
combo3=creer_comboBox(combo3);
for(int k=1998;k<=2004;k++)
combo3=ajouter_number_comboBox(combo3,k);



MonFrame*frame4=NULL;
frame4=init_Frame(NULL,d7,10,10,"frame3",GTK_SHADOW_IN,walo());
frame4=creer_Frame(frame4);
fixed=ajouter_Fixed(fixed,frame4->frame.elem,c4);

MonBox*box4=NULL;
box4=Init_box(frame4->frame.elem,"box4",'H',0,d7,50,0,NULL,walo());
box4=creer_Box(box4);
MonComboBox*combo4;
combo4=init_comboBox(NULL,"combo4",walo2(),walo(),1);
combo4=creer_comboBox(combo4);
combo4->dim.height=30;combo4->dim.width=60;
box4=ajouter_Box(box4,combo4->combo.elem,0,0,0,0);
Monlabel*label=NULL;
label=init_label("lab1","Faire un choix",NULL,walo2(),walo());
label=creer_label(label);
box4=ajouter_Box(box4,label->label.elem,0,0,50,0);







MonFrame*frame5=NULL;
frame5=init_Frame(NULL,d2,0,10,"frame5",GTK_SHADOW_IN,walo());
frame5=creer_Frame(frame5);
ajouter_Fixed(fixed,frame5->frame.elem,c5);

MonBox*box5=NULL;
box5=Init_box(frame5->frame.elem,"box5",'H',0,d2,30,0,NULL,walo());
box5=creer_Box(box5);
MonBouton*b111,*b222,*b333,*b444;
b111=init_Button(NULL,NULL,NULL,d3,"malade",NULL,3,0,NULL,walo());
b222=init_Button(NULL,NULL,NULL,d3,"suspect",NULL,3,0,NULL,walo());
b333=init_Button(NULL,NULL,NULL,d3,"Gueri",NULL,3,0,NULL,walo());
b444=init_Button(NULL,NULL,NULL,d3,"Spain",NULL,3,0,NULL,walo());
b111=creer_Bouton(b1);
b222=creer_Bouton(b2);
b333=creer_Bouton(b3);
b444=creer_Bouton(b4);
box5=ajouter_Box(box5,b111->button.elem,0,0,20,1);
box5=ajouter_Box(box5,b222->button.elem,0,0,20,1);
box5=ajouter_Box(box5,b333->button.elem,0,0,20,1);
box5=ajouter_Box(box5,b444->button.elem,0,0,20,1);

MonBox*box6=NULL;
box6=Init_box(NULL,"box6",'H',0,d2,30,0,NULL,walo());
box6=creer_Box(box6);
box6->D.width=1000;
fixed=ajouter_Fixed(fixed,box6->box.elem,c6);
MonBouton*btn1,*btn2;
btn1=init_Button(NULL,NULL,NULL,walo2(),"   Quitter   ",NULL,1,1,NULL,walo());
btn1=creer_Bouton(btn1);
btn2=init_Button(NULL,NULL,NULL,walo2(),"    OK   ",NULL,1,1,NULL,walo());
btn2=creer_Bouton(btn2);
box6=ajouter_Box(box6,btn1->button.elem,0,0,0,0);
box6=ajouter_Box(box6,btn2->button.elem,0,0,30,0);

MonDialog*dialog;
dialog=init_dialog(window->window,"Mondialog",NULL,1,walo2(),NULL);
creer_dialog(dialog);
dialog->dim.height=100;
dialog->dim.width=300;
coordonnees cc={100,0};
ajouter_button_dialog(dialog,"         oui       ",1);
ajouter_button_dialog(dialog,"         non      ",2);
ajouter_button_dialog(dialog,"         annuler       ",3);
Monfixed *fixed2=NULL;
fixed2=init_Fixed(walo2(),"monfixed2",window->window,NULL,walo());
fixed2=creer_Fixed(fixed2);

Monlabel*label2;
label2=init_label("label2","vous voulez afficher un message ?",NULL,walo2(),walo());
label2=creer_label(label2);
fixed2=ajouter_Fixed(fixed2,label2->label.elem,cc);

ajouter_content_area(dialog,fixed2->fix.elem);



Afficher_widget(window->window);
Afficher_widget(dialog->dialog);
gtk_main();
}



















































