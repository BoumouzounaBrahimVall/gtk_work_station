#include <gtk/gtk.h>


typedef struct ls
{
    GtkWidget *im;
    gint x,y,vitesse;
    struct ls*next;
    struct ls*prev;
}listObj;

typedef struct {
    GtkWidget *fix;

   listObj*elem;
    gint j;
    int count;
    gint i;
    guint t;
}mov;
//


listObj *creer(gint x,gint y,gint vitesse)
{
    listObj *L=(listObj*)(malloc(sizeof (listObj)));
    L->x=x;
    L->y=y;
    L->vitesse=vitesse;
    L->next=NULL;
    L->prev=NULL;
    return (listObj*)L;
}
listObj *inseretDeb(listObj*L,gint x,gint y,gint vitesse)
{
    listObj *e= creer(x,y,vitesse);
    if(!L)
    {

        return (listObj*)e;
    }
    L->prev=e;
    e->next=L;
    L=e;
    return L;
}
/*
 * this is the fuction that moves the images (objects)
 * you can make your own rules here
*/
void pos(gint *x,gint *y,gint z)
{
    //the step
   gint a=g_random_int_range(1,5);
   printf("\na=%d\n",a);
   //srandom(time(NULL));
    switch (a) {
        case 1:(*x)=(*x)+z;break;
        case 2: (*x)=(*x)-z;break;
        case 3:(*y)=(*y)-z;break;
        default:(*y)=(*y)+z;break;
    }
    //bountries control
    if((*x)<=z) (*x)=(*x)+z;
    if((*y)<=z) (*y)=(*y)+z;
    if((*x)>=680)(*x)=(*x)-z;
    if((*y)>=680)(*y)=(*y)-z;
    /// IMPORTANTE :: safety control
    if((*x)>700) (*x)=680;
   if((*y)>700) (*y)=680;
   //tracing coordinates generated
    printf("\nz.x=%dz.y=%d\n",*x,*y);

}
char*photo(int i)//taille des photo 24x24
{
    switch (i) {
        case 1:return "/Users/brahimvall/Desktop/people/1.png";
        case 2:return "/Users/brahimvall/Desktop/people/2.png";
        case 3:return "/Users/brahimvall/Desktop/people/3.png";
        case 4:return "/Users/brahimvall/Desktop/people/4.png";
        case 5:return "/Users/brahimvall/Desktop/people/5.png";
        default:return "/Users/brahimvall/Desktop/people/6.png";
    }
}
mov* ajouter_element2(mov*fixe){

    fixe->elem= inseretDeb(fixe->elem,g_random_int_range(0,600),g_random_int_range(0,600), g_random_int_range(3,11));
    GtkWidget *im=gtk_image_new_from_file(photo(g_random_int_range(1,7)));
    fixe->elem->im=im;
    //fixe->elem= g_list_insert(fixe->elem,(gpointer *)im,a);
    gtk_fixed_put(GTK_FIXED(fixe->fix),GTK_WIDGET(fixe->elem->im),fixe->elem->x,fixe->elem->y);
    gtk_widget_show(fixe->elem->im);
    return fixe;
}
gint length(listObj*L)
{
    listObj *q=L;
    gint i=0;
    while (q)
    {
      i++;q=q->next;
    }
    return i;
}
gint deplacer(mov*fixe )
{

    //update the counter
    fixe->count++;
    if(fixe->j)
    {
        printf("i'm here");
        fixe=ajouter_element2(fixe);
        fixe->j=0;
    }
    //move the first images to their new positions
    listObj *L=fixe->elem;
    while (L)
    {
        pos(&(L->x),&( L->y),L->vitesse);
        gtk_fixed_move(GTK_FIXED(fixe->fix),GTK_WIDGET(L->im),L->x ,L->y);
        L=L->next;
    }
    //if counter is less than 10000ms return true
    return (fixe->count<10000);
}

//when the user clicks the update button
// this fuction is called to start the simulation
gboolean on_update (GtkWidget * but, gpointer *data)
{
    mov*fixe=(mov*)data;

    if(fixe->i==1)//if i=1 then call deplacer() to move the objects
    {
        fixe->i=0;
        /*call deplacer every 10ms to move the objects,
         * note that while t=true deplacer() will be called every 10ms*/
        fixe->t=g_timeout_add(70, G_SOURCE_FUNC(deplacer), fixe);

    }
    else if(fixe->i==0)//if i=0 stop g_timeout_add()
    {
        fixe->i=1;//update i
        //this will give t the false value and stop g_timeout_add()
        g_source_remove(fixe->t);
    }
    return TRUE;
}
//used to initialize and alocate memory for a mov struct
mov*initmov()
{
    mov*fix=(mov*) malloc(sizeof (mov));
    //j is used to add images
    fix->j=0;
    //play and pause
    fix->i=1;
    //g_source tag
    fix->t=0;
    //element list
    fix->elem=NULL;
    //initialiese the counter
    fix->count=0;
    return fix;
}
//add by incrementing j
gboolean incrim(GtkWidget * but, gpointer *data){
    mov*fixe=(mov*)data;
    fixe->j++;
    return TRUE;
}

int
main (int   argc,char *argv[])
{

    GtkWidget *update_button,*window,*box,*btn;
    mov*fixe;
    gtk_init (&argc, &argv);
    //create a window
    window= gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_widget_set_size_request(window,700,750);
    gtk_window_set_resizable(GTK_WINDOW(window),0);
    //create a box and an update button
    box= gtk_box_new(GTK_ORIENTATION_VERTICAL,10);
    update_button=gtk_button_new_with_label("update");
    btn=gtk_button_new_with_label("add");
    //add the button to the box
    gtk_box_pack_start(GTK_BOX(box),update_button,0,0,0);
    gtk_box_pack_start(GTK_BOX(box),btn,0,0,0);
    //create a mov structure
    //fixe have one fixed widget, 2 images widget and 2 coordinates tables
    fixe=initmov();
    fixe->fix= gtk_fixed_new();
    gtk_widget_set_size_request(fixe->fix,700,700);

    //add the fixed widget to the box
    gtk_box_pack_start(GTK_BOX(box),fixe->fix,0,0,0);

    // signal to start simulation when update button is clicked
    g_signal_connect(update_button, "clicked", G_CALLBACK(on_update),(gpointer *) fixe);
    printf("holla");
    g_signal_connect(btn, "clicked", G_CALLBACK(incrim),(gpointer *) fixe);
    //signal destroy (ma3rof hada hhh)
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit),NULL);
    // add the box to the window
    gtk_container_add(GTK_CONTAINER(window),box);
    //show all
    gtk_widget_show_all (window);
    gtk_main ();
    return 0;
}
