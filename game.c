/** 
    \file game.c
    \author Farbod Ahmadian
    \version 1.0
    \date 2023-01-27
*/


#include <ncurses.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

#include "menu.h"
#include "game.h"
#include "appearance.h"
#include "minunit.h"

/**
    \fn int checkGame(int y, int x, int diY, int diX)
    \brief Check if the game is going to finish in this turn of loop (controlla se la partita finisce in questo giro di ciclo)
    \param int y: y coordinate  (coordinata y)
    \param int x: x coordinate  (coordinata x)
    \param int diY: y coordinate of the dinosaur (coordinata y del dinosauro)
    \param int diX: x coordinate of the dinosaur (coordinata y dei dinosauro)
    \return returns '1' if 'diY == y' so if the dinosaur touch the cactus, instead returns '0' if the absolute value of ((diX+14)-x) is <= 4   (ritorna '1' se 'diY == y' quindi se il dinosauro tocca il cactus, invece ritorna '0' se il valore assoluto di ((diX+14)-x) e' <= 4)
*/

int checkGame(int y, int x, int diY, int diX) {
    if (diY == y) {
        if (abs((diX+14)-x) <= 4) {
            return 0;
        }
    }
    return 1;
}

/**
    \fn int computeTime(int delayTime)
    \brief Make the game faster (rende il gioco piu' veloce)
    \param int delayTime: delay time (tempo di ritardo)
    \return returns the modified delay time to speed op the game (ritorna il tempo di ritardo modificato in modo da velocizzare il gioco) 
*/

int computeTime(int delayTime) {
    if (delayTime >= 250000) {
        delayTime -= 1000;
    }
    else if (delayTime >= 200000) {
        delayTime -= 600;
    }
    else {
        delayTime -= 200;
    }
    return delayTime;
}

/**
    \fn void showdinosaur(int diY, int diX)
    \brief Check which dinosaur should be printed (controlla quale dinosauro deve essere stampato)
    \param int y: diY coordinate of dinosaur (coordinata y del dinosauro)
    \param int x: diX coordinate of dinosaur (coordinata x del dinosauro)
    \return void function, no return (funzione void, nessun return)
*/

void showdinosaur(int diY, int diX) {
    static int counter = 0;
    if (counter == 0) {
        dinosaur1(diY, diX);
        counter++;
    }
    else {
        dinosaur2(diY, diX);
        counter--;
    }
}

/**
    \fn int computePrize(int score, int usedPrize)
    \brief Give user the prize, so as time passes, the score increases (da all'utente il 'premio', quindi con il passare del tempo lo score incrementa)
    \param int score: the score before the new loop update (lo score prima dell'ultimo ciclo)
    \param int usedPrize:
    \return returns '1' if score gets certain values, otherwise returns '0' (ritorna '1' se lo score ottiene certi valori, altrimenti ritorna '0')
*/

int computePrize(int score, int usedPrize) {
    if (score >= 20 && score <= 40 && usedPrize == 0) {
        return 1;
    }
    else if (score >= 60 && score <= 80 && usedPrize <= 1) {
        return 1;
    }
    else if (score >= 100 && score <= 120 && usedPrize <= 2) {
        return 1;
    }   
    return 0;
}

/**
    \fn void startEngine(int highScore, struct user firstUser)
    \brief The main engine of the game, it calls all the funcions needed (Il motore principale del gioco, chiama tutte le funzioni necessarie)
    \param int highScore: the highest score achieved (lo score piu' alto raggiunto)
    \param struct user firstUser: struct containing the data of the player (struct contenente i dati del giocatore)
    \return void function, no return (funzione void, nessun return)
*/

