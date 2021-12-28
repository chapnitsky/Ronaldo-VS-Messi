# Ronaldo-VS-Messi
A game in C++ language of countering images



The program is basically a fun game. You play the messi character while you need to avoid the
ronaldo characters which represents the enemys. Your movement keys are the arrows in your keyboard 
or the numpad arrows. The enemys move randomly and change direction every couple of seconds and when
they bump into each other they create a bigger enemy that contains both of them. If you touch an enemy
you lose and if you avoid them long enough untill they are all connected together you win!

How to compile: Terminal -> Run Build Task -> Build -> Continue
How to run: F5 or type in Terminal: ./Ex7

The class MyApp is acting like the main of the game and inheriting from wxApp. The function OnInit() initiallising the game
by creating the frame with MyFrame class, loading the images of the characters, creating the player and 3 enemys and 
giving them positions.

The class MyFrame creates the game windows and inheriting from wxFrame. It does it with creating
a canvas by the class MyCanvas;

The class MyCanvas is inheriting from wxScrolledWindow and incharge on the functioning of
the game window. It holds the player, a timer and the enemys that are inside a queue of pointers
of the class DragShape. It also has functions that repaints the window, drawing the characters,
getting input from the keyboard and by that moving the player and also the main function is OnTimer()
that most of the code is inside. OnTimer() happens every 50 milliseconds. I will expand later on this 
function.Also a default construcor and a destructor.

The class DragShape is inheriting from wxObject and represents a moving character on the window.
The player in the game is a DragShape and also the enemys. The class has a direction and a counter
that are type of Integer, position and an image. Also it has a function that getting the closing
rectangle of the character image. Also functions that getting,setting and generating a new direction.
reseting, incrementing and getting the counter. getting and setting the image and the position.
Also a default construcor that also generate a random direction, reseting the counter
and also a destructor.

The last class is Pair and inheriting from DragShape class. Pair is a bunch of enemys, 2 or more,
that are "glued" together into one bigger enemy when they bump each other. 
It has its own 2 DragShape pointers, drawing, closing rectangle, setting and getting position,
and also a default construcor and a destructor.


Back to MyCanvas class, OnTimer() function:
direction: 1 = UP, 2 = DOWN, 3 = RIGHT, 4 = LEFT, 5 = UP RIGHT, 6 = UP LEFT
          , 7 = DOWN RIGHT and 8 = DOWN LEFT.
The first for loop is moving the enemys by their directions and incrementing their counter each movement.
It does it by poping each enemy from the queue and then returning it back to the back of the queue.
If their counter equals 170 it generate a new direction, also if they bump into the walls of the game
windows they change direction with considering their current direction. For instance, if an enemy
bump into the left wall with a direction of up left (6) it changes its direction to up right (5).
The second loops are a for inside a for that checks for an enemy that bumps another enemy and need
to be paired together with creating a new Pair object and inserting only it to the queue.
It also checks if after creating a Pair and inserting it to the queue, if it the only Pair inside the
queue that means you won and showing a message that you won and exiting the game after deleting the memory.
It also checks if any kind of enemy (DragShape or Pair) bumped into the player that means you lose
and showing a message that you lost and exiting the game after deleting the memory. 
