I am developing a C++ OpenGL/GLUT midterm project. I need to create an original interactive graphical application using geometric primitives, colors, text, keyboard/mouse interaction, conditions, loops, functions, and vertex arrays. Help me brainstorm ideas, but do not choose the final project for me.
Think of an original visual application, simulation, scene, mini-game, educational visualization, or interactive environment.

Possible categories include:

Mini-game
Traffic simulation
Smart city
Solar system
Interactive house
Airport scene
Aquarium
Farm simulation
Weather visualization
Campus map
Parking simulation`
Factory visualization
Interactive dashboard
Maze
Drawing application
Sports visualization
Animated landscape
Traffic-light system
Robot control display
Space exploration
Environmental simulation
These are only categories. You must develop your own specific idea.

Your project should have a purpose, not simply be a collection of random shapes.

The idea must be able to answer these following questions:

What real-world situations could be represented graphically?
What interactive OpenGL projects can be created from basic geometric primitives?
How could I make my idea more creative?
What objects should appear in my scene?
Which objects can be constructed using points, lines, triangles, quads, or polygons?
What should happen when the user presses keyboard keys?
What useful actions could the mouse perform?
What information should be displayed as text?
What parts of my project could use loops?
What conditions or decisions should control the application's behavior?
How can I make the application change while the user interacts with it?
Which repeated graphics could be stored/rendered more efficiently using arrays?
What invalid actions or boundary conditions should my application prevent?
What additional feature would make the application more interesting without making it too difficult?
How can I test whether every interaction works correctly?

A static drawing alone is NOT sufficient for the project.

Your application must respond to the user.

Module 3 specifically introduces GLUT's event-driven approach and keyboard, mouse, and display callbacks. Module 03(3) Input handling is intended to capture keyboard presses, mouse movements, and clicks and connect them to application behavior. Module 03(3)

Your project must therefore implement at least five meaningful user interactions.

Examples:

Move an object left/right/up/down.
Start/pause animation.
Change object color.
Change speed.
Select an object.
Add an object.
Remove an object.
Reset the scene.
Toggle day/night.
Activate/deactivate something.
Click an object to trigger an action.
Show/hide information.
Change scene or mode.
Increase/decrease a value.
Do not make all interactions merely change colors. Different controls should perform different meaningful actions.

The application should feel like a small interactive system, not a programming exercise.

It must contain:

At least 5 visually distinguishable graphical objects

For example, a car should not count as one triangle. A car can itself be a composite object consisting of body, windows, wheels, lights, etc.

At least 3 types of OpenGL primitives

Examples include:

GL_POINTS
GL_LINES
GL_LINE_STRIP
GL_LINE_LOOP
GL_TRIANGLES
GL_TRIANGLE_STRIP
GL_QUADS
GL_POLYGON
Module 2 establishes primitives as the fundamental graphical entities represented by vertices. Module 02

At least 3 application states

For example:

MENU → RUNNING → PAUSED

or:

DAY → SUNSET → NIGHT

or:

RED LIGHT → YELLOW LIGHT → GREEN LIGHT

This forces students to use programming logic rather than merely drawing a scene.

must implement at least one feature beyond the basic requirements.

Examples include:

Animation
Collision detection
Score system
Timer
Day/night transition
Multiple scenes
Menu system
Character movement
Object spawning
Simple physics
Dynamic weather
Traffic simulation
Interactive buttons
Mouse selection
Difficulty levels
Randomized objects
Health/progress bar

Suggest a feature but I will decide which feature to implement.

The final program must meaningfully demonstrate concepts from the course.

Requirement	Expected Application
C++	Main programming language
OpenGL/GLUT	Graphics environment
Functions	Organize major program behaviors
User Interaction	Keyboard and/or mouse
Conditions	Control decisions and states
Loops	Handle repeated objects/operations
Variables	Maintain program state
Geometric Primitives	Construct visual objects
Coordinates/Vertices	Position graphical elements
Colors	Differentiate objects/states
GLUT Callbacks	Respond to events
Text	Instructions/status/information
Arrays	Store related graphics data
Vertex Arrays	Render appropriate graphical data
Module 2 covers points, line segments, polygons and their attributes, including filled primitives and colors. Module 02 Module 3 adds raster-positioned/bitmap text and interactive callbacks. Module 03 Module 4 introduces vertex arrays as a way to store and efficiently reuse graphical vertex data.
You can refer to the PDF files for reference on what each module tackles.

Throughout this planning process, prompt me suggestions and I'll choose to help build the final idea.