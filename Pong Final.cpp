// pong version 12.01
//Changed menu
#include "graphics.h"
#include<stdio.h>
#include<time.h>/* For Time count */
#include<windows.h> /* for audible sound */
#include<math.h> /* For Time printing */

void about();
void background();
void ball_clear();
void ball_create();
void ball_limit_control();
void ball_movement();
void bat_clear();
void bat_create();
void create_menu();
void draw_triangle(int x,int y,int z);  // x,y is the point where the sharp end will be, z+10 is the size of the triangle
void first_page();
void game_over();
void hexagon(int x, int y);
int last_page();
void menu();
void pause();
void read_from_user(char *str);
void score();
void score_card();
void score_check(int n, int p);
void score_menu();
void select_menu(int x , int y);
void settings_menu();
void start_pong();
int time();

/* Screen Controls */

#define screen_top 0  /*top line of the screen.it will always be zero, otherwise we will have to change all co-ordinates*/
int screen_left=75;  /*start of black screen*/
int screen_right=675;  /*end of black screen*/
int screen_bottom=480; /*bottom of black screen*/
int screen_end=screen_right+screen_left;  /* end line of screen...value of screen_end should be a multiple of 6 */
int screen_mid=(screen_end)/2; /*middle of the screen, where the net is */


/* GamePlay Settings */

int high_score=1;//set the highest score
#define D_E 15
#define D_H 10
#define SPEED 15 // serve speed
int mode=3; // Mode 1 = Survival, Mode 2 = Time Challenge
int TIME= 30,x=0,timez=0,timey=1; // Time limit for time challenge, timey - timez =1 means one second has elapsed
time_t start_t, end_t;
char timex[10];

/* Game Basics */

int  bat_speed=90;  /*speed of the BAT can be changed*/
#define bat_size 120   /*size of the BAT can be changed*/
#define bat_width 10
#define bat_pos_left screen_left+50
#define bat_pos_right screen_right-60
#define ball_size_x 10
#define ball_size_y 30 // for rectangular ball
int ball_y=screen_bottom/2,ball_x=screen_mid,input_key,left_baty,right_baty,f=1,g,l,r,g_o=0;// all variables are declared here
int ball_radius=8;
int prex=screen_left+10,prey=screen_bottom+ball_radius;
int dev=SPEED,det,de,i,ball_speed_x=7,ball_speed_y=7;	//Ball speed variables.dev=ball Speed(delay),det=lower limit of delay, SPEED = higher limit of delay, ball_speed_x=x axis wise speed,ball_speed_y=y axis wise speed
int bat_pause_y= 0,pausey=0,quit=0;

/*Menu */
#define FONT SANS_SERIF_FONT
#define FONT_SIZE 4
#define GREY COLOR(105,105,105)
#define MENU_C COLOR(240,255,240)
#define score_col WHITE
#define ball_col WHITE
int bat_col=WHITE;
int menu_x1, menu_x2,menu_x3,menu_y1,menu_y2,menu_ymid,menu_ytop;
int SOUND=55; // Sets the sounds when the ball misses the bat
int score_pos=2,score_back=GREY;
int midx,midy,m,flag,screen_x=getmaxwidth(),screen_y;//m=number of top scores available,flag sets the controls when we enter the game for the first time 
int p1[550],p2[550],dif[5][550]; /* Leaderboard array */
char left_score[3],right_score[3],choice,control_l,l_u,l_d;	//choice decides if the player will continue or not]
//left_up prints the up key for left player & l_u stores the up key for left player
int left_bat_up, left_bat_down,	time_flag=0; // Used for Time-Trial Mode
char top[50][10]; /* Generates the Leaderboard */
// value of f & g controls the limit of the motion of ball leftward-rightwards & ups-down respectively
// l & r for the score of left & right side respectively...g_o=1 means game over.

FILE *score_list=fopen("score log.txt","a");

int main( )
{
	l=0,r=0;
    initwindow(getmaxwidth(), getmaxheight(), "Pong Edition 6.8");
	screen_x=getmaxx();
	screen_y=getmaxy();
	midx = screen_x/ 2;
	midy = screen_y / 2;
	menu_x1=(int)(screen_x/3);
	menu_x2=(int)((2*screen_x)/3);
	menu_x3=screen_x;
	menu_y1=(int)(200+(screen_y-200)/2);
	menu_y2=screen_y;
	menu_ymid=(int) ((screen_y-200)/4);
	menu_ytop=200+ (screen_y-200)/8;
	while(1)
	{
		if(screen_right-screen_left>550)
			det=D_H;
		else
			det=D_E;
		first_page();
		delay(1000);
		cleardevice();
		setbkcolor(BLACK);
		background();
		prex=screen_left+10,prey=screen_bottom+ball_radius;
		time(&start_t);
		while(1)
		{	
				setcolor(WHITE);
				setlinestyle(DASHED_LINE,3,1);
				line(screen_mid,screen_top,screen_mid,screen_bottom);

				//ball_clear();

				ball_movement(); //movement of ball

				bat_create();
				if(quit==1)
					goto gamer;
		
				bat_clear();

				ball_create();

				delay(dev);

				ball_limit_control();

				score();

				if(l==high_score || r==high_score)
					game_over();
				else if(mode==2)
				{
					time();
					time_t timer=end_t-start_t; // returns the time between start and current time
					if(time()==1) // returns 1 when time is up
					{
						game_over();
					}
					timey=timez;//previous timez
					timez=TIME-timer; // countdown timer
					sprintf(timex,"%d",timez);
					// We will print the timer once every second, so timey-timez==1 
					if(score_pos==2 &&( timey-timez==1 ||timez==TIME) ) /* score at middle,so timer at bottom */
					{
						setcolor(score_back);
						setfillstyle(SOLID_FILL,score_back);
						bar(0,screen_bottom/2+75,screen_left,screen_bottom);
						bar(screen_right,screen_bottom/2+75,screen_end,screen_bottom); 
						setbkcolor(score_back);
						settextjustify(CENTER_TEXT, CENTER_TEXT);
						setcolor(COLOR(255,165,0));
						settextstyle(3,HORIZ_DIR,4);
						outtextxy(screen_left/2,screen_bottom-100,timex);
						outtextxy(screen_end-(screen_left/2),screen_bottom-100,timex);
					}
					else if( timey-timez==1 ||timez==TIME)  /* score not at middle  , so timer at Middle */
					{
						setcolor(score_back);
						setfillstyle(SOLID_FILL,score_back);
						bar(0,screen_bottom/2-100,screen_left,screen_bottom/2+100);
						bar(screen_right,screen_bottom/2-100,screen_end,screen_bottom/2+100); 
						setbkcolor(score_back);
						settextjustify(CENTER_TEXT, CENTER_TEXT);
						setcolor(COLOR(255,165,0));
						settextstyle(3,HORIZ_DIR,4);
						outtextxy(screen_left/2,screen_bottom/2,timex);
						outtextxy(screen_end-(screen_left/2),screen_bottom/2,timex);
					}

					settextjustify(CENTER_TEXT, CENTER_TEXT);
				}
				gamer:
				if(quit==1)
				{
					last_page();
					quit=0; //  Quitting is complete
					l=0,r=0; // initialize the scores to 0
					g_o=1; // so that quit works as Game Over
					ball_x=screen_mid; // initialize the ball to the middle, so the next game starts from the middle
				}
				if(g_o==1)
				{
					x=0;
					time(&start_t);
					if(choice==1)
					{
						g_o=0;
						menu();
						cleardevice();
						setbkcolor(BLACK);
						background();
					}
					else if(choice==2)
					{	
						cleardevice();
						g_o=0;
						score();
						setbkcolor(BLACK);
						background();
					}

					else if(choice==3)
					{	
						g_o=0;
						cleardevice();
						settings_menu();
						setbkcolor(BLACK);
						background();
					}
				}
		}
	}
	system("pause");
    return 0;
}

