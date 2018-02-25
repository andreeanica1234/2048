#include <ncurses.h> 
#include <time.h>
#include <sys/select.h>
#include <string.h>
#include <stdlib.h>

#define MAX 10000

void margini(WINDOW *s) {   //deseneaza marginile ferestrei jocului
    int i, x, y;
    getmaxyx(s, y, x);		//seteaza marimea ferestrei
    
    mvwprintw(s, 0, 0, "*");
    mvwprintw(s, y-1, 0, "*" );
    mvwprintw(s, 0, x-1, "*");
    mvwprintw(s, y-1, x-1, "*");
    
    for (i = 0; i < (y-1); i++) {
    	mvwprintw(s, i, 0, "*");
        mvwprintw(s, i, x-1, "*");
    }
    
    for (i = 1; i<(x-1); i++) {
        mvwprintw(s, 0, i, "*");
        mvwprintw(s, y-1, i, "*");
    }
}

void print(WINDOW *s, int k[4][4]) {     // afiseaza matricea k in fereasra s 

	int i,j;
	start_color();    	// initializeaza culorile in terminal
	
	init_pair(5, COLOR_BLACK, COLOR_WHITE);
	init_pair(6, COLOR_YELLOW, COLOR_BLACK);
	attron(COLOR_PAIR(6));
	wbkgd(s, COLOR_PAIR(6));		
	attroff(COLOR_PAIR(6));
	
	for (i = 0; i < 4; i++)
			for (j = 0; j < 4; j++)
				if (j < 3)					 // daca j=3, formula 5*i+3 nu ar fi acoperit si ultima coloana a matricei 
					mvwprintw(s, 5*i+3, 8*j+4, "    ");
				else
					mvwprintw(s, 5*i+3, 4*j+16, "    ");	
					
	for (i = 0; i < 4; i++)
			for (j = 0; j < 4; j++)
				if (k[i][j] != 0) {
					if (j < 3)
						mvwprintw(s, 5*i+3, 8*j+4, "%d", k[i][j]);
					else
						mvwprintw(s, 5*i+3, 4*j+16, "%d", k[i][j]);	
				}
	wrefresh(s);	
	refresh();				// afisarea nu se realizeaza fara actualizarea ferestrei
}

void generare_cifre(WINDOW *s, int k[4][4]) {  // genereaza random 2 sau 4 pe o pozitie libera din matricea k;
	
	int i, j, m, o;
	int n[2] = { 2, 4 };  
	int l = 1;
	srand(time(NULL));
	
	while (l == 1) {
		// i si j devin un nr random din intervalul [0,3]
		i = rand() % 4;     
		j = rand() % 4;
		if (k[i][j] == 0) {    // se genereaza o cifra numai pe o casuta libera
			l = 0;       // dupa generarea unei cifre, se iese din bucla
			m = rand() % 2;  // m=0 sau m=1; 
			o = n[m];
			k[i][j] = o;
		}
	}
}

void matrice(WINDOW *s) { // "deseneaza" casutele tablei de joc

	int i;
	getmaxyx(s, i, i);
	
	for (i = 0; i < 32; i++) {		
		mvwprintw(s, 5, i, "_" );
		mvwprintw(s, 10, i, "_" );
		mvwprintw(s, 15, i, "_" );
	}
	
	for (i = 0; i < 22; i++) {	
		mvwprintw(s, i, 8, "|" );
		mvwprintw(s, i, 16, "|" );
		mvwprintw(s, i, 24, "|" );
	}
}

void ordonare_s(int k[4][4]) {	//duce valorile !=0 din matricea k pe pozitiile inferioare disponibile, in ordinea aparitiei;

	int i, h = 3, j = 0, aux;
    while (j < 4) {
        for (i = 3; i >= 0; i--)	
	        if (k[i][j] != 0) {
				//se porneste de pe pozitiile superioare; daca gaseste un 0 il aseaza in "varful" coloanei matricei;
                aux = k[i][j];
                k[i][j] = k[h][j];
                k[h][j] = aux;
                h--;
	        }
        j++;
        h = 3;
   }
}

void ordonare_w(int k[4][4]) {	// duce valorile !=0 din matricea k pe pozitiile superioare disponibile, in ordinea aparitiei;

	int i, j = 0, h = 0, aux;
	while (j < 4) {
		for (i=0;i<4;i++)
			if (k[i][j]!=0) {
		        aux = k[i][j];
		        k[i][j] = k[h][j];
		        k[h][j] = aux;
		        h++;
            }
		j++;
		h = 0;
	}	
}

