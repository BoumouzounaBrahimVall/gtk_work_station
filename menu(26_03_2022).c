
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
