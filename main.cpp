#include <SFML/Graphics.hpp>

#define GAME_WIDTH  1280.f
#define GAME_HEIGHT 720.f

#define BG_WIDTH  1920.f
#define BG_HEIGHT 1080.f
#define BG_NUM   8
#define BG_FILE  "png/Background 07/PARALLAX/layer_%02d_1920 x 1080.png"

#define HERO_SCALE .15f
#define HERO_POS_X .45f
#define HERO_POS_Y .69f
#define HERO_ANIM_SPEED .02f
#define HERO_WALK_SPEED .1f
#define HERO_WALK_NUM 10
#define HERO_WALK_FILE "png/cat/Walk (%d).png"

int main()
{
    sf::RenderWindow app(sf::VideoMode(GAME_WIDTH, GAME_HEIGHT), "My game");

    char filename[1024];
    float curX=10000.f;

    // Background
    sf::Texture txBg[BG_NUM];
    sf::Sprite  spBg[BG_NUM];
    float bgDepth[BG_NUM] = { 1.f, .9f, .8f, .6f, .5f, .4f, .01f, .0f };

    for(int i=0; i<BG_NUM; i++)
    {
        sprintf(filename, BG_FILE, i+1);
        if (!txBg[i].loadFromFile(filename))
            return EXIT_FAILURE;
        spBg[i].setTexture(txBg[i], true);
        spBg[i].setScale(GAME_WIDTH/BG_WIDTH, GAME_HEIGHT/BG_HEIGHT);
    }

    // Hero walk
    float heroLiving = 0.f;
    float heroIdle=0;
    float dirHeroWalk = 0.f;

    sf::Texture txHeroWalk[HERO_WALK_NUM*2];
    sf::Sprite  spHeroWalk[HERO_WALK_NUM*2];

    for(int i=0; i<HERO_WALK_NUM; i++)
    {
        sprintf(filename, HERO_WALK_FILE, i+1);
        if (!txHeroWalk[i].loadFromFile(filename))
            return EXIT_FAILURE;
        spHeroWalk[i].setTexture(txHeroWalk[i], true);
        spHeroWalk[i].setPosition(GAME_WIDTH*HERO_POS_X, GAME_HEIGHT*HERO_POS_Y);
        spHeroWalk[i].setScale(HERO_SCALE, HERO_SCALE);

        spHeroWalk[HERO_WALK_NUM+i].setTexture(txHeroWalk[i], true);
        spHeroWalk[HERO_WALK_NUM+i].setPosition(GAME_WIDTH*HERO_POS_X+txHeroWalk[i].getSize().x*HERO_SCALE, GAME_HEIGHT*HERO_POS_Y);
        spHeroWalk[HERO_WALK_NUM+i].setScale(-HERO_SCALE, HERO_SCALE);
    }

    sf::Clock clock;

	// Start the game loop
    while (app.isOpen())
    {
        sf::Time deltaTime = clock.restart();

        // Process events
        sf::Event event;
        while (app.pollEvent(event))
        {
            // Close window : exit
            switch(event.type)
            {
            case sf::Event::Closed:
                app.close();
                break;
            case sf::Event::KeyPressed:
                switch(event.key.code) {
                    case sf::Keyboard::Left : dirHeroWalk = -1.f; heroIdle = HERO_WALK_NUM; break;
                    case sf::Keyboard::Right: dirHeroWalk =  1.f; heroIdle = 0; break;
                }
                break;
            case sf::Event::KeyReleased: dirHeroWalk = 0.f; break;
            }
        }

        int curAnim = heroIdle;

        heroLiving += deltaTime.asMilliseconds();
        if(dirHeroWalk) {
            curX += deltaTime.asMilliseconds()*HERO_WALK_SPEED*dirHeroWalk;
            curAnim = (dirHeroWalk<0 ? HERO_WALK_NUM : 0) + (int)(heroLiving*HERO_ANIM_SPEED) % HERO_WALK_NUM;
        }

        // Clear screen
        app.clear();

        // Draw the sprites
        for(int i=BG_NUM-1; i>=0; i--)
        {
            float bgX = (int)(bgDepth[i]*curX)%(int)GAME_WIDTH;

            spBg[i].setPosition(-bgX          , 0); app.draw(spBg[i]);
            spBg[i].setPosition(GAME_WIDTH-bgX, 0); app.draw(spBg[i]);
        }
        app.draw(spHeroWalk[curAnim]);

        // Update the window
        app.display();
    }

    return EXIT_SUCCESS;
}
