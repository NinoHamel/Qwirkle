#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_ttf.h>
#include <time.h>
#include <math.h>
#include "qwirkle.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*#include "boutons.h"
#include "ia.h"
#include "jeu.h"*/

//Mode DEBOGAGE
#define DEBOGAGE


ALLEGRO_DISPLAY* init_allegro(void);
ALLEGRO_BITMAP*     recup_sprite(ALLEGRO_BITMAP*scr,
                             int tx, int ty,
                             int startx, int starty,
                             int colonne, int i);
void             erreur         (const char*txt);
/*****************************************************************
*****************************************************************/

int main(){

    display = init_allegro();

    al_init_font_addon();
        if (!al_init_ttf_addon())
        erreur("al_init_ttf_addon()");

    TousLesFichiers();

    // FILE D'EVENEMENTS
    ALLEGRO_EVENT_QUEUE*queue;
    queue = al_create_event_queue();
    if (!queue)
        erreur("al_create_event_queue()");

    // SELECTION DES EVENEMENTS à prendre dans la file
    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_mouse_event_source());

    int position;

    position = Menu1();

    while(position != 0){

        switch(position){
            case 1:
                position = Menu1(queue);
            break;

            case 2:
                position = Menu2(queue);
            break;

            case 3:
                position = Jeu(queue);
            break;

            case 4:
                position = Options(queue);
            break;

            case 5:
                position = Save(queue);
            break;

            case 6:
                position = Regles(queue);
            break;
            case 7:
                position = Scores(queue);
            break;
            }
        }

    //LIBERER LES RESSOURCES UTILISEES
    al_destroy_bitmap(decor);
    al_destroy_display(display);
    al_destroy_event_queue(queue);

    return 0;
}

int Scores(queue){

    //effacer les images en arriere plan
    al_clear_to_color(al_map_rgb(255, 255, 255));
    // affichage de sauvegarde.png
    al_draw_bitmap(all_Scores, 0, 0, 0);

        //recuperation fichier ttf
    all_comicsansms = al_load_font("img/comicsansms.ttf",35,0);
    if(!all_comicsansms)
        erreur("al_load_font()");

    //affiche les scores des joueurs selon leur couleurs respectives
    al_draw_textf(all_comicsansms, ROUGE, 260, 265, 0, "Joueur 1 : %d", scorej1);
    al_draw_textf(all_comicsansms, BLEU, 260, 336, 0, "Joueur 2 : %d", scorej2);
    if(NbJoueur + NbIA > 2)
        al_draw_textf(all_comicsansms, VERT, 260, 407, 0, "Joueur 3 : %d", scorej3);
    if(NbJoueur + NbIA > 3)
        al_draw_textf(all_comicsansms, JAUNE, 260, 478, 0, "Joueur 4 : %d", scorej4);

    al_destroy_font(all_comicsansms);

    //affiche le gagnant
    if(scorej1 >= scorej2 && scorej1 >= scorej3 && scorej1 >= scorej4){
        all_comicsansms = al_load_font("img/comicsansms.ttf",45,0);
        if(!all_comicsansms)
            erreur("al_load_font()");
        al_draw_textf(all_comicsansms, ROUGE, 589, 513, 0, "joueur 1 !");
    }

    if(scorej2 >= scorej1 && scorej2 >= scorej3 && scorej2 >= scorej4){
        all_comicsansms = al_load_font("img/comicsansms.ttf",45,0);
        if(!all_comicsansms)
            erreur("al_load_font()");

        al_draw_textf(all_comicsansms, BLEU, 600, 513, 0, "joueur 2 !");
    }

    if(scorej3 >= scorej2 && scorej3 >= scorej1 && scorej3 >= scorej4){
        all_comicsansms = al_load_font("img/comicsansms.ttf",45,0);
        if(!all_comicsansms)
            erreur("al_load_font()");

        al_draw_textf(all_comicsansms, VERT, 600, 513, 0, "joueur 3 !");
    }

    if(scorej4 >= scorej2 && scorej4 >= scorej3 && scorej4 >= scorej1){
        all_comicsansms = al_load_font("img/comicsansms.ttf",45,0);
        if(!all_comicsansms)
            erreur("al_load_font()");

        al_draw_textf(all_comicsansms, JAUNE, 600, 513, 0, "joueur 4 !");
    }
    al_destroy_font(all_comicsansms);
    al_flip_display();

    int fin = 0;
    while (!fin){

        // RECUPERATION DES EVENEMENTS
        ALLEGRO_EVENT event = { 0 };
        al_wait_for_event(queue, &event);

        switch (event.type){
                // EVENEMENTS SOURIS
            case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:

                //retour au menu 1
                if (event.mouse.x>=QUITMENUX && event.mouse.x <= QUITMENUX + QUITMENUL && event.mouse.y >= QUITMENUY && event.mouse.y <= QUITMENUY + QUITMENUH){
                    return 1;
                }
            break;

            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                return 0;
            break;
            }
        }
}

void TousLesFichiers(){

    //fichier IHM
    all_ihm = al_load_bitmap("img/ihm.png");
    if (!all_ihm)
        erreur("al_load_bitmap(\"IHM.png\")");
    // récupération du fichier MENU2
    all_Menu2 = al_load_bitmap("img/Menu2.png");
    if (!all_Menu2)
        erreur("al_load_bitmap(\"Menu2.png\")");
    // récupération des fichiers select et deselect
    all_select = al_load_bitmap("img/select.png");
    if (!all_select)
        erreur("al_load_bitmap(\"select.png\")");
    all_deselect = al_load_bitmap("img/deselect.png");
    if (!all_deselect)
        erreur("al_load_bitmap(\"deselect.png\")");

        all_select32 = al_load_bitmap("img/select32.png");
    if (!all_select32)
        erreur("al_load_bitmap(\"select32.png\")");
    all_deselect32 = al_load_bitmap("img/deselect32.png");
    if (!all_deselect32)
        erreur("al_load_bitmap(\"deselect32.png\")");

        //fichier sauvegarde et option.png
    all_Option = al_load_bitmap("img/Option.png");
    if (!all_Option)
        erreur("al_load_bitmap(\"Option.png\")");
    all_Sauvegarde = al_load_bitmap("img/Sauvegarde.png");
    if (!all_Sauvegarde)
        erreur("al_load_bitmap(\"Sauvegarde.png\")");

    //decor est dans le répertoire du programme
    decor = al_create_bitmap(DECORX, DECORY);
    if (!decor)
        erreur("al_create_bitmap()");

    //"pion vide" --> quand y'a plus rien dans la pioche
    all_vide = al_load_bitmap("img/vide.png");

    // récupération du fichier MENU1
    all_Menu1 = al_load_bitmap("img/Menu1.png");
    if (!all_Menu1)
        erreur("al_load_bitmap(\"Menu1.png\")");
    // récupération du fichier de tuiles
    all_tuiles = al_load_bitmap("img/kit_de_tuiles.png");
    if (!all_tuiles)
        erreur("al_load_bitmap(\"kit_de_tuiles.png\")");
    all_Regles = al_load_bitmap("img/Regles.png");
    if(!all_Regles)
        erreur("al_load_bitmap(\"Regles.png\")");

    all_interro = al_load_bitmap("img/PointInterro.png");
    if(!all_interro)
        erreur("al_load_bitmap(\"interro.png\")");
    all_Scores = al_load_bitmap("img/Scores.png");
    if(!all_Scores)
        erreur("al_load_bitmap(\"scores.png\")");

}

int Options(queue){

    //effacer les images en arriere plan
    al_clear_to_color(al_map_rgb(255, 255, 255));
    // affichage de sauvegarde.png
    al_draw_bitmap(all_Option, 0, 0, 0);
    al_flip_display();

    int fin = 0;
    while (!fin){

        // RECUPERATION DES EVENEMENTS
        ALLEGRO_EVENT event = { 0 };
        al_wait_for_event(queue, &event);

        switch (event.type){
                // EVENEMENTS SOURIS
            case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:

                //retour au menu 1
                if (event.mouse.x>=QUITMENUX && event.mouse.x <= QUITMENUX + QUITMENUL && event.mouse.y >= QUITMENUY && event.mouse.y <= QUITMENUY + QUITMENUH){
                    return 1;
                }
            break;

            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                return 0;
            break;
            }
        }
}

int Save(queue){

    //effacer les images en arriere plan
    al_clear_to_color(al_map_rgb(255, 255, 255));
    // affichage de sauvegarde.png
    al_draw_bitmap(all_Sauvegarde, 0, 0, 0);
    al_flip_display();

    int fin = 0;
    while (!fin){

        // RECUPERATION DES EVENEMENTS
        ALLEGRO_EVENT event = { 0 };
        al_wait_for_event(queue, &event);

        switch (event.type){
                // EVENEMENTS SOURIS
            case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:

                //retour au menu 1
                if (event.mouse.x>=QUITMENUX && event.mouse.x <= QUITMENUX + QUITMENUL && event.mouse.y >= QUITMENUY && event.mouse.y <= QUITMENUY + QUITMENUH){
                    return 1;
                }
            break;

            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                return 0;
            break;
            }
        }
}

int Regles(queue){

    // affichage du menu
    al_draw_bitmap(all_Regles, 0, 0, 0);
    al_flip_display();

    int fin = 0;
    while (!fin){

        // RECUPERATION DES EVENEMENTS
        ALLEGRO_EVENT event = { 0 };
        al_wait_for_event(queue, &event);

        switch (event.type){
                // EVENEMENTS SOURIS
            case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
                if (event.mouse.x>=LREGLEX && event.mouse.x <= LREGLEX + LREGLEL && event.mouse.y >= LREGLEY && event.mouse.y <= LREGLEY + LREGLEH){
                    return 1;
                }
            break;

            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                return 0;
            break;
            }
        }
}

