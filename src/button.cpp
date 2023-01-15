#include "button.hpp"
#include "utils.hpp"

#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

Button::Button(FontHolder& fontHolder, std::string text)
	: m_shape()
	, m_text(text, fontHolder.get(Fonts::ID::Upheaval), 15)
	, m_timer(sf::Time::Zero)
	, m_showText(true)
{
	m_shape.setSize(sf::Vector2f(70, 30));
	m_shape.setOutlineThickness(1.0f);
	m_shape.setOutlineColor(sf::Color::Green);
    utils::centerOrigin(m_text);
	m_text.setPosition(sf::Vector2f(getPosition().x + m_shape.getSize().x / 2, getPosition().y + m_shape.getSize().y / 2));
}

Button::~Button()
{
}

void Button::setText(const std::string& text)
{
	m_text.setString(text);
    utils::centerOrigin(m_text);
}

void Button::setSize(const sf::Vector2f& size)
{
	m_shape.setSize(size);
}

void Button::setBackgroundColor(const sf::Color& color)
{
	m_shape.setFillColor(color);
}

void Button::setPressedCallback(const std::function<void()>& pressedCallback)
{
	m_pressedCallback = std::move(pressedCallback);
}

sf::FloatRect Button::getLocalBounds() const
{
	auto width = m_shape.getLocalBounds().width;
	auto height = m_shape.getLocalBounds().height;

	return sf::FloatRect({0.0f, 0.0f}, {width, height});
}

sf::FloatRect Button::getGlobalBounds() const
{
	return getTransform().transformRect(getLocalBounds());
}

void Button::handleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::MouseButtonPressed)
	{
		if (event.mouseButton.button == sf::Mouse::Left)
		{
			m_pressedCallback();
        }
    }
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	sf::Transform transform = states.transform;
	transform *= getTransform();

	target.draw(m_shape, transform);
	target.draw(m_text, transform);
}

void Button::update(sf::Time dt, const sf::RenderWindow& window)
{
	m_timer += dt;

	if (isSelected(window))
	{
		m_text.setFillColor(sf::Color::Yellow);
	}
	else
	{
		m_text.setFillColor(sf::Color::Black);
	}
}

bool Button::isSelected(const sf::RenderWindow& window) const
{
	sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
	return getGlobalBounds().contains(sf::Vector2f(mousePosition.x, mousePosition.y));
}
