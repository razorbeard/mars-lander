#include "container.hpp"

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

Container::Container(const sf::RenderWindow& window)
    : m_window(window)
    , m_buttons()
{
}

Container::~Container()
{
}

void Container::pack(const std::shared_ptr<Button>& button)
{
    if (m_buttons.empty())
    {
        button->setPosition({0.0f, 0.0f});
    }
    else
    {
        // Horizontal alignement
        const float xOffset = m_buttons.back()->getPosition().x + button->getGlobalBounds().width;
        button->setPosition({xOffset, 0.0f});
    }
    
    m_buttons.push_back(std::move(button));
}

void Container::handleEvent(const sf::Event& event, bool isSimulationRunning)
{
    if (!isSimulationRunning)
    {
        for (const std::shared_ptr<Button>& button : m_buttons)
        {
            if (button->isSelected(m_window))
                button->handleEvent(event);
        }
    }
}

void Container::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    for (const std::shared_ptr<Button>& button : m_buttons)
    {
        button->draw(target, states);
    }
}

void Container::update(sf::Time dt)
{
    for (const std::shared_ptr<Button>& button : m_buttons)
    {
        button->update(dt, m_window);
    }
}
