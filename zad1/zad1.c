#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/times.h>

void errorMessage () {
	printf("\nERROR\n");
	printf("lib/sys   shuffle/generate/sort   file_name   ile_rekordow   dl_rekordu ");
}


void shuffleSys(char *fileName , int recordNum, int recordLeng) {
  	int fileHandler = open(fileName, O_RDWR);
	char* record1 = (char*) calloc(recordLeng, sizeof(char));
	char* record2 = (char*) calloc(recordLeng, sizeof(char));
	int currentRecord = 0;
	int random;
	ssize_t tmp;
	off_t check;
	long int endOfFile;
	
	if (lseek(fileHandler, 0, SEEK_SET) != 0 || fileHandler < 0) {
		printf("\nBlad otwierania pliku\n");
		exit(1);
	}		

	if (record1 == NULL || record2 == NULL) {
		printf("\nBlad przy alokacji pamieci\n");
		exit(1);
	}
	
	check = lseek(fileHandler, 0, SEEK_END);
	if (check == -1){
		printf("\nlseek error\n");
		exit(1);
	}
	endOfFile = lseek(fileHandler, 0, SEEK_CUR);
	if (endOfFile == -1){
		printf("\nlseek error\n");
		exit(1);
	}
	check = lseek(fileHandler, 0, SEEK_SET);
	if (check == -1){
		printf("\nlseek error\n");
		exit(1);
	}

	for (int i = 0; i < recordNum; i++) {
		lseek(fileHandler, 0, SEEK_SET);
		if (lseek(fileHandler, 0, SEEK_CUR) < endOfFile) {			
			tmp = read(fileHandler, record1, recordLeng); 
			if (tmp != recordLeng) exit(1);
		}
		random = rand()%recordNum;
		check = lseek(fileHandler, recordLeng * random, SEEK_SET);
		if (check == -1){
			printf("\nlseek error\n");
			exit(1);
		}
		tmp = read(fileHandler, record2, recordLeng);
		if (tmp != recordLeng) exit(1);
		check = lseek(fileHandler, recordLeng * currentRecord, SEEK_SET);
		if (check == -1){
			printf("\nlseek error\n");
			exit(1);
		}		
		tmp = write(fileHandler, record2, recordLeng);
		if (tmp != recordLeng) exit(1);
		check = lseek(fileHandler, recordLeng * random, SEEK_SET);
		if (check == -1){
			printf("\nlseek error\n");
			exit(1);
		}		
		tmp = write(fileHandler, record1, recordLeng);
		if (tmp != recordLeng) exit(1);
	}
}



void shuffleLib(char* file, int recordNum, int recordLeng)
{
	FILE* fileHandler;
	char* record1 = (char*) calloc(recordLeng, sizeof(char));
	char* record2 = (char*) calloc(recordLeng, sizeof(char));
	int random;
	int currentRecord = 0; 
	long int endOfFile;
	int pom;  //fseek zwraca 0 jak sie udalo
	size_t check;  //zwraca ilosc itemow poprawnie freadowanych albo fwritowanych
		
	
	fileHandler = fopen(file, "r+w");
	if (fileHandler == NULL || fseek(fileHandler, 0, SEEK_SET) != 0) {
		printf("\nNie mozna otworzyc pliku\n");
		exit(1);
	}
	
	pom = fseek(fileHandler, 0, SEEK_END);
	if (pom != 0) {
		printf ("\nfseek error\n");
		exit(1);
	}
	endOfFile = ftell(fileHandler);
	pom = fseek(fileHandler, 0, SEEK_SET);
	if (pom != 0) {
		printf ("\nfseek error\n");
		exit(1);
	}
	
	if (record1 == NULL || record2 == NULL) {
		printf("\nPamiec nie zostala poprawnie zaalokowana.\n");
		exit(1);
	}

	for (int i = 0; i < recordNum; i++) {
		pom = fseek(fileHandler, 0, SEEK_SET);
		if (pom != 0) {
			printf ("\nfseek error\n");
			exit(1);
		}
		if (ftell(fileHandler) < endOfFile) {			
			check = fread(record1, recordLeng, 1, fileHandler);
			if (check != 1) exit(1);
		}
		random = rand()%recordNum;
		pom = fseek(fileHandler, recordLeng * random, SEEK_SET);
		if (pom != 0) {
			printf ("\nfseek error\n");
			exit(1);
		}
		check = fread(record2, recordLeng, 1, fileHandler);
		if (check != 1) exit(1);
		pom = fseek(fileHandler, recordLeng * currentRecord, SEEK_SET);	
		if (pom != 0) {
			printf ("\nfseek error\n");
			exit(1);
		}
		check = fwrite(record2, recordLeng, 1, fileHandler);
		if (check != 1) exit(1);
		pom = fseek(fileHandler, recordLeng * random, SEEK_SET);
		if (pom != 0) {
			printf ("\nfseek error\n");
			exit(1);
		}		
		check = fwrite(record1, recordLeng, 1, fileHandler);
		if (check != 1) exit(1);
	}
}


