#include "bots.h"
#include <SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>

SDL_Surface* set_screen(int w, int h, int fw, int fh) {

    SDL_Surface *screen = SDL_SetVideoMode(w, h, 0, SDL_OPENGL | SDL_RESIZABLE);
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, fw, 0, fh);
    return screen;
}

int main()
{

    const int field_width = 100;
    const int field_height = 100;

    const int win_width = 500;
    const int win_height = 500;

    bots bots(field_width, field_height);

    bots.generate(4, 20);

    /* initialize SDL */
    SDL_Init(SDL_INIT_VIDEO);

    /* set the title bar */
    SDL_WM_SetCaption("bots", "bots");

    /* create window */
    SDL_Surface *screen = set_screen(win_width, win_height, field_width, field_height);

    SDL_Event event;
    bool gameover = false;

    std::function < void (void) > team_color[] = {
        []() { glColor3f(1.0f, 0.0f, 0.0); },
        []() { glColor3f(0.0f, 1.0f, 0.0); },
        []() { glColor3f(0.0f, 0.0f, 1.0); },
        []() { glColor3f(1.0f, 1.0f, 0.0); },
        []() { glColor3f(0.0f, 1.0f, 1.0); },
        []() { glColor3f(1.0f, 0.0f, 1.0); },
        []() { glColor3f(1.0f, 1.0f, 1.0); },
    };

    Uint32 previous_time = SDL_GetTicks();

    int acc_time = 0;

    /* message pump */
    while (!gameover) {


        /* look for an event */
        if (SDL_PollEvent(&event)) {
            /* an event was found */
            switch (event.type) {
                /* close button clicked */
                case SDL_QUIT:
                    gameover = true;
                    break;
                /* handle the keyboard */
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) {
                        case SDLK_ESCAPE:
                        case SDLK_q:
                            gameover = true;
                            break;
                    }
                    break;
                // resizing
                case SDL_VIDEORESIZE:
                    screen = set_screen(event.resize.w, event.resize.h, field_width, field_height);
                    break;

            }
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        Uint32 current_time = SDL_GetTicks();

        Uint32 delta = current_time - previous_time;

        previous_time = current_time;

        acc_time += delta;

        // one step each n milliseconds
        if (acc_time > 100) {

            bots.step(delta);
            acc_time = 0;
        }



        /////////////////////////////////////////
        // AI goes here
        //auto m = bots.bot_count();
        //for_each(m.begin(), m.end(), [] (const std::pair<bot::team_id, size_t> &kv) {
                //std::cout << kv.first << ", " << kv.second << std::endl;
                //});
        bots.for_each_bot([&team_color, &bots] (bot & the_bot) {
                the_bot.try_to_do(S);
                });
        /////////////////////////////////////////

        bots.for_each_bot([&team_color, &bots] (const bot & the_bot) {

                team_color[the_bot.get_team()]();

                // WARNING deprecated OpenGL!
                glPushMatrix();
                    const bot::position & pos = the_bot.get_position();

                    glTranslatef(pos.first, pos.second, 0);

                    glBegin(GL_QUADS); 
                        glVertex3f(0.0f, 0.0f, 0.0f);
                        glVertex3f(1.0f, 0.0f, 0.0f);
                        glVertex3f(1.0f, 1.0f, 0.0f);
                        glVertex3f(0.0f, 1.0f, 0.0f); 
                    glEnd();

                glPopMatrix();
            }
        );

        SDL_GL_SwapBuffers();

        gameover = gameover || bots.game_over();
    }

    /* cleanup SDL */
    SDL_Quit();


    return 0;
}

