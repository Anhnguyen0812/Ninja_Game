﻿
#include "CommonFunction.h"
#include "BaseObject.h"
#include "game_map.h"
#include "MainObject.h"
#include "ImpTimer.h"
#include "MainObject.h"
#include <string.h>
#include "TextObject.h"
BaseObject g_background;

TTF_Font* font_score = NULL;

bool InitData()
{
    bool success = true;
    int ret = SDL_Init(SDL_INIT_VIDEO);

    if (ret < 0) {
        return false;
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

    g_window = SDL_CreateWindow("",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH, SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN);

    if (g_window == NULL)
    {
        success = false;
    }
    else
    {
        g_screen = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED);
        if (g_screen == NULL)
            success = false;
        else
        {
            SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
            int imgFlags = IMG_INIT_PNG;
            if (!(IMG_Init(imgFlags) && imgFlags))
                success = false;
        }
    }

    if (TTF_Init() == -1)
    {
        success = false;
    }
    font_score = TTF_OpenFont("font//SuperMario256.ttf", 30);
    if (font_score == NULL)
    {
        success = false;
    }
    
}

bool LoadBackground()
{
    bool ret = g_background.LoadImg("img//background.png", g_screen);
    if (ret == false)
        return false;
    return true;
}

void close()
{
    g_background.Free();
    SDL_DestroyRenderer(g_screen);
    g_screen = NULL;

    SDL_DestroyWindow(g_window);
    g_window = NULL;

    IMG_Quit();
    SDL_Quit();
}

int main(int argc, char* argv[])
{
    ImpTimer fps_time;

    if (InitData() == false)
        return -1;

    if (LoadBackground() == false)
        return -1;

    GameMap game_map;
    game_map.LoadMap("map/map01.dat");
    game_map.LoadTiles(g_screen);
    

    MainObject p_player;
    p_player.LoadImg("img//player sprite//player_right.png", g_screen);
    p_player.set_clips();


    bool is_quit = false;
   
    TextObject score;
    score.SetColor(TextObject::GRAY_TEXT);
  
   
    while (!is_quit)
    {
       
        fps_time.start();

        while (SDL_PollEvent(&g_event) != 0)
        {
            if (g_event.type == SDL_QUIT)
            {
                is_quit = true;
            }

            p_player.HandelInputAction(g_event, g_screen);
        }

        SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
        SDL_RenderClear(g_screen);

        g_background.Render(g_screen, NULL);
        
        // Do player
        Map map_data = game_map.getMap();

        p_player.SetMapXY(map_data.start_x_, map_data.start_y_);
        p_player.DoPlayer(map_data);
        p_player.Show(g_screen);

        game_map.SetMap(map_data);
        game_map.DrawMap(g_screen);
        std::string str_score = "Score: ";



        str_score += std::to_string(p_player.return_coins());
        score.SetText(str_score);
        score.LoadFromRenderText(font_score, g_screen);
        score.RenderText(g_screen, SCREEN_WIDTH - 200, 25);
        SDL_RenderPresent(g_screen);

        
        
        int real_imp_time = fps_time.get_ticks();
        int time_one_frame = 1000 / FRAME_PER_SECOND; // time_ ms

        if (real_imp_time < time_one_frame)
        {
            int delay_time = time_one_frame - real_imp_time;
            SDL_Delay(delay_time);
        }

    }

    close();

    return 0;
}