int Menu2(queue){

    al_clear_to_color(al_map_rgb(255, 255, 255));
    // affichage du menu
    al_draw_bitmap(all_Menu2, 0, 0, 0);

    al_draw_bitmap(all_select, NBJOUEURSX + NBJOUEURSL/4, NBJOUEURSY, 0);

    al_draw_bitmap(all_select, NBIAX, NBIAY, 0);

    al_draw_bitmap(all_select, NVDIFFX, NVDIFFY, 0);

    al_flip_display();
    NbJoueur = 2;
    NbIA = 0;

    //Mode DEBOGAGE
    #ifdef DEBOGAGE
    // CONSOLE NATIVE ALLEGRO pour affichage des tests
    ALLEGRO_TEXTLOG*textlog = NULL;
    textlog = al_open_native_text_log("Evénements souris, clavier", ALLEGRO_TEXTLOG_MONOSPACE);
    #endif // DEBOGAGE

    // BOUCLE EVENEMENTS
    int fin = 0;
    while (!fin){

        // RECUPERATION DES EVENEMENTS
        ALLEGRO_EVENT event = { 0 };
        al_wait_for_event(queue, &event);

        // SELON EVENEMENT TROUVE
        switch (event.type){
            case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN: {
                mousepressed = TRUE;

                // Clique dans la zone NbJoueurs
                if (event.mouse.x >= COMMENCERPARTIEX && event.mouse.x <= COMMENCERPARTIEX + COMMENCERPARTIEL && event.mouse.y >= COMMENCERPARTIEY && event.mouse.y <= COMMENCERPARTIEY + COMMENCERPARTIEH)
                    return 3;

                if (event.mouse.x >= RETOURX && event.mouse.x <= RETOURX + RETOURL && event.mouse.y >= RETOURY && event.mouse.y <= RETOURY + RETOURH)
                    return 1;

                // Clique dans la zone NbJoueurs
                if (event.mouse.x >= NBJOUEURSX && event.mouse.x <= NBJOUEURSX + NBJOUEURSL && event.mouse.y >= NBJOUEURSY && event.mouse.y <= NBJOUEURSY + NBJOUEURSH) {
                    //Mode DEBOGAGE
                    #ifdef DEBOGAGE
                    al_append_native_text_log(textlog,"Zone NBRJOUEUR\n");
                    #endif // DEBOGAGE
                    if (event.mouse.x >= NBJOUEURSX && event.mouse.x <= NBJOUEURSX + NBJOUEURSL/4 && event.mouse.y >= NBJOUEURSY && event.mouse.y <= NBJOUEURSY + NBJOUEURSH) {
                        al_draw_bitmap(all_deselect, NBJOUEURSX + NBJOUEURSL/4, NBJOUEURSY, 0);
                        al_draw_bitmap(all_deselect, NBJOUEURSX + 2*NBJOUEURSL/4 -1, NBJOUEURSY, 0);
                        al_draw_bitmap(all_deselect, NBJOUEURSX + 3*NBJOUEURSL/4 -1, NBJOUEURSY, 0);
                        al_draw_bitmap(all_select, NBJOUEURSX, NBJOUEURSY, 0);
                        NbJoueur = 1;
                    }
                    if (event.mouse.x >= NBJOUEURSX + NBJOUEURSL/4 && event.mouse.x <= NBJOUEURSX + 2*NBJOUEURSL/4 && event.mouse.y >= NBJOUEURSY && event.mouse.y <= NBJOUEURSY + NBJOUEURSH) {
                        al_draw_bitmap(all_deselect, NBJOUEURSX, NBJOUEURSY, 0);
                        al_draw_bitmap(all_deselect, NBJOUEURSX + 2*NBJOUEURSL/4 -1, NBJOUEURSY, 0);
                        al_draw_bitmap(all_deselect, NBJOUEURSX + 3*NBJOUEURSL/4 -1, NBJOUEURSY, 0);
                        al_draw_bitmap(all_select, NBJOUEURSX + NBJOUEURSL/4, NBJOUEURSY, 0);
                        NbJoueur = 2;
                    }
                    if (event.mouse.x >= NBJOUEURSX + 2*NBJOUEURSL/4 && event.mouse.x <= NBJOUEURSX + 3*NBJOUEURSL/4 && event.mouse.y >= NBJOUEURSY && event.mouse.y <= NBJOUEURSY + NBJOUEURSH) {
                        al_draw_bitmap(all_deselect, NBJOUEURSX, NBJOUEURSY, 0);
                        al_draw_bitmap(all_deselect, NBJOUEURSX + NBJOUEURSL/4, NBJOUEURSY, 0);
                        al_draw_bitmap(all_deselect, NBJOUEURSX + 3*NBJOUEURSL/4 -1, NBJOUEURSY, 0);
                        al_draw_bitmap(all_select, NBJOUEURSX + 2*NBJOUEURSL/4 -1, NBJOUEURSY, 0);
                        NbJoueur = 3;
                    }
                    if (event.mouse.x >= NBJOUEURSX + 3*NBJOUEURSL/4 && event.mouse.x <= NBJOUEURSX + NBJOUEURSL && event.mouse.y >= NBJOUEURSY && event.mouse.y <= NBJOUEURSY + NBJOUEURSH) {
                        al_draw_bitmap(all_deselect, NBJOUEURSX, NBJOUEURSY, 0);
                        al_draw_bitmap(all_deselect, NBJOUEURSX + NBJOUEURSL/4, NBJOUEURSY, 0);
                        al_draw_bitmap(all_deselect, NBJOUEURSX  + 2*NBJOUEURSL/4 -1, NBJOUEURSY, 0);
                        al_draw_bitmap(all_select, NBJOUEURSX + 3*NBJOUEURSL/4 -1, NBJOUEURSY, 0);
                        NbJoueur = 4;
                    }
                    //selection correcte du nombre d'IA/joueurs
                    if(NbIA == 0 && NbJoueur == 1 || NbJoueur == 3 && NbJoueur + NbIA > 4){
                        al_draw_bitmap(all_deselect, NBIAX, NBIAY, 0);
                        al_draw_bitmap(all_deselect, NBIAX + 2*NBIAL/4 -1, NBIAY, 0);
                        al_draw_bitmap(all_deselect, NBIAX + 3*NBIAL/4 -1, NBIAY, 0);
                        al_draw_bitmap(all_select, NBIAX + NBIAL/4, NBIAY, 0);
                        NbIA = 1;
                    }
                    if(NbJoueur == 4 && NbIA != 0){
                        al_draw_bitmap(all_deselect, NBIAX + NBIAL/4, NBIAY, 0);
                        al_draw_bitmap(all_deselect, NBIAX + 2*NBIAL/4 -1, NBIAY, 0);
                        al_draw_bitmap(all_deselect, NBIAX + 3*NBIAL/4 -1, NBIAY, 0);
                        al_draw_bitmap(all_select, NBIAX, NBIAY, 0);
                        NbIA = 0;
                    }
                    if(NbJoueur == 2 && NbJoueur + NbIA > 4){
                        al_draw_bitmap(all_deselect, NBIAX, NBIAY, 0);
                        al_draw_bitmap(all_deselect, NBIAX + NBIAL/4, NBIAY, 0);
                        al_draw_bitmap(all_deselect, NBIAX + 3*NBIAL/4 -1, NBIAY, 0);
                        al_draw_bitmap(all_select, NBIAX + 2*NBIAL/4 -1, NBIAY, 0);
                        NbIA = 2;
                    }

                }
                // Clique dans la zone NbIA
                if (event.mouse.x >= NBIAX && event.mouse.x <= NBIAX + NBIAL && event.mouse.y >= NBIAY && event.mouse.y <= NBIAY + NBIAH) {
                    //Mode DEBOGAGE
                    #ifdef DEBOGAGE
                    al_append_native_text_log(textlog,"Zone NbIA\n");
                    #endif // DEBOGAGE
                    if (event.mouse.x >= NBIAX && event.mouse.x <= NBIAX + NBIAL/4 && event.mouse.y >= NBIAY && event.mouse.y <= NBIAY + NBIAH) {
                        al_draw_bitmap(all_deselect, NBIAX + NBIAL/4, NBIAY, 0);
                        al_draw_bitmap(all_deselect, NBIAX + 2*NBIAL/4 -1, NBIAY, 0);
                        al_draw_bitmap(all_deselect, NBIAX + 3*NBIAL/4 -1, NBIAY, 0);
                        al_draw_bitmap(all_select, NBIAX, NBIAY, 0);
                        NbIA = 0;
                    }
                    if (event.mouse.x >= NBIAX + NBIAL/4 && event.mouse.x <= NBIAX + 2*NBIAL/4 && event.mouse.y >= NBIAY && event.mouse.y <= NBIAY + NBIAH) {
                        al_draw_bitmap(all_deselect, NBIAX, NBIAY, 0);
                        al_draw_bitmap(all_deselect, NBIAX + 2*NBIAL/4 -1, NBIAY, 0);
                        al_draw_bitmap(all_deselect, NBIAX + 3*NBIAL/4 -1, NBIAY, 0);
                        al_draw_bitmap(all_select, NBIAX + NBIAL/4, NBIAY, 0);
                        NbIA = 1;
                    }
                    if (event.mouse.x >= NBIAX + 2*NBIAL/4 && event.mouse.x <= NBIAX + 3*NBIAL/4 && event.mouse.y >= NBIAY && event.mouse.y <= NBIAY + NBIAH) {
                        al_draw_bitmap(all_deselect, NBIAX, NBIAY, 0);
                        al_draw_bitmap(all_deselect, NBIAX + NBIAL/4, NBIAY, 0);
                        al_draw_bitmap(all_deselect, NBIAX + 3*NBIAL/4 -1, NBIAY, 0);
                        al_draw_bitmap(all_select, NBIAX + 2*NBIAL/4 -1, NBIAY, 0);
                        NbIA = 2;
                    }
                    if (event.mouse.x >= NBIAX + 3*NBIAL/4 && event.mouse.x <= NBIAX + NBIAL && event.mouse.y >= NBIAY && event.mouse.y <= NBIAY + NBIAH) {
                        al_draw_bitmap(all_deselect, NBIAX, NBIAY, 0);
                        al_draw_bitmap(all_deselect, NBIAX + NBIAL/4, NBIAY, 0);
                        al_draw_bitmap(all_deselect, NBIAX  + 2*NBIAL/4 -1, NBIAY, 0);
                        al_draw_bitmap(all_select, NBIAX + 3*NBIAL/4 -1, NBIAY, 0);
                        NbIA = 3;
                    }
                    //selection correcte du nombre d'IA/joueurs
                    if(NbIA == 0 || NbIA == 2 && NbJoueur + NbIA > 4){
                        al_draw_bitmap(all_deselect, NBJOUEURSX, NBJOUEURSY, 0);
                        al_draw_bitmap(all_deselect, NBJOUEURSX + 2*NBJOUEURSL/4 -1, NBJOUEURSY, 0);
                        al_draw_bitmap(all_deselect, NBJOUEURSX + 3*NBJOUEURSL/4 -1, NBJOUEURSY, 0);
                        al_draw_bitmap(all_select, NBJOUEURSX + NBJOUEURSL/4, NBJOUEURSY, 0);
                        NbJoueur = 2;
                    }

                    if(NbIA == 3 && NbJoueur != 1){
                        al_draw_bitmap(all_deselect, NBJOUEURSX + NBJOUEURSL/4, NBJOUEURSY, 0);
                        al_draw_bitmap(all_deselect, NBJOUEURSX + 2*NBJOUEURSL/4 -1, NBJOUEURSY, 0);
                        al_draw_bitmap(all_deselect, NBJOUEURSX + 3*NBJOUEURSL/4 -1, NBJOUEURSY, 0);
                        al_draw_bitmap(all_select, NBJOUEURSX, NBJOUEURSY, 0);
                        NbJoueur = 1;
                    }

                    if(NbIA == 1 && NbJoueur + NbIA > 4){
                        al_draw_bitmap(all_deselect, NBJOUEURSX, NBJOUEURSY, 0);
                        al_draw_bitmap(all_deselect, NBJOUEURSX + NBJOUEURSL/4, NBJOUEURSY, 0);
                        al_draw_bitmap(all_deselect, NBJOUEURSX + 3*NBJOUEURSL/4 -1, NBJOUEURSY, 0);
                        al_draw_bitmap(all_select, NBJOUEURSX + 2*NBJOUEURSL/4 -1, NBJOUEURSY, 0);
                        NbJoueur = 3;
                    }

                }
                // Clique dans la zone NvDiff
                if (event.mouse.x >= NVDIFFX && event.mouse.x <= NVDIFFX + NVDIFFL && event.mouse.y >= NVDIFFY && event.mouse.y <= NVDIFFY + NVDIFFH) {
                    //Mode DEBOGAGE
                    #ifdef DEBOGAGE
                    al_append_native_text_log(textlog,"Zone NvDiff\n");
                    #endif // DEBOGAGE
                    if (event.mouse.x >= NVDIFFX && event.mouse.x <= NVDIFFX + NVDIFFL/3 && event.mouse.y >= NVDIFFY && event.mouse.y <= NVDIFFY + NVDIFFH) {
                        al_draw_bitmap(all_deselect, NVDIFFX + NVDIFFL/3, NVDIFFY, 0);
                        al_draw_bitmap(all_deselect, NVDIFFX + 2*NVDIFFL/3 -1, NVDIFFY, 0);
                        al_draw_bitmap(all_select, NVDIFFX, NVDIFFY, 0);
                        Difficulte = 1;
                    }
                    if (event.mouse.x >= NVDIFFX + NVDIFFL/3 && event.mouse.x <= NVDIFFX + 2*NVDIFFL/3 && event.mouse.y >= NVDIFFY && event.mouse.y <= NVDIFFY + NVDIFFH) {
                        al_draw_bitmap(all_deselect, NVDIFFX, NVDIFFY, 0);
                        al_draw_bitmap(all_deselect, NVDIFFX + 2*NVDIFFL/3 -1, NVDIFFY, 0);
                        al_draw_bitmap(all_select, NVDIFFX + NVDIFFL/3, NVDIFFY, 0);
                        Difficulte = 2;
                    }
                    if (event.mouse.x >= NVDIFFX + 2*NVDIFFL/3 && event.mouse.x <= NVDIFFX + NVDIFFL && event.mouse.y >= NVDIFFY && event.mouse.y <= NVDIFFY + NVDIFFH) {
                        al_draw_bitmap(all_deselect, NVDIFFX, NVDIFFY, 0);
                        al_draw_bitmap(all_deselect, NVDIFFX + NVDIFFL/3, NVDIFFY, 0);
                        al_draw_bitmap(all_select, NVDIFFX + 2*NVDIFFL/3 -1, NVDIFFY, 0);
                        Difficulte = 3;
                    }
                }
            }
                al_flip_display();
            break;

                //EVENEMENTS FENETRE
            case ALLEGRO_EVENT_DISPLAY_CLOSE: {
                fin = TRUE;
                }
            break;
        }
    }
    return 0;
}

int Menu1(queue){

    al_clear_to_color(al_map_rgb(255, 255, 255));

    // affichage du menu
    al_draw_bitmap(all_Menu1, 0, 0, 0);
    al_flip_display();


    //Mode DEBOGAGE
    #ifdef DEBOGAGE
    // CONSOLE NATIVE ALLEGRO pour affichage des tests
    ALLEGRO_TEXTLOG*textlog = NULL;
    textlog = al_open_native_text_log("Evénements souris, clavier", ALLEGRO_TEXTLOG_MONOSPACE);
    #endif // DEBOGAGE


    int fin = 0,regle = 0;
    while (!fin){

             // RECUPERATION DES EVENEMENTS
        ALLEGRO_EVENT event = { 0 };
        al_wait_for_event(queue, &event);


        switch (event.type){
                // EVENEMENTS SOURIS
            case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN: {
                mousepressed = TRUE;
                if (event.mouse.x >= JOUERX && event.mouse.x <= JOUERX + JOUERL && event.mouse.y >= JOUERY && event.mouse.y <= JOUERY + JOUERH) {
                //Mode DEBOGAGE
                #ifdef DEBOGAGE
                al_append_native_text_log(textlog,"Zone JOUER menu1\n");
                #endif // DEBOGAGE

                return 2;
                }

                if(event.mouse.x >= QUITMENUX && event.mouse.x <= QUITMENUX + QUITMENUL && event.mouse.y >= QUITMENUY && event.mouse.y <= QUITMENUY + QUITMENUH){
                fin = TRUE;
                al_flip_display();
                }

                if (event.mouse.x>=REGLEX && event.mouse.x <= REGLEX + REGLEL && event.mouse.y >= REGLEY && event.mouse.y <= REGLEY + REGLEH && regle == 0 ){
                // Mode DEBOGAGE
                #ifdef DEBOGAGE
                al_append_native_text_log(textlog,"Zone REGLES QUITTER\n");
                #endif // DEBOGAGE
                al_draw_bitmap(all_Regles, 0, 0, 0);
                al_flip_display();
                regle = 1;

                return 6;
                }


                if (event.mouse.x >= SAVEX && event.mouse.x <= SAVEX + SAVEL && event.mouse.y >= SAVEY && event.mouse.y <= SAVEY + SAVEH ){
                // Mode DEBOGAGE
                #ifdef DEBOGAGE
                al_append_native_text_log(textlog,"Zone SAUVEGARDE\n");
                #endif // DEBOGAGE
                return 5;
                }
                if (event.mouse.x >= OPTIONX && event.mouse.x <= OPTIONX + OPTIONL && event.mouse.y >= OPTIONY && event.mouse.y <= OPTIONY + OPTIONH ){
                // Mode DEBOGAGE
                #ifdef DEBOGAGE
                al_append_native_text_log(textlog,"Zone OPTIONS\n");
                #endif // DEBOGAGE
                return 4;
                }
            }
            break;

            case ALLEGRO_EVENT_DISPLAY_CLOSE: {
                fin = TRUE;
                }
            break;
        }
    }
    return 0;
}

