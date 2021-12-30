#include <3ds.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

double pi(int);
double portion(double);

int main(int argc, char **argv)
{
	gfxInitDefault();
	consoleInit(GFX_TOP, NULL);

	printf("Press A to choose accuracy\n\n");
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
			swkbdSetHintText(&swkbd, "Accuracy: ");
			button = swkbdInputText(&swkbd, mybuf, sizeof(mybuf));
		}

		if (didit)
		{
			if (button != SWKBD_BUTTON_NONE)
			{
				int P = atoi(mybuf);
				printf("\npi = %.*f\n",P,P,(double)(floor(pi(P)*pow(10,P))/pow(10,P)));
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
	long double val=3;
	int count=0, i=2; 
	long int previous,current;
	
	while(count!=3)   
	{	

		previous = floor(val*pow(10,a));

		val+=6*portion(i); i++;

		current = floor(val*pow(10,a));
	
		if(previous == current)
			count++;
		else
			count=0;
	}
	return val;
}


double portion(double n)
{

	if(n==2)
	{
		return ((pow((1/2.0),4)*(1/3.0)));
	}
	else
	{
		return ((((2*n)-3)*portion(n-1) * pow((1/2.0),2) * ((2*n)-3)/((2*n)-2))/((2*n)-1));
	}
}
