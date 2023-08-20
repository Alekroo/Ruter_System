	#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARRAYSIZE 200

struct Ruter
{
	unsigned char ruterID;	
	unsigned char FLAGG;
	unsigned char pmSize; //produsent/modell 
	char pmStr[253];
};
struct Ruter ruterArr[ARRAYSIZE];
int antallRutere = 0;

struct Ruter *ruter_map(char ID)
{
	int i = 0;
	while(i < ARRAYSIZE){
		if(ruterArr[i].ruterID == ID)
		{
			return &ruterArr[i];
		}
		i++;
	}

	return NULL;
}

int read_file(char *filename)
{

	FILE *fp = fopen(filename, "r");
	unsigned char buf[400] = {0};
	int num[4];	
	int v = 0;

    if (fp == NULL) {
        printf("Could not open %s\n", filename);
        return 1;
    }

	fread(num, 4, 1, fp);	
	antallRutere = num[0];
    fseek(fp,1,SEEK_CUR);

 	int ruterNR = 0	;
    while(ruterNR < antallRutere)
    {	
    	struct Ruter *rptr = malloc(sizeof *rptr);	

    	fread(buf, 1, 1, fp);	
    	rptr->ruterID = *buf;

    	fread(buf, 1, 1, fp);	
    	rptr->FLAGG = *buf;

	    fread(buf, 1, 1, fp);	
    	rptr->pmSize = *buf;
    	v = *buf;

	 	int i;
		for(i = 0; i < v; i++){
	 	 	fread(buf, 1, 1, fp);
	  	 	rptr->pmStr[i] = *buf;
		}
		ruterArr[ruterNR] = *rptr;
		ruterNR++;

	}	

    fclose(fp);

	printf("(Innlesing av filen vellykket)\n\n\n");
	return 0;
}

void print_menu()
{	printf("\n\n\n");
	printf("(Antall rutere registrert: %d)\n", antallRutere);
	printf("---------------------------------------------\n");
	printf("(1) Print ruterens info\n");
	printf("(2) Endre flagg for en ruter\n");
	printf("(3) Endre produsent/modell for en ruter\n");
	printf("(4) Legg til en ny ruter\n");
	printf("(5) Slett en ruter fra databasen\n");
	printf("(6) Avslutt programmet\n");
	printf("---------------------------------------------\n");
}

int regn_ut_antallendringer(int v)
{	
	int antallEndringer = 0;
  	int binearteller = 8;

  	int i;
  	for(i = 7; i > 3; i--) 
  	{
	  	if(('0' + ((v >> i) & 1)) == '0')
	  	{
	  		binearteller = binearteller/2;
	  	}	
	  	else
	  	{
	  		antallEndringer = antallEndringer + binearteller;
	  		binearteller = binearteller/2;
	  	}	
	}
	return antallEndringer;
}

void print_ruter_info(struct Ruter *rptr)
{	if(rptr->pmSize != 0){
		printf("\n\n---------------------------------------------\n");
		printf("Informasjon til ruter %d\n", rptr->ruterID);
		printf("Produsent/Modell: %s", rptr->pmStr);
		int v = rptr->FLAGG;

	  	int i; // for C89 compatability
	  	for(i = 3; i >= 0; i--) {
			if(i == 3)
			{
		  		if(('0' + ((v >> i) & 1) == '0'))
		  		{
		  			printf("D: I bruk\n");
		  		}
		  		else{
			  		printf("D: Ikke i bruk\n");
			  	}
			}
			else if(i == 2)
			{
			  	if(('0' + ((v >> i) & 1) == '0'))
			  	{
			  		printf("C: Støtter ikke 5GHz\n");
			  	}
			  	else{
			  		printf("C: Støtter 5GHz\n");
			  	}
			}
			else if(i == 1)
			{
			  	if(('0' + ((v >> i) & 1) == '0'))
			  	{
			  		printf("B: Ikke trådløs\n");
			  	}
			  	else
			  	{
			  		printf("B: Trådløs\n");
			  	}
			}
			else if(i == 0)
			{
			  	if(('0' + ((v >> i) & 1) == '0'))
			  	{
			  		printf("A: Ikke aktiv\n");
			  	}
			  	else{
			  		printf("A: Aktiv\n");
			  	}
			}
	  	}
	  	printf("Har blitt endret %d gang(er)\n", regn_ut_antallendringer(v));
		printf("---------------------------------------------");
	}
	else{
		printf("Ruteren finnes ikke!\n");
	}
}

