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
            m_simulator.clearTrajectories();
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

        m_container.handleEvent(event, m_simulator.isRunning());
    }
}

void Application::update(sf::Time dt)
{
	m_simulationUpdateTime += dt;

    if (m_simulator.isRunning() && m_simulationUpdateTime > sf::seconds(1.0f))
    {
        m_simulator.clearTrajectories();
        m_simulator.iteration();
        m_simulationUpdateTime -= sf::seconds(1.0f);
    }
	
    m_container.update(dt);
}

void Application::render()
{
	m_window.clear();

	m_levelLoader.render(m_window);
    m_simulator.render(m_window);
	m_window.draw(m_container);

	m_window.display();
}
