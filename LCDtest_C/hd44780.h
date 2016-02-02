//------------------------------------------------------
/* File:       Library for HD44780 compatible displays  */
/* Version:	   v2.01  						 			*/
/* Author:     GrAnd/www.MakeSystem.net		 			*/
/* Tested on:  AVR, STM32F10X		 	 	 		 	*/
/* License:	   GNU LGPLv2.1		 		 	 			*/
//------------------------------------------------------
/* Copyright (C)2014 GrAnd. All right reserved 			*/
//------------------------------------------------------

/*
	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Lesser General Public
	License as published by the Free Software Foundation; either
	version 2.1 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public
	License along with this library; if not, write to the Free Software
	Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

Contact information :
						mail@makesystem.net
						http://makesystem.net/?page_id=2
*/

#ifndef HD44780_H
#define HD44780_H

#include "lcd_cfg.h"

#ifdef __cplusplus
extern "C" {
#endif

//-------------------------------
// CONSTANTS
//-------------------------------
#define CGRAM	 	   	  		 		5u	/* used in "lcd_goto(CGRAM,address)" function */
#define RIGHT   	  					'>'
#define LEFT       			  			'<'
#define LCD_1st_LINE					1u
#define LCD_2nd_LINE					2u
#define LCD_3rd_LINE					3u
#define LCD_4th_LINE					4u
#define START_ADDRESS_1st_LINE			0x00u
#define START_ADDRESS_2nd_LINE			0x40u
/* 5x8 pixel font only */
#define FONT_HEIGHT    					8u	/* 8 pixels */
#define FONT_WIDTH						5u	/* 5 pixels */
#define PROGRESS_BAR_MAX_LOAD			(PROGRESS_BAR_WIDTH * FONT_WIDTH) /* 100% full */

//-------------------------------
// DISPLAY CONFIG MACROS
//-------------------------------
#define DISPLAY_CONFIG_4bit_2L_5x8		0x28u /* Use 4-bit interface, 2 or 4 Lines, 5x8 pixel resolution */
#define DISPLAY_CONFIG_4bit_1L_5x8		0x20u /* Use 4-bit interface, 1 Line, 5x8 pixel resolution */

//-------------------------------
// ENTRY MODE MACROS
//-------------------------------
#define ENTRY_MODE_DEC_NO_SHIFT			0x04u // справа налево, без сдвига видимой части DDRAM памяти
#define ENTRY_MODE_DEC_WITH_SHIFT     	0x05u // справа налево, со сдвигом видимой части DDRAM памяти
#define ENTRY_MODE_INC_NO_SHIFT	      	0x06u // слева направо, без сдвига видимой части DDRAM памяти
#define ENTRY_MODE_INC_WITH_SHIFT     	0x07u // слева направо, со сдвигом видимой части DDRAM памяти

//-------------------------------
// VIEW_MODE MACROS
//-------------------------------
#define VIEW_MODE_DispOn_BlkOn_CrsOn	0x0Fu /* Display On, Blink On, Cursor On */
#define VIEW_MODE_DispOn_BlkOff_CrsOn	0x0Eu /* Display On, Blink Off, Cursor On */
#define VIEW_MODE_DispOn_BlkOn_CrsOff	0x0Du /* Display On, Blink On, Cursor Off */
#define VIEW_MODE_DispOn_BlkOff_CrsOff	0x0Cu /* Display On, Blink Off, Cursor Off */
#define VIEW_MODE_DispOff_BlkOff_CrsOff	0x08u /* Display Off, Blink Off, Cursor Off */

//-------------------------------
// LCDlib API
//-------------------------------
// Очищает всю DDRAM память, т.е. всю видимую и скрытую области дисплея.
extern void lcd_clrscr(void);

// Возвращает курсор в первую ячейку первой строки дисплея (DDRAM памяти), т.е. в верхний левый угол дисплея.																
extern void lcd_return(void);

// Функцию lcd_setmode(param) следует использовать для установки нового режима ввода (entry mode) или нового режима отображения (display control). Доступны следующие режимы работы дисплея (в файле hd44780.h) :
extern void lcd_setmode(uint8_t param);

// Функция lcd_puts(“string”) предназначена для вывода символьной строки (массив типа char, заканчивающийся нулем) на дисплей (DDRAM память). Также, для большего удобства работы с дисплеем, можно активировать форматированный вывод символов (системные символы : ‘\n‘, ‘\t‘, ‘\r‘) на дисплей при помощи макроса USE_FORMATTED_OUTPUT.
extern void lcd_puts(const uint8_t *str);

// Функция lcd_putc(data) используется для записи символов в DDRAM (вывода символов на дисплей) или CGRAM память (предварительно следует перейти в адресное пространство CGRAM памяти при помощи функции lcd_goto(CGRAM, address)).
extern void lcd_putc(uint8_t data);		

// Функция lcd_loadchar(symbol, position) используется для загрузки пользовательского символа (шрифт 5?8 пикселей) в CGRAM память.
extern void lcd_loadchar(uint8_t *vector, uint8_t position);								

// Функция lcd_drawchar, как и функция lcd_loadchar, записывает пользовательский символ (параметр vector) в некоторую область CGRAM память (параметр position), но в отличии от lcd_loadchar, сразу выводит этот символ на дисплей в указанную строку и ячейку DDRAM памяти (параметры “line” и “address”).
extern void lcd_drawchar(uint8_t *vector, uint8_t position, uint8_t line, uint8_t address);	

// Функция lcd_backspace() используется для очистки ячейки DDRAM памяти слева от курсора. Эту функцию следует использовать только при режиме ввода “слева направо”.
extern void lcd_backspace(void);															

// Каждый вызов функции lcd_scroll смещает видимую часть дисплея (DDRAM памяти) влево/вправо (при помощи макросов LEFT и RIGHT) на одну позицию. При помощи данной функции можно получить эффект “бегущей строки”.
extern void lcd_scroll(uint8_t direction);		

// Каждый вызов функции cursor_shift смещает курсор влево/вправо (при помощи макросов LEFT и RIGHT) на одну позицию. Эту функцию можно использовать для редактирования в режиме реального времени ошибок при вводе значений с клавиатуры (к примеру “кодовый замок”, “терминал ввода” и т.п.).
extern void cursor_shift(uint8_t direction);	

// Функция lcd_itos выводит целые числа (в диапазоне +/- 2147483647) на дисплей.
extern void lcd_itos(int32_t value);			

// Функция lcd_ftos выводит на дисплей числа с плавающей точкой. Параметр “n” указывает сколько символов после точки отображать.
extern void lcd_ftos(float value, uint8_t n);	

// Функция lcd_ntos используется для вывода нескольких разрядов целого числа (в диапазоне : 0 .. 4294967295) на дисплей. В случае если у выводимого числа меньше разрядов чем указано в параметре “nDigit”, то тогда оставшиеся разряды заполняются нулями, т.е. “выводимое число” занимает фиксированное число знакомест дисплея
extern void lcd_ntos(uint32_t value, uint8_t n);

// Индикатор выполнения мгновенно заполняется до значения передаваемого через параметр “data”. Индикатору можно присваивать значения от “0? до “PROGRESS_BAR_MAX_LOAD” (100% заполнение или максимальное число шагов для полного заполнения индикатора). Если присвоить макросу USE_REGRESS_BAR = “1?, то во время работы, заполнение индикатора может как увеличиваться (передавая значение больше предыдущего) так и уменьшаться (передавая значение меньше предыдущего).
extern void lcd_drawbar(uint8_t data);			

// Функция lcd_clrbar( ) очищает все ячейки индикатора выполнения, т.е. переводит индикатор в начальное состояние (0% заполнение).
extern void lcd_clrbar(void);					

// Функция lcd_init( ) вызывается (до начала использования других функций HD44780-драйвера) для инициализации дисплея (конфигурация дисплея, выбор режима ввода и т.д.). Также, до вызова функции lcd_init( ), пользователь должен настроить все выводы управляющего микроконтроллера (LCD_E, LCD_RS, LCD_RW (если используется), LCD_D7, .. LCD_D4) как выходы!
extern void lcd_init(void);						

#ifdef __cplusplus
}
#endif

#endif /* HD44780_H */

//-------------------------------
/* END OF FILE */
//-------------------------------