void about() /* Add more Instructions */
{
	int x,y;
	start_pong();
	setcolor(COLOR(255,165,50));
	setfillstyle(SOLID_FILL,COLOR(255,165,0));
	fillellipse(screen_left/2,215+(menu_ymid/2),10,10);
	fillellipse(screen_left/2,215+menu_ymid,10,10);
	fillellipse(screen_left/2,215+(menu_ymid*3)/2,10,10);
	setcolor(COLOR(240,232,200));
	setfillstyle(SOLID_FILL,COLOR(240,232,170));
	draw_triangle(screen_left/2+10,212+menu_ymid*2,3);
	draw_triangle(screen_left/2+10,212+(menu_ymid*5)/2,3);
	draw_triangle(screen_left/2+10,212+menu_ymid*3,3);
	settextjustify(LEFT_TEXT, TOP_TEXT);
	setcolor(GREY);
	settextstyle(FONT,HORIZ_DIR,3);
	outtextxy(screen_left,200+menu_ymid/2,"Trivia: Pong is considered- the Mother of all Arcade Games");
	outtextxy(screen_left,200+menu_ymid,"You can set the Game Point from the Menu");
	outtextxy(screen_left,200+(menu_ymid*3)/2,"Instructions -");
	outtextxy(screen_left+20,200+menu_ymid*2,"Historically,The only instruction to play the game is: Avoid missing ball for high score.");
	outtextxy(screen_left+20,200+(menu_ymid*5)/2,"Time Challenge: The Higher scorer in the Time limit wins the game");
	outtextxy(screen_left+20,200+menu_ymid*3,"Press ESC to Pause and Resume the Game");
	
	settextjustify(LEFT_TEXT, TOP_TEXT);
	settextstyle(FONT,HORIZ_DIR,3);
	int backx=textwidth("BACK");
	int backy=textheight("BACK");
	setfillstyle(SOLID_FILL,GREY);
	bar(screen_x/2-backx/2-10,200+(7*menu_ymid)/2-10,screen_x/2+backx/2+10,200+(7*menu_ymid)/2+backy+10);
	
	setbkcolor(GREY);
	setcolor(WHITE);
	outtextxy(screen_x/2-backx/2,200+(7*menu_ymid)/2,"BACK");
	again:
	clearmouseclick(WM_LBUTTONDOWN);
	while(!ismouseclick(WM_LBUTTONDOWN))
	{
		delay(100);
	}
	getmouseclick(WM_LBUTTONDOWN,x,y);
	if(x>screen_x/2-backx/2-10 && y>200+(7*menu_ymid)/2-10 && x<screen_x/2+backx/2+10 && y<200+(7*menu_ymid)/2+backy+10)
		return;
	else 
		goto again;
}

void background() /*Seg 1 */
{
	setfillstyle(SOLID_FILL,BLACK);
	bar(0,0,screen_end,screen_bottom);
    setfillstyle(SOLID_FILL,score_back);
    bar(0,screen_top,screen_left,screen_y);
    bar(screen_right,screen_top,screen_x,screen_y); 
	bar(0,screen_bottom,screen_right,screen_y);
}
void ball_create()/*Seg 1 */
{
		ball_clear();
        setcolor(ball_col);
        setfillstyle(SOLID_FILL,ball_col);
        fillellipse(ball_x,ball_y,ball_radius,ball_radius);
		prex=ball_x;
		prey=ball_y;
}

void ball_movement()/*Seg 1 */
{
    if(f==0)				// if f=0, BALL moves leftward and vice-versa
        ball_x-=ball_speed_x;			// the values of ball_x & ball_y controls the SPEED & DIRECTION of the BALL along x & y-axis respectively
    else if(f==1)
        ball_x+=ball_speed_x;
    if(g==0)				//if g=0, BALL moves upward and vice-versa
        ball_y-=ball_speed_y;
    else if(g==1)
        ball_y+=ball_speed_y;
	ball_create();
}


void ball_clear()/*Seg 1 */
{
	    setcolor(BLACK);
        setfillstyle(SOLID_FILL,BLACK);
        fillellipse(prex,prey,ball_radius,ball_radius);
		setcolor(score_back);
		setfillstyle(SOLID_FILL,score_back);
		bar(0,screen_bottom,screen_x,screen_bottom+50);
}

void ball_limit_control() /*Seg 1 */
{

    if(ball_x>= bat_pos_left && ball_x -ball_radius <= bat_pos_left+bat_width && ball_y + ball_radius >= left_baty && ball_y - ball_radius <= left_baty + bat_size)	
        {
			if(ball_x>= bat_pos_left+bat_width/2 && ball_x -ball_radius <= bat_pos_left+bat_width)
				f=1;       //ball touches the left bat & direction of the ball is changed to rightward.
			if(ball_y-ball_radius<=left_baty && g==1) //ball is above the left bat
				g=0;
			else if(ball_y+ball_radius>= left_baty + bat_size && g==0)//ball is below the left bat
				g=1;

			if(ball_y-ball_radius<=left_baty || ball_y+ball_radius>= left_baty + bat_size) //corner case
			{
				if(dev>12)
					dev--; 
				ball_speed_x=8;
				ball_speed_y=5;
			}
			else if(ball_y-ball_radius>left_baty && ball_y+ball_radius < left_baty + bat_size) //
			{
				ball_speed_x=7;
				ball_speed_y=7;
			}
		}

	else if(ball_x+ball_radius>= bat_pos_right && ball_x<= bat_pos_right+bat_width && ball_y + ball_radius >= right_baty && ball_y - ball_radius <= right_baty + bat_size)	
        {
			if(ball_x<= bat_pos_right+bat_width/2 && ball_x +ball_radius >= bat_pos_right)
				f=0;       //ball touches the left bat & direction of the ball is changed to rightward.
			if(ball_y-ball_radius<=right_baty && g==1) //ball is above the left bat
				g=0;
			else if(ball_y+ball_radius>= right_baty + bat_size && g==0)//ball is below the left bat
				g=1;

			if(ball_y-ball_radius<=right_baty || ball_y+ball_radius>= right_baty + bat_size) //corner case
			{
				if(dev>12)
					dev--; 
				ball_speed_x=8;
				ball_speed_y=5;
			}
			else if(ball_y-ball_radius>right_baty && ball_y+ball_radius < right_baty + bat_size) //
			{
				ball_speed_x=7;
				ball_speed_y=7;
			}
		}

    if(ball_x<= bat_pos_left-20)	
    {									// BALL misses the left bat
        f=1;
		dev=SPEED;
		ball_speed_x=10;
		ball_speed_y=10;
        ball_x=screen_mid;						// BALL starts again from the mid position 
        r++; // one score is added to the right side
		ball_clear();
		if(SOUND!=0)
			Beep(SOUND+25,500);
		else 
			delay(500);
    }
    else if(ball_x>=bat_pos_right+bat_width+20)
    {									//similarly BALL misses the right bat...
        f=1;
		dev=SPEED;
        ball_x=screen_mid;
        l++;
		ball_clear();
		if(SOUND!=0)
			Beep(SOUND,500);
		else 
			delay(500);
    }
	 
	if(ball_y-ball_radius<=screen_top)										
	{
		g=1; // if BALL touches the upper bound of window, direction is changed to downward
		ball_speed_x=8;
		ball_speed_x=7;
	}
    else if(ball_y+ball_radius>=screen_bottom)
	{
		g=0;
		ball_speed_x=8;
		ball_speed_x=5;
		setcolor(score_back);
		setfillstyle(SOLID_FILL,score_back);
		bar(0,screen_bottom,screen_end,screen_bottom+50);
	}
}


