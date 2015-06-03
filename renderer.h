#include <SFML/Graphics.hpp>
#include <list>

class Model;

class Renderer
{
public:
    Renderer();
    void Run();
    void SetModel(const Model& m) { model = &m; }

protected:
    void renderScene();

    sf::RenderWindow window;
    const Model* model;

    // Mouse
    sf::Vector2i cursor;
    bool button;
};
