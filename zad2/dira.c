#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <dirent.h>
#include <ftw.h>
#include <string.h>

void p(){
	printf("-");
}

void pw(){
	printf("w");
}

void pr(){
	printf("r");
}

void px(){
	printf("x");
}

void printFiles(char *file, int size) {
	struct stat buf; 					//info o plikach, podaza za dowiazaniami
	DIR *kat;
	kat = opendir(file);            //zwraca uchwyt	
	size_t l;
	char fileTab[FILENAME_MAX];
	struct dirent *nextElement;
	long fileSize;
	
	if (strlen(file) > FILENAME_MAX){
		printf("\nERROR\n Podana sciezka jest za dluga.\n");
		exit(1);
	}
	if (kat == NULL) {
		printf("\nERROR\n Zla sciezka.\n");
		exit(1);
	}

	stat(file, &buf); 					//uzyskanie informacji na temat file, zapisanie w buf
	
	l = strlen(file); 
	strcpy(fileTab, file); 				//skopiowanie nazwy pliku 
	fileTab[l++] = '/';             
	//readdir Zwraca wskaźnik do struktury dirent opisującej kolejny element znajdujący się w danym katalogu
	while ((nextElement = readdir(kat)) != NULL) {  
		if (!strcmp(nextElement->d_name, ".") || !strcmp(nextElement->d_name, "..")) continue;
		strncpy(fileTab + l, nextElement->d_name, FILENAME_MAX - l);
		stat(fileTab, &buf);                         //uzyskanie informacji na temat nazwy, zapisanie w buf
			
		if (S_ISDIR(buf.st_mode)) printFiles(fileTab,size);  //jesli plik jest katalogiem
		else if (S_ISREG(buf.st_mode) == 1) { 			//jesli jest plikiem reg.
			FILE *file2 = fopen(fileTab, "r"); 
			fseek(file2, 0, SEEK_END); 
			fileSize = ftell(file2);
		
			if(fileSize <= size ){   		//warunek z argumentu spelniony
				printf("\n[Plik]Sciezka bezwzgledna: %s/%s\nRozmiar: %ld B\nCzas od ostatniej modyfikacji: %ld\n", file, nextElement->d_name, fileSize, buf.st_mtim.tv_sec);
				printf("Prawa dostepu:\n-");

				if(S_IRUSR & buf.st_mode) pr();     //wlasciciel
				else p();

				if(S_IWUSR & buf.st_mode) pw();
				else p();

				if(S_IXUSR & buf.st_mode) px();
				else p();

				if(S_IRGRP & buf.st_mode) pr();		//grupa
				else p();

				if(S_IWGRP & buf.st_mode) pw();
				else p();

				if(S_IXGRP & buf.st_mode) px();
				else p();

				if(S_IROTH & buf.st_mode) pr();     //inni
				else p();

				if(S_IWOTH & buf.st_mode) pw();
				else p();

				if(S_IXOTH & buf.st_mode) px();
				else p();
			}
			printf("\n\n\n");
			fclose(file2);
		}		
	}
  	closedir(kat); 	
}

int main(int argc, char *argv[]) {
	if (argc != 3 || argv[2] < 0) {
		printf("\nERROR\n[sciezka do katalogu] [rozmiar pliku]\n");
		exit(1);
	}
	int size = atoi(argv[2]);
	char *path = argv[1];

	printFiles(path,size);

	return 0;
}