void bat_clear() /*Seg 1 */
{
    setcolor(BLACK);
    setfillstyle(SOLID_FILL,BLACK);
    bar(bat_pos_left, screen_top, bat_pos_left+bat_width, left_baty);
    bar(bat_pos_left, left_baty+bat_size, bat_pos_left+bat_width, screen_bottom);
    bar(bat_pos_right, screen_top, bat_pos_right+bat_width, right_baty);
    bar(bat_pos_right, right_baty+bat_size,bat_pos_right+bat_width, screen_bottom);
}


void bat_create()/*Seg 1 */
{
    while(kbhit())				//takes input for moving Bats
    {
		setcolor(BLACK);
		setfillstyle(SOLID_FILL,BLACK);
		bar(bat_pos_left,left_baty,bat_pos_left+bat_width,left_baty+bat_size); 
       input_key=getch();
	   if(input_key==27)
	   {
		   	pausey=1;
			pause();
			if(quit==1)
				return;
	   }
       if(input_key==left_bat_up && left_baty-bat_size>=0)
	        left_baty-=bat_speed;							//changing the postion of left bat
	   else if(input_key==left_bat_up && left_baty-bat_size<0)
	        left_baty=0;
	   else if(input_key==left_bat_down && left_baty+bat_size+bat_speed>screen_bottom)
			left_baty=screen_bottom-bat_size;
	   else if(input_key==left_bat_down && left_baty+bat_size<=screen_bottom) //screen limit =700, so last end of bat has to be inside 580 
			left_baty+=bat_speed;
	   rewind( stdin );
		setcolor(WHITE);
		setfillstyle(SOLID_FILL,WHITE);
		bar(bat_pos_left,left_baty,bat_pos_left+bat_width,left_baty+bat_size); 
    }
	if(ismouseclick(WM_MOUSEMOVE))
		right_baty=mousey();
	if(right_baty>=screen_bottom-bat_size)
		right_baty=screen_bottom-bat_size;
	else if (pausey==1)
	{
		right_baty=bat_pause_y;
		pausey=0;
	}
	setcolor(bat_col);
    setfillstyle(SOLID_FILL,bat_col);
	bar(bat_pos_left,left_baty,bat_pos_left+bat_width,left_baty+bat_size); 
	bar(bat_pos_right,right_baty,bat_pos_right+bat_width,right_baty+bat_size);
}

void pause()
{
	setcolor(bat_col);
	setfillstyle(SOLID_FILL,bat_col);
	bar(bat_pos_left,left_baty,bat_pos_left+bat_width,left_baty+bat_size); 
	bar(bat_pos_right,right_baty,bat_pos_right+bat_width,right_baty+bat_size);
	bat_pause_y=right_baty;
	setbkcolor(score_back);
	settextjustify(CENTER_TEXT, TOP_TEXT);
	setcolor(score_col);
	settextstyle(BOLD_FONT,HORIZ_DIR,6);
	outtextxy(screen_end/2,screen_bottom+2,"GAME PAUSED");

	int color=COLOR(255,255,240);
	int d=(screen_x-450)/4;
	int h=150;
	int x,y;
	setcolor(color);
	setfillstyle(SOLID_FILL,color);
	bar(0,screen_bottom+50,screen_x,screen_bottom+100);
	setbkcolor(color);
	settextjustify(CENTER_TEXT, CENTER_TEXT);
	setcolor(score_back);
	settextstyle(FONT,HORIZ_DIR,3);
	outtextxy(h/2,screen_bottom+80,"PRESS KEY :");
	outtextxy(d+h/2,screen_bottom+80,"QUIT (Q) ");
	outtextxy(2*d+h+h/2,screen_bottom+80,"RESUME (ESC)");
	outtextxy(3*d+2*h+h/2,screen_bottom+80,"SETTINGS (S) ");
	
	again:
	control_l=getch();

	if(control_l=='q' ||control_l=='Q')
		control_l=1;
	else if (control_l=='s' ||control_l=='S')
		control_l=2;
	else if (control_l==27)
		control_l=3;
	else 
		goto again;
	if(control_l==1)
	{
		quit=1;
		return;
	}
	else if(control_l==2)
	{
		writeimagefile("Paused.bmp",0,0,screen_x,screen_y);
		settings_menu();
		pausey=0;
		readimagefile("Paused.bmp",0,0,screen_x,screen_y);
		goto again;
	}
	setfillstyle(SOLID_FILL,score_back);
	bar(0,screen_bottom,screen_x,screen_y);
	bar(0,0,screen_left,screen_bottom);
	bar(screen_right,0,screen_x,screen_bottom);
}

void create_menu() /* seg 2u */
{
	int bk= COLOR(255,255,240);
	int font=COLOR(255,165,0);
	setfillstyle(SOLID_FILL,bk);
	bar(0,0,screen_x,screen_y);
	setbkcolor(bk);
	settextjustify(CENTER_TEXT, CENTER_TEXT);
	setcolor(font);
    settextstyle(BOLD_FONT,HORIZ_DIR,10);
    outtextxy(midx,150,"MENU");
	setfillstyle(SOLID_FILL,GREY);
	bar(0,200,screen_x,205);
	bar(0,menu_y1,screen_x,menu_y1+5);
	bar(0,screen_y-5,screen_x,screen_y);
	bar(menu_x1,200,menu_x1+5,screen_y);
	bar(menu_x2,200,menu_x2+5,screen_y);
	setcolor(font);
	setbkcolor(bk);
	settextstyle(FONT,HORIZ_DIR,3);

	outtextxy(menu_x1/2,200+menu_ymid,"SET GAME SCREEN SIZE");
	outtextxy(menu_x1+menu_x1/2,200+menu_ymid,"PLAY TIME CHALLENGE");
	outtextxy(menu_x2+menu_x1/2,200+menu_ymid,"LEADERBOARD");
	outtextxy(menu_x1/2,menu_y1+menu_ymid,"GAME POINT");
	outtextxy(menu_x1+menu_x1/2,menu_y1+menu_ymid,"PLAY PONG");
	outtextxy(menu_x2+menu_x1/2,menu_y1+menu_ymid,"BACK");
}