void endre_flagg(struct Ruter *rptr)
{
	char input = '0';
	int newFlagg = 0;
	int binaryCount = 1;

	int c;
	while ((c = getchar()) != '\n' && c != EOF); //scanf /n fjerner

	printf("Er ruteren aktiv? (y/n)\n");
	scanf(" %c",&input);	
	if(input == 'y')
	{
		newFlagg = newFlagg + binaryCount;
		binaryCount = binaryCount *2;
	}
	else
	{
		binaryCount = binaryCount *2;
	}

	printf("Er ruteren trådløs? (y/n)\n");
	scanf(" %c",&input);
	if(input == 'y')
	{
		newFlagg = newFlagg + binaryCount;
		binaryCount = binaryCount *2;
	}
	else
	{
		binaryCount = binaryCount *2;
	}

	printf("Støtter ruteren 5Ghz?(y/n)\n");
	scanf(" %c",&input);
	if(input == 'y')
	{
		newFlagg = newFlagg + binaryCount;
		binaryCount = binaryCount *2;
	}
	else
	{
		binaryCount = binaryCount *2;
	}

	printf("Er ruteren i bruk? (y/n)\n");
	scanf(" %c",&input);
	if(input == 'y')
	{
		newFlagg = newFlagg + binaryCount;
		binaryCount = binaryCount *2;
	}
	else
	{
		binaryCount = binaryCount *2;
	}
	
	int antallEndringer = regn_ut_antallendringer(rptr->FLAGG) + 1;
	antallEndringer=antallEndringer << 4;
	rptr->FLAGG = antallEndringer | newFlagg;}

void endre_promo(struct Ruter *rptr)
{
	char pmInput[254];

	int c;
	while ((c = getchar()) != '\n' && c != EOF); //scanf /n fjerner

	printf("Skriv inn det nye produsent/modell navnet: \n");
	fgets(pmInput, sizeof(pmInput), stdin);	
	strcpy(rptr->pmStr, pmInput);
	int antallEndringer = regn_ut_antallendringer(rptr->FLAGG) + 1;	
	antallEndringer=antallEndringer << 4;		//"dytter bort" de fire første "antallEndringer" verdienene 4 indekser bort

	int i = 0; 									
	for(i = 7; i > 3; i--){						//gjør de 4 siste bitene til 0
		rptr->FLAGG &= ~(1<<i);
	}

	rptr->FLAGG = antallEndringer | rptr->FLAGG;	//setter sammen den nye antallEndringer verdien med de første 4 bitsene av flagget
	rptr->pmSize = strlen(pmInput); 
}	

void ny_ruter(char ID)
{

		if(antallRutere < ARRAYSIZE)
		{
			int i;
			for(i = 0; i < ARRAYSIZE;i++)
			{
				if(ruterArr[i].pmSize == 0)
				{
					printf("Fant ledig plass på: %d\n", i);	
			    	struct Ruter *rptr = malloc(sizeof *rptr);	
			    	rptr->ruterID = ID;
			    	endre_flagg(rptr);

			    	endre_promo(rptr);

					int x = 0; 									
					for(x = 7; x > 3; x--){						//gjør de 4 siste flagg bitene til 0 (0000XXXX)
						rptr->FLAGG &= ~(1<<x);
					}

			    	ruterArr[i] = *rptr;
			    	antallRutere++;
			    	printf("\n*Ruteren ble registrert*\n");
			    	break;
				}
			}
		}
	
	else{
		printf("ID-en er allerede brukt...\n");
	}
}

