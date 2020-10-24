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
int konami(int in,int i);

void clear();
void initialize(int mna[][35],int v[][35]);
void phy(int mna[][35],int v[][35]);
void flud(int *x,int *y,int i,int mna[][35],int v[][35]);
void slid(int x,int y,int i,int mna[][35]);
void move(int mna[][35]);
void render(int mna[][35],int v[][35],int ste);

int main(int argv, char**argc){
	srand(time(NULL));
	int mna[90][35],v[90][35];
	initialize(mna,v);
	move(mna);
	phy(mna,v);
return 0;
}

void initialize(int mna[][35],int v[][35]){
	int x,y;

	for(y=0;y<35;y=y+1){
		for(x=0;x<90;x=x+1){
			mna[x][y]=0;
		}
	}
	for(x=0;x<90;x=x+1)
		mna[x][0]=3;
	for(y=0;y<34;y=y+1){
		mna[0][y]=3;
		mna[89][y]=3;
	}
	for(x=0;x<90;x=x+1)
		mna[x][34]=3;
	for(y=0;y<35;y=y+1){
		for(x=0;x<90;x=x+1)
			v[x][y]=0;
	}
}

void phy(int mna[][35],int v[][35]){
	int x,y,i,t=1;

	while(1){
		i=rand()%2;
		for(y=33;y>-1;y=y-1){
			if(!i)
				x=89;
			else
				x=0;
			while(x>-1&&x<90){
				switch(mna[x][y]){
					case 1:
						flud(&x,&y,i,mna,v);
					break;
					case 2:
						slid(x,y,i,mna);
					break;
					case 4:
					case 5:
						if(t){
							if(mna[x][y+1]==0)
								mna[x][y+1]=mna[x][y]-3;
						}
					break;
				}
				if(!i)
					x=x-1;
				else
					x=x+1;
			}
		}
		render(mna,v,1);
		sleepm(70);
		t=1-t;
	}
}

void flud(int *x,int *y,int i,int mna[][35],int v[][35]){
	int m,n;

	if(mna[*x][*y+1]==0){
		mna[*x][*y]=mna[*x][*y+1];
		mna[*x][*y+1]=1;
		v[*x][*y+1]=v[*x][*y];
		v[*x][*y]=0;
	}
	else{
		if(v[*x][*y]==0){
			m=0;
			n=0;
			if(mna[*x-1][*y]==0)
				m=1;
			if(mna[*x+1][*y]==0)
				n=1;
			if(m==1&&n==1){
				m=rand()%2;
				n=n-m;
			}
			if(m==1){
				mna[*x][*y]=mna[*x-1][*y];
				mna[*x-1][*y]=1;
				v[*x-1][*y]=1;
				if(!i)
					*x=*x-1;
			}
			if(n==1){
				mna[*x][*y]=mna[*x+1][*y];
				mna[*x+1][*y]=1;
				v[*x+1][*y]=2;
				if(i)
					*x=*x+1;
			}
		}
		else{
			switch(v[*x][*y]){
				case 1:
					if(mna[*x-1][*y]==0){
						mna[*x][*y]=mna[*x-1][*y];
						v[*x][*y]=0;
						mna[*x-1][*y]=1;
						v[*x-1][*y]=1;
						if(!i)
							*x=*x-1;
					}
					else
						v[*x][*y]=2;
				break;
				case 2:
					if(mna[*x+1][*y]==0){
						mna[*x][*y]=mna[*x+1][*y];
						v[*x][*y]=0;
						mna[*x+1][*y]=1;
						v[*x+1][*y]=2;
						if(i)
							*x=*x+1;
					}
					else
						v[*x][*y]=1;
				break;
			}
		}
	}
}

void slid(int x,int y,int i,int mna[][35]){
	int m,n;

	if(mna[x][y+1]<2){
		mna[x][y]=mna[x][y+1];
		mna[x][y+1]=2;
	}
	else{
		m=0;
		n=0;
		if(mna[x-1][y+1]<2&&mna[x-1][y]<2)
			m=1;
		if(mna[x+1][y+1]<2&&mna[x+1][y]<2)
			n=1;
		if(m==1&&n==1){
			m=rand()%2;
			n=n-m;
		}
		if(m==1){
			mna[x][y]=mna[x-1][y];
			mna[x-1][y]=2;
		}
		if(n==1){
			mna[x][y]=mna[x+1][y];
			mna[x+1][y]=2;
		}
	}
}

