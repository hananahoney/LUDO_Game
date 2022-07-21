//=====================================================
//							\\
//			Authors			\\
//							\\
//	Member1:					\\
//							\\
//	Name: 		Hanan Ali			\\
//	Roll #:	i192172			\\
//							\\
//	Member2:					\\
//							\\
//	Name:		Bilal Saeed			\\
//	Roll#:		i192016			\\
//							\\
//====================================================\\

#include<iostream>
#include<pthread.h>
#include<time.h>
#include<ctime>
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>
#include<unistd.h>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>

using namespace std;

int x_yellowt[4], y_yellowt[4]  ;

int x_greent[4], y_greent[4] ;

int x_bluet[4], y_bluet[4] ;

int x_redt[4], y_redt[4] ;

bool players[4] = {false,false,false,false};			//winning check


int x_red_path[53], y_red_path[53];				//red token path stored
int x_blue_path[53], y_blue_path[53];				//blue token path stored
int x_yellow_path[53], y_yellow_path[53];			//yellow token path stored
int x_green_path[53], y_green_path[53];			//green token path stored

pthread_mutex_t lock1 = PTHREAD_MUTEX_INITIALIZER;
sem_t lock2;

pthread_t tid[4];
pthread_t master_id,tokenid;

int init_x_yellowt[4], init_y_yellowt[4];;
int init_x_greent[4], init_y_greent[4];
int init_x_bluet[4], init_y_bluet[4];
int init_x_redt[4], init_y_redt[4];

bool token_check[4][4] = { {false,false,false,false},
			    {false,false,false,false},
			    {false,false,false,false},
			    {false,false,false,false},
			};				//2d array for tokkens to check where they are in game or died or not opend yet
							//false would show token is died and true will show token is in game

int hit_rate[4] = {0,0,0,0};

struct paras{
 	int player_no;
};

       
paras obj[4];

int dice;

