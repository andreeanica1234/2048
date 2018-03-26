NICA ANDREEA MARIA

		(!!) Deschiderea terminalul trebuie sa fie  pe tot ecranul.
	
	
		Jocul are la baza o tabla de joc sub forma unei matrice patratice cu 4 linii si 4 coloane, fiecare patrat fiind
	liber sau continand un numar intreg - o putere a lui 2. La inceperea jocului, toate patratele care compun tabla de joc
	sunt libere, cu exceptia a doua care pot contine una din valorile 2 sau 4, alegerea valorii fiind una aleatoare, precum
	si cea a pozitiilor in care sunt plasate aceste patrate pe tabla de joc.

		Patratele din tabla de joc care contin valori (nu sunt pozitii libere) se pot deplasa in directia impusa de
	jucator (nord, vest, sud sau est). In functie de directie, patratele din tabla de joc care contin valori  se vor deplasa astfel:
		• Se vor deplasa in ordinea in care se afla pe tabla de joc spre directia indicata , ocupand spatiile libere,
	pana se vor intalni cu alte patrate valide sau cu marginea tablei de joc;
		• Daca doua patrate valide cu acceasi valoare se intalnesc in timpul mutarii, ele se vor uni intr-un singur
	patrat care va avea ca valoare suma valorilor patratelor din care este compus;
		• Celula obtinuta prin alipire conform regulii de mai sus nu se poate uni cu o alta celula de acceasi
	valoare in aceasi mutare. Au prioritate de alipire patratele situate cat mai aproape de marginea indicata de
	directia de deplasare.
		
		In cazul in care nu se poate muta nicio celula valida, prin alegerea unei anumite directii, intr-un anumit
	moment al jocului, mutarea respectiva este considerata invalida.
		Dupa fiecare mutare valida, aplicatia trebuie sa adauge un nou patrat, avand valoarea 2 sau 4 si fiind pozitionat
	intr-o celula libera, alegerea pozitiei pe tabla de joc fiind una aleatoare. Daca nu mai exista directii care sa genereze mutari valide,
	jocul se termina.
		Daca pe tabla de joc se generezeaza in urma unei mutari o celula cu valoarea 2048 atunci jocul este castigat.
		
		Optiunea undo este valida doar pentru ultima mutare executata.
		
		Jucatorul are la dispozitie un meniu, de unde poate alege sa inceapa un joc nou, ce contine jocul curent sau sa iasa din joc.
		
		

	Compilare in linia de comanda: make
	Rulare: make run
	
