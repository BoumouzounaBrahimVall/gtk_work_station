GtkWidget*fix, *btn,*window;
  //create a window
    window= gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_widget_set_size_request(window,700,750);
    gtk_window_set_resizable(GTK_WINDOW(window),0);
  int x=100,y=200;
  //creation d'un fixed
  fix= gtk_fixed_new();
  //ajouter un widget dans une position (100,200)
   gtk_fixed_put(GTK_FIXED(fix),btn,x,y);
   //deplacer un element vers une nouvelle position (40,10)
  gtk_fixed_move(GTK_FIXED(fix),btn,40,10);
   gtk_container_add(GTK_CONTAINER(window),fix);