void movement( int token_no, int dice_roll, int player_no )
{
    	//player_no 1 is red
	//player_no 2 is blue
	//player_no 3 is yellow
	//player_no 4 is green
    
	cout<<endl<<"Player NO: "<<player_no<<endl;
	cout<<"Dice Roll : "<<dice_roll<<endl;
	cout<<"Token NO : "<<token_no<<endl;

    //Find current location of the token
	
    if( player_no == 1 )		//red player
    {
    	if( dice_roll == 6 && !token_check[player_no-1][token_no] )
    	{
    		x_redt[token_no] = 175;
    		y_redt[token_no] = 280; 
    		token_check[player_no-1][token_no] = true;
    	}
    	else if( token_check[player_no-1][token_no] )
    	{
    		int x_t=x_redt[token_no];
		int y_t=y_redt[token_no];
		int st = 0;
		int st_store=0;
		
		for( st=0;st<52;st++)
		{
			if((x_t==x_red_path[st] )&& (y_t==y_red_path[st]))
			{
					st_store=st;
					break;
			}
		}
		
		if( st_store + dice_roll < 54 )
		{
		
			st_store += dice_roll;
			st_store--;
			
			if( st_store > 46 && hit_rate[player_no-1] == 0 )
			{
				x_redt[token_no] = x_red_path[0];
				y_redt[token_no] = y_red_path[0];
			}
			else
			{
				x_redt[token_no] = x_red_path[st_store];
				y_redt[token_no] = y_red_path[st_store];
			}
		}
		
		bool collision = false;
		
		if( !collision )				//detecting collision with yellow tokens 
		{
			for( int i = 0 ; i < 4 ; i++ )
			{
				if( x_redt[token_no] == x_yellowt[i] && y_redt[token_no] == y_yellowt[i] )
				{
					if( (x_redt[token_no] == 175 && y_redt[token_no] == 280) || (x_redt[token_no] == 375 && y_redt[token_no] == 130) || (x_redt[token_no] == 475 && y_redt[token_no] == 80) || (x_redt[token_no] == 625 && y_redt[token_no] == 280) || (x_redt[token_no] == 675 && y_redt[token_no] == 380) || (x_redt[token_no] == 475 && y_redt[token_no] == 580) || (x_redt[token_no] == 375 && y_redt[token_no] == 580) || (x_redt[token_no] == 225 && y_redt[token_no] == 380)  )
					{
						cout<<"Stop"<<endl;
						break;
					}
					else
					{
						x_yellowt[i] = init_x_yellowt[i];
						y_yellowt[i] = init_y_yellowt[i];
						token_check[2][i] = false;
						collision = true;
						hit_rate[0]++;
						break;
					}
				}
			}
		}
		if( !collision )				//detecting collision with blue tokens 
		{
			for( int i = 0 ; i < 4 ; i++ )
			{
				if( x_redt[token_no] == x_bluet[i] && y_redt[token_no] == y_bluet[i] )
				{
					if( (x_redt[token_no] == 175 && y_redt[token_no] == 280) || (x_redt[token_no] == 375 && y_redt[token_no] == 130) || (x_redt[token_no] == 475 && y_redt[token_no] == 80) || (x_redt[token_no] == 625 && y_redt[token_no] == 280) || (x_redt[token_no] == 675 && y_redt[token_no] == 380) || (x_redt[token_no] == 475 && y_redt[token_no] == 580) || (x_redt[token_no] == 375 && y_redt[token_no] == 580) || (x_redt[token_no] == 225 && y_redt[token_no] == 380)   )
					{
						cout<<"Stop"<<endl;
						break;
					}
					else
					{
						x_bluet[i] = init_x_bluet[i];
						y_bluet[i] = init_y_bluet[i];
						token_check[1][i] = false;
						collision = true;
						hit_rate[0]++;
						break;
					}
				}
			}
		}
		if( !collision )				//detecting collision with green tokens 
		{
			for( int i = 0 ; i < 4 ; i++ )
			{
				if( x_redt[token_no] == x_greent[i] && y_redt[token_no] == y_greent[i])
				{
					if( (x_redt[token_no] == 175 && y_redt[token_no] == 280) || (x_redt[token_no] == 375 && y_redt[token_no] == 130) || (x_redt[token_no] == 475 && y_redt[token_no] == 80) || (x_redt[token_no] == 625 && y_redt[token_no] == 280) || (x_redt[token_no] == 675 && y_redt[token_no] == 380) || (x_redt[token_no] == 475 && y_redt[token_no] == 580) || (x_redt[token_no] == 375 && y_redt[token_no] == 580) || (x_redt[token_no] == 225 && y_redt[token_no] == 380)   )
					{
						cout<<"Stop"<<endl;
						break;
					}
					else
					{
						x_greent[i] = init_x_greent[i];
						y_greent[i] = init_y_greent[i];
						token_check[3][i] = false;
						collision = true;
						hit_rate[0]++;
						break;
					}
				}
			}
		}
			
		if( x_redt[0] == 425 && y_redt[0] == 330 && x_redt[1] == 425 && y_redt[1] == 330 && x_redt[2] == 425 && y_redt[2] == 330 && x_redt[3] == 425 && y_redt[3] == 330 )
		{
			players[0] = true;
		}
		
    	}
    }
    else if( player_no == 2 )		//blue player
    {
    	if( dice_roll == 6 && !token_check[player_no-1][token_no] )
    	{
    		x_bluet[token_no] = 475;
    		y_bluet[token_no] = 80; 
    		token_check[player_no-1][token_no] = true;
    	}
    	else if( token_check[player_no-1][token_no] )
    	{
    		int x_t=x_bluet[token_no];
		int y_t=y_bluet[token_no];
		int st = 0;
		int st_store=0;
		
		for( st=0;st<52;st++)
		{
			if((x_t==x_blue_path[st] )&& (y_t==y_blue_path[st]))
			{
					st_store=st;
					break;
			}
		}
		
		if( st_store + dice_roll < 54 )
		{
		
			st_store += dice_roll;
			st_store--;
			
			if( st_store > 46 && hit_rate[player_no-1] == 0 )
			{
				x_bluet[token_no] = x_blue_path[0];
				y_bluet[token_no] = y_blue_path[0];
			}
			else
			{
				x_bluet[token_no] = x_blue_path[st_store];
				y_bluet[token_no] = y_blue_path[st_store];
			}
		
		}
    	}
    	bool collision = false;
		
		if( !collision )				//detecting collision with yellow tokens 
		{
			for( int i = 0 ; i < 4 ; i++ )
			{
				if( x_bluet[token_no] == x_yellowt[i] && y_bluet[token_no] == y_yellowt[i] )
				{
					if( (x_bluet[token_no] == 175 && y_bluet[token_no] == 280) || (x_bluet[token_no] == 375 && y_bluet[token_no] == 130) || (x_bluet[token_no] == 475 && y_bluet[token_no] == 80) || (x_bluet[token_no] == 625 && y_bluet[token_no] == 280) || (x_bluet[token_no] == 675 && y_bluet[token_no] == 380) || (x_bluet[token_no] == 475 && y_bluet[token_no] == 580) || (x_bluet[token_no] == 375 && y_bluet[token_no] == 580) || (x_bluet[token_no] == 225 && y_bluet[token_no] == 380) )
					{
						cout<<"Stop"<<endl;
						break;
					}
					else
					{
						x_yellowt[i] = init_x_yellowt[i];
						y_yellowt[i] = init_y_yellowt[i];
						token_check[2][i] = false;
						collision = true;
						hit_rate[1]++;
						break;
					}
				}
			}
		}
		if( !collision )				//detecting collision with  red tokens 
		{
			for( int i = 0 ; i < 4 ; i++ )
			{
				if( x_bluet[token_no] == x_redt[i] && y_bluet[token_no] == y_redt[i] )
				{
					if( (x_bluet[token_no] == 175 && y_bluet[token_no] == 280) || (x_bluet[token_no] == 375 && y_bluet[token_no] == 130) || (x_bluet[token_no] == 475 && y_bluet[token_no] == 80) || (x_bluet[token_no] == 625 && y_bluet[token_no] == 280) || (x_bluet[token_no] == 675 && y_bluet[token_no] == 380) || (x_bluet[token_no] == 475 && y_bluet[token_no] == 580) || (x_bluet[token_no] == 375 && y_bluet[token_no] == 580) || (x_bluet[token_no] == 225 && y_bluet[token_no] == 380) )
					{
						cout<<"Stop"<<endl;
						break;
					}
					else
					{
						x_redt[i] = init_x_redt[i];
						y_redt[i] = init_y_redt[i];
						token_check[0][i] = false;
						collision = true;
						hit_rate[1]++;
						break;
					}
				}
			}
		}
		if( !collision )				//detecting collision with green tokens 
		{
			for( int i = 0 ; i < 4 ; i++ )
			{
				if( x_bluet[token_no] == x_greent[i] && y_bluet[token_no] == y_greent[i] )
				{
					if( (x_bluet[token_no] == 175 && y_bluet[token_no] == 280) || (x_bluet[token_no] == 375 && y_bluet[token_no] == 130) || (x_bluet[token_no] == 475 && y_bluet[token_no] == 80) || (x_bluet[token_no] == 625 && y_bluet[token_no] == 280) || (x_bluet[token_no] == 675 && y_bluet[token_no] == 380) || (x_bluet[token_no] == 475 && y_bluet[token_no] == 580) || (x_bluet[token_no] == 375 && y_bluet[token_no] == 580) || (x_bluet[token_no] == 225 && y_bluet[token_no] == 380) )
					{
						cout<<"Stop"<<endl;
						break;
					}
					else
					{
						x_greent[i] = init_x_greent[i];
						y_greent[i] = init_y_greent[i];
						token_check[3][i] = false;
						collision = true;
						hit_rate[1]++;
						break;
					}
				}
			}
		}
		
		if( x_bluet[0] == 425 && y_bluet[0] == 330 && x_bluet[1] == 425 && y_bluet[1] == 330 && x_bluet[2] == 425 && y_bluet[2] == 330 && x_bluet[3] == 425 && y_bluet[3] == 330 )
		{
			players[1] = true;
		}
    }
    else if( player_no == 3 )		//yellow player
    {
    	if( dice_roll == 6 && !token_check[player_no-1][token_no] )
    	{
    		x_yellowt[token_no] = 675;
    		y_yellowt[token_no] = 380; 
    		token_check[player_no-1][token_no] = true;
    	}
    	else if( token_check[player_no-1][token_no] )
    	{
    		int x_t=x_yellowt[token_no];
		int y_t=y_yellowt[token_no];
		int st = 0;
		int st_store=0;
		
		for( st=0;st<52;st++)
		{
			if((x_t==x_yellow_path[st] )&& (y_t==y_yellow_path[st]))
			{
					st_store=st;
					break;
			}
		}
		
		if( st_store + dice_roll < 54 )
		{
		
			st_store += dice_roll;
			st_store--;
			
			if( st_store > 46 && hit_rate[player_no-1] == 0 )
			{
				x_yellowt[token_no] = x_yellow_path[0];
				y_yellowt[token_no] = y_yellow_path[0];
			}
			else
			{
				x_yellowt[token_no] = x_yellow_path[st_store];
				y_yellowt[token_no] = y_yellow_path[st_store];
			}
		
		}
    	}
    	    	bool collision = false;
		
		if( !collision )				//detecting collision with  blue tokens 
		{
			for( int i = 0 ; i < 4 ; i++ )
			{
				if( x_yellowt[token_no] == x_bluet[i] && y_yellowt[token_no] == y_bluet[i] )
				{
				
					if((x_yellowt[token_no] == 175 && y_yellowt[token_no] == 280) || (x_yellowt[token_no] == 375 && y_yellowt[token_no] == 130) || (x_yellowt[token_no] == 475 && y_yellowt[token_no] == 80) || (x_yellowt[token_no] == 625 && y_yellowt[token_no] == 280) || (x_yellowt[token_no] == 675 && y_yellowt[token_no] == 380) || (x_yellowt[token_no] == 475 && y_yellowt[token_no] == 580) || (x_yellowt[token_no] == 375 && y_yellowt[token_no] == 580) || (x_yellowt[token_no] == 225 && y_yellowt[token_no] == 380 ) )
					{
						cout<<"Stop"<<endl;
						break;
					}
					else
					{
						x_bluet[i] = init_x_bluet[i];
						y_bluet[i] = init_y_bluet[i];
						token_check[1][i] = false;
						collision = true;
						hit_rate[2]++;
						break;
					}
				}
			}
		}
		if( !collision )				//detecting collision with  red tokens 
		{
			for( int i = 0 ; i < 4 ; i++ )
			{
				if( x_yellowt[token_no] == x_redt[i] && y_yellowt[token_no] == y_redt[i] )
				{
				
					if((x_yellowt[token_no] == 175 && y_yellowt[token_no] == 280) || (x_yellowt[token_no] == 375 && y_yellowt[token_no] == 130) || (x_yellowt[token_no] == 475 && y_yellowt[token_no] == 80) || (x_yellowt[token_no] == 625 && y_yellowt[token_no] == 280) || (x_yellowt[token_no] == 675 && y_yellowt[token_no] == 380) || (x_yellowt[token_no] == 475 && y_yellowt[token_no] == 580) || (x_yellowt[token_no] == 375 && y_yellowt[token_no] == 580) || (x_yellowt[token_no] == 225 && y_yellowt[token_no] == 380 ) ) 
					{
						cout<<"Stop"<<endl;
						break;
					}
					else
					{
						x_redt[i] = init_x_redt[i];
						y_redt[i] = init_y_redt[i];
						token_check[0][i] = false;
						collision = true;
						hit_rate[2]++;
						break;
					}
				}
			}
		}
		if( !collision )				//detecting collision with green tokens 
		{
			for( int i = 0 ; i < 4 ; i++ )
			{
				if( x_yellowt[token_no] == x_greent[i] && y_yellowt[token_no] == y_greent[i]  )
				{
					if( (x_yellowt[token_no] == 175 && y_yellowt[token_no] == 280) || (x_yellowt[token_no] == 375 && y_yellowt[token_no] == 130) || (x_yellowt[token_no] == 475 && y_yellowt[token_no] == 80) || (x_yellowt[token_no] == 625 && y_yellowt[token_no] == 280) || (x_yellowt[token_no] == 675 && y_yellowt[token_no] == 380) || (x_yellowt[token_no] == 475 && y_yellowt[token_no] == 580) || (x_yellowt[token_no] == 375 && y_yellowt[token_no] == 580) || (x_yellowt[token_no] == 225 && y_yellowt[token_no] == 380 ) )
					{
						cout<<"Stop"<<endl;
						break;
					}
					else
					{
						x_greent[i] = init_x_greent[i];
						y_greent[i] = init_y_greent[i];
						token_check[3][i] = false;
						collision = true;
						hit_rate[2]++;
						break;
					}
				}
			}
		}
		if( x_yellowt[0] == 425 && y_yellowt[0] == 330 && x_yellowt[1] == 425 && y_yellowt[1] == 330 && x_yellowt[2] == 425 && y_yellowt[2] == 330 && x_yellowt[3] == 425 && y_yellowt[3] == 330 )
		{
			players[2] = true;
		}
    
    }
    else if( player_no == 4 )		//green player
    {
    	if( dice_roll == 6 && !token_check[player_no-1][token_no] )
    	{
    		x_greent[token_no] = 375;
    		y_greent[token_no] = 580; 
    		token_check[player_no-1][token_no] = true;
    	}
    	else if( token_check[player_no-1][token_no] )
    	{
    		int x_t=x_greent[token_no];
		int y_t=y_greent[token_no];
		int st = 0;
		int st_store=0;
		
		for( st=0;st<52;st++)
		{
			if((x_t==x_green_path[st] )&& (y_t==y_green_path[st]))
			{
					st_store=st;
					break;
			}
		}
		
		if( st_store + dice_roll < 54 )
		{
		
			st_store += dice_roll;
			st_store--;
			
			if( st_store > 46 && hit_rate[player_no-1] == 0 )
			{
				x_greent[token_no] = x_green_path[0];
				y_greent[token_no] = y_green_path[0];
			}
			else
			{
				x_greent[token_no] = x_green_path[st_store];
				y_greent[token_no] = y_green_path[st_store];
			}
		}
    	}
    	    	bool collision = false;
		
		if( !collision )				//detecting collision with   yellow tokens 
		{
			for( int i = 0 ; i < 4 ; i++ )
			{
				if( x_greent[token_no] == x_yellowt[i] && y_greent[token_no] == y_yellowt[i] )
				{
					if( (x_greent[token_no] == 175 && y_greent[token_no] == 280) || (x_greent[token_no] == 375 && y_greent[token_no] == 130) || (x_greent[token_no] == 475 && y_greent[token_no] == 80) || (x_greent[token_no] == 625 && y_greent[token_no] == 280) || (x_greent[token_no] == 675 && y_greent[token_no] == 380) || (x_greent[token_no] == 475 && y_greent[token_no] == 580) || (x_greent[token_no] == 375 && y_greent[token_no] == 580) || (x_greent[token_no] == 225 && y_greent[token_no] == 380)  )
					{
						cout<<"Stop"<<endl;
						break;
					}
					else
					{
						x_yellowt[i] = init_x_yellowt[i];
						y_yellowt[i] = init_y_yellowt[i];
						token_check[2][i] = false;
						collision = true;
						hit_rate[3]++;
						break;
					}
				}
			}
		}
		if( !collision )				//detecting collision with  red tokens 
		{
			for( int i = 0 ; i < 4 ; i++ )
			{
				if( x_greent[token_no] == x_redt[i] && y_greent[token_no] == y_redt[i] )
				{
					if((x_greent[token_no] == 175 && y_greent[token_no] == 280) || (x_greent[token_no] == 375 && y_greent[token_no] == 130) || (x_greent[token_no] == 475 && y_greent[token_no] == 80) || (x_greent[token_no] == 625 && y_greent[token_no] == 280) || (x_greent[token_no] == 675 && y_greent[token_no] == 380) || (x_greent[token_no] == 475 && y_greent[token_no] == 580) || (x_greent[token_no] == 375 && y_greent[token_no] == 580) || (x_greent[token_no] == 225 && y_greent[token_no] == 380) )
					{
						cout<<"Stop"<<endl;
						break;
					}
					else
					{
						x_redt[i] = init_x_redt[i];
						y_redt[i] = init_y_redt[i];
						token_check[0][i] = false;
						collision = true;
						hit_rate[3]++;
						break;
					}
				}
			}
		}
		if( !collision )				//detecting collision with blue tokens 
		{
			for( int i = 0 ; i < 4 ; i++ )
			{
				if( x_greent[token_no] == x_bluet[i] && y_greent[token_no] == y_bluet[i] )
				{
					if( (x_greent[token_no] == 175 && y_greent[token_no] == 280) || (x_greent[token_no] == 375 && y_greent[token_no] == 130) || (x_greent[token_no] == 475 && y_greent[token_no] == 80) || (x_greent[token_no] == 625 && y_greent[token_no] == 280) || (x_greent[token_no] == 675 && y_greent[token_no] == 380) || (x_greent[token_no] == 475 && y_greent[token_no] == 580) || (x_greent[token_no] == 375 && y_greent[token_no] == 580) || (x_greent[token_no] == 225 && y_greent[token_no] == 380) )
					{
						cout<<"Stop"<<endl;
						break;
					}
					else
					{
						x_bluet[i] = init_x_bluet[i];
						y_bluet[i] = init_y_bluet[i];
						token_check[1][i] = false;
						collision = true;
						hit_rate[3]++;
						break;
					}
				}
			}
		}
		
		if( x_greent[0] == 425 && y_greent[0] == 330 && x_greent[1] == 425 && y_greent[1] == 330 && x_greent[2] == 425 && y_greent[2] == 330 && x_greent[3] == 425 && y_greent[3] == 330 )
		{
			players[3] = true;
		}
		
    
    }
        
        return;
        
	
}