void slett_ruter(struct Ruter *rptr)
{
	memset(rptr,0,sizeof(struct Ruter));
	printf("\n*Ruteren har blitt slettet*\n");
	antallRutere--;
}

void skriv_til_fil(char *filnavn)
{
   FILE *fp;
   unsigned char newLine = '\n';

   fp = fopen( filnavn , "w" );
   fwrite(&antallRutere , 1 , sizeof(antallRutere) , fp );

   fwrite(&newLine , 1 , sizeof(newLine) , fp );
   int x = 0;
   while(x < antallRutere){

   		fwrite(&ruterArr[x].ruterID,1,sizeof(ruterArr[x].ruterID),fp);
   		fwrite(&ruterArr[x].FLAGG,1,sizeof(ruterArr[x].FLAGG), fp);
   		fwrite(&ruterArr[x].pmSize,1,sizeof(ruterArr[x].pmSize), fp);

   		int i;
   		for (i = 0; i < ruterArr[x].pmSize; i++)
   		{
   			fwrite(&ruterArr[x].pmStr[i],1,sizeof(ruterArr[x].pmStr[i]),fp);
   		}

   		x++;
   	}

   fclose(fp);
}

int main(int argc, char *argv[])
{

	if (argc != 2)
	{
		printf("Oppgi ET fil som argument\n");
		return 0;
	}

	read_file(argv[1]); 

	unsigned char usrDigit;
	unsigned char usrInput;

	while(usrDigit != 6)
	{
		print_menu();
		scanf(" %hhu",&usrDigit);

		if(usrDigit == 1)
		{
			printf("Skriv inn ruter-ID:  ");
			scanf("%hhu",&usrInput);

			if(ruter_map(usrInput) == 0)
			{
				printf("Finner ikke ruteren med ID-en %hhu \n", usrInput);	
			}
			else{
				print_ruter_info(ruter_map(usrInput));
			}


		}
		else if(usrDigit == 2)
		{
			printf("Skriv inn ruter-ID: ");
			scanf("%hhu",&usrInput);
			if(ruter_map(usrInput) == 0)
			{
				printf("Finner ikke ruteren med ID-en %hhu\n", usrInput);	
			}
			else{
				endre_flagg(ruter_map(usrInput));
				printf("\n*Endringen har blitt registrert!*\n");
			}
		}	
		else if(usrDigit == 3)
		{
			printf("Skriv inn ruter-ID: ");
			scanf("%hhu",&usrInput);
			if(ruter_map(usrInput) == 0)
			{
				printf("Finner ikke ruteren med ID-en %hhu\n", usrInput);	
			}
			else{
				endre_promo(ruter_map(usrInput));
				printf("\n*Endringen har blitt registrert!*\n");
			}
		}	
		else if(usrDigit == 4)
		{
			if(antallRutere < ARRAYSIZE)
			{
				printf("Skriv inn ID til den nye ruteren(som ikke er brukt): ");
				scanf("%hhu",&usrInput);
				if(ruter_map(usrInput) == 0)
				{
					ny_ruter(usrInput);
				}
				else{
					printf("ID-en er allerede brukt!\n");
				}
			}	
			else
			{
				printf("Kan ikke legge til nye rutere fordi databasen er full\n");
			}
		}
		else if(usrDigit == 5)
		{
			if(antallRutere > 0)
			{
				printf("Skriv inn ID til ruteren du vil slette: ");
				scanf("%hhu",&usrInput);
				if(ruter_map(usrInput) != 0)
				{
					slett_ruter(ruter_map(usrInput));
				}
				else{
					printf("Finner ikke ruteren med ID-en %hhu\n", usrInput);	
				}
			}	
			else
			{
				printf("Databasen er tom!\n");
			}
		}
		else if(usrDigit == 6)
		{
			printf("Overskriver filen %s og avslutter programmet......\n", argv[1]);
			skriv_til_fil(argv[1]);	
			return 0;
		}
		else
		{
			printf("Ugyldig input...\n");
		}

	}
	return 0;
}