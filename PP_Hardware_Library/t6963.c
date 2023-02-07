#include "t6963.h"
#include "config.h"

extern const unsigned char markPP_space[];
extern const unsigned char markPP_dot[];
extern const unsigned char markPP_plus[];
extern const unsigned char markPP_min[];
extern const unsigned char markPP_row[];
extern const unsigned char markPP_colon[];
extern const unsigned char markPP_pipe[];
extern const unsigned char markPP_greaterThan[];
extern const unsigned char markPP_lessThan[];
extern const unsigned char markPP_X[];
extern const unsigned char markPP_Y[];
extern const unsigned char markPP_Z[];
extern const unsigned char markPP_A[];
extern const unsigned char markPP_0[];
extern const unsigned char markPP_1[];
extern const unsigned char markPP_2[];
extern const unsigned char markPP_3[];
extern const unsigned char markPP_4[];
extern const unsigned char markPP_5[];
extern const unsigned char markPP_6[];
extern const unsigned char markPP_7[];
extern const unsigned char markPP_8[];
extern const unsigned char markPP_9[];
extern const unsigned char markPP_underline[];

const SFont infoFont[2]={{8,1},{0x18, 0x02}};


void LCDsetTextAtribiuteModeEnable(LCDdata* data, _Bool enable){
	
	data->textAtribiuteModeEnable=enable;
	
	if(data->textAtribiuteModeEnable)lcd_write_0cmd(0x84);	
	else{
		lcd_write_0cmd(0x80);	
		data->blink=false;
		data->bold=false;
		data->reverse=false;
		data->dependenceAtribiutesOnArea=false;
	}
}

void LCDsetCursorEnable(LCDdata* data, _Bool enable){
	
	data->cursorEnable=enable;
	
	if(data->cursorEnable){
		lcd_write_0cmd(0x9F);	//display ON + cursor
	}else{
		lcd_write_0cmd(0x9c);//display ON
	}

}

void LCDseekCursor(LCDdata *data, unsigned int cursorX, unsigned int cursorY){
	
	lcd_write_2cmd(cursorY,cursorX,0x21);
}


void LCDintro(LCDdata* data){
	extern const unsigned char logoZAPppLarge[]; 
	extern const unsigned char logoZAPbp[]; 
			
		LCDgotoGraph(data, 6, 28);
		send_icon_ram(data, logoZAPbp);


//		LCDgotoGraph(data, 1, 29);
//		send_icon_ram(data, logoZAPppLarge);

		delay_ms(3000);
		LCDclearPages(data, 0, 0);
}


void LCDputcharGraph(LCDdata *data, const char c) {
		
  if (c == '\n'){
		LCDgotoGraph(data, 0, data->posGraphY+infoFont[data->typeOfFont].height);
  }else if (c == '\r'){
		LCDgotoGraph(data, 0, data->posGraphY);
  }else if (c == '\t'){
		if(data->posGraphX+2<LCD_COLUMNS){
			LCDgotoGraph(data, data->posGraphX+2, data->posGraphY); 
		}
  }else{
		
		if (data->posGraphX+infoFont[data->typeOfFont].width>LCD_COLUMNS){
			LCDgotoGraph(data, 0, data->posGraphY+infoFont[data->typeOfFont].height); 
		} 
		
		if(data->typeOfFont==FONT_1){
			switch(c){	
						case 'X':	send_icon_ram(data, markPP_X); 
											break;
						case 'Y':	send_icon_ram(data, markPP_Y); 
											break;
						case 'Z':	send_icon_ram(data, markPP_Z); 
											break;
						case 'A':	send_icon_ram(data, markPP_A);  
											break;
						case '|':	send_icon_ram(data, markPP_pipe); 
											break;
						case '<':	send_icon_ram(data, markPP_lessThan); 
											break;
						case '>':	send_icon_ram(data, markPP_greaterThan); 
											break;
						case '+':	send_icon_ram(data, markPP_plus); 
											break;
						case '-':	send_icon_ram(data, markPP_min); 
											break;
						case '=':	send_icon_ram(data, markPP_row); 
											break;
						case ':':	send_icon_ram(data, markPP_colon); 
											break;
						case '0':	send_icon_ram(data, markPP_0); 
											break;					
						case '1':	send_icon_ram(data, markPP_1); 
											break;
						case '2':	send_icon_ram(data, markPP_2); 
											break;
						case '3':	send_icon_ram(data, markPP_3); 
											break;
						case '4':	send_icon_ram(data, markPP_4); 
											break;
						case '5':	send_icon_ram(data, markPP_5); 
											break;
						case '6':	send_icon_ram(data, markPP_6); 
											break;
						case '7':	send_icon_ram(data, markPP_7);  
											break;
						case '8':	send_icon_ram(data, markPP_8); 
											break;
						case '9':	send_icon_ram(data, markPP_9); 
											break;
						case '.':	send_icon_ram(data, markPP_dot); 
											break;
						case '~':	send_icon_ram(data, markPP_underline); 
											break;
						default: send_icon_ram(data, markPP_space); 
											break;
					}
				data->posGraphX+=infoFont[data->typeOfFont].width;
			}
  }
}


