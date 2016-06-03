#include <unistd.h>
#include <termios.h>
#include <stdio.h>


#define ASCII_ESC 27
void clearScreen (void) {
    printf( "%c[2J", ASCII_ESC );
}

void moveCursor (int v, int h) {
	char command[31];
	sprintf (command, "%c[%d;%df", ASCII_ESC, v, h);
	printf(command);
}

void homeCursor (void) {
	printf("%c[0;0H", ASCII_ESC);
}
char getch() {
        char buf = 0;
        struct termios old = {0};
        if (tcgetattr(0, &old) < 0)
                perror("tcsetattr()");
        old.c_lflag &= ~ICANON;
        old.c_lflag &= ~ECHO;
        old.c_cc[VMIN] = 1;
        old.c_cc[VTIME] = 0;
        if (tcsetattr(0, TCSANOW, &old) < 0)
                perror("tcsetattr ICANON");
        if (read(0, &buf, 1) < 0)
                perror ("read()");
        old.c_lflag |= ICANON;
        old.c_lflag |= ECHO;
        if (tcsetattr(0, TCSADRAIN, &old) < 0)
                perror ("tcsetattr ~ICANON");

        return (buf);
}

// int main() {
	// clearScreen();
	// moveCursor(8,0);
	// printf("hello\n");
	// homeCursor();
	// printf("AJFALWFJ\nFHWOFHO");
	// fflush(stdout);
	// char c;
	// getchar();
	// c = getch();
	// moveCursor(8,20);
	// printf("%c\n", c);
	// moveCursor(11,0);
	// printf("WHWHWH\nFWFWF\n");
// }