#include "Game.h"

//Private functions
void Game::initVariables()
{
	this->window = nullptr;

	//Game logic
	this->endGame = false;
	this->points = 0;
	this->health = 20;
	this->enemySpawnTimerMax = 10.f;
	this->enemySpawnTimer = this->enemySpawnTimerMax;
	this->maxEnemies = 3;
	this->mouseHeld = false;
}
void Game::playGame()
{
	// Always poll events and clear/draw
	this->pollEvents();

	// Clear old frame
	this->window->clear();

	// Update only if the game state is PLAYING
	if (gameState == GameState::PLAYING) {

		// Based on points, decide which background to draw
		if (points >= 450) {
			// Use alternate background and music
			backgroundSprite.setTexture(alternateBackgroundTexture2);
			if (alternateMusic2.getStatus() != sf::SoundSource::Status::Playing) {
				alternateMusic.stop();
				alternateMusic2.play();
			}
		}
		else if (points >= 200) {
			// Use default background and music
			backgroundSprite.setTexture(alternateBackgroundTexture);
			if (alternateMusic.getStatus() != sf::SoundSource::Status::Playing) {
				backgroundMusic.stop();
				alternateMusic.play();
			}
		}
		else {
			// Use default background and music
			backgroundSprite.setTexture(backgroundTexture);
			if (backgroundMusic.getStatus() != sf::SoundSource::Status::Playing) {
				alternateMusic.stop();
				backgroundMusic.play();
			}
		}

		// Draw the background
		this->window->draw(this->backgroundSprite);

		if (this->endGame == false)
		{
			this->updateMousePositions();
			this->updateText();
			this->updateEnemies();
		}

		//End game condition
		if (this->health <= 0)
		{
			this->endGame = true;
			this->lastScore = this->points; // Save the score
			alternateMusic.stop();
			alternateMusic2.stop();
			gameState = GameState::MENU;  // set gameState to MENU
		}

		// Draw game objects
		this->renderEnemies(*this->window);
		this->renderText(*this->window);
	}
	else if (gameState == GameState::MENU) {
		// If in MENU state, draw the menu
		for (const auto& text : menu)
		{
			this->window->draw(text);
		}
	}

	// Display frame in window
	this->window->display();
}






void Game::resetGame()
{
	this->window->clear();
	this->endGame = false;
	this->points = 0;
	this->health = 20;
	this->enemySpawnTimerMax = 10.f;
	this->enemySpawnTimer = this->enemySpawnTimerMax;
	this->maxEnemies = 3;
	this->mouseHeld = false;
	this->enemies.clear();
	this->circles.clear();


}

void Game::initWindow()
{
	this->videoMode.height = 600;
	this->videoMode.width = 800;

	this->window = new sf::RenderWindow(this->videoMode, "AimBooster", sf::Style::Titlebar | sf::Style::Close);

	this->window->setFramerateLimit(60);
}

void Game::initFonts()
{
	if (this->font.loadFromFile("ARIAL.ttf"))
	{
		//error handeling
	}
}

void Game::initText()
{
	this->uiText.setFont(this->font);
	this->uiText.setCharacterSize(24);
	this->uiText.setFillColor(sf::Color::White);
	this->uiText.setString("NONE");
}

void Game::initEnemies()
{
	this->enemy.setPosition(10.f, 10.f);
	this->enemy.setSize(sf::Vector2f(100.f, 100.f));
	this->enemy.setFillColor(sf::Color::Cyan);
	this->enemy.setOutlineColor(sf::Color::Black);//outline
	this->enemy.setOutlineThickness(1.f);
}

