//LED EFFECTS//
//(w_przerwaniu_od_przycisku_po_ruchu)
//prawdopodobnie wszedzie trzeba bedzie i tak se indeksy w setledzie pozmieniac bo bedziemy pewnie mieli inaczej odwrocone ledy 
for(int i=0;i<64;i++)
setled(i,0,0,0);
//1.defaultowe podswietlenie tam gdzie stoja figury
for(int i=0;i<8;i++)
{
	for(int j=0;j<8;j++)
	{
	if(current_board[i][j]>10)
	{
		setled(i*8+j,100,0,0);  //wybor koloru dla czarnych
	}
	else if(current_board[i][j]<10 && current_board[i][j]>0)
	{
		setled(i*8+j,0,0,100); //wybor koloru dla bialych
	}
	}	
}
//2.ostatni ruch

setled(rowmovedfrom*8+columnmovedfrom,0,160,0);
setled(rowmovedto*8+columnmovedto,0,160,0);

//3.efekt przy zbiciu

if(!found)
{
	//tu kolory moga byc na odwrot 
	if(current_player==0)
	{
	
	
	
	}
	else
	{
		
		
	}
}


///////////////