void draw_triangle(int x,int y,int z)
{
	int points[9]={320,150,420,300,250,300,320,150};
	points[0]=x+10+z;
	points[1]=y;
	points[6]=x+10+z;
	points[7]=y;
	points[2]=x-z;
	points[3]=y+10;
	points[4]=x-z;
	points[5]=y-10;
	setcolor(COLOR(255,165,0));
	setfillstyle(SOLID_FILL,COLOR(255,165,0));
	fillpoly(4,points);
}

void first_page()/*seg 1u */
{
	int color=COLOR(210,231,3);
	int d=(screen_x-450)/4;
	int h=150;
	int x,y;
	start_pong();
	draw_triangle(screen_left-30,315,5);
	draw_triangle(screen_left-30,375,5);
	settextjustify(LEFT_TEXT, TOP_TEXT);
	setcolor(GREY);
	settextstyle(FONT,HORIZ_DIR,FONT_SIZE);
	outtextxy(screen_left,300,"Use the mouse for Right Bat Control");
	outtextxy(screen_left,360,"Use 'UP' & 'DOWN' key for Left Bat Control");
	setcolor(color);
	setfillstyle(SOLID_FILL,color);
	//hexagon length 150, height 80
	bar(d,450,d+h,500); 
	bar(2*d+h,450,2*d+2*h,500);
	bar(3*d+2*h,450,3*d+3*h,500);
	setbkcolor(color);
	settextjustify(CENTER_TEXT, CENTER_TEXT);
	setcolor(GREY);
	settextstyle(FONT,HORIZ_DIR,3);
	outtextxy(d+h/2,475,"MENU");
	outtextxy(2*d+h+h/2,475,"PLAY");
	outtextxy(3*d+2*h+h/2,475,"SETTINGS");

	starter:
	clearmouseclick(WM_LBUTTONDOWN);
	while(!ismouseclick(WM_LBUTTONDOWN))
	{
		delay(100);
	}
	getmouseclick(WM_LBUTTONDOWN, x, y);

	if(x>d && x<d+h  && y>=450 && y<=500)
		control_l=1;
	else if (x>2*d+h && x<(2*d+2*h) && y>=450 && y<=500)
		control_l=2;
	else if(x>3*d+2*h && x<(3*d+3*h)  && y>=450 && y<=500)
		control_l=3;
	else 
		goto starter;
	if(control_l==1)
	{
		menu();
	}
	else if(control_l==3)
	{
		settings_menu();
	}
	else
		return;
	setbkcolor(BLACK);
}

void game_over() /*Seg 1 */
{
    if(l>r)				//limit of score
    {
		settextjustify(RIGHT_TEXT,CENTER_TEXT);
		setbkcolor(BLACK);
        setcolor(COLOR(30,144,255));
        settextstyle(FONT,HORIZ_DIR,5);
        outtextxy(screen_mid -30,300,"WIN");
    }
    else if(r>l)
    {
		settextjustify(LEFT_TEXT,CENTER_TEXT);
		setbkcolor(BLACK);
        setcolor(COLOR(30,144,255));
        settextstyle(FONT,HORIZ_DIR,5);
        outtextxy(screen_mid +30,300,"WIN");
    }
	else if(l==r)
	{
		settextjustify(CENTER_TEXT,CENTER_TEXT);
		setbkcolor(BLACK);
        setcolor(COLOR(30,144,255));
        settextstyle(FONT,HORIZ_DIR,5);
        outtextxy(screen_mid,300,"TIE");
	}
	settextjustify(CENTER_TEXT,CENTER_TEXT);
	fprintf(score_list," %d %d \n",l,r);
	g_o=1;
    delay(1000);						// delay after WIN
    l=r=0;							// score 0-0
	last_page();
}

void hexagon(int x, int y)
{
	int points[15]={x,y,x+50,y+40,x+100,y+40,x+150,y,x+100,y-40,x+50,y-40,x,y};	
	fillpoly(6,points);
}

int last_page() /*Seg 1u */
{
	int x,y;
	start_pong();
	int d=(screen_x-540)/5;
	int h= 150; //h=hexagon length 150, height 80
	int color=COLOR(255,215,0) ;
	setcolor(color);
	setfillstyle(SOLID_FILL,color);
	bar(d,350,d+h,400);
	bar(d*2+h,350,d*2+h*2,400);
	bar(d*3+h*2,350,d*3+h*3,400);
	bar(d*4+h*3,350,d*4+h*4,400);
	setbkcolor(color);
	settextjustify(CENTER_TEXT, CENTER_TEXT);
	setcolor(COLOR(122,19,215));
    settextstyle(FONT,HORIZ_DIR,2);
    outtextxy(d+h/2,375,"MAIN MENU");
	outtextxy(d*2+h+h/2,375,"PLAY AGAIN");
	outtextxy(d*3+h*2+h/2,375,"SETTINGS");
	outtextxy(d*4+h*3+h/2,375,"EXIT");

	checker:
	clearmouseclick(WM_LBUTTONDOWN);
	while(!ismouseclick(WM_LBUTTONDOWN))
	{
		delay(100);
	}
	getmouseclick(WM_LBUTTONDOWN, x, y);

	if(x>d && x<d+h && y>=350 && y<=400)
		choice=1;
	else if (x>(d*2+h) && x< (d*2+2*h) && y>=350 && y<=400)
		choice=2;
	else if(x>(d*3+2*h) && x<(d*3+3*h) && y>=350 && y<=400)
	{
		choice=3;
	}
	else if(x>(d*4+3*h) && x<d*4+4*h && y>=350 && y<=400)
	{
		fclose(score_list);
		cleardevice();
		exit(1);
	}
	else
		goto checker; 
	return choice;
}

void menu()
{
	int x=0, y=0;
	create_menu();
	clearmouseclick(WM_LBUTTONDOWN);
	while(!ismouseclick(WM_LBUTTONDOWN))
	{
		delay(100);
	}
	getmouseclick(WM_LBUTTONDOWN, x, y);
	select_menu(x,y);
} 

void score() /*Seg 1 */
{
    sprintf(left_score, "%d", l);
    sprintf(right_score, "%d", r);
	settextjustify(CENTER_TEXT, CENTER_TEXT);
	setbkcolor(score_back);
    setcolor(score_col);
    settextstyle(FONT,HORIZ_DIR,5);
	if(score_pos==1) /*score at top */
	{
		outtextxy(screen_left/2,100,left_score);
		outtextxy(screen_end-(screen_left/2),100,right_score);
	}
	else if(score_pos==2) /* score at middle */
	{
		outtextxy(screen_left/2,screen_bottom/2,left_score);
		outtextxy(screen_end-(screen_left/2),screen_bottom/2,right_score);
	}

	else if(score_pos==3) /* score at bottom */
	{
		outtextxy(screen_left/2,screen_bottom-100,left_score);
		outtextxy(screen_end-(screen_left/2),screen_bottom-100,right_score);
	}
}