int Jeu(queue){

    al_clear_to_color(al_map_rgb(255, 255, 255));
    // affichage de l'ihm
    al_draw_bitmap(all_ihm, 0, 0, 0);

    //affichage Carrés rouges : infos de la partie
    if(NbJoueur == 1){
        al_draw_bitmap(all_select, NBJOUEURS2X, NBJOUEURS2Y, 0);
    }
    if(NbJoueur == 2){
        al_draw_bitmap(all_select, NBJOUEURS2X + NBJOUEURS2L/4, NBJOUEURS2Y, 0);
    }
    if(NbJoueur == 3){
        al_draw_bitmap(all_select, NBJOUEURS2X + 2*NBJOUEURS2L/4 - 1, NBJOUEURS2Y, 0);
    }
    if(NbJoueur == 4){
        al_draw_bitmap(all_select, NBJOUEURS2X + 3*NBJOUEURS2L/4 - 1, NBJOUEURS2Y, 0);
    }

    if(NbIA == 0){
        al_draw_bitmap(all_select, NBIA2X, NBIA2Y, 0);
    }
    if(NbIA == 1){
        al_draw_bitmap(all_select, NBIA2X + NBIA2L/4, NBIA2Y, 0);
    }
    if(NbIA == 2){
        al_draw_bitmap(all_select, NBIA2X + 2*NBIA2L/4 -1 , NBIA2Y, 0);
    }
    if(NbIA == 3){
        al_draw_bitmap(all_select, NBIA2X + 3*NBIA2L/4 -1, NBIA2Y, 0);
    }

    if(Difficulte == 1){
        al_draw_bitmap(all_select, NVDIFF2X -2, NVDIFF2Y, 0);
    }
    if(Difficulte == 2){
        al_draw_bitmap(all_select, NVDIFF2X + NVDIFF2L/3 -2, NVDIFF2Y, 0);
    }
    if(Difficulte == 3){
        al_draw_bitmap(all_select, NVDIFF2X + 2*NVDIFF2L/3 -3, NVDIFF2Y, 0);
    }

    //remplir le tableau de cases vides
    for(int i = 0 ; i<MAPY; i++){
        for(int j = 0; j< MAPX; j++){
            MAP[i][j] = 06;
        }
    }

    //création d'un décor de tuiles
    for (y = 0, posy = 0; y < DECORY; y += TY){
        for (x = 0, posx = 0; x < DECORX; x += TX){
            ALLEGRO_BITMAP*tuile = recup_sprite(
                all_tuiles,
                TX + 1, TY + 1,
                0, 0,
                NB_COL,
                MAP[posy][posx++]); // élément
            if (!tuile)
                erreur("recup_sprite()");
            al_set_target_bitmap(decor);
            al_draw_bitmap(tuile, x, y, 0);
            al_destroy_bitmap(tuile);
        }
        posy++;
    }
    // retour affichage dans la fenêtre
    al_set_target_backbuffer(display);
    // au départ l'image est calée sur le centre
    x = (MAPX - VIEWX) * 32 / 2;
    y = (MAPY - VIEWY) * 32 / 2;

    al_draw_bitmap_region(decor, x, y, GAMEL, GAMEH, GAMEX, GAMEY, 0);

    al_flip_display();

    int localisation = 3;

    //On donne les mains des joueurs
    distribution();

    //pour savoir qui commence
    commence1 = qui_commence(mainj1);
    commence2 = qui_commence(mainj2);

    if(NbJoueur + NbIA == 3)
        commence3 = qui_commence(mainj3);

    if(NbJoueur + NbIA == 4)
        commence3 = qui_commence(mainj3);
        commence4 = qui_commence(mainj4);

    if(commence1 >= commence2 && commence1 >= commence3 && commence1 >= commence4){ //joueur1 commence
        commence_final = commence1;
        printf("le joueur 1 commence");
    }

    else if(commence2 >= commence3 && commence2 >= commence4){ //joueur2 commence
        commence_final = commence2;
        printf("le joueur 2 commence\n");
        tour = 2;
        affichageMain();
    }

        else if(commence3 >= commence4){ //joueur3 commence
            commence_final = commence3;
            printf("le joueur 3 commence\n");
            tour = 3;
            affichageMain();
        }

            else{ //joueur4 commence
                commence_final = commence4;
                printf("le joueur 4 commence\n");
                tour = 4;
                affichageMain();
            }

    nbPionPose = 0;

    unPionPoseOuPas = 0;
    DeselectOuPas = 0;

    if(NbJoueur + NbIA == 2)
        localisation = BoucleJeu2Joueurs(queue);

    if(NbJoueur + NbIA == 3)
        localisation = BoucleJeu3Joueurs(queue);

    if(NbJoueur + NbIA == 4)
        localisation = BoucleJeu4Joueurs(queue);

    return localisation;

}

ALLEGRO_DISPLAY* init_allegro(){
    ALLEGRO_DISPLAY*display;
    // Initialisation d'Allegro
    if (!al_init())
        erreur("al_init()");
    // Initialisation du clavier
    if (!al_install_keyboard())
        erreur("al_install_keyboard()");
    // Initialisation de la souris
    if (!al_install_mouse())
        erreur("al_install_mouse()");
    if (!al_init_image_addon())
        erreur("al_init_image_addon()");
    // Création de la fenètre
    display = al_create_display(SCREENX, SCREENY);
    if (!display)
        erreur("al_create_display()");
    return display;
}
/*****************************************************************
Récupérer les tuiles
*****************************************************************/
ALLEGRO_BITMAP*recup_sprite(
    ALLEGRO_BITMAP*scr, // bitmap d'origine
    int tx, int ty, // taille élément
    int startx, int starty, // à partir de
    int colonne, // nombre de colonnes
    int i) // ieme élément
{
    ALLEGRO_BITMAP*sprite = NULL;
    int x, y;
    sprite = al_create_bitmap(tx, ty);
    if (sprite != NULL){
        // attention colonne doit être > 0
        x = startx + (i%colonne)*tx;
        y = starty + (i / colonne)*ty;

        al_set_target_bitmap(sprite);
        al_draw_bitmap_region(scr, x, y, tx, ty, 0, 0, 0);
    }
    return sprite;
}
/*****************************************************************
*****************************************************************/
void erreur(const char*txt)
{
    ALLEGRO_DISPLAY*d;
    d = al_is_system_installed() ? al_get_current_display() : NULL;
    al_show_native_message_box(d, "ERREUR", txt, NULL, NULL, 0);
    exit(EXIT_FAILURE);
}
/*****************************************************************
*****************************************************************/

//*********************PARTIE IA*********************\\

int flistepieces(int x, int y, int dx, int dy, int *couleur, int *forme, int *ilongueur, int *ipiece) {
    //Déclaration des varaibles locales
    int icouleurpiece, iformepiece;
    //Si la case [x,y] n'est pas vide
    if(MAP[x+dx][y+dy] != 06) {
        //Passer � la case suivante
        flistepieces(x+dx, y+dy, dx, dy, couleur, forme, ilongueur, ipiece);
        //R�cup�rer la couleur de la case test�e
        *couleur = *couleur | (int)pow(2,(int)MAP[x+dx][y+dy] / 10);
        //R�cup�rer la forme de la case
        *forme = *forme | (int)pow(2,MAP[x+dx][y+dy] % 10);
        *ilongueur += 1;
    } //Finsi
    else {
        *couleur = 0;
        *forme = 0;
        return 0;
    }
    //R�cup�rer la couleur de la case test�e
    icouleurpiece = pow(2,*ipiece / 10);
    //R�cup�rer la forme de la case
    iformepiece = pow(2,*ipiece % 10);
    if((icouleurpiece == *couleur && (iformepiece & *forme) == 0) ^ (iformepiece == *forme && (icouleurpiece & *couleur) == 0)) {
        //Retourner 0
        return 0;
    }
    //Finsi
    //Sinon
    else{
        //Retourner - 1
        return -1;
    }
}

int fiapositionvalide(int x, int y, int ipiece) {

    //Déclaration des variables locales
    int couleur, forme, ilongueur = 0;
    int ivalide = 0, iscoreia = 0, rentreIf = 0;
    //Si les coordonnées x sont dans le cadre
    if(x+1 < MAPX && x-1 >= 0) {
            //Si la case du haut n'est pas vide
        if(MAP[x-1][y] != 06 && ivalide == 0) {
            rentreIf = 1;
            //Comparer les pièces du haut avec celle à poser
            ivalide = flistepieces(x, y, -1, 0, &couleur, &forme, &ilongueur, &ipiece);
        }
        //Si la case du bas n'est pas vide
        if(MAP[x+1][y] != 06 && ivalide == 0) {
            rentreIf = 1;
            //Comparer les pièces du bas avec celle à poser
            ivalide = flistepieces(x, y, 1, 0, &couleur, &forme, &ilongueur, &ipiece);
        }
        if(ilongueur != 0) {
            iscoreia = iscoreia + ilongueur + 1;
            if(ilongueur == 5) {
                iscoreia += 6;
            }
        }
    }
    ilongueur = 0;
    //Si les coordonn�es y sont dans le cadre
    if(y+1 < MAPY && y-1 >= 0) {
        //Si la case de gauche n'est pas vide
        if(MAP[x][y-1] != 06 && ivalide == 0) {
                rentreIf = 1;
            //Comparer les pi�ces de gauche avec celle � poser
            ivalide = flistepieces(x, y, 0, -1, &couleur, &forme, &ilongueur, &ipiece);
        }
        //Si la case de droite bas n'est pas vide
        if(MAP[x][y+1] != 06 && ivalide == 0) {
            rentreIf = 1;
            //Comparer les pi�ces de droite avec celle � poser
            ivalide = flistepieces(x, y, 0, 1, &couleur, &forme, &ilongueur, &ipiece);
        }
        if(ilongueur != 0) {
            iscoreia = iscoreia + ilongueur + 1;
            if(ilongueur == 5) {
                iscoreia += 6;
            }
        }
    }
    if(ivalide != 0 || (rentreIf == 0 && nbPionPose != 0)) {
        iscoreia = -1;
    }
    return iscoreia;
}


//************** PARTIE GESTION DES TUILES *************************\\

int melange(int debut){
    int random, temp;

    srand(time(NULL));

    for(int i=debut; i<=107; i++){

        random = rand() % 108;
        temp = pioche[i];
        pioche[i] = pioche[random];
        pioche[random] = temp;

    }
}

void distribution(){

    melange(0);
    posi_pioche = 0;

    for(int i=0; i< NbIA + NbJoueur; i++){ //pour chaque joueur

        switch(i){
            case 0: //j1
                for(int j=0; j < 6; j++){ //remplir les mains de 6 cases

                        mainj1[j] = pioche[posi_pioche];  //mainj1 et pioche sont des tableaux de valeurs numériques : main contien la main du joueur et
                                                          //pioche contient la pioche (toutes les tuiles mélangées)

                        sprintf(CheminImage, "img/tuiles/%d.png", mainj1[j]);//les fichiers pnj portent les nom 00.pnj, 01.pnj ect ..

                        posi_pioche++;// on avance d'un pas dans le tableau de pioche (qui a été mélangé)

                        all_xx = al_load_bitmap(CheminImage);// on charge la tuile de format png en mémoire
                        if (!all_xx)
                            erreur("al_load_bitmap(\"xx.png\")");

                        al_draw_bitmap(all_xx, MAINX1 + j*30, MAIN1Y, 0);//on affiche la tuile dans la main du joueur (j sert à cibler cet endroit)
                        al_draw_bitmap(all_deselect32, MAINX1 + j*30, MAIN1Y, 0);// le contour noir pour dire que la tuile n'est pas sélectionnée
                        al_flip_display();//on rafraichi l'image
                        al_destroy_bitmap(all_xx);//on détruit l'image en mémoire

                }
                break;

            case 1: //j2

                for(int j=0; j<6; j++){
                        mainj2[j] = pioche[posi_pioche];
                        sprintf(CheminImage, "img/tuiles/%d.png", mainj2[j]);

                        posi_pioche++;

                        all_xx = al_load_bitmap(CheminImage);
                        if (!all_xx)
                            erreur("al_load_bitmap(\"xx.png\")");

                        al_draw_bitmap(all_xx, MAINX1 + j*30, MAIN2Y, 0);
                        al_draw_bitmap(all_interro, MAINX1 + j*30, MAIN2Y, 0);
                        al_draw_bitmap(all_deselect32, MAINX1 + j*30, MAIN2Y, 0);
                        al_flip_display();
                        al_destroy_bitmap(all_xx);

                }
                break;
            case 2: //j3

                for(int j=0; j<6; j++){
                        mainj3[j] = pioche[posi_pioche];
                        sprintf(CheminImage, "img/tuiles/%d.png", mainj3[j]);

                        posi_pioche++;

                        all_xx = al_load_bitmap(CheminImage);
                        if (!all_xx)
                            erreur("al_load_bitmap(\"xx.png\")");

                        al_draw_bitmap(all_xx, MAINX1 + j*30, MAIN3Y, 0);
                        al_draw_bitmap(all_interro, MAINX1 + j*30, MAIN3Y, 0);
                        al_draw_bitmap(all_deselect32, MAINX1 + j*30, MAIN3Y, 0);
                        al_flip_display();
                        al_destroy_bitmap(all_xx);
                }
                break;
            case 3: //j4

                for(int j=0; j<6; j++){ //remplir les mains de 6 cases
                        mainj4[j] = pioche[posi_pioche];  //mainj1 et pioche sont des tableaux de valeurs numériques

                        sprintf(CheminImage, "img/tuiles/%d.png", mainj4[j]);//les fichiers pnj portent les nom 00.pnj, 01.pnj ect ..

                        posi_pioche++;

                        all_xx = al_load_bitmap(CheminImage);
                        if (!all_xx)
                            erreur("al_load_bitmap(\"xx.png\")");

                        al_draw_bitmap(all_xx, MAINX1 + j*30, MAIN4Y, 0);
                        al_draw_bitmap(all_interro, MAINX1 + j*30, MAIN4Y, 0);
                        al_draw_bitmap(all_deselect32, MAINX1 + j*30, MAIN4Y, 0);
                        al_flip_display();
                        al_destroy_bitmap(all_xx);
                }
                break;
        }
    }
}

