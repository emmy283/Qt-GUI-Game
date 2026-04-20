# Qt-GUI-Game
Dodge the Falling Enemies - Qt Interactive Game
An interactive, survival-style 2D game built using the C++ Qt Framework. The project demonstrates the implementation of a real-time game loop, collision detection, and dynamic graphical rendering.

🚀 Features
Dynamic Gameplay: Control a player character to dodge circular enemies falling from the top of the screen at varying speeds.

Real-time Score Tracking: Points are awarded for every enemy successfully dodged, with a live display in the UI.

Game State Management: Includes a "Game Over" state upon collision and a quick-restart functionality using the 'R' key.

Custom Graphics: Utilizes QPainter for high-quality rendering, including linear gradients for the background and game entities.

🛠 Technical Implementation
Event Handling: Custom implementation of keyPressEvent to handle smooth player movement (Left/Right arrows) and game resets.

Game Loop: Managed via QTimer, triggering frequent update() calls to handle movement logic and collision detection.

Collision Detection: Uses QRect::intersects logic to determine spatial overlap between the player and enemy objects.

Object Management: Dynamically manages a collection of enemy objects, including spawning new entities and removing those that have moved off-screen to optimize memory usage.

💻 Technologies Used
Language: C++

Framework: Qt (GUI Module)

Tools: QPainter, QTimer, QKeyEvent

🎮 How to Play
Move: Use the Left and Right arrow keys to move the player.

Objective: Avoid the falling circles to keep your score increasing.

Restart: If you hit an enemy, press 'R' to reset the game and try again.