void startEngine(int highScore, struct user firstUser) {
    srand(time(NULL));
    int x, y, diX=5, prize=0, usedPrize=0, score=0, delayTime = 300000
    , gameStatus=1, cactusNum=0;
    int maxX=getmaxx(stdscr);
    x = maxX-20;
    y = getmaxy(stdscr)-6;
    int diY = y;
    int arrowX=(diX+15), arrowY=(diY-3);
    char userInput;
    int jumping=-1;
    bool fire=FALSE;
    clear();
    nodelay(stdscr, TRUE);
    init_pair(1,COLOR_WHITE,COLOR_BLACK);
    init_pair(4,COLOR_BLUE,COLOR_BLACK);
    init_pair(5,COLOR_GREEN,COLOR_BLACK);
    init_pair(6,COLOR_YELLOW,COLOR_BLACK);
    while (gameStatus == 1) {
        userInput = getch();
        // Show day or night
        if((score/50)%2 != 0) {
            moon(10, (maxX/2)-10);
        }
        else {
            attron(COLOR_PAIR(1));
            sun(10, (maxX/2)-10);           
        }
        // clear arrow
        if (fire) {
            mvprintw(arrowY, arrowX-2, " ");
        }

        score++;
        // Show informations
        mvprintw(1, 6, "%s %s %s", firstUser.name, firstUser.lastName, firstUser.age);
        mvprintw(1, getmaxx(stdscr)-9, "%d:%d", highScore, score);
        // Use prize to destroy cactus
        prize = computePrize(score, usedPrize);
        mvprintw(3, 6, "Prize: %d    ", prize);
        if (prize == 1) {
            if (userInput == 'k') {
                usedPrize++;
                fire = TRUE;
            }
        }
        if (fire) {
            mvprintw(arrowY, arrowX, "*");
            arrowX += 2;
        }
        if ((x+4)-arrowX <= 1 && fire) {
            clearCactus1(y, x-1);
            mvprintw(arrowY, arrowX-2, " ");
            x = getmaxx(stdscr)-20;
            fire = FALSE;
            arrowX = diX+15;
        }
        // ----------
        box(stdscr, ACS_VLINE, ACS_HLINE);
        //for clearing screen
        cleardinosaurUp(diY, diX);
        if (x <= 7) {
            x = getmaxx(stdscr)-20;
            cactusNum = rand() % 2;
        }
        // if input is equal to ' ' then jump
        if (userInput == ' ' && jumping<0) {
            diY -= 7;
            jumping = 3;
        }
        showdinosaur(diY, diX);
        if (userInput == ' ') {
            cleardinosaurDown(diY, diX);        
        }
        if (x-diX <= 7) {
            x -= 10;
        }
        if (cactusNum == 0) {
            cactus1(y, x);
        }
        else {
            cactus2(y, x);
        }
        if (x-diX <= 7) {
            x += 10;
        }
        gameStatus = checkGame(y, x, diY, diX);
        // Bring back dinosaur
        jumping--;
        if (jumping==0) {
            diY += 7;
        }
        mvhline(y+1, 1, '-', getmaxx(stdscr)-3);
        refresh();
        clearCactus1(y, x);
        refresh();
        usleep(delayTime);
        x -= 7;
        delayTime = computeTime(delayTime);
        userInput = 'q';
    }

    endGame(score, highScore, diY, diX, firstUser);
    attroff(COLOR_PAIR(1));
}


//
// START of tests section
//

// sample variables to be tested
int foo = 4;
int bar = 5;


// init tests vars
int tests_run = 0;
int tests_passed = 0;

// sample test function 1
void test_checkGame() {
    int  ritorno= 0;
    mu_assert("ERRORE TEST 1 ", checkGame(1,14,1,1) == ritorno);
}

void test_checkGame2() {
    int  ritorno= 1;
    mu_assert("ERRORE TEST 2", checkGame(1,14,1,12) == ritorno);
}

void test_checkGame3() {
    int  ritorno= 0;
    mu_assert("ERRORE TEST 3", checkGame(1,15,1,5) == ritorno);
}

void test_checkGame4() {
    int  ritorno= 1;
    mu_assert("ERRORE TEST 4", checkGame(1,-16,1,4) == ritorno);
}

void test_checkGame5() {
    int  ritorno= 0;
    mu_assert("ERRORE TEST 5", checkGame(1,5,1,-5) == ritorno);
}

void test_checkGame6() {
    int  ritorno= 1;
    mu_assert("ERRORE TEST 6", checkGame(2,4,92,14) == ritorno);
}

void test_checkGame7() {
    int  ritorno= 1;
    mu_assert("ERRORE TEST 7", checkGame(24,10,13,15) == ritorno);
}

void test_checkGame8() {
    int  ritorno= 1;
    mu_assert("ERRORE TEST 8", checkGame(1,3,9,4) == ritorno);
}

