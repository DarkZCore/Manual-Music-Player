#include <iostream>
#include <string>
#include <vector>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include "restclient-cpp/restclient.h"
#include <SFML/Audio.hpp>
#include <Windows.h>
#include <Dwmapi.h>
#include <filesystem>
#include <math.h>
#include <time.h>
#pragma comment (lib, "Dwmapi.lib")

using namespace std;


void initMusicVec(std::vector<string> *musicPaths) {
    musicPaths->clear();
    for (const auto& entry : filesystem::directory_iterator("Playlist")) {
        musicPaths->push_back(entry.path().string());
    }
}

int ranNum(int max, int min) {
    if (max == 0) {
        return 0;
    }
    return rand() % max + min;
}

int main() {
    srand(time(NULL));
    std::vector<std::string> musicPaths;
    bool mouseDrag = false;
    initMusicVec(&musicPaths);
    int lastDownX = 0;
    int lastDownY = 0;
    bool musicPaused = false;
    int musicPos = ranNum(musicPaths.size(), 0);
    sf::Music music;
    sf::Texture tx;
    if (!music.openFromFile(musicPaths[musicPos])) {
        return 0;
    }
    if (!tx.loadFromFile("megumin.png")) {
        return 0;
    }
    sf::Sprite sp;
    sp.setTexture(tx);
    sp.setScale(0.1,0.1);

    sf::RenderWindow window(sf::VideoMode(100, 100), "My window", sf::Style::None);
    window.setFramerateLimit(60);
    MARGINS margins;
    margins.cxLeftWidth = -1;
    HWND hwnd = window.getSystemHandle();
    SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
    SetWindowLong(window.getSystemHandle(), GWL_STYLE, WS_POPUP | WS_VISIBLE);
    DwmExtendFrameIntoClientArea(window.getSystemHandle(), &margins);


    // run the program as long as the window is open
    while (window.isOpen())
    {
        if (music.getStatus() == sf::Music::Stopped) {
            initMusicVec(&musicPaths);
            cout << "Music Stopped\n";
                if (musicPos >= musicPaths.size()-1) {
                    musicPos--;
                }
                else if (musicPos <= 0) {
                    musicPos++;
                }
                else {
                    musicPos++;
                }
                std::cout << musicPos;
                if (!music.openFromFile(musicPaths[musicPos])) {
                    return 0;
                }
                music.play();
        }
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
            if (mouseDrag && sf::Event::MouseMoved == event.type) {
                window.setPosition(window.getPosition() + sf::Vector2<int>(event.mouseMove.x - lastDownX, event.mouseMove.y - lastDownY));
                
            }
            else if (sf::Event::MouseButtonPressed == event.type) {
                lastDownX = event.mouseButton.x;
                lastDownY = event.mouseButton.y;
                mouseDrag = true;
                if (sp.getLocalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    initMusicVec(&musicPaths);
                    musicPaused = !musicPaused;
                    if (musicPaused)
                        music.play();
                    if (!musicPaused)
                        music.pause();
                }
            }
            else if(sf::Event::MouseButtonReleased == event.type){

                mouseDrag = false;
            }
            if (event.KeyPressed && sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
                initMusicVec(&musicPaths);
                music.stop();
                music.play();
            }
            if (event.KeyPressed && sf::Keyboard::isKeyPressed(sf::Keyboard::P)) {
                initMusicVec(&musicPaths);
                int temp = ranNum(musicPaths.size(), 0);
                if (musicPos == temp) {
                    if (musicPos >= musicPaths.size()) {
                        musicPos--;
                    }
                    else if(musicPos <= 0){
                        musicPos++;
                    }
                    else {
                        musicPos++;
                    }
                }
                if (!music.openFromFile(musicPaths[temp])) {
                    return 0;
                }
                musicPos = temp;
                music.play();
            }
            
        }

        window.clear(sf::Color::Transparent);
        window.draw(sp);

        window.display();
    }

	return 0;
}