void* player(void *arg)
{

//-------------------------------Die Rolling---------------------------------------------------

	 paras* object1 = (paras*)arg;
	 
	 pthread_mutex_lock(&lock1);
	 
	 srand(time(0));
    	 dice = (rand()%6)+1;
	 
	 pthread_mutex_unlock(&lock1);
	 
	 sem_wait(&lock2);
	 
//--------------------------------Token Selection-----------------------------------------------
	 
       int token_no = -1;
    if( dice == 6 )
    {
	    for( int i = 0 ; i < 4 ; i++ )
	    {
	    	if( !token_check[(object1->player_no)-1][i])
	    	{
	    		token_no = i;					//must lie in 0,1,2,3
	    		cout<<"i: "<<i<<endl;
	    		break;
	    	}
	  
	    }
	}
	else if( dice != 6 )
	{
	    
	    for( int i = 0 ; i < 4 ; i++ )
	    {
	    	if( token_check[(object1->player_no)-1][i] )
	    	{
	    		token_no = i;					//must lie in 0,1,2,3
	    		cout<<"i: "<<i<<endl;
	    		break;
	    	}
	    }
	}
       
       
           movement(token_no, dice, object1->player_no);
        
        sem_post(&lock2);
        
        
        pthread_exit(NULL);
	
}

