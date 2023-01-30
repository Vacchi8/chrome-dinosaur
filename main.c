// build with "gcc main.c game.c menu.c appearance.c -lncurses -o dino.exe"

/**
	\file main.c
	\author Farbod Ahmadian
	\version 1.0
	\date 2023-01-30
*/

#include <ncurses.h>
#include <stdio.h>
#include "menu.h"
#include "minunit.h"

/**
	\fn int main(int argc, char **argv)
	\brief if the test functions return 1 start the game (se le funzioni test ritornano 1 avvia il gioco)
	\param int argc: arguments int passed to the program on the command line (argomenti int  passati al programma nella linea di comando) 
	\param char argv: arguments char passed to the program on the command line (argomenti char passati al programma nella linea di comando) 
	\return int fuction, return 0(funzione int, return 0)

*/

int main(int argc, char **argv) {
	// Run tests (if the -test command argument has been specified)
	int testsResult = run_all_tests(argc, argv);
	if(testsResult > 0)
		return testsResult != 0;

	//this is the normal start 
	initscr();
	start_color();
	curs_set(FALSE);
	startMenu();
	endwin();

    return 0;
}
