#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>

int fs, fr, fg, fb;
sf::Text mainText;

void write(const std::string& message, float posX, float posY) {
    mainText.setString(message);
    // Center the text by offsetting half its width and height
    sf::FloatRect textRect = mainText.getLocalBounds();
    mainText.setOrigin(textRect.width / 2, textRect.height / 2);
    mainText.setPosition(posX, posY);
}

struct Rect {
    std::string Name;
    float iPosX, iPosY;
    float iSpeedX, iSpeedY;
    int r, g, b;
    float w, h;
} typedef rect;

struct Circ {
    std::string Name;
    float iPosX, iPosY;
    float iSpeedX, iSpeedY;
    int r, g, b;
    float radius;
} typedef circ;

int main() {
    int wWidth = 0;
    int wHeight = 0;

    std::string filepath = "config/config.txt";
    std::ifstream fin(filepath);
    std::string w;
    fin >> w;
    if (w == "Window" || w == "WINDOW") {
        fin >> wWidth >> wHeight;
    }

    sf::RenderWindow window(sf::VideoMode(wWidth, wHeight), "SFML WORKS!");

    fin >> w;
    if (w == "Font") {
        fin >> w;
    }

    sf::Font myFont;
    if (!myFont.loadFromFile(w)) {
        std::cerr << "Couldn't load the font\n";
        exit(-1);
    }

    fin >> fs >> fr >> fg >> fb;
    std::vector<std::shared_ptr<rect>> rects;
    std::vector<std::shared_ptr<circ>> circs;
    mainText.setFont(myFont);
    mainText.setFillColor(sf::Color(fr, fg, fb));
    mainText.setCharacterSize(fs);

    while (fin >> w) {
        if (w == "Circle") {
            circ c;
            fin >> c.Name >> c.iPosX >> c.iPosY >> c.iSpeedX >> c.iSpeedY >> c.r >> c.g >> c.b >> c.radius;
            circs.push_back(std::make_shared<circ>(c));
        }
        else if (w == "Rectangle") {
            rect r;
            fin >> r.Name >> r.iPosX >> r.iPosY >> r.iSpeedX >> r.iSpeedY >> r.r >> r.g >> r.b >> r.w >> r.h;
            rects.push_back(std::make_shared<rect>(r));
        }
    }

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        window.clear();

        // Handle circles
        for (auto& c : circs) {
            // Move the circle
            c->iPosX += c->iSpeedX;
            c->iPosY += c->iSpeedY;

            // Bounce off walls 
            if (c->iPosX < 0 || c->iPosX + 2 * c->radius > wWidth) {
                c->iSpeedX = -c->iSpeedX;
            }
            if (c->iPosY < 0 || c->iPosY + 2 * c->radius > wHeight) {
                c->iSpeedY = -c->iSpeedY;
            }

            // Render circle
            sf::CircleShape shape(c->radius);
            shape.setPosition(c->iPosX, c->iPosY);
            shape.setFillColor(sf::Color(c->r, c->g, c->b));
            window.draw(shape);

            // Render circle name centered
            write(c->Name, c->iPosX + c->radius, c->iPosY + c->radius);
            window.draw(mainText);
        }

        // Handle rectangles
        for (auto& r : rects) {
            // Move the rectangle
            r->iPosX += r->iSpeedX;
            r->iPosY += r->iSpeedY;

            // Bounce off walls
            if (r->iPosX < 0 || r->iPosX + r->w > wWidth) {
                r->iSpeedX = -r->iSpeedX;
            }
            if (r->iPosY < 0 || r->iPosY + r->h > wHeight) {
                r->iSpeedY = -r->iSpeedY;
            }

            // Render rectangle
            sf::RectangleShape shape(sf::Vector2f(r->w, r->h));
            shape.setPosition(r->iPosX, r->iPosY);
            shape.setFillColor(sf::Color(r->r, r->g, r->b));
            window.draw(shape);

            // Render rectangle name centered
            write(r->Name, r->iPosX + r->w / 2, r->iPosY + r->h / 2);
            window.draw(mainText);
        }

        window.display();
    }
}