void ordonare_d(int k[4][4]) {	// muta la dreapta valorile != 0 din matricea k;

	int i = 0, j, h = 3, aux;
	while (i < 4) {
        for (j = 3;j >= 0; j--)
            if (k[i][j] != 0) {
                aux = k[i][j];
                k[i][j] = k[i][h];
                k[i][h] = aux;
                h--;
        	}
		i++;
		h = 3;
	}
}

void ordonare_a(int k[4][4]) { 	//muta la stanga valorile !=0 din matricea k;

	int i = 0, h = 0, j, aux;
    while (i < 4) {
        for (j=0;j<4;j++)
            if (k[i][j] != 0) {
	            aux = k[i][j];
	            k[i][j] = k[i][h];
	            k[i][h] = aux;
	            h++;
	    	}
		i++;
		h = 0;
 	}
}

void adunare_s(int k[4][4], int scor[1]) {  //aduna elementele care sunt egale si consecutive, pe coloana, de jos in sus, si actualizeaza scorul;

	int i = 3, j = 0;
    while (j < 4) {
        while (i >= 0) {
                if (k[i][j] == k[i-1][j]) {
                    k[i][j] = k[i][j] * 2;
					scor[0] = scor[0] + k[i][j];
                    k[i-1][j] = 0;
                    i = i-2;
                }
                else 
                	i--;
        }
       	j++;
        i = 3;
    }
    ordonare_s(k);   // dupa ce elementele sunt adunate, se reordoneaza fiecare coloana astfel incat "0"-urile sa se afle pe pozitiile superioare;
}

void adunare_a(int k[4][4], int scor[1]) {	//aduna elementele care sunt egale si consecutive, pe rand, de la dreapta la stanga, si actualizeaza scorul;

	int i = 0, j = 0;
    while (i < 4) {
        while (j < 4) {
                if (k[i][j] == k[i][j + 1]) {
                    k[i][j + 1] = k[i][j] * 2;
					scor[0] = scor[0] + k[i][j + 1];
                    k[i][j] = 0;
                    j = j + 2;
                }
                else
                	j++;
        }
        i++;
        j = 0;
    }
	ordonare_a(k);	// dupa ce elementele sunt adunate, se reordoneaza fiecare rand, astfel incat "0"-urile sa se afle pe pozitiile din dreapta;
}

void adunare_d(int k[4][4], int scor[1]) { //aduna elementele care sunt egale si consecutive, pe coloana, de la stanga la dreapta, si actualizeaza scorul
	
	int i = 0, j = 3;
   while (i < 4) {
   		while (j >= 0) {
		    if (k[i][j] == k[i][j - 1]) {
		            k[i][j] = k[i][j] * 2;
					scor[0] = scor[0] + k[i][j];
		            k[i][j - 1] = 0;
		            j = j - 2;
		    }
		    else
		   		j--;
		}
        i++;
        j = 3;
    }
	ordonare_d(k); // dupa ce elementele sunt adunate, se reordoneaza fiecare rand, astfel incat "0"-urile sa se afle pe pozitiile din stanga;
}

void adunare_w(int k[4][4], int scor[1]) { //aduna elementele care sunt egale si consecutive, pe coloana, de sus in jos, si actualizeaza scorul;

	int i = 0, j = 0;
	while (j < 4) {
		while (i < 4) {
			if (k[i][j] == k[i + 1][j]) {
				k[i + 1][j] = k[i + 1][j] * 2;
				scor[0] = scor[0] + k[i + 1][j];
				k[i][j] = 0;
				i = i + 2;
			}
			else
		 		i++;
		}
		j++;		
		i = 0;
	}
	ordonare_w(k);	 // dupa ce elementele sunt adunate, se reordoneaza fiecare coloana, astfel incat "0"-urile sa se afle pe pozitiile inferioare;
}

void copiaza(int id[4][4], int k[4][4]) { //face o copie a unei matrici;

	int i, j;
	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++)
			id[i][j] = k[i][j];

}

int compara(int id[4][4], int k[4][4]) { //compara doua matrici; 

	int i, j;
	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++)
			if (id[i][j] != k[i][j])		//cand pe aceeasi pozitie din cele doua matrici se gasesc elemente diferite, functia returneaza 1
				return 1; 		
	return 0;					//functia returneaza 0 cand matricile sunt egale
}

