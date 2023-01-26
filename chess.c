#include "MKL05Z4.h" 
#include "stdbool.h"
#include "lcd1602.h"
#include "frdm_bsp.h"
#include "string.h"
/*

0-empty
01-white pawn
11-black pawn
02-white knight
12-black knight
03-white bishop
13-black bishop
04-white rook
14-black rook
05-white queen
15-black queen
06-white king
16-black king
*/

uint8_t current_board[8][8]=
{
{14,12,13,15,16,13,12,14},
{11,11,11,11,11,11,11,11},
{0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0},
{01,01,01,01,01,01,01,01},
{04,02,03,05,06,03,02,04}
};
//czy prev board w ogole jest potrzebny?
uint8_t prev_board[8][8]=
{
{14,12,13,15,16,13,12,14},
{11,11,11,11,11,11,11,11},
{0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0},
{01,01,01,01,01,01,01,01},
{04,02,03,05,06,03,02,04}
};

//char display[]={0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20};

bool current_detect_board[8][8];
bool prev_detect_board [8][8];
bool current_player;

uint8_t rowmovedto,columnmovedto;
uint8_t rowmovedfrom,columnmovedfrom;
uint8_t possibilities;
int subarray [8][8];

uint8_t countsetbits(uint8_t n)
{
	uint8_t count=0;
	while(n)
	{
		n&=(n-1);
		count++;
		
	}
	return count;
	
}

uint8_t checksetbit(uint8_t n)
{
	bool found=false;
	int i=0;
	while(!found)
	{
		if((n&(1<<i))==(1<<i))
		{
			found=true;
		}
	}
	return i;
}


void substract(bool arr1[8][8],bool arr2[8][8])
{
	//tu tez sprawdzic czy nie musi byc globalnie deklarowana czy cos 
	
		for(int i=0;i<8;i++)
	{
		for(int j=0;j<8;j++)
		{
			subarray[i][j]=arr1[i][j]-arr2[i][j];
		}
	}
	}

void copyprevdetect()
{
		for(int i=0;i<8;i++)
	{
		for(int j=0;j<8;j++)
		{
			current_detect_board[i][j]=prev_detect_board[i][j];
		}
	}
	
}

void copyprevboard()
{
	
		for(int i=0;i<8;i++)
	{
		for(int j=0;j<8;j++)
		{
			prev_board[i][j]=current_board[i][j];
		}
	}
	
	
}
	
bool  search_array(int arr [8][8])
{
	bool found=false;
for(int i=0;i<8;i++)
	{
		for(int j=0;j<8;j++)
		{
			if(arr[i][j]==1)
			{
				rowmovedto=i;
				columnmovedto=j;
				found=true;
			}
			else if(arr[i][j]==-1)
			{
				rowmovedfrom=i;
				columnmovedfrom=j;
			}
			//sprawdz czy to dobrze
		}
	}
return found;
}

struct rowcolumnpair{uint8_t row;uint8_t column;};

