#include <3ds.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

double pi(int);
double parcela(double);

int main(int argc, char **argv)
{
	gfxInitDefault();
	consoleInit(GFX_TOP, NULL);

	printf("Pressione A para calcular pi\n\n");
	printf("\nSTART: exit\n");

	while (aptMainLoop())
	{
		hidScanInput();

		u32 kDown = hidKeysDown();

		if (kDown & KEY_START)
			break;

		static SwkbdState swkbd;
		static char mybuf[60];
		static SwkbdStatusData swkbdStatus;
		static SwkbdLearningData swkbdLearning;
		SwkbdButton button = SWKBD_BUTTON_NONE;
		bool didit = false;

		if (kDown & KEY_A)
		{
			didit = true;
			swkbdInit(&swkbd, SWKBD_TYPE_NUMPAD, 1, 8);
			swkbdSetPasswordMode(&swkbd, SWKBD_PASSWORD_HIDE_DELAY);
			swkbdSetValidation(&swkbd, SWKBD_ANYTHING, 0, 0);
			swkbdSetFeatures(&swkbd, SWKBD_FIXED_WIDTH);
			swkbdSetNumpadKeys(&swkbd, L'ツ', L'益');
			swkbdSetHintText(&swkbd, "Quantas casas? (Max 8)");
			button = swkbdInputText(&swkbd, mybuf, sizeof(mybuf));
		}

		if (didit)
		{
			if (button != SWKBD_BUTTON_NONE)
			{
				int P = atoi(mybuf);
				printf("\npi a %d casas decimais  = %.*f\n",P,P,(double)(floor(pi(P)*pow(10,P))/pow(10,P)));
//				printf("pi a %s casas decimais: \n", mybuf);
			} else
				printf("swkbd event: %d\n", swkbdGetResult(&swkbd));
		}

		// Flush and swap framebuffers
		gfxFlushBuffers();
		gfxSwapBuffers();

		gspWaitForVBlank();
	}

	gfxExit();
	return 0;
}


double pi(int a)
{
	//Todas as parcelas são multiplicadas por 6 baseando-se na propriedade de que a(b+c) = ab + ac e que, na fórmula dada, para obter o valor de pi,
	//a soma de todas as parcelas deve ser multiplicada por 6.

	long double val=3; //Valor inicial de pi a partir da primeira parcela (0.5 * 6)
	int cont=0, i=2; //i começa em 2 pois o calculo das parcelas começará a partir da segunda, visto que a primeira já possui o valor pré estabelescido
	long int casaAnterior,casaAtual; //Armazena como um número inteiro o valor de pi até a P-ésima casa decimal, dado por pi*10^P

	while(cont!=3)   //Calcula e soma as parcelas da série infinita indefinidamente até que a P-ésima casa decimal de pi(variavel val) permaneça inalterada
			//após a soma de 3 parcelas seguidas
	{	

		casaAnterior = floor(val*pow(10,a)); //Armazena o valor de pi até a P-ésima casa decimal como um inteiro antes e depois da soma de uma parcela

		val+=6*parcela(i); i++; //Calcula a i-ésima parcela da soma e soma ela ao resto


		casaAtual = floor(val*pow(10,a));
	
		if(casaAnterior == casaAtual) //Compara os valores obtidos antes e depois da soma de uma parcela e incrementa o contador caso sejam iguais
			cont++;
		else
			cont=0; //Reinicia o contador caso sejam diferentes
	}
	return val;
}


double parcela(double n)
{

	if(n==2)
	{
		return ((pow((1/2.0),4)*(1/3.0))); //Retorna o valor da segunda parcela da soma, a qual inicia o padrão seguido pelas parcelas seguintes
	}
	else
	{
		return ((((2*n)-3)*parcela(n-1) * pow((1/2.0),2) * ((2*n)-3)/((2*n)-2))/((2*n)-1));
		//Calcula cada uma das parcelas a partir da fórmula (2n-1)*f(n) = (2n-3)*f(n-1) * (1/2)^2 * (2n-3)/(2n-2)
	}
}
