#include "MenuSystem.h"

// Button implementation
Button::Button(const sf::Font& font, const std::string& buttonText, const sf::Vector2f& position, const sf::Vector2f& size) {
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

bool Button::isMouseOver(const sf::Vector2f& mousePos) const {
  return shape.getGlobalBounds().contains(mousePos);
}

void Button::update(const sf::Vector2f& mousePos) {
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

bool Button::isClicked(const sf::Vector2f& mousePos) const {
  return isMouseOver(mousePos);
}

void Button::draw(sf::RenderWindow& window) const {
  window.draw(shape);
  window.draw(text);
}

// Checkbox implementation
Checkbox::Checkbox(const sf::Font& font, const std::string& labelText, const sf::Vector2f& position, bool initialState) {
  // Set up the checkbox box
  float size = 20.0f;
  box.setSize(sf::Vector2f(size, size));
  box.setPosition(position);
  box.setFillColor(sf::Color(50, 50, 50));
  box.setOutlineThickness(2);
  box.setOutlineColor(sf::Color(120, 120, 120));

  // Set up the checkmark
  float margin = 4.0f;
  checkmark.setSize(sf::Vector2f(size - 2 * margin, size - 2 * margin));
  checkmark.setPosition(position.x + margin, position.y + margin);
  checkmark.setFillColor(sf::Color(20, 200, 20));

  // Set up the label
  label.setFont(font);
  label.setString(labelText);
  label.setCharacterSize(18);
  label.setFillColor(sf::Color::White);
  label.setPosition(position.x + size + 10.0f, position.y);

  checked = initialState;
  isHovered = false;
  onChange = nullptr;
}

void Checkbox::setOnChange(std::function<void(bool)> callback) {
  onChange = callback;
}

bool Checkbox::isChecked() const {
  return checked;
}

void Checkbox::setChecked(bool state) {
  checked = state;
  if (onChange) {
    onChange(checked);
  }
}

bool Checkbox::isMouseOver(const sf::Vector2f& mousePos) const {
  return box.getGlobalBounds().contains(mousePos);
}

void Checkbox::update(const sf::Vector2f& mousePos) {
  bool hover = isMouseOver(mousePos);

  if (hover && !isHovered) {
    box.setOutlineColor(sf::Color(180, 180, 180));
    isHovered = true;
  }
  else if (!hover && isHovered) {
    box.setOutlineColor(sf::Color(120, 120, 120));
    isHovered = false;
  }
}

void Checkbox::toggle() {
  checked = !checked;
  if (onChange) {
    onChange(checked);
  }
}

bool Checkbox::handleClick(const sf::Vector2f& mousePos) {
  if (isMouseOver(mousePos)) {
    toggle();
    return true;
  }
  return false;
}

void Checkbox::draw(sf::RenderWindow& window) const {
  window.draw(box);
  if (checked) {
    window.draw(checkmark);
  }
  window.draw(label);
}

// Slider implementation
Slider::Slider(const sf::Font& font, const std::string& labelText, const sf::Vector2f& position,
  float width, float minVal, float maxVal, float initialValue) {
  // Set up the track
  track.setSize(sf::Vector2f(width, 6.0f));
  track.setPosition(position);
  track.setFillColor(sf::Color(80, 80, 80));
  track.setOutlineThickness(1);
  track.setOutlineColor(sf::Color(120, 120, 120));

  // Set up the handle
  float handleSize = 16.0f;
  handle.setSize(sf::Vector2f(handleSize, handleSize));
  handle.setFillColor(sf::Color(50, 150, 50));
  handle.setOutlineThickness(2);
  handle.setOutlineColor(sf::Color(20, 100, 20));

  // Set up the label
  label.setFont(font);
  label.setString(labelText);
  label.setCharacterSize(18);
  label.setFillColor(sf::Color::White);
  label.setPosition(position.x, position.y - 25.0f);

  // Set up value text
  valueText.setFont(font);
  valueText.setCharacterSize(16);
  valueText.setFillColor(sf::Color::White);
  valueText.setPosition(position.x + width + 10.0f, position.y - 5.0f);

  minValue = minVal;
  maxValue = maxVal;
  currentValue = initialValue;
  isDragging = false;
  onChange = nullptr;

  // Initialize handle position
  updateHandlePosition();
  updateValueText();
}

void Slider::setOnChange(std::function<void(float)> callback) {
  onChange = callback;
}

float Slider::getValue() const {
  return currentValue;
}

void Slider::setValue(float value) {
  currentValue = std::max(minValue, std::min(maxValue, value));
  updateHandlePosition();
  updateValueText();
  if (onChange) {
    onChange(currentValue);
  }
}

void Slider::updateHandlePosition() {
  float trackWidth = track.getSize().x;
  float valueRatio = (currentValue - minValue) / (maxValue - minValue);
  float handleX = track.getPosition().x + trackWidth * valueRatio - handle.getSize().x / 2.0f;
  handle.setPosition(handleX, track.getPosition().y - handle.getSize().y / 2.0f + track.getSize().y / 2.0f);
}

void Slider::updateValueText() {
  valueText.setString(std::to_string(static_cast<int>(currentValue)));
}

bool Slider::isHandleMouseOver(const sf::Vector2f& mousePos) const {
  return handle.getGlobalBounds().contains(mousePos);
}

bool Slider::isTrackMouseOver(const sf::Vector2f& mousePos) const {
  return track.getGlobalBounds().contains(mousePos);
}

void Slider::startDrag() {
  isDragging = true;
}

void Slider::stopDrag() {
  isDragging = false;
}

void Slider::updateFromMousePosition(const sf::Vector2f& mousePos) {
  if (isDragging) {
    float trackWidth = track.getSize().x;
    float trackStart = track.getPosition().x;
    float trackEnd = trackStart + trackWidth;

    float mouseX = std::max(trackStart, std::min(trackEnd, mousePos.x));
    float valueRatio = (mouseX - trackStart) / trackWidth;

    float newValue = minValue + valueRatio * (maxValue - minValue);
    setValue(newValue);
  }
}

bool Slider::handleClick(const sf::Vector2f& mousePos) {
  if (isHandleMouseOver(mousePos) || isTrackMouseOver(mousePos)) {
    startDrag();
    updateFromMousePosition(mousePos);
    return true;
  }
  return false;
}

void Slider::handleRelease() {
  stopDrag();
}

void Slider::draw(sf::RenderWindow& window) const {
  window.draw(track);
  window.draw(handle);
  window.draw(label);
  window.draw(valueText);
}

// Dropdown implementation
Dropdown::Dropdown(const sf::Font& font, const std::vector<std::string>& items,
  const sf::Vector2f& position, float width) {
  // Set up the main box
  float height = 30.0f;
  mainBox.setSize(sf::Vector2f(width, height));
  mainBox.setPosition(position);
  mainBox.setFillColor(sf::Color(50, 50, 50));
  mainBox.setOutlineThickness(2);
  mainBox.setOutlineColor(sf::Color(120, 120, 120));

  // Set up the selected text
  selectedText.setFont(font);
  selectedText.setCharacterSize(18);
  selectedText.setFillColor(sf::Color::White);
  selectedText.setPosition(position.x + 10.0f, position.y + 5.0f);

  // Set up dropdown list (initially hidden)
  float dropdownHeight = items.size() * height;
  dropdownList.setSize(sf::Vector2f(width, dropdownHeight));
  dropdownList.setPosition(position.x, position.y + height);
  dropdownList.setFillColor(sf::Color(40, 40, 40));
  dropdownList.setOutlineThickness(2);
  dropdownList.setOutlineColor(sf::Color(120, 120, 120));

  // Set up options
  for (size_t i = 0; i < items.size(); i++) {
    sf::Text text;
    text.setFont(font);
    text.setString(items[i]);
    text.setCharacterSize(18);
    text.setFillColor(sf::Color::White);
    text.setPosition(position.x + 10.0f, position.y + height + i * height + 5.0f);
    options.push_back(text);

    sf::RectangleShape bg;
    bg.setSize(sf::Vector2f(width, height));
    bg.setPosition(position.x, position.y + height + i * height);
    bg.setFillColor(sf::Color(40, 40, 40));
    optionBackgrounds.push_back(bg);
  }

  isOpen = false;
  isHovered = false;
  selectedIndex = -1;
  onChange = nullptr;

  // If there are items, select the first one
  if (!items.empty()) {
    setSelectedIndex(0);
  }
}

void Dropdown::setOnChange(std::function<void(int, const std::string&)> callback) {
  onChange = callback;
}

int Dropdown::getSelectedIndex() const {
  return selectedIndex;
}

std::string Dropdown::getSelectedText() const {
  if (selectedIndex >= 0 && selectedIndex < static_cast<int>(options.size())) {
    return options[selectedIndex].getString();
  }
  return "";
}

void Dropdown::setSelectedIndex(int index) {
  if (index >= 0 && index < static_cast<int>(options.size())) {
    selectedIndex = index;
    selectedText.setString(options[index].getString());

    if (onChange) {
      onChange(selectedIndex, options[selectedIndex].getString());
    }
  }
}

bool Dropdown::isMainBoxMouseOver(const sf::Vector2f& mousePos) const {
  return mainBox.getGlobalBounds().contains(mousePos);
}

bool Dropdown::isDropdownListMouseOver(const sf::Vector2f& mousePos) const {
  return isOpen && dropdownList.getGlobalBounds().contains(mousePos);
}

int Dropdown::getOptionIndexAtPosition(const sf::Vector2f& mousePos) const {
  for (size_t i = 0; i < optionBackgrounds.size(); i++) {
    if (optionBackgrounds[i].getGlobalBounds().contains(mousePos)) {
      return static_cast<int>(i);
    }
  }
  return -1;
}

void Dropdown::update(const sf::Vector2f& mousePos) {
  bool mainHover = isMainBoxMouseOver(mousePos);

  if (mainHover && !isHovered) {
    mainBox.setOutlineColor(sf::Color(180, 180, 180));
    isHovered = true;
  }
  else if (!mainHover && isHovered && !isDropdownListMouseOver(mousePos)) {
    mainBox.setOutlineColor(sf::Color(120, 120, 120));
    isHovered = false;
  }

  // Update option backgrounds when hovered
  if (isOpen) {
    int hoveredIndex = getOptionIndexAtPosition(mousePos);
    for (size_t i = 0; i < optionBackgrounds.size(); i++) {
      if (static_cast<int>(i) == hoveredIndex) {
        optionBackgrounds[i].setFillColor(sf::Color(60, 60, 60));
      }
      else {
        optionBackgrounds[i].setFillColor(sf::Color(40, 40, 40));
      }
    }
  }
}

void Dropdown::toggle() {
  isOpen = !isOpen;
}

bool Dropdown::handleClick(const sf::Vector2f& mousePos) {
  if (isMainBoxMouseOver(mousePos)) {
    toggle();
    return true;
  }
  else if (isOpen && isDropdownListMouseOver(mousePos)) {
    int index = getOptionIndexAtPosition(mousePos);
    if (index >= 0) {
      setSelectedIndex(index);
      toggle(); // Close after selection
    }
    return true;
  }
  else if (isOpen) {
    // Click outside the dropdown while open - close it
    isOpen = false;
    return true;
  }
  return false;
}

void Dropdown::draw(sf::RenderWindow& window) const {
  window.draw(mainBox);
  window.draw(selectedText);

  if (isOpen) {
    window.draw(dropdownList);
    for (size_t i = 0; i < options.size(); i++) {
      window.draw(optionBackgrounds[i]);
      window.draw(options[i]);
    }
  }
}

// BaseScreen implementation
BaseScreen::BaseScreen(const sf::Font& font) : font(font) {}

void BaseScreen::update(const sf::Vector2f& mousePos) {
  for (auto& button : buttons) {
    button.update(mousePos);
  }
}

void BaseScreen::draw(sf::RenderWindow& window) {
  for (const auto& button : buttons) {
    button.draw(window);
  }
  window.draw(headerText);
}

// MenuScreen implementation
MenuScreen::MenuScreen(const sf::Font& font, const sf::Vector2f& windowSize) : BaseScreen(font) {
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

Screen MenuScreen::handleClick(const sf::Vector2f& mousePos) {
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

// ContentScreen implementation
ContentScreen::ContentScreen(const sf::Font& font, const std::string& title, const std::string& content, const sf::Vector2f& windowSize)
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

void ContentScreen::draw(sf::RenderWindow& window) {
  BaseScreen::draw(window);
  window.draw(contentText);
}

Screen ContentScreen::handleClick(const sf::Vector2f& mousePos) {
  if (buttons[0].isClicked(mousePos)) {
    return Screen::MENU;
  }
  return Screen::MENU; // Fallback
}

// RunScreen implementation
RunScreen::RunScreen(const sf::Font& font, const sf::Vector2f& windowSize)
  : ContentScreen(font, "RUN SCREEN",
    "This is the Run screen.\n\nHere you can start your application or simulation.",
    windowSize) {
}

// ManualScreen implementation
ManualScreen::ManualScreen(const sf::Font& font, const sf::Vector2f& windowSize)
  : ContentScreen(font, "MANUAL SCREEN",
    "This is the Manual screen.\n\nHere you can add instructions or documentation for your application.",
    windowSize) {
}

// TestScreen implementation
TestScreen::TestScreen(const sf::Font& font, const sf::Vector2f& windowSize)
  : ContentScreen(font, "TEST SCREEN",
    "This is the Test screen with UI components.\nTry interacting with the controls below:",
    windowSize),
  checkbox1(font, "Enable Feature 1", sf::Vector2f(50.0f, 200.0f), false),
  checkbox2(font, "Enable Feature 2", sf::Vector2f(50.0f, 230.0f), true),
  slider(font, "Volume", sf::Vector2f(50.0f, 280.0f), 200.0f, 0.0f, 100.0f, 50.0f),
  dropdown(font, { "Option 1", "Option 2", "Option 3", "Option 4" }, sf::Vector2f(50.0f, 340.0f), 200.0f)
{
  // Set up callbacks
  checkbox1.setOnChange([](bool checked) {
    std::cout << "Feature 1 is now " << (checked ? "enabled" : "disabled") << std::endl;
  });

  checkbox2.setOnChange([](bool checked) {
    std::cout << "Feature 2 is now " << (checked ? "enabled" : "disabled") << std::endl;
  });

  slider.setOnChange([](float value) {
    std::cout << "Volume changed to " << value << std::endl;
  });

  dropdown.setOnChange([](int index, const std::string& text) {
    std::cout << "Selected option " << index << ": " << text << std::endl;
  });
}

void TestScreen::update(const sf::Vector2f& mousePos) {
  BaseScreen::update(mousePos);
  checkbox1.update(mousePos);
  checkbox2.update(mousePos);

  // For slider, manually update position if dragging
  if (slider.getValue() >= 0) { // Just a way to access slider without calling .update()
    if (slider.isHandleMouseOver(mousePos)) {
      // Could add hover effects here if wanted
    }
  }

  dropdown.update(mousePos);
}

void TestScreen::draw(sf::RenderWindow& window) {
  ContentScreen::draw(window);
  checkbox1.draw(window);
  checkbox2.draw(window);
  slider.draw(window);
  dropdown.draw(window);
}

Screen TestScreen::handleClick(const sf::Vector2f& mousePos) {
  // Check UI components first
  if (checkbox1.handleClick(mousePos) ||
    checkbox2.handleClick(mousePos) ||
    slider.handleClick(mousePos) ||
    dropdown.handleClick(mousePos)) {
    return Screen::TEST; // Stay on this screen after handling UI component clicks
  }

  // Then check the back button
  if (buttons[0].isClicked(mousePos)) {
    return Screen::MENU;
  }

  return Screen::TEST; // Default: stay on this screen
}

void TestScreen::handleMouseRelease() {
  slider.handleRelease();
}

void TestScreen::handleMouseDrag(const sf::Vector2f& mousePos) {
  // Update slider position if it's being dragged
  if (slider.getValue() >= 0) { // Just to access the slider
    slider.updateFromMousePosition(mousePos);
  }
}