void* master(void* arg)
{





	bool check[4] = {true,true,true,true};
	bool arr[4] = {true,false,false,false};
	sf::Font font;
	if (!font.loadFromFile("arial.ttf"))
	{
	    cout<<"Error in selecting font"<<endl;
	}
	//printing Turn on window
	sf::Text text;
	text.setFont(font);
	text.setString("Turn");
	text.setCharacterSize(24);
	text.setFillColor(sf::Color::White);
	text.setStyle(sf::Text::Bold);
	text.setPosition(35,280);

	sf::RenderWindow window(sf::VideoMode(900,700),"Ludo",sf::Style::Default);	//creating window
	
	sf::RectangleShape square[13];			//boxes of grid
	sf::RectangleShape box;			//turn box
	sf::RectangleShape texture(sf::Vector2f(47,47));
	texture.setPosition(425,330);
	sf::VertexArray triangle(sf::Triangles,3);
	sf::Texture home;
	home.loadFromFile("home.jpeg");
	texture.setTexture(&home);
	
	
	sf::CircleShape circle(20);	//object of circle creating
	
	while(window.isOpen())
	{
label:
		sf::Event ev;
		while(window.pollEvent(ev))
		{
			switch(ev.type)
			{
				case sf::Event::Closed:
				window.close();
				break;
				
			}
		}
		
		if( sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q) )
		{
			window.close();
		}
		
		window.clear();
		
//----------------------------------drawing grid box-------------------------------------------------------------

		box.setSize(sf::Vector2f(647, 647));
		box.setPosition(125,30);
		box.setFillColor(sf::Color::Black);
		box.setOutlineThickness(3);
		box.setOutlineColor(sf::Color::White);
		window.draw(box);

//----------------------------------creating horizontal grid boxes-----------------------------------------------		
		int x_axis = 125;
		int y_axis = 280;
		for( int j = 0 ; j < 3 ; j++ )
		{
			x_axis = 125;
			for( int i = 0 ; i < 13 ; i++ )
			{
				if( j == 0 && i == 1 )
				{
					square[i].setSize(sf::Vector2f(47, 47));
					square[i].setPosition(x_axis,y_axis);
					square[i].setFillColor(sf::Color::Red);
					square[i].setOutlineThickness(3);
					square[i].setOutlineColor(sf::Color::White);
					window.draw(square[i]);
				}
				else if( j == 1 && ( i == 1 || i == 2 ||i == 3 ||i == 4 ||i == 5) )
				{
					square[i].setSize(sf::Vector2f(47, 47));
					square[i].setPosition(x_axis,y_axis);
					square[i].setFillColor(sf::Color::Red);
					square[i].setOutlineThickness(3);
					square[i].setOutlineColor(sf::Color::White);
					window.draw(square[i]);
				}
				else if( j == 2 && i == 2 )
				{
					square[i].setSize(sf::Vector2f(47, 47));
					square[i].setPosition(x_axis,y_axis);
					square[i].setFillColor(sf::Color::Red);
					square[i].setOutlineThickness(3);
					square[i].setOutlineColor(sf::Color::White);
					window.draw(square[i]);
				}
				else if( j == 1 && ( i == 7 || i == 8 ||i == 9 ||i == 10 ||i == 11) )
				{
					square[i].setSize(sf::Vector2f(47, 47));
					square[i].setPosition(x_axis,y_axis);
					square[i].setFillColor(sf::Color::Yellow);
					square[i].setOutlineThickness(3);
					square[i].setOutlineColor(sf::Color::White);
					window.draw(square[i]);
				}
				else if( j == 0 && i == 10 )
				{
					square[i].setSize(sf::Vector2f(47, 47));
					square[i].setPosition(x_axis,y_axis);
					square[i].setFillColor(sf::Color::Yellow);
					square[i].setOutlineThickness(3);
					square[i].setOutlineColor(sf::Color::White);
					window.draw(square[i]);
				}
				else if( j == 2 && i == 11 )
				{
					square[i].setSize(sf::Vector2f(47, 47));
					square[i].setPosition(x_axis,y_axis);
					square[i].setFillColor(sf::Color::Yellow);
					square[i].setOutlineThickness(3);
					square[i].setOutlineColor(sf::Color::White);
					window.draw(square[i]);
				}
				else
				{
					square[i].setSize(sf::Vector2f(47, 47));
					square[i].setPosition(x_axis,y_axis);
					square[i].setFillColor(sf::Color::Black);
					square[i].setOutlineThickness(3);
					square[i].setOutlineColor(sf::Color::White);
					window.draw(square[i]);
				}
				x_axis += 50;
			}
			
			y_axis += 50;
		}
//------------------------------------------drawing vertical grid boxes---------------------------------

		x_axis = 375;
		y_axis = 30;
		for( int j = 0 ; j < 3 ; j++ )
		{
			y_axis = 30;
			for( int i = 0 ; i < 13 ; i++ )
			{
				if( j == 2 && i == 6 )
				{
					square[i].setSize(sf::Vector2f(47, 47));
					square[i].setPosition(x_axis,y_axis);
					square[i].setFillColor(sf::Color::Yellow);
					square[i].setOutlineThickness(3);
					square[i].setOutlineColor(sf::Color::White);
					window.draw(square[i]);
				}
				else if( j == 0 && i == 6 )
				{
					square[i].setSize(sf::Vector2f(47, 47));
					square[i].setPosition(x_axis,y_axis);
					square[i].setFillColor(sf::Color::Red);
					square[i].setOutlineThickness(3);
					square[i].setOutlineColor(sf::Color::White);
					window.draw(square[i]);
				}
				else if( j == 2 && i == 1 )
				{
					square[i].setSize(sf::Vector2f(47, 47));
					square[i].setPosition(x_axis,y_axis);
					square[i].setFillColor(sf::Color::Blue);
					square[i].setOutlineThickness(3);
					square[i].setOutlineColor(sf::Color::White);
					window.draw(square[i]);
				}
				else if( j == 1 && ( i == 1 || i == 2 ||i == 3 ||i == 4 ||i == 5) )
				{
					square[i].setSize(sf::Vector2f(47, 47));
					square[i].setPosition(x_axis,y_axis);
					square[i].setFillColor(sf::Color::Blue);
					square[i].setOutlineThickness(3);
					square[i].setOutlineColor(sf::Color::White);
					window.draw(square[i]);
				}
				else if( j == 0 && i == 2 )
				{
					square[i].setSize(sf::Vector2f(47, 47));
					square[i].setPosition(x_axis,y_axis);
					square[i].setFillColor(sf::Color::Blue);
					square[i].setOutlineThickness(3);
					square[i].setOutlineColor(sf::Color::White);
					window.draw(square[i]);
				}
				else if( j == 1 && ( i == 7 || i == 8 ||i == 9 ||i == 10 ||i == 11) )
				{
					square[i].setSize(sf::Vector2f(47, 47));
					square[i].setPosition(x_axis,y_axis);
					square[i].setFillColor(sf::Color::Green);
					square[i].setOutlineThickness(3);
					square[i].setOutlineColor(sf::Color::White);
					window.draw(square[i]);
				}
				else if( j == 0 && i == 11 )
				{
					square[i].setSize(sf::Vector2f(47, 47));
					square[i].setPosition(x_axis,y_axis);
					square[i].setFillColor(sf::Color::Green);
					square[i].setOutlineThickness(3);
					square[i].setOutlineColor(sf::Color::White);
					window.draw(square[i]);
				}
				else if( j == 2 && i == 10 )
				{
					square[i].setSize(sf::Vector2f(47, 47));
					square[i].setPosition(x_axis,y_axis);
					square[i].setFillColor(sf::Color::Green);
					square[i].setOutlineThickness(3);
					square[i].setOutlineColor(sf::Color::White);
					window.draw(square[i]);
				}
				else
				{
					square[i].setSize(sf::Vector2f(47, 47));
					square[i].setPosition(x_axis,y_axis);
					square[i].setFillColor(sf::Color::Black);
					square[i].setOutlineThickness(3);
					square[i].setOutlineColor(sf::Color::White);
					window.draw(square[i]);
				}
				y_axis += 50;
			}
			
			x_axis += 50;
		}

//------------------------------drawing home-----------------------------------------------------
		
//------------------------------drawing init tokens for red---------------------------------------

		x_axis = 150;
		y_axis = 100;
		
		if( check[0] == true )
		{
		
			for( int i = 0, k=0 ; i < 2 ; i++ )
			{
				x_axis = 150;
				for( int j = 0 ; j < 2 ; j++ )
				{
					
					x_redt[k] = x_axis;
					y_redt[k] = y_axis;
					
					init_x_redt[k] = x_axis;
					init_y_redt[k] = y_axis;
			
					k++;
					
					x_axis += 75;
				}
				y_axis += 75;
			}
			
			check[0] = false;
		}
		
		for( int i = 0 ; i < 4 ; i++ )
		{
			circle.setPosition(x_redt[i],y_redt[i]);
			circle.setFillColor(sf::Color::Red);
			circle.setOutlineThickness(3);
			circle.setOutlineColor(sf::Color::White);
			window.draw(circle);
		}

	

//------------------------------drawing init tokens for blue---------------------------------------

		x_axis = 640;
		y_axis = 100;
		
		if( check[1] == true )
		{
			for( int i = 0, k=0 ; i < 2 ; i++ )
			{
				x_axis = 640;
				for( int j = 0 ; j < 2 ; j++ )
				{
					
					x_bluet[k] = x_axis;
					y_bluet[k] = y_axis;
					
					init_x_bluet[k] = x_axis;
					init_y_bluet[k] = y_axis;
		
					k++;
					
					x_axis += 75;
				}
				y_axis += 75;
			}
			
			check[1] = false;
		}
		
		for( int i = 0 ; i < 4 ; i++ )
		{
			circle.setPosition(x_bluet[i],y_bluet[i]);
			circle.setFillColor(sf::Color::Blue);
			circle.setOutlineThickness(3);
			circle.setOutlineColor(sf::Color::White);
			window.draw(circle);
		}

//------------------------------drawing init tokens for green---------------------------------------

		
		x_axis = 150;
		y_axis = 500;
		
		if( check[2] == true )
		{
			for( int i = 0, k=0 ; i < 2 ; i++ )
			{
				x_axis = 150;
				for( int j = 0 ; j < 2 ; j++ )
				{
					
					x_greent[k] = x_axis;
					y_greent[k] = y_axis;
					
					init_x_greent[k] = x_axis;
					init_y_greent[k] = y_axis;
			
					k++;
					
					x_axis += 75;
				}
				y_axis += 75;
			}
			
			check[2] = false;
		}
		
		for( int i = 0 ; i < 4 ; i++ )
		{
			circle.setPosition(x_greent[i],y_greent[i]);
			circle.setFillColor(sf::Color::Green);
			circle.setOutlineThickness(3);
			circle.setOutlineColor(sf::Color::White);
			window.draw(circle);
		}	
		
//------------------------------drawing init tokens for yellow---------------------------------------

		x_axis = 640;
		y_axis = 500;
		
		if( check[3] == true )
		{
			for( int i = 0, k=0 ; i < 2 ; i++ )
			{
				x_axis = 640;
				for( int j = 0 ; j < 2 ; j++ )
				{
					
					x_yellowt[k] = x_axis;
					y_yellowt[k] = y_axis;
					
					init_x_yellowt[k] = x_axis;
					init_y_yellowt[k] = y_axis;
					
					k++;
					
					x_axis += 75;
				}
				y_axis += 75;
			}
			
			check[3] = false;
		}
		
		
		
		if( arr[0] )
		{
			
			
			box.setSize(sf::Vector2f(47, 47));
			box.setPosition(40,330);
			box.setFillColor(sf::Color::Red);
			box.setOutlineThickness(3);
			box.setOutlineColor(sf::Color::White);
			window.draw(box);
	
		}
		else if( arr[1] )
		{
			
			box.setSize(sf::Vector2f(47, 47));
			box.setPosition(40,330);
			box.setFillColor(sf::Color::Blue);
			box.setOutlineThickness(3);
			box.setOutlineColor(sf::Color::White);
			window.draw(box);
		}
		else if( arr[2] )
		{
			box.setSize(sf::Vector2f(47, 47));
			box.setPosition(40,330);
			box.setFillColor(sf::Color::Yellow);
			box.setOutlineThickness(3);
			box.setOutlineColor(sf::Color::White);
			window.draw(box);

		}
		else if( arr[3] )
		{
			
			box.setSize(sf::Vector2f(47, 47));
			box.setPosition(40,330);
			box.setFillColor(sf::Color::Green);
			box.setOutlineThickness(3);
			box.setOutlineColor(sf::Color::White);
			window.draw(box);
		}
		
		
		
		for( int i = 0 ; i < 4 ; i++ )
		{
			circle.setPosition(x_yellowt[i],y_yellowt[i]);
			circle.setFillColor(sf::Color::Yellow);
			circle.setOutlineThickness(3);
			circle.setOutlineColor(sf::Color::White);
			window.draw(circle);
		}	
		
		window.draw(text);
		window.draw(triangle);
		window.draw(texture);
		window.display();
		
		
		
	if( arr[0] == true && players[0] == false )
	{
		sleep(1);
		
		obj[0].player_no=1;  
		pthread_create(&tid[0],NULL,player,(void*)&obj[0]);
		arr[0] = false;
		arr[1] = true;
		
		goto label;
        }
        else if( arr[1] == true && players[1] == false )
        {
        	sleep(1);

		 obj[1].player_no=2;  
		pthread_create(&tid[1],NULL,player,(void*)&obj[1]);
		
		arr[1] = false;
		arr[2] = true;
		
		goto label;
        }
        else if( arr[2] == true && players[2] == false )
        {
        	sleep(1);
        	
		 obj[2].player_no=3;  
		pthread_create(&tid[2],NULL,player,(void*)&obj[2]);
		
		arr[2] = false;
		arr[3] = true;
		
		goto label;
        }
        else if( arr[3] == true && players[3] == false )
        {
        	sleep(1);
    
		 obj[3].player_no=4;  
		pthread_create(&tid[3],NULL,player,(void*)&obj[3]);
		arr[0] = true;
		arr[3] = false;
		
		
        }
		
	}

        pthread_exit(NULL);

}