void sortSys(char *fileName,int recordNum, int recordLeng){
	int file;
	off_t lseekCheck;
	long int endOfFile;
	ssize_t w1, w2, check;
	char* record1;
	char* record2;
	off_t offset;
	
	file = open (fileName, O_RDWR);  //zwraca nr deskryptora pliku //open zwraca -1 jesli error
	if (file == -1 || lseek(file, 0, SEEK_SET) != 0){ 
		printf("/nNie mozna otworzyc pliku.\n");
		exit (1);
	}
	//lseek zwraca polozenie wskaznika po przesunieciu, On error, the value (off_t) -1 is returned
	lseekCheck = lseek(file, 0, SEEK_END);
	endOfFile = lseek(file, 0, SEEK_CUR);
	if (lseekCheck == -1 || endOfFile == -1) {
		printf("\nlseek error\n");
		exit(1);
	}
	lseekCheck = lseek(file, 0, SEEK_SET);  //powrot na poczatek
	if (lseekCheck == -1) {
		printf("\nlseek error\n");
		exit(1);
	}

	record1 = (char*) calloc(recordLeng, sizeof(char));	
	record2 = (char*) calloc(recordLeng, sizeof(char));

	if (record1 == NULL || record2 == NULL){
		printf("\nBlad przy alokacji pamieci\n");
		exit(1);
	}

						//read zwraca ilosc wczytanych bajtow i jako argument bierze ile bajtow wczytac
						//przesuwa sie o ilosc wczytanych bajtow

	for (int i = 0; i < recordNum; i++){
		offset = 0;
		for (int j = 0; j < recordNum - 1; j++){
			lseekCheck = lseek (file, offset, SEEK_SET);
			if (lseekCheck == -1) {
				printf("\nlseek error\n");
				exit(1);
			}
			check = read (file, record1, recordLeng);
			if (check != recordLeng) exit(1);
			check = read (file, record2, recordLeng);
			if (check != recordLeng) exit(1);
			
			if (strcmp(record1, record2) > 0) {
				lseekCheck = lseek (file, offset, SEEK_SET);
				if (lseekCheck == -1) {
					printf("\nlseek error\n");
					exit(1);
				}
				w1 = write(file, record2, recordLeng);
				w2 = write(file, record1, recordLeng);
				if (w1 != recordLeng || w2 != recordLeng){
					printf("\nNie udalo sie zapisac rekordu.\n");
					exit (1);
				}
			}
			offset += recordLeng;
		}		
	}

	free(record1);
	free(record2);
	close(file);
}


void sortLib(char *fileName, int recordNum, int recordLeng){
	FILE* file;
	char* record1;
	char* record2;
	int check;
	long int endOfFile;
	size_t readCheck;
	size_t fwrite1, fwrite2;
	long int offset;
	
	
	file = fopen(fileName, "r+w");					//jak error to zwraca null
	if (file == NULL || fseek(file, 0, SEEK_SET) != 0){
		printf("/nNie mozna otworzyc pliku lub wskaznik odczytu nie wskazuje na poczatek./n");
		exit(1);
	}
	//fseek -1 jesli error
	check = fseek(file, 0, SEEK_END);
	if (check == -1){
		printf("\nfseek error\n");
		exit(1);
	}
	endOfFile = ftell(file);
	if (endOfFile == -1){
		printf("\nftell error\n");
		exit(1);
	}
	check = fseek(file, 0, SEEK_SET);
	if (check == -1){
		printf("\nfseek error\n");
		exit(1);
	}

	record1 = (char*) calloc(recordLeng, sizeof(char));	
	record2 = (char*) calloc(recordLeng, sizeof(char));
	
	if (record1 == NULL || record2 == NULL){
		printf("\nBlad przy alokacji pamieci\n");
		exit(1);
	}
	

	for (int i = 0; i < recordNum; i++){
		offset = 0;
		for (int j = 0; j < recordNum - 1; j++){
			check = fseek (file, offset, SEEK_SET);
			if (check == -1){
				printf("\nfseek error\n");
				exit(1);
			}
			
			readCheck = fread(record1, recordLeng, 1, file);
				if (readCheck != 1) exit(1);
			readCheck = fread(record2, recordLeng, 1, file);
				if (readCheck != 1) exit(1);
				
			if (strcmp(record1, record2) > 0) {
				check = fseek(file, offset, SEEK_SET);
				if (check == -1){
					printf("\nfseek error\n");
					exit(1);
				}
				
				fwrite1 = fwrite(record2, recordLeng, 1, file);
				fwrite2 = fwrite(record1, recordLeng, 1, file);
				if (fwrite1 != 1 || fwrite2 != 1) {
					printf("\nNie udalo sie zapisac rekordu.\n");
					exit (1);
				}
			}
			offset += recordLeng;
		}		
	}

	free(record1);
	free(record2);
	fclose(file);

}


