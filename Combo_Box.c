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
 * structure d'un widget fils
 */
typedef struct
{
    GtkWidget *elem;       //l'element lui meme
    gchar *titre;         //titre du widget
    GtkWidget *conteneur;//le pere conteneur du widget
}widg;

void test_allocation(void *pointeur, char*nom)
{
    if(!pointeur) {
        printf("erreur d'allocation du %s\n", nom);
        exit(-1);
    }
}

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
