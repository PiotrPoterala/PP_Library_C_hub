#include "pp_lcdgraph.h"


void PLCDgraphConstruct(PLCDgraph *lcd, uPin *ioPins, PPortsData *ioPorts){
	
			LCDsetIO(&lcd->data, ioPins, ioPorts);
			lcd_init(&lcd->data);
			LCDreset(&lcd->data, 1);
	
			lcd->setFont=LCDsetFont;
			lcd->setTextAtribiuteModeEnable=LCDsetTextAtribiuteModeEnable;
			lcd->setReverse=LCDsetReverse;
			lcd->setBlink=LCDsetBlink;
			lcd->writeTextAtribiutesOnTheArea=LCDwriteTextAtribiutesOnTheArea;
			lcd->setIndependentWriteTextAtribiutes=LCDsetIndependentWriteTextAtribiutes;
			lcd->clearTextAtribiuteOnTheArea=LCDclearTextAtribiuteOnTheArea;
			lcd->setCursorEnable=LCDsetCursorEnable;
			lcd->seekCursor=LCDseekCursor;
	
			lcd->intro=LCDintro;
			lcd->reset=LCDreset;
	
			lcd->seek=LCDgoto;
			lcd->seekPage=LCDgotoPage;
			lcd->clear=LCDclear;
			lcd->clearPages=LCDclearPages;
			
			lcd->write=LCDwrite;
			lcd->writeChar=LCDputchar;
			lcd->writeFromString=LCDwriteFromString;
	
			lcd->drawIcon=send_icon_ram;	
			
}

void LCDreset(LCDdata *data, unsigned int noOfPageToClear){
	
		LCDsetFont(data, BASE_FONT);
		LCDclearPages(data, 0, noOfPageToClear);
		LCDgotoPage(data, 0);
		LCDsetTextAtribiuteModeEnable(data, false);
		LCDseekCursor(data, LCD_COLUMNS, 0);
		LCDsetCursorEnable(data, false);
	
}


void LCDwrite(LCDdata* data, const char *s) {

		while(*s){ 
			LCDputchar(data, *(s++));
		}

}

void LCDwriteFromString(LCDdata* data, const char *s, unsigned int num) {

	int numOfWriteChars=0;
	
	while(*s){ 
		if(numOfWriteChars>=num)break;
		LCDputchar(data, *(s++));
		numOfWriteChars++;
	}

}


