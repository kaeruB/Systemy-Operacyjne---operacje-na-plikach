#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>



void printChar (int file){
	int b, buf;
	off_t check;
	printf("\nWypisywanie znaku.\nKtory bajt chcesz wypisac?\n");
	scanf("%d", &b);
	check = lseek(file, b, SEEK_SET);
	if (check == -1) {
		printf("\nlseek error\n");
		exit(1);
	}
	read(file, &buf, 1);
	printf("\nWczytano: %c\n", buf);
}

void writeChar (int file) {
	int b, buf;
	off_t check;
	printf("\nZmiana znaku.\nPodaj, ktory bajt chcesz zmienic oraz na co chcesz zamienic podany znak?\n");
	scanf("%d %c", &b, &buf);
	check = lseek(file, b, SEEK_SET);
	if (check == -1) {
		printf("\nlseek error\n");
		exit(1);
	}
	ssize_t w = write(file, &buf, 1);
	if (w != 1){
		printf("\nERROR\nNie udalo sie zapisac znaku\n");
		exit(1);
	}
}

void freeLock (int file){
	int b;
	struct flock* fl;
	printf("\nFree lock\nPodaj, ktory bajt trzeba odblokowac.\n");
	scanf("%d",&b);
	fl = calloc(1, sizeof(struct flock));	//ustawienie pol struktury flock
	fl->l_type = F_UNLCK; 			//zwolnienie blokady
	fl->l_whence = SEEK_SET;		//poczatek zakresu		
	fl->l_start = b;			//offset od poczatku zakresu
	fl->l_len = 1;				//ilosc blokowanych bajtow
	int check = fcntl(file, F_SETLK, fl);   	//natychmiastowe zwolnienie albo zwrocenie -1
	if (check == -1) {
		printf("\nZwolnienie blokady nie jest mozliwe\n");
		exit(1);	
	}
	free(fl);
}


void writeLock (int file) {
	int b;
	printf("\nWrite lock\nKtory bajt chcesz zaryglowac?\n");
	scanf("%d", &b);
	struct flock* fl = calloc(1, sizeof(struct flock));
	fl->l_type = F_WRLCK;			//blokada wylaczna 
	fl->l_whence = SEEK_SET;		//poczatek zakresu
	fl->l_len = 1;				//ilosc blokowanych bajtow
	fl->l_start = b;			//offset
	fl->l_pid = getpid();			//PID procesu uniemozliwiajacy zalozenie blokady
	int check = fcntl(file,F_SETLK,fl);	//natychmiastowa
	if (check == -1){
		printf("\nZalozenie blokady do zapisu nie jest mozliwe\n");
		exit(1);	
	}			
	free(fl);   												
}


void readLock (int file){
	int mode, b;
	printf("\nRead lock\nWybierz 0 jesli blokada ma nastapic natychmiastowo.\n");
	scanf("%d", &mode);
	printf("\nKtory bajt chcesz zaryglowac?\n");
	scanf("%d", &b);

	struct flock* fl = calloc(1,sizeof(struct flock));

	fl->l_type = F_RDLCK;			//blokada dzielona
	fl->l_whence = SEEK_SET;		//od poczatku
	fl->l_start = b;			//offset
	fl->l_pid = getpid();			//PID procesu uniemozliwiajacy zalozenie blokady
	fl->l_len = 1;				//1 bajt
	
	//F_SETLK - natychmiastowy, jak sie nie da to zwraca -1
	if (mode == 0){
		if (fcntl(file, F_SETLK, fl) == -1) {
			printf("\nNie udalo sie ustawic blokady.\n");
			exit(1);
		}
	}
	else fcntl(file, F_SETLKW, fl);		//czeka az bedzie mogl zalozyc blokade
}


void print (int file){
	printf("\nLista zablokowanych bajtow:\n");
	long int endOfFile = lseek(file,0,SEEK_END);
	int i = 0;
	struct flock* fl;
	//dopoki nie jest na koncu pliku ustaw na i-ty bit
	while(lseek(file, i, SEEK_SET) != endOfFile){		//wypisanie z blokada wylaczna
		fl = calloc(1, sizeof(struct flock));
		fl->l_type = F_WRLCK;			//blokada wylaczna
		fl->l_whence = SEEK_SET;
		fl->l_len = 1;
		fl->l_start = i;
		fcntl(file, F_GETLK, fl);		//przekazujemy strukture opisujaca blokade, 
					//dostajemy PID blokujacego procesu uniemozliwiajacego zakladanie blokady
					//lub info, o niemozliwosci zalozenia blokady
		if(fl->l_type != F_UNLCK){		//F_UNLOCK zwolnienie/brak blokady, jesli nie to znaczy ze jest blokada
			printf("- write lock na %d bicie o id: %d\n", i,fl->l_pid);
		}
		free(fl);
		i++;
	}
	i = 0;
	while(lseek(file, i, SEEK_SET) != endOfFile) {		//z blokada dzielona
		fl = calloc(1, sizeof(struct flock));
		fl->l_type = F_RDLCK;			//blokada dzielona
		fl->l_whence = SEEK_SET;
		fl->l_len = 1;
		fl->l_start = i;
		fcntl(file, F_GETLK, fl);
		if(fl->l_type != F_UNLCK){  
			printf("- read lock na %d bicie o id: %d\n", i, fl->l_pid);
		}
		free(fl);
		i++;
	}

}


void message(){
	printf("\n1 - wypisz znak z pliku\n2 - zamien znak w pliku\n3 - zwolnij blokade\n4 - zaloz blokade - read lock\n5 - zaloz blokade - write lock\n6 - wypisz zalozone blokady\n0 - zakoncz\n");
}

int main(int argc, char **argv){
	if (argc != 2) {
		printf("\nERROR\nPodaj nazwe pliku jako argument wywoalania.\n");
		exit(1);
	}

	int file = open(argv[1], O_RDWR);
	if (!(file > 0)){
		printf("\nERROR\nZla sciezka do pliku.");
	}
	int c = 1;
	
	while (c != 0){
		message();
		scanf("\n%c", &c);
		switch(c){
			case '1':
				printChar(file);
				break;
			case '2':
				writeChar(file);
				break;
			case '3':
				freeLock(file);
				break;
			case '4':
				readLock(file);
				break;
			case '5':
				writeLock(file);
				break;
			case '6':
				print(file);
				break;
			case '0':
				close(file);
				return 0;
		}
		
	}
}

