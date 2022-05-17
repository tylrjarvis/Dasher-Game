#include "raylib.h"

int main()
{
    //window dimensions
    const int windowWidth = 512;
    const int windowHeight = 380;

    //initialize the window
    InitWindow(windowWidth, windowHeight, "Dasher Game");

    //gravity acceleration update (pixel/s/s)
    const int gravity = 1'000;

    Texture2D scarfy = LoadTexture("textures/scarfy.png");
    Rectangle scarfyRec;
    scarfyRec.width = scarfy.width/6;
    scarfyRec.height = scarfy.height;
    scarfyRec.x = 0;
    scarfyRec.y = 0;
    Vector2 scarfyPos;
    scarfyPos.x = (windowWidth/2) - scarfyRec.width/2;
    scarfyPos.y = windowHeight - scarfyRec.height;

    //animation frame
    int frame = 0;

    //amount of time before updating animation frame
    const float updateTime = 1.0/12.0;
    float runningTime = 0;

    //jump velocity pixels/second
    const int jumpVel = -600;

    int velocity = 0;

    bool isInAir = false;

    SetTargetFPS(60);
    while(!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(WHITE);

        //game logic begins

        //delta time
        const float dT = GetFrameTime();


        //check if on ground
        if(scarfyPos.y >= windowHeight - scarfyRec.height)
        {
            //rectangle on ground
            velocity = 0;
            isInAir = false;
        }
        else
        {
            //rectangle in air
            velocity += gravity * dT;
            isInAir = true;
        }

        //user presses jump
        if(IsKeyPressed(KEY_SPACE) && !isInAir)
        {
            velocity += jumpVel;
        }

        //update position
        scarfyPos.y += velocity * dT;

        //update running time
        runningTime += dT;

        if(runningTime >= updateTime)
        {
            runningTime = 0;

            //update animation frame
            scarfyRec.x = frame*scarfyRec.width;
            frame++;

            if(frame > 5)
            {
                frame = 0;
            }
        }



        DrawTextureRec(scarfy, scarfyRec, scarfyPos, WHITE);

        //game logic ends
        EndDrawing();
    }
    UnloadTexture(scarfy);
    CloseWindow();
}