void generate(char* fileName,int recordNum, int recordLeng){
	FILE* wFile;
	FILE* frand;
	char* buff;
	int rCounter = 0;
	size_t writtenRecordsNum;
	
	
	wFile = fopen(fileName, "w+");
	frand = fopen("/dev/urandom", "r");
	buff = (char*) calloc(recordNum * recordLeng, sizeof(char));
		
	if (wFile == NULL || frand == NULL) {
		printf("/nNie mozna otworzyc pliku/n");
		exit(1);
	}
		
	if (buff == NULL){
		printf("\nBlad przy alokacji pamieci\n");
		exit(1);
	}

	while (rCounter < recordNum){
		for (int i = 0; i < recordLeng; i++) buff[i] = (char) getc(frand);
		if (buff == NULL){
			printf("/nRekord nie zostal wczytany/n");
			exit (1);
		}
		writtenRecordsNum = fwrite (buff, sizeof(char), recordLeng, wFile);
		if (writtenRecordsNum != recordLeng){
			printf("/nBlad podczas zapisu do pliku./n");
			exit(1);		
		}
		rCounter++;
	}
	free(buff);
	fclose(frand);
	fclose(wFile);
}

void showTimes (char* msg, struct tms* start, struct tms* stop) {
	printf ("\n%s\nUsr time: %ld  ; Sys time: %ld \n", msg, stop->tms_utime-start->tms_utime, stop->tms_stime-start->tms_stime);
}


int main(int argc, char *argv[])
{

	struct tms *startTimeBuf = (struct tms*) malloc (sizeof(struct tms));
	struct tms *stopTimeBuf = (struct tms*) malloc (sizeof(struct tms));


	if (argc != 6) {
  		errorMessage();
  		exit (1);
  	}
	int recordNum, recordLeng;
	if (argc == 6) {
  		recordNum = atoi(argv[4]);
  		recordLeng = atoi(argv[5]);
	  	if (strcmp(argv[1],"sys") != 0 && strcmp(argv[1],"lib") != 0) {
	    		errorMessage();
	    		exit (1);
	   	}
	  	if (strcmp(argv[2],"shuffle") != 0 && strcmp(argv[2],"sort") && strcmp(argv[2],"generate") != 0){
	      		errorMessage();
	      		exit (1);
	      	}

		if(strcmp(argv[2],"shuffle") == 0) {
			if (strcmp(argv[1],"sys") == 0) {
				times(startTimeBuf);
				shuffleSys(argv[3],recordNum, recordLeng);
				times(stopTimeBuf);
				showTimes ("SHUFFLE SYS", startTimeBuf, stopTimeBuf);
			}
			if (strcmp(argv[1],"lib") == 0){
				times(startTimeBuf);
				shuffleLib(argv[3],recordNum, recordLeng);
				times(stopTimeBuf);	
				showTimes ("SHUFFLE LIB", startTimeBuf, stopTimeBuf);
			}
		}
		if(strcmp(argv[2],"sort") == 0) {
			if (strcmp(argv[1],"sys") == 0) {
				times(startTimeBuf);
		    		sortSys(argv[3],recordNum, recordLeng);
				times(stopTimeBuf);
				showTimes ("SORT SYS", startTimeBuf, stopTimeBuf);	
			}
			if (strcmp(argv[1],"lib") == 0){
				times(startTimeBuf);
				sortLib(argv[3],recordNum, recordLeng);
				times(stopTimeBuf);
				showTimes ("SORT LIB", startTimeBuf, stopTimeBuf);	
			}

		}

		if(strcmp(argv[2],"generate") == 0) 
			generate(argv[3],recordNum, recordLeng);	

	}
	//tms trzeba zwolnic ???
	return 0;
}
