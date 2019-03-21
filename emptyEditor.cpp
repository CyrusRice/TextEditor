#include <iostream>
#include <ncurses.h>
#include <unistd.h>
#include <cctype>
using namespace std;

struct Node {
	char data;
	Node* next;
	Node* prev;
};

class Line {
	private:
	Node* head;
	Node* tail;
	Node* cursor;

	public:
	// start with a Line that only contains a node of ' '
	Line() {
		Node* firstNode = new Node;  // make new node
		firstNode->data = ' ';  // store a space in node
		firstNode->prev = NULL;  // point its prev to null
		firstNode->next = NULL;  // point its next to null
		head = firstNode;  // point head to this node
		tail = firstNode;	// point tail to this node
		cursor = firstNode;  // point cursor to this node
	}
	
	// same as above, but load str into the list also
	Line(string str) {
		// create first empty space node
		Node* firstNode = new Node;  // make new node
		firstNode->data = ' ';  // store a space in node
		firstNode->prev = NULL;  // point its prev to null
		firstNode->next = NULL;  // point its next to null
		head = firstNode;  // point head to this node
		tail = firstNode;	// point tail to this node
		cursor = firstNode;  // point cursor to this node
	
		// load str into the list
		for(int i = str.length() - 1; i >= 0; i--) {  // add str from right to left
			Node* ptr = head;  // ptr to follow head
			Node* newNode = new Node;  // new node to add
			newNode->data = str[i];  // store proper char into it
			head->prev = newNode;  // point node ahead of newnode to it
			head = head->prev;  // move head to new head of list
			head->next = ptr;  // point this node at that node
		}
	}

	// add the new node before the cursor
	void add(char key) {
		Node* ptr = cursor->prev;  // ptr to hold node before cursor
		if(ptr == NULL) {  // adding before head
			Node* newNode = new Node;  // make new node
			newNode->data = key;
			newNode->prev = NULL;     // set prev to null
			newNode->next = cursor;  // set next to cursor
			cursor->prev = newNode;  
			head = newNode;   // update head
		} else { // adding anywhere else
			Node* newNode = new Node;  // make new node  
			newNode->data = key;  
			ptr->next = newNode;   
			newNode->prev = ptr;
			newNode->next = cursor;
			cursor->prev = newNode;
		}
	}

	// remove the node before the cursor
	void remove() {
		Node* toDelete = cursor->prev; // mark node to be deleted
		if(toDelete == NULL) {  // if trying to delete before head, do nothing
			delete toDelete;
		} else {
			if(toDelete == head) {  // if deleting head
				cursor->prev = NULL;  
				delete toDelete;
				head = cursor;  // update head
			} else {
				Node* ptr = cursor->prev->prev; // set ptr to sandwich todelete between it and cursor
				ptr->next = cursor;  // link the two
				cursor->prev = ptr;
				delete toDelete;  // delete node
			}
		}
	}

	// print the line
	void print() {
		clear();
		int y = 0;  // coords to store position of chars
		int x = 0;
		Node* ptr = head;  // start at head
		while(ptr != cursor) {  // print up to cursor
			mvaddch(y, x, ptr->data);  // print char
			ptr = ptr->next;  // move to next node
			x++;  // update coord
		}
		start_color();
		attron(COLOR_PAIR(1));
		mvaddch(y, x, cursor->data);  // print cursor
		attroff(COLOR_PAIR(1));
		ptr = ptr->next;  // advance ptr
		x++; // update coords
		while(ptr != NULL) {  // print to end of lsit
			mvaddch(y, x, ptr->data); 	
			ptr = ptr->next;
			x++;
		}
	}

	// move the cursor left or right
	void moveCursor(char input) {
		switch(input) {
		case 'l':
			if(cursor->prev != NULL) {
				cursor = cursor->prev;
			}
		break;
		case'r':
			if(cursor->next != NULL) {
				cursor = cursor->next;
			}
		break;
		}	
	}

	// destructor deletes all nodes in the list
	~Line() {
		Node* ptr = head->next;
		while(ptr != NULL) {
			delete ptr->prev;
			ptr = ptr->next;
		}	
		delete tail;
		delete ptr;
	}
};


int main() {
	// set up ncurses stuff
	initscr();
	keypad(stdscr, TRUE);	// lets the arrow keys work
	start_color();
	init_pair(1, COLOR_BLACK, COLOR_WHITE);
	noecho();				// makes the keystroke NOT be printed
	nodelay(stdscr, TRUE);	// makes getch NOT WAIT for input
	curs_set(0);			// turn off the regular cursor
	
	// declare line
	Line l("Start with this text"); // construct the starting line
	int input = 'a';
	// modification loop
	while(input != 27) { // 27 is the escape key
		// get input
		input = getch();
		
		// put the input into the line or move the cursor
		if(isalpha(input) || input == ' ') {
			l.add(input);
		} else if(input == KEY_LEFT) {
			l.moveCursor('l');
		} else if(input == KEY_RIGHT) {
			l.moveCursor('r');
		} else if(input == KEY_BACKSPACE) { // backspace
			l.remove();
		}

		// display line with cursor and pause
		l.print();
		refresh();
		usleep(10000);
	}
	// exit
	endwin();
	return 0;
}
