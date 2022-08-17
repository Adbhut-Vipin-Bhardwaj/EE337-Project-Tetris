#include <at89c5131.h>
#include "lcd.h"		//Header file with LCD interfacing functions
// global vars
unsigned char tile_num; // 0 to 8
//unsigned char tile_width[] = {1,2, 2, 1,2, 2, 2, 1,2};
//														*|**|* |*|**|**| *| |  
//														 |	|**|*|* | *|**|*|**
//tile_nums:									0| 1| 2|3| 4| 5| 6|7| 8
unsigned char curr_pos; // 0 to 15
char r1[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; // top row    | 1 => filled
char r2[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; // bottom row | 0 => empty
unsigned char lfsr = 0x01; // 0000(b3)(b2)(b1)(b0)

void uart_init(void)
{
	TMOD=0x20;			//Configure Timer 1 in Mode 2
	TH1=204;					//Load TH1 to obtain require Baudrate (Refer Serial.pdf for calculations)
	SCON=0x50;			//Configure UART peripheral for 8-bit data transfer 
	TR1 = 1;								//Start Timer 1
	IEN0 = 0x90;									//Enable Serial Interrupt
										//Enable Global Interrupt
}
//
char get_new_tile(void){
	char b_three = lfsr & 0x08; // 0000(b3)000
	char b_zero = ((lfsr << 3) & 0x08); // 0000(b0)000
	lfsr = (lfsr >> 1) ^ (b_three ^ b_zero); // update lfsr
	lfsr = (lfsr & 0x0F); // update lfsr
	return (lfsr % 3);
}
//
bit fun_can_move(void){
	bit res = 1;
	if(curr_pos == 0){
		return 0;
	}
	/*
	if (tile_num==0 || tile_num==1){
		if(r1[curr_pos-1]){res = 0;}
	}else if (tile_num==2 || tile_num==3 || tile_num==4 || tile_num==5 || tile_num==6){
		if(r1[curr_pos-1] || r2[curr_pos-1]){res = 0;}
	}else{
		if(r2[curr_pos-1]){res = 0;}
	}
	*/
	switch(tile_num){
		case 0:{
			if(r1[curr_pos-1]){res = 0;}
			break;
		}
		case 1:{
			if(r1[curr_pos-1]){res = 0;}
			break;
		}
		case 2:{
			if(r1[curr_pos-1] || r2[curr_pos-1]){res = 0;}
			break;
		}
		case 3:{
			if(r1[curr_pos-1] || r2[curr_pos-1]){res = 0;}
			break;
		}
		case 4:{
			if(r1[curr_pos-1] || r2[curr_pos-1]){res = 0;}
			break;
		}
		case 5:{
			if(r1[curr_pos-1] || r2[curr_pos]){res = 0;}
			break;
		}
		case 6:{
			if(r1[curr_pos] || r2[curr_pos-1]){res = 0;}
			break;
		}
		case 7:{
			if(r2[curr_pos-1]){res = 0;}
			break;
		}
		case 8:{
			if(r2[curr_pos-1]){res = 0;}
			break;
		}
	}
	return res;
}
//
void move_left(void){
	switch(tile_num){
		case 0:{
			lcd_cmd(128 + curr_pos - 1); lcd_write_char('*'); lcd_write_char(' ');
			break;
		}
		case 1:{
			lcd_cmd(128 + curr_pos - 1); lcd_write_char('*');
			lcd_cmd(128 + curr_pos + 1); lcd_write_char(' ');
			break;
		}
		case 2:{
			lcd_cmd(128 + curr_pos - 1); lcd_write_char('*'); lcd_write_char(' ');
			lcd_cmd(192 + curr_pos - 1); lcd_write_char('*');
			lcd_cmd(192 + curr_pos + 1); lcd_write_char(' ');
			break;
		}
		case 3:{
			lcd_cmd(128 + curr_pos - 1); lcd_write_char('*'); lcd_write_char(' ');
			lcd_cmd(192 + curr_pos - 1); lcd_write_char('*'); lcd_write_char(' ');
			break;
		}
		case 4:{
			lcd_cmd(128 + curr_pos - 1); lcd_write_char('*');
			lcd_cmd(128 + curr_pos + 1); lcd_write_char(' ');
			lcd_cmd(192 + curr_pos - 1); lcd_write_char('*'); lcd_write_char(' ');
			break;
		}
		case 5:{
			lcd_cmd(128 + curr_pos - 1); lcd_write_char('*');
			lcd_cmd(128 + curr_pos + 1); lcd_write_char(' ');
			lcd_cmd(192 + curr_pos); lcd_write_char('*'); lcd_write_char(' ');
			break;
		}
		case 6:{
			lcd_cmd(128 + curr_pos); lcd_write_char('*'); lcd_write_char(' ');
			lcd_cmd(192 + curr_pos - 1); lcd_write_char('*');
			lcd_cmd(192 + curr_pos + 1); lcd_write_char(' ');
			break;
		}
		case 7:{
			lcd_cmd(192 + curr_pos - 1); lcd_write_char('*'); lcd_write_char(' ');
			break;
		}
		case 8:{
			lcd_cmd(192 + curr_pos - 1); lcd_write_char('*');
			lcd_cmd(192 + curr_pos + 1); lcd_write_char(' ');
			break;
		}
	}
}
//
void update_r(void){
	/*
	if (tile_num != 7 && tile_num != 8){
		r1[curr_pos] = 1;
	}
	if (tile_num != 0 && tile_num != 1){
		r2[curr_pos] = 1;
	}
	if (tile_num==1 || tile_num==4 || tile_num==5 || tile_num==6){
		r1[curr_pos + 1] = 1;
	}
	if (tile_num==1 || tile_num==4 || tile_num==5 || tile_num==6){
		r2[curr_pos + 1] = 1;
	}
	*/
	switch(tile_num){
		case 0:{
			r1[curr_pos] = 1;
			break;
		}
		case 1:{
			r1[curr_pos] = 1;
			r1[curr_pos+1] = 1;
			break;
		}
		case 2:{
			r1[curr_pos] = 1;
			r2[curr_pos] = 1;
			r2[curr_pos+1] = 1;
			break;
		}
		case 3:{
			r1[curr_pos] = 1;
			r2[curr_pos] = 1;
			break;
		}
		case 4:{
			r1[curr_pos] = 1;
			r2[curr_pos] = 1;
			r1[curr_pos + 1] = 1;
			break;
		}
		case 5:{
			r1[curr_pos] = 1;
			r2[curr_pos] = 1;
			r1[curr_pos + 1] = 1;
			r2[curr_pos + 1] = 1;
			break;
		}
		case 6:{
			r1[curr_pos] = 1;
			r2[curr_pos] = 1;
			r1[curr_pos + 1] = 1;
			r2[curr_pos + 1] = 1;
			break;
		}
		case 7:{
			r2[curr_pos] = 1;
			break;
		}
		case 8:{
			r2[curr_pos] = 1;
			r2[curr_pos + 1] = 1;
			break;
		}
	}
}
//
void move_up(){
	if ((tile_num == 7) && (!r1[curr_pos])){
		lcd_cmd(128 + curr_pos); lcd_write_char('*');
		lcd_cmd(192 + curr_pos); lcd_write_char(' ');
		tile_num = 0;
	}else if (((tile_num == 8) && (!r1[curr_pos])) && (!r1[curr_pos + 1])){
		lcd_cmd(128 + curr_pos); lcd_write_char('*'); lcd_write_char('*');
		lcd_cmd(192 + curr_pos); lcd_write_char(' '); lcd_write_char(' ');
		tile_num = 1;
	}else{}
}
//
void move_down(){
	if ((tile_num == 0) && (!r2[curr_pos])){
		lcd_cmd(128 + curr_pos); lcd_write_char(' ');
		lcd_cmd(192 + curr_pos); lcd_write_char('*');
		tile_num = 7;
	}else if (((tile_num == 1) && (!r2[curr_pos])) && (!r2[curr_pos + 1])){
		lcd_cmd(128 + curr_pos); lcd_write_char(' '); lcd_write_char(' ');
		lcd_cmd(192 + curr_pos); lcd_write_char('*'); lcd_write_char('*');
		tile_num = 8;
	}else{}
}
//
void rotate(){
	switch (tile_num){
		case 0:{break;}
		case 1:{
			if (!r2[curr_pos + 1]){
				// can rotate
				lcd_cmd(128 + curr_pos); lcd_write_char(' ');
				lcd_cmd(192 + curr_pos + 1); lcd_write_char('*');
				tile_num = 3; curr_pos++;
			}
			break;
		}
		case 2:{
			lcd_cmd(128 + curr_pos + 1); lcd_write_char('*');
			lcd_cmd(192 + curr_pos + 1); lcd_write_char(' ');
			tile_num = 4;
			break;
		}
		case 3:{
			if (!r1[curr_pos-1]){
				lcd_cmd(128 + curr_pos - 1); lcd_write_char('*');
				lcd_cmd(192 + curr_pos); lcd_write_char(' ');
				tile_num = 1; curr_pos--;
			}
			break;
		}
		case 4:{
			lcd_cmd(192 + curr_pos); lcd_write_char(' '); lcd_write_char('*');
			tile_num = 5;
			break;
		}
		case 5:{
			if(!r2[curr_pos]){
				lcd_cmd(128 + curr_pos); lcd_write_char(' ');
				lcd_cmd(192 + curr_pos); lcd_write_char('*');
				tile_num = 6;
			}
			break;
		}
		case 6:{
			if(!r1[curr_pos]){
				lcd_cmd(128 + curr_pos); lcd_write_char('*'); lcd_write_char(' ');
				tile_num = 2;
			}
			break;
		}
		case 7:{break;}
		case 8:{
			if(!r1[curr_pos + 1]){
				lcd_cmd(128 + curr_pos + 1); lcd_write_char('*');
				lcd_cmd(192 + curr_pos); lcd_write_char(' ');
				tile_num = 3; curr_pos++;
			}
			break;
		}
	}
}
//	
void serial_ISR(void) interrupt 4
{ // can see unsigned char tile_num , r1, r2 , curr_pos
	unsigned char ch1 = 0;
	ch1 = SBUF;
	switch(ch1){
		case 'q': {move_up(); break;}
		case 'a': {move_down(); break;}
		case 'r': {rotate(); break;}
		default: {}
	}
	RI = 0;			//Clear RI flag
}
//
//Main function
void main(void)
{
	unsigned int high_score = 0;
	char score_str[5]={0,0,0,0,'\0'};
	unsigned int score = 0;
	char high_score_str[5]={0,0,0,0,'\0'};
	bit can_move = 1;
	int i; // iterator
	
	//Call initialization functions
	lcd_init();
	uart_init();
	
	while(1){ // play game indefinitely, to loop over games
		score = 0;
		for (i=0; i<16; i++){
			r1[i] = 0;
			r2[i] = 0;
		}
		while((!r1[15]) || (!r2[15])){ // while place to move in this game, to loop over tiles
			tile_num = get_new_tile();
			curr_pos = 16;

			if (tile_num==0 && r1[15]){
				break;
			}else if (tile_num==1 && (r1[15] || r1[14])){
				break;
			}else if (tile_num==2 && (r1[14] || r1[15] || r2[14] || r2[15])){
				break;
			}
			if(tile_num == 0){
				curr_pos--;
				lcd_cmd(0x8F); lcd_write_char('*');
			}else if (tile_num == 1){
				curr_pos -= 2;
				lcd_cmd(0x8E); lcd_write_char('*'); lcd_write_char('*');
			}else{
				curr_pos -= 2;
				lcd_cmd(0x8E); lcd_write_char('*');
				lcd_cmd(0xCE); lcd_write_char('*'); lcd_write_char('*');
			}
			score++; msdelay(1000);
			can_move = fun_can_move();
			while(can_move){
				// move_left(); curr_pos--; score++; one_sec_delay();
				EA  = 0;
				move_left();
				EA = 1;
				curr_pos--; score++;
				msdelay(1000); // 1 sec delay
				can_move = fun_can_move(); // update can_move
			}
			// tile has reached destination, update r1, r2
			update_r();
		} // game has ended
		if(score > high_score){
			high_score = score;
			int_to_string(high_score, high_score_str);
		}
		lcd_cmd(0x01);
		int_to_string(score, score_str);
		lcd_cmd(0x80);
		lcd_write_string("Score: ");
		lcd_write_string(score_str);
		lcd_cmd(0xC0);
		lcd_write_string("High Score: ");
		lcd_write_string(high_score_str);
		msdelay(3000);
		lcd_cmd(0x01);
	}
}
