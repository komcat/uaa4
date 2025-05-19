#ifndef MENU_SYSTEM_H
#define MENU_SYSTEM_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <iostream>
#include <functional>

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
  Button(const sf::Font& font, const std::string& buttonText, const sf::Vector2f& position, const sf::Vector2f& size);
  bool isMouseOver(const sf::Vector2f& mousePos) const;
  void update(const sf::Vector2f& mousePos);
  bool isClicked(const sf::Vector2f& mousePos) const;
  void draw(sf::RenderWindow& window) const;
};

// Checkbox component
class Checkbox {
private:
  sf::RectangleShape box;
  sf::RectangleShape checkmark;
  sf::Text label;
  bool checked;
  bool isHovered;
  std::function<void(bool)> onChange;

public:
  Checkbox(const sf::Font& font, const std::string& labelText, const sf::Vector2f& position, bool initialState = false);
  void setOnChange(std::function<void(bool)> callback);
  bool isChecked() const;
  void setChecked(bool state);
  bool isMouseOver(const sf::Vector2f& mousePos) const;
  void update(const sf::Vector2f& mousePos);
  void toggle();
  bool handleClick(const sf::Vector2f& mousePos);
  void draw(sf::RenderWindow& window) const;
};

// Slider component
class Slider {
private:
  sf::RectangleShape track;
  sf::RectangleShape handle;
  sf::Text label;
  sf::Text valueText;
  float minValue;
  float maxValue;
  float currentValue;
  bool isDragging;
  std::function<void(float)> onChange;

public:
  Slider(const sf::Font& font, const std::string& labelText, const sf::Vector2f& position,
    float width, float minVal, float maxVal, float initialValue);
  void setOnChange(std::function<void(float)> callback);
  float getValue() const;
  void setValue(float value);
  void updateHandlePosition();
  void updateValueText();
  bool isHandleMouseOver(const sf::Vector2f& mousePos) const;
  bool isTrackMouseOver(const sf::Vector2f& mousePos) const;
  void startDrag();
  void stopDrag();
  void updateFromMousePosition(const sf::Vector2f& mousePos);
  bool handleClick(const sf::Vector2f& mousePos);
  void handleRelease();
  void draw(sf::RenderWindow& window) const;
};


// Dropdown component
class Dropdown {
private:
  sf::RectangleShape mainBox;
  sf::Text selectedText;
  sf::RectangleShape dropdownList;
  std::vector<sf::Text> options;
  std::vector<sf::RectangleShape> optionBackgrounds;
  bool isOpen;
  bool isHovered;
  int selectedIndex;
  std::function<void(int, const std::string&)> onChange;

public:
  Dropdown(const sf::Font& font, const std::vector<std::string>& items,
    const sf::Vector2f& position, float width);
  void setOnChange(std::function<void(int, const std::string&)> callback);
  int getSelectedIndex() const;
  std::string getSelectedText() const;
  void setSelectedIndex(int index);
  bool isMainBoxMouseOver(const sf::Vector2f& mousePos) const;
  bool isDropdownListMouseOver(const sf::Vector2f& mousePos) const;
  int getOptionIndexAtPosition(const sf::Vector2f& mousePos) const;
  void update(const sf::Vector2f& mousePos);
  void toggle();
  bool handleClick(const sf::Vector2f& mousePos);
  void draw(sf::RenderWindow& window) const;
};

// Base Screen class
class BaseScreen {
protected:
  std::vector<Button> buttons;
  sf::Font font;
  sf::Text headerText;

public:
  BaseScreen(const sf::Font& font);
  virtual void update(const sf::Vector2f& mousePos);
  virtual void draw(sf::RenderWindow& window);
  virtual Screen handleClick(const sf::Vector2f& mousePos) = 0;
};

// Menu Screen
class MenuScreen : public BaseScreen {
public:
  MenuScreen(const sf::Font& font, const sf::Vector2f& windowSize);
  Screen handleClick(const sf::Vector2f& mousePos) override;
};

// Content Screen base class (for Run, Manual, Test screens)
class ContentScreen : public BaseScreen {
protected:
  sf::Text contentText;

public:
  ContentScreen(const sf::Font& font, const std::string& title, const std::string& content, const sf::Vector2f& windowSize);
  void draw(sf::RenderWindow& window) override;
  Screen handleClick(const sf::Vector2f& mousePos) override;
};

// Run Screen
class RunScreen : public ContentScreen {
public:
  RunScreen(const sf::Font& font, const sf::Vector2f& windowSize);
};

// Manual Screen
class ManualScreen : public ContentScreen {
public:
  ManualScreen(const sf::Font& font, const sf::Vector2f& windowSize);
};

// Test Screen
class TestScreen : public ContentScreen {
private:
  Checkbox checkbox1;
  Checkbox checkbox2;
  Slider slider;
  Dropdown dropdown;

public:
  TestScreen(const sf::Font& font, const sf::Vector2f& windowSize);
  void update(const sf::Vector2f& mousePos) override;
  void draw(sf::RenderWindow& window) override;
  Screen handleClick(const sf::Vector2f& mousePos) override;
  void handleMouseRelease();
  void handleMouseDrag(const sf::Vector2f& mousePos);
};

#endif // MENU_SYSTEM_H