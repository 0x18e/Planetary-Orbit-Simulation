#include<SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include<random>
#include<iostream>
#include<vector>

#define LOG(x) std::cout << x << "\n"
#define COLOR sf::Color::Blue
const int window_width = 1920;
const int window_height = 1080;
int balls = 1;
float gravity_multiplier = 0.8f;
sf::Color color = COLOR;

float getRandomFloat(float x, float y, std::mt19937 &generator){
    std::uniform_real_distribution<float> rng(x, y);
    return rng(generator);

}
void init(std::vector<sf::Vector2f *> velocity, std::vector<sf::CircleShape *> obj, int balls, std::mt19937 &generator){
    for (int x = 0; x < balls; x++){
        obj[x]->setPosition(sf::Vector2f(getRandomFloat(0, window_width, generator), getRandomFloat(0,window_height, generator)));
        //obj[x]->setRadius(20.f);
        obj[x]->setFillColor(sf::Color::Magenta);
        velocity[x]->x = 0.f;
        velocity[x]->y = 0.f;
    }
}
void moveBalls(sf::Vector2f &velocity, sf::CircleShape &obj){
    if (obj.getPosition().y + obj.getGlobalBounds().height > window_height/2 && obj.getPosition().x + obj.getGlobalBounds().width < window_width/2){
        velocity.y -= 2.f;
        velocity.x += 2.f;
        obj.setFillColor(sf::Color::Green);
        LOG("BOTTOM LEFT");
    }
        //BOTTOM RIGHT
    else if (obj.getPosition().y + obj.getGlobalBounds().height > window_height/2 && obj.getPosition().x + obj.getGlobalBounds().width > window_width/2){
        velocity.y -= 2.f;
        velocity.x -= 2.f;
        obj.setFillColor(sf::Color::Yellow);
        LOG("BOTTOM RIGHT");
    }
        //TOP RIGHT
    else if (obj.getPosition().y + obj.getGlobalBounds().height < window_height/2 && obj.getPosition().x + obj.getGlobalBounds().width > window_width/2){
        velocity.y += 2.f;
        velocity.x -= 2.f;
        obj.setFillColor(sf::Color::Red);
        LOG("TOP RIGHT");
    }
        //TOP LEFT
    else if (obj.getPosition().y + obj.getGlobalBounds().height < window_height/2 && obj.getPosition().x + obj.getGlobalBounds().width < window_width/2){
        velocity.y += 2.f;
        velocity.x += 2.f;
        obj.setFillColor(sf::Color::Blue);
        LOG("TOP LEFT");
    }
    else {
        velocity.y += 2.f;
    }
}

int main()
{
    bool quit = false;
    std::random_device device;
    std::mt19937 generator(device());
    std::uniform_real_distribution<float> rng(window_width, window_height);

    float dt;
    sf::RenderWindow window(sf::VideoMode(window_width, window_height), "blackholes");
    sf::CircleShape blackhole(50.f);
    blackhole.setOrigin(blackhole.getGlobalBounds().width/2, blackhole.getGlobalBounds().height/2);
    blackhole.setPosition(sf::Vector2f(window_width/2, window_height/2));
    std::vector<sf::CircleShape*> obj;
    std::vector<sf::Vector2f*> velocity;

    for (int x = 0; x < balls; x++){
        sf::CircleShape *ball = new sf::CircleShape(getRandomFloat(20, 50, generator));
        sf::Vector2f *vel = new sf::Vector2f(getRandomFloat(1.f, 6.f, generator), getRandomFloat(1.f, 6.f, generator));
        obj.push_back(ball);
        velocity.push_back(vel);

    }
    init(velocity, obj, balls, generator);
    //obj.setPosition(sf::Vector2f(window_width/5, 0.f));
    sf::Vertex vertex[] {
        sf::Vector2f(0, window_height/2),
        sf::Vector2f(window_width,window_height/2),

    };
    sf::Vertex v2[] {
        sf::Vector2f(window_width/2, 0),
        sf::Vector2f(window_width/2, window_height)
    };
    sf::Clock clock;
    while (window.isOpen())
    {
        dt = clock.restart().asSeconds();
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                quit = true;
                for (sf::CircleShape *objects : obj){
                    delete objects;
                }
                obj.clear();
                for (sf::Vector2f *velocities : velocity){
                    delete velocities;
                }
                velocity.clear();
                window.close();
            }
            if (event.type == sf::Event::KeyPressed){
                if (event.key.code == sf::Keyboard::Escape){
                    quit = true;
                    for (sf::CircleShape *objects : obj){
                        delete objects;
                    }
                    obj.clear();
                    for (sf::Vector2f *velocities : velocity){
                        delete velocities;
                    }
                    velocity.clear();
                    window.close();
                }
            }
        }
        //BOTTOM LEFT corner
        //moveBalls(velocity, obj);
        //obj.move(sf::Vector2f(velocity.x, velocity.y)*dt);
        window.clear();
        window.draw(v2, 2, sf::Lines);
        window.draw(vertex, 2, sf::Lines);
        window.draw(blackhole);
        if(!quit) {
            for (int x = 0; x < balls; x++) {

                moveBalls(*velocity[x], *obj[x]);
                obj[x]->move(*velocity[x] * dt);
                window.draw(*obj[x]);
            }
        }
        //window.draw(obj);
        window.display();
    }
}