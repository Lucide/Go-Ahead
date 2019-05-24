//Cavasin Riccardo

#include<stdio.h>
#include<stdlib.h>
#include<termios.h>
#include<unistd.h>
#include<time.h>

int getch(){
	struct termios oldattr,newattr;
	int ch;
	tcgetattr(STDIN_FILENO,&oldattr);
	newattr=oldattr;
	newattr.c_lflag&=~(ICANON|ECHO);
	tcsetattr(STDIN_FILENO,TCSANOW,&newattr);
	ch=getchar();
	tcsetattr(STDIN_FILENO,TCSANOW,&oldattr);
	if(ch=='\033'){
		getch();
		ch=getch();
	}
return ch;
}
void sleepm(int milliseconds){
    clock_t time_end;
    time_end=clock()+milliseconds*CLOCKS_PER_SEC/1000;
    while(clock()<time_end);
}

int main(int argv, char**argc){
	int i;
	printf("please,\nin order to display the program correctly,\nmaximize the terminal or go fullscreen\nand set the text size to 10 [monospace].\n");
	getch();
	do{
		system("clear");
		printf("\n                                                    _       _                   _              _        _            _\n                                                   _\\ \\    /\\_\\               /\\ \\            /\\ \\     /\\ \\         /\\ \\\n                                                  /\\__ \\  / / /         _    /  \\ \\           \\ \\ \\   /  \\ \\____   /  \\ \\\n                                                 / /_ \\_\\ \\ \\ \\__      /\\_\\ / /\\ \\ \\          /\\ \\_\\ / /\\ \\_____\\ / /\\ \\ \\\n                                                / / /\\/_/  \\ \\___\\    / / // / /\\ \\ \\        / /\\/_// / /\\/___  // / /\\ \\_\\\n                                               / / /        \\__  /   / / // / /  \\ \\_\\      / / /  / / /   / / // /_/_ \\/_/\n                                              / / /         / / /   / / // / /    \\/_/     / / /  / / /   / / // /____/\\\n                                             / / / ____    / / /   / / // / /             / / /  / / /   / / // /\\____\\/\n                                            / /_/_/ ___/\\ / / /___/ / // / /________  ___/ / /__ \\ \\ \\__/ / // / /______\n                                           /_______/\\__\\// / /____\\/ // / /_________\\/\\__\\/_/___\\ \\ \\___\\/ // / /_______\\\n                                           \\_______\\/    \\/_________/ \\/____________/\\/_________/  \\/_____/ \\/__________/\n\n                                                                             .:PRESENTS:.\n\n                                                                     ╭─────────────────────────╮\n                                                                     │   ░░░░░░░░   ░░░░░░░░   │\n                                                                     │  ░▒░    ░▒░ ░▒░    ░▒░  │\n                                                                     │  ▒░▒        ▒░▒    ▒░▒  │\n                                                                     │  ░█░        ▒█▒    ▒░▒  │\n                                                                     │  ▒█▒   ▒█▒█ ▒█▒    ▒█▒  │\n                                                                    ╱   █▒█    █▒█ █▒█    █▒█   ╲\n                                                         ──────────╱     ████████   ████████     ╲──────────\n                                                         ╲  ______   __  __   ______   ______   _____     ╱\n                                                          │/\\  __ \\ /\\ \\_\\ \\ /\\  ___\\ /\\  __ \\ /\\  __-.  │\n                                                          │\\ \\  __ \\\\ \\  __ \\\\ \\  __\\ \\ \\  __ \\\\ \\ \\/\\ \\ │\n                                                          │ \\ \\_\\ \\_\\\\ \\_\\ \\_\\\\ \\_____\\\\ \\_\\ \\_\\\\ \\____- │ \n                                                         ╱   \\/_/\\/_/ \\/_/\\/_/ \\/_____/ \\/_/\\/_/ \\/____/  ╲\n                                                         ───────────────────────────────────────────────────\n                                                        ▕▁▁▁▁▁▁▁▁▁▂▃▄▅▆▇█▓▒░PHYSICS ENGINE░▒▓█▇▆▅▄▃▂▁▁▁▁▁▁▁▁▏\n                                                                                [DEMO]\n\n\n\n\n\n                                                                 .:PRESS ANY KEY TO LAUNCH, X TO EXIT:.\n\n");
		i=getch();
		system("chmod 777 ./urb/core32");
		system("chmod 777 ./urb/core64");
		if(i!='x'){
			system("./urb/core32");
			system("./urb/core64");
		}
	}while(i!='x');
	system("clear");
return 0;
}