//Constructors / Destructors
Game::Game()
{
	this->initVariables();
	this->initWindow();
	this->initFonts();
	this->initText();
	this->initEnemies();
	this->healthCircle.setRadius(30.f);//healing enemy
	this->healthCircle.setFillColor(sf::Color::Yellow);
	gameState = GameState::MENU;
	play.setPosition(sf::Vector2f(window->getSize().x / 2 - 50, window->getSize().y / 2 - 25)); // center the button
	play.setSize(sf::Vector2f(100, 50)); // size of the button

	// Load the background music
	if (!backgroundMusic.openFromFile("music_beat.ogg"))
	{
		// handle error, for example:
		std::cout << "Error loading background music\n";
	}
	else
	{
		// Play the background music
		backgroundMusic.setLoop(true);  // if you want the music to loop
		backgroundMusic.play();
	}

	// Load the alternate music
	if (!alternateMusic.openFromFile("Faster_Version.ogg"))
	{
		std::cout << "Error loading alternate music\n";
	}
	else
	{
		alternateMusic.setLoop(true);
	}
	// Load the alternate music2
	if (!alternateMusic2.openFromFile("Lights-_HARDSTYLE_.ogg"))
	{
		std::cout << "Error loading alternate music\n";
	}
	else
	{
		alternateMusic2.setLoop(true);
	}

	// Load the default background image
	if (!backgroundTexture.loadFromFile("yes.JPG"))
	{
		std::cout << "Failed to load background image" << std::endl;
	}
	// Set the sprite's texture
	backgroundSprite.setTexture(backgroundTexture);

	// Load the alternate background image
	if (!alternateBackgroundTexture.loadFromFile("doja_cat.JPG"))
	{
		std::cout << "Failed to load alternate background image" << std::endl;
	}
	// Set the sprite's texture
	alternateBackgroundSprite.setTexture(alternateBackgroundTexture);

	// Load the alternate background image2
	if (!alternateBackgroundTexture2.loadFromFile("sigma.JPG"))
	{
		std::cout << "Failed to load alternate background image" << std::endl;
	}
	// Set the sprite's texture
	alternateBackgroundSprite2.setTexture(alternateBackgroundTexture2);

	this->mainLoop();
}


Game::~Game()
{
	delete this->window;
}

//Accessors
const bool Game::running() const
{
	return this->window->isOpen();
}

const bool Game::getEndGame() const
{
	return this->endGame;
}

//Functions
void Game::spawnEnemy()
{
	//Randomize enemy type
	int type = rand() % 5;

	switch (type)
	{
	case 0:
		this->enemy.setSize(sf::Vector2f(20.f, 20.f));
		this->enemy.setFillColor(sf::Color::Magenta);
		break;
	case 1:
		this->enemy.setSize(sf::Vector2f(30.f, 30.f));
		this->enemy.setFillColor(sf::Color::Blue);
		break;
	case 2:
		this->enemy.setSize(sf::Vector2f(50.f, 50.f));
		this->enemy.setFillColor(sf::Color::Cyan);
		break;
	case 3:
		this->enemy.setSize(sf::Vector2f(70.f, 70.f));
		this->enemy.setFillColor(sf::Color::Red);
		break;
	case 4:
		this->enemy.setSize(sf::Vector2f(100.f, 100.f));
		this->enemy.setFillColor(sf::Color::Green);
		break;
	default:
		this->enemy.setSize(sf::Vector2f(100.f, 100.f));
		this->enemy.setFillColor(sf::Color::Yellow);
		break;
	}

	// Determine spawn position based on current score
	float spawnY = 0.f; // Default spawn at the top
	if (this->points >= 200 && this->points < 450) {
		spawnY = static_cast<float>(this->window->getSize().y - this->enemy.getSize().y); // Spawn at the bottom
	}
	else if (this->points >= 450) {
		spawnY = 0.f; // Back to top
	}

	this->enemy.setPosition(
		static_cast<float>(rand() % static_cast<int>(this->window->getSize().x - this->enemy.getSize().x)),
		spawnY
	);

	// Randomly decide whether to spawn a health circle
	bool spawnHealthCircle = rand() % 10 == 0;  // 10% chance
	if (spawnHealthCircle)
	{
		// Determine spawn position for health circle based on current score
		float circleSpawnY = 0.f; // Default spawn at the top
		if (this->points >= 200 && this->points < 450) {
			circleSpawnY = static_cast<float>(this->window->getSize().y - this->healthCircle.getRadius() * 2); // Spawn at the bottom
		}
		else if (this->points >= 450) {
			circleSpawnY = 0.f; // Back to top
		}

		this->healthCircle.setPosition(
			static_cast<float>(rand() % static_cast<int>(this->window->getSize().x - this->healthCircle.getRadius() * 2)),
			circleSpawnY
		);

		// Add to enemy vector
		this->circles.push_back(this->healthCircle);
	}

	//Spawn the enemy
	this->enemies.push_back(this->enemy);
}



