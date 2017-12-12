//#include "Linked_List.h"
#include "iostream"
#include <SFML/Graphics.hpp>
#include <math.h>
#include <experimental/filesystem>
#include <ctime>        // std::time
#include <cstdlib>      // std::rand, std::srand
#include <thread>         // std::thread

namespace fs = std::experimental::filesystem;

sf::Vector2i size;
//Selection struct is for, well selections, as there needs to be several of them, and we need to know both if selection exists, and what the selection contains thus a struct.
struct selection
{
        bool exists = false;
        sf::Vector2i coords{-1,-1};
};
//Card Structure contains all the cards, and is possibly passed around to alter spesific cards in case of events like 2 matching cards ... hopefully
sf::Texture background;
struct card {
        sf::Texture textureObj;
        sf::Sprite spriteObj;
        sf::RectangleShape rect;
        sf::FloatRect floatRect;
        bool turned;
        bool enabled = true;
        int number;
};

//Numberlist contains numbers from 0 -> <card grid x > * <card grid y> / 2
std::vector<int> numberList;
//pathlist contains paths of cards in directory spesified lower
std::vector<std::string> pathList;
//RNG generation that i got no clue about, just 'borrowed' it from website
int myrandom (int i) {
        return std::rand()%i;
}

//genNumberPairs generates numberlist with 2 of each number, and suffles the list so that we get random mix of cards in grid
void genNumberPairs() {
        std::srand ( unsigned ( std::time(0) ) );
        for (int i = 1; i < size.x * size.y / 2; ++i) {
                numberList.push_back(i);
                numberList.push_back(i);
        }
        std::random_shuffle(numberList.begin(), numberList.end(), myrandom);
        for (int i = 0; i < numberList.size(); ++i)
        {
                /* code */
                std::cout << numberList[i];

        }
        std::cout << "\n";
}

//genFilePaths generates a vector of strins where we store *some* paths of cards for later use in loading textures with card number matching the slot in the path list
void genFilePaths()
{
        std::string path = "./textures";
        for (auto & p : fs::directory_iterator(path))
        {
                std::string temp;
                //std::cout << p << std::endl;
                temp = p.path();
                pathList.push_back(temp);
        }
}

//creating the selections for all to use
selection sel1;
selection sel2;

/* NOT USED
   void changeCardImageToBGIMG(card card1, card card2)
   {

    card1.spriteObj.setTexture(background);
    card2.spriteObj.setTexture(background);
   }*/

//----------------- ANNOYING AND COMPLEX SEMI BROKEN TIMER STUFF THAT I DONT WANNA TOUCH--------------------
bool waitTimerIsDone = false;

//Timer thread quic explanation: Start timer thread if it's not running, and if it's done running set wait timer is done flag
//
bool running = false;
void timerThread(int seconds)
{
        running = true;

        /* code */
        clock_t startTime = clock();         //Start timer
        double secondsPassed;
        double secondsToDelay = seconds;
        std::cout << "Time to delay: " << secondsToDelay << std::endl;
        bool flag = true;
        while(flag)
        {
                secondsPassed = (clock() - startTime) / CLOCKS_PER_SEC;
                if(secondsPassed >= secondsToDelay)
                {
                        std::cout << secondsPassed << " seconds have passed" << std::endl;
                        waitTimerIsDone = true;
                        flag = false;
                }
        }

        running = false;

}
//--------------------------------------------------------------------------------------------------------------

