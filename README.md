Wordle
Time of implementation - 16 hours
-------------------------------------------------------------------------------
	I have implemented 4 functions in creating the game: one for generating the
words to be guessed, one for drawing the game board, one for inputting and
coloring the corresponding letters, and one for the menu.
	The "board" function creates 30 boxes using the "newwin()" function, where
the words will be inputted, and stores the addresses of each window in a matrix
tobe accessed later.
	The "game" function implements the game mechanics by considering the cases
for each pressed letter: for "enter", it checks if enough letters have been
entered and colors them accordingly, or displays an error message if not enough
letters have been entered; for ":", it displays the menu to exit or start a new
game; for "backspace", it implements letter deletion by erasing the current
window completely, creating a new box, and moving the cursor. Additionally, it
considers the logic for coloring the letters, initially checking if the letters
entered from the keyboard are in the correct positions and then checking the
other letters.
	The "menu" function implements a menu with two options: starting a new game
or completely exiting the program, which can be accessed by pressing the
":" key.