void move(int mna[][35]){
	int x=42,y=17,t,in,i=0;

	t=mna[x][y];
	mna[x][y]=6;
	do{
		render(mna,0,1);
		in=getch();
		switch(in){
			case 'A':	//w
				mna[x][y]=t;
				if(y-1==0)
					y=33;
				else
					y=y-1;
				t=mna[x][y];
				mna[x][y]=6;
			break;
			case 'B': 	//s
				mna[x][y]=t;
				if(y+1==34)
					y=1;
				else
					y=y+1;
				t=mna[x][y];
				mna[x][y]=6;
			break;
			case 'C':	//d
				mna[x][y]=t;
				if(x+1==89)
					x=1;
				else
					x=x+1;
				t=mna[x][y];
				mna[x][y]=6;
			break;
			case 'D':	//a
				mna[x][y]=t;
				if(x-1==0)
					x=88;
				else
					x=x-1;
				t=mna[x][y];
				mna[x][y]=6;
			break;
			case ' ':
				mna[x][y]=0;
				t=0;
			break;
			case 'w':
				mna[x][y]=1;
				t=1;
			break;
			case 'r':
				mna[x][y]=2;
				t=2;
			break;
			case 's':
				mna[x][y]=3;
				t=3;
			break;
			case 'd':
				mna[x][y]=4;
				t=4;
			break;
			case 'f':
				mna[x][y]=5;
				t=5;
			break;
		}
		i=konami(in,i);
	}while(in!='x');
	mna[x][y]=t;
}

