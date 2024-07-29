// simulator.cpp
#include "simulator.hpp"
#include "../modules//utils/print.hpp"

Simulator::Simulator(Environment& env)
        : environment(env),
          window(sf::VideoMode(800, 600), env.getTitle()),
        state(State::Playing),
        rendering(true),
        camera(CameraController(env.getBounds(), window)){
    std::cout << "Loading Environment: " << env.getTitle() << std::endl;

}

void Simulator::run() {
    sf::Clock clock;
    while (window.isOpen()) {
        if (time == 0) this->reset();

        sf::Time elapsed = clock.restart(); // Restart the clock and get elapsed time
        float deltaTime = elapsed.asSeconds(); // Convert elapsed time to seconds

        sf::Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            camera.updateEvent(event);
        }
        camera.update(deltaTime);

        if (state == State::Playing) {
            environment.simulate(deltaTime);
        }

        if (rendering) {
            std::clock_t now = std::clock();
            auto renderDelta = static_cast<double>(now - lastRenderTime);
            if (renderDelta >= FRAME_INTERVAL) {
                lastRenderTime = now;

                window.clear();
                vertexManager.clear();
                //Draw VertexManager to window
                environment.render(vertexManager);
                vertexManager.draw(window);
                window.display();
            }
        }

        time++;
    }
}

void Simulator::setState(State newState) {
    state = newState;
}

void Simulator::reset() {
    environment.reset();
}

void Simulator::setRendering(bool render) {
    rendering = render;
}