void LCDputchar(LCDdata *data, const unsigned char c) {


  if (c == '\n'){
		LCDgoto(data, 0, data->posY+1); 
  }else if (c == '\r'){  
		LCDgoto(data, 0, data->posY); 
  }else if (c == '\t'){
		if(data->posX+3<LCD_COLUMNS){
			LCDgoto(data, data->posX+3, data->posY); 
		}
  }else{
		if (data->posX+1>LCD_COLUMNS){
			LCDgoto(data, 0, data->posY+1); 
		} 
		
		unsigned int pos=TA+(data->posY*LCD_COLUMNS)+data->posX;
		lcd_write_2cmd(((pos>>8) & 0xff),(pos & 0xff),0x24);//address pointer set 
		
		if(c>=0x80){
			lcd_write_1cmd(c,0xc0);
		}else{
			lcd_write_1cmd(c-0x20,0xc0);
		}
		
		if(!data->dependenceAtribiutesOnArea)LCDsetTextAtribiuteOnTheArea(data, data->posX, data->posY, 1, 1);
		
		(data->posX)++;
	}
}


void LCDsetTextAtribiuteOnTheArea(LCDdata *data, unsigned int posX, unsigned int posY, unsigned int width, unsigned int height){
	
		if(data->textAtribiuteModeEnable){
			unsigned int pos=GA+(posY*LCD_COLUMNS)+posX;
			lcd_write_2cmd(((pos>>8) & 0xff),(pos & 0xff),0x24);//address pointer set 
			
			for(int i=0;i<height;i++){
				
				for(int j=0;j<width;j++){ 
					if(data->reverse){
						if(data->blink){
							lcd_write_1cmd(0xD,0xc0);
						}else{
							lcd_write_1cmd(0x5,0xc0);
						}
					}else{
						if(data->bold && data->blink) lcd_write_1cmd(0xF,0xc0);
						else if(data->bold)lcd_write_1cmd(0x7,0xc0);
						else if(data->blink)lcd_write_1cmd(0x8,0xc0);
						else lcd_write_1cmd(0x0,0xc0);
					}
					pos++;
				}

				pos+=(LCD_COLUMNS-width);
				lcd_write_2cmd(((pos>>8) & 0xff),(pos & 0xff),0x24);
			}
		}
}