void score_card() /*seg 2u*/
{
	int x,y;
	int choice;
	int wid=(screen_y-205)/10;
	int bk= COLOR(255,255,240);
	int font=COLOR(255,165,0);
	setfillstyle(SOLID_FILL,bk);
	bar(0,0,screen_x,screen_y);
	setbkcolor(bk);
	settextjustify(CENTER_TEXT, CENTER_TEXT);
	setcolor(font);
    settextstyle(BOLD_FONT,HORIZ_DIR,9);
    outtextxy(screen_x/2,100,"SCORECARD VIEW");
	setfillstyle(SOLID_FILL,GREY);
	bar(0,150,screen_x,155);
	bar(0,200,screen_x,205);
	bar(0,205+(screen_y-200)/3,menu_x1,210+(screen_y-200)/3);
	bar(0,screen_y-(screen_y-200)/3,menu_x1,screen_y+5-(screen_y-200)/3);
	bar(0,screen_y-5,screen_x,screen_y);
	bar(menu_x1,150,menu_x1+5,screen_y);
	bar(menu_x2,150,menu_x2+5,screen_y);
	setcolor(font);
	setbkcolor(bk);
	settextstyle(FONT,HORIZ_DIR,3);
	outtextxy(menu_x1/2,185,"OPTIONS");
	outtextxy(menu_x1/2,205+(screen_y-200)/6,"SELECT BAT COLOR");
	outtextxy(menu_x1/2,205+(screen_y-200)/3+(screen_y-200)/6,"SELECT SCOREBOARD COLOR");
	outtextxy(menu_x1/2,screen_y-(screen_y-200)/3+(screen_y-200)/6,"BACK");
	
	starter:
	clearmouseclick(WM_LBUTTONDOWN);
	while(!ismouseclick(WM_LBUTTONDOWN))
	{
		delay(100);
	}
	getmouseclick(WM_LBUTTONDOWN, x, y);

	if(x>0 && x<menu_x1 && y>200  && y<205+(screen_y-200)/3)
	{
		choice=1;
		outtextxy(menu_x1+(menu_x1)/2,185,"BAT COLOR");
		setfillstyle(SOLID_FILL,COLOR(240,255,240)); //HoneyDew
		bar(menu_x1+5,205,menu_x2,205+wid);
		setfillstyle(SOLID_FILL,COLOR(255,255,240)); //Ivory
		bar(menu_x1+5,205+wid+1,menu_x2,205+2*wid);
		setfillstyle(SOLID_FILL,COLOR(251,249,249)); // Beige 5%
		bar(menu_x1+5,205+2*wid+1,menu_x2,205+wid*3);
		setfillstyle(SOLID_FILL,COLOR(255,250,205)); //Lemon Chiffon
		bar(menu_x1+5,205+wid*3+1,menu_x2,205+wid*4);
		setfillstyle(SOLID_FILL,COLOR(255,248,220)); //Corn Silk
		bar(menu_x1+5,205+wid*4+1,menu_x2,205+wid*5);
		setfillstyle(SOLID_FILL,COLOR(245,245,220)); //Beige
		bar(menu_x1+5,205+wid*5+1,menu_x2,205+wid*6);
		setfillstyle(SOLID_FILL,COLOR(239, 235, 214)); //Aths Special
		bar(menu_x1+5,205+wid*6+1,menu_x2,205+wid*7);
		setfillstyle(SOLID_FILL,COLOR(245,255,250) ); //Mint Cream
		bar(menu_x1+5,205+wid*7+1,menu_x2,205+wid*8);
		setfillstyle(SOLID_FILL,COLOR(223, 239, 240)); //Catskill White
		bar(menu_x1+5,205+wid*8+1,menu_x2,205+wid*9);
		setfillstyle(SOLID_FILL,COLOR(249, 246, 244)); //Peach
		bar(menu_x1+5,205+wid*9+1,menu_x2,205+wid*10);
		clearmouseclick(WM_LBUTTONDOWN);
		while(!ismouseclick(WM_LBUTTONDOWN));
		getmouseclick(WM_LBUTTONDOWN, x, y);
		bat_col=getpixel(x,y);
		setfillstyle(SOLID_FILL,bat_col);
		bar(menu_x1/2- 40,210+(screen_y-200)/3-75,menu_x1/2+ 40,210+(screen_y-200)/3-25);
		setfillstyle(SOLID_FILL,bk);
		bar(menu_x1+5,155,menu_x2,screen_y);
		setfillstyle(SOLID_FILL,GREY);
		bar(menu_x1+5,200,menu_x2,205);
		goto starter;
	}
	else if (x>0 && x<menu_x1 && y>205+(screen_y-200)/3 && y<screen_y-(screen_y-200)/3)
	{
		choice=2;
		outtextxy(menu_x2+(menu_x1)/2,185,"SCOREBOARD COLOR");
		setfillstyle(SOLID_FILL,COLOR(157,149,115)); //Beige 100%
		bar(menu_x2+5,205,menu_x3,205+wid);
		setfillstyle(SOLID_FILL,COLOR(221,207,153)); //Sandstone
		bar(menu_x2+5,205+wid+1,menu_x3,205+2*wid);
		setfillstyle(SOLID_FILL,COLOR( 138,136,125)); // Gray 60%
		bar(menu_x2+5,205+2*wid+1,menu_x3,205+wid*3);
		setfillstyle(SOLID_FILL,COLOR(88,87,84)); //Black 80%
		bar(menu_x2+5,205+wid*3+1,menu_x3,205+wid*4);
		setfillstyle(SOLID_FILL,COLOR( 140,21,21)); //Cardinal red
		bar(menu_x2+5,205+wid*4+1,menu_x3,205+wid*5);
		setfillstyle(SOLID_FILL,COLOR(21, 52, 80)); //Elephant Midnight Blue
		bar(menu_x2+5,205+wid*5+1,menu_x3,205+wid*6);
		setfillstyle(SOLID_FILL,COLOR(56, 46, 28)); //Bistre
		bar(menu_x2+5,205+wid*6+1,menu_x3,205+wid*7);
		setfillstyle(SOLID_FILL,COLOR(173, 116, 96) ); //Santa fe
		bar(menu_x2+5,205+wid*7+1,menu_x3,205+wid*8);
		setfillstyle(SOLID_FILL,COLOR(198, 199, 140)); //Pine glade
		bar(menu_x2+5,205+wid*8+1,menu_x3,205+wid*9);
		setfillstyle(SOLID_FILL,COLOR(231, 195, 156)); //Cashmere 
		bar(menu_x2+5,205+wid*9+1,menu_x3,205+wid*10);

		clearmouseclick(WM_LBUTTONDOWN);
		while(!ismouseclick(WM_LBUTTONDOWN));
		getmouseclick(WM_LBUTTONDOWN, x, y);
		score_back=getpixel(x,y);
		setfillstyle(SOLID_FILL,score_back);
		bar(menu_x1/2- 40,screen_y+5-(screen_y-200)/3-75,menu_x1/2+ 40,screen_y+5-(screen_y-200)/3-25);
		setfillstyle(SOLID_FILL,bk);
		bar(menu_x2+5,155,screen_x,screen_y);
		setfillstyle(SOLID_FILL,GREY);
		bar(menu_x2+5,200,screen_x,205);
		goto starter;
	}
	else if (x>0 && x<menu_x1 && y>(screen_y-(screen_y-200)/3) && y<screen_end)
	{	
		choice=3;
		return ;
	}
	else if (x>menu_x1)
		goto starter;
}