int mutare_optima(int k[4][4]) {      //verifica daca mai exista mutari valide

	int i, j;
	int mutare[4][4];
	copiaza(mutare, k);  		//verificarea se face pe o copie a matricei jocului, pentru a nu altera jocul;
	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++)
			if(mutare[i][j] == 0)
				return 0;		//daca mai exista pozitii disponibile => mai exista mutari valide; 
	//daca doua elemente consecutive sunt egale =>mai exista cel putin o mutare valida;	
	i = 0;
	j = 0;
	while (i < 4) {
		while (j < 4)                       
			if (k[i][j] == k[i][j + 1])
				return 0;
			else 
				j++;
		i++;
		j = 0;
	}
	i = 0;
	j = 0;
	while (j < 4) {
		while (i < 4)
			if(k[i][j] == k[i + 1][j])
				return 0;
			else
				i++;
		j++;
		i = 0;
	}
	return 1;				// returneaza 1 daca nu mai exista nicio mutare valida
}

void meniu(int u, int copie[4][4], int scor[1]) {	//meniul jocului

    start_color();		//initialzeaza culorile
    init_pair(8, COLOR_YELLOW, COLOR_BLACK);		//text galben, fond negru
    int ch, i = 0, k = 0, x, y, j;
    int l[4][4], sco[1];
   
	//formam o fereastra in care se va afla fereastra meniului jocului
    getmaxyx(stdscr,y, x);
    WINDOW *z = newwin(y, x, 0, 0);
    wrefresh(z);
    box(z, 0, 0);

    char cuv[3][9] = { "New Game" , "Resume", "Quit" };  // memoram optiunile din meniu;
    char a[9];   
    WINDOW *w = newwin(10, 20, 7, 30); // creeaza fereastra meniului
    box(w, 0, 0); // "deseneaza" marginile ferestrei meniului
    attron(COLOR_PAIR(8));  //activeaza culorile
    wbkgd(w, COLOR_PAIR(8)); //aplica culorile ferestrei
    attroff(COLOR_PAIR(8));  //dezactiveaza culorile
    refresh();

	// afiseaza elementele meniului si evidentiaza primul element
	for (i = 0; i < 3; i++ ) {
		if (i == 0)
			wattron( w, A_STANDOUT);   // selecteaza primul element
		else
			wattroff( w, A_STANDOUT);
        sprintf(a, "%s",  cuv[i]);
        mvwprintw(w, i + 1, 2, "%s", a);
    }

    wrefresh(w); 		// reactualizeaza fereastra
    i = 0;
    noecho(); 	// caracterele tastate nu sunt afisate pe ecran
    keypad(w, TRUE); 	// porneste tastatura pentru fereastra  meniului
    curs_set(0); // ascunde cursorul
    while((ch != '\n' || i != 2) && k == 0) {  
        
		ch = wgetch(w); 		// ch preia valoarea ascii a tastei apasate
	    sprintf(a, "%-8s", cuv[i]);
	    mvwprintw(w, i + 1, 2, "%s", a);
	          
	    if (u == 1)    //cand u==1 este afisat meniul cu posibilitatea selectiei optiunii RESUME
			switch(ch) {
	        	case KEY_UP:
	                        i--;
	                        if (i < 0)  //key_up pe "new game" duce pe "quit"
								i = 2;	
							break;
	            case KEY_DOWN:
	                        i++;
	                        if (i > 2)	//key_down pe "quit" duce pe "new game"
								i = 0;
	                        break;
	        }
		else
			//cand u=0, nu exista posibiliatea de a selecta optiunea RESUME (la inceput de jos sau dupa Game Over)
			switch(ch) {
			    case KEY_UP:
			        i--;
			        if (i < 0)
						i = 2;
					if (i == 1)
						i--;	
					break;
			    case KEY_DOWN:
	                i++;
	                if (i > 2)
						i = 0;
					if (i == 1)
						i++;        
				    break;
			    }
			    
        // evidentiaza urmatorul element din lista de optiuni
        wattron(w, A_STANDOUT);
        sprintf(a, "%-7s",  cuv[i]);
        mvwprintw(w, i + 1, 2, "%s", a);
        wattroff(w, A_STANDOUT);

		//pentru fiecare optiune aleasa lui k ii este atribuita o valoare
		if (ch == '\n' && i == 0)
			  k = 1;		//New Game
		else
			if (ch == '\n' && i == 2)
				k = 2;		//Quit
			else
				if (ch == '\n' && i == 1)
					k = 3;		//Resume
	}
	
	if (k == 1) {		//New Game
		//sunt eliminate ferestrele meniului
		delwin(w);
		delwin(z);
		erase();
		refresh();
		
		//este initilaizata o matrice de 4X4 care va reprezenta matricea jocului (initial tabla este goala)
		for (i = 0; i < 4; i++)
			for (j = 0; j < 4; j++)
				l[i][j] = 0;
		sco[0] = 0;    //scorul initial este 0;
		joc(l, sco);	//se intra in jocul propriu-zis
	}
	else
		if (k == 2) { //Quit
			//se inchid toate ferestrele
			delwin(w);
			delwin(z);
       		erase();
         	refresh();
         	endwin();   //este dezactivat modul ncurses
		}
		else 
			if(k == 3) {  //Resume; se intoarce in joc 
					
				delwin(w);
				delwin(z);
		       	erase();
			 	refresh();
				joc(copie, scor);          //(inainte de a iesi din joc, matricea k si scorul au fost salvate in copie, resprectiv scor" )     		
			}	
}

