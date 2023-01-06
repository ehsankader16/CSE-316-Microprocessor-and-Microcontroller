/*
 * CSE316_Project.c
 *
 * Created: 8/18/2022 12:24:27 AM
 * Author : ASUS
 */ 

#include <avr/io.h>
#define F_CPU 1000000
#include <util/delay.h>
#include <avr/interrupt.h>


//char images[48] = {0x18,0x18,0x3c,0x3c,0x7e,0xff,0x18,0x18,0x00,0x66,0x99,0x81,0x81,0x42,0x24,0x18,0x3c,0x42,0xa5,0x81,0xa5,0x99,0x42,0x3c,0x00,0xff,0xff,0xbd,0x99,0x81,0xff,0x00,0x3c,0x42,0xa1,0x87,0x8f,0xfb,0x7e,0x3c,0x18,0x2c,0x42,0x81,0x99,0x76,0x00,0x5a};

long binImage[] = {
	0b000000000000000000000000,
	0b000000000000000000000000,
	0b000000000000000000000000,
	0b000000000000000000000000,
	0b000000000000000000000000,
	0b000000000000000000000000,
	0b000000000000000000000000,
	0b000000000000000000000000,
	0b000000000000000000000000,
	0b000000000000000000000000,
	0b000000000000000000000000,
	0b000000000000000000000000,
	0b000000000000000000000000,
	0b000000000000000000000000,
	0b000000000000000000000000,
	0b000000000000000000000000
};

int playerPos = 1; // can be between 1 and 14
char bulletPos = 0;
int playerAlive = 1;
int bulletPosCount = 0;


int enemyPos = 5;
int enemyUpFlag = 1;
int waitForReload = 0;
int shiftCou = 0;

int health = 100;
int damage = 10;
struct EnemyBullet{
	int isActive;
	int hasHit;
	int posX;
	int posY;
	int timeout;
};

struct EnemyBullet enemyBullet[10];
void initEnemyBullets() {
	for (int i = 0; i< 10; i++)
	{
		enemyBullet[i].isActive = 0;
		enemyBullet[i].hasHit = 0;
		enemyBullet[i].timeout = 0;
	}
}
char images[6][8] = {
	0
};

void binToImage()
{
	long lMask = 0b111111110000000000000000;
	long cMask = 0b000000001111111100000000;
	long rMask = 0b000000000000000011111111;
	
	for (int i = 0 ; i  < 8; i++)
	{
		char lVal = (char) ((binImage[i] & lMask) >> 16);
		char cVal = (char) ((binImage[i] & cMask) >> 8);
		char rVal = (char) (binImage[i] & rMask);
		images[0][i] = lVal;
		images[1][i] = cVal;
		images[2][i] = rVal;
	}
	
	for (int i = 0 ; i  < 8; i++)
	{
		char lVal = (char) ((binImage[i+8] & lMask) >> 16);
		char cVal = (char) ((binImage[i+8] & cMask) >> 8);
		char rVal = (char) (binImage[i+8] & rMask);
		images[3][i] = lVal;
		images[4][i] = cVal;
		images[5][i] = rVal;
	}
}

void erasePlayer()
{
	binImage[playerPos-1] &= 0b011111111111111111111111;
	binImage[playerPos] &= 0b001111111111111111111111;
	binImage[playerPos+1] &= 0b011111111111111111111111;
}

void drawPlayer()
{
	binImage[playerPos-1]	|=	0b100000000000000000000000;
	binImage[playerPos]		|=	0b110000000000000000000000;
	binImage[playerPos+1]	|=	0b100000000000000000000000;
}

void eraseEnemy()
{
	binImage[enemyPos-1] &= 0b111111111111111111111100;
	binImage[enemyPos] &= 0b111111111111111111111110;
	binImage[enemyPos+1] &= 0b111111111111111111111100;
}

void drawEnemy()
{
	binImage[enemyPos-1] |=	0b000000000000000000000011;
	binImage[enemyPos] |=		0b000000000000000000000001;
	binImage[enemyPos+1] |=	0b000000000000000000000011;
}

void handleEnemyMovement(){
	eraseEnemy();
	if(enemyPos == 1) {
		enemyUpFlag = 0;
		} else if(enemyPos == 12) {
		enemyUpFlag = 1;
	}
	if(enemyUpFlag) {
		enemyPos--;
		} else {
		enemyPos++;
	}
	drawEnemy();
}

