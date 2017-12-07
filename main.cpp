//#include "Linked_List.h"
#include "iostream"
#include <SFML/Graphics.hpp>
#include <math.h>

sf::Vector2i size;
struct selection
{
        bool exists = false;
        sf::Vector2i coords{-1,-1};
};
sf::Texture background;
struct card {
        sf::Texture picture;
        sf::Sprite sprite;
        sf::RectangleShape rect;
        sf::FloatRect floatRect;
        bool turned;
        bool enabled = true;
        int number;
};

std::vector<int> numberList;
void genNumberPairs() {
        for (int i = 1; i < size.x * size.y / 2; ++i) {
                numberList.push_back(i);
                numberList.push_back(i);
        }
        std::random_shuffle(numberList.begin(), numberList.end());
}

selection sel1;
selection sel2;

int main() {
        sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");

        std::cout << "rows" << "\n";
        std::cin >> size.y;
        std::cout << "columns" << "\n";
        std::cin >> size.x;

        card cards[size.x][size.y];
        genNumberPairs();
        int counter = 0;
        for (int i = 0; i < size.x; ++i) {
                for (int j = 0; j < size.y; ++j) {
                        sf::Color color(int(rand() % 256 + 1),
                                        int(rand() % 256 + 1),
                                        int(rand() % 256 + 1));
                        cards[i][j].rect.setSize(sf::Vector2f(100, 100));
                        cards[i][j].rect.setPosition(100 * i * 1.05, 100 * j * 1.05);
                        cards[i][j].rect.setFillColor(color);
                        cards[i][j].turned = false;
                        cards[i][j].floatRect = cards[i][j].rect.getGlobalBounds();
                        cards[i][j].number = numberList[counter];
                        counter++;
                }
        }

        while (window.isOpen()) {

                sf::Vector2i point = sf::Mouse::getPosition(window);

                sf::Event event;
                while (window.pollEvent(event)) {
                        if (event.type == sf::Event::Closed)
                                window.close();

                        if (event.type == sf::Event::KeyPressed) {
                                if (event.key.code == sf::Keyboard::Escape) {
                                        return 0;
                                }
                        }
                        if (event.type == sf::Event::MouseButtonPressed)
                        {
                                if (event.mouseButton.button == sf::Mouse::Left)
                                {
                                        for (int i = 0; i < size.x; ++i)
                                        {
                                                for (int j = 0; j < size.y; ++j)
                                                {
                                                        if (cards[i][j].floatRect.contains(sf::Vector2f(point)) && cards[i][j].enabled)
                                                        {
                                                                std::cout << "i " << i << " j " << j << " number " << cards[i][j].number << "\n";
                                                                std::cout << "SEL1 STATUS = " << sel1.exists << "\n";
                                                                std::cout << "SEL1 content = " << sel1.coords.x << "," << sel1.coords.y << "\n";
                                                                std::cout << "SEL2 STATUS = " << sel2.exists << "\n";
                                                                std::cout << "SEL2 content = " << sel2.coords.x << "," << sel2.coords.y << "\n";


                                                                if (sel1.exists)
                                                                {
                                                                        sel2.coords = sf::Vector2i(i,j);
                                                                        sel2.exists = true;
                                                                        std::cout << "sel2 = [" << i << " , " << j << "]\n";
                                                                }
                                                                else if(!sel1.exists)
                                                                {
                                                                        sel1.coords = sf::Vector2i(i,j);
                                                                        sel1.exists = true;
                                                                        std::cout << "sel1 = [" << i << " , " << j << "]\n";
                                                                }
                                                        }
                                                }
                                        }

                                }
                        }
                }
                if (sel1.exists && sel2.exists)
                {
                        if ((cards[sel1.coords.x][sel1.coords.y].number ==
                             cards[sel2.coords.x][sel2.coords.y].number) &&
                            (sel1.coords != sel2.coords))
                        {
                                //cards[sel1.coords.x][sel1.coords.y].rect.setFillColor(sf::Color::Transparent);
                                cards[sel1.coords.x][sel1.coords.y].enabled = false;
                                //cards[sel2.coords.x][sel2.coords.y].rect.setFillColor(sf::Color::Transparent);
                                cards[sel2.coords.x][sel2.coords.y].enabled = false;
                        }
                        sel1.coords = sf::Vector2i(-1,-1);
                        sel2.coords = sf::Vector2i(-1,-1);
                        sel1.exists = false;
                        sel2.exists = false;
                        std::cout << "SEL CLEARED!!" << "\n";
                }
                window.clear();
                for (int i = 0; i < size.x; ++i) {
                        for (int j = 0; j < size.y; ++j) {
                                if (cards[i][j].enabled)
                                {
                                        window.draw(cards[i][j].rect);
                                }
                        }
                }
                window.display();
        }
        int lolint;
        std::cin >> lolint;
        return 0;
}
