#include <stdio.h>
#include <dirent.h>
#include <ftw.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

void pn(){
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

int size;
typedef struct FTW FTW;
void nftw(char *pathF, int (*fn)(const char*, const struct stat*, int, struct FTW*), int i, int flag);

int fn(const char *pathF, const struct stat *stat_, int tflag, struct FTW *ftwS) {
  	if (tflag == FTW_F ) {
		FILE *file;
		long fileSize;
	int check;

	    char pFile[PATH_MAX];
	    realpath(pathF, pFile);
	    file = fopen(pathF, "r");
	    check = fseek(file, 0, SEEK_END); 
		if (check == -1) exit (1);
	    fileSize = fseek(file, 0, SEEK_CUR);
		if (fileSize == -1) exit(1);
		
	    if(fileSize <= size ){
	      	printf("\nSciezka bezwzgledna: %s\nRozmiar: %ld B\nCzas od ostatniej modyfikacji: %ld\n",pathF, fileSize, stat_->st_mtim.tv_sec);
			printf("\nPrawa dostepu:\n-");
			if(S_IRUSR & stat_->st_mode) pr();
				else pn();

			if(S_IWUSR & stat_->st_mode) pw();
				else pn();

			if(S_IXUSR & stat_->st_mode) px();
				else pn();

			if(S_IRGRP & stat_->st_mode)pr();		//grupa
				else pn();

			if(S_IWGRP & stat_->st_mode) pw();
				else pn();

			if(S_IXGRP & stat_->st_mode) px();
				else pn();

			if(S_IROTH & stat_->st_mode) pr();     //inni
				else pn();

			if(S_IWOTH & stat_->st_mode) pw();
				else pn();

			if(S_IXOTH & stat_->st_mode) px();
				else pn();
	    	}
		printf("\n\n");
		fclose(file);
 	}
  	return 0;
}
//Rekurencyjnie przeszukuje drzewo katalogów z korzeniem w dirpath i wywołuje funkcję fn dla każdego pliku/katalogu
//nopenfd specifies the maximum number of directories that ftw() will hold open simultaneously
//int nftw(const char *dirpath, int (*fn) (const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf), int nopenfd, int flags) 
void printFiles(char *path) {
  nftw(path, fn, 30, FTW_F);
}

int main(int argc, char *argv[]) {
	if (argc != 3 || argv[2] < 0) {
		printf("\nERROR\n[sciezka do katalogu] [rozmiar pliku]\n");
		exit(1);
	}
	size = atoi(argv[2]);	
	printFiles(argv[1]);

	return 0;
}