void echange(int mainj[6]){

    if(posi_pioche == 108){ //pioche vide
        printf("Vous ne pouvez pas échanger de cartes car la pioche est vide\n");
        return 0; //sortir de la fonction
    }

    int echange_temp;
        switch(caseSelect){ //on identifie c'est quelle case

        case 1:
                if(limit_echange[0] != 1){ //si on a pas encore échangé cette case
                    echange_temp = pioche[posi_pioche];          //échange des 2 pions
                    mainj[caseSelect-1] = pioche[posi_pioche];
                    pioche[posi_pioche] = echange_temp;          //fin de l'échange
                    limit_echange[0] = 1; //empeche d'échanger 2 fois la meme pièce
                    melange(posi_pioche); //on remélange la pioche apres
                } break;

        case 2:
                if(limit_echange[1] != 1){ //si on a pas encore échangé cette case
                    echange_temp = pioche[posi_pioche];          //échange des 2 pions
                    mainj[caseSelect-1] = pioche[posi_pioche];
                    pioche[posi_pioche] = echange_temp;          //fin de l'échange
                    limit_echange[1] = 1; //empeche d'échanger 2 fois la meme pièce
                    melange(posi_pioche); //on remélange la pioche apres
                } break;

        case 3:
                if(limit_echange[2] != 1){ //si on a pas encore échangé cette case
                    echange_temp = pioche[posi_pioche];          //échange des 2 pions
                    mainj[caseSelect-1] = pioche[posi_pioche];
                    pioche[posi_pioche] = echange_temp;          //fin de l'échange
                    limit_echange[2] = 1; //empeche d'échanger 2 fois la meme pièce
                    melange(posi_pioche); //on remélange la pioche apres
                } break;

        case 4:
                if(limit_echange[3] != 1){ //si on a pas encore échangé cette case
                    echange_temp = pioche[posi_pioche];          //échange des 2 pions
                    mainj[caseSelect-1] = pioche[posi_pioche];
                    pioche[posi_pioche] = echange_temp;          //fin de l'échange
                    limit_echange[3] = 1; //empeche d'échanger 2 fois la meme pièce
                    melange(posi_pioche); //on remélange la pioche apres
                } break;

        case 5:
                if(limit_echange[4] != 1){ //si on a pas encore échangé cette case
                    echange_temp = pioche[posi_pioche];          //échange des 2 pions
                    mainj[caseSelect-1] = pioche[posi_pioche];
                    pioche[posi_pioche] = echange_temp;          //fin de l'échange
                    limit_echange[4] = 1; //empeche d'échanger 2 fois la meme pièce
                    melange(posi_pioche); //on remélange la pioche apres
                } break;

        case 6:
                if(limit_echange[5] != 1){ //si on a pas encore échangé cette case
                    echange_temp = pioche[posi_pioche];          //échange des 2 pions
                    mainj[caseSelect-1] = pioche[posi_pioche];
                    pioche[posi_pioche] = echange_temp;          //fin de l'échange
                    limit_echange[5] = 1; //empeche d'échanger 2 fois la meme pièce
                    melange(posi_pioche); //on remélange la pioche apres
                } break;

        case 7:
                if(limit_echange[0] != 1){ //si on a pas encore échangé cette case
                    echange_temp = pioche[posi_pioche];          //échange des 2 pions
                    mainj[caseSelect-7] = pioche[posi_pioche];
                    pioche[posi_pioche] = echange_temp;          //fin de l'échange
                    limit_echange[0] = 1; //empeche d'échanger 2 fois la meme pièce
                    melange(posi_pioche); //on remélange la pioche apres
                } break;

        case 8:
                if(limit_echange[1] != 1){ //si on a pas encore échangé cette case
                    echange_temp = pioche[posi_pioche];          //échange des 2 pions
                    mainj[caseSelect-7] = pioche[posi_pioche];
                    pioche[posi_pioche] = echange_temp;          //fin de l'échange
                    limit_echange[1] = 1; //empeche d'échanger 2 fois la meme pièce
                    melange(posi_pioche); //on remélange la pioche apres
                } break;

        case 9:
                if(limit_echange[2] != 1){ //si on a pas encore échangé cette case
                    echange_temp = pioche[posi_pioche];          //échange des 2 pions
                    mainj[caseSelect-7] = pioche[posi_pioche];
                    pioche[posi_pioche] = echange_temp;          //fin de l'échange
                    limit_echange[2] = 1; //empeche d'échanger 2 fois la meme pièce
                    melange(posi_pioche); //on remélange la pioche apres
                } break;

        case 10:
                if(limit_echange[3] != 1){ //si on a pas encore échangé cette case
                    echange_temp = pioche[posi_pioche];          //échange des 2 pions
                    mainj[caseSelect-7] = pioche[posi_pioche];
                    pioche[posi_pioche] = echange_temp;          //fin de l'échange
                    limit_echange[3] = 1; //empeche d'échanger 2 fois la meme pièce
                    melange(posi_pioche); //on remélange la pioche apres
                } break;

        case 11:
                if(limit_echange[4] != 1){ //si on a pas encore échangé cette case
                    echange_temp = pioche[posi_pioche];          //échange des 2 pions
                    mainj[caseSelect-7] = pioche[posi_pioche];
                    pioche[posi_pioche] = echange_temp;          //fin de l'échange
                    limit_echange[4] = 1; //empeche d'échanger 2 fois la meme pièce
                    melange(posi_pioche); //on remélange la pioche apres
                } break;

        case 12:
                if(limit_echange[5] != 1){ //si on a pas encore échangé cette case
                    echange_temp = pioche[posi_pioche];          //échange des 2 pions
                    mainj[caseSelect-13] = pioche[posi_pioche];
                    pioche[posi_pioche] = echange_temp;          //fin de l'échange
                    limit_echange[5] = 1; //empeche d'échanger 2 fois la meme pièce
                    melange(posi_pioche); //on remélange la pioche apres
                } break;

        case 13:
                if(limit_echange[0] != 1){ //si on a pas encore échangé cette case
                    echange_temp = pioche[posi_pioche];          //échange des 2 pions
                    mainj[caseSelect-13] = pioche[posi_pioche];
                    pioche[posi_pioche] = echange_temp;          //fin de l'échange
                    limit_echange[0] = 1; //empeche d'échanger 2 fois la meme pièce
                    melange(posi_pioche); //on remélange la pioche apres
                } break;

        case 14:
                if(limit_echange[1] != 1){ //si on a pas encore échangé cette case
                    echange_temp = pioche[posi_pioche];          //échange des 2 pions
                    mainj[caseSelect-13] = pioche[posi_pioche];
                    pioche[posi_pioche] = echange_temp;          //fin de l'échange
                    limit_echange[1] = 1; //empeche d'échanger 2 fois la meme pièce
                    melange(posi_pioche); //on remélange la pioche apres
                } break;

        case 15:
                if(limit_echange[2] != 1){ //si on a pas encore échangé cette case
                    echange_temp = pioche[posi_pioche];          //échange des 2 pions
                    mainj[caseSelect-13] = pioche[posi_pioche];
                    pioche[posi_pioche] = echange_temp;          //fin de l'échange
                    limit_echange[2] = 1; //empeche d'échanger 2 fois la meme pièce
                    melange(posi_pioche); //on remélange la pioche apres
                } break;

        case 16:
                if(limit_echange[3] != 1){ //si on a pas encore échangé cette case
                    echange_temp = pioche[posi_pioche];          //échange des 2 pions
                    mainj[caseSelect-13] = pioche[posi_pioche];
                    pioche[posi_pioche] = echange_temp;          //fin de l'échange
                    limit_echange[3] = 1; //empeche d'échanger 2 fois la meme pièce
                    melange(posi_pioche); //on remélange la pioche apres
                } break;

        case 17:
                if(limit_echange[4] != 1){ //si on a pas encore échangé cette case
                    echange_temp = pioche[posi_pioche];          //échange des 2 pions
                    mainj[caseSelect-13] = pioche[posi_pioche];
                    pioche[posi_pioche] = echange_temp;          //fin de l'échange
                    limit_echange[4] = 1; //empeche d'échanger 2 fois la meme pièce
                    melange(posi_pioche); //on remélange la pioche apres
                } break;

        case 18:
                if(limit_echange[5] != 1){ //si on a pas encore échangé cette case
                    echange_temp = pioche[posi_pioche];          //échange des 2 pions
                    mainj[caseSelect-13] = pioche[posi_pioche];
                    pioche[posi_pioche] = echange_temp;          //fin de l'échange
                    limit_echange[5] = 1; //empeche d'échanger 2 fois la meme pièce
                    melange(posi_pioche); //on remélange la pioche apres
                } break;

        case 19:
                if(limit_echange[0] != 1){ //si on a pas encore échangé cette case
                    echange_temp = pioche[posi_pioche];          //échange des 2 pions
                    mainj[caseSelect-19] = pioche[posi_pioche];
                    pioche[posi_pioche] = echange_temp;          //fin de l'échange
                    limit_echange[0] = 1; //empeche d'échanger 2 fois la meme pièce
                    melange(posi_pioche); //on remélange la pioche apres
                } break;

        case 20:
                if(limit_echange[1] != 1){ //si on a pas encore échangé cette case
                    echange_temp = pioche[posi_pioche];          //échange des 2 pions
                    mainj[caseSelect-19] = pioche[posi_pioche];
                    pioche[posi_pioche] = echange_temp;          //fin de l'échange
                    limit_echange[1] = 1; //empeche d'échanger 2 fois la meme pièce
                    melange(posi_pioche); //on remélange la pioche apres
                } break;

        case 21:
                if(limit_echange[2] != 1){ //si on a pas encore échangé cette case
                    echange_temp = pioche[posi_pioche];          //échange des 2 pions
                    mainj[caseSelect-19] = pioche[posi_pioche];
                    pioche[posi_pioche] = echange_temp;          //fin de l'échange
                    limit_echange[2] = 1; //empeche d'échanger 2 fois la meme pièce
                    melange(posi_pioche); //on remélange la pioche apres
                } break;

        case 22:
                if(limit_echange[3] != 1){ //si on a pas encore échangé cette case
                    echange_temp = pioche[posi_pioche];          //échange des 2 pions
                    mainj[caseSelect-19] = pioche[posi_pioche];
                    pioche[posi_pioche] = echange_temp;          //fin de l'échange
                    limit_echange[3] = 1; //empeche d'échanger 2 fois la meme pièce
                    melange(posi_pioche); //on remélange la pioche apres
                } break;

        case 23:
                if(limit_echange[4] != 1){ //si on a pas encore échangé cette case
                    echange_temp = pioche[posi_pioche];          //échange des 2 pions
                    mainj[caseSelect-19] = pioche[posi_pioche];
                    pioche[posi_pioche] = echange_temp;          //fin de l'échange
                    limit_echange[4] = 1; //empeche d'échanger 2 fois la meme pièce
                    melange(posi_pioche); //on remélange la pioche apres
                } break;

        case 24:
                if(limit_echange[5] != 1){ //si on a pas encore échangé cette case
                    echange_temp = pioche[posi_pioche];          //échange des 2 pions
                    mainj[caseSelect-19] = pioche[posi_pioche];
                    pioche[posi_pioche] = echange_temp;          //fin de l'échange
                    limit_echange[5] = 1; //empeche d'échanger 2 fois la meme pièce
                    melange(posi_pioche); //on remélange la pioche apres
                } break;

        }

}

//************************fin tuile ****************************\\

