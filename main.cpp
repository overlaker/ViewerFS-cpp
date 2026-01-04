#include "PicInfo.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char* argv[]) {
    int scr_w = SCR_Width;
    int scr_h = SCR_Height;

    //清單檔案
    string listFile = "picList00.ipl";  //C:\temp\compare

    cout << "argv[0]: " << argv[0] << endl;
    if (argc > 1)
    {
        listFile = argv[1];
    }
    cout << "argc = " << argc << "; listFile = " << listFile << endl;

    //檔案共同路徑
    string srcPath;

    //圖檔list,包含所有檔案與顯示資訊~
    vector<PicDispInfo> photolist;

    //**讀取清單**
    std::ifstream file(listFile); // Open the CSV file
    std::string line;

    std::getline(file, srcPath);    //read the Path of all pictures
    std::getline(file, line);       // Skip the header line

    // Read data line by line
    while (std::getline(file, line)) {
        
        if (line[0] != '#')
        {
            std::stringstream ss(line);
            std::string field;
            int field_index = 0;

            // Parse each field in the line using a comma delimiter
            string fname;
            int orgX, orgY;
            float r;
            int sec;
            while (std::getline(ss, field, ',')) {
                if (field_index == 0) {
                    fname = field;
                }
                else if (field_index == 1) {
                    orgX = std::stoi(field); // Convert string to integer
                }
                else if (field_index == 2) {
                    orgY = std::stoi(field); // Convert string to integer
                }
                else if (field_index == 3) {
                    r = std::stof(field);
                }
                else if (field_index == 4) {
                    sec = std::stoi(field); // Convert string to integer
                }
                field_index++;
            }
            PicDispInfo p(fname, { -orgX, -orgY }, r, sec);
            photolist.push_back(p);
        }
    }

    //check:
    cout << srcPath << endl;
    for (PicDispInfo p : photolist) cout << p.ToCSVString() << endl;

    PicListToPlay list(listFile, srcPath, photolist);

    float scale, duration;
    sf::Vector2i org;
    sf::Image img = list.RenderNextPic(true, scale, org, duration);
    sf::Clock _clock;    //the timer ~
    bool _changed = true;

    //fullscreen window
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "SFML stb_image Display", sf::Style::Fullscreen);
    //sf::RenderWindow window(sf::VideoMode(scr_w, scr_h), "SFML stb_image Display", sf::Style::Fullscreen);

    //Draw the sprite in your SFML main loop
    while (window.isOpen()) {

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        {
            window.close();
        }

        /*
        ****************************************
        Handle the players input
        ****************************************
        */
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) window.close();
            else if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    cout << "Point(L): ";
                    img = list.RenderNextPic(true, scale, org, duration);
                    _clock.restart();
                    _changed = true;
                }
                else if (event.mouseButton.button == sf::Mouse::Right)
                {
                    cout << "Point(R): ";
                    img = list.RenderNextPic(false, scale, org, duration);
                    _clock.restart();
                    _changed = true;
                }
                //cout << event.mouseButton.x << ", " << event.mouseButton.y << endl;
            }
            else if (event.type == sf::Event::MouseMoved)
            {
                //real-time mouse cursor position
                //cPlane.setMouseLocation({ event.mouseMove.x, event.mouseMove.y });
            }
        }

        /* Check time for going on... */
        sf::Time dt = _clock.getElapsedTime();
        if (dt.asSeconds() > duration)
        {
            img = list.RenderNextPic(true, scale, org, duration);
            _clock.restart();
            _changed = true;
        }

        /*
        ****************************************
        Update the scene - do it with sf::Event
        ****************************************
        */
        if (_changed && img.getSize().x > 0 && img.getSize().y > 0)
        {
            //Create an sf::Texture and sf::Sprite
            sf::Texture texture;
            texture.loadFromImage(img);
            //+
            sf::Sprite sprite(texture);
            //Set sprite scale
            sprite.setScale(scale, scale);   //pic.getScale(), pic.getScale());
            //Set sprite position
            sprite.setPosition(org.x, org.y);    //pic.getOrigin().x, pic.getOrigin().y);

            /*
            ****************************************
            Draw the scene
            ****************************************
            */
            window.clear();
            window.draw(sprite);    //顯示圖片~
            window.display();

            _changed = false;
        }
    }

    return 0;
}