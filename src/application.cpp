#include "application.hpp"

#include <SFML/Window/Event.hpp>
#include <SFML/Window/VideoMode.hpp>

const sf::Time Application::s_timePerFrame = sf::seconds(1.0f / 60.0f);

Application::Application()
    : m_window(sf::VideoMode(1000, 428), "Mars Lander", sf::Style::Close)
    , m_container(m_window)
{
    m_window.setKeyRepeatEnabled(false);

    m_levelLoader.load("resources/data/level_01.txt");
    m_fonts.load(Fonts::Upheaval, "resources/fonts/upheavtt.ttf");
    createButtons();

    m_statisticsText.setFont(m_fonts.get(Fonts::Upheaval));
    m_statisticsText.setPosition(700.0f, 5.0f);
    m_statisticsText.setCharacterSize(20u);
}

Application::~Application()
{

}

void Application::run()
{
    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;

    while (m_window.isOpen())
    {
        sf::Time dt = clock.restart();
        timeSinceLastUpdate += dt;

        while (timeSinceLastUpdate > s_timePerFrame)
        {
            timeSinceLastUpdate -= s_timePerFrame;

            processInput();
            update(s_timePerFrame);
        }

        render();
    }
}

void Application::createButtons()
{
    std::shared_ptr<Button> button = std::make_shared<Button>(m_fonts, "Play");
    auto callback = [this] ()
    {
        const LevelData data = m_levelLoader.levelData();
        m_simulator.run(data.position, data.velocity, data.fuel, data.angle, data.thrust, m_levelLoader.surfacePoints());
    };
    button->setSize(sf::Vector2f(70, 30));
    button->setPressedCallback(callback);
    m_container.pack(button);

    const std::vector<std::string> levels{"Level 1", "Level 2", "Level 3", "Level 4", "Level 5"};

    for (std::size_t id = 1; id < 6; ++id)
    {
        std::shared_ptr<Button> button = std::make_shared<Button>(m_fonts, levels[id-1]);
        auto callback = [this, id] ()
        {
            m_levelLoader.load("resources/data/level_0" + std::to_string(id) + ".txt");
            m_simulator.clear();
        };

        button->setSize(sf::Vector2f(70, 30));
        button->setPressedCallback(callback);
        m_container.pack(button);
    }
}

void Application::processInput()
{
    sf::Event event;
    while (m_window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            m_window.close();

        bool isSimulationRunning = m_simulator.status() == Simulator::Status::RUNNING;
        m_container.handleEvent(event, isSimulationRunning);
    }
}

void Application::update(sf::Time dt)
{
    m_simulator.update(dt);
    m_container.update(dt);

    m_statisticsText.setString("Number of iterations : " + std::to_string(m_simulator.numberOfIterations()));
}

void Application::render()
{
    m_window.clear();

    m_levelLoader.render(m_window);
    m_simulator.render(m_window);
    m_window.draw(m_container);
    m_window.draw(m_statisticsText);

    m_window.display();
}