int BoucleJeu2Joueurs(queue){

    //Mode DEBOGAGE
    #ifdef DEBOGAGE
    // CONSOLE NATIVE ALLEGRO pour affichage des tests
    ALLEGRO_TEXTLOG*textlog = NULL;
    textlog = al_open_native_text_log("Evénements souris, clavier", ALLEGRO_TEXTLOG_MONOSPACE);
    #endif // DEBOGAGE

    // BOUCLE EVENEMENTS
    int fin = 0, quitter = 0,selectOnOff = 0, selectOnOff2 = 0, afficheUneFois = 0;
    while (!fin){

        //si une main est vide + pioche vide = fin du jeu
        if(posi_pioche == 108){
            if(cond_fin(mainj1) == 1 || cond_fin(mainj2) == 1){
                return 7;
            }
        }

        // RECUPERATION DES EVENEMENTS
        ALLEGRO_EVENT event = { 0 };
        al_wait_for_event(queue, &event);

        // SELON EVENEMENT TROUVE
        switch (event.type){

                // EVENEMENTS SOURIS
            case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN: {
                mousepressed = TRUE;

                if ((selectOnOff == 1 && event.mouse.x >= GAMEX + GAMEL && event.mouse.x <= SCREENX-1) || (event.mouse.x <= GAMEX && event.mouse.x >= 1) || (event.mouse.y <= GAMEY && event.mouse.y >= 1)) {
                    DeselectOuPas = 0;
                    printf("On est passé dans tt sauf grille \n");
                    #ifdef DEBOGAGE
                    al_append_native_text_log(textlog,"Zone tout sauf plateau");
                    #endif
                }

                //pour déselectionner les carrés selectionnés si on clique(change selon le nombre de joueurs)
                if (selectOnOff == 1) {
                    printf("On est passé dans deselection \n");
                    #ifdef DEBOGAGE
                    al_append_native_text_log(textlog,"Zone tout");
                    #endif

                    // Clique dans la zone ECHANGE
                if (event.mouse.x >= ECHANGEX && event.mouse.x <= ECHANGEX + ECHANGEL && event.mouse.y >= ECHANGEY && event.mouse.y <= ECHANGEY + ECHANGEH) {
                    //Mode DEBOGAGE
                    #ifdef DEBOGAGE
                    al_append_native_text_log(textlog,"Zone ECHANGE\n");
                    #endif // DEBOGAGE

                    if(limit_action == 2){ //le joueur a déjà jouer, il ne peut donc pas échanger pendant le même tour
                        printf("Vous ne pouvez pas échanger et jouer pendant le même tour\n");
                    }

                    else{ //tout va bien --> le joueur n'a pas encore jouer de pion sur la grille pendant ce tour

                        switch(tour){

                        case 1:
                            echange(mainj1);
                            break;

                        case 2:
                            echange(mainj2);
                            break;

                        case 3:
                            echange(mainj3);
                            break;

                        case 4:
                            echange(mainj4);
                            break;

                        }
                        affichageMain();
                        al_flip_display();

                    }
                }

                    selectOnOff = 0;// Y a t il un carré de séléctionné ? 0 non 1 oui

                    for(int j=0; j < 6; j++){

                        al_draw_bitmap(all_deselect32, MAINX1 + j*30, MAIN1Y, 0);//contours noirs main joueur 1

                    }
                    for(int j=0; j < 6; j++){

                        al_draw_bitmap(all_deselect32, MAINX1 + j*30, MAIN2Y, 0);//contours noirs main joueur 2

                    }
                    al_flip_display();
                }

                //séléction du carré
                for(int i = 0; i<6; i++){
                    // clique dans la zone carré joueur 1
                    if (tour == 1 && selectOnOff == 0 && event.mouse.x >= MAINX1 + i*30 && event.mouse.x <= MAINX1 + 32 +30*i && event.mouse.y >= MAIN1Y && event.mouse.y <= MAIN1Y + 32 ) {
                        selectOnOff = 1;
                        afficheUneFois = 1;
                        DeselectOuPas = 1;
                        printf("On est passe dans selection j1\n");
                        switch(i){//quelle case est séléctionnée ?
                            case 0 :
                                    caseSelect = 1;

                                break;
                            case 1 :
                                    caseSelect = 2;

                                break;
                            case 2 :
                                    caseSelect = 3;

                                break;
                            case 3 :
                                    caseSelect = 4;

                                break;
                            case 4 :
                                    caseSelect = 5;

                                break;
                            case 5 :
                                    caseSelect = 6;

                                break;
                        }
                        #ifdef DEBOGAGE
                        al_append_native_text_log(textlog,"Zone carré joueurs\n");
                        #endif
                        al_draw_bitmap(all_select32, MAINX1 + i*30, MAIN1Y, 0); // contour rouge de la case
                        al_flip_display();
                        }

                        // clique dans la zone carré joueur 2
                        if (tour == 2 && selectOnOff == 0 && event.mouse.x >= MAINX1 + i*30 && event.mouse.x <= MAINX1 + 32 +30*i && event.mouse.y >= MAIN2Y && event.mouse.y <= MAIN2Y + 32) {
                            selectOnOff = 1;
                            afficheUneFois = 1;
                            DeselectOuPas = 1;
                            printf("On est passe dans selection j2\n");
                            switch(i){//quelle case est séléctionnée ?
                                case 0 :
                                    caseSelect = 7;

                                    break;
                                case 1 :
                                    caseSelect = 8;

                                    break;
                                case 2 :
                                    caseSelect = 9;

                                    break;
                                case 3 :
                                    caseSelect = 10;

                                    break;
                                case 4 :
                                    caseSelect = 11;

                                    break;
                                case 5 :
                                    caseSelect = 12;

                                    break;
                            }
                            #ifdef DEBOGAGE
                            al_append_native_text_log(textlog,"Zone carré joueurs\n");
                            #endif
                            al_draw_bitmap(all_select32, MAINX1 + i*30, MAIN2Y, 0); // contour rouge de la case
                            al_flip_display();
                        }
                    }

                // Clique dans la zone VALIDE
                if (event.mouse.x >= VALIDEX && event.mouse.x <= VALIDEX + VALIDEL && event.mouse.y >= VALIDEY && event.mouse.y <= VALIDEY + VALIDEH) {
                    switch(tour){
                    case 1:
                        tour = 2;
                    break;
                    default :
                        tour = 1;
                    break;
                    }
                    printf("Le tour est mtn : %d\n", tour);
                    affichageMain();
                    for(int i = 0; i<6; i++){
                        selectmainj1[i]=0;
                        selectmainj2[i]=0;
                        selectmainj3[i]=0;
                        selectmainj4[i]=0;
                    }

                    //Pour les échanges --> éviter d'échanger 2 fois la même pièce le même tour
                    // 0 = la pièce n'a pas encore été échangée | 1 = la pièce a déjà été échangée
                    for(i=0;i<6;i++){ //remplir le tableau de 0
                        limit_echange[i] = 0;
                    }

                    //pour empécher de faire un échange et placer un pion le même tour
                    limit_action = 0;
                    //Savoir si le joueur a déjà posé un pion ou pas(on reset a la fin du tour)
                    unPionPoseOuPas = 0;
                    //Mode DEBOGAGE
                    #ifdef DEBOGAGE
                    al_append_native_text_log(textlog,"Zone VALIDE\n");
                    #endif // DEBOGAGE
                }
                // Clique dans la zone CHARGE
                if (event.mouse.x >= CHARGEX && event.mouse.x <= CHARGEX + CHARGEL && event.mouse.y >= CHARGEY && event.mouse.y <= CHARGEY + CHARGEH) {
                    //Mode DEBOGAGE
                    #ifdef DEBOGAGE
                    al_append_native_text_log(textlog,"Zone CHARGE\n");
                    #endif // DEBOGAGE
                }
                // Clique dans la zone QUITTE
                if (event.mouse.x >= QUITTEX && event.mouse.x <= QUITTEX + QUITTEL && event.mouse.y >= QUITTEY && event.mouse.y <= QUITTEY + QUITTEH && quitter == 0) {
                    quitter = 1;
                    //Mode DEBOGAGE
                    #ifdef DEBOGAGE
                    al_append_native_text_log(textlog,"Zone QUITTE\n");
                    #endif // DEBOGAGE
                    return 1;
                }

                //Mode DEBOGAGE
                #ifdef DEBOGAGE
                al_append_native_text_log(textlog,"bouton %d presse\n", event.mouse.button);
                #endif // DEBOGAGE
                bmovement = FALSE;
                }

                break;
                al_flip_display();
            case ALLEGRO_EVENT_MOUSE_BUTTON_UP: {
                mousepressed = FALSE;
                //Mode DEBOGAGE
                #ifdef DEBOGAGE
                al_append_native_text_log(textlog,"bouton %d relache\n", event.mouse.button);
                #endif // DEBOGAGE
                if (bmovement == FALSE && event.mouse.x >= GAMEX && event.mouse.x <= GAMEX + GAMEL && event.mouse.y >= GAMEY && event.mouse.y <= GAMEY + GAMEH) {
                    al_append_native_text_log(textlog,"bouton clique\n", event.mouse.button);

                    iclicx = (x + event.mouse.x - GAMEX) / 32;
                    iclicy = (y + event.mouse.y - GAMEY) / 32;

                    if (DeselectOuPas == 1 && afficheUneFois == 1 && event.mouse.x >= GAMEX && event.mouse.x <= GAMEX + GAMEL && event.mouse.y >= GAMEY && event.mouse.y <= GAMEY + GAMEH){
                        printf("On est passé dans main vers grille \n");

                        for(i=0;i<6;i++){
                            if(limit_echange[i] == 1){ //on regarde si un échange a déjà été fait
                                limit_action = 1;
                            }
                        }

                        if(limit_action == 1){ //un échange a déjà été fait
                            printf("vous ne pouvez pas échanger et jouer pendant le même tour\n");
                        }

                        else{//tout va bien

                            afficheUneFois = 0;
                            //pour empecher de faire un échange et placer un pion le même tour
                            limit_action = 2;

                            if(unPionPoseOuPas == 1){
                                printf("on est passe dans unPionPoseOuPas (= 1)\n");
                                if(aiclicy == iclicy){
                                    printf("on est passe dans aiclicy == iclicy\n");
                                    PoserUneTuilej1();
                                    PoserUneTuilej2();
                                    aiclicx = -1;
                                }
                                if(aiclicx == iclicx){
                                    printf("on est passe dans aiclicx == iclicx\n");
                                    PoserUneTuilej1();
                                    PoserUneTuilej2();
                                    aiclicy = -1;
                                }
                            }
                            else{
                                printf("Y'a pas de pion pose avant celui la normalement");

                                PoserUneTuilej1();
                                PoserUneTuilej2();
                            }
                        }
                    }
                }
            }
            break;
                // si mouvement
            case ALLEGRO_EVENT_MOUSE_AXES: {
                // Si bouton de souris appuyé et curseur sur la zone de jeu
                if (mousepressed && event.mouse.x >= GAMEX && event.mouse.x <= GAMEX + GAMEL && event.mouse.y >= GAMEY && event.mouse.y <= GAMEY + GAMEH && quitter == 0) {
                    // déplacement du décor
                    if (event.mouse.dx != 0) {
                            x = (x - event.mouse.dx < 0) ? 0 : x - event.mouse.dx;
                            bmovement = TRUE;
                    }
                    if (event.mouse.dy != 0) {
                           y = (y - event.mouse.dy < 0) ? 0 : y - event.mouse.dy;
                           bmovement = TRUE;
                    }
                    // affichage
                    al_draw_bitmap_region(decor, x, y, GAMEL, GAMEH, GAMEX, GAMEY, 0);
                    al_flip_display();
                    //al_rest(1 / 30.0);
                }
                //Mode DEBOGAGE
                #ifdef DEBOGAGE
                al_append_native_text_log(textlog,"x:%4d y:%4d dx:%4d dy%4d z:%3d w%3d\n",
                    event.mouse.x, event.mouse.y, //position verticale, horizontale
                    event.mouse.dx, event.mouse.dy,// mouvement
                    event.mouse.z,// position molette verticale
                    event.mouse.w);// position molette horizontale
                #endif // DEBOGAGE
                }
                break;

                // EVENEMENTS CLAVIER
            case ALLEGRO_EVENT_KEY_DOWN: {
                const char*nomkey = al_keycode_to_name(event.keyboard.keycode);
                // vitesse de déplacement du décor
                pas = 32;
                // déplacement du décor
                if (event.keyboard.keycode == ALLEGRO_KEY_LEFT)
                    x = (x - pas < 0) ? 0 : x - pas;

                if (event.keyboard.keycode == ALLEGRO_KEY_RIGHT)
                    x = (x + pas >DECORX - GAMEL) ? DECORX - GAMEL : x + pas;

                if (event.keyboard.keycode == ALLEGRO_KEY_UP)
                    y = (y - pas < 0) ? 0 : y - pas;

                if (event.keyboard.keycode == ALLEGRO_KEY_DOWN)
                    y = (y + pas > DECORY - GAMEH) ? DECORY - GAMEH : y + pas;

                // affichage
                al_draw_bitmap_region(decor, x, y, GAMEL, GAMEH, GAMEX, GAMEY, 0);
                al_flip_display();
                al_rest(1 / 30.0);
                //Mode DEBOGAGE
                #ifdef DEBOGAGE
                al_append_native_text_log(textlog, "%8s : %s\n","DOWN", nomkey);
                #endif // DEBOGAGE
                }
                break;
            case ALLEGRO_EVENT_KEY_UP: {
                //Mode DEBOGAGE
                #ifdef DEBOGAGE
                const char*nomkey = al_keycode_to_name(event.keyboard.keycode);
                al_append_native_text_log(textlog, "%8s : %s\n","UP", nomkey);
                #endif // DEBOGAGE
                }
                break;
                // caractères et répétitions
            case ALLEGRO_EVENT_KEY_CHAR: {
                char*label = event.keyboard.repeat ?"repeat" : "KEY_CHAR";
                // vitesse de déplacement du décor
                pas = 12;
                // déplacement du décor
                if (event.keyboard.keycode == ALLEGRO_KEY_LEFT)
                    x = (x - pas < 0) ? 0 : x - pas;

                if (event.keyboard.keycode == ALLEGRO_KEY_RIGHT)
                    x = (x + pas >DECORX - GAMEL) ? DECORX - GAMEL : x + pas;

                if (event.keyboard.keycode == ALLEGRO_KEY_UP)
                    y = (y - pas < 0) ? 0 : y - pas;

                if (event.keyboard.keycode == ALLEGRO_KEY_DOWN)
                    y = (y + pas > DECORY - GAMEH) ? DECORY - GAMEH : y + pas;

                // affichage
                al_draw_bitmap_region(decor, x, y, GAMEL, GAMEH, GAMEX, GAMEY, 0);
                al_flip_display();
                al_rest(1 / 30.0);
                //Mode DEBOGAGE
                #ifdef DEBOGAGE
                const char*nomkey = al_keycode_to_name(event.keyboard.keycode);
                al_append_native_text_log(textlog, "%8s : %s\n",label, nomkey);
                #endif // DEBOGAGE
                }
                break;

                // EVENEMENTS FENETRE
            case ALLEGRO_EVENT_DISPLAY_CLOSE: {
                return 0;
                }
            break;
        }

    }
}