void analyze()
{
	struct rowcolumnpair movelist[8];
	copyprevboard();
	possibilities=0;
	//detect the piece position here 
	substract(current_detect_board,prev_detect_board);
	if(search_array(subarray))
	{
		//nic nie zostalo zbite, jeszcze problem moze byc z rozszada, en passant, promocja
	current_board[rowmovedfrom][columnmovedfrom]=0;
		current_board[rowmovedto][columnmovedto]=prev_board[rowmovedfrom][columnmovedfrom];
	}
	else
	{
		bool found=false;
		bool enemyfound=false;
		uint8_t x=1,y=1;
		switch(prev_board[rowmovedfrom][columnmovedfrom])
		{
			
			case 01: //pawn
			if(columnmovedfrom>0)
			{
				//option1 taking left
				if(current_player==0)
				{
				if(prev_board[rowmovedfrom+1][columnmovedfrom-1]>10)
					enemyfound=1;
				}
				else
				{
				if(prev_board[rowmovedfrom+1][columnmovedfrom-1]>0 && prev_board[rowmovedfrom+1][columnmovedfrom-1]<10)
					enemyfound=1;
				}
				if(enemyfound==1)
				{
				possibilities=1|possibilities;
				movelist[0].row=rowmovedfrom+1;
			  movelist[0].column=columnmovedfrom-1;
				}
			}
			if(columnmovedfrom<7)
			{
				//option2 taking right
				if(current_player==0)
				{
				if(prev_board[rowmovedfrom+1][columnmovedfrom+1]>10)
					enemyfound=1;
				}
				else
					{
				if(prev_board[rowmovedfrom+1][columnmovedfrom+1]<10 && prev_board[rowmovedfrom+1][columnmovedfrom+1]>0)
					enemyfound=1;
				}
					if(enemyfound==1)
					{
				possibilities=2|possibilities;
				movelist[1].row=rowmovedfrom+1;
			  movelist[1].column=columnmovedfrom+1;
					}
			}
			
			break;
			case 02: //knight
				if(columnmovedfrom>0 && rowmovedfrom>1)
			{
				//option1 up left
				if(current_player==0)
				{
				if(prev_board[rowmovedfrom-3][columnmovedfrom-1]>10)
					enemyfound=1;
				}
				else
				{
				if(prev_board[rowmovedfrom-3][columnmovedfrom-1]<10 && prev_board[rowmovedfrom-3][columnmovedfrom-1]>0)
					enemyfound=1;
			}
				if(enemyfound==1)
				{
				possibilities=1|possibilities;
				movelist[0].row=rowmovedfrom-3;
			  movelist[0].column=columnmovedfrom-1;
				}
			}
			if(columnmovedfrom<7 && rowmovedfrom>1)
			{
				//option2 up right
				if(current_player==0)
				{
				if(prev_board[rowmovedfrom-3][columnmovedfrom+1]>10)
					enemyfound=1;
				}
				else
				{
					if(prev_board[rowmovedfrom-3][columnmovedfrom+1]<10 && prev_board[rowmovedfrom-3][columnmovedfrom+1]>0)
						enemyfound=1;
				}
				if(enemyfound==1)
				{
				possibilities=2|possibilities;
				movelist[1].row=rowmovedfrom-3;
			  movelist[1].column=columnmovedfrom+1;
				}
			}
						if(columnmovedfrom<6 && rowmovedfrom>0)
			{
				//option3 right up
				if(current_player==0)
				{
				if(prev_board[rowmovedfrom-1][columnmovedfrom+3]>10)
				enemyfound=1;
				}
				else
				{
					if(prev_board[rowmovedfrom-1][columnmovedfrom+3]<10 && prev_board[rowmovedfrom-1][columnmovedfrom+3]>0)
						enemyfound=1;
				}
				if(enemyfound==1)
				{
				possibilities=4|possibilities;
				movelist[2].row=rowmovedfrom-1;
			  movelist[2].column=columnmovedfrom+3;
				}
			}
						if(columnmovedfrom<6 && rowmovedfrom<7)
			{
				//option4 right down 
				if(current_player==0)
				{
				if(prev_board[rowmovedfrom+1][columnmovedfrom+3]>10)
					enemyfound=1;
				}
				else
				{
					if(prev_board[rowmovedfrom+1][columnmovedfrom+3]<10 && prev_board[rowmovedfrom+1][columnmovedfrom+3]>0)
						enemyfound=1;
				}
				if(enemyfound==1)
				{
				possibilities=8|possibilities;
				movelist[3].row=rowmovedfrom+1;
			  movelist[3].column=columnmovedfrom+3;
				}
			}
						if(columnmovedfrom<7 && rowmovedfrom<6)
			{
				//option5 down right
				if(current_player==0)
				{
				if(prev_board[rowmovedfrom+3][columnmovedfrom+1]>10)
					enemyfound=1;
				}
				else
				{
					if(prev_board[rowmovedfrom+3][columnmovedfrom+1]<10 && prev_board[rowmovedfrom+3][columnmovedfrom+1]>0)
						enemyfound=1;
				}
				if(enemyfound==1)
				{
				possibilities=16|possibilities;
				movelist[4].row=rowmovedfrom+3;
			  movelist[4].column=columnmovedfrom+1;
				}
			}
						if(columnmovedfrom>0 && rowmovedfrom<6)
			{
				//option6 down left
				if(current_player==0)
				{
				if(prev_board[rowmovedfrom+3][columnmovedfrom-1]>10)
					enemyfound=1;
				}
				else
				{
					if(prev_board[rowmovedfrom+3][columnmovedfrom-1]<10 && prev_board[rowmovedfrom+3][columnmovedfrom-1]>0)
						enemyfound=1;
				}
				if(enemyfound==1)
				{
				possibilities=32|possibilities;
				movelist[5].row=rowmovedfrom+3;
			  movelist[5].column=columnmovedfrom-1;
				}
			}
						if(columnmovedfrom>1 && rowmovedfrom<7)
			{
				//option7 left down
				if(current_player==0)
				{
				if(prev_board[rowmovedfrom+1][columnmovedfrom-3]>10)
					enemyfound=1;
				}
				else
				{
					if(prev_board[rowmovedfrom+1][columnmovedfrom-3]<10 && prev_board[rowmovedfrom+1][columnmovedfrom-3]>0)
						enemyfound=1;
				}
				if(enemyfound==1)
				{
				possibilities=64|possibilities;
				movelist[6].row=rowmovedfrom+1;
			  movelist[6].column=columnmovedfrom-3;
				}
			}
						if(columnmovedfrom>1 && rowmovedfrom>0)
			{
				//option8 left up
				if(current_player==0)
				{
				if(prev_board[rowmovedfrom-1][columnmovedfrom-3]>10)
					enemyfound=1;
				}
				else
				{
					if(prev_board[rowmovedfrom-1][columnmovedfrom-3]<10 && prev_board[rowmovedfrom-1][columnmovedfrom-3]>0)
						enemyfound=1;
				}
				if(enemyfound==1)
				{
				possibilities=128|possibilities;
				movelist[7].row=rowmovedfrom-1;
			  movelist[7].column=columnmovedfrom-3;
				}
			}
			break;
			
			case 03: //bishop
			while(!found && rowmovedfrom+y<8 && columnmovedfrom+x<8)
			{
				if(current_player==0)
				{
				if(prev_board[rowmovedfrom+y][columnmovedfrom+x]>10)
					enemyfound=1;
				}
				else
				{
					if(prev_board[rowmovedfrom+y][columnmovedfrom+x]<10 && prev_board[rowmovedfrom+y][columnmovedfrom+x]>0)
						enemyfound=1;
				}
				if(enemyfound==1)
				{
					possibilities=1|possibilities;
					movelist[0].row=rowmovedfrom+y;
					movelist[0].column=columnmovedfrom+x;
					found=true;
				}
				else 
				{
				 x++;
				 y++;
				}	
			}
			found=false;
			x=1;y=1;
			while(!found && rowmovedfrom+y<8 && columnmovedfrom-x>=0)
			{
				if(current_player==0)
				{
				if(prev_board[rowmovedfrom+y][columnmovedfrom-x]>10)
						enemyfound=1;
				}
				else
				{
					if(prev_board[rowmovedfrom+y][columnmovedfrom-x]<10 && prev_board[rowmovedfrom+y][columnmovedfrom-x]>0)
						enemyfound=1;
				}
				if(enemyfound==1)
				{
					possibilities=2|possibilities;
					movelist[1].row=rowmovedfrom+y;
					movelist[1].column=columnmovedfrom-x;
					found=true;
				}
				else 
				{
				 x++;
				 y++;
				}	
			}
			found=false;
			x=1;y=1;
			while(!found && rowmovedfrom-y>=0 && columnmovedfrom+x<8)
			{
				if(current_player==0)
				{
				if(prev_board[rowmovedfrom-y][columnmovedfrom+x]>10)
						enemyfound=1;
				}
				else
				{
					if(prev_board[rowmovedfrom-y][columnmovedfrom+x]<10 && prev_board[rowmovedfrom-y][columnmovedfrom+x]>0)
						enemyfound=1;
				}
				if(enemyfound==1)
				
				{
					possibilities=4|possibilities;
					movelist[2].row=rowmovedfrom-y;
					movelist[2].column=columnmovedfrom+x;
					found=true;
				}
				else 
				{
				 x++;
				 y++;
				}	
			}
			found=false;
			x=1;y=1;
				while(!found && rowmovedfrom-y>=0 && columnmovedfrom-x>=0)
			{
				if(current_player==0)
				{
				if(prev_board[rowmovedfrom-y][columnmovedfrom-x]>10)
						enemyfound=1;
				}
				else
				{
					if(prev_board[rowmovedfrom-y][columnmovedfrom-x]<10 && prev_board[rowmovedfrom-y][columnmovedfrom-x]>0)
						enemyfound=1;
				}
				if(enemyfound==1)
				
				{
					possibilities=8|possibilities;
					movelist[3].row=rowmovedfrom-y;
					movelist[3].column=columnmovedfrom-x;
					found=true;
				}
				else 
				{
				 x++;
				 y++;
				}	
			}
				break;
				
				
			case 04: //rook
				while(!found && rowmovedfrom+y<8)
			{
				if(current_player==0)
				{
				if(prev_board[rowmovedfrom+y][columnmovedfrom]>10)
						enemyfound=1;
				}
				else
				{
					if(prev_board[rowmovedfrom+y][columnmovedfrom]<10 && prev_board[rowmovedfrom+y][columnmovedfrom]>0)
						enemyfound=1;
				}
				if(enemyfound==1)
				{
					possibilities=1|possibilities;
					movelist[0].row=rowmovedfrom+y;
					movelist[0].column=columnmovedfrom;
					found=true;
				}
				else 
				{
				 y++;
				}	
			}
			found=false;
			x=1;y=1;
			while(!found && rowmovedfrom-y>=0)
			{
				if(current_player==0)
				{
				if(prev_board[rowmovedfrom-y][columnmovedfrom]>10)
						enemyfound=1;
				}
				else
				{
					if(prev_board[rowmovedfrom-y][columnmovedfrom]<10 && prev_board[rowmovedfrom-y][columnmovedfrom]>0)
						enemyfound=1;
				}
				if(enemyfound==1)
				{
					possibilities=2|possibilities;
					movelist[1].row=rowmovedfrom-y;
					movelist[1].column=columnmovedfrom;
					found=true;
				}
				else 
				{
				 y++;
				}	
			}
			found=false;
			x=1;y=1;
			while(!found && columnmovedfrom+x<8)
			{
				if(current_player==0)
				{
				if(prev_board[rowmovedfrom][columnmovedfrom+x]>10)
						enemyfound=1;
				}
				else
				{
					if(prev_board[rowmovedfrom][columnmovedfrom+x]<10 && prev_board[rowmovedfrom][columnmovedfrom+x]>0)
						enemyfound=1;
				}
				if(enemyfound==1)
				{
					possibilities=4|possibilities;
					movelist[2].row=rowmovedfrom;
					movelist[2].column=columnmovedfrom+x;
					found=true;
				}
				else 
				{
				 x++;
				}	
			}
			found=false;
			x=1;y=1;
				while(!found && columnmovedfrom-x>=0)
			{
				if(current_player==0)
				{
				if(prev_board[rowmovedfrom][columnmovedfrom-x]>10)
						enemyfound=1;
				}
				else
				{
					if(prev_board[rowmovedfrom][columnmovedfrom-x]<10 && prev_board[rowmovedfrom][columnmovedfrom-x]>0)
						enemyfound=1;
				}
				if(enemyfound==1)
				{
					possibilities=8|possibilities;
					movelist[3].row=rowmovedfrom;
					movelist[3].column=columnmovedfrom-x;
					found=true;
				}
				else 
				{
				 x++;
				}	
			}
				break;
			case 05: //queen
				while(!found && rowmovedfrom+y<8 && columnmovedfrom+x<8)
			{
				if(current_player==0)
				{
				if(prev_board[rowmovedfrom+y][columnmovedfrom+x]>10)
						enemyfound=1;
				}
				else
				{
					if(prev_board[rowmovedfrom+y][columnmovedfrom+x]<10 && prev_board[rowmovedfrom+y][columnmovedfrom+x]>0)
						enemyfound=1;
				}
				if(enemyfound==1)
				{
					possibilities=1|possibilities;
					movelist[0].row=rowmovedfrom+y;
					movelist[0].column=columnmovedfrom+x;
					found=true;
				}
				else 
				{
				 x++;
				 y++;
				}	
			}
			found=false;
			x=1;y=1;
			while(!found && rowmovedfrom+y<8 && columnmovedfrom-x>=0)
			{
				if(current_player==0)
				{
				if(prev_board[rowmovedfrom+y][columnmovedfrom-x]>10)
						enemyfound=1;
				}
				else
				{
					if(prev_board[rowmovedfrom+y][columnmovedfrom-x]<10 && prev_board[rowmovedfrom+y][columnmovedfrom-x]>0)
						enemyfound=1;
				}
				if(enemyfound==1)
				{
					possibilities=2|possibilities;
					movelist[1].row=rowmovedfrom+y;
					movelist[1].column=columnmovedfrom-x;
					found=true;
				}
				else 
				{
				 x++;
				 y++;
				}	
			}
			found=false;
			x=1;y=1;
			while(!found && rowmovedfrom-y>=0 && columnmovedfrom+x<8)
			{
				if(current_player==0)
				{
				if(prev_board[rowmovedfrom-y][columnmovedfrom+x]>10)
						enemyfound=1;
				}
				else
				{
					if(prev_board[rowmovedfrom-y][columnmovedfrom+x]<10 && prev_board[rowmovedfrom-y][columnmovedfrom+x]>0)
						enemyfound=1;
				}
				if(enemyfound==1)
				{
					possibilities=4|possibilities;
					movelist[2].row=rowmovedfrom-y;
					movelist[2].column=columnmovedfrom+x;
					found=true;
				}
				else 
				{
				 x++;
				 y++;
				}	
			}
			found=false;
			x=1;y=1;
				while(!found && rowmovedfrom-y>=0 && columnmovedfrom-x>=0)
			{
				if(current_player==0)
				{
				if(prev_board[rowmovedfrom-y][columnmovedfrom-x]>10)
						enemyfound=1;
				}
				else
				{
					if(prev_board[rowmovedfrom-y][columnmovedfrom-x]<10 && prev_board[rowmovedfrom-y][columnmovedfrom-x]>0)
						enemyfound=1;
				}
				if(enemyfound==1)
				{
					possibilities=8|possibilities;
					movelist[3].row=rowmovedfrom-y;
					movelist[3].column=columnmovedfrom-x;
					found=true;
				}
				else 
				{
				 x++;
				 y++;
				}	
			}
				found=false;
			x=1;y=1;
				while(!found && rowmovedfrom+y<8)
			{
				if(current_player==0)
				{
				if(prev_board[rowmovedfrom+y][columnmovedfrom]>10)
						enemyfound=1;
				}
				else
				{
					if(prev_board[rowmovedfrom+y][columnmovedfrom]<10 && prev_board[rowmovedfrom+y][columnmovedfrom]>0)
						enemyfound=1;
				}
				if(enemyfound==1)
				{
					possibilities=16|possibilities;
					movelist[4].row=rowmovedfrom+y;
					movelist[4].column=columnmovedfrom;
					found=true;
				}
				else 
				{
				 y++;
				}	
			}
			found=false;
			x=1;y=1;
			while(!found && rowmovedfrom-y>=0)
			{
				if(current_player==0)
				{
				if(prev_board[rowmovedfrom-y][columnmovedfrom]>10)
						enemyfound=1;
				}
				else
				{
					if(prev_board[rowmovedfrom-y][columnmovedfrom]<10 && prev_board[rowmovedfrom-y][columnmovedfrom]>0)
						enemyfound=1;
				}
				if(enemyfound==1)
				{
					possibilities=32|possibilities;
					movelist[5].row=rowmovedfrom-y;
					movelist[5].column=columnmovedfrom;
					found=true;
				}
				else 
				{
				 y++;
				}	
			}
			found=false;
			x=1;y=1;
			while(!found && columnmovedfrom+x<8)
			{
				if(current_player==0)
				{
				if(prev_board[rowmovedfrom][columnmovedfrom+x]>10)
						enemyfound=1;
				}
				else
				{
					if(prev_board[rowmovedfrom][columnmovedfrom+x]<10 && prev_board[rowmovedfrom][columnmovedfrom+x]>0)
						enemyfound=1;
				}
				if(enemyfound==1)
				{
					possibilities=64|possibilities;
					movelist[6].row=rowmovedfrom;
					movelist[6].column=columnmovedfrom+x;
					found=true;
				}
				else 
				{
				 x++;
				}	
			}
			found=false;
			x=1;y=1;
				while(!found && columnmovedfrom-x>=0)
			{
				if(current_player==0)
				{
				if(prev_board[rowmovedfrom][columnmovedfrom-x]>10)
						enemyfound=1;
				}
				else
				{
					if(prev_board[rowmovedfrom][columnmovedfrom-x]<10 && prev_board[rowmovedfrom][columnmovedfrom-x]>0)
						enemyfound=1;
				}
				if(enemyfound==1)
				{
					possibilities=128|possibilities;
					movelist[7].row=rowmovedfrom;
					movelist[7].column=columnmovedfrom-x;
					found=true;
				}
				else 
				{
				 x++;
				}	
			}
				break;
			case 06: //king
					if( rowmovedfrom+1<8 && columnmovedfrom+1<8)
			{
				if(current_player==0)
				{
				if(prev_board[rowmovedfrom+1][columnmovedfrom+1]>10)
						enemyfound=1;
				}
				else
				{
					if(prev_board[rowmovedfrom+1][columnmovedfrom+1]<10 && prev_board[rowmovedfrom+1][columnmovedfrom+1]>0)
						enemyfound=1;
				}
				if(enemyfound==1)
				{
					possibilities=1|possibilities;
					movelist[0].row=rowmovedfrom+1;
					movelist[0].column=columnmovedfrom+1;
				}
			}
			if (rowmovedfrom+1<8 && columnmovedfrom-1>=0)
			{
				if(current_player==0)
				{
				if(prev_board[rowmovedfrom+1][columnmovedfrom-1]>10)
						enemyfound=1;
				}
				else
				{
					if(prev_board[rowmovedfrom+1][columnmovedfrom-1]<10 && prev_board[rowmovedfrom+1][columnmovedfrom-1]>0)
						enemyfound=1;
				}
				if(enemyfound==1)
				{
					possibilities=2|possibilities;
					movelist[1].row=rowmovedfrom+y;
					movelist[1].column=columnmovedfrom-x;

				}
			}
			if(rowmovedfrom-1>=0 && columnmovedfrom+1<8)
			{
				if(current_player==0)
				{
				if(prev_board[rowmovedfrom-1][columnmovedfrom+1]>10)
						enemyfound=1;
				}
				else
				{
					if(prev_board[rowmovedfrom-1][columnmovedfrom+1]<10 && prev_board[rowmovedfrom-1][columnmovedfrom+1]>0)
						enemyfound=1;
				}
				if(enemyfound==1)
				{
					possibilities=4|possibilities;
					movelist[2].row=rowmovedfrom-1;
					movelist[2].column=columnmovedfrom+1;

				}
			}
				if(rowmovedfrom-1>=0 && columnmovedfrom-1>=0)
			{
				if(current_player==0)
				{
				if(prev_board[rowmovedfrom-1][columnmovedfrom-1]>10)
						enemyfound=1;
				}
				else
				{
					if(prev_board[rowmovedfrom-1][columnmovedfrom-1]<10 && prev_board[rowmovedfrom-1][columnmovedfrom-1]>0)
						enemyfound=1;
				}
				if(enemyfound==1)
				{
					possibilities=8|possibilities;
					movelist[3].row=rowmovedfrom-1;
					movelist[3].column=columnmovedfrom-1;
				}
			}
				if(rowmovedfrom+1<8)
			{
				if(current_player==0)
				{
				if(prev_board[rowmovedfrom+1][columnmovedfrom]>10)
						enemyfound=1;
				}
				else
				{
					if(prev_board[rowmovedfrom+1][columnmovedfrom]<10 && prev_board[rowmovedfrom+1][columnmovedfrom]>0)
						enemyfound=1;
				}
				if(enemyfound==1)
				{
					possibilities=16|possibilities;
					movelist[4].row=rowmovedfrom+1;
					movelist[4].column=columnmovedfrom;
				}
			}
			if(rowmovedfrom-1>=0)
			{
				if(current_player==0)
				{
				if(prev_board[rowmovedfrom-1][columnmovedfrom]>10)
						enemyfound=1;
				}
				else
				{
					if(prev_board[rowmovedfrom-1][columnmovedfrom]<10 && prev_board[rowmovedfrom-1][columnmovedfrom]>0)
						enemyfound=1;
				}
				if(enemyfound==1)
				{
					possibilities=32|possibilities;
					movelist[5].row=rowmovedfrom-1;
					movelist[5].column=columnmovedfrom;
				}
			}

			if(columnmovedfrom+1<8)
			{
				if(current_player==0)
				{
				if(prev_board[rowmovedfrom][columnmovedfrom+1]>10)
						enemyfound=1;
				}
				else
				{
					if(prev_board[rowmovedfrom][columnmovedfrom+1]<10 && prev_board[rowmovedfrom][columnmovedfrom+1]>0)
						enemyfound=1;
				}
				if(enemyfound==1)
				{
					possibilities=64|possibilities;
					movelist[6].row=rowmovedfrom;
					movelist[6].column=columnmovedfrom+1;

				}
			}

				if(columnmovedfrom-1>=0)
			{
				if(current_player==0)
				{
				if(prev_board[rowmovedfrom][columnmovedfrom-1]>10)
						enemyfound=1;
				}
				else
				{
					if(prev_board[rowmovedfrom][columnmovedfrom-1]<10 && prev_board[rowmovedfrom][columnmovedfrom-1]>0)
						enemyfound=1;
				}
				if(enemyfound==1)
				{
					possibilities=128|possibilities;
					movelist[7].row=rowmovedfrom;
					movelist[7].column=columnmovedfrom-1;
				}
			}
				break;
		}
		//sprawdzic jak sie te playery przelaczaja czy nie jest tak ze najpierw sie zmieni player a potem dopiero sie posprawdza te zbicia!
		if(countsetbits(possibilities)>1)
			{
				  //pytasiecozostalozbite
			}
			else
			{
				current_board[rowmovedfrom][columnmovedfrom]=0;
				current_board[movelist[checksetbit(possibilities)].row][movelist[checksetbit(possibilities)].column]=prev_board[rowmovedfrom][columnmovedfrom];
			}
	}

	copyprevdetect();
}