void LCDclearTextAtribiuteOnTheArea(LCDdata *data, unsigned int posX, unsigned int posY, unsigned int width, unsigned int height){
	
		if(data->textAtribiuteModeEnable){
			unsigned int pos=GA+(posY*LCD_COLUMNS)+posX;
			lcd_write_2cmd(((pos>>8) & 0xff),(pos & 0xff),0x24);//address pointer set 
			
			lcd_write_0cmd(0xb0);//data auto write
			for(int i=0;i<height;i++){
				for(int j=0;j<width;j++){ 
					while((lcd_read_status()&8)!=8);
					lcd_write_data(0x0);
					pos++;
				}
				lcd_write_0cmd(0xb2);//reset auto write
				pos+=(LCD_COLUMNS-width);
				lcd_write_2cmd(((pos>>8) & 0xff),(pos & 0xff),0x24);
				lcd_write_0cmd(0xb0);//data auto write
			}
			lcd_write_0cmd(0xb2);//reset auto write
			
		}
}


void LCDgoto(LCDdata *data, unsigned int lcd_X, unsigned int lcd_Y) {
	
	data->posX=lcd_X;
	data->posY=lcd_Y;
	
	LCDgotoFontGraph(data, lcd_X, lcd_Y);
	
}

void LCDgotoPage(LCDdata *data, unsigned int page){

	if(page<LCD_PAGES){
		unsigned int pos=TA+page*(LCD_ROWS*LCD_COLUMNS); 
		lcd_write_2cmd(((pos>>8) & 0xff),(pos & 0xff),0x40);
		data->posX=0;
		data->posY=0;	
	}
	
	LCDgotoGraphPage(data, page);
	
}

void LCDclear(LCDdata *data, unsigned int x_start, unsigned int y_start, unsigned int width, unsigned int height) {
  init_text_ram(x_start, y_start, width, height);//clear VRAM
	
	data->blink=false;
	data->bold=false;
	data->reverse=false;
	LCDsetTextAtribiuteOnTheArea(data, x_start, y_start, width, height);
	
	
	LCDgoto(data, x_start, y_start);
	
}

void LCDclearPages(LCDdata *data, unsigned int firstPage, unsigned int lastPage){
		unsigned int noOfPage=lastPage-firstPage+1;
	
		LCDclear(data, 0, firstPage*LCD_ROWS, LCD_COLUMNS, noOfPage*LCD_ROWS);
	
		LCDclearPagesGraph(data, firstPage, lastPage);
}

void LCDgotoFontGraph(LCDdata *data, unsigned int lcd_X, unsigned int lcd_Y) {

	data->posGraphX=lcd_X*infoFont[data->typeOfFont].width;
	data->posGraphY=lcd_Y*infoFont[data->typeOfFont].height;
}

void LCDclearFontGraph(LCDdata *data, unsigned int x_start, unsigned int y_start, unsigned int width, unsigned int height) {

	init_graph_ram(x_start*infoFont[data->typeOfFont].width, y_start*infoFont[data->typeOfFont].height, width*infoFont[data->typeOfFont].width, height*infoFont[data->typeOfFont].height);
	LCDgotoFontGraph(data, x_start, y_start);
}






void LCDgotoGraph(LCDdata *data, unsigned int lcd_X, unsigned int lcd_Y) {

	data->posGraphX=lcd_X;
	data->posGraphY=lcd_Y;
}

void LCDgotoGraphPage(LCDdata *data, unsigned int page){

	if(page<LCD_PAGES_GRAPH){
		unsigned int pos;
		if(data->textAtribiuteModeEnable==false){
			pos=GA+page*(LCD_ROWS_GRAPH*LCD_COLUMNS); 
		}else{
			pos=GA+page*(LCD_ROWS*LCD_COLUMNS); 
		}
		lcd_write_2cmd(((pos>>8) & 0xff),(pos & 0xff),0x42);//graphic home adress set 
		data->posGraphX=0;
		data->posGraphY=0;
		
	}

}

void LCDclearGraph(LCDdata *data, unsigned int x_start, unsigned int y_start, unsigned int width, unsigned int height) {

	init_graph_ram(x_start, y_start, width, height);
	LCDgotoGraph(data, x_start, y_start);
}