int BoucleJeu3Joueurs(queue){

    //Mode DEBOGAGE
    #ifdef DEBOGAGE
    // CONSOLE NATIVE ALLEGRO pour affichage des tests
    ALLEGRO_TEXTLOG*textlog = NULL;
    textlog = al_open_native_text_log("Evénements souris, clavier", ALLEGRO_TEXTLOG_MONOSPACE);
    #endif // DEBOGAGE

    // BOUCLE EVENEMENTS
    int fin = 0, quitter = 0,selectOnOff = 0, selectOnOff2 = 0, afficheUneFois = 0;
    while (!fin){

        //si une main est vide + pioche vide = fin du jeu
        if(posi_pioche == 108){
            if(cond_fin(mainj1) == 1 || cond_fin(mainj2) == 1 || cond_fin(mainj3) == 1){
                return 7;
            }
        }

        // RECUPERATION DES EVENEMENTS
        ALLEGRO_EVENT event = { 0 };
        al_wait_for_event(queue, &event);

        // SELON EVENEMENT TROUVE
        switch (event.type){

                // EVENEMENTS SOURIS
            case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN: {
                mousepressed = TRUE;

                if ((selectOnOff == 1 && event.mouse.x >= GAMEX + GAMEL && event.mouse.x <= SCREENX-1) || (event.mouse.x <= GAMEX && event.mouse.x >= 1) || (event.mouse.y <= GAMEY && event.mouse.y >= 1)) {
                    DeselectOuPas = 0;
                    printf("On est passé dans tt sauf grille \n");
                    #ifdef DEBOGAGE
                    al_append_native_text_log(textlog,"Zone tout sauf plateau");
                    #endif
                }

                //pour déselectionner les carrés selectionnés si on clique(change selon le nombre de joueurs)
                if (selectOnOff == 1) {
                    printf("On est passé dans deselection \n");
                    #ifdef DEBOGAGE
                    al_append_native_text_log(textlog,"Zone tout");
                    #endif

                    // Clique dans la zone ECHANGE
                if (event.mouse.x >= ECHANGEX && event.mouse.x <= ECHANGEX + ECHANGEL && event.mouse.y >= ECHANGEY && event.mouse.y <= ECHANGEY + ECHANGEH) {
                    //Mode DEBOGAGE
                    #ifdef DEBOGAGE
                    al_append_native_text_log(textlog,"Zone ECHANGE\n");
                    #endif // DEBOGAGE

                    if(limit_action == 2){ //le joueur a déjà jouer, il ne peut donc pas échanger pendant le même tour
                        printf("Vous ne pouvez pas échanger et jouer pendant le même tour\n");
                    }

                    else{ //tout va bien --> le joueur n'a pas encore jouer de pion sur la grille pendant ce tour
                        switch(tour){

                        case 1:
                            echange(mainj1);
                            break;

                        case 2:
                            echange(mainj2);
                            break;

                        case 3:
                            echange(mainj3);
                            break;

                        case 4:
                            echange(mainj4);
                            break;

                        }
                        affichageMain();
                        al_flip_display();

                    }
                }

                    selectOnOff = 0;// Y a t il un carré de séléctionné ? 0 non 1 oui

                    for(int j=0; j < 6; j++){

                        al_draw_bitmap(all_deselect32, MAINX1 + j*30, MAIN1Y, 0);//contours noirs main joueur 1

                    }
                    for(int j=0; j < 6; j++){

                        al_draw_bitmap(all_deselect32, MAINX1 + j*30, MAIN2Y, 0);//contours noirs main joueur 2

                    }
                    for(int j=0; j < 6; j++){

                        al_draw_bitmap(all_deselect32, MAINX1 + j*30, MAIN3Y, 0);//contours noirs main joueur 3

                    }
                    al_flip_display();
                }

                //séléction du carré
                for(int i = 0; i<6; i++){
                    // clique dans la zone carré joueur 1
                    if (tour == 1 && selectOnOff == 0 && event.mouse.x >= MAINX1 + i*30 && event.mouse.x <= MAINX1 + 32 +30*i && event.mouse.y >= MAIN1Y && event.mouse.y <= MAIN1Y + 32 ) {
                        selectOnOff = 1;
                        afficheUneFois = 1;
                        DeselectOuPas = 1;
                        printf("On est passe dans selection j1\n");
                        switch(i){//quelle case est séléctionnée ?
                            case 0 :
                                    caseSelect = 1;

                                break;
                            case 1 :
                                    caseSelect = 2;

                                break;
                            case 2 :
                                    caseSelect = 3;

                                break;
                            case 3 :
                                    caseSelect = 4;

                                break;
                            case 4 :
                                    caseSelect = 5;

                                break;
                            case 5 :
                                    caseSelect = 6;

                                break;
                        }
                        #ifdef DEBOGAGE
                        al_append_native_text_log(textlog,"Zone carré joueurs\n");
                        #endif
                        al_draw_bitmap(all_select32, MAINX1 + i*30, MAIN1Y, 0); // contour rouge de la case
                        al_flip_display();
                        }

                        // clique dans la zone carré joueur 2
                        if (tour == 2 && selectOnOff == 0 && event.mouse.x >= MAINX1 + i*30 && event.mouse.x <= MAINX1 + 32 +30*i && event.mouse.y >= MAIN2Y && event.mouse.y <= MAIN2Y + 32) {
                            selectOnOff = 1;
                            afficheUneFois = 1;
                            DeselectOuPas = 1;
                            printf("On est passe dans selection j2\n");
                            switch(i){//quelle case est séléctionnée ?
                                case 0 :
                                    caseSelect = 7;

                                    break;
                                case 1 :
                                    caseSelect = 8;

                                    break;
                                case 2 :
                                    caseSelect = 9;

                                    break;
                                case 3 :
                                    caseSelect = 10;

                                    break;
                                case 4 :
                                    caseSelect = 11;

                                    break;
                                case 5 :
                                    caseSelect = 12;

                                    break;
                            }
                            #ifdef DEBOGAGE
                            al_append_native_text_log(textlog,"Zone carré joueur 2\n");
                            #endif
                            al_draw_bitmap(all_select32, MAINX1 + i*30, MAIN2Y, 0); // contour rouge de la case
                            al_flip_display();
                        }

                        if (tour == 3 && selectOnOff == 0 && event.mouse.x >= MAINX1 + i*30 && event.mouse.x <= MAINX1 + 32 +30*i && event.mouse.y >= MAIN3Y && event.mouse.y <= MAIN3Y + 32) {
                            selectOnOff = 1;
                            afficheUneFois = 1;
                            DeselectOuPas = 1;
                            printf("On est passe dans selection j1\n");
                            switch(i){//quelle case est séléctionnée ?
                                case 0 :
                                    caseSelect = 13;

                                break;
                                case 1 :
                                    caseSelect = 14;

                                break;
                                case 2 :
                                    caseSelect = 15;

                                break;
                                case 3 :
                                    caseSelect = 16;

                                break;
                                case 4 :
                                    caseSelect = 17;

                                break;
                                case 5 :
                                    caseSelect = 18;

                                break;
                            }
                            #ifdef DEBOGAGE
                            al_append_native_text_log(textlog,"Zone carré joueurs\n");
                            #endif
                            al_draw_bitmap(all_select32, MAINX1 + i*30, MAIN3Y, 0); // contour rouge de la case
                            al_flip_display();
                        }
                    }


                // Clique dans la zone VALIDE
                if (event.mouse.x >= VALIDEX && event.mouse.x <= VALIDEX + VALIDEL && event.mouse.y >= VALIDEY && event.mouse.y <= VALIDEY + VALIDEH) {
                    switch(tour){
                    case 1:
                        tour = 2;
                    break;
                    case 2:
                        tour = 3;
                    break;
                    default :
                        tour = 1;
                    break;
                    }
                    printf("Le tour est mtn : %d\n", tour);
                    affichageMain();
                    for(int i = 0; i<6; i++){
                        selectmainj1[i]=0;
                        selectmainj2[i]=0;
                        selectmainj3[i]=0;
                        selectmainj4[i]=0;
                    }

                    //Pour les échanges --> éviter d'échanger 2 fois la même pièce le même tour
                    // 0 = la pièce n'a pas encore été échangée | 1 = la pièce a déjà été échangée
                    for(i=0;i<6;i++){ //remplir le tableau de 0
                        limit_echange[i] = 0;
                    }

                    //pour empécher de faire un échange et placer un pion le même tour
                    limit_action = 0;

                    unPionPoseOuPas = 0;

                    //Mode DEBOGAGE
                    #ifdef DEBOGAGE
                    al_append_native_text_log(textlog,"Zone VALIDE\n");
                    #endif // DEBOGAGE
                }
                // Clique dans la zone CHARGE
                if (event.mouse.x >= CHARGEX && event.mouse.x <= CHARGEX + CHARGEL && event.mouse.y >= CHARGEY && event.mouse.y <= CHARGEY + CHARGEH) {
                    //Mode DEBOGAGE
                    #ifdef DEBOGAGE
                    al_append_native_text_log(textlog,"Zone CHARGE\n");
                    #endif // DEBOGAGE
                }
                // Clique dans la zone QUITTE
                if (event.mouse.x >= QUITTEX && event.mouse.x <= QUITTEX + QUITTEL && event.mouse.y >= QUITTEY && event.mouse.y <= QUITTEY + QUITTEH && quitter == 0) {
                    quitter = 1;
                    //Mode DEBOGAGE
                    #ifdef DEBOGAGE
                    al_append_native_text_log(textlog,"Zone QUITTE\n");
                    #endif // DEBOGAGE
                    return 1;
                }

                //Mode DEBOGAGE
                #ifdef DEBOGAGE
                al_append_native_text_log(textlog,"bouton %d presse\n", event.mouse.button);
                #endif // DEBOGAGE
                bmovement = FALSE;
                }

                break;
                al_flip_display();
            case ALLEGRO_EVENT_MOUSE_BUTTON_UP: {
                mousepressed = FALSE;
                //Mode DEBOGAGE
                #ifdef DEBOGAGE
                al_append_native_text_log(textlog,"bouton %d relache\n", event.mouse.button);
                #endif // DEBOGAGE
                if (bmovement == FALSE && event.mouse.x >= GAMEX && event.mouse.x <= GAMEX + GAMEL && event.mouse.y >= GAMEY && event.mouse.y <= GAMEY + GAMEH) {
                    al_append_native_text_log(textlog,"bouton clique\n", event.mouse.button);

                    iclicx = (x + event.mouse.x - GAMEX) / 32;
                    iclicy = (y + event.mouse.y - GAMEY) / 32;

                    if (DeselectOuPas == 1 && afficheUneFois == 1 && event.mouse.x >= GAMEX && event.mouse.x <= GAMEX + GAMEL && event.mouse.y >= GAMEY && event.mouse.y <= GAMEY + GAMEH){
                        printf("On est passé dans main vers grille \n");

                        for(i=0;i<6;i++){
                            if(limit_echange[i] == 1){ //on regarde si un échange a déjà été fait
                                limit_action = 1;
                            }
                        }

                        if(limit_action == 1){ //un échange a déjà été fait
                            printf("vous ne pouvez pas échanger et jouer pendant le même tour\n");
                        }

                        else{ //tout va bien
                            afficheUneFois = 0;
                            //pour empécher de faire un échange et placer un pion le même tour
                            limit_action = 2;

                            if(unPionPoseOuPas == 1){
                                printf("on est passe dans unPionPoseOuPas (= 1)\n");
                                if(aiclicy == iclicy){
                                    printf("on est passe dans aiclicy == iclicy\n");
                                    PoserUneTuilej1();
                                    PoserUneTuilej2();
                                    PoserUneTuilej3();
                                    aiclicx = -1;
                                }
                                if(aiclicx == iclicx){
                                    printf("on est passe dans aiclicx == iclicx\n");
                                    PoserUneTuilej1();
                                    PoserUneTuilej2();
                                    PoserUneTuilej3();
                                    aiclicy = -1;
                                }
                            }
                            else{
                                printf("Y'a pas de pion pose avant celui la normalement\n");

                                PoserUneTuilej1();
                                PoserUneTuilej2();
                                PoserUneTuilej3();
                            }
                        }
                    }
                }
            }
            break;
                // si mouvement
            case ALLEGRO_EVENT_MOUSE_AXES: {
                // Si bouton de souris appuyé et curseur sur la zone de jeu
                if (mousepressed && event.mouse.x >= GAMEX && event.mouse.x <= GAMEX + GAMEL && event.mouse.y >= GAMEY && event.mouse.y <= GAMEY + GAMEH && quitter == 0) {
                    // déplacement du décor
                    if (event.mouse.dx != 0) {
                            x = (x - event.mouse.dx < 0) ? 0 : x - event.mouse.dx;
                            bmovement = TRUE;
                    }
                    if (event.mouse.dy != 0) {
                           y = (y - event.mouse.dy < 0) ? 0 : y - event.mouse.dy;
                           bmovement = TRUE;
                    }
                    // affichage
                    al_draw_bitmap_region(decor, x, y, GAMEL, GAMEH, GAMEX, GAMEY, 0);
                    al_flip_display();
                    //al_rest(1 / 30.0);
                }
                //Mode DEBOGAGE
                #ifdef DEBOGAGE
                al_append_native_text_log(textlog,"x:%4d y:%4d dx:%4d dy%4d z:%3d w%3d\n",
                    event.mouse.x, event.mouse.y, //position verticale, horizontale
                    event.mouse.dx, event.mouse.dy,// mouvement
                    event.mouse.z,// position molette verticale
                    event.mouse.w);// position molette horizontale
                #endif // DEBOGAGE
                }
                break;

                // EVENEMENTS CLAVIER
            case ALLEGRO_EVENT_KEY_DOWN: {
                const char*nomkey = al_keycode_to_name(event.keyboard.keycode);
                // vitesse de déplacement du décor
                pas = 32;
                // déplacement du décor
                if (event.keyboard.keycode == ALLEGRO_KEY_LEFT)
                    x = (x - pas < 0) ? 0 : x - pas;

                if (event.keyboard.keycode == ALLEGRO_KEY_RIGHT)
                    x = (x + pas >DECORX - GAMEL) ? DECORX - GAMEL : x + pas;

                if (event.keyboard.keycode == ALLEGRO_KEY_UP)
                    y = (y - pas < 0) ? 0 : y - pas;

                if (event.keyboard.keycode == ALLEGRO_KEY_DOWN)
                    y = (y + pas > DECORY - GAMEH) ? DECORY - GAMEH : y + pas;

                // affichage
                al_draw_bitmap_region(decor, x, y, GAMEL, GAMEH, GAMEX, GAMEY, 0);
                al_flip_display();
                al_rest(1 / 30.0);
                //Mode DEBOGAGE
                #ifdef DEBOGAGE
                al_append_native_text_log(textlog, "%8s : %s\n","DOWN", nomkey);
                #endif // DEBOGAGE
                }
                break;
            case ALLEGRO_EVENT_KEY_UP: {
                //Mode DEBOGAGE
                #ifdef DEBOGAGE
                const char*nomkey = al_keycode_to_name(event.keyboard.keycode);
                al_append_native_text_log(textlog, "%8s : %s\n","UP", nomkey);
                #endif // DEBOGAGE
                }
                break;
                // caractères et répétitions
            case ALLEGRO_EVENT_KEY_CHAR: {
                char*label = event.keyboard.repeat ?"repeat" : "KEY_CHAR";
                // vitesse de déplacement du décor
                pas = 12;
                // déplacement du décor
                if (event.keyboard.keycode == ALLEGRO_KEY_LEFT)
                    x = (x - pas < 0) ? 0 : x - pas;

                if (event.keyboard.keycode == ALLEGRO_KEY_RIGHT)
                    x = (x + pas >DECORX - GAMEL) ? DECORX - GAMEL : x + pas;

                if (event.keyboard.keycode == ALLEGRO_KEY_UP)
                    y = (y - pas < 0) ? 0 : y - pas;

                if (event.keyboard.keycode == ALLEGRO_KEY_DOWN)
                    y = (y + pas > DECORY - GAMEH) ? DECORY - GAMEH : y + pas;

                // affichage
                al_draw_bitmap_region(decor, x, y, GAMEL, GAMEH, GAMEX, GAMEY, 0);
                al_flip_display();
                al_rest(1 / 30.0);
                //Mode DEBOGAGE
                #ifdef DEBOGAGE
                const char*nomkey = al_keycode_to_name(event.keyboard.keycode);
                al_append_native_text_log(textlog, "%8s : %s\n",label, nomkey);
                #endif // DEBOGAGE
                }
                break;

                // EVENEMENTS FENETRE
            case ALLEGRO_EVENT_DISPLAY_CLOSE: {
                return 0;
                }
            break;
        }

    }
}

