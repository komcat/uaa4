#include <SFML/Graphics.hpp>
#include "MenuSystem.h" // Include the new header file

int main()
{
  // Create window
  sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Menu System");
  window.setFramerateLimit(60);

  // Load font
  sf::Font font;
  if (!font.loadFromFile("arial.ttf")) {
    // If the font file doesn't exist, try a system font
    if (!font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf")) {
      std::cerr << "Error loading font" << std::endl;
      return -1;
    }
  }

  // Window size vector for convenience
  sf::Vector2f windowSize(window.getSize());

  // Create screens
  MenuScreen menuScreen(font, windowSize);
  RunScreen runScreen(font, windowSize);
  ManualScreen manualScreen(font, windowSize);
  TestScreen testScreen(font, windowSize);

  // Current screen tracker
  Screen activeScreen = Screen::MENU;

  // Main loop
  while (window.isOpen())
  {
    sf::Event event;
    while (window.pollEvent(event))
    {
      if (event.type == sf::Event::Closed)
        window.close();
      else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
      {
        // Handle button clicks
        sf::Vector2f mousePos(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));

        // Determine which screen should be active based on button clicks
        Screen newScreen;

        switch (activeScreen) {
        case Screen::MENU:
          newScreen = menuScreen.handleClick(mousePos);
          break;
        case Screen::RUN:
          newScreen = runScreen.handleClick(mousePos);
          break;
        case Screen::MANUAL:
          newScreen = manualScreen.handleClick(mousePos);
          break;
        case Screen::TEST:
          newScreen = testScreen.handleClick(mousePos);
          break;
        }

        activeScreen = newScreen;
      }
      else if (event.type == sf::Event::Resized)
      {
        // Adjust the viewport when the window is resized
        sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
        window.setView(sf::View(visibleArea));
      }
    }

    // Get current mouse position for hover effects
    sf::Vector2f mousePos(static_cast<float>(sf::Mouse::getPosition(window).x),
      static_cast<float>(sf::Mouse::getPosition(window).y));

    // Update the current screen (for hover effects)
    switch (activeScreen) {
    case Screen::MENU:
      menuScreen.update(mousePos);
      break;
    case Screen::RUN:
      runScreen.update(mousePos);
      break;
    case Screen::MANUAL:
      manualScreen.update(mousePos);
      break;
    case Screen::TEST:
      testScreen.update(mousePos);
      break;
    }

    // Clear the window
    window.clear(sf::Color(30, 30, 30));

    // Draw the current screen
    switch (activeScreen) {
    case Screen::MENU:
      menuScreen.draw(window);
      break;
    case Screen::RUN:
      runScreen.draw(window);
      break;
    case Screen::MANUAL:
      manualScreen.draw(window);
      break;
    case Screen::TEST:
      testScreen.draw(window);
      break;
    }

    // Display what was drawn
    window.display();
  }

  return 0;
}