void LCDclearPagesGraph(LCDdata *data, unsigned int firstPage, unsigned int lastPage){
		unsigned int noOfPage=lastPage-firstPage+1;
	
		LCDclearGraph(data, 0, firstPage*LCD_ROWS_GRAPH, LCD_COLUMNS, noOfPage*LCD_ROWS_GRAPH);
}



void lcd_init(void){
	unsigned int area;
	unsigned int charPP_a[8]={0x00, 0x0e, 0x01, 0x0f, 0x11, 0x0f, 0x02, 0x03};		//ą
	unsigned int charPP_c[8]={0x02, 0x04, 0x00, 0x0e, 0x10, 0x10, 0x0e, 0x00};		//ć
	unsigned int charPP_e[8]={0x00, 0x0e, 0x11, 0x1f, 0x10, 0x0e, 0x04, 0x06};		//ę
	unsigned int charPP_l[8]={0x0c, 0x04, 0x04, 0x06, 0x0c, 0x04, 0x0e, 0x00};		//ł
	unsigned int charPP_n[8]={0x02, 0x04, 0x00, 0x16, 0x19, 0x11, 0x11, 0x00};		//ń
	unsigned int charPP_o[8]={0x02, 0x04, 0x00, 0x0e, 0x11, 0x11, 0x0e, 0x00};		//ó
	unsigned int charPP_s[8]={0x02, 0x04, 0x00, 0x0f, 0x10, 0x0e, 0x01, 0x1e};		//ś
	unsigned int charPP_z[8]={0x00, 0x04, 0x00, 0x1e, 0x04, 0x08, 0x1e, 0x00};		//ż
	unsigned int charPP_x[8]={0x02, 0x04, 0x00, 0x1e, 0x04, 0x08, 0x1e, 0x00};		//ź
	unsigned int charPP_u[8]={0x00,0x00,0x12,0x12,0x12,0x12,0x1D,0x10};					//µ
	unsigned int charPP_2[8]={0x06,0x09,0x02,0x04,0x0F,0x00,0x00,0x00};					//2
	unsigned int charPP_s6[8]={0x00, 0x04, 0x02, 0x7f, 0x02, 0x04, 0x00, 0x00};		//->
	unsigned int charPP_s4[8]={0x00, 0x10, 0x20, 0x7f, 0x20, 0x10, 0x00, 0x00};		//<-
	unsigned int charPP_s9[8]={0x0f, 0x03, 0x05, 0x09, 0x10, 0x20, 0x00, 0x00};		
	unsigned int charPP_s1[8]={0x01, 0x02, 0x24, 0x28, 0x30, 0x3c, 0x00, 0x00};		
	unsigned int charPP_s8[8]={0x08, 0x1c, 0x2a, 0x08, 0x08, 0x08, 0x08, 0x00};		
	unsigned int charPP_s2[8]={0x08, 0x08, 0x08, 0x08, 0x2a, 0x1c, 0x08, 0x00};		
	unsigned int charPP_ok[8]={0x03, 0x07, 0x8e, 0xdc, 0xf8, 0x70, 0x20, 0x00};		
	unsigned int charPP_circle[8]={0x00, 0x3c, 0x66, 0x42, 0x42, 0x66, 0x3c, 0x00};		
	unsigned int charPP_square[8]={0x00, 0x7e, 0x42, 0x42, 0x42, 0x42, 0x7e, 0x00};		
	unsigned int charPP_progress[8]={0x00, 0x3e, 0x3e, 0x3e, 0x3e, 0x3e, 0x3e, 0x00};		
	unsigned int charPP_underline[8]={0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00};	
	unsigned int charPP_right_arrow[8]={0x10, 0x18, 0x1c, 0x1e, 0x1c, 0x18, 0x10, 0x00};
	unsigned int charPP_round[8]={0x00,0x08,0x15,0x02,0x08,0x15,0x02,0x00};
	unsigned int charPP_backspace[8]={0x00,0x00,0x3F,0x75,0xFB,0x75,0x3F,0x00};
	unsigned int charPP_mm[8]={0x1A,0x15,0x15,0x00,0x68,0x54,0x54,0x00};
	unsigned int charPP_um[8]={0x1A,0x15,0x15,0x00,0x50,0x50,0x68,0x40};
	unsigned int charPP_div_s[8]={0x08,0x10,0x17,0x28,0x26,0x41,0x4E,0x00};
	unsigned int charPP_omega[8]={0x00,0x00,0x0A,0x11,0x15,0x15,0x0A,0x00};
	unsigned int charPP_copyright[8]={0x3E,0x63,0x5D,0x51,0x5D,0x63,0x3E,0x00};
	
	lcd_hwd_init();
	
	lcd_write_0cmd(0x80);		//OR, internal CG mode
	area=GA;
	lcd_write_2cmd(((area>>8) & 0xff),(area & 0xff),0x42);//graphic home adress set
	lcd_write_2cmd(0,LCD_COLUMNS,0x43);				//graphic area 30*8=240 
	area=TA;
	lcd_write_2cmd(((area>>8) & 0xff),(area & 0xff),0x40);//text home address
	lcd_write_2cmd(0,LCD_COLUMNS,0x41);			//text area 30*8=240
	lcd_write_2cmd(0,15,0x22);//offset external CG RAM 
	
	//special character indexes start with 0x80 (in case uses internal CG mode)
	init_CGRAM(charPP_a, 8, 0xA0);
	init_CGRAM(charPP_c, 8, 0xA1);
	init_CGRAM(charPP_e, 8, 0xA2);
	init_CGRAM(charPP_l, 8, 0xA3);
	init_CGRAM(charPP_n, 8, 0xA4);
	init_CGRAM(charPP_o, 8, 0xA5);
	init_CGRAM(charPP_s, 8, 0xA6);
	init_CGRAM(charPP_z, 8, 0xA7);
	init_CGRAM(charPP_x, 8, 0xA8);
	init_CGRAM(charPP_s6, 8, 0xA9);
	init_CGRAM(charPP_s4, 8, 0xAA);
	init_CGRAM(charPP_s9, 8, 0xAB);
	init_CGRAM(charPP_s1, 8, 0xAC);
	init_CGRAM(charPP_s8, 8, 0xAD);
	init_CGRAM(charPP_s2, 8, 0xAE);
	init_CGRAM(charPP_ok, 8, 0xAF);
	init_CGRAM(charPP_circle, 8, 0xB0);
	init_CGRAM(charPP_square, 8, 0xB1);
	init_CGRAM(charPP_progress, 8, 0xB2);
	init_CGRAM(charPP_underline, 8, 0xB3);
	init_CGRAM(charPP_right_arrow, 8, 0xB4);
	
	init_CGRAM(charPP_u, 8, 0xB5);
	init_CGRAM(charPP_2, 8, 0xB6);
	init_CGRAM(charPP_round, 8, 0xB7);
	init_CGRAM(charPP_backspace, 8, 0xB8);
	init_CGRAM(charPP_mm, 8, 0xB9);
	init_CGRAM(charPP_um, 8, 0xBA);
	init_CGRAM(charPP_div_s, 8, 0xBB);
	init_CGRAM(charPP_omega, 8, 0xBC);
	init_CGRAM(charPP_copyright, 8, 0xBD);
	
	init_text_ram(0, 0, LCD_COLUMNS, LCD_MAX_ROWS);
	init_graph_ram(0, 0, LCD_COLUMNS, LCD_MAX_ROWS_GRAPH);
	
//	lcd_write_2cmd(0,0x2,0x50);//set blink time =0.5s
	
	lcd_write_0cmd(0xA1);//cursor pattern select =2 lines
	
  lcd_write_0cmd(0x9c);//display ON
	
}