void render(int mna[][35],int v[][35],int ste){
	int x,y;
	system("clear");
	printf("\n\n                                       ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n");
	for(y=0;y<35;y=y+1){
		switch(y){
			case 6:
				printf("        ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫");
			break;
			case 7:
				printf("        ┃       ◅ ISTRUCTIONS ▻        ┃");
			break;
			case 8:
				printf("        ┠───────────┬──────────────────┨");
			break;
			case 9:
				printf("        ┃ ◁ PRESS ▷ │      ◁ TO ▷      ┃");
			break;
			case 10:
				printf("        ┃           │                  ┃");
			break;
			case 11:
				printf("        ┃    Ⓦ      │  Put a block of  ┃");
			break;
			case 12:
				printf("        ┃           │      WATER       ┃");
			break;
			case 13:
				printf("        ┃    Ⓡ      │  Put a block of  ┃");
			break;
			case 14:
				printf("        ┃           │      ROCK        ┃");
			break;
			case 15:
				printf("        ┃    Ⓢ      │  Put a block of  ┃");
			break;
			case 16:
				printf("        ┃           │      STEEL       ┃");
			break;
			case 17:
				printf("        ┃    Ⓓ      │  Put a WATER TAP ┃");
			break;
			case 18:
				printf("        ┃           │                  ┃");
			break;
			case 19:
				printf("        ┃    Ⓕ      │  Put a ROCK TAP  ┃");
			break;
			case 20:
				printf("        ┃           │                  ┃");
			break;
			case 21:
				printf("        ┃   space   │  Clear the block ┃");
			break;
			case 22:
				printf("        ┃           │                  ┃");
			break;
			case 23:
				printf("        ┃    Ⓧ      │     Run the      ┃");
			break;
			case 24:
				printf("        ┃           │    SIMULATION    ┃");
			break;
			case 25:
				printf("        ┃  ctrl+C   │       EXIT       ┃");
			break;
			case 26:
				printf("        ┗━━━━━━━━━━━┷━━━━━━━━━━━━━━━━━━┫");
			break;
			case 27:
				printf("                                ⓛⓤⓒⓘⓓⓔ ┃");
			break;
			default:
				printf("                                       ┃");
			break;
		}
		for(x=0;x<90;x=x+1){
			switch(mna[x][y]){
				case 0:
					printf(" ");
				break;
				case 1:
					printf("░");
				break;
				case 2:
					printf("█");
				break;
				case 3:
					printf("#");
				break;
				case 4:
					printf("△");
				break;
				case 5:
					printf("▲");
				break;
				case 6:
					printf("◊");
				break;
			}
		}	
		switch(y){
			case 3:
				printf("┣━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n");
			break;
			case 4:
				printf("┃       ◅ ISTRUCTIONS ▻        ┃        \n");
			break;
			case 5:
				printf("┠───────────┬──────────────────┨\n");
			break;
			case 6:
				printf("┃ ◁ PRESS ▷ │      ◁ TO ▷      ┃\n");
			break;
			case 7:
				printf("┃           │                  ┃\n");
			break;
			case 8:
				printf("┃     ⇑     │    To move up    ┃\n");
			break;
			case 9:
				printf("┃           │                  ┃\n");
			break;
			case 10:
				printf("┃     ⇓     │   To move down   ┃\n");
			break;
			case 11:
				printf("┃           │                  ┃\n");
			break;
			case 12:
				printf("┃     ⇐     │   To move left   ┃\n");
			break;
			case 13:
				printf("┃           │                  ┃\n");
			break;
			case 14:
				printf("┃     ⇒     │   To move right  ┃\n");
			break;
			case 15:
				printf("┣━━━━━━━━━━━┷━━━━━━━━━━━━━━━━━━┛\n");
			break;
			case 18:
				printf("┃         ᖗ KONAMI CODE ᖘ\n");
			break;
			case 20:
				printf("┃  █▀▀▀▀▀▀▀███▀████▀█▀▀▀██▀▀▀▀▀▀▀█\n");
			break;
			case 21:
				printf("┃  █ █▀▀▀█ █▀▄█▀█▄ ▄▀▄█ ▀█ █▀▀▀█ █\n");
			break;
			case 22:
				printf("┃  █ █   █ ██ ██ █▀▀██  ▄█ █   █ █\n");
			break;
			case 23:
				printf("┃  █ ▀▀▀▀▀ █▀█ ▄ ▄▀▄ █ ▄▀█ ▀▀▀▀▀ █\n");
			break;
			case 24:
				printf("┃  █▀▀▀█▀▀▀▀ ██▄▀ ██  ▄▄▀▀▀███▀███\n");
			break;
			case 25:
				printf("┃  █▀ ██▄█▀▀▄▄▀   ▀█▄█▀█▀ ▄█▀▀▀▀ █\n");
			break;
			case 26:
				printf("┃  █ █▀  ▄▀ ▄▄ ▀▄ ▀ ▀ █▄▄▀ ██▀█▄▀█\n");
			break;
			case 27:
				printf("┃  █  █▀█▀▀ ▀▀ ▄█▄▀█ ▄▄█ ▀ █▀▄▀▄ █\n");
			break;
			case 28:
				printf("┃  █ █▄ █▀▀▄ ▀▄▄▀ ███ ▀█ ▀ ▄█▄█▄▀█\n");
			break;
			case 29:
				printf("┃  █▀▄█ ▀▀▀▀▀▄▀  ▄▀█ ▄▀█▄ ▄▄█▄▀▀ █\n");
			break;
			case 30:
				printf("┃  █▀▄  ██▀▄▀█▀▀▄▄ ▄▀ █▄   ▀ ██▀▀█\n");
			break;
			case 31:
				printf("┃  █▀▀▀▀▀▀▀█ ▄ ▄█▀██▀██  █▀█ █▀▀ █\n");
			break;
			case 32:
				printf("┃  █ █▀▀▀█ █ ▄█▄▀██▄▄ ██ ▀▀▀ █████\n");
			break;
			case 33:
				printf("┃  █ █   █ █▀▄█  ▄▀█ ▄█ █▄█  ▀▄▄ █\n");
			break;
			case 34:
				printf("┃  █ ▀▀▀▀▀ █ ▀▀▀▄▄ █  ▄  ▄▀▀ ▀█ ▀█\n");
			break;
			default:
				printf("┃\n");
			break;
		}
	}
	printf("                                       ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛  ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀\n");
}