int joc(int k[4][4], int scor[1])   //jocul propriu-zis
{
	start_color();  //activarea culorilor
	keypad(stdscr, TRUE); //activarea tastaturii
    noecho();	// caracterele tastate nu sunt afisate pe ecran
    curs_set(FALSE);	//nu afiseaza cursorul	

    int x, y, i, j, q = 0, z, w, u, ch;
    int mar_anexa = 5;  
	int id[4][4], scorfake[1], copie[4][4],  undo[4][4], scorundo[1], undoundo[4][4], scorundoundo[1];
	
	//se formeaza ferestrele jocului
    getmaxyx(stdscr,y,x);
    WINDOW *joc = newwin(y-mar_anexa, x, 0, 0);   //fereastra principala
    WINDOW *anexa = newwin(mar_anexa, x, y-mar_anexa, 0); //anexa care va contine timpul, data si scorul
    WINDOW *a = newwin(22, 33, 6, 10); //tabla de joc
	mvwprintw(joc, 20,10,"To start press any key");
	
	//activarea tastaturii pentru tabla de joc si pentru fereastra principala
	keypad( a , TRUE); 
	keypad(joc, TRUE);
	
	//initializarea perechilor de culori si aplicarea acestora pe ferestre
	init_pair(2, COLOR_WHITE, COLOR_YELLOW);
	attron(COLOR_PAIR(2));
	wbkgd(anexa, COLOR_PAIR(2));
	attroff(COLOR_PAIR(2));
	init_pair(3, COLOR_YELLOW, COLOR_MAGENTA);
	attron(COLOR_PAIR(3));
	wbkgd(joc, COLOR_PAIR(3));
	attroff(COLOR_PAIR(3));
	
	//reactualizarea ferestrelor
	refresh();
	wrefresh(anexa);
	wrefresh(joc);
        
	while (ch != 'q') { 
 
		ch = wgetch(joc);  // ch= valoarea ascii a tastei apasate
		ch = tolower(ch);	//nu tine cont daca tasta apasa este majuscula sau nu
        wrefresh(anexa);
		
		//afisarea indicatiilor
		mvwprintw(joc, 1, 15, "2048                   Have fun!      ");
       	mvwprintw(anexa, 1, 2, "Score: ");
		mvwprintw(anexa, 1, 10, " %d ", scor[0] );
		mvwprintw(joc, 15, 53, "To move use the arrow keys");
		mvwprintw(joc, 17, 53, "Press 'q' to access the menu.");
		mvwprintw(joc, 19, 53, "Press 'u' for UNDO");
		
		//evidentierea ferestrelor
    	margini(joc);
    	margini(anexa);

		time_t rawtime;	
		time(&rawtime);

      	mvwprintw(anexa, 3, 2, ctime(&rawtime)); //afisarea timpului curent in anexa
		mvwprintw(anexa, 3, 18, "  2018     " );
		
		wrefresh(anexa);
		
		// desenarea casutelor tablei de joc
		matrice(a);
		wrefresh(a);
		wrefresh(anexa);
		
       	box(a, 0, 0);   // incadrarea tablei de joc

       	wrefresh(joc); 
		wrefresh(anexa); 
		wrefresh(a);
		
		//daca exista element diferit de 0 => nu a fost prima miscare
		for (i = 0; i < 4; i++)
			for (j = 0; j < 4; j++)
				if (k[i][j] != 0)
					q = 1; 
		if (q == 0)	{	//doar inaninte de prima miscare se genereaza 2 cifre
			generare_cifre(a, k);
			generare_cifre(a, k);
			q++;	
		}

		scorundo[0] = scor[0];  //se salveaza scorul inainte de a se efectua o mutare 

		print(a, k);  //afisarea tablei de joc dupa generare;
		wrefresh(a);
		
		/*
		La apasarea oricarei sagetute, se verifica mai intai daca mutarea este valida; daca da, se efectueaza mutarea, altfel, nu;
		Inainte de fiecare mutare se retine matricea,in vederea optiunii UNDO;
		Se afiseaza scorul si matricea actualizate dupa fiecare miscare;
		*/
		switch (ch) {
			case KEY_DOWN:
				copiaza(undo, k);
				scorundo[0] = scor[0];
				copiaza(id, k);
				ordonare_s(id);
				adunare_s(id, scorfake);
				if (compara(id,k) == 0)
					break;
				ordonare_s(k);
				adunare_s(k, scor); 
				mvwprintw(anexa, 1, 10, " %d ", scor[0]);
				wrefresh(anexa);
				generare_cifre(a, k);
				wrefresh(a);
				print(a, k);
				break;
			case KEY_UP:
				copiaza(undo, k);
				scorundo[0] = scor[0];
				copiaza(id, k);
				ordonare_w(id);
				adunare_w(id, scorfake);
				if (compara(id, k) == 0)
					break;
				ordonare_w(k);
				adunare_w(k, scor);
				mvwprintw(anexa, 1, 10, " %d ", scor[0]);
				wrefresh(anexa);
				generare_cifre(a, k);
				print(a, k);
				break;
			case KEY_RIGHT:
				copiaza(undo, k);
				scorundo[0] = scor[0];
				copiaza(id, k);
				ordonare_d(id);
				adunare_d(id, scorfake);
				if( compara(id, k) == 0)
					break;
				ordonare_d(k);
				adunare_d(k, scor);
				mvwprintw(anexa, 1, 10, " %d ", scor[0]);
				wrefresh(anexa);
				generare_cifre(a, k);
				print(a, k);
				break;
			case KEY_LEFT: 
				copiaza(undo, k);
				copiaza (id, k);
				ordonare_a(id);
				adunare_a(id, scorfake);
				if(compara(id, k) == 0)			
					break;
				ordonare_a(k);
				adunare_a(k, scor);
				mvwprintw(anexa, 1, 10, " %d ", scor[0]);
				wrefresh(anexa);
				generare_cifre(a, k);
				print(a, k);
				break;
			//in cazul selectiei UNDO, matricea curenta se intoarce la starea anterioara mutarii
			case 'u':
				copiaza(k, undoundo);
				scor[0] = scorundoundo[0];
				mvwprintw(anexa, 1, 10, " %d ", scor[0]);
				wrefresh(anexa);
				print(a, k);
				break;
			}
			
			copiaza(undoundo, undo); 
			scorundoundo[0] = scorundo[0];
			
			//Jocul se incheie cand s-a format numarul 2048 sau cand nu mai exista mutari valide
			for (i = 0; i < 4; i++)
				for (j = 0; j < 4; j++)
					if ( k[i][j] == 2048) {
						//Se afiseaza mesajul aferent
						getmaxyx(stdscr,z,w);
       					WINDOW *final=newwin(z, w, 0, 0);

						start_color();
						init_pair(9, COLOR_GREEN, COLOR_BLACK);
						attron(COLOR_PAIR(9));
   						wbkgd(final, COLOR_PAIR(9));
    					attroff(COLOR_PAIR(9));
						refresh();

						mvwprintw(final, 17, 26, "You won!!!");
						mvwprintw(final, 19,23,"Press 'q' for menu." );
		
						wrefresh(final);
						delwin(final);	
						u = 1;	
					}
					
			if ( (mutare_optima(k)) == 1) {
				//Se afiseaza mesajul aferent
				getmaxyx(stdscr,z,w);
				WINDOW *final=newwin(z, w, 0, 0);
				
				start_color();
				init_pair(9, COLOR_GREEN, COLOR_BLACK);
				attron(COLOR_PAIR(9));
				wbkgd(final, COLOR_PAIR(9));
				attroff(COLOR_PAIR(9));
				refresh();
				
				mvwprintw(final, 16, 30, "Game over" );
				mvwprintw(final, 17,22,"Apasa 'q' pentru a accesa meniul." );
				
				wrefresh(final);
				delwin(final);	
				u = 0;	
			}
			else 
				u = 1;
	}     	
	copiaza(copie, k);  //inainte de a accesameniul se saleaza matricea k si scorul in vederea optiunii RESUME
   	meniu (u, copie, scor);	
	return 0;
}


int main() 
{	
	int u = 0, x[4][4], y[1];
	initscr();
	noecho(); 
   	curs_set(0); 
	meniu(u, x, y);  //jocul incepe cu meniul;
	endwin();

	return 0;
}