void score_check(int n, int p) /*seg2u */
{
	int bk= COLOR(255,255,240);
	m=0;
	char scorer[1000];
	int i,j,k,max,maxi,x,y;
	fclose(score_list);
	score_list=fopen("score log.txt","r");
	fseek(score_list,0L,0);
	setfillstyle(SOLID_FILL,bk);
	bar(0,155,menu_x2,screen_y-5);
	outtextxy(menu_x1,menu_y1,"CLICK ANYWHERE TO RETURN TO MENU");
	for(i=0;;i++)
	{
		fscanf(score_list,"%d %d",&p2[i],&p1[i]);//p2 = left, p1=right
		dif[1][i]=p1[i]-p2[i];
		dif[2][i]=p2[i]-p1[i];
		if(feof(score_list))
		{
			break;
		}
	}
	for(k=0;k<i;k++)
	{
		max=-100;
		for(j=0;j<i;j++)
		{
			if(dif[p][j]>max)
			{
				max=dif[p][j];
				maxi=j;
			}	
		}
		if(p==1)
			sprintf(top[m++],"%d - %d",p1[maxi],p2[maxi]);
		else if (p==2)
			sprintf(top[m++],"%d - %d",p2[maxi],p1[maxi]);
		dif[p][maxi]=-105;
    }
	if(i<n)
	{
		if(i==0)
			sprintf(top[m++],"Sorry, But you never played with us");
		else
			sprintf(top[m++],"Sorry you played only %d times",i);
	}
	fclose(score_list);
	int font=COLOR(255,165,0);
	setfillstyle(SOLID_FILL,bk);
	bar(menu_x2+5,205,menu_x3,screen_y-5); 
	setbkcolor(bk);
	setcolor(font);
	settextstyle(FONT,HORIZ_DIR,3);
	sprintf(scorer,"Best %d Results for Player %d",n,p);
	outtextxy(menu_x2+menu_x1/2,185,scorer); 
	for(j=0;j<n && j<m;j++)
	{
		outtextxy(menu_x2+menu_x1/2,240+((screen_y-240)/n)*(j),top[j]); 
	}
	score_list=fopen("score log.txt","a");
	r=0;
	l=0;
	clearmouseclick(WM_LBUTTONDOWN);
	while(!ismouseclick(WM_LBUTTONDOWN))
	{
		delay(100);
	}
} 

void score_menu() /*seg 2u*/
{
	int x,y,n,p;
	int bk= COLOR(255,255,240);
	int font=COLOR(255,165,0);
	setfillstyle(SOLID_FILL,bk);
	bar(0,0,screen_x,screen_y);
	setbkcolor(bk);
	settextjustify(CENTER_TEXT, CENTER_TEXT);
	setcolor(font);
    settextstyle(BOLD_FONT,HORIZ_DIR,8);
    outtextxy(screen_x/2,100,"BEST RECORDS");
	setfillstyle(SOLID_FILL,GREY);
	bar(0,150,screen_x,155);
	bar(0,200,screen_x,205);
	bar(0,menu_y1,screen_x,menu_y1+5);
	bar(0,screen_y-5,screen_x,screen_y);
	bar(menu_x1,150,menu_x1+5,screen_y);
	bar(menu_x2,150,menu_x2+5,screen_y);
	setcolor(font);
	setbkcolor(bk);
	settextstyle(FONT,HORIZ_DIR,3);
	outtextxy(menu_x1/2,185,"Select Player");
	outtextxy(menu_x1/2,200+menu_ymid,"Player 1");
	outtextxy(menu_x1/2,menu_y1+menu_ymid,"Player 2");

	player_select:
	clearmouseclick(WM_LBUTTONDOWN);
	while(!ismouseclick(WM_LBUTTONDOWN))
	{
		delay(100);
	}
	getmouseclick(WM_LBUTTONDOWN, x, y);

	if(x>0 && x<menu_x1 && y>200  && y<menu_y1)
	{
		p=1;
		setfillstyle(SOLID_FILL,BLUE);
		fillellipse(100,200+menu_ymid-5,10,10);
	}
	else if (x>0 && x<menu_x1 && y>menu_y1 && y<screen_y)
	{
		p=2;
		setfillstyle(SOLID_FILL,BLUE);
		fillellipse(100,menu_y1+menu_ymid-10,10,10);
	}
	else 
		goto player_select;

	outtextxy(menu_x1+(menu_x1)/2,185,"Number of Records");
	outtextxy(menu_x1+(menu_x1)/2,200+menu_ymid,"5");
	outtextxy(menu_x1+(menu_x1)/2,menu_y1+menu_ymid,"10");
	
	record_select:
	clearmouseclick(WM_LBUTTONDOWN);
	while(!ismouseclick(WM_LBUTTONDOWN))
	{
		delay(100);
	}
	getmouseclick(WM_LBUTTONDOWN, x, y);

	if(x>menu_x1 && y>200 && x<menu_x2 && y< menu_y1)
		n=5;
	else if (x>menu_x1 && y>menu_y1 && x<menu_x2 && y<screen_y)
		n=10;
	else
		goto record_select;
	score_check(n,p);
}

