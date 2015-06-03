#include <SFML/Graphics.hpp>
#include <list>

class Model
{
public:
    void Load(const std::string& filename);

    const std::list<sf::Vertex>& Points() const { return points; }
    const std::list<sf::VertexArray>& Polygons() const { return polygons; };
    void Clear() { points.clear(); polygons.clear(); }

    sf::FloatRect BoundingBox() const;

    void Process();

private:
    static bool testPNP(const sf::Vertex& point, const sf::VertexArray& polygon);

    std::list<sf::Vertex> points;
    std::list<sf::VertexArray> polygons;
};