int konami(int in,int i){
	switch(i){
		case 0:
			if(in=='A')
				i=i+1;
			else
				i=0;
		break;
		case 1:
			if(in=='A')
				i=i+1;
			else
				i=0;
		break;
		case 2:
			if(in=='B')
				i=i+1;
			else
				i=0;
		break;
		case 3:
			if(in=='B')
				i=i+1;
			else
				i=0;
		break;
		case 4:
			if(in=='D')
				i=i+1;
			else
				i=0;
		break;
		case 5:
			if(in=='C')
				i=i+1;
			else
				i=0;
		break;
		case 6:
			if(in=='D')
				i=i+1;
			else
				i=0;
		break;
		case 7:
			if(in=='C')
				i=i+1;
			else
				i=0;
		break;
		case 8:
			if(in=='b')
				i=i+1;
			else
				i=0;
		break;
		case 9:
			if(in=='a'){
				getch();
				for(i=0;i<25;i=i+1){
					printf("\n\n                                              ███████ ████        ███████████         █████████        █████ ████████\n                                            ███████  ███████████████████ █████       █████ ██████████████████  ████████\n                                           ██ ███   ██████████████████  ███████████████████  █████████████████   ████ ██\n                                           █ ████   █████████ ███████  █████████████████████  ███████ ████████   █████ █\n                                              ████   ██████    ████  █████████████████████████  ████    █████   █████\n                                                 ███████        █        █████████████████        █        ███████\n                                           █                                ███████████                                █\n                                           ██                         ███       ███       ███                         ██\n                                           ████               █     ██████████       ██████████     █               ███\n                                           ████████       ███████   ██████████       ██████████   ███████       ████████\n                                            ███████████████████████   ██                   ██    ██████████████████████\n                                                       ██████████              █████              ██████████\n                                              ██████████████████              ███ ███              ██████████████████\n                                                   ████████████              ███   ███              ████████████\n                                                 ██████████████     ███                     ███     ██████████████\n                                                      ██████████  ██████        ███        ██████  ██████████\n                                              ███████████████████████████    █       █    ██████████████████████████████\n                                                     ████████████████████       ███       ████████████████████\n                                                  ███████████████████████  █ ██ ███ ██ █  ███████████████████████\n                                                      ███████████████████  ██ ███████ ██  ███████████████████\n                                                            ██████████████ █████████████ ██████████████\n\n          ▄▄▄█████▓ ██░ ██  ██▓  ██████     ██▓  ██████     ▄▄▄       ███▄    █    ▓█████  ▄▄▄        ██████ ▄▄▄█████▓▓█████  ██▀███     ▓█████   ▄████   ▄████\n         ▓  ██▒ ▓▒▓██░ ██▒▓██▒▒██    ▒    ▓██▒▒██    ▒    ▒████▄     ██ ▀█   █    ▓█   ▀ ▒████▄    ▒██    ▒ ▓  ██▒ ▓▒▓█   ▀ ▓██ ▒ ██▒   ▓█   ▀  ██▒ ▀█▒ ██▒ ▀█▒\n         ▒ ▓██░ ▒░▒██▀▀██░▒██▒░ ▓██▄      ▒██▒░ ▓██▄      ▒██  ▀█▄  ▓██  ▀█ ██▒   ▒███   ▒██  ▀█▄  ░ ▓██▄   ▒ ▓██░ ▒░▒███   ▓██ ░▄█ ▒   ▒███   ▒██░▄▄▄░▒██░▄▄▄░\n         ░ ▓██▓ ░ ░▓█ ░██ ░██░  ▒   ██▒   ░██░  ▒   ██▒   ░██▄▄▄▄██ ▓██▒  ▐▌██▒   ▒▓█  ▄ ░██▄▄▄▄██   ▒   ██▒░ ▓██▓ ░ ▒▓█  ▄ ▒██▀▀█▄     ▒▓█  ▄ ░▓█  ██▓░▓█  ██▓\n           ▒██▒ ░ ░▓█▒░██▓░██░▒██████▒▒   ░██░▒██████▒▒    ▓█   ▓██▒▒██░   ▓██░   ░▒████▒ ▓█   ▓██▒▒██████▒▒  ▒██▒ ░ ░▒████▒░██▓ ▒██▒   ░▒████▒░▒▓███▀▒░▒▓███▀▒\n           ▒ ░░    ▒ ░░▒░▒░▓  ▒ ▒▓▒ ▒ ░   ░▓  ▒ ▒▓▒ ▒ ░    ▒▒   ▓▒█░░ ▒░   ▒ ▒    ░░ ▒░ ░ ▒▒   ▓▒█░▒ ▒▓▒ ▒ ░  ▒ ░░   ░░ ▒░ ░░ ▒▓ ░▒▓░   ░░ ▒░ ░ ░▒   ▒  ░▒   ▒\n             ░     ▒ ░▒░ ░ ▒ ░░ ░▒  ░ ░    ▒ ░░ ░▒  ░ ░     ▒   ▒▒ ░░ ░░   ░ ▒░    ░ ░  ░  ▒   ▒▒ ░░ ░▒  ░ ░    ░     ░ ░  ░  ░▒ ░ ▒░    ░ ░  ░  ░   ░   ░   ░\n           ░       ░  ░░ ░ ▒ ░░  ░  ░      ▒ ░░  ░  ░       ░   ▒      ░   ░ ░       ░     ░   ▒   ░  ░  ░    ░         ░     ░░   ░       ░   ░ ░   ░ ░ ░   ░\n                   ░  ░  ░ ░        ░      ░        ░           ░  ░         ░       ░  ░      ░  ░      ░              ░  ░   ░           ░  ░      ░       ░\n");
					sleepm(60);
					system("clear");
					printf("\n\n                                                     █                                                      █\n                                                   ██                   █                 █                  ██\n                                             █   ███                  ██                   ██                 ███    █\n                                            █    ███         █       ██                     ██       █        ███     █\n                                           ███    ███      ████    ██                         ██    ████     ███     ███\n                                           ██████       ████████ ████████                 ████████ ████████       ██████\n                                            ████████████████████████████████           ████████████████████████████████\n                                             █████████████████████████   ███████   ███████   █████████████████████████\n                                               ███████████████ █████          ███████          █████ ███████████████\n                                                   ███████       ███          ███████          ███       ███████\n                                                                   ███  ███████████████████  ████\n                                                                 ██████████████     ██████████████\n                                                                ██████████████   █   ██████████████\n                                                               ██████████████   ███   ██████████████\n                                                               █████   █████████████████████   █████\n                                                                ██      ████████   ████████      ██\n                                                                         ████ ███████ ████\n                                                                         ███████   ███████\n                                                                         ██ █  █   █  █ ██\n                                                                         ██  █       █  ██\n                                                                          █             █\n\n          ▄▄▄█████▓ ██░ ██  ██▓  ██████     ██▓  ██████     ▄▄▄       ███▄    █    ▓█████  ▄▄▄        ██████ ▄▄▄█████▓▓█████  ██▀███     ▓█████   ▄████   ▄████\n         ▓  ██▒ ▓▒▓██░ ██▒▓██▒▒██    ▒    ▓██▒▒██    ▒    ▒████▄     ██ ▀█   █    ▓█   ▀ ▒████▄    ▒██    ▒ ▓  ██▒ ▓▒▓█   ▀ ▓██ ▒ ██▒   ▓█   ▀  ██▒ ▀█▒ ██▒ ▀█▒\n         ▒ ▓██░ ▒░▒██▀▀██░▒██▒░ ▓██▄      ▒██▒░ ▓██▄      ▒██  ▀█▄  ▓██  ▀█ ██▒   ▒███   ▒██  ▀█▄  ░ ▓██▄   ▒ ▓██░ ▒░▒███   ▓██ ░▄█ ▒   ▒███   ▒██░▄▄▄░▒██░▄▄▄░\n         ░ ▓██▓ ░ ░▓█ ░██ ░██░  ▒   ██▒   ░██░  ▒   ██▒   ░██▄▄▄▄██ ▓██▒  ▐▌██▒   ▒▓█  ▄ ░██▄▄▄▄██   ▒   ██▒░ ▓██▓ ░ ▒▓█  ▄ ▒██▀▀█▄     ▒▓█  ▄ ░▓█  ██▓░▓█  ██▓\n           ▒██▒ ░ ░▓█▒░██▓░██░▒██████▒▒   ░██░▒██████▒▒    ▓█   ▓██▒▒██░   ▓██░   ░▒████▒ ▓█   ▓██▒▒██████▒▒  ▒██▒ ░ ░▒████▒░██▓ ▒██▒   ░▒████▒░▒▓███▀▒░▒▓███▀▒\n           ▒ ░░    ▒ ░░▒░▒░▓  ▒ ▒▓▒ ▒ ░   ░▓  ▒ ▒▓▒ ▒ ░    ▒▒   ▓▒█░░ ▒░   ▒ ▒    ░░ ▒░ ░ ▒▒   ▓▒█░▒ ▒▓▒ ▒ ░  ▒ ░░   ░░ ▒░ ░░ ▒▓ ░▒▓░   ░░ ▒░ ░ ░▒   ▒  ░▒   ▒\n             ░     ▒ ░▒░ ░ ▒ ░░ ░▒  ░ ░    ▒ ░░ ░▒  ░ ░     ▒   ▒▒ ░░ ░░   ░ ▒░    ░ ░  ░  ▒   ▒▒ ░░ ░▒  ░ ░    ░     ░ ░  ░  ░▒ ░ ▒░    ░ ░  ░  ░   ░   ░   ░\n           ░       ░  ░░ ░ ▒ ░░  ░  ░      ▒ ░░  ░  ░       ░   ▒      ░   ░ ░       ░     ░   ▒   ░  ░  ░    ░         ░     ░░   ░       ░   ░ ░   ░ ░ ░   ░\n                   ░  ░  ░ ░        ░      ░        ░           ░  ░         ░       ░  ░      ░  ░      ░              ░  ░   ░           ░  ░      ░       ░\n");
					sleepm(60);
					system("clear");
				}
				printf("TheMadHatter: \"Have I Gone Mad?\"\n");
				getch();
				printf("Alice: \"I'm afraid so. You're entirely bonkers.");
				getch();
				printf("\n        But I'll tell you a secret.");
				getch();
				printf(" All the best people are.\"\n");
				getch();
				printf("\n        ...starting...\n");
				system("sleep 2");
				system("clear");
				printf("#include<stdio.h>\n#include<stdlib.h>\n#include<termios.h>\n#include<unistd.h>\n#include<time.h>\n\nint getch(){\n	struct termios oldattr,newattr;\n	int ch;\n	tcgetattr(STDIN_FILENO,&oldattr);\n	newattr=oldattr;\n	newattr.c_lflag&=~(ICANON|ECHO);\n	tcsetattr(STDIN_FILENO,TCSANOW,&newattr);\n	ch=getchar();\n	tcsetattr(STDIN_FILENO,TCSANOW,&oldattr);\n	if(ch=='\\033'){\n		getch();\n		ch=getch();\n	}\nreturn ch;\n}\nvoid sleepm(int milliseconds){\n    clock_t time_end;\n    time_end=clock()+milliseconds*CLOCKS_PER_SEC/1000;\n    while(clock()<time_end);\n}\n\nvoid clear();\nvoid initialize(int mna[][35],int v[][35]);\nvoid phy(int mna[][35],int v[][35]);\nvoid flud(int *x,int *y,int i,int mna[][35],int v[][35]);\nvoid slid(int x,int y,int i,int mna[][35]);\nvoid move(int mna[][35]);\nvoid render(int mna[][35],int v[][35],int ste);\n\nint main(int argv, char**argc){\n	srand(time(NULL));\n	int mna[90][35],v[90][35];\n	initialize(mna,v);\n	move(mna);\n	phy(mna,v);\nreturn 0;\n}\n\nvoid initialize(int mna[][35],int v[][35]){\n	int x,y;\n\n	for(y=0;y<35;y=y+1){\n		for(x=0;x<90;x=x+1){\n			mna[x][y]=0;\n		}\n	}\n	for(x=0;x<90;x=x+1)\n		mna[x][0]=3;\n	for(y=0;y<34;y=y+1){\n		mna[0][y]=3;\n		mna[89][y]=3;\n	}\n	for(x=0;x<90;x=x+1)\n		mna[x][34]=3;\n	for(y=0;y<35;y=y+1){\n		for(x=0;x<90;x=x+1)\n			v[x][y]=0;\n	}\n}\n\nvoid phy(int mna[][35],int v[][35]){\n	int x,y,i,t=1;\n\n	while(1){\n		i=rand()%2;\n		for(y=33;y>-1;y=y-1){\n			if(!i)\n				x=89;\n			else\n				x=0;\n			while(x>-1&&x<90){\n				switch(mna[x][y]){\n					case 1:\n						flud(&x,&y,i,mna,v);\n					break;\n					case 2:\n						slid(x,y,i,mna);\n					break;\n					case 4:\n					case 5:\n						if(t){\n							if(mna[x][y+1]==0)\n								mna[x][y+1]=mna[x][y]-3;\n						}\n					break;\n				}\n				if(!i)\n					x=x-1;\n				else\n					x=x+1;\n			}\n		}\n		render(mna,v,1);\n		sleepm(70);\n		t=1-t;\n	}\n}\n\nvoid flud(int *x,int *y,int i,int mna[][35],int v[][35]){\n	int m,n;\n\n	if(mna[*x][*y+1]==0){\n		mna[*x][*y]=mna[*x][*y+1];\n		mna[*x][*y+1]=1;\n		v[*x][*y+1]=v[*x][*y];\n		v[*x][*y]=0;\n	}\n	else{\n		if(v[*x][*y]==0){\n			m=0;\n			n=0;\n			if(mna[*x-1][*y]==0)\n				m=1;\n			if(mna[*x+1][*y]==0)\n				n=1;\n			if(m==1&&n==1){\n				m=rand()%2;\n				n=n-m;\n			}\n			if(m==1){\n				mna[*x][*y]=mna[*x-1][*y];\n				mna[*x-1][*y]=1;\n				v[*x-1][*y]=1;\n				if(!i)\n					*x=*x-1;\n			}\n			if(n==1){\n				mna[*x][*y]=mna[*x+1][*y];\n				mna[*x+1][*y]=1;\n				v[*x+1][*y]=2;\n				if(i)\n					*x=*x+1;\n			}\n		}\n		else{\n			switch(v[*x][*y]){\n				case 1:\n					if(mna[*x-1][*y]==0){\n						mna[*x][*y]=mna[*x-1][*y];\n						v[*x][*y]=0;\n						mna[*x-1][*y]=1;\n						v[*x-1][*y]=1;\n						if(!i)\n							*x=*x-1;\n					}\n					else\n						v[*x][*y]=2;\n				break;\n				case 2:\n					if(mna[*x+1][*y]==0){\n						mna[*x][*y]=mna[*x+1][*y];\n						v[*x][*y]=0;\n						mna[*x+1][*y]=1;\n						v[*x+1][*y]=2;\n						if(i)\n							*x=*x+1;\n					}\n					else\n						v[*x][*y]=1;\n				break;\n			}\n		}\n	}\n}\n\nvoid slid(int x,int y,int i,int mna[][35]){\n	int m,n;\n\n	if(mna[x][y+1]<2){\n		mna[x][y]=mna[x][y+1];\n		mna[x][y+1]=2;\n	}\n	else{\n		m=0;\n		n=0;\n		if(mna[x-1][y+1]<2&&mna[x-1][y]<2)\n			m=1;\n		if(mna[x+1][y+1]<2&&mna[x+1][y]<2)\n			n=1;\n		if(m==1&&n==1){\n			m=rand()%2;\n			n=n-m;\n		}\n		if(m==1){\n			mna[x][y]=mna[x-1][y];\n			mna[x-1][y]=2;\n		}\n		if(n==1){\n			mna[x][y]=mna[x+1][y];\n			mna[x+1][y]=2;\n		}\n	}\n}\n\nvoid move(int mna[][35]){\n	int x=42,y=17,t,in;\n\n	t=mna[x][y];\n	mna[x][y]=6;\n	do{\n		render(mna,0,1);\n		in=getch();\n		switch(in){\n			case 'A':	//w\n				mna[x][y]=t;\n				if(y-1==0)\n					y=33;\n				else\n					y=y-1;\n				t=mna[x][y];\n				mna[x][y]=6;\n			break;\n			case 'B': 	//s\n				mna[x][y]=t;\n				if(y+1==34)\n					y=1;\n				else\n					y=y+1;\n				t=mna[x][y];\n				mna[x][y]=6;\n			break;\n			case 'C':	//d\n				mna[x][y]=t;\n				if(x+1==89)\n					x=1;\n				else\n					x=x+1;\n				t=mna[x][y];\n				mna[x][y]=6;\n			break;\n			case 'D':	//a\n				mna[x][y]=t;\n				if(x-1==0)\n					x=88;\n				else\n					x=x-1;\n				t=mna[x][y];\n				mna[x][y]=6;\n			break;\n			case ' ':\n				mna[x][y]=0;\n				t=0;\n			break;\n			case 'w':\n				mna[x][y]=1;\n				t=1;\n			break;\n			case 'r':\n				mna[x][y]=2;\n				t=2;\n			break;\n			case 's':\n				mna[x][y]=3;\n				t=3;\n			break;\n			case 'd':\n				mna[x][y]=4;\n				t=4;\n			break;\n			case 'f':\n				mna[x][y]=5;\n				t=5;\n			break;\n		}\n	}while(in!='x');\n	mna[x][y]=t;\n}\n\nvoid render(int mna[][35],int v[][35],int ste){\n	itward\n}\n\n");
				while(1);
			}
			else
				i=0;
		break;
	}
return i;
}
