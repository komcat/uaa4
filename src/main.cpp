#include <SFML/Graphics.hpp>
#include <iostream>
#include "MenuSystem.h"

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
  sf::Vector2f windowSize(static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y));

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
      else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
      {
        // Handle mouse release for sliders
        if (activeScreen == Screen::TEST) {
          testScreen.handleMouseRelease();
        }
      }
      else if (event.type == sf::Event::MouseMoved)
      {
        // Update UI hover effects when the mouse moves
        sf::Vector2f mousePos(static_cast<float>(event.mouseMove.x), static_cast<float>(event.mouseMove.y));

        // Update the current screen for hover effects
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

          // Specifically handle slider dragging in the test screen
          if (activeScreen == Screen::TEST) {
            // If we're dragging a slider, update its position with the mouse
            sf::Vector2f mousePos(static_cast<float>(event.mouseMove.x), static_cast<float>(event.mouseMove.y));
            testScreen.handleMouseDrag(mousePos);
          }
          break;
        }
      }
      else if (event.type == sf::Event::Resized)
      {
        // Adjust the viewport when the window is resized
        sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
        window.setView(sf::View(visibleArea));

        // You might want to update your UI components' positions here if needed
        // For example: menuScreen.updatePositions(sf::Vector2f(event.size.width, event.size.height));
      }
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