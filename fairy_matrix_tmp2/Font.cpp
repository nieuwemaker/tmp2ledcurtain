/*
 * Font.cpp
 *
 *  Created on: 14 jul. 2016
 *      Author: wvanvliet
 */

#include "Font.h"



Font::Font() {
	fillLetters5();
}

void Font::fillLetters5(void){
	uint8_t counter = 0;
	addLetter5('0', 0b0111101101101111, counter++);
	addLetter5('1', 0b0010110010010111, counter++);
	addLetter5('2', 0b0111001111100111, counter++);
	addLetter5('3', 0b0111001011001111, counter++);
	addLetter5('4', 0b0101101111001001, counter++);
	addLetter5('5', 0b0111100111001111, counter++);
	addLetter5('6', 0b0111100111101111, counter++);
	addLetter5('7', 0b0111001001001001, counter++);
	addLetter5('8', 0b0111101111101111, counter++);
	addLetter5('9', 0b0111101111001111, counter++); // 10
	addLetter5('A', 0b0010101111101101, counter++);
	addLetter5('B', 0b0110101110101110, counter++);
	addLetter5('C', 0b0011100100100011, counter++);
	addLetter5('D', 0b0110101101101110, counter++);
	addLetter5('E', 0b0111100111100111, counter++);
	addLetter5('F', 0b0111100111100100, counter++);
	addLetter5('G', 0b0011100111101011, counter++);
	addLetter5('H', 0b0101101111101101, counter++);
	addLetter5('I', 0b0111010010010111, counter++);
	addLetter5('J', 0b0001001001101010, counter++); // 20
	addLetter5('K', 0b0101101110101101, counter++);
	addLetter5('L', 0b0100100100100111, counter++);
	addLetter5('M', 0b0101111111101101, counter++);
	addLetter5('N', 0b0101111111111101, counter++);
	addLetter5('O', 0b0010101101101010, counter++);
	addLetter5('P', 0b0110101110100100, counter++);
	addLetter5('Q', 0b0010101101111011, counter++);
	addLetter5('R', 0b0110101111110101, counter++);
	addLetter5('S', 0b0011100010001110, counter++);
	addLetter5('T', 0b0111010010010010, counter++); // 30
	addLetter5('U', 0b0101101101101011, counter++);
	addLetter5('V', 0b0101101101010010, counter++);
	addLetter5('W', 0b0101101111111101, counter++);
	addLetter5('X', 0b0101101010101101, counter++);
	addLetter5('Y', 0b0101101010010010, counter++);
	addLetter5('Z', 0b0111001010100111, counter++);
	addLetter5(' ', 0b0000000000000000, counter++);
	addLetter5('.', 0b0000000000000010, counter++);
	addLetter5(':', 0b0000010000000010, counter++);
	addLetter5('!', 0b0010010010000010, counter++); // 40
	addLetter5('*', 0b0101010111010101, counter++);
	addLetter5('/', 0b0000001010100000, counter++);
}

void Font::addLetter5(char _index, uint16_t _value, uint8_t counter){
	Letter5 tmp;
	tmp.index = _index;
	tmp.value = _value;
	letters5[counter] = tmp;
}

uint16_t Font::getLetter5(char letter){
	for(uint8_t i=0;i<42;i++){
		if(letters5[i].index == letter){
			return letters5[i].value;
		}
	}
	return 0b0000000000000000;
}

Font::~Font() {
	// TODO Auto-generated destructor stub
}

