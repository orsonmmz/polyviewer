#include "model.h"
#include <iostream>
#include <fstream>

void Model::Load(const std::string& filename)
{
    Clear();

    std::fstream f(filename.c_str(), std::fstream::in);

    while(!f.eof()) {
        std::string s;
        f >> s;

        if(s == "polygon") {
            f >> s;
        int corners = atoi(s.c_str());
        sf::VertexArray poly(sf::LinesStrip, corners);

            for(int i = 0; i < corners; ++i) {
                f >> s;
                poly[i].position.x = atoi(s.c_str());
                f >> s;
                poly[i].position.y = atoi(s.c_str());
            }

            polygons.push_back(poly);
        } else if(s == "point") {
            sf::Vector2f point;
            f >> s;
            point.x = atoi(s.c_str());
            f >> s;
            point.y = atoi(s.c_str());

            points.push_back(sf::Vertex(point));
        } else if(!s.empty()) {
            throw std::runtime_error("Invalid data: " + s);
        }
    }

    f.close();
}

void Model::Process()
{
    std::vector<sf::Color> colormap;
    int polyCount = polygons.size();
    //int base = 0xffffff;
    //int quant = base / (polyCount + 2);    // skip black & white

    // assign colors to polygons
    for(auto it = polygons.begin(); it != polygons.end(); ++it)
    {
        sf::VertexArray& polygon = *it;
        int color = reinterpret_cast<int64_t>(&polygon);

        for(int j = 0; j < polygon.getVertexCount(); ++j)
        {
            polygon[j].color.r = color >> 16;
            polygon[j].color.g = (color >> 8) & 0xff;
            polygon[j].color.b = color & 0xff;
        }
    }

    // check point in polygon
    for(auto it = points.begin(); it != points.end(); ++it)
    {
        sf::Vertex& point = *it;
        bool assigned = false;

        for(auto jt = polygons.begin(); jt != polygons.end(); ++jt)
        {
            if(testPNP(point, *jt))
            {
                if(assigned) {
                    std::cout << "already assigned " << point.position.x << " " << point.position.y << std::endl;
                    point.color = sf::Color(255, 0, 0);
                } else {
                    point.color = (*jt)[0].color;
                    assigned = true;
                }
            }
        }
    }
}

sf::FloatRect Model::BoundingBox() const
{
    sf::FloatRect bbox;

    bbox.left = std::numeric_limits<float>::max();
    bbox.top = std::numeric_limits<float>::max();
    bbox.width = std::numeric_limits<float>::min();
    bbox.height = std::numeric_limits<float>::min();

    for(auto it = polygons.begin(); it != polygons.end(); ++it) {
        sf::FloatRect poly_bbox = it->getBounds();

        if(poly_bbox.left < bbox.left)
            bbox.left = poly_bbox.left;

        if(poly_bbox.top < bbox.top)
            bbox.top = poly_bbox.top;

        if(poly_bbox.width > bbox.width)
            bbox.width = poly_bbox.width;

        if(poly_bbox.height > bbox.height)
            bbox.height = poly_bbox.height;
    }

    for(auto it = points.begin(); it != points.end(); ++it) {
        const sf::Vertex& point = *it;

        if(!bbox.contains(point.position)) {
            if(point.position.x < bbox.left) {
                bbox.width += bbox.left - point.position.x;
                bbox.left = point.position.x;
            } else if(point.position.x > bbox.left + bbox.width) {
                bbox.width += point.position.x - (bbox.left + bbox.width);
            }

            if(point.position.y < bbox.top) {
                bbox.height += bbox.top - point.position.y;
                bbox.top = point.position.y;
            } else if(point.position.y > bbox.top + bbox.height) {
                bbox.height += point.position.y - (bbox.top + bbox.height);
            }
        }
    }

    return bbox;
}

bool Model::testPNP(const sf::Vertex& point, const sf::VertexArray& polygon)
{
    int poly_count = polygon.getVertexCount();
    int i, j = poly_count - 1;
    int x = point.position.x, y = point.position.y;
    bool inside = false;

    // http://alienryderflex.com/polygon
    for( i = 0; i < poly_count; ++i )
    {
        if( ( ( polygon[i].position.y < y && polygon[j].position.y >= y ) || ( polygon[j].position.y < y && polygon[i].position.y >= y ) )
            && ( polygon[i].position.x <= x || polygon[j].position.x <= x ) )
        {
            // TODO polygon -> m_points
            inside ^= ( polygon[i].position.x + ( y - polygon[i].position.y ) /
                        (int64_t)( polygon[j].position.y - polygon[i].position.y ) * (int64_t)( polygon[j].position.x - polygon[i].position.x ) < x );
        }

        j = i;
    }

    return inside;
}