int BoucleJeu4Joueurs(queue){

    //Mode DEBOGAGE
    #ifdef DEBOGAGE
    // CONSOLE NATIVE ALLEGRO pour affichage des tests
    ALLEGRO_TEXTLOG*textlog = NULL;
    textlog = al_open_native_text_log("Evénements souris, clavier", ALLEGRO_TEXTLOG_MONOSPACE);
    #endif // DEBOGAGE

    //si une main est vide + pioche vide = fin du jeu
        if(posi_pioche == 108){
            if(cond_fin(mainj1) == 1 || cond_fin(mainj2) == 1 || cond_fin(mainj3) == 1 || cond_fin(mainj4) == 1){
                return 7;
            }
        }

    // BOUCLE EVENEMENTS
    int fin = 0, quitter = 0,selectOnOff = 0, selectOnOff2 = 0, afficheUneFois = 0;
    while (!fin){

        //si une main est vide + pioche vide = fin du jeu
        if(posi_pioche == 108){
            if(cond_fin(mainj1) == 1 || cond_fin(mainj2) == 1 || cond_fin(mainj3) == 1 || cond_fin(mainj4) == 1){
                return 1;
            }
        }

        // RECUPERATION DES EVENEMENTS
        ALLEGRO_EVENT event = { 0 };
        al_wait_for_event(queue, &event);

        // SELON EVENEMENT TROUVE
        switch (event.type){

                // EVENEMENTS SOURIS
            case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN: {
                mousepressed = TRUE;

                if ((selectOnOff == 1 && event.mouse.x >= GAMEX + GAMEL && event.mouse.x <= SCREENX-1) || (event.mouse.x <= GAMEX && event.mouse.x >= 1) || (event.mouse.y <= GAMEY && event.mouse.y >= 1)) {
                    DeselectOuPas = 0;
                    printf("On est passé dans tt sauf grille \n");
                    #ifdef DEBOGAGE
                    al_append_native_text_log(textlog,"Zone tout sauf plateau");
                    #endif
                }

                //pour déselectionner les carrés selectionnés si on clique(change selon le nombre de joueurs)
                if (selectOnOff == 1) {
                    printf("On est passé dans deselection \n");
                    #ifdef DEBOGAGE
                    al_append_native_text_log(textlog,"Zone tout");
                    #endif

                    // Clique dans la zone ECHANGE
                if (event.mouse.x >= ECHANGEX && event.mouse.x <= ECHANGEX + ECHANGEL && event.mouse.y >= ECHANGEY && event.mouse.y <= ECHANGEY + ECHANGEH) {
                    //Mode DEBOGAGE
                    #ifdef DEBOGAGE
                    al_append_native_text_log(textlog,"Zone ECHANGE\n");
                    #endif // DEBOGAGE

                    if(limit_action == 2){ //le joueur a déjà jouer, il ne peut donc pas échanger pendant le même tour
                        printf("Vous ne pouvez pas échanger et jouer pendant le même tour\n");
                    }

                    else{ //tout va bien --> le joueur n'a pas encore jouer de pion sur la grille pendant ce tour
                        switch(tour){

                        case 1:
                            echange(mainj1);
                            break;

                        case 2:
                            echange(mainj2);
                            break;

                        case 3:
                            echange(mainj3);
                            break;

                        case 4:
                            echange(mainj4);
                            break;

                        }

                        affichageMain();
                        al_flip_display();
                    }
                }

                    selectOnOff = 0;// Y a t il un carré de séléctionné ? 0 non 1 oui

                    for(int j=0; j < 6; j++){

                        al_draw_bitmap(all_deselect32, MAINX1 + j*30, MAIN1Y, 0);//contours noirs main joueur 1

                    }
                    for(int j=0; j < 6; j++){

                        al_draw_bitmap(all_deselect32, MAINX1 + j*30, MAIN2Y, 0);//contours noirs main joueur 2

                    }
                    for(int j=0; j < 6; j++){

                        al_draw_bitmap(all_deselect32, MAINX1 + j*30, MAIN3Y, 0);//contours noirs main joueur 3

                    }
                    for(int j=0; j < 6; j++){

                        al_draw_bitmap(all_deselect32, MAINX1 + j*30, MAIN4Y, 0);//contours noirs main joueur 3

                    }
                    al_flip_display();
                }

                //séléction du carré
                for(int i = 0; i<6; i++){
                    // clique dans la zone carré joueur 1
                    if (tour == 1 && selectOnOff == 0 && event.mouse.x >= MAINX1 + i*30 && event.mouse.x <= MAINX1 + 32 +30*i && event.mouse.y >= MAIN1Y && event.mouse.y <= MAIN1Y + 32 ) {
                        selectOnOff = 1;
                        afficheUneFois = 1;
                        DeselectOuPas = 1;
                        printf("On est passe dans selection j1\n");
                        switch(i){//quelle case est séléctionnée ?
                            case 0 :
                                    caseSelect = 1;

                                break;
                            case 1 :
                                    caseSelect = 2;

                                break;
                            case 2 :
                                    caseSelect = 3;

                                break;
                            case 3 :
                                    caseSelect = 4;

                                break;
                            case 4 :
                                    caseSelect = 5;

                                break;
                            case 5 :
                                    caseSelect = 6;

                                break;
                        }
                        #ifdef DEBOGAGE
                        al_append_native_text_log(textlog,"Zone carré joueurs\n");
                        #endif
                        al_draw_bitmap(all_select32, MAINX1 + i*30, MAIN1Y, 0); // contour rouge de la case
                        al_flip_display();
                        }

                        // clique dans la zone carré joueur 2
                        if (tour == 2 && selectOnOff == 0 && event.mouse.x >= MAINX1 + i*30 && event.mouse.x <= MAINX1 + 32 +30*i && event.mouse.y >= MAIN2Y && event.mouse.y <= MAIN2Y + 32) {
                            selectOnOff = 1;
                            afficheUneFois = 1;
                            DeselectOuPas = 1;
                            printf("On est passe dans selection j2\n");
                            switch(i){//quelle case est séléctionnée ?
                                case 0 :
                                    caseSelect = 7;

                                    break;
                                case 1 :
                                    caseSelect = 8;

                                    break;
                                case 2 :
                                    caseSelect = 9;

                                    break;
                                case 3 :
                                    caseSelect = 10;

                                    break;
                                case 4 :
                                    caseSelect = 11;

                                    break;
                                case 5 :
                                    caseSelect = 12;

                                    break;
                            }
                            #ifdef DEBOGAGE
                            al_append_native_text_log(textlog,"Zone carré joueur 2\n");
                            #endif
                            al_draw_bitmap(all_select32, MAINX1 + i*30, MAIN2Y, 0); // contour rouge de la case
                            al_flip_display();
                        }

                        if (tour == 3 && selectOnOff == 0 && event.mouse.x >= MAINX1 + i*30 && event.mouse.x <= MAINX1 + 32 +30*i && event.mouse.y >= MAIN3Y && event.mouse.y <= MAIN3Y + 32) {
                            selectOnOff = 1;
                            afficheUneFois = 1;
                            DeselectOuPas = 1;
                            printf("On est passe dans selection j3\n");
                            switch(i){//quelle case est séléctionnée ?
                                case 0 :
                                    caseSelect = 13;

                                break;
                                case 1 :
                                    caseSelect = 14;

                                break;
                                case 2 :
                                    caseSelect = 15;

                                break;
                                case 3 :
                                    caseSelect = 16;

                                break;
                                case 4 :
                                    caseSelect = 17;

                                break;
                                case 5 :
                                    caseSelect = 18;

                                break;
                            }
                            #ifdef DEBOGAGE
                            al_append_native_text_log(textlog,"Zone carré joueurs\n");
                            #endif
                            al_draw_bitmap(all_select32, MAINX1 + i*30, MAIN3Y, 0); // contour rouge de la case
                            al_flip_display();
                        }
                        if (tour == 4 && selectOnOff == 0 && event.mouse.x >= MAINX1 + i*30 && event.mouse.x <= MAINX1 + 32 +30*i && event.mouse.y >= MAIN4Y && event.mouse.y <= MAIN4Y + 32) {
                            selectOnOff = 1;
                            afficheUneFois = 1;
                            DeselectOuPas = 1;
                            printf("On est passe dans selection j4\n");
                            switch(i){//quelle case est séléctionnée ?
                                case 0 :
                                    caseSelect = 19;

                                break;
                                case 1 :
                                    caseSelect = 20;

                                break;
                                case 2 :
                                    caseSelect = 21;

                                break;
                                case 3 :
                                    caseSelect = 22;

                                break;
                                case 4 :
                                    caseSelect = 23;

                                break;
                                case 5 :
                                    caseSelect = 24;

                                break;
                            }
                            #ifdef DEBOGAGE
                            al_append_native_text_log(textlog,"Zone carré joueurs\n");
                            #endif
                            al_draw_bitmap(all_select32, MAINX1 + i*30, MAIN4Y, 0); // contour rouge de la case
                            al_flip_display();
                        }
                    }

                // Clique dans la zone VALIDE
                if (event.mouse.x >= VALIDEX && event.mouse.x <= VALIDEX + VALIDEL && event.mouse.y >= VALIDEY && event.mouse.y <= VALIDEY + VALIDEH) {
                    switch(tour){
                    case 1:
                        tour = 2;
                    break;
                    case 2:
                        tour = 3;
                    break;
                    case 3:
                        tour = 4;
                    break;
                    default :
                        tour = 1;
                    break;
                    }
                    printf("Le tour est mtn : %d\n", tour);
                    affichageMain();
                    for(int i = 0; i<6; i++){
                        selectmainj1[i]=0;
                        selectmainj2[i]=0;
                        selectmainj3[i]=0;
                        selectmainj4[i]=0;
                    }

                    //Pour les échanges --> éviter d'échanger 2 fois la même pièce le même tour
                    // 0 = la pièce n'a pas encore été échangée | 1 = la pièce a déjà été échangée
                    for(i=0;i<6;i++){ //remplir le tableau de 0
                        limit_echange[i] = 0;
                    }

                    //pour empécher de faire un échange et placer un pion le même tour
                    limit_action = 0;
                    unPionPoseOuPas = 0;
                    //Mode DEBOGAGE
                    #ifdef DEBOGAGE
                    al_append_native_text_log(textlog,"Zone VALIDE\n");
                    #endif // DEBOGAGE
                }
                // Clique dans la zone CHARGE
                if (event.mouse.x >= CHARGEX && event.mouse.x <= CHARGEX + CHARGEL && event.mouse.y >= CHARGEY && event.mouse.y <= CHARGEY + CHARGEH) {
                    //Mode DEBOGAGE
                    #ifdef DEBOGAGE
                    al_append_native_text_log(textlog,"Zone CHARGE\n");
                    #endif // DEBOGAGE
                }
                // Clique dans la zone QUITTE
                if (event.mouse.x >= QUITTEX && event.mouse.x <= QUITTEX + QUITTEL && event.mouse.y >= QUITTEY && event.mouse.y <= QUITTEY + QUITTEH && quitter == 0) {
                    quitter = 1;
                    //Mode DEBOGAGE
                    #ifdef DEBOGAGE
                    al_append_native_text_log(textlog,"Zone QUITTE\n");
                    #endif // DEBOGAGE
                    return 1;
                }

                //Mode DEBOGAGE
                #ifdef DEBOGAGE
                al_append_native_text_log(textlog,"bouton %d presse\n", event.mouse.button);
                #endif // DEBOGAGE
                bmovement = FALSE;
                }

                break;

            case ALLEGRO_EVENT_MOUSE_BUTTON_UP: {
                mousepressed = FALSE;
                //Mode DEBOGAGE
                #ifdef DEBOGAGE
                al_append_native_text_log(textlog,"bouton %d relache\n", event.mouse.button);
                #endif // DEBOGAGE
                if (bmovement == FALSE && event.mouse.x >= GAMEX && event.mouse.x <= GAMEX + GAMEL && event.mouse.y >= GAMEY && event.mouse.y <= GAMEY + GAMEH) {
                    al_append_native_text_log(textlog,"bouton clique\n", event.mouse.button);

                    iclicx = (x + event.mouse.x - GAMEX) / 32;
                    iclicy = (y + event.mouse.y - GAMEY) / 32;

                    if (DeselectOuPas == 1 && afficheUneFois == 1 && event.mouse.x >= GAMEX && event.mouse.x <= GAMEX + GAMEL && event.mouse.y >= GAMEY && event.mouse.y <= GAMEY + GAMEH){
                        printf("On est passé dans main vers grille \n");

                        for(i=0;i<6;i++){
                            if(limit_echange[i] == 1){ //on regarde si un échange a déjà été fait
                                limit_action = 1;
                            }
                        }

                        if(limit_action == 1){ //un échange a déjà été fait
                            printf("vous ne pouvez pas échanger et jouer pendant le même tour\n");
                        }

                        else{ //tout va bien
                            afficheUneFois = 0;
                            //pour empécher de faire un échange et placer un pion le même tour
                            limit_action = 2;

                            if(unPionPoseOuPas == 1){
                                printf("on est passe dans unPionPoseOuPas (= 1)\n");
                                if(aiclicy == iclicy){
                                    printf("on est passe dans aiclicy == iclicy\n");
                                    PoserUneTuilej1();
                                    PoserUneTuilej2();
                                    PoserUneTuilej3();
                                    PoserUneTuilej4();
                                    aiclicx = -1;
                                }
                                if(aiclicx == iclicx){
                                    printf("on est passe dans aiclicx == iclicx\n");
                                    PoserUneTuilej1();
                                    PoserUneTuilej2();
                                    PoserUneTuilej3();
                                    PoserUneTuilej4();
                                    aiclicy = -1;
                                }
                            }
                            else{
                                printf("Y'a pas de pion pose avant celui la normalement\n");

                                PoserUneTuilej1();
                                PoserUneTuilej2();
                                PoserUneTuilej3();
                                PoserUneTuilej4();
                            }
                        }
                    }
                }
            }
            break;
                // si mouvement
            case ALLEGRO_EVENT_MOUSE_AXES: {
                // Si bouton de souris appuyé et curseur sur la zone de jeu
                if (mousepressed && event.mouse.x >= GAMEX && event.mouse.x <= GAMEX + GAMEL && event.mouse.y >= GAMEY && event.mouse.y <= GAMEY + GAMEH && quitter == 0) {
                    // déplacement du décor
                    if (event.mouse.dx != 0) {
                            x = (x - event.mouse.dx < 0) ? 0 : x - event.mouse.dx;
                            bmovement = TRUE;
                    }
                    if (event.mouse.dy != 0) {
                           y = (y - event.mouse.dy < 0) ? 0 : y - event.mouse.dy;
                           bmovement = TRUE;
                    }
                    // affichage
                    al_draw_bitmap_region(decor, x, y, GAMEL, GAMEH, GAMEX, GAMEY, 0);
                    al_flip_display();
                    //al_rest(1 / 30.0);
                }
                //Mode DEBOGAGE
                #ifdef DEBOGAGE
                al_append_native_text_log(textlog,"x:%4d y:%4d dx:%4d dy%4d z:%3d w%3d\n",
                    event.mouse.x, event.mouse.y, //position verticale, horizontale
                    event.mouse.dx, event.mouse.dy,// mouvement
                    event.mouse.z,// position molette verticale
                    event.mouse.w);// position molette horizontale
                #endif // DEBOGAGE
                }
                break;

                // EVENEMENTS CLAVIER
            case ALLEGRO_EVENT_KEY_DOWN: {
                const char*nomkey = al_keycode_to_name(event.keyboard.keycode);
                // vitesse de déplacement du décor
                pas = 32;
                // déplacement du décor
                if (event.keyboard.keycode == ALLEGRO_KEY_LEFT)
                    x = (x - pas < 0) ? 0 : x - pas;

                if (event.keyboard.keycode == ALLEGRO_KEY_RIGHT)
                    x = (x + pas >DECORX - GAMEL) ? DECORX - GAMEL : x + pas;

                if (event.keyboard.keycode == ALLEGRO_KEY_UP)
                    y = (y - pas < 0) ? 0 : y - pas;

                if (event.keyboard.keycode == ALLEGRO_KEY_DOWN)
                    y = (y + pas > DECORY - GAMEH) ? DECORY - GAMEH : y + pas;

                // affichage
                al_draw_bitmap_region(decor, x, y, GAMEL, GAMEH, GAMEX, GAMEY, 0);
                al_flip_display();
                al_rest(1 / 30.0);
                //Mode DEBOGAGE
                #ifdef DEBOGAGE
                al_append_native_text_log(textlog, "%8s : %s\n","DOWN", nomkey);
                #endif // DEBOGAGE
                }
                break;
            case ALLEGRO_EVENT_KEY_UP: {
                //Mode DEBOGAGE
                #ifdef DEBOGAGE
                const char*nomkey = al_keycode_to_name(event.keyboard.keycode);
                al_append_native_text_log(textlog, "%8s : %s\n","UP", nomkey);
                #endif // DEBOGAGE
                }
                break;
                // caractères et répétitions
            case ALLEGRO_EVENT_KEY_CHAR: {
                char*label = event.keyboard.repeat ?"repeat" : "KEY_CHAR";
                // vitesse de déplacement du décor
                pas = 12;
                // déplacement du décor
                if (event.keyboard.keycode == ALLEGRO_KEY_LEFT)
                    x = (x - pas < 0) ? 0 : x - pas;

                if (event.keyboard.keycode == ALLEGRO_KEY_RIGHT)
                    x = (x + pas >DECORX - GAMEL) ? DECORX - GAMEL : x + pas;

                if (event.keyboard.keycode == ALLEGRO_KEY_UP)
                    y = (y - pas < 0) ? 0 : y - pas;

                if (event.keyboard.keycode == ALLEGRO_KEY_DOWN)
                    y = (y + pas > DECORY - GAMEH) ? DECORY - GAMEH : y + pas;

                // affichage
                al_draw_bitmap_region(decor, x, y, GAMEL, GAMEH, GAMEX, GAMEY, 0);
                al_flip_display();
                al_rest(1 / 30.0);
                //Mode DEBOGAGE
                #ifdef DEBOGAGE
                const char*nomkey = al_keycode_to_name(event.keyboard.keycode);
                al_append_native_text_log(textlog, "%8s : %s\n",label, nomkey);
                #endif // DEBOGAGE
                }
                break;

                // EVENEMENTS FENETRE
            case ALLEGRO_EVENT_DISPLAY_CLOSE: {
                return 0;
                }
            break;
        }

    }
}