void handleEnemyBullet()
{
	
	for(int i = 0; i < 5; i++) {
		if(enemyBullet[i].isActive) {
			enemyBullet[i].posX--;
			if((enemyBullet[i].posX == 1 && enemyBullet[i].posY == playerPos) || (enemyBullet[i].posX == 0 && (enemyBullet[i].posY == playerPos-1 ||   enemyBullet[i].posY == playerPos+1)))
			{
				enemyBullet[i].hasHit = 1;
				health -= damage;
				if(health == 0)	{
					playerAlive = 0;
				}													
			}
			if(enemyBullet[i].posX < 0 || enemyBullet[i].hasHit) {
				enemyBullet[i].isActive = 0;
				enemyBullet[i].hasHit = 0;
				enemyBullet[i].timeout = 5;
				long eraseBulletMask = ~(0b110000000000000000000000 >> (enemyBullet[i].posX+1));
				binImage[enemyBullet[i].posY] &= eraseBulletMask;
			} else {
				long eraseBulletMask = ~(0b110000000000000000000000 >> (enemyBullet[i].posX+1));
				binImage[enemyBullet[i].posY] &= eraseBulletMask;
				binImage[enemyBullet[i].posY] |= (0b110000000000000000000000 >> (enemyBullet[i].posX));
			}
		} else {
			if(enemyBullet[i].timeout == 0) {
				if(waitForReload) {
					waitForReload--;
				} else {
					enemyBullet[i].posX = 21;
					enemyBullet[i].posY = enemyPos;
					enemyBullet[i].isActive = 1;
					enemyBullet[i].timeout = 0;
					binImage[enemyBullet[i].posY] |= (0b000000000000000000000110);
					waitForReload = 3;
				}
			} else {
				enemyBullet[i].timeout--; 
			}
		}
	}
	
}



void updateBullet()
{
	if (bulletPos)
	{
		bulletPosCount++;
		if (bulletPosCount)
		{
			long eraseBulletMask = ~(0b110000000000000000000000 >> (bulletPos-1));
			binImage[playerPos] &= eraseBulletMask;
			bulletPos++;
			binImage[playerPos] |= (0b110000000000000000000000 >> (bulletPos-1));
		}
	}
}
void drawBullet()
{
	if (!bulletPos)
	{
		binImage[playerPos] |= 0b000110000000000000000000;
		bulletPos = 4;
	}
}



void updatePlayer(int direction){
	
	
	erasePlayer();
	
	if (direction == 1){
		// debug
		PORTA = PORTA | 0b00010000;
		// go up
		playerPos --;
		if (playerPos <= 1){
			playerPos = 1;
		}
	}
	
	if (direction == 0){
		// debug
		PORTA = PORTA | 0b00001000;
		// go down
		playerPos ++;
		if (playerPos >= 12){
			playerPos = 12;
		}
	}
	
	
}

void drawEnvironment(){
	if (shiftCou == 6) {
		binImage[15]=0b001000111000011000001001101;
		
	}
	if (shiftCou == 12) {
		binImage[15]=0b001000111000011001101001101;
		
	}
	if (shiftCou == 18) {
		binImage[15]=0b001000111000011011001001101;
		
	}
	if (shiftCou == 24) {
		binImage[15]=0b001000111000011010101001101;
		shiftCou = 0;
	}
	binImage[15]	=	binImage[15] << 1;
	shiftCou++;
	
	
}


ISR(INT0_vect)
{
	drawBullet();
	while (bulletPos < 25)
	{
		updateBullet();
		binToImage();
		for (int i = 0 ; i < 6; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				if (i == 0 && j == 0)
				PORTA = PORTA & 0b11111110;
				else
				PORTA = PORTA | 0b00000001;
				PORTA = PORTA | 0b00000010;
				PORTA = PORTA & 0b11111101;  // shift done
				PORTA = PORTA | 0b00000100;
				PORTA = PORTA & 0b11111011;  // store done
				PORTB = images[i][j];
				_delay_us(1000);
			}
		}
	}
	bulletPos = 0;
}

int main(void)
{
	DDRC = 0xFF;
	DDRA = 0xFF;
	DDRB = 0xFF;
	DDRD = 0x0;
	PORTA = 0x1;
	
	binImage[15]=0b00000000001110001010101;
	
	
	// set up an interrupt to catch rising edge
	GICR = (1 << INT0);
	MCUCR = MCUCR | (1 << ISC01);
	MCUCR = MCUCR & (~(1 << ISC00));
	sei();
	
	for (int i = 0; i < 48; i++)
	{
		PORTA = PORTA | 0b00000010;
		PORTA = PORTA & 0b11111101;  // shift done
		PORTA = PORTA | 0b00000100;
		PORTA = PORTA & 0b11111011;  // store done
		_delay_ms(1);
	}
	
	initEnemyBullets();
	while (1)
	{
		if(playerAlive)
			drawPlayer();
		else
		 erasePlayer();
		drawEnvironment();
		
		if (PIND & 0b00000001) {
			// button pressed
			//_delay_ms(22);
			
			updatePlayer(1);
		}
		if (PIND & 0b00000010) {
			// button pressed
			//_delay_ms(22);
			updatePlayer(0);
		}
		
		handleEnemyMovement();
		handleEnemyBullet();
		binToImage();
		
		for (int i = 0 ; i < 6; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				if (i == 0 && j == 0)
				PORTA = PORTA & 0b11111110;
				else
				PORTA = PORTA | 0b00000001;
				PORTA = PORTA | 0b00000010;
				PORTA = PORTA & 0b11111101;  // shift done
				PORTA = PORTA | 0b00000100;
				PORTA = PORTA & 0b11111011;  // store done
				PORTB = images[i][j];
				_delay_ms(1);
			}
		}
	}
}