void Game::pollEvents()
{
	while (this->window->pollEvent(this->ev))
	{
		switch (this->ev.type)
		{
		case sf::Event::Closed:
			this->window->close();
			break;
		case sf::Event::KeyPressed:
			if (this->ev.key.code == sf::Keyboard::Escape)
				this->window->close();
			break;
		case sf::Event::MouseButtonPressed:
			if (this->ev.mouseButton.button == sf::Mouse::Left)
			{
				sf::Vector2i mousePos = sf::Mouse::getPosition(*this->window);
				if (this->play.getGlobalBounds().contains(window->mapPixelToCoords(mousePos)))
				{
					this->gameState = GameState::PLAYING;
				}
			}
			break;

		}
	}
}

void Game::updateMousePositions()
{
	/**
		@ return void
		Updates the mouse positions:
		- Mouse position relative to window (Vector2i)
	*/

	this->mousePosWindow = sf::Mouse::getPosition(*this->window);
	this->mousePosView = this->window->mapPixelToCoords(this->mousePosWindow);
}

void Game::updateText()
{
	std::stringstream ss;

	ss << "Points: " << this->points << "\n"
		<< "Health: " << this->health << "\n";

	this->uiText.setString(ss.str());

	if (this->health <= 10)
	{
		this->uiText.setFillColor(sf::Color::Red);
	}
	else
	{
		this->uiText.setFillColor(sf::Color::White);  
	}
}

void Game::updateEnemies()
{
	// Updating the number of enemies that can spawn based on points
	if (this->points >= 1000) {
		this->maxEnemies = 20;
	}
	else if (this->points >= 500) {
		this->maxEnemies = 10;
	}
	else if (this->points >= 250) {
		this->maxEnemies = 8;
	}
	else if (this->points >= 150) {
		this->maxEnemies = 5;
	}

	// Updating the timer for enemy spawning
	if (this->enemies.size() < this->maxEnemies)
	{
		if (this->enemySpawnTimer >= this->enemySpawnTimerMax)
		{
			// Spawn the enemy and reset the timer
			this->spawnEnemy();
			this->enemySpawnTimer = 0.f;
		}
		else
			this->enemySpawnTimer += 1.f;
	}

	// Determine enemy speed based on current score
	float enemySpeed = 4.f;  // default speed
	if (this->points >= 1000) {
		enemySpeed = 10.f;
	}
	else if (this->points >= 500) {
		enemySpeed = 8.f;
	}
	else if (this->points >= 200 && this->points < 450) {
		enemySpeed = -5.f;  // Negative speed moves the enemies upwards
	}
	else if (this->points >= 450) {
		enemySpeed = 6.f;  // Switch back to positive speed
	}

	// Moving and updating enemies
	for (int i = 0; i < this->enemies.size(); i++)
	{
		this->enemies[i].move(0.f, enemySpeed);

		// Check if the enemies have moved off the screen
		if (this->enemies[i].getPosition().y > this->window->getSize().y && enemySpeed > 0)
		{
			this->enemies.erase(this->enemies.begin() + i);
			this->health -= 1;
			std::cout << "Health: " << this->health << "\n";
		}
		else if (this->enemies[i].getPosition().y < 0 && enemySpeed < 0)
		{
			this->enemies.erase(this->enemies.begin() + i);
			this->health -= 1;
			std::cout << "Health: " << this->health << "\n";
		}
	}

	// Moving and updating circles
	for (int i = 0; i < this->circles.size(); i++)
	{
		this->circles[i].move(0.f, enemySpeed);

		// Check if the circles have moved off the screen
		if (this->circles[i].getPosition().y > this->window->getSize().y && enemySpeed > 0)
		{
			this->circles.erase(this->circles.begin() + i);
			this->health -= 3;
			std::cout << "Health: " << this->health << "\n";
		}
		else if (this->circles[i].getPosition().y < 0 && enemySpeed < 0)
		{
			this->circles.erase(this->circles.begin() + i);
			this->health -= 3;
			std::cout << "Health: " << this->health << "\n";
		}
	}

	// Check if clicked upon
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		if (this->mouseHeld == false)
		{
			this->mouseHeld = true;
			bool deleted = false;
			for (size_t i = 0; i < this->enemies.size() && deleted == false; i++)
			{
				if (this->enemies[i].getGlobalBounds().contains(this->mousePosView))
				{
					//Gain points
					if (this->enemies[i].getFillColor() == sf::Color::Magenta)
						this->points += 8;
					else if (this->enemies[i].getFillColor() == sf::Color::Blue)
						this->points += 5;
					else if (this->enemies[i].getFillColor() == sf::Color::Cyan)
						this->points += 4;
					else if (this->enemies[i].getFillColor() == sf::Color::Red)
						this->points += 2;
					else if (this->enemies[i].getFillColor() == sf::Color::Green)
						this->points += 1;

					std::cout << "Points: " << this->points << "\n";

					// Delete the enemy
					deleted = true;
					this->enemies.erase(this->enemies.begin() + i);
				}
			}

			// Check if clicked on circle
			for (size_t i = 0; i < this->circles.size() && deleted == false; i++)
			{
				if (this->circles[i].getGlobalBounds().contains(this->mousePosView))
				{
					// Gain health
					this->health += 5;
					std::cout << "Health: " << this->health << "\n";

					// Delete the circle
					deleted = true;
					this->circles.erase(this->circles.begin() + i);
				}
			}
		}
	}
	
	else
	{
		this->mouseHeld = false;
	}

}