void test_checkGame9() {
    int  ritorno= 1;
    mu_assert("ERRORE TEST 9", checkGame(2,18,1,24) == ritorno);
}
void test_checkGame10() {
    int  ritorno= 0;
    mu_assert("ERRORE TEST 10", checkGame(5,23,5,7) == ritorno);
}
void test_computePrize1() {
    int ritorno = 0;
    mu_assert("ERRORE TEST 1 COMPUERPRIZE", computePrize(0, 0)== ritono);

}
void test_computePrize2() {
    int ritorno = 0;
    mu_assert("ERRORE TEST 2 COMPUERPRIZE", computePrize(10, 0)== ritono);

}
void test_computePrize3() {
    int ritorno = 1;
    mu_assert("ERRORE TEST 3 COMPUERPRIZE", computePrize(20, 0)== ritono);

}
void test_computePrize4() {
    int ritorno = 1;
    mu_assert("ERRORE TEST 4 COMPUERPRIZE", computePrize(65, 1)== ritono);

}
void test_computePrize5() {
    int ritorno = 0;
    mu_assert("ERRORE TEST 5 COMPUERPRIZE", computePrize(55, 1)== ritono);

}
void test_computePrize6() {
    int ritorno = 0;
    mu_assert("ERRORE TEST 6 COMPUERPRIZE", computePrize(99, 2)== ritono);

}
void test_computePrize7() {
    int ritorno = 1;
    mu_assert("ERRORE TEST 7 COMPUERPRIZE", computePrize(110, 2)== ritono);

}
void test_computePrize8() {
    int ritorno = 0;
    mu_assert("ERRORE TEST 8 COMPUERPRIZE", computePrize(110, 1)== ritono);

}
void test_computePrize9() {
    int ritorno = 0;
    mu_assert("ERRORE TEST 9 COMPUERPRIZE", computePrize(65, 2)== ritono);

}
void test_computePrize10() {
    int ritorno = 1;
    mu_assert("ERRORE TEST 10 COMPUERPRIZE", computePrize(69, 1)== ritono);

}
void test_computetime1() {
	int ritorno = 1;
	mu_assert("ERRORE TEST 1 COMPUTETIME", computetime(0) == ritorno);
}
void test_computetime2() {
	int ritorno = 4;
	mu_assert("ERRORE TEST 2 COMPUTETIME", computetime(3) == ritorno);
}
void test_computetime3() {
	int ritorno = 48;
	mu_assert("ERRORE TEST 3 COMPUTETIME", computetime(47) == ritorno);
}
void test_computetime4() {
	int ritorno = 12;
	mu_assert("ERRORE TEST 4 COMPUTETIME", computetime(11) == ritorno);
}
void test_computetime5() {
	int ritorno = 41;
	mu_assert("ERRORE TEST 5 COMPUTETIME", computetime(40) == ritorno);
}
void test_computetime6() {
	int ritorno = 101;
	mu_assert("ERRORE TEST 6 COMPUTETIME", computetime(100) == ritorno);
}
void test_computetime7() {
	int ritorno = 104;
	mu_assert("ERRORE TEST 7 COMPUTETIME", computetime(103) == ritorno);
}
void test_computetime8() {
	int ritorno = 210;
	mu_assert("ERRORE TEST 8 COMPUTETIME", computetime(209) == ritorno);
}
void test_computetime9() {
	int ritorno = 847;
	mu_assert("ERRORE TEST 9 COMPUTETIME", computetime(846) == ritorno);
}
void test_computetime10() {
	int ritorno = 357;
	mu_assert("ERRORE TEST 10 COMPUTETIME", computetime(356) == ritorno);
}


/**
    \fn static char * test_foo()
    \brief test
    \return void function, no return (funzione void, nessun return)
*/
static char * test_foo() {
    mu_assert("error, foo != 7", foo == 7);
}

// sample test function 2

/**
    \fn static char * test_bar()
    \brief test
    \return void function, no return (funzione void, nessun return)
*/

static char * test_bar() {
    mu_assert("error, bar != 5", bar == 5);
}

// put all tests here

/**
    \fn void all_test()
    \brief call the test functions (chiama le funzioni test)
    \return void function, no return (funzione void, nessun return)
*/
void all_tests() {
    mu_run_test(test_foo);
    mu_run_test(test_bar);
    mu_run_test(test_checkGame);
    mu_run_test(test_checkGame2);
    mu_run_test(test_checkGame3);
    mu_run_test(test_checkGame4);
    mu_run_test(test_checkGame5);
    mu_run_test(test_checkGame6);
    mu_run_test(test_checkGame7);
    mu_run_test(test_checkGame8);
    mu_run_test(test_checkGame9);
    mu_run_test(test_checkGame10);
    mu_run_test(test_computePrize1);
    mu_run_test(test_computePrize2);
    mu_run_test(test_computePrize3);
    mu_run_test(test_computePrize4);
    mu_run_test(test_computePrize5);
    mu_run_test(test_computePrize6);
    mu_run_test(test_computePrize7);
    mu_run_test(test_computePrize8);
    mu_run_test(test_computePrize9);
    mu_run_test(test_computePrize10);
	mu_run_test(test_computetime1);
	mu_run_test(test_computetime2);
	mu_run_test(test_computetime3);
	mu_run_test(test_computetime4);
	mu_run_test(test_computetime5);
	mu_run_test(test_computetime6);
	mu_run_test(test_computetime7);
	mu_run_test(test_computetime8);
	mu_run_test(test_computetime9);
	mu_run_test(test_computetime10);
}

// call this to run all tests

/**
    \fn int run_all_tests(int argc, char **argv)
    \brief call the test functions and print the result (chiama le funzioni test)
    \param int argc: indicates the number of parameters entered in the program (indica il numero di stringhe inserite nel programma)
    \param char **argv: array of the parameters entered in the program (array dei parametri inseriti nel programma)
    \return int fuction, return tests_run-tests_passed or -1(funzione int, return  tests_run-tests_passed o -1)
    */
int run_all_tests(int argc, char **argv) {
    if(argc<2 || strcmp(argv[1],"-test")!=0) {
        return -1;
    }
    
    printf("--- RUNNING TESTS ---\n");
    all_tests();
    printf("--- SUMMARY ---\n");
    printf("Total number of tests: %d\n", tests_run);
    printf("Tests passed: %d\n", tests_passed);
    return tests_run-tests_passed;
}
//
// END of tests section
//