
#include <ncurses.h>

#define KEY_SPACE 32

int main() {
  int ch;
  
  initscr();
  raw();
  //keypad(stdscr, TRUE);
  noecho();

  printw("Type any character to see it in bold\n");
  while ((ch = getch()) != ' ') {
    /*
    if (ch == KEY_F(1))
      printw("F1 Key pressed");
    else {
      printw("The pressed key is ");
    */
    attron(A_BOLD);
    printw("%c", ch);
    attroff(A_BOLD);
    /*printw("That's key %d if you didn't know.", ch);
      }*/
    refresh();
  }
  refresh();
  getch();
  endwin();

  return 0;
}