void init_CGRAM(const unsigned int *sign, unsigned int size, unsigned int id){
	
	unsigned int pos=CG_RAM+(id*8);
	
	lcd_write_2cmd(((pos>>8) & 0xFF),(pos & 0xFF),0x24);
	lcd_write_0cmd(0xb0);//data auto write
	for(int i=0;i<size;i++){
		while((lcd_read_status()&8)!=8);
		lcd_write_data(sign[i]);
	}
	lcd_write_0cmd(0xb2);//reset auto write
}

void init_text_ram(unsigned int x_start, unsigned int y_start, unsigned int width, unsigned int height){
	unsigned int px_start=0;
	
	px_start=TA;
	px_start+=x_start;
	px_start+=y_start*LCD_COLUMNS;

	lcd_write_2cmd(((px_start>>8) & 0xff),(px_start & 0xff),0x24);//address pointer set 
	lcd_write_0cmd(0xb0);//data auto write
	for(int i=0;i<height;i++){
		for(int j=0;j<width;j++){ 
			while((lcd_read_status()&8)!=8);
			lcd_write_data(0);
			px_start++;
		}
		lcd_write_0cmd(0xb2);//reset auto write
		px_start=px_start+(LCD_COLUMNS-width);
		lcd_write_2cmd(((px_start>>8) & 0xff),(px_start & 0xff),0x24);//address pointer set
		lcd_write_0cmd(0xb0);//data auto write 
	}
	lcd_write_0cmd(0xb2);//reset auto write

}

