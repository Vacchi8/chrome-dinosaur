/**
	\file appearance.c
	\author Farbod Ahmadian
	\version 1.0
	\date 2023-01-29
*/
#include <ncurses.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

#include "menu.h"
#include "game.h"
#include "appearance.h"
/**
	\fn void startMenu
	\brief  show best score and get user information (mostra il miglior punteggio e prende le informazioni dell'utente)
	\param  no parameters needed (nessun parametro richiesto)
	\return void function, no return (funzione void, nessun return)
*/
void startMenu() {
	struct user firstUser;
	int highScore;
    // Read high score
	FILE *highScoreFile;
	highScoreFile = fopen("./highScore.txt", "r");
	fscanf(highScoreFile, "%d", &highScore);
	fclose(highScoreFile);
	int maxX = getmaxx(stdscr)/2;
	int maxY = getmaxy(stdscr)/2;
	init_pair(3,COLOR_GREEN,COLOR_BLACK);
	attron(COLOR_PAIR(3));
	showTrex(maxY, maxX);
	attroff(COLOR_PAIR(3));
    // Get info
	mvprintw(maxY+1, maxX-28, "Write inputs and press Enter to start the game.");
    mvprintw(maxY+2, maxX-26, "When you had prize, fire it with 'k' key!");
	mvprintw(maxY+3, maxX-21, "You can jump with space key!");
	mvprintw(maxY+4, maxX-15, "Name: ");
	getstr(firstUser.name);
	mvprintw(maxY+5, maxX-15, "Last name: ");
	getstr(firstUser.lastName);
	mvprintw(maxY+6, maxX-15, "Age: ");
	getstr(firstUser.age);
	noecho();
	startEngine(highScore, firstUser);
}

/**
	\fn void endGame(int score, int highScore, int diY, int diX, struct user firstUser)
	\brief  saves the score and takes you back to the initial menu (salva il punteggio e ti riporta al menu iniziale )
	\param int highScore: best score (miglior punteggio)
	\param struct user firsUsers: structure for user data (struttura per dati utente)
	\param int score: score done(punteggio fatto)
	\param int y: diY coordinate of the characters to clean (coordinata y dei caratteri da stampare)
	\param int x: diX coordinate of the characters to clean (coordinata x dei caratteri da stampare)
	\return void function, no return (funzione void, nessun return)
*/

void endGame(int score, int highScore, int diY, int diX, struct user firstUser) {
	nodelay(stdscr, FALSE);
	init_pair(2,COLOR_RED,COLOR_BLACK);
    // Save
	if (score > highScore) {
		highScore = score;
		FILE *highScoreFile;
		highScoreFile = fopen("./highScore.txt", "w");
		fprintf(highScoreFile, "%d", highScore);
		fclose(highScoreFile);
	}
	int maxX = getmaxx(stdscr)/2;
	int maxY = getmaxy(stdscr)/2;
	attron(COLOR_PAIR(2));
	showLoss(maxY, maxX);
	mvprintw(diY-4, diX, "          X-X ");
	mvprintw(diY, diX, "      ||");
	char keyToExit = getch();
    // Exit or restart
	if (keyToExit == 'r') {
		attroff(COLOR_PAIR(2));
		startEngine(highScore, firstUser);
	}
	else if (keyToExit == 'q') {
		return;
	}
	else {
		endGame(score, highScore, diY, diX, firstUser);
	}
}
