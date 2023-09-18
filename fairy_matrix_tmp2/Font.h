/*
 * Font.h
 *
 *  Created on: 14 jul. 2016
 *      Author: wvanvliet
 */
#include "Arduino.h"

#ifndef FONT_H_
#define FONT_H_

class Font {
public:
	Font();
	virtual ~Font();
	uint16_t
			getLetter5(char letter);

private:
	void
		fillLetters5(void),
		addLetter5(char _index, uint16_t _value, uint8_t counter);



	struct Letter5{
		char index;
		uint16_t value;
	};
	Letter5 letters5[42];
};

#endif /* FONT_H_ */