void GrilleRefresh(){

            ALLEGRO_BITMAP*tuile = recup_sprite(
                all_tuiles,
                TX + 1, TY + 1,
                0, 0,
                NB_COL,
                MAP[iclicx][iclicy]); // élément
            if (!tuile)
                erreur("recup_sprite()");
            al_set_target_bitmap(decor);
            al_draw_bitmap(tuile, iclicx*TX, iclicy*TY, 0);
            al_destroy_bitmap(tuile);
            al_set_target_backbuffer(display);
            al_flip_display();

}

void DistributionUneCarte(){
    switch(tour){
        case 1 :
            if(posi_pioche == 108){ //si y'a plus rien dans la pioche
                mainj1[posiTabMain] = 6;

                al_draw_bitmap(all_vide, MAINX1 + 30*(caseSelect - 1), MAIN1Y, 0);
                al_flip_display();
            }

            else{ //si il reste des pions dans la pioche (aka tout va bien)
                mainj1[posiTabMain] = pioche[posi_pioche];

                posi_pioche++;
                al_draw_bitmap(all_interro, MAINX1 + 30*(caseSelect - 1), MAIN1Y, 0);
                al_flip_display();
            }

        break;

        case 2 :
            if(posi_pioche == 108){ //si y'a plus rien dans la pioche
                mainj2[posiTabMain] = 6;

                al_draw_bitmap(all_vide, MAINX1 + 30*(caseSelect - 7), MAIN2Y, 0);
                al_flip_display();
            }

            else{ //si il reste des pions dans la pioche (aka tout va bien)
                mainj2[posiTabMain] = pioche[posi_pioche];

                posi_pioche++;
                al_draw_bitmap(all_interro, MAINX1 + 30*(caseSelect - 7), MAIN2Y, 0);
                al_flip_display();
            }

        break;
        case 3 :
            if(posi_pioche == 108){ //si y'a plus rien dans la pioche
                mainj3[posiTabMain] = 6;

                al_draw_bitmap(all_vide, MAINX1 + 30*(caseSelect - 13), MAIN3Y, 0);
                al_flip_display();
            }

            else{ //si il reste des pions dans la pioche (aka tout va bien)
                mainj3[posiTabMain] = pioche[posi_pioche];

                posi_pioche++;
                al_draw_bitmap(all_interro, MAINX1 + 30*(caseSelect - 13), MAIN3Y, 0);
                al_flip_display();
            }

        break;
        case 4 :
            if(posi_pioche == 108){ //si y'a plus rien dans la pioche
                mainj4[posiTabMain] = 6;

                al_draw_bitmap(all_vide, MAINX1 + 30*(caseSelect - 19), MAIN4Y, 0);
                al_flip_display();
            }

            else{ //si il reste des pions dans la pioche (aka tout va bien)
                mainj4[posiTabMain] = pioche[posi_pioche];

                posi_pioche++;
                al_draw_bitmap(all_interro, MAINX1 + 30*(caseSelect - 19), MAIN4Y, 0);
                al_flip_display();
            };

        break;
    }
}

int cond_fin(int mainj[6]){
    main_vide = 0; //initialisation du compteur

        for(j=0;j<6;j++){

            if(mainj[j] == 6){
                main_vide++;
            }
        }

        if(main_vide == 6){
            printf("fin du jeu\n");
            return 1;
        }

        else{return 0;} //il reste des pions dans la pioche

}

int qui_commence(int mainj[6]){ //détermine qui commence
    int same_color_temp, same_type_temp;
    same_color_final = 1; same_type_final = 1;

    for(j=0;j<=5;j++){ //on compare chaque pièce
        same_color_temp = 1;
        same_type_temp = 1;

        for(i=0;i<=5;i++){ //à toutes les autres pièces

            if( !(mainj[j] % 10 == mainj[i] % 10 && mainj[j] / 10 == mainj[i] / 10) ){ //si c'est pas le meme pion

                if(mainj[j] / 10 == mainj[i] / 10){ //dizaine

                    same_color_temp++;

                    if(same_color_final < same_color_temp){
                        same_color_final = same_color_temp;
                    }
                }


                if(mainj[j] % 10 == mainj[i] % 10){ //unité

                    same_type_temp++;

                    if(same_type_final < same_type_temp){
                        same_type_final = same_type_temp;
                    }
                }

            }

        }
    }


    //pour comparaison entre les joueurs
    if(same_type_final > same_color_final){
        return(same_type_final);}

    else{return(same_color_final);}


}

void affichageMain(){

        switch(tour){
            case 1: {//j1
                printf("On est rentré dans affiche main j1\n");
                for(int j=0; j < 6; j++){ //afficher les mains de 6 cases

                    if(mainj1[j]==6){ //si la case ne contient pas de pion

                        al_draw_bitmap(all_vide, MAINX1 + j*30, MAIN1Y, 0);
                        al_draw_bitmap(all_deselect32, MAINX1 + j*30, MAIN1Y, 0);
                        al_draw_bitmap(all_interro, MAINX1 + j*30, MAIN2Y, 0);
                        if(NbJoueur + NbIA == 3)
                        al_draw_bitmap(all_interro, MAINX1 + j*30, MAIN3Y, 0);

                        if(NbJoueur + NbIA == 4)
                            al_draw_bitmap(all_interro, MAINX1 + j*30, MAIN4Y, 0);
                        }

                    else{ //si y'a encore des pions dans la pioche
                    sprintf(CheminImage, "img/tuiles/%d.png", mainj1[j]);//les fichiers pnj portent les noms 00.pnj, 01.pnj ect ..

                    all_xx = al_load_bitmap(CheminImage);
                    if (!all_xx)
                            erreur("al_load_bitmap(\"xx.png\")");

                    al_draw_bitmap(all_xx, MAINX1 + j*30, MAIN1Y, 0);
                    al_draw_bitmap(all_deselect32, MAINX1 + j*30, MAIN1Y, 0);
                    al_destroy_bitmap(all_xx);
                    al_draw_bitmap(all_interro, MAINX1 + j*30, MAIN2Y, 0);
                    if(NbJoueur + NbIA == 3)
                        al_draw_bitmap(all_interro, MAINX1 + j*30, MAIN3Y, 0);

                    if(NbJoueur + NbIA == 4)
                        al_draw_bitmap(all_interro, MAINX1 + j*30, MAIN4Y, 0);
                    }
                }
            }
            break;
            case 2:{//j2
                printf("On est rentré dans affiche main j2\n");
                for(int j=0; j < 6; j++){ //afficher les mains de 6 cases

                    if(mainj2[j]==6){ //si la case ne contient pas de pion

                        al_draw_bitmap(all_vide, MAINX1 + j*30, MAIN2Y, 0);
                        al_draw_bitmap(all_deselect32, MAINX1 + j*30, MAIN2Y, 0);
                        al_draw_bitmap(all_interro, MAINX1 + j*30, MAIN1Y, 0);
                        if(NbJoueur + NbIA == 3)
                        al_draw_bitmap(all_interro, MAINX1 + j*30, MAIN3Y, 0);

                        if(NbJoueur + NbIA == 4)
                            al_draw_bitmap(all_interro, MAINX1 + j*30, MAIN4Y, 0);
                    }

                    else{ //si y'a encore des pions dans la pioche
                    sprintf(CheminImage, "img/tuiles/%d.png", mainj2[j]);//les fichiers pnj portent les noms 00.pnj, 01.pnj ect ..

                    all_xx = al_load_bitmap(CheminImage);
                    if (!all_xx)
                        erreur("al_load_bitmap(\"xx.png\")");

                    al_draw_bitmap(all_xx, MAINX1 + j*30, MAIN2Y, 0);
                    al_draw_bitmap(all_deselect32, MAINX1 + j*30, MAIN2Y, 0);
                    al_destroy_bitmap(all_xx);
                    al_draw_bitmap(all_interro, MAINX1 + j*30, MAIN1Y, 0);
                    if(NbJoueur + NbIA == 3)
                        al_draw_bitmap(all_interro, MAINX1 + j*30, MAIN3Y, 0);

                    if(NbJoueur + NbIA == 4)
                        al_draw_bitmap(all_interro, MAINX1 + j*30, MAIN4Y, 0);
                    }
                }
            }
            break;

            case 3:{//j3
                printf("On est rentré dans affiche main j3\n");
                for(int j=0; j < 6; j++){ //afficher les mains de 6 cases

                    if(mainj3[j]==6){ //si la case ne contient pas de pion

                        al_draw_bitmap(all_vide, MAINX1 + j*30, MAIN3Y, 0);
                        al_draw_bitmap(all_deselect32, MAINX1 + j*30, MAIN3Y, 0);
                        al_draw_bitmap(all_interro, MAINX1 + j*30, MAIN1Y, 0);
                        al_draw_bitmap(all_interro, MAINX1 + j*30, MAIN2Y, 0);
                        if(NbJoueur + NbIA == 4)
                            al_draw_bitmap(all_interro, MAINX1 + j*30, MAIN4Y, 0);
                        }


                    else{ //si y'a encore des pions dans la pioche
                    sprintf(CheminImage, "img/tuiles/%d.png", mainj3[j]);//les fichiers pnj portent les noms 00.pnj, 01.pnj ect ..

                    all_xx = al_load_bitmap(CheminImage);
                    if (!all_xx)
                        erreur("al_load_bitmap(\"xx.png\")");

                    al_draw_bitmap(all_xx, MAINX1 + j*30, MAIN3Y, 0);
                    al_draw_bitmap(all_deselect32, MAINX1 + j*30, MAIN3Y, 0);
                    al_destroy_bitmap(all_xx);
                    al_draw_bitmap(all_interro, MAINX1 + j*30, MAIN1Y, 0);
                    al_draw_bitmap(all_interro, MAINX1 + j*30, MAIN2Y, 0);
                    if(NbJoueur + NbIA == 4)
                        al_draw_bitmap(all_interro, MAINX1 + j*30, MAIN4Y, 0);
                    }
                }
            }
            break;
            case 4:{//j4
                printf("On est rentré dans affiche main j4\n");
                for(int j=0; j < 6; j++){ //afficher les mains de 6 cases

                    if(mainj4[j]==6){ //si la case ne contient pas de pion

                        al_draw_bitmap(all_vide, MAINX1 + j*30, MAIN4Y, 0);
                        al_draw_bitmap(all_deselect32, MAINX1 + j*30, MAIN4Y, 0);
                        al_draw_bitmap(all_interro, MAINX1 + j*30, MAIN1Y, 0);
                        al_draw_bitmap(all_interro, MAINX1 + j*30, MAIN2Y, 0);
                        al_draw_bitmap(all_interro, MAINX1 + j*30, MAIN3Y, 0);
                    }

                    else{ //si y'a encore des pions dans la pioche
                    sprintf(CheminImage, "img/tuiles/%d.png", mainj4[j]);//les fichiers pnj portent les noms 00.pnj, 01.pnj ect ..

                    all_xx = al_load_bitmap(CheminImage);
                    if (!all_xx)
                        erreur("al_load_bitmap(\"xx.png\")");

                    al_draw_bitmap(all_xx, MAINX1 + j*30, MAIN4Y, 0);
                    al_draw_bitmap(all_deselect32, MAINX1 + j*30, MAIN4Y, 0);
                    al_destroy_bitmap(all_xx);
                    al_draw_bitmap(all_interro, MAINX1 + j*30, MAIN1Y, 0);
                    al_draw_bitmap(all_interro, MAINX1 + j*30, MAIN2Y, 0);
                    al_draw_bitmap(all_interro, MAINX1 + j*30, MAIN3Y, 0);
                    }
                }
            }
            break;
        }
        al_flip_display();
}

void PoserUneTuilej1(){

    for(int i = 1; i<7; i++){
        if(i == caseSelect && selectmainj1[i-1] != 1){
            iscore = fiapositionvalide(iclicx, iclicy, mainj1[i-1]);
            if(iscore != -1){
                selectmainj1[i-1] = 1;
                MAP[iclicx][iclicy] = mainj1[i-1];
                GrilleRefresh();
                posiTabMain = i-1;
                DistributionUneCarte();
                DeselectOuPas = 1;
                nbPionPose += 1;
                aiclicx = iclicx;
                aiclicy = iclicy;
                printf("unpionposeoupas devient = 1 maintenant\n");
                unPionPoseOuPas = 1;
            }
        }
    }

}

void PoserUneTuilej2(){

    for(int i = 7; i<13 ; i++){
        if(i == caseSelect && selectmainj2[i-7] != 1){
            iscore = fiapositionvalide(iclicx, iclicy, mainj2[i-7]);
            if(iscore != -1){
                selectmainj2[i-7] = 1;
                MAP[iclicx][iclicy] = mainj2[i-7];
                GrilleRefresh();
                posiTabMain = i-7;
                DistributionUneCarte();
                DeselectOuPas = 1;
                nbPionPose += 1;
                unPionPoseOuPas = 1;
                aiclicx = iclicx;
                aiclicy = iclicy;
            }
        }
    }

}

void PoserUneTuilej3(){
                            for(int i = 13; i<19; i++){
                                    if(i == caseSelect && selectmainj2[i-13] != 1){
                                        iscore = fiapositionvalide(iclicx, iclicy, mainj3[i-13]);
                                        if(iscore != -1){
                                            selectmainj1[i-13] = 1;
                                            MAP[iclicx][iclicy] = mainj3[i-13];
                                            GrilleRefresh();
                                            posiTabMain = i-13;
                                            DistributionUneCarte();
                                            DeselectOuPas = 1;
                                            nbPionPose += 1;
                                            unPionPoseOuPas = 1;
                                            aiclicx = iclicx;
                                            aiclicy = iclicy;
                                        }
                                    }
                            }

}

void PoserUneTuilej4(){

    for(int i = 19; i<25; i++){
        if(i == caseSelect && selectmainj1[i-19] != 1){
            iscore = fiapositionvalide(iclicx, iclicy, mainj4[i-19]);
            if(iscore != -1){
                selectmainj4[i-19] = 1;
                MAP[iclicx][iclicy] = mainj4[i-19];
                GrilleRefresh();
                posiTabMain = i-19;
                DistributionUneCarte();
                DeselectOuPas = 1;
                nbPionPose += 1;
                unPionPoseOuPas = 1;
                aiclicx = iclicx;
                aiclicy = iclicy;
            }
        }
    }

}