//a thing...
char cinDump;
int main() {
        sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
        //timerThread(5); // timerThread test

        // ask the user for rows and columns of grid
        std::cout << "rows" << "\n";
        std::cin >> size.y;
        std::cout << "columns" << "\n";
        std::cin >> size.x;

        // creatinf 2d array of cards for that size
        card cards[size.x][size.y];

        //some pre generation for program [explained above at their respectative functions]
        genNumberPairs();
        genFilePaths();
        int counter = 0;

        //load cardback to it's global variable
        if (!background.loadFromFile("cardBack.png"))
        {
                // error...
        }
        //list through all the cards
        for (int i = 0; i < size.x; ++i) {
                for (int j = 0; j < size.y; ++j) {
                        cards[i][j].number = numberList[counter];
                        counter++;

                        if (cards[i][j].number >= 0 ||
                            cards[i][j].number <= pathList.max_size())
                        {
                                /* code

                                   sf::Color color(int(rand() % 256 + 1),
                                                int(rand() % 256 + 1),
                                                int(rand() % 256 + 1));
                                   cards[i][j].rect.setSize(sf::Vector2f(100, 100));
                                   cards[i][j].rect.setPosition(100 * i * 1.05, 100 * j * 1.05);
                                   cards[i][j].rect.setFillColor(color);
                                   cards[i][j].turned = false;
                                   cards[i][j].floatRect = cards[i][j].rect.getGlobalBounds();
                                 */
                                //std::cout << "at number [" << cards[i][j].number << "]" <<  std::endl;//OLD DEBUG CODE
                                //std::cout << "PathFile to be loaded = [" << pathList[cards[i][j].number] << "] " << std::endl;//OLD DEBUG CODE
                                //std::cin >> cinDump;
                                if (!cards[i][j].textureObj.loadFromFile(pathList[cards[i][j].number]))
                                {
                                        // error...
                                }
                                cards[i][j].spriteObj.setTexture(cards[i][j].textureObj);
                                cards[i][j].spriteObj.setTexture(background);
                                cards[i][j].spriteObj.setPosition(500/2 * i * 1.05, 726/2 * j * 1.05);
                                cards[i][j].spriteObj.setScale(sf::Vector2f(0.5, 0.5));
                                cards[i][j].floatRect = cards[i][j].spriteObj.getGlobalBounds();
                        }
                }
        }
        //std::thread thread(timerThread, 3);
        //thread.detach();
        while (window.isOpen()) {
                //Store mouse position on every tic
                sf::Vector2i point = sf::Mouse::getPosition(window);

                sf::Event event;
                while (window.pollEvent(event)) {
                        if (event.type == sf::Event::Closed)
                                window.close();
                        //escape closes window (SFML Wiki)
                        if (event.type == sf::Event::KeyPressed) {
                                if (event.key.code == sf::Keyboard::Escape) {
                                        return 0;
                                }
                        }
                        //if mouse left button is pressed
                        if (event.type == sf::Event::MouseButtonPressed)
                        {
                                if (event.mouseButton.button == sf::Mouse::Left)
                                {
                                        //list through all the cards
                                        for (int i = 0; i < size.x; ++i)
                                        {
                                                for (int j = 0; j < size.y; ++j)
                                                {
                                                        //if mouse is on a card
                                                        if (cards[i][j].floatRect.contains(sf::Vector2f(point)) && cards[i][j].enabled)
                                                        {
                                                                //print some debugg stuff

                                                                /*
                                                                   std::cout << "i " << i << " j " << j << " number " << cards[i][j].number << "\n";//OLD DEBUG CODE
                                                                   std::cout << "SEL1 STATUS = " << sel1.exists << "\n";//OLD DEBUG CODE
                                                                   std::cout << "SEL1 content = " << sel1.coords.x << "," << sel1.coords.y << "\n";//OLD DEBUG CODE
                                                                   std::cout << "SEL2 STATUS = " << sel2.exists << "\n";//OLD DEBUG CODE
                                                                   std::cout << "SEL2 content = " << sel2.coords.x << "," << sel2.coords.y << "\n";//OLD DEBUG CODE
                                                                 */

                                                                //if selection 1 exists BUT 2 doesn't
                                                                if (sel1.exists && !sel2.exists)
                                                                {
                                                                        sel2.coords = sf::Vector2i(i,j);
                                                                        cards[i][j].spriteObj.setTexture(cards[i][j].textureObj);
                                                                        sel2.exists = true;
                                                                        //std::cout << "sel2 = [" << i << " , " << j << "]\n";//OLD DEBUG CODE
                                                                }
                                                                //if neather selection exists
                                                                else if(!sel1.exists)
                                                                {
                                                                        sel1.coords = sf::Vector2i(i,j);
                                                                        cards[i][j].spriteObj.setTexture(cards[i][j].textureObj);
                                                                        sel1.exists = true;
                                                                        //std::cout << "sel1 = [" << i << " , " << j << "]\n";//OLD DEBUG CODE
                                                                }
                                                        }
                                                }
                                        }

                                }
                        }
                }
                //If wait timer is done (so every <X> seconds clear selection and set card texture to background if both selections are selected and if card numbers are same, delete cards)
                if (sel1.exists && sel2.exists)
                {
                        if (!running)
                        {
                                std::thread thread1 (timerThread, 2);
                                thread1.detach();
                        }
                        if (waitTimerIsDone)
                        {
                                //if both selections exist
                                //if both selectios numbers are same, and both selections cordinates are different disable cards
                                if ((cards[sel1.coords.x][sel1.coords.y].number ==
                                     cards[sel2.coords.x][sel2.coords.y].number) &&
                                    (sel1.coords != sel2.coords))
                                {
                                        //disable the card
                                        //cards[sel1.coords.x][sel1.coords.y].rect.setFillColor(sf::Color::Transparent); //OLD CODE
                                        cards[sel1.coords.x][sel1.coords.y].enabled = false;
                                        //cards[sel2.coords.x][sel2.coords.y].rect.setFillColor(sf::Color::Transparent);//OLD CODE
                                        cards[sel2.coords.x][sel2.coords.y].enabled = false;
                                        //waitTimerIsDone = true;//OLD CODE
                                }

                                //card * cardPTR2 = &cards[sel2.coords.x][sel2.coords.y];//OLD CODE
                                //card * cardPTR1 = &cards[sel1.coords.x][sel1.coords.y];//OLD CODE
                                //std::thread changeCardImageToBGIMG(cardPTR1, cardPTR2);//OLD CODE
                                //changeCardImageToBGIMG.detach();//OLD CODE

                                //set cards texture back to background texture
                                cards[sel1.coords.x][sel1.coords.y].spriteObj.setTexture(background);
                                cards[sel2.coords.x][sel2.coords.y].spriteObj.setTexture(background);


                                //clear selections
                                sel1.coords = sf::Vector2i(-1,-1);
                                sel2.coords = sf::Vector2i(-1,-1);
                                sel1.exists = false;
                                sel2.exists = false;
                                //std::cout << "SEL CLEARED!!" << "\n";//OLD DEBUG CODE
                                //unset waitTimerIsDone
                        }
                        waitTimerIsDone = false;
                }
                //std::cout << waitTimerIsDone << "\n";//OLD DEBUG CODE
                window.clear();
                //list through all the cards
                for (int i = 0; i < size.x; ++i) {
                        for (int j = 0; j < size.y; ++j) {
                                //If card is enabled, draw it
                                if (cards[i][j].enabled)
                                {
                                        window.draw(cards[i][j].spriteObj);
                                }
                        }
                }
                //display drawn cards
                window.display();
        }
        //int lolint;//OLD DEBUG CODE
        //std::cin >> lolint;//OLD DEBUG CODE
        return 0;
}