int main(){

	sem_init(&lock2,0,4);

//----------------------------------saving path of red tokens--------------------------------------------------

	int j = 0;
	x_red_path[j] = 175;
	y_red_path[j] = 280;
	j++;
	for( int i = 0 ;  i < 4 ; i++ )
	{
		x_red_path[j] = x_red_path[j-1]+50;
		y_red_path[j] = y_red_path[j-1];
		j++;
	}
	
	x_red_path[j] = x_red_path[j-1];
	y_red_path[j] = y_red_path[j-1] - 50;
	j++;
	for( int i = 0 ;  i < 4 ; i++ )
	{
		x_red_path[j] = x_red_path[j-1];
		y_red_path[j] = y_red_path[j-1]-50;
		j++;
	}
	
	x_red_path[j] = x_red_path[j-1]+50;
	y_red_path[j] = y_red_path[j-1];
	j++;
	
	
	x_red_path[j] = x_red_path[j-1]+50;
	y_red_path[j] = y_red_path[j-1];
	j++;
	
	for( int i = 0 ;  i < 5 ; i++ )
	{
		x_red_path[j] = x_red_path[j-1];
		y_red_path[j] = y_red_path[j-1] + 50;
		j++;
	}
	
	x_red_path[j] = x_red_path[j-1]+50;
	y_red_path[j] = y_red_path[j-1];
	j++;
	
	for( int i = 0 ;  i < 4 ; i++ )
	{
		x_red_path[j] = x_red_path[j-1] + 50;
		y_red_path[j] = y_red_path[j-1];
		j++;
	}
	
	x_red_path[j] = x_red_path[j-1];
	y_red_path[j] = y_red_path[j-1] + 50;
	j++;

	
	x_red_path[j] = x_red_path[j-1];
	y_red_path[j] = y_red_path[j-1] + 50;
	j++;
	
	for( int i = 0 ;  i < 5 ; i++ )
	{
		x_red_path[j] = x_red_path[j-1] - 50;
		y_red_path[j] = y_red_path[j-1];
		j++;
	}
	
	x_red_path[j] = x_red_path[j-1];
	y_red_path[j] = y_red_path[j-1] + 50;
	j++;
	
	for( int i = 0 ;  i < 4 ; i++ )
	{
		x_red_path[j] = x_red_path[j-1];
		y_red_path[j] = y_red_path[j-1] + 50;
		j++;
	}
	
	x_red_path[j] = x_red_path[j-1] - 50;
	y_red_path[j] = y_red_path[j-1];
	j++;
	
	
	x_red_path[j] = x_red_path[j-1] - 50;
	y_red_path[j] = y_red_path[j-1];
	j++;
	
	for( int i = 0 ;  i < 5 ; i++ )
	{
		x_red_path[j] = x_red_path[j-1];
		y_red_path[j] = y_red_path[j-1] - 50;
		j++;
	}
	
	x_red_path[j] = x_red_path[j-1] - 50;
	y_red_path[j] = y_red_path[j-1];
	j++;
	
	for( int i = 0 ;  i < 4 ; i++ )
	{
		x_red_path[j] = x_red_path[j-1] - 50;
		y_red_path[j] = y_red_path[j-1];
		j++;
	}
	
	x_red_path[j] = x_red_path[j-1];
	y_red_path[j] = y_red_path[j-1] - 50;
	j++;
	
	for( int i = 0 ;  i < 6 ; i++ )
	{
		x_red_path[j] = x_red_path[j-1] + 50;
		y_red_path[j] = y_red_path[j-1];
		j++;
	}
	
//----------------------------------saving path of blue tokens--------------------------------------------------

	j = 0;
	x_blue_path[j] = 475;
	y_blue_path[j] = 80;
	j++;
	for( int i = 0 ;  i < 4 ; i++ )
	{
		x_blue_path[j] = x_blue_path[j-1];
		y_blue_path[j] = y_blue_path[j-1]+50;
		j++;
	}
	
	x_blue_path[j] = x_blue_path[j-1]+50;
	y_blue_path[j] = y_blue_path[j-1];
	j++;
	for( int i = 0 ;  i < 4 ; i++ )
	{
		x_blue_path[j] = x_blue_path[j-1]+50;
		y_blue_path[j] = y_blue_path[j-1];
		j++;
	}
	
	x_blue_path[j] = x_blue_path[j-1];
	y_blue_path[j] = y_blue_path[j-1]+50;
	j++;
	
	
	x_blue_path[j] = x_blue_path[j-1];
	y_blue_path[j] = y_blue_path[j-1]+50;
	j++;
	
	for( int i = 0 ;  i < 5 ; i++ )
	{
		x_blue_path[j] = x_blue_path[j-1]-50;
		y_blue_path[j] = y_blue_path[j-1];
		j++;
	}
	
	x_blue_path[j] = x_blue_path[j-1];
	y_blue_path[j] = y_blue_path[j-1]+50;
	j++;
	
	for( int i = 0 ;  i < 4 ; i++ )
	{
		x_blue_path[j] = x_blue_path[j-1];
		y_blue_path[j] = y_blue_path[j-1]+50;
		j++;
	}
	
	x_blue_path[j] = x_blue_path[j-1]-50;
	y_blue_path[j] = y_blue_path[j-1];
	j++;

	
	x_blue_path[j] = x_blue_path[j-1]-50;
	y_blue_path[j] = y_blue_path[j-1];
	j++;
	
	for( int i = 0 ;  i < 5 ; i++ )
	{
		x_blue_path[j] = x_blue_path[j-1];
		y_blue_path[j] = y_blue_path[j-1]-50;
		j++;
	}
	
	x_blue_path[j] = x_blue_path[j-1]-50;
	y_blue_path[j] = y_blue_path[j-1];
	j++;
	
	for( int i = 0 ;  i < 4 ; i++ )
	{
		x_blue_path[j] = x_blue_path[j-1]-50;
		y_blue_path[j] = y_blue_path[j-1];
		j++;
	}
	
	x_blue_path[j] = x_blue_path[j-1] ;
	y_blue_path[j] = y_blue_path[j-1]-50;
	j++;
	
	
	x_blue_path[j] = x_blue_path[j-1];
	y_blue_path[j] = y_blue_path[j-1]-50;
	j++;
	
	for( int i = 0 ;  i < 5 ; i++ )
	{
		x_blue_path[j] = x_blue_path[j-1]+50;
		y_blue_path[j] = y_blue_path[j-1];
		j++;
	}
	
	x_blue_path[j] = x_blue_path[j-1];
	y_blue_path[j] = y_blue_path[j-1]-50;
	j++;
	
	for( int i = 0 ;  i < 4 ; i++ )
	{
		x_blue_path[j] = x_blue_path[j-1];
		y_blue_path[j] = y_blue_path[j-1]-50;
		j++;
	}
	
	x_blue_path[j] = x_blue_path[j-1]+50;
	y_blue_path[j] = y_blue_path[j-1];
	j++;
	
	for( int i = 0 ;  i < 6 ; i++ )
	{
		x_blue_path[j] = x_blue_path[j-1];
		y_blue_path[j] = y_blue_path[j-1]+50;
		j++;
	}
	
//----------------------------------saving path of yellow tokens--------------------------------------------------

	j = 0;
	x_yellow_path[j] = 675;
	y_yellow_path[j] = 380;
	j++;
	for( int i = 0 ;  i < 4 ; i++ )
	{
		x_yellow_path[j] = x_yellow_path[j-1]-50;
		y_yellow_path[j] = y_yellow_path[j-1];
		j++;
	}
	
	x_yellow_path[j] = x_yellow_path[j-1];
	y_yellow_path[j] = y_yellow_path[j-1]+50;
	j++;
	for( int i = 0 ;  i < 4 ; i++ )
	{
		x_yellow_path[j] = x_yellow_path[j-1];
		y_yellow_path[j] = y_yellow_path[j-1]+50;
		j++;
	}
	
	x_yellow_path[j] = x_yellow_path[j-1]-50;
	y_yellow_path[j] = y_yellow_path[j-1];
	j++;
	
	
	x_yellow_path[j] = x_yellow_path[j-1]-50;
	y_yellow_path[j] = y_yellow_path[j-1];
	j++;
	
	for( int i = 0 ;  i < 5 ; i++ )
	{
		x_yellow_path[j] = x_yellow_path[j-1];
		y_yellow_path[j] = y_yellow_path[j-1]-50;
		j++;
	}
	
	x_yellow_path[j] = x_yellow_path[j-1]-50;
	y_yellow_path[j] = y_yellow_path[j-1];
	j++;
	
	for( int i = 0 ;  i < 4 ; i++ )
	{
		x_yellow_path[j] = x_yellow_path[j-1]-50;
		y_yellow_path[j] = y_yellow_path[j-1];
		j++;
	}
	
	x_yellow_path[j] = x_yellow_path[j-1];
	y_yellow_path[j] = y_yellow_path[j-1]-50;
	j++;

	
	x_yellow_path[j] = x_yellow_path[j-1];
	y_yellow_path[j] = y_yellow_path[j-1]-50;
	j++;
	
	for( int i = 0 ;  i < 5 ; i++ )
	{
		x_yellow_path[j] = x_yellow_path[j-1]+50;
		y_yellow_path[j] = y_yellow_path[j-1];
		j++;
	}
	
	x_yellow_path[j] = x_yellow_path[j-1];
	y_yellow_path[j] = y_yellow_path[j-1]-50;
	j++;
	
	for( int i = 0 ;  i < 4 ; i++ )
	{
		x_yellow_path[j] = x_yellow_path[j-1];
		y_yellow_path[j] = y_yellow_path[j-1]-50;
		j++;
	}
	
	x_yellow_path[j] = x_yellow_path[j-1] +50;
	y_yellow_path[j] = y_yellow_path[j-1];
	j++;
	
	
	x_yellow_path[j] = x_yellow_path[j-1]+50;
	y_yellow_path[j] = y_yellow_path[j-1];
	j++;
	
	for( int i = 0 ;  i < 5 ; i++ )
	{
		x_yellow_path[j] = x_yellow_path[j-1];
		y_yellow_path[j] = y_yellow_path[j-1]+50;
		j++;
	}
	
	x_yellow_path[j] = x_yellow_path[j-1]+50;
	y_yellow_path[j] = y_yellow_path[j-1];
	j++;
	
	for( int i = 0 ;  i < 4 ; i++ )
	{
		x_yellow_path[j] = x_yellow_path[j-1]+50;
		y_yellow_path[j] = y_yellow_path[j-1];
		j++;
	}
	
	x_yellow_path[j] = x_yellow_path[j-1];
	y_yellow_path[j] = y_yellow_path[j-1]+50;
	j++;
	
	for( int i = 0 ;  i < 6 ; i++ )
	{
		x_yellow_path[j] = x_yellow_path[j-1]-50;
		y_yellow_path[j] = y_yellow_path[j-1];
		j++;
	}
	
//----------------------------------saving path of green tokens--------------------------------------------------

	j = 0;
	x_green_path[j] = 375;
	y_green_path[j] = 580;
	j++;
	for( int i = 0 ;  i < 4 ; i++ )
	{
		x_green_path[j] = x_green_path[j-1];
		y_green_path[j] = y_green_path[j-1]-50;
		j++;
	}
	
	x_green_path[j] = x_green_path[j-1]-50;
	y_green_path[j] = y_green_path[j-1];
	j++;
	for( int i = 0 ;  i < 4 ; i++ )
	{
		x_green_path[j] = x_green_path[j-1]-50;
		y_green_path[j] = y_green_path[j-1];
		j++;
	}
	
	x_green_path[j] = x_green_path[j-1];
	y_green_path[j] = y_green_path[j-1]-50;
	j++;
	
	
	x_green_path[j] = x_green_path[j-1];
	y_green_path[j] = y_green_path[j-1]-50;
	j++;
	
	for( int i = 0 ;  i < 5 ; i++ )
	{
		x_green_path[j] = x_green_path[j-1]+50;
		y_green_path[j] = y_green_path[j-1];
		j++;
	}
	
	x_green_path[j] = x_green_path[j-1];
	y_green_path[j] = y_green_path[j-1]-50;
	j++;
	
	for( int i = 0 ;  i < 4 ; i++ )
	{
		x_green_path[j] = x_green_path[j-1];
		y_green_path[j] = y_green_path[j-1]-50;
		j++;
	}
	
	x_green_path[j] = x_green_path[j-1]+50;
	y_green_path[j] = y_green_path[j-1];
	j++;

	
	x_green_path[j] = x_green_path[j-1]+50;
	y_green_path[j] = y_green_path[j-1];
	j++;
	
	for( int i = 0 ;  i < 5 ; i++ )
	{
		x_green_path[j] = x_green_path[j-1];
		y_green_path[j] = y_green_path[j-1]+50;
		j++;
	}
	
	x_green_path[j] = x_green_path[j-1]+50;
	y_green_path[j] = y_green_path[j-1];
	j++;
	
	for( int i = 0 ;  i < 4 ; i++ )
	{
		x_green_path[j] = x_green_path[j-1]+50;
		y_green_path[j] = y_green_path[j-1];
		j++;
	}
	
	x_green_path[j] = x_green_path[j-1] ;
	y_green_path[j] = y_green_path[j-1]+50;
	j++;
	
	
	x_green_path[j] = x_green_path[j-1];
	y_green_path[j] = y_green_path[j-1]+50;
	j++;
	
	for( int i = 0 ;  i < 5 ; i++ )
	{
		x_green_path[j] = x_green_path[j-1]-50;
		y_green_path[j] = y_green_path[j-1];
		j++;
	}
	
	x_green_path[j] = x_green_path[j-1];
	y_green_path[j] = y_green_path[j-1]+50;
	j++;
	
	for( int i = 0 ;  i < 4 ; i++ )
	{
		x_green_path[j] = x_green_path[j-1];
		y_green_path[j] = y_green_path[j-1]+50;
		j++;
	}
	
	x_green_path[j] = x_green_path[j-1]-50;
	y_green_path[j] = y_green_path[j-1];
	j++;
	
	for( int i = 0 ;  i < 6 ; i++ )
	{
		x_green_path[j] = x_green_path[j-1];
		y_green_path[j] = y_green_path[j-1]-50;
		j++;
	}

//-------------------------------------------------------------------------------------------------------------

		pthread_create(&master_id,NULL,master,NULL);
        
        	cout<<endl;
        	cout<<endl;
        	pthread_join(master_id,NULL);	
        	
        	sem_destroy(&lock2);
        	
        return 0;

}
