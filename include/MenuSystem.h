#ifndef MENU_SYSTEM_H
#define MENU_SYSTEM_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <iostream>

// Enum to track which screen we're on
enum class Screen {
  MENU,
  RUN,
  MANUAL,
  TEST
};

// Button class for SFML
class Button {
private:
  sf::RectangleShape shape;
  sf::Text text;
  bool isHovered;

public:
  Button(const sf::Font& font, const std::string& buttonText, const sf::Vector2f& position, const sf::Vector2f& size) {
    // Set up the button shape
    shape.setSize(size);
    shape.setPosition(position);
    shape.setFillColor(sf::Color(50, 150, 50));
    shape.setOutlineThickness(2);
    shape.setOutlineColor(sf::Color(20, 100, 20));

    // Set up the text
    text.setFont(font);
    text.setString(buttonText);
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::White);

    // Center text on button
    sf::FloatRect textBounds = text.getLocalBounds();
    text.setPosition(
      position.x + (size.x - textBounds.width) / 2.0f,
      position.y + (size.y - textBounds.height) / 2.0f - textBounds.top
    );

    isHovered = false;
  }

  // Check if the mouse is over the button
  bool isMouseOver(const sf::Vector2f& mousePos) const {
    return shape.getGlobalBounds().contains(mousePos);
  }

  // Update button appearance when hovered
  void update(const sf::Vector2f& mousePos) {
    bool hover = isMouseOver(mousePos);

    if (hover && !isHovered) {
      shape.setFillColor(sf::Color(70, 170, 70));
      isHovered = true;
    }
    else if (!hover && isHovered) {
      shape.setFillColor(sf::Color(50, 150, 50));
      isHovered = false;
    }
  }

  // Check if button is clicked
  bool isClicked(const sf::Vector2f& mousePos) const {
    return isMouseOver(mousePos);
  }

  // Draw the button
  void draw(sf::RenderWindow& window) const {
    window.draw(shape);
    window.draw(text);
  }
};

// Base Screen class
class BaseScreen {
protected:
  std::vector<Button> buttons;
  sf::Font font;
  sf::Text headerText;

public:
  BaseScreen(const sf::Font& font) : font(font) {}

  virtual void update(const sf::Vector2f& mousePos) {
    for (auto& button : buttons) {
      button.update(mousePos);
    }
  }

  virtual void draw(sf::RenderWindow& window) {
    for (const auto& button : buttons) {
      button.draw(window);
    }
    window.draw(headerText);
  }

  virtual Screen handleClick(const sf::Vector2f& mousePos) = 0;
};

// Menu Screen
class MenuScreen : public BaseScreen {
public:
  MenuScreen(const sf::Font& font, const sf::Vector2f& windowSize) : BaseScreen(font) {
    // Set up header text
    headerText.setFont(font);
    headerText.setString("MAIN MENU");
    headerText.setCharacterSize(36);
    headerText.setFillColor(sf::Color::White);

    // Center the header text
    sf::FloatRect textBounds = headerText.getLocalBounds();
    headerText.setPosition(
      (windowSize.x - textBounds.width) / 2.0f,
      50.0f
    );

    // Create menu buttons
    float buttonWidth = 200.0f;
    float buttonHeight = 50.0f;
    float startY = 150.0f;
    float spacing = 70.0f;

    // Add Run button
    buttons.emplace_back(font, "Run",
      sf::Vector2f((windowSize.x - buttonWidth) / 2.0f, startY),
      sf::Vector2f(buttonWidth, buttonHeight));

    // Add Manual button
    buttons.emplace_back(font, "Manual",
      sf::Vector2f((windowSize.x - buttonWidth) / 2.0f, startY + spacing),
      sf::Vector2f(buttonWidth, buttonHeight));

    // Add Test button
    buttons.emplace_back(font, "Test",
      sf::Vector2f((windowSize.x - buttonWidth) / 2.0f, startY + spacing * 2),
      sf::Vector2f(buttonWidth, buttonHeight));

    // Add Exit button
    buttons.emplace_back(font, "Exit",
      sf::Vector2f((windowSize.x - buttonWidth) / 2.0f, startY + spacing * 3),
      sf::Vector2f(buttonWidth, buttonHeight));
  }

  Screen handleClick(const sf::Vector2f& mousePos) override {
    if (buttons[0].isClicked(mousePos)) {
      return Screen::RUN;
    }
    else if (buttons[1].isClicked(mousePos)) {
      return Screen::MANUAL;
    }
    else if (buttons[2].isClicked(mousePos)) {
      return Screen::TEST;
    }
    else if (buttons[3].isClicked(mousePos)) {
      // Exit button clicked
      std::exit(0);
    }

    return Screen::MENU;
  }
};

// Content Screen base class (for Run, Manual, Test screens)
class ContentScreen : public BaseScreen {
protected:
  sf::Text contentText;

public:
  ContentScreen(const sf::Font& font, const std::string& title, const std::string& content, const sf::Vector2f& windowSize)
    : BaseScreen(font) {
    // Set up header text
    headerText.setFont(font);
    headerText.setString(title);
    headerText.setCharacterSize(36);
    headerText.setFillColor(sf::Color::White);

    // Position header text (centered horizontally)
    sf::FloatRect textBounds = headerText.getLocalBounds();
    headerText.setPosition(
      (windowSize.x - textBounds.width) / 2.0f,
      50.0f
    );

    // Set up content text
    contentText.setFont(font);
    contentText.setString(content);
    contentText.setCharacterSize(24);
    contentText.setFillColor(sf::Color::White);
    contentText.setPosition(50.0f, 150.0f);

    // Back button
    buttons.emplace_back(font, "Back", sf::Vector2f(20.0f, 20.0f), sf::Vector2f(100.0f, 40.0f));
  }

  void draw(sf::RenderWindow& window) override {
    BaseScreen::draw(window);
    window.draw(contentText);
  }

  Screen handleClick(const sf::Vector2f& mousePos) override {
    if (buttons[0].isClicked(mousePos)) {
      return Screen::MENU;
    }
    return Screen::MENU; // Fallback
  }
};

// Run Screen
class RunScreen : public ContentScreen {
public:
  RunScreen(const sf::Font& font, const sf::Vector2f& windowSize)
    : ContentScreen(font, "RUN SCREEN",
      "This is the Run screen.\n\nHere you can start your application or simulation.",
      windowSize) {
  }
};

// Manual Screen
class ManualScreen : public ContentScreen {
public:
  ManualScreen(const sf::Font& font, const sf::Vector2f& windowSize)
    : ContentScreen(font, "MANUAL SCREEN",
      "This is the Manual screen.\n\nHere you can add instructions or documentation for your application.",
      windowSize) {
  }
};

// Test Screen
class TestScreen : public ContentScreen {
public:
  TestScreen(const sf::Font& font, const sf::Vector2f& windowSize)
    : ContentScreen(font, "TEST SCREEN",
      "This is the Test screen.\n\nHere you can add test features or debugging tools for your application.",
      windowSize) {
  }
};

#endif // MENU_SYSTEM_H