#include "audio.h"
#include "video.h"
#include "time.h"
#include "input.h"
#include "math.h"

#define CELL_SIZE 12
#define MAP_HEIGHT 21
#define MAP_WIDTH 21

#define PLAYER_SIZE 8

#define LabyrinthX ((screen_width / 2) - (MAP_WIDTH * CELL_SIZE) / 2 - CELL_SIZE / 2)
#define LabyrinthY ((screen_height / 2) - (MAP_HEIGHT * CELL_SIZE) / 2 - CELL_SIZE / 2)

#define background 0

#define PlayerSpeed 1

#define Music 0
#define Eating 1

int[MAP_HEIGHT][MAP_WIDTH] map;

#include "Maps.h"
#include "PacMan.h"


bool is_colliding(float x, float y)
{
    for (int i=0; i< MAP_HEIGHT; i++)
    {
        for (int j=0; j<MAP_WIDTH; j++)
        {
            if (map[i][j] != 1) continue;
            float cx = LabyrinthX + j * CELL_SIZE;
            float cy = LabyrinthY + i * CELL_SIZE;

            if (x <= cx + CELL_SIZE &&
                x + PLAYER_SIZE >= cx &&
                y <= cy + CELL_SIZE &&
                y + PLAYER_SIZE >= cy
            )
                return true;

        }
    }
    return false;
}

void is_colliding_with_coin(float x, float y, int *ix, int *iy)
{
    for (int i=0; i< MAP_HEIGHT; i++)
    {
        for (int j=0; j<MAP_WIDTH; j++)
        {
            if (map[i][j] != 3) continue;
            float cx = LabyrinthX + j * CELL_SIZE;
            float cy = LabyrinthY + i * CELL_SIZE;

            if (x <= cx + CELL_SIZE &&
                x + PLAYER_SIZE >= cx &&
                y <= cy + CELL_SIZE &&
                y + PLAYER_SIZE >= cy
            )
            {
                *ix = j;
                *iy = i;
                return ;
            }
        }
    }
    *ix = -1;
    *iy = -1;
}

// https://github.com/vircon32/ConsoleSoftware/blob/main/Tutorials/WritingText/Tutorial03-CustomNumbers/Tutorial03-CustomNumbers.c#L108
void PrintScore( int LeftX, int TopY, int Number )
{
    // adjust number to the printable range (6 digits)
    if( Number < 000000 ) Number = 000000;  // written this way for clarity
    if( Number > 999999 ) Number = 999999;
    
    // an easy way to get leading zeroes is
    // to add 1000000 and omit the first digit
    Number += 1000000;
    int CurrentDigitBase = 100000;
    
    // now just draw all digits, including zeroes
    while( CurrentDigitBase > 0 )
    {
        int Digit = (Number / CurrentDigitBase) % 10;
        select_region( RegionZero + Digit );
        draw_region_at( LeftX, TopY );
        
        // advance to the right for next digit
        CurrentDigitBase /= 10;
        LeftX += DigitWidth+2;
    }    
}

void reset(int *PlayerX, int *PlayerY, int *score, int *DX, int *DY)
{
    *PlayerX = LabyrinthX + (CELL_SIZE + 1);
    *PlayerY = (MAP_HEIGHT - CELL_SIZE + 1) * CELL_SIZE;
    *score = 0;
    *DX=0;
    *DY=0;

    get_map();
    for (int i=0; i< MAP_HEIGHT; i++)
        for (int j=0; j< MAP_WIDTH; j++)
            if (map[i][j] == 4)
            {
                *PlayerX = LabyrinthX + (j * CELL_SIZE+2);
                *PlayerY = LabyrinthY + (i * CELL_SIZE+2);
                map[i][j] = 0;
            }
}

void main()
{
    int PlayerX, PlayerY;
    int score = 0;
    int DX, DY;
    int state =  0;

    DefineRegions_background();


    select_sound(Music);
    select_channel(3);
    set_channel_volume(0.3);

    set_sound_loop(true);
    play_sound_in_channel(Music, 3);
  
    while (true)
    {   
        if (state != 1)
        {
            if(state == 2)
            {
                select_region(RegionYouWin);
                draw_region_at(0,0);
            }
            else
            { 
                select_region(RegionStart);
                draw_region_at(0,0);
            }
            if (gamepad_button_start() > 0)
            {
                reset(&PlayerX, &PlayerY, &score, &DX, &DY);
                state = 1;
            }

        }
        else
        {

            if (score >= 176)
                state = 2;

            if (gamepad_up()> 0)
            {
                DY = -1;
                DX = 0;
            }
            else if(gamepad_left() > 0)
            {
                DX = -1;
                DY = 0;
            }
            else if (gamepad_down()> 0)
            {
                DX = 0;
                DY = 1;
            }
            else if(gamepad_right() > 0)
            {
                DY = 0;
                DX = 1;
            }

            int res = is_colliding(PlayerX + DX * PlayerSpeed, PlayerY + DY * PlayerSpeed);
            int ix, iy;
            
            ix = -1;
            iy = -1;
            is_colliding_with_coin(PlayerX + DX * PlayerSpeed, PlayerY + DY * PlayerSpeed, &ix, &iy);


            if (!res)
            {
                PlayerX += DX * PlayerSpeed;
                PlayerY += DY * PlayerSpeed;
            }

            if(iy!=-1 && ix!=-1)
            {
                if (map[iy][ix] == 3){
                    map[iy][ix]=0;
                    score++;
                    play_sound(Eating);
                }
            }

            if (PlayerX < LabyrinthX-CELL_SIZE) PlayerX = screen_width - LabyrinthX + CELL_SIZE;
            if (PlayerY < LabyrinthY-CELL_SIZE) PlayerY = screen_width - LabyrinthY + CELL_SIZE;
            if (PlayerX > screen_width - LabyrinthX + CELL_SIZE) PlayerX = LabyrinthX-CELL_SIZE;
            if (PlayerY > screen_width - LabyrinthY + CELL_SIZE) PlayerY = LabyrinthY-CELL_SIZE;

            clear_screen(color_black);

            select_region( RegionScoreFrame );
            draw_region_at( 10,10 );
            PrintScore( 15, 27, score );

            for (int i=0; i<MAP_HEIGHT; i++)
            {
                for (int j=0; j<MAP_WIDTH; j++)
                {
                    if (map[i][j] == 1)
                    {
                        select_region(RegionWall);
                        draw_region_at(LabyrinthX + CELL_SIZE*j, LabyrinthY + CELL_SIZE*i);
                    }
                    else if(map[i][j] == 3)
                    {
                        select_region(RegionPellet);
                        draw_region_at(LabyrinthX + CELL_SIZE*j+2, LabyrinthY + CELL_SIZE*i+2);
                    }
                }
            }

            select_region(RegionPlayer);
            draw_region_at(PlayerX, PlayerY);
        }
        end_frame();
    }

}
