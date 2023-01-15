#ifndef BUTTON_HPP
#define BUTTON_HPP

#include "resourceHolder.hpp"
#include "resourceIdentifiers.hpp"

#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/NonCopyable.hpp>

#include <string>
#include <functional>

namespace sf { class Event; class RenderWindow; class RenderTarget; class RenderStates; }

class Button : public sf::Drawable, public sf::Transformable, private sf::NonCopyable
{
public:
    explicit Button(FontHolder& fontHolder, std::string text);
    virtual ~Button();

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void update(sf::Time dt, const sf::RenderWindow& window);
    void handleEvent(const sf::Event& event);

    void setText(const std::string& text);
    void setSize(const sf::Vector2f& size);
    void setBackgroundColor(const sf::Color& color);

    void setPressedCallback(const std::function<void()>& pressedCallback);

    sf::FloatRect getLocalBounds() const;
    sf::FloatRect getGlobalBounds() const;

    bool isSelected(const sf::RenderWindow& window) const;

private:
    sf::RectangleShape m_shape;
    sf::Transformable m_transform;
    sf::Text m_text;
    sf::Time m_timer;
    std::function<void()> m_pressedCallback;
    bool m_showText;
};

#endif