void init_graph_ram(unsigned int x_start, unsigned int y_start, unsigned int width, unsigned int height){
	unsigned int px_start=0;
	
	px_start=GA;
	px_start+=x_start;
	px_start+=y_start*LCD_COLUMNS;

	lcd_write_2cmd(((px_start>>8) & 0xff),(px_start & 0xff),0x24);//address pointer set 
	lcd_write_0cmd(0xb0);//data auto write
	for(int i=0;i<height;i++){
		for(int j=0;j<width;j++){ 
			while((lcd_read_status()&8)!=8);
			lcd_write_data(0);
			px_start++;
		}
		lcd_write_0cmd(0xb2);//reset auto write
		px_start=px_start+(LCD_COLUMNS-width);
		lcd_write_2cmd(((px_start>>8) & 0xff),(px_start & 0xff),0x24);//address pointer set
		lcd_write_0cmd(0xb0);//data auto write 
	}
	lcd_write_0cmd(0xb2);//reset auto write	
}



void send_icon_ram(LCDdata *data, const unsigned char *buffer){
	unsigned int iz, jz, point=0;
	
	iz=*buffer++;
	jz=*buffer++;
	point=GA+(data->posGraphY*LCD_COLUMNS)+data->posGraphX;
	lcd_write_2cmd(((point>>8) & 0xff),(point & 0xff),0x24);//address pointer set 
	
	lcd_write_0cmd(0xb0);//data auto write
	for(int i=0;i<iz;i++){
		for(int j=0;j<jz;j++){ 
			while((lcd_read_status()&8)!=8);
			lcd_write_data(*buffer++);
			point++;
		}
		lcd_write_0cmd(0xb2);//reset auto write
		point=point+(LCD_COLUMNS-jz);
		lcd_write_2cmd(((point>>8) & 0xff),(point & 0xff),0x24);//address pointer set
		lcd_write_0cmd(0xb0);//data auto write 
	}
	lcd_write_0cmd(0xb2);//reset auto write
	
}


void lcd_write_0cmd(unsigned char cmd){
	while((lcd_read_status()&3)!=3);
	lcd_write_cmd(cmd);
}


void lcd_write_1cmd(unsigned char arg, unsigned char cmd){
 	while((lcd_read_status()&3)!=3);
	lcd_write_data(arg);
	while((lcd_read_status()&3)!=3); 
	lcd_write_cmd(cmd);
}

void lcd_write_2cmd(unsigned char arg1, unsigned char arg2, unsigned char cmd){
 	while((lcd_read_status()&3)!=3);
	lcd_write_data(arg2);
	while((lcd_read_status()&3)!=3);
	lcd_write_data(arg1);
	while((lcd_read_status()&3)!=3);
	lcd_write_cmd(cmd);
}


//***************************************************************
//hardware procedures
//***************************************************************

void lcd_hwd_init(void){

	PIN_CLR(LCD_PORT, RES);	
	delay_ms(50);
	PIN_SET(LCD_PORT, RES);
	PIN_SET(LCD_PORT, (WR|RD|CD|CE));	delay(2);
	
}


void lcd_write_data(unsigned char data){
 	PIN_CLR(LCD_PORT, CD);
	PIN_SET(LCD_PORT, RD);	delay(2);
	wr_lcd_bus(data);
}

void lcd_write_cmd(unsigned char cmd){
		PIN_SET(LCD_PORT, CD);	
		PIN_SET(LCD_PORT, RD);	delay(2);   
		wr_lcd_bus(cmd);
}

unsigned char lcd_read_data(void){
		PIN_CLR(LCD_PORT, CD);
		PIN_SET(LCD_PORT, WR);	delay(2); 
		return(rd_lcd_bus());
}


unsigned char lcd_read_status(void){
		PIN_SET(LCD_PORT, CD);
		PIN_SET(LCD_PORT, WR);	delay(2); 
		return(rd_lcd_bus());
}


unsigned char rd_lcd_bus(void){
		unsigned char data;
		DataLinesIn(); 
		PIN_CLR(LCD_PORT, CE);	
		PIN_CLR(LCD_PORT, RD);	delay(2); 
		data=LCD_PORT->IDR&LCD_DB; 	
		PIN_SET(LCD_PORT, RD);	
		PIN_SET(LCD_PORT, CE);	
		return(data);
}


void wr_lcd_bus(unsigned char data){
		DataLinesOut(); 
		LCD_PORT->ODR&=~LCD_DB;
		LCD_PORT->ODR|=data;
		delay(5);
		PIN_CLR(LCD_PORT, CE);	
		PIN_CLR(LCD_PORT, WR); 	delay(2);
		PIN_SET(LCD_PORT, WR);	
		PIN_SET(LCD_PORT, CE); 	
}


static void DataLinesIn(void) {

	PIN_CLR(LCD_PORT, LCD_DB);

	IN_SET_REG(LCD_PORT, Pin0, IO_IN_PU);	
	IN_SET_REG(LCD_PORT, Pin1, IO_IN_PU);
	IN_SET_REG(LCD_PORT, Pin2, IO_IN_PU);
	IN_SET_REG(LCD_PORT, Pin3, IO_IN_PU);
	IN_SET_REG(LCD_PORT, Pin4, IO_IN_PU);
	IN_SET_REG(LCD_PORT, Pin5, IO_IN_PU);
	IN_SET_REG(LCD_PORT, Pin6, IO_IN_PU);
	IN_SET_REG(LCD_PORT, Pin7, IO_IN_PU);

}

static void DataLinesOut(void) {

	OUT_SET_REG(LCD_PORT, Pin0, IO_OUT_HS);	
	OUT_SET_REG(LCD_PORT, Pin1, IO_OUT_HS);
	OUT_SET_REG(LCD_PORT, Pin2, IO_OUT_HS);
	OUT_SET_REG(LCD_PORT, Pin3, IO_OUT_HS);
	OUT_SET_REG(LCD_PORT, Pin4, IO_OUT_HS);
	OUT_SET_REG(LCD_PORT, Pin5, IO_OUT_HS);
	OUT_SET_REG(LCD_PORT, Pin6, IO_OUT_HS);
	OUT_SET_REG(LCD_PORT, Pin7, IO_OUT_HS);

}