void Game::update()
{
	this->pollEvents();

	if (this->endGame == false)
	{
		this->updateMousePositions();

		this->updateText();

		this->updateEnemies();
	}

	//End game condition
	if (this->health <= 0)
		this->endGame = true;
}

void Game::renderText(sf::RenderTarget& target)
{
	target.draw(this->uiText);
}

void Game::renderEnemies(sf::RenderTarget& target)
{
	//Rendering all the enemies
	for (auto& e : this->enemies)
	{
		target.draw(e);
	}
	// draw circles
	for (auto& c : this->circles)
	{
		target.draw(c);
	}
}

void Game::render()
{
	/**
		@return void
		- clear old frame
		- render objects
		- display frame in window
		Renders the game objects.
	*/

	this->window->clear();

	// Draw background only when game state is PLAYING
	if (this->gameState == GameState::PLAYING) {
		this->window->draw(this->backgroundSprite);
	}

	//Draw game objects
	this->renderEnemies(*this->window);

	this->renderText(*this->window);

	this->window->display();
}


void Game::showMenu()
{
	if (!font.loadFromFile("ARIAL.ttf"))
	{
		// handle error...
	}

	std::vector<sf::Text> menu;
	std::vector<sf::RectangleShape> menuBorders;  // Create the borders vector

	sf::Text play;
	play.setFont(font);
	play.setString("Play");
	play.setCharacterSize(50);
	play.setFillColor(sf::Color::White);
	sf::FloatRect playBounds = play.getLocalBounds();
	play.setOrigin(playBounds.left + playBounds.width / 2.0f,
		playBounds.top + playBounds.height / 2.0f);
	play.setPosition(sf::Vector2f(this->window->getSize().x / 2, this->window->getSize().y / 2 - 50));

	sf::RectangleShape playBorder(sf::Vector2f(playBounds.width + 10, playBounds.height + 10));
	playBorder.setFillColor(sf::Color::Transparent);
	playBorder.setOutlineThickness(5);
	playBorder.setOutlineColor(sf::Color::Red);
	playBorder.setOrigin(playBounds.left + playBounds.width / 2.0f, playBounds.top + playBounds.height / 2.0f);
	playBorder.setPosition(play.getPosition());

	menu.push_back(play);
	menuBorders.push_back(playBorder);

	sf::Text quit;
	quit.setFont(font);
	quit.setString("Quit");
	quit.setCharacterSize(50);
	quit.setFillColor(sf::Color::White);
	sf::FloatRect quitBounds = quit.getLocalBounds();
	quit.setOrigin(quitBounds.left + quitBounds.width / 2.0f,
		quitBounds.top + quitBounds.height / 2.0f);
	quit.setPosition(sf::Vector2f(this->window->getSize().x / 2, this->window->getSize().y / 2 + 50));

	sf::RectangleShape quitBorder(sf::Vector2f(quitBounds.width + 10, quitBounds.height + 10));
	quitBorder.setFillColor(sf::Color::Transparent);
	quitBorder.setOutlineThickness(5);
	quitBorder.setOutlineColor(sf::Color::Red);
	quitBorder.setOrigin(quitBounds.left + quitBounds.width / 2.0f, quitBounds.top + quitBounds.height / 2.0f);
	quitBorder.setPosition(quit.getPosition());

	menu.push_back(quit);
	menuBorders.push_back(quitBorder);

	// Set health text and border
	sf::Text setHealth;
	setHealth.setFont(font);
	setHealth.setString("Set Starting Health: " + std::to_string(startingHealth));
	setHealth.setCharacterSize(30);
	setHealth.setFillColor(sf::Color::White);
	sf::FloatRect healthBounds = setHealth.getLocalBounds();
	setHealth.setOrigin(healthBounds.left + healthBounds.width / 2.0f,
		healthBounds.top + healthBounds.height / 2.0f);
	setHealth.setPosition(sf::Vector2f(this->window->getSize().x / 2, this->window->getSize().y / 2 + 150));

	sf::RectangleShape healthBorder(sf::Vector2f(healthBounds.width + 10, healthBounds.height + 10));
	healthBorder.setFillColor(sf::Color::Transparent);
	healthBorder.setOutlineThickness(5);
	healthBorder.setOutlineColor(sf::Color::Red);
	healthBorder.setOrigin(healthBounds.left + healthBounds.width / 2.0f, healthBounds.top + healthBounds.height / 2.0f);
	healthBorder.setPosition(setHealth.getPosition());

	menu.push_back(setHealth);
	menuBorders.push_back(healthBorder);

	sf::Text scoreDisplay;
	scoreDisplay.setFont(font);
	scoreDisplay.setString("Last Score: " + std::to_string(this->lastScore));
	scoreDisplay.setCharacterSize(30);
	sf::FloatRect scoreBounds = scoreDisplay.getLocalBounds();
	scoreDisplay.setOrigin(scoreBounds.left + scoreBounds.width / 2.0f,
		scoreBounds.top + scoreBounds.height / 2.0f);
	scoreDisplay.setPosition(sf::Vector2f(this->window->getSize().x / 2, this->window->getSize().y / 2 + 200));
	scoreDisplay.setFillColor(sf::Color::White);

	menu.push_back(scoreDisplay);

	sf::Event event;
	sf::Vector2i mousePos;

	// Wait for an event
	if (window->pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Closed:
			window->close();
			break;

		case sf::Event::MouseButtonReleased:
			mousePos = sf::Mouse::getPosition(*window);
			for (int i = 0; i < menu.size(); ++i)
			{
				sf::FloatRect textRect = menu[i].getGlobalBounds();

				if (textRect.contains(static_cast<sf::Vector2f>(mousePos)))
				{
					switch (i)
					{
					case 0: // Play button
						std::cout << "Play button has been pressed" << std::endl;
						this->resetGame(); // Reset the game state
						this->health = this->startingHealth; // Set the health to the chosen value
						gameState = GameState::PLAYING;
						break;
					case 1: // Quit button
						std::cout << "Quit button has been pressed" << std::endl;
						window->close();
						break;
					case 2: // Set Health button
						std::cout << "Set health button has been pressed" << std::endl;
						backgroundMusic.stop();
						this->changeStartingHealth();
						menu[2].setString("Set Starting Health: " + std::to_string(startingHealth));
						break;
					default:
						break;
					}
				}
			}
			break;

		default:
			break;
		}
	}

	// Draw the menu
	window->clear();
	for (const auto& text : menu)
	{
		window->draw(text);
	}
	for (const auto& border : menuBorders)  // Draw the borders
	{
		window->draw(border);
	}
	window->display();
}

void Game::changeStartingHealth()
{
	std::string input;
	std::cout << "Enter your desired starting health: ";
	std::getline(std::cin, input);
	int newHealth = std::stoi(input);
	if (newHealth >= 1 && newHealth <= 100)  // validate input
	{
		this->startingHealth = newHealth;
	}
	else
	{
		std::cout << "Invalid health value! Please enter a number between 1 and 100.\n";
	}
}



void Game::mainLoop() {
	while (window->isOpen()) {
		switch (gameState) {
		case GameState::MENU:
			showMenu();
			backgroundMusic.stop();
			break;
		case GameState::PLAYING:
			playGame();
			break;
		case GameState::END:
			// end game code here
			break;
		}
	}
}