void select_menu(int x , int y) /*seg2u */
{
	int space,wid,hei ; 
	int bk= COLOR(255,255,240);
	if((x>0 && x<menu_x1) && (y>200 && y<menu_y1))
	{
		printf("Set game screen\n");//set game screen
		setfillstyle(SOLID_FILL,bk);
		bar(0,205,menu_x1,menu_y1); 
		setcolor(GREY);
		setfillstyle(SOLID_FILL,GREY);
		bar(0,200+menu_ymid,menu_x1,200+menu_ymid+5);//The barrier
		bar(menu_x1/2,200,menu_x1/2 + 5,menu_y1);
		setbkcolor(bk);
		setcolor(GREY);
		settextstyle(FONT,HORIZ_DIR,FONT_SIZE);
		outtextxy(menu_x1/4,menu_ytop,"600 * 480"); 
		outtextxy(menu_x1-menu_x1/4,menu_ytop,"800 * 640"); 
		outtextxy(menu_x1/4,menu_y1- menu_ymid/2,"750 * 500"); 
		outtextxy(menu_x1-menu_x1/4,menu_y1- menu_ymid/2,"900 * 600"); 
		clearmouseclick(WM_LBUTTONDOWN);
		while(!ismouseclick(WM_LBUTTONDOWN))
		{
			delay(100);
		}
		getmouseclick(WM_LBUTTONDOWN, x, y); 

		if((x>0 && x<menu_x1/2) && (y>200 && y<200+menu_ymid))/*600 * 480 = 5:4*/
		{
			wid=600;
			hei=480;
		}
		else if((x>0 && x<menu_x1/2) && (y>200+menu_ymid && y<menu_y1))/*800 * 640 = 5:4*/
		{
			wid=800;
			hei=640;
		}
		else if((x>menu_x1/2 && x<menu_x1) && (y>200 && y<200+menu_ymid)) /*750 * 500 = 3:2 */
		{
			wid=750;
			hei=500;
		}
		else if((x>menu_x1/2 && x<menu_x1) && (y>200+menu_ymid && y<menu_y1))/*900 * 600 = 3:2*/
		{
			wid=900;
			hei=600;
		} 
		space= (screen_x-wid)/2;
		screen_left=space;  /*start of black screen*/
		screen_right=screen_left+wid;  /*end of black screen*/
		screen_bottom=hei; /*bottom of black screen*/
		screen_end=screen_left+screen_right;
		screen_mid=screen_left+ wid/2;
		ball_y=screen_bottom/2,ball_x=screen_mid;
	}
	else if((x>menu_x1 && x<menu_x2) && (y>200 && y<menu_y1))
	{
		printf("Play TIME CHALLENGE\n");
		mode=2;
		high_score=99;
		setfillstyle(SOLID_FILL,bk);
		bar(menu_x1+5,205,menu_x2,menu_y1); 
		setcolor(GREY);
		setfillstyle(SOLID_FILL,GREY);
		bar(menu_x1,200+menu_ymid,menu_x2,200+menu_ymid+5);//The barrier
		bar(menu_x1+menu_x1/2,200,menu_x1+menu_x1/2+5,menu_y1);
		setbkcolor(bk);
		setcolor(GREY);
		settextstyle(FONT,HORIZ_DIR,3);
		outtextxy(menu_x1+menu_x1/4,menu_ytop,"30 seconds"); 
		outtextxy(menu_x2-menu_x1/4,menu_ytop,"60 seconds"); 
		outtextxy(menu_x1+menu_x1/4,menu_y1- menu_ymid/2,"150 seconds"); 
		outtextxy(menu_x2-menu_x1/4,menu_y1- menu_ymid/2,"450 seconds"); 
		clearmouseclick(WM_LBUTTONDOWN);
		while(!ismouseclick(WM_LBUTTONDOWN))
		{
			delay(100);
		}
		getmouseclick(WM_LBUTTONDOWN, x, y);

		if((x>menu_x1 && x<menu_x1+menu_x1/2) && (y>200 && y<200+menu_ymid))/*OPTION 1*/
			TIME=30;
		else if((x>menu_x1 && x<menu_x1+menu_x1/2) && (y>200+menu_ymid && y<menu_y1))/*OPTION 2*/
			TIME=60;
		else if((x>menu_x1+menu_x1/2 && x<menu_x2) && (y>200 && y<200+menu_ymid)) /*OPTION 3*/
			TIME=150;
		else if((x>menu_x1+menu_x1/2 && x<menu_x2) && (y>200+menu_ymid && y<menu_y1))/*OPTION 4*/
			TIME=450; 
		else 
			TIME=5;
		time(&start_t);
		return;
	}
	else if((x>menu_x2 && x<screen_x) && (y>200 && y<menu_y1))
	{
		printf("Leaderboard\n");
		score_menu();
	}
	/* Game Point Menu */
	else if((x>0 && x<menu_x1) && (y>menu_y1 && y<screen_y))
	{
		printf("GAME POINT\n");
		setfillstyle(SOLID_FILL,bk);
		bar(0,menu_y1+5,menu_x1,menu_y2); 
		setcolor(GREY);
		setfillstyle(SOLID_FILL,GREY);
		bar(0,menu_y1+menu_ymid,menu_x1,menu_y1+menu_ymid+5);//The barrier
		bar(menu_x1/2,menu_y1,menu_x1/2 + 5,menu_y2);
		setbkcolor(bk);
		setcolor(GREY);
		settextstyle(FONT,HORIZ_DIR,FONT_SIZE);
		outtextxy(menu_x1/4,menu_y1+ menu_ymid/2,"5");
		outtextxy(menu_x1-menu_x1/4,menu_y1+ menu_ymid/2,"10"); 
		outtextxy(menu_x1/4,menu_y1+3*menu_ymid/2,"15"); 
		outtextxy(menu_x1-menu_x1/4,menu_y1+3*menu_ymid/2,"20"); 
		clearmouseclick(WM_LBUTTONDOWN);
		while(!ismouseclick(WM_LBUTTONDOWN))
		{
			delay(100);
		}
		getmouseclick(WM_LBUTTONDOWN, x, y);

		if((x>0 && x<menu_x1/2) && (y>menu_y1 && y<menu_y1+menu_ymid))/* Game Point 5 */
			high_score=5;
		else if((x>0 && x<menu_x1/2) && (y>menu_y1+menu_ymid && y<menu_y2))/* Game Point 15 */
			high_score=15;
		else if((x>menu_x1/2 && x<menu_x1) && (y>menu_y1 && y<menu_y1+menu_ymid)) /* Game Point 10 */
			high_score=10;
		else if((x>menu_x1/2 && x<menu_x1) && (y>menu_y1+menu_ymid && y<menu_y2))/* Game Point 20 */
			high_score=20;
		printf("%d\n",high_score);
	}
	else if((x>menu_x1 && x<menu_x2) && (y>menu_y1 && y<screen_y))
	{
		printf("Play\n");
		return;
	}
	else if((x>menu_x2 && x<screen_x) && (y>menu_y1 && y<screen_y))
	{
		first_page();
		return;
	}
	menu();
} 

