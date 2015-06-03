#include <iostream>
#include "renderer.h"
#include "model.h"

Renderer::Renderer()
    : window(sf::VideoMode(1024, 768), "Viewer"), cursor(0, 0),
    button(false), model(NULL)
{
    window.setFramerateLimit(60);
}

void Renderer::Run()
{
    // show the loaded file in the center view
    if(model) {
        sf::FloatRect bbox = model->BoundingBox();
        //sf::View view = window.getView();
        //view.setCenter(sf::Vector2f(bbox.left + bbox.width / 2, bbox.top + bbox.height / 2));
        sf::View view(bbox);
        window.setView(view);
    }

    while(window.isOpen())
    {
        sf::Event event;

        while(window.pollEvent(event))
        {
            switch(event.type)
            {
                case sf::Event::Closed:
                    window.close();
                    break;

                case sf::Event::MouseMoved:
                    if( button )  // dragging
                    {
                        sf::Vector2f delta = window.mapPixelToCoords(cursor) -
                            window.mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y));

                        sf::View view = window.getView();
                        view.move(delta);
                        window.setView(view);

                        cursor.x = event.mouseMove.x;
                        cursor.y = event.mouseMove.y;
                    }
                    break;

                case sf::Event::MouseWheelMoved:
                    {   // zooming
                        sf::View view = window.getView();
                        //view.setCenter(window.mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y)));
                        view.zoom(event.mouseWheel.delta < 0 ? 1.15 : 0.85);
                        window.setView(view);
                    }
                    break;

                case sf::Event::MouseButtonPressed:
                    cursor.x = event.mouseButton.x;
                    cursor.y = event.mouseButton.y;
                    button = true;
                    break;

                case sf::Event::MouseButtonReleased:
                    button = false;
                    break;
            }
        }

        window.clear(sf::Color::Black);
        renderScene();
        window.display();
    }
}

void Renderer::renderScene()
{
    if(model) {
        const std::list<sf::Vertex>& points = model->Points();
        for(auto it = points.begin(); it != points.end(); ++it) {
            //window.draw(&(*it), 1, sf::Points);

            sf::CircleShape point(100000);
            //sf::CircleShape point(window.mapPixelToCoords(sf::Vector2i(20, 20)).x);
            point.setPosition(it->position);
            point.setFillColor(it->color);
            window.draw(point);
        }

        const std::list<sf::VertexArray>& polygons = model->Polygons();
        for(auto it = polygons.begin(); it != polygons.end(); ++it) {
            window.draw(*it);
        }
    }
}
