
///---------------------------------------------------------------------------------------------------------------------------
///-------------------------------------------    Menu   -----------------------------------------------------------------------------
///-------------------------------------------------------------------------------------------------------------------------


typedef struct
{
    GtkWidget *barre; ///la barre de menu elle-même
    GtkWidget *menu; ///la partie déroulante qui contient les différentes éléments
    GtkWidget *ptr_menu;///c'est sur ce widget que l'utilisateur clique pour lancer une action
    GtkWidget  *conteneur; /// le conteneur
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


///********************************************************************************************/
/// Ajouter couleur

void color_menu(MonMenu* M,int choix,color_widg* C)
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
}

/**************************************************************************/
///initialiser menu
 MonMenu* init_menu(gchar*label,GtkWidget *cont)
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
   if(cont)
        menu->conteneur=cont;
return (MonMenu*) menu;
}

/******************************************************************************/
/// creer menu
void Creer_Menu(MonMenu* monMenu)
{
    monMenu->barre=gtk_menu_bar_new();
    monMenu->menu=gtk_menu_new();
    monMenu->ptr_menu=gtk_menu_item_new_with_label(monMenu->titre);
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(monMenu->ptr_menu),monMenu->menu); /// attacher
    gtk_menu_shell_append(GTK_MENU_SHELL(monMenu->barre), monMenu->ptr_menu); /// insertion du menu dans la barre
if(monMenu->conteneur)
   gtk_container_add(GTK_CONTAINER(monMenu->conteneur),monMenu->barre);

}
/*************************************************************************************/
/// creer un menu dans une barre spécifique
void Creer_Menu_barre(GtkWidget* barre,MonMenu* monMenu)
{
    monMenu->barre=barre;
    monMenu->menu=gtk_menu_new();
    monMenu->ptr_menu=gtk_menu_item_new_with_label(monMenu->titre);
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(monMenu->ptr_menu),monMenu->menu); /// attacher
    gtk_menu_shell_append(GTK_MENU_SHELL(monMenu->barre), monMenu->ptr_menu); /// insertion du menu dans la barre


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
void Ajouter_sous_menu(MonMenu* menu_principale,MonMenu* Sous_Menu,place_elem* pos)
{
   Sous_Menu->menu=gtk_menu_new();
   Sous_Menu->ptr_menu=gtk_menu_item_new_with_label(Sous_Menu->titre);
   gtk_menu_item_set_submenu(GTK_MENU_ITEM(Sous_Menu->ptr_menu),Sous_Menu->menu);
   if(Sous_Menu->conteneur)
    {
        if(pos) gtk_menu_attach(GTK_MENU(menu_principale->menu),Sous_Menu->ptr_menu,pos->x1,pos->x2,pos->y1,pos->y2);
        else gtk_menu_shell_append(GTK_MENU_SHELL(Sous_Menu->conteneur), Sous_Menu->ptr_menu ); /// insertion du sous menu dans le menu principale
    }
}

///-----------------------------------------------------------------------------------------------------------------/
typedef struct
{
    GtkWidget* barre;
    GtkWidget * conteneur; /// où tu veut placer la barre
    color_widg * coleur;

}bar_menu;
///Creer barre menu
bar_menu* Creer_bar_menu(GtkWidget * cont,color_widg * couleur)
{
    bar_menu* MaBar=NULL;
    MaBar=( bar_menu*)malloc(sizeof(bar_menu));
    test_allocation(MaBar,"barre menu");
    MaBar->barre=gtk_menu_bar_new();
    if(cont) MaBar->conteneur=cont;
    if(couleur) add_bgcolor(MaBar->barre,couleur->color,couleur->opacity);


return (bar_menu*) MaBar;
}

/// Ajouter  elem barre
GtkWidget * Ajouter_elm_bar(bar_menu* bar , gchar* label)
{
    GtkWidget * elem;
    elem=gtk_menu_item_new_with_label(label);
    gtk_container_add(GTK_MENU_BAR(bar->barre),elem);
 return (GtkWidget *) elem;
}

/// positionner une barre

 bar_menu* positionner_bar(bar_menu*MaBar,coordonnees c)
{
    Monfixed *f;
    dimension d={20,40}; /// fi ntidar t7aydoha
f=init_Fixed(d,"fixed",MaBar->conteneur,MaBar->coleur);
f=creer_Fixed(f);
f=ajouter_Fixed(f,MaBar->barre,c);
return  (bar_menu* )MaBar;
}




///**********************************************************************************************************************
///----------------------------------------------  MAIN  ---------------------------------------------------------------------
///**********************************************************************************************************************

int main(int argc, char *argv[])
{
    gtk_init(&argc, &argv);
    Fenetre* window;
    POS pos;
    dimension d={400,400};
   place_elem *p;
   p=(place_elem *)malloc(sizeof(place_elem));
   p->x1=0;
   p->x2=1;
   p->y1=0;
   p->y2=1;
 place_elem  *p1;
 p1=(place_elem *)malloc(sizeof(place_elem));
   p1->x1=1;
   p1->x2=2;
   p1->y1=0;
   p1->y2=1;

    pos.pos='C';

color_widg t={"white",90};

color_widg t1={"red",90};

window=init_window("fenetre",pos,d,1,"logo.png",t,10);
window=creer_Window(window);


MonBox *b;
b=Init_box(window->window,"BOX",'V',0,0,0,t);
b=creer_Box(b);




MonMenu* menu,*sous_m;
coordonnees c={100,100};
menu=init_menu("menu",NULL);
bar_menu  *bar;
bar=Creer_bar_menu(b->box.elem,&t1);
bar=positionner_bar(bar,c);
Ajouter_elm_bar(bar,"hassni");
Ajouter_elm_bar(bar,"mhd");
Creer_Menu_barre(bar->barre,menu);

sous_m=init_menu("sous-menu",menu->menu);
Ajouter_sous_menu(menu,sous_m,NULL);
Ajouter_elem(sous_m," sous-elem 1",NULL,p);
Ajouter_elem(sous_m,"sous-elem 2",NULL,p1);
Ajouter_elem(menu,"elem",NULL,NULL);

   gtk_widget_show_all(window->window);
   gtk_main();
return 0;
}