/**
  ****************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ****************************************************************************
*/


#include "stm32f0xx.h"
#include "stm32f0_discovery.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
int save_pixel=0b00000000;
int clear=0;
int encs1=0,encs2=0;


void nano_wait(int t) {
    asm("       mov r0,%0\n"
        "repeat:\n"
        "       sub r0,#83\n"
        "       bgt repeat\n"
        : : "r"(t) : "r0", "cc");
}

void transferdata(void){
	nano_wait(8000);
	GPIOA->ODR |= (0x1 << 7);
	nano_wait(8000);
	GPIOA->ODR &= ~(0x1 << 7);
}

void turnon(void){

	//Turn on
	 GPIOA->ODR &= ~(0b11 << 11);
	 GPIOA->ODR |= (0b00 << 11);
	 GPIOC->ODR &= ~(0xff);
	 GPIOC->ODR |= (0b00111111);
	 transferdata();
}
void encs(int cs){
	 GPIOA->ODR &= ~(0b1 << (10-cs)); //Enable cs1
	 GPIOA->ODR |= 0b1 << (7+cs); //disable the other
	 transferdata();
	 if(cs==1) {
		 encs1=1;
		 encs2=0;
	 }
	 else {
		 encs2=1;
		 encs1=0;
	 }
	 //transferdata();

}

void LCD_init(void){
	
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
	GPIOA->MODER |= (0x555 << 14);
	GPIOA->ODR |= (0b001111 << 7);

	RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
	GPIOC->MODER |= 0x5555;
}

void coord1(int x,int y){
	startline();
	//y
	if(y/64==0 && encs1==0) encs(1);//select cs1 or cs2
	else if(y/64==1 && encs2==0) encs(2);
	Y(y%64);

	//x
	page(x/8);//selcet page
	x=x%8;
	switch(x){
		case 0:readdata(0b00000001);break;
		case 1:readdata(0b00000010);break;
		case 2:readdata(0b00000100);break;
		case 3:readdata(0b00001000);break;
		case 4:readdata(0b00010000);break;
		case 5:readdata(0b00100000);break;
		case 6:readdata(0b01000000);break;
		case 7:readdata(0b10000000);break;
		default:readdata(0b00000000);break;
	}
}

void readdata(int d){
	  //Read data cs
	 GPIOA->ODR &= ~(0b11 << 11);
	 GPIOA->ODR |= (0b10 << 11);
	 GPIOC->ODR &= ~(0xff);

	 //GPIOC->ODR |= (0b00000000 | d);//0: white 1: black
	 GPIOC->ODR |= (save_pixel | d);//0: white 1: black
	 transferdata();
}

void clear_cs(int cs){
	int i=0,j=0;

	encs(cs);

	startline();
	for(i=0;i<8;i++){
		page(i);
		for(j=0;j<64;j++){
			Y(j);
			 GPIOA->ODR &= ~(0b11 << 11);
			 GPIOA->ODR |= (0b10 << 11);
			 GPIOC->ODR &= ~(0xff);
			 GPIOC->ODR |= (0b00000000);//0: white 1: black
			 save_pixel=0b00000000;
			 transferdata();
		}
	}

	clear=1;
}


void startline(void){
	 //start line
	 GPIOA->ODR &= ~(0b11 << 11);
	 GPIOA->ODR |= (0b00 << 11);
	 GPIOC->ODR &= ~(0xff);
	 GPIOC->ODR |= (0b11000000);
	 transferdata();
}

void page(int p){
	 GPIOA->ODR &= ~(0b11 << 11);
	 GPIOA->ODR |= (0b00 << 11);
	 GPIOC->ODR &= ~(0xff);
	 GPIOC->ODR |= (0b10111000 + p);
	 transferdata();
}

void Y(int y){

	 //Y address
	 GPIOA->ODR &= ~(0b11 << 11);
	 GPIOA->ODR |= (0b00 << 11);
	 GPIOC->ODR &= ~(0xff);
	 GPIOC->ODR |= ((0b01000000) + y);
	 transferdata();
}

/*
void shoe(int x,int y){
	startline();
	//y
	encs(y/64+1);//select cs1 or cs2
	Y(y%64);

	//x
	page(x/8);//selcet page
	x=x%8;
	readdata(0b11111111);
}

void coord3_woen(int x,int y){
	startline();
	//y
	//encs(y/64+1);//select cs1 or cs2
	Y(y%64);

	//x
	page(x/8);//selcet page
	x=x%8;
	switch(x){
		case 0:readdata(0b00000111);break;
		case 1:readdata(0b00001110);break;
		case 2:readdata(0b00011100);break;
		case 3:readdata(0b00111000);break;
		case 4:readdata(0b01110000);break;
		case 5:readdata(0b11100000);break;
		case 6:readdata(0b11000000);break;
		case 7:readdata(0b10000000);break;
		default:readdata(0b00000000);break;
	}
}
void coord4(int x,int y){
	startline();
	//y
	if(y/64==0 && encs1==0) encs(1);//select cs1 or cs2
	else if(y/64==1 && encs2==0) encs(2);
	Y(y%64);

	//x
	page(x/8);//selcet page
	x=x%8;
	switch(x){
		case 0:readdata(0b00001111);break;
		case 1:readdata(0b00011110);break;
		case 2:readdata(0b00111100);break;
		case 3:readdata(0b01111000);break;
		case 4:readdata(0b11110000);break;
		case 5:readdata(0b11100000);break;
		case 6:readdata(0b11000000);break;
		case 7:readdata(0b10000000);break;
		default:readdata(0b00000000);break;
	}
}
void coord5(int x,int y){
	startline();
	//y
	if(y/64==0 && encs1==0) encs(1);//select cs1 or cs2
	else if(y/64==1 && encs2==0) encs(2);
	Y(y%64);

	//x
	page(x/8);//selcet page
	x=x%8;
	switch(x){
		case 0:readdata(0b00011111);break;
		case 1:readdata(0b00111110);break;
		case 2:readdata(0b01111100);break;
		case 3:readdata(0b11111000);break;
		case 4:readdata(0b11110000);break;
		case 5:readdata(0b11100000);break;
		case 6:readdata(0b11000000);break;
		case 7:readdata(0b10000000);break;
		default:readdata(0b00000000);break;
	}
}
void coord6(int x,int y){
	startline();
	//y
	if(y/64==0 && encs1==0) encs(1);//select cs1 or cs2
	else if(y/64==1 && encs2==0) encs(2);
	Y(y%64);

	//x
	page(x/8);//selcet page
	x=x%8;
	switch(x){
		case 0:readdata(0b00111111);break;
		case 1:readdata(0b01111110);break;
		case 2:readdata(0b11111100);break;
		case 3:readdata(0b11111000);break;
		case 4:readdata(0b11110000);break;
		case 5:readdata(0b11100000);break;
		case 6:readdata(0b11000000);break;
		case 7:readdata(0b10000000);break;
		default:readdata(0b00000000);break;
	}
}

void coord3(int x,int y){
	startline();
	//y
	if(y/64==0 && encs1==0) encs(1);//select cs1 or cs2
	else if(y/64==1 && encs2==0) encs(2);
	Y(y%64);

	//x
	page(x/8);//selcet page
	x=x%8;
	switch(x){
		case 0:readdata(0b00000111);break;
		case 1:readdata(0b00001110);break;
		case 2:readdata(0b00011100);break;
		case 3:readdata(0b00111000);break;
		case 4:readdata(0b01110000);break;
		case 5:readdata(0b11100000);break;
		case 6:readdata(0b11000000);break;
		case 7:readdata(0b10000000);break;
		default:readdata(0b00000000);break;
	}
}

void coord2(int x,int y){
	startline();
	//y
	if(y/64==0 && encs1==0) encs(1);//select cs1 or cs2
	else if(y/64==1 && encs2==0) encs(2);
	Y(y%64);

	//x
	page(x/8);//selcet page
	x=x%8;
	switch(x){
		case 0:readdata(0b00000011);break;
		case 1:readdata(0b00000110);break;
		case 2:readdata(0b00001100);break;
		case 3:readdata(0b00011000);break;
		case 4:readdata(0b00110000);break;
		case 5:readdata(0b01100000);break;
		case 6:readdata(0b11000000);break;
		case 7:readdata(0b10000000);break;
		default:readdata(0b00000000);break;
	}
}


void left_leg0(){
	//left leg
	int i;
	for(i=32;i<43;i++){
		coord3(i-2,i+84);
	}
	i--;

	//coord2(i-2,i+83);coord3(i-2-2,i+83);
	coord2(i-2,i+83);coord1(i-2-1,i+84-1);
	coord4(i-2-1,i+85-1);
	coord6(i-2,i+86-1);coord1(i-3,i+86-1);

}

void left_leg1(){
	int i=0;
	//left leg
	coord3(32-i,32+84);
	coord3(32-i,32+84+1);
	coord3(33-i,32+84+2);
	coord3(33-i,32+84+3);
	coord3(34-i,32+84+4);
	coord3(34-i,32+84+5);
	coord3(35-i,32+84+6);
	coord3(35-i,32+84+7);
	coord3(36-i,32+84+8);
	coord3(36-i,32+84+9);
	coord3(37-i,32+84+10);

	coord3(40-i,32+84+10);
	coord3(40-i,32+84+11);coord3(37-i,32+84+11);
	//coord3(42,32+84+10);
	//coord3(41,32+84+10);
}

void right_leg0(){
	//right leg
	int i;
	for(i=15;i<24;i++){
		coord3(24-i+21,103+i);
	}
	coord2(24,125);
	//coord3(23,126);
	//coord3(23,127);
	coord4(24,126);
	coord6(24,127);coord2(22,127);

}

void right_leg1(){
	//right leg
	coord3(30,118);
	coord3(30,119);
	coord3(29,120);
	coord3(29,121);
	coord3(28,122);
	coord3(28,123);
	coord3(27,124);
	coord3(27,125);

	coord5(27,126);
	coord5(27,127);
	//coord2(30,127);
	//coord3(24,125);
//	shoe(27,126);

}
void Short(int p, int y, int r){
 startline();
 page(p);//0~7
 Y(y);//0~63
 readdata(r);//0~63
}

void draw_head(){
 int i, j;

 //encs(2);

 Short(3, 14, (0b11100000));
 Short(3, 15, (0b11110000));
 Short(3, 16, (0b11110000));
 Short(3, 17, (0b11111000));
 Short(3, 18, (0b11111111));
 Short(3, 19, (0b11111111));

 for(i=20; i<26; i++){
  Short(3, i, (0b11111100));
 }

 Short(3, 26, (0b11111000));
 Short(3, 27, (0b11110000));
 Short(3, 28, (0b11100000));


 Short(4, 14, (0b00000111));
 Short(4, 15, (0b00001111));
 Short(4, 16, (0b00001111));
 Short(4, 17, (0b00011111));
 Short(4, 18, (0b11111111));

 Short(4, 19, (0b11111111));

 for(j=20; j<26; j++){
  Short(4, j, (0b00111111));
 }

 Short(4, 26, (0b00011111));
 Short(4, 27, (0b00001111));
 Short(4, 28, (0b00000111));

}

void draw_body1(){
 int i;
 //encs(2);
 for(i=29; i<33; i++){
  Short(3, i, (0b11000000));
 }

 for(i=33; i<54; i++){
  Short(3, i, (0b11000000));
 }

 for(i=29; i<33; i++){
  Short(4, i, (0b00000011));
 }

 for(i=33; i<54; i++){
  Short(4, i, (0b00000011));
 }
}

void draw_body2(){
 int i;
 //encs(2);
 for(i=29; i<33; i++){
  Short(3, i, (0b11000000));
 }

 for(i=33; i<54; i++){
  Short(3, i, (0b11100000));
 }

 for(i=29; i<33; i++){
  Short(4, i, (0b00000011));
 }

 for(i=33; i<54; i++){
  Short(4, i, (0b00000001));
 }
}

void move_body(){
	int i;
	for(i=0; i<100; i++){
  draw_body1();
  nano_wait(1000000000);
  draw_body2();
  nano_wait(1000000000);
 }

}

void merge_leg0(){
	int i,j;
	j=4;
	//upper
	for(i=0;i<j;i++) coord2(30,i+116);

	for(i=0;i<j-1;i++) coord1(32,i+116);
	coord2(32,i+116);
	//lower-left

	coord3(32,i+117);
	coord3(33,i+118);
	coord3(33,i+119);
	coord3(34,i+120);
	coord3(34,i+121);
	coord3(35,i+122);
	coord3(35,i+123);
	//coord3(36,i+124);
	coord4(36,i+124);
	coord2(40,i+124);
	//coord2(9,i+124);
	//lower-right
	coord3(30,i+117);
	coord3(29,i+118);
	coord3(29,i+119);
	coord3(28,i+120);
	coord3(27,i+121);
	coord3(27,i+122);
	coord3(26,i+123);
	coord6(26,i+124);

}

void merge_leg1(){
	int i,j;
	j=11;
	//upper
	for(i=0;i<j;i++) coord2(30,i+116);

	for(i=0;i<j-1;i++) coord1(32,i+116);
	coord6(32,i+116);
	coord6(32,i+116+1);
	coord2(30,i+117);
}

void stop_leg(){
	int i;
	//left leg
	for(i=32;i<43;i++){
		coord3(i-2,i+84);
	}
	i--;

	//coord2(i-2,i+83);coord3(i-2-2,i+83);
	coord2(i-2,i+83);coord1(i-2-1,i+84-1);
	coord4(i-2-1,i+85-1);
	coord6(i-2,i+86-1);coord1(i-3,i+86-1);
	//right leg
	for(i=15;i<24;i++){
		coord3(24-i+21,103+i);
	}

	coord2(24,i+101);
	coord2(24,i+102);
	coord2(24,i+103);coord4(18+2,i+103);


}
void draw_shoulder1(){
 Short(3, 31, (0b11000000));
 Short(3, 32, (0b11000000));
 Short(3, 33, (0b11000000));
 Short(3, 34, (0b11000000));
 Short(3, 35, (0b11000000));
 Short(3, 36, (0b11000000));
 Short(3, 37, (0b11000000));
 Short(3, 38, (0b11000000));
 Short(3, 39, (0b11000000));
 Short(2, 39, (0b11000000));
 Short(3, 40, (0b11000000));
 Short(3, 41, (0b11000000));
 Short(3, 42, (0b11000000));
 Short(3, 43, (0b11000000));

 Short(3, 31, (0b11000000));
 Short(3, 32, (0b11000000));
 Short(3, 33, (0b11100000));
 Short(3, 34, (0b11110000));
 Short(3, 35, (0b11111000));
 Short(3, 36, (0b11011100));
 Short(3, 37, (0b11001110));
 Short(3, 38, (0b11000111));
 Short(3, 39, (0b11000011));
 Short(2, 39, (0b10000000));
 Short(3, 40, (0b11000001));
 Short(2, 40, (0b11000000));
 Short(2, 41, (0b11100000));
 Short(2, 42, (0b01110000));
 Short(2, 43, (0b00111000));
 Short(2, 44, (0b00011100));
 Short(2, 45, (0b00001000));

 Short(4, 31, (0b00000011));
 Short(4, 32, (0b00000011));
 Short(4, 33, (0b00000011));
 Short(4, 34, (0b00000011));
 Short(4, 35, (0b00000011));
 Short(4, 36, (0b00000011));
 Short(4, 37, (0b00000011));
 Short(4, 38, (0b00000011));
 Short(4, 39, (0b00000011));
 Short(4, 40, (0b00000011));
 Short(4, 41, (0b00000011));
 Short(4, 42, (0b00000011));
 Short(4, 43, (0b00000011));

 Short(4, 31, (0b00000011));
 Short(4, 32, (0b00000011));
 Short(4, 33, (0b00000111));
 Short(4, 34, (0b00001111));
 Short(4, 35, (0b00011111));
 Short(4, 36, (0b00111011));
 Short(4, 37, (0b01110011));
 Short(4, 38, (0b11100011));
 Short(4, 39, (0b11000011));
 Short(5, 39, (0b00000001));
 Short(4, 40, (0b10000011));
 Short(5, 40, (0b00000011));
 Short(5, 41, (0b00000111));
 Short(5, 42, (0b00001110));
 Short(5, 43, (0b00011100));
 Short(5, 44, (0b00111000));
 Short(5, 45, (0b00010000));
}

void draw_shoulder2(){
 Short(3, 31, (0b11000000));
 Short(3, 32, (0b11000000));
 Short(3, 33, (0b11000000));
 Short(3, 34, (0b11000000));
 Short(3, 35, (0b11000000));
 Short(3, 36, (0b11000000));
 Short(3, 37, (0b11000000));
 Short(3, 38, (0b11000000));
 Short(3, 39, (0b11000000));
 Short(3, 40, (0b11000000));
 Short(2, 39, (0b00000000));
 Short(2, 40, (0b00000000));
 Short(2, 41, (0b00000000));
 Short(2, 42, (0b00000000));
 Short(2, 43, (0b00000000));
 Short(2, 44, (0b00000000));
 Short(2, 45, (0b00000000));

 Short(3, 31, (0b11000000));
 Short(3, 32, (0b11000000));
 Short(3, 33, (0b11100000));
 Short(3, 34, (0b11100000));
 Short(3, 35, (0b11110000));
 Short(3, 36, (0b11010000));
 Short(3, 37, (0b11011000));
 Short(3, 38, (0b11001100));
 Short(3, 39, (0b11001100));
 Short(3, 40, (0b11000110));
 Short(3, 41, (0b11000111));
 Short(3, 42, (0b11000011));
 Short(3, 43, (0b11000001));

 Short(4, 31, (0b00000011));
 Short(4, 32, (0b00000011));
 Short(4, 33, (0b00000011));
 Short(4, 34, (0b00000011));
 Short(4, 35, (0b00000011));
 Short(4, 36, (0b00000011));
 Short(4, 37, (0b00000011));
 Short(4, 38, (0b00000011));
 Short(4, 39, (0b00000011));
 Short(4, 40, (0b00000011));
 Short(5, 39, (0b00000000));
 Short(5, 40, (0b00000000));
 Short(5, 41, (0b00000000));
 Short(5, 42, (0b00000000));
 Short(5, 43, (0b00000000));
 Short(5, 44, (0b00000000));
 Short(5, 45, (0b00000000));

 Short(4, 31, (0b00000011));
 Short(4, 32, (0b00000011));
 Short(4, 33, (0b00000111));
 Short(4, 34, (0b00000111));
 Short(4, 35, (0b00001111));
 Short(4, 36, (0b00001011));
 Short(4, 37, (0b00011011));
 Short(4, 38, (0b00110011));
 Short(4, 39, (0b00110011));
 Short(4, 40, (0b01100011));
 Short(4, 41, (0b11100011));
 Short(4, 42, (0b11000011));
 Short(4, 43, (0b10000011));
}

void draw_shoulder3(){
 Short(3, 31, (0b11000000));
 Short(3, 32, (0b11000000));
 Short(3, 33, (0b11000000));
 Short(3, 34, (0b11000000));
 Short(3, 35, (0b11000000));
 Short(3, 36, (0b11000000));
 Short(3, 37, (0b11000000));
 Short(3, 38, (0b11000000));
 Short(3, 39, (0b11000000));
 Short(2, 39, (0b11000000));
 Short(3, 40, (0b11000000));
 Short(3, 41, (0b11000000));
 Short(3, 42, (0b11000000));
 Short(3, 43, (0b11000000));

 Short(4, 31, (0b00000011));
 Short(4, 32, (0b00000011));
 Short(4, 33, (0b00000011));
 Short(4, 34, (0b00000011));
 Short(4, 35, (0b00000011));
 Short(4, 36, (0b00000011));
 Short(4, 37, (0b00000011));
 Short(4, 38, (0b00000011));
 Short(4, 39, (0b00000011));
 Short(4, 40, (0b00000011));
 Short(4, 41, (0b00000011));
 Short(4, 42, (0b00000011));
 Short(4, 43, (0b00000011));

 Short(3, 31, (0b11000000));
 Short(3, 32, (0b11000000));
 Short(3, 33, (0b11000000));
 Short(3, 34, (0b11000000));
 Short(3, 35, (0b11000000));
 Short(3, 36, (0b11000000));
 Short(3, 37, (0b11000000));
 Short(3, 38, (0b11000000));
 Short(3, 39, (0b11000000));
 Short(3, 40, (0b11000000));
 Short(2, 39, (0b00000000));
 Short(2, 40, (0b00000000));
 Short(2, 41, (0b00000000));
 Short(2, 42, (0b00000000));
 Short(2, 43, (0b00000000));
 Short(2, 44, (0b00000000));
 Short(2, 45, (0b00000000));

 Short(4, 31, (0b00000011));
 Short(4, 32, (0b00000011));
 Short(4, 33, (0b00000011));
 Short(4, 34, (0b00000011));
 Short(4, 35, (0b00000011));
 Short(4, 36, (0b00000011));
 Short(4, 37, (0b00000011));
 Short(4, 38, (0b00000011));
 Short(4, 39, (0b00000011));
 Short(4, 40, (0b00000011));
 Short(5, 39, (0b00000000));
 Short(5, 40, (0b00000000));
 Short(5, 41, (0b00000000));
 Short(5, 42, (0b00000000));
 Short(5, 43, (0b00000000));
 Short(5, 44, (0b00000000));
 Short(5, 45, (0b00000000));
}

void draw_stop_sign(){
	int i;
	for(i=0;i<63;i++){
		coord6(i,i);
	}
	for(i=0;i<63;i++){
		coord6(63-i,i);
	}

}
void draw9(){
    int i;
    for(i=16; i<20; i++) Short(1, i, (0b11111111));
    for(i=20; i<32; i++) Short(1, i, (0b00001111));
    for(i=32; i<36; i++) Short(1, i, (0b11111111));
    for(i=36; i<49; i++) Short(1, i, (0b00001111));
    for(i=49; i<52; i++) Short(1, i, (0b11111111));

    for(i=16; i<20; i++) Short(2, i, (0b11111111));
    for(i=20; i<32; i++) Short(2, i, (0b11110000));
    for(i=32; i<36; i++) Short(2, i, (0b11111111));
    for(i=36; i<49; i++) Short(2, i, (0b00000000));
    for(i=49; i<52; i++) Short(2, i, (0b11111111));
}

void draw8(){
 int i;
 for(i=16; i<20; i++) Short(1, i, (0b11111111));
 for(i=20; i<32; i++) Short(1, i, (0b00001111));
 for(i=32; i<36; i++) Short(1, i, (0b11111111));
 for(i=36; i<48; i++) Short(1, i, (0b00001111));
 for(i=48; i<52; i++) Short(1, i, (0b11111111));

 for(i=16; i<20; i++) Short(2, i, (0b11111111));
 for(i=20; i<32; i++) Short(2, i, (0b11110000));
 for(i=32; i<36; i++) Short(2, i, (0b11111111));
 for(i=36; i<48; i++) Short(2, i, (0b11110000));
 for(i=48; i<52; i++) Short(2, i, (0b11111111));
}

void draw7(){
	int i;
	 for(i=16; i<20; i++) Short(1, i, (0b11111111));
	 for(i=20; i<32; i++) Short(1, i, (0b00001111));
	 for(i=32; i<36; i++) Short(1, i, (0b00001111));
	 for(i=36; i<48; i++) Short(1, i, (0b00001111));
	 for(i=48; i<52; i++) Short(1, i, (0b00001111));

	 for(i=16; i<20; i++) Short(2, i, (0b11111111));
	 for(i=20; i<32; i++) Short(2, i, (0b00000000));
	 for(i=32; i<36; i++) Short(2, i, (0b00000000));
	 for(i=36; i<48; i++) Short(2, i, (0b00000000));
	 for(i=48; i<52; i++) Short(2, i, (0b00000000));
}

void draw6(){
 int i;
 for(i=16; i<20; i++) Short(1, i, (0b11111111));
 for(i=20; i<32; i++) Short(1, i, (0b00000000));
 for(i=32; i<36; i++) Short(1, i, (0b11111111));
 for(i=36; i<48; i++) Short(1, i, (0b00001111));
 for(i=48; i<52; i++) Short(1, i, (0b11111111));

 for(i=16; i<20; i++) Short(2, i, (0b11111111));
 for(i=20; i<32; i++) Short(2, i, (0b11110000));
 for(i=32; i<36; i++) Short(2, i, (0b11111111));
 for(i=36; i<48; i++) Short(2, i, (0b11110000));
 for(i=48; i<52; i++) Short(2, i, (0b11111111));
}

void draw5(){
 int i;
 for(i=16; i<20; i++) Short(1, i, (0b11111111));
 for(i=20; i<32; i++) Short(1, i, (0b00000000));
 for(i=32; i<36; i++) Short(1, i, (0b11111111));
 for(i=36; i<48; i++) Short(1, i, (0b00001111));
 for(i=48; i<52; i++) Short(1, i, (0b11111111));

 for(i=16; i<20; i++) Short(2, i, (0b11111111));
 for(i=20; i<32; i++) Short(2, i, (0b11110000));
 for(i=32; i<36; i++) Short(2, i, (0b11111111));
 for(i=36; i<48; i++) Short(2, i, (0b00000000));
 for(i=48; i<52; i++) Short(2, i, (0b11111111));
}

void draw4(){
	int i;
	 for(i=16; i<20; i++) Short(1, i, (0b00001111));
	 for(i=20; i<32; i++) Short(1, i, (0b00001111));
	 for(i=32; i<36; i++) Short(1, i, (0b11111111));
	 for(i=36; i<48; i++) Short(1, i, (0b00001111));
	 for(i=48; i<52; i++) Short(1, i, (0b00001111));

	 for(i=16; i<20; i++) Short(2, i, (0b11110000));
	 for(i=20; i<32; i++) Short(2, i, (0b11110000));
	 for(i=32; i<36; i++) Short(2, i, (0b11111111));
	 for(i=36; i<48; i++) Short(2, i, (0b00000000));
	 for(i=48; i<52; i++) Short(2, i, (0b00000000));
}

void draw3(){
 int i;
 for(i=16; i<20; i++) Short(1, i, (0b11111111));
 for(i=20; i<32; i++) Short(1, i, (0b00001111));
 for(i=32; i<36; i++) Short(1, i, (0b11111111));
 for(i=36; i<48; i++) Short(1, i, (0b00001111));
 for(i=48; i<52; i++) Short(1, i, (0b11111111));

 for(i=16; i<20; i++) Short(2, i, (0b11111111));
 for(i=20; i<32; i++) Short(2, i, (0b00000000));
 for(i=32; i<36; i++) Short(2, i, (0b11111111));
 for(i=36; i<48; i++) Short(2, i, (0b00000000));
 for(i=48; i<52; i++) Short(2, i, (0b11111111));
}

void draw2(){
 int i;
 for(i=16; i<20; i++) Short(1, i, (0b11111111));
 for(i=20; i<32; i++) Short(1, i, (0b00001111));
 for(i=32; i<36; i++) Short(1, i, (0b11111111));
 for(i=36; i<48; i++) Short(1, i, (0b00000000));
 for(i=48; i<52; i++) Short(1, i, (0b11111111));

 for(i=16; i<20; i++) Short(2, i, (0b11111111));
 for(i=20; i<32; i++) Short(2, i, (0b00000000));
 for(i=32; i<36; i++) Short(2, i, (0b11111111));
 for(i=36; i<48; i++) Short(2, i, (0b11110000));
 for(i=48; i<52; i++) Short(2, i, (0b11111111));
}

void draw1(){
 int i;
 for(i=16; i<20; i++) Short(1, i, (0b00001111));
 for(i=20; i<32; i++) Short(1, i, (0b00001111));
 for(i=32; i<36; i++) Short(1, i, (0b00001111));
 for(i=36; i<48; i++) Short(1, i, (0b00001111));
 for(i=48; i<52; i++) Short(1, i, (0b00001111));

 for(i=16; i<20; i++) Short(2, i, (0b00000000));
 for(i=20; i<32; i++) Short(2, i, (0b00000000));
 for(i=32; i<36; i++) Short(2, i, (0b00000000));
 for(i=36; i<48; i++) Short(2, i, (0b00000000));
 for(i=48; i<52; i++) Short(2, i, (0b00000000));
}

void draw0(){
 int i;
 for(i=16; i<20; i++) Short(1, i, (0b11111111));
 for(i=20; i<32; i++) Short(1, i, (0b00001111));
 for(i=32; i<36; i++) Short(1, i, (0b00001111));
 for(i=36; i<48; i++) Short(1, i, (0b00001111));
 for(i=48; i<52; i++) Short(1, i, (0b11111111));

 for(i=16; i<20; i++) Short(2, i, (0b11111111));
 for(i=20; i<32; i++) Short(2, i, (0b11110000));
 for(i=32; i<36; i++) Short(2, i, (0b11110000));
 for(i=36; i<48; i++) Short(2, i, (0b11110000));
 for(i=48; i<52; i++) Short(2, i, (0b11111111));
}

void draw1x(){
 int i;
 for(i=16; i<20; i++) Short(5, i, (0b00001111));
 for(i=20; i<32; i++) Short(5, i, (0b00001111));
 for(i=32; i<36; i++) Short(5, i, (0b00001111));
 for(i=36; i<48; i++) Short(5, i, (0b00001111));
 for(i=48; i<52; i++) Short(5, i, (0b00001111));

 for(i=16; i<20; i++) Short(6, i, (0b00000000));
 for(i=20; i<32; i++) Short(6, i, (0b00000000));
 for(i=32; i<36; i++) Short(6, i, (0b00000000));
 for(i=36; i<48; i++) Short(6, i, (0b00000000));
 for(i=48; i<52; i++) Short(6, i, (0b00000000));
}

void draw9X(){
    int i;
    for(i=16; i<20; i++) Short(5, i, (0b11111111));
    for(i=20; i<32; i++) Short(5, i, (0b00001111));
    for(i=32; i<36; i++) Short(5, i, (0b11111111));
    for(i=36; i<49; i++) Short(5, i, (0b00001111));
    for(i=49; i<52; i++) Short(5, i, (0b11111111));

    for(i=16; i<20; i++) Short(6, i, (0b11111111));
    for(i=20; i<32; i++) Short(6, i, (0b11110000));
    for(i=32; i<36; i++) Short(6, i, (0b11111111));
    for(i=36; i<49; i++) Short(6, i, (0b00000000));
    for(i=49; i<52; i++) Short(6, i, (0b11111111));
}

void draw8X(){
    int i;
    for(i=16; i<20; i++) Short(5, i, (0b11111111));
    for(i=20; i<32; i++) Short(5, i, (0b00001111));
    for(i=32; i<36; i++) Short(5, i, (0b11111111));
    for(i=36; i<48; i++) Short(5, i, (0b00001111));
    for(i=48; i<52; i++) Short(5, i, (0b11111111));

    for(i=16; i<20; i++) Short(6, i, (0b11111111));
    for(i=20; i<32; i++) Short(6, i, (0b11110000));
    for(i=32; i<36; i++) Short(6, i, (0b11111111));
    for(i=36; i<48; i++) Short(6, i, (0b11110000));
    for(i=48; i<52; i++) Short(6, i, (0b11111111));
}

void draw7X(){
    int i;
    for(i=16; i<20; i++) Short(5, i, (0b11111111));
    for(i=20; i<32; i++) Short(5, i, (0b00001111));
    for(i=32; i<36; i++) Short(5, i, (0b00001111));
    for(i=36; i<48; i++) Short(5, i, (0b00001111));
    for(i=48; i<52; i++) Short(5, i, (0b00001111));

    for(i=16; i<20; i++) Short(6, i, (0b11111111));
    for(i=20; i<32; i++) Short(6, i, (0b00000000));
    for(i=32; i<36; i++) Short(6, i, (0b00000000));
    for(i=36; i<48; i++) Short(6, i, (0b00000000));
    for(i=48; i<52; i++) Short(6, i, (0b00000000));
}

void draw6X(){
    int i;
    for(i=16; i<20; i++) Short(5, i, (0b11111111));
    for(i=20; i<32; i++) Short(5, i, (0b00000000));
    for(i=32; i<36; i++) Short(5, i, (0b11111111));
    for(i=36; i<48; i++) Short(5, i, (0b00001111));
    for(i=48; i<52; i++) Short(5, i, (0b11111111));

    for(i=16; i<20; i++) Short(6, i, (0b11111111));
    for(i=20; i<32; i++) Short(6, i, (0b11110000));
    for(i=32; i<36; i++) Short(6, i, (0b11111111));
    for(i=36; i<48; i++) Short(6, i, (0b11110000));
    for(i=48; i<52; i++) Short(6, i, (0b11111111));
}

void draw5X(){
    int i;
    for(i=16; i<20; i++) Short(5, i, (0b11111111));
    for(i=20; i<32; i++) Short(5, i, (0b00000000));
    for(i=32; i<36; i++) Short(5, i, (0b11111111));
    for(i=36; i<48; i++) Short(5, i, (0b00001111));
    for(i=48; i<52; i++) Short(5, i, (0b11111111));

    for(i=16; i<20; i++) Short(6, i, (0b11111111));
    for(i=20; i<32; i++) Short(6, i, (0b11110000));
    for(i=32; i<36; i++) Short(6, i, (0b11111111));
    for(i=36; i<48; i++) Short(6, i, (0b00000000));
    for(i=48; i<52; i++) Short(6, i, (0b11111111));
}

void draw4X(){
    int i;
    for(i=16; i<20; i++) Short(5, i, (0b00001111));
    for(i=20; i<32; i++) Short(5, i, (0b00001111));
    for(i=32; i<36; i++) Short(5, i, (0b11111111));
    for(i=36; i<48; i++) Short(5, i, (0b00001111));
    for(i=48; i<52; i++) Short(5, i, (0b00001111));

    for(i=16; i<20; i++) Short(6, i, (0b11110000));
    for(i=20; i<32; i++) Short(6, i, (0b11110000));
    for(i=32; i<36; i++) Short(6, i, (0b11111111));
    for(i=36; i<48; i++) Short(6, i, (0b00000000));
    for(i=48; i<52; i++) Short(6, i, (0b00000000));
}

void draw3X(){
    int i;
    for(i=16; i<20; i++) Short(5, i, (0b11111111));
    for(i=20; i<32; i++) Short(5, i, (0b00001111));
    for(i=32; i<36; i++) Short(5, i, (0b11111111));
    for(i=36; i<48; i++) Short(5, i, (0b00001111));
    for(i=48; i<52; i++) Short(5, i, (0b11111111));

    for(i=16; i<20; i++) Short(6, i, (0b11111111));
    for(i=20; i<32; i++) Short(6, i, (0b00000000));
    for(i=32; i<36; i++) Short(6, i, (0b11111111));
    for(i=36; i<48; i++) Short(6, i, (0b00000000));
    for(i=48; i<52; i++) Short(6, i, (0b11111111));
}

void draw2X(){
    int i;
    for(i=16; i<20; i++) Short(5, i, (0b11111111));
    for(i=20; i<32; i++) Short(5, i, (0b00001111));
    for(i=32; i<36; i++) Short(5, i, (0b11111111));
    for(i=36; i<48; i++) Short(5, i, (0b00000000));
    for(i=48; i<52; i++) Short(5, i, (0b11111111));

    for(i=16; i<20; i++) Short(6, i, (0b11111111));
    for(i=20; i<32; i++) Short(6, i, (0b00000000));
    for(i=32; i<36; i++) Short(6, i, (0b11111111));
    for(i=36; i<48; i++) Short(6, i, (0b11110000));
    for(i=48; i<52; i++) Short(6, i, (0b11111111));
}

void draw1X(){
    int i;
    for(i=16; i<20; i++) Short(5, i, (0b00001111));
    for(i=20; i<32; i++) Short(5, i, (0b00001111));
    for(i=32; i<36; i++) Short(5, i, (0b00001111));
    for(i=36; i<48; i++) Short(5, i, (0b00001111));
    for(i=48; i<52; i++) Short(5, i, (0b00001111));

    for(i=16; i<20; i++) Short(6, i, (0b00000000));
    for(i=20; i<32; i++) Short(6, i, (0b00000000));
    for(i=32; i<36; i++) Short(6, i, (0b00000000));
    for(i=36; i<48; i++) Short(6, i, (0b00000000));
    for(i=48; i<52; i++) Short(6, i, (0b00000000));
}

void draw0X(){
    int i;
    for(i=16; i<20; i++) Short(5, i, (0b11111111));
    for(i=20; i<32; i++) Short(5, i, (0b00001111));
    for(i=32; i<36; i++) Short(5, i, (0b00001111));
    for(i=36; i<48; i++) Short(5, i, (0b00001111));
    for(i=48; i<52; i++) Short(5, i, (0b11111111));

    for(i=16; i<20; i++) Short(6, i, (0b11111111));
    for(i=20; i<32; i++) Short(6, i, (0b11110000));
    for(i=32; i<36; i++) Short(6, i, (0b11110000));
    for(i=36; i<48; i++) Short(6, i, (0b11110000));
    for(i=48; i<52; i++) Short(6, i, (0b11111111));
}

void digit(int x){
	//ones
	switch(x%10){
		case 0:draw0();break;
		case 1:draw1();break;
		case 2:draw2();break;
		case 3:draw3();break;
		case 4:draw4();break;
		case 5:draw5();break;
		case 6:draw6();break;
		case 7:draw7();break;
		case 8:draw8();break;
		case 9:draw9();break;
	}

	//tens
	switch(x/10){
		//case 0:draw0X();break;
		case 1:draw1X();break;
		case 2:draw2X();break;
		case 3:draw3X();break;
		case 4:draw4X();break;
		case 5:draw5X();break;
		case 6:draw6X();break;
		case 7:draw7X();break;
		case 8:draw8X();break;
		case 9:draw9X();break;
	}
}


void step(int s,int k){


	int i;
	int time=0*1000000/2;
	int time_red_light=0;
	//time=10*time/12;
	//while(1){
	//for(j=0,k=t_time;j<((t_time+1)/4);j++){

	switch(s){
		case 1:{
			clear_cs(1);digit(k);clear_cs(2);
				left_leg0();//pose 0
				right_leg0();//pose 1
				draw_head();
				draw_body1();
				draw_shoulder1();
				for(i=0;i<1000;i++) nano_wait(time);
		} break;
		case 2:{
			clear_cs(1);digit(k);clear_cs(2);
			right_leg1();//pose 1
			left_leg1();
			draw_head();
			draw_body1();
			draw_shoulder2();
			for(i=0;i<1000;i++) nano_wait(time);
		} break;
		case 3:{
			clear_cs(1);digit(k);clear_cs(2);
			merge_leg1();
			draw_head();

			draw_body1();
			draw_shoulder3();
			for(i=0;i<1000;i++) nano_wait(time);
		}break;
		case 4:{
			clear_cs(1);digit(k);clear_cs(2);
			merge_leg0();
			draw_head();
			draw_body1();
			draw_shoulder2();
			for(i=0;i<1000;i++) nano_wait(time);
		}break;
		case 5:{
			clear_cs(2);
			stop_leg();
			draw_head();
			draw_body1();
			draw_shoulder2();
			clear_cs(1);
			draw_stop_sign();
			for(i=0;i<1000;i++) nano_wait(time_red_light);
		}break;
	}


}
*/
