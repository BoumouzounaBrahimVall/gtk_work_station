

void add_bgcolor(GtkWidget*widget,gchar*couleur,gfloat opc)

{

    if(couleur==NULL) return;

    GdkRGBA color;
    gdk_rgba_parse(&color,couleur);
    color.alpha=opc;
    gtk_widget_override_background_color(widget,GTK_STATE_FLAG_NORMAL,&color);
}
typedef struct
{
    gint height; //longueur
    gint width; //largeur
}dimension;
typedef struct {
    GtkWidget *swin;
    GtkWidget *cont;//generalement un window mais peut etre un box
    GtkWidget *contFils;// grid generalement ou fixed
    gint borderWidth;
    dimension dim;
}MonScroll;
/*
 remember dim of contFils bigger than swin and swin dim equal or less than cont
 */
void test_allocation(void *pointeur, char*nom)
{
    if(!pointeur) {
        printf("erreur d'allocation du %s\n", nom);
        exit(-1);
    }
}
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

typedef struct
{
    gchar*color;
    gfloat opacity;
}color_widg;
/// ***SpinButton

typedef struct
{
    GtkWidget *SpinButton;//SpinButton
    gdouble min;//la valeur minimale
    gdouble max;//la valeur maximale
    gdouble step;//le pas
    gdouble value;//valeur par defaut
    gint height; //Hauteur du SpinButton
    gint width; //Longueur du SpinButton
    gint wrap;//0:FALSE , 1:TRUE
    gint digits;//nombre de chiffres en partie décimale
    gint numeric;//0:FALSE(tapez de caracteres en generale) , 1:TRUE(juste les chiffres)
    gint arrondissement;//0:FALSE , 1:TRUE
    color_widg *bgcolor;//background color

}MonSpin;


/*********************Macros ********************************/

/**
    fonction: init_spin_button
    entreés :
               gdouble val_min   :indique la valeur minimale du spin button
               gdouble val_max   :indique la valeur maximale du spin button
               gdouble pas       :indique le pas d'incrémentation ou décrémentation
               gdouble val_defaut:indique la valeur par défaut du spin button
               gint height       :indique la longueur du spin button
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
                                    gdouble val_defaut ,gint height, gint width, gint wrap ,
                                    gint digits ,gint numeric  , gint arrondis,color_widg *bgcolor)
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
    Sb->value =val_defaut;
    Sb->height = height;
    Sb->width =width ;
    Sb->wrap = wrap;
    Sb->digits = digits;
    Sb->numeric = numeric;
    Sb->arrondissement =arrondis;
    //Sb->bgcolor->color =NULL;
    //color
    if(bgcolor)
    {
        Sb->bgcolor= (color_widg*)malloc(sizeof (color_widg));
        Sb->bgcolor->color = (gchar*)malloc(30*sizeof(gchar));
        strcpy(Sb->bgcolor->color , bgcolor->color);
        Sb->bgcolor->opacity= bgcolor->opacity ;
    }



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
        if( (Sb->height >0)&&(Sb->width > 0) )

            gtk_widget_set_size_request(GTK_WIDGET(Sb->SpinButton),Sb->width ,Sb->height);
        else
            printf("Height & Width should be positivs\n");

        //digits
        if(Sb->digits >=0 )
            gtk_spin_button_set_digits(GTK_SPIN_BUTTON(Sb->SpinButton) , Sb->digits);
        else
            printf("\nDigits should be positive\n");

        //Wrap
        if(Sb->wrap == 1)
            gtk_spin_button_set_wrap (GTK_SPIN_BUTTON(Sb->SpinButton) , TRUE);

        //Numeric
        if(!(Sb->numeric))
            gtk_spin_button_set_numeric(GTK_SPIN_BUTTON(Sb->SpinButton) , FALSE);

        //Arrondissement
        if(Sb->arrondissement == 1)
            gtk_spin_button_set_snap_to_ticks(GTK_SPIN_BUTTON(Sb->SpinButton) , TRUE);

        //color
        add_bgcolor(GTK_WIDGET(Sb->SpinButton) ,Sb->bgcolor->color,Sb->bgcolor->opacity);

    }

    else
    {
        printf("\nIntervalle du SpinButton n'est pas correct\n");
        exit(0);
    }


}