void settings_menu() /*seg 2u */
{
	cleardevice();
	int x,y;
	int bk= COLOR(255,255,240);
	int font=COLOR(255,165,0);
	setfillstyle(SOLID_FILL,bk);
	bar(0,0,screen_x,screen_y);
	setbkcolor(bk);
	settextjustify(CENTER_TEXT, CENTER_TEXT);
	setcolor(font);
    settextstyle(BOLD_FONT,HORIZ_DIR,10);
    outtextxy(midx,150,"SETTINGS");
	setfillstyle(SOLID_FILL,GREY);
	bar(0,200,screen_x,205);
	bar(0,menu_y1,screen_x,menu_y1+5);// middle horizontal bar
	bar(0,screen_y-5,screen_x,screen_y); // bottom bar
	bar(menu_x1,200,menu_x1+5,screen_y);
	bar(menu_x2,200,menu_x2+5,screen_y);
	setcolor(font);
	setbkcolor(bk);
	settextstyle(FONT,HORIZ_DIR,3);

	outtextxy(menu_x1/2,200+menu_ymid,"ABOUT");
	outtextxy(menu_x1+menu_x1/2,200+menu_ymid,"SOUNDS");
	outtextxy(menu_x2+menu_x1/2,200+menu_ymid,"BAT SPEED");
	outtextxy(menu_x1/2,menu_y1+menu_ymid,"SCORE POSITION");
	outtextxy(menu_x1+menu_x1/2,menu_y1+menu_ymid,"SCORECARD VIEW");
	outtextxy(menu_x2+menu_x1/2,menu_y1+menu_ymid,"BACK");
	
	clearmouseclick(WM_LBUTTONDOWN);
	while(!ismouseclick(WM_LBUTTONDOWN))
	{
		delay(100);
	}
	getmouseclick(WM_LBUTTONDOWN, x, y);

	if((x>0 && x<menu_x1) && (y>200 && y<menu_y1))
	{
		printf("About\n");
		about();
	}
	else if((x>menu_x1 && x<menu_x2) && (y>200 && y<menu_y1))
	{
		printf("SOUNDS\n");
		setfillstyle(SOLID_FILL,bk);
		bar(menu_x1+5,205,menu_x2,menu_y1); 
		setcolor(GREY);
		setfillstyle(SOLID_FILL,GREY);
		bar(menu_x1,200+menu_ymid,menu_x2,200+menu_ymid+5);//The barrier
		bar(menu_x1+menu_x1/2,200,menu_x1+menu_x1/2+5,menu_y1);
		setbkcolor(bk);
		setcolor(GREY);
		settextstyle(FONT,HORIZ_DIR,3);
		outtextxy(menu_x1+menu_x1/4,menu_ytop,"MUTE"); 
		outtextxy(menu_x2-menu_x1/4,menu_ytop,"SOUND 1"); 
		outtextxy(menu_x1+menu_x1/4,menu_y1- menu_ymid/2,"SOUND 2"); 
		outtextxy(menu_x2-menu_x1/4,menu_y1- menu_ymid/2,"SOUND 3"); 
		clearmouseclick(WM_LBUTTONDOWN);
		while(!ismouseclick(WM_LBUTTONDOWN))
		{
			delay(100);
		}
		getmouseclick(WM_LBUTTONDOWN, x, y);

		if((x>menu_x1 && x<menu_x1+menu_x1/2) && (y>200 && y<200+menu_ymid))/*MUTE*/
			SOUND=0;
		else if((x>menu_x1 && x<menu_x1+menu_x1/2) && (y>200+menu_ymid && y<menu_y1))/*SOUND 1*/
			SOUND=55;
		else if((x>menu_x1+menu_x1/2 && x<menu_x2) && (y>200 && y<200+menu_ymid)) /*SOUND 2*/
			SOUND=85;
		else if((x>menu_x1+menu_x1/2 && x<menu_x2) && (y>200+menu_ymid && y<menu_y1))/*SOUND 3*/
			SOUND=150; 
	}
	else if((x>menu_x2 && x<screen_x) && (y>200 && y<menu_y1))
	{
		printf("BAT SPEED\n");
		setfillstyle(SOLID_FILL,bk);
		bar(menu_x2+5,205,menu_x3,menu_y1); 
		setcolor(GREY);
		setfillstyle(SOLID_FILL,GREY);
		bar(menu_x2,200+menu_ymid,menu_x3,200+menu_ymid+5);//The barrier
		bar(menu_x2+menu_x1/2,200,menu_x2+menu_x1/2+5,menu_y1);
		setbkcolor(bk);
		setcolor(GREY);
		settextstyle(FONT,HORIZ_DIR,3);
		outtextxy(menu_x2+menu_x1/4,menu_ytop,"1x"); 
		outtextxy(menu_x3-menu_x1/4,menu_ytop,"1.2x"); 
		outtextxy(menu_x2+menu_x1/4,menu_y1- menu_ymid/2,"1.4x"); 
		outtextxy(menu_x3-menu_x1/4,menu_y1- menu_ymid/2,"1.75x"); 

		clearmouseclick(WM_LBUTTONDOWN);
		while(!ismouseclick(WM_LBUTTONDOWN))
		{
			delay(100);
		}
		getmouseclick(WM_LBUTTONDOWN, x, y);

		if((x>menu_x2 && x<menu_x2+menu_x1/2) && (y>200 && y<200+menu_ymid))/*batspeed 100 */
			bat_speed=100;
		else if((x>menu_x2 && x<menu_x2+menu_x1/2) &&  (y>200+menu_ymid && y<menu_y1))/*batspeed 140 seconds*/
			bat_speed=140;
		else if((x>menu_x2+menu_x1/2 && x<menu_x3) && (y>200 && 200+menu_ymid)) /* batspeed 120 seconds */
			bat_speed=120;
		else if((x>menu_x2+menu_x1/2 && x<menu_x3) &&  (y>200+menu_ymid && y<menu_y1))/*batspeed 175 seconds */
			bat_speed=175;
	}
	else if((x>0 && x<menu_x1) && (y>menu_y1 && y<screen_y)) // Set Score Position
	{
		int x,y;
		int wid=(screen_y-205)/10;
		int bk= COLOR(255,255,240);
		int font=COLOR(255,165,0);
		setfillstyle(SOLID_FILL,bk);
		bar(0,menu_y1+5,menu_x1,menu_y2-5); 
		setcolor(GREY);
		setfillstyle(SOLID_FILL,GREY);
		bar(0,menu_y1+(menu_y2-menu_y1)/3,menu_x1,menu_y1+(menu_y2-menu_y1)/3+5);//horizontal barrier 1
		bar(0,menu_y1+2*(menu_y2-menu_y1)/3,menu_x1,menu_y1+2*(menu_y2-menu_y1)/3+5);//horizontal barrier 2
		setcolor(GREY);
		setbkcolor(bk);
		settextstyle(FONT,HORIZ_DIR,FONT_SIZE);
		outtextxy(menu_x1/2,menu_y1+(menu_y2-menu_y1)/6+3,"TOP");
		outtextxy(menu_x1/2,menu_y1+(menu_y2-menu_y1)/2+3,"MIDDLE");
		outtextxy(menu_x1/2,menu_y2-(menu_y2-menu_y1)/6+3,"BOTTOM");
		clearmouseclick(WM_LBUTTONDOWN);
		while(!ismouseclick(WM_LBUTTONDOWN))
		{
			delay(100);
		}
		getmouseclick(WM_LBUTTONDOWN, x, y);

		if((x>0 && x<menu_x1) && (y>menu_y1 && y<menu_y1+(menu_y2-menu_y1)/3))/*TOP */
			score_pos=1;
		else if((x>0 && x<menu_x1) && (y>menu_y1+(menu_y2-menu_y1)/3 && y<2*(menu_y2-menu_y1)/3)) /* MIDDLE*/
			score_pos=2;
		else if((x>0 && x<menu_x1) && (y>2*(menu_y2-menu_y1)/3 && y<menu_y2))/*BOTTOM*/
			score_pos=3;
		printf("%d\n",score_pos);
	}
	else if((x>menu_x1 && x<menu_x2) && (y>menu_y1 && y<screen_y))
	{
		printf("SCORECARD VIEW\n");
		score_card();
	}
	else if((x>menu_x2 && x<menu_x3) && (y>menu_y1 && y<screen_y))
	{
		printf("BACK\n");
		first_page();
		return;
	}
	if(pausey!=1)
		settings_menu();
}

void start_pong() /*Seg 1u */
{
	int bk= COLOR(255,255,240);
	setfillstyle(SOLID_FILL,bk);
	bar(0,0,screen_x,screen_y);
	setbkcolor(bk);
	settextjustify(CENTER_TEXT, CENTER_TEXT);
	setcolor(COLOR(255,165,0));
    settextstyle(BOLD_FONT,HORIZ_DIR,10);
    outtextxy(screen_x/2,150,"PONG");

	settextjustify(RIGHT_TEXT, TOP_TEXT);
	setcolor(COLOR(154,205,50));
    settextstyle(FONT,HORIZ_DIR,4);
    outtextxy(screen_x/2+250,180,"Edition 6.8");
	settextjustify(RIGHT_TEXT, TOP_TEXT);
	settextstyle(FONT,HORIZ_DIR,3);
	outtextxy(screen_x-10,screen_y-100,"Designed and Developed By :");
	outtextxy(screen_x-10,screen_y-50,"Md.Shorifuzzaman and Fahim Arefin");
	settextjustify(LEFT_TEXT, TOP_TEXT);
	if(flag==0)
	{
		left_bat_up=72;
		left_bat_down=80;
	}
	flag=1;
}

int time()
{
	double diff_t;
	int time_flag=0;
    time(&end_t);
    diff_t = difftime(end_t, start_t);
    if(diff_t-TIME==1)
		time_flag=1;
	return time_flag;
}
