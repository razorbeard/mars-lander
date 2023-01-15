#ifndef CONTAINER_HPP
#define CONTAINER_HPP

#include "button.hpp"

#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>

#include <memory>
#include <vector>

namespace sf { class Event; class RenderWindow; class RenderTarget; class RenderStates; }

class Container : public sf::Drawable, public sf::Transformable, private sf::NonCopyable
{
public:
    Container(const sf::RenderWindow& window);
    virtual ~Container();

    void pack(const std::shared_ptr<Button>& button);
    void handleEvent(const sf::Event& event, bool isSimulationRunning);
    void update(sf::Time dt);
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    const sf::RenderWindow& m_window;
    std::vector<std::shared_ptr<Button>> m_buttons;
};

#endif
