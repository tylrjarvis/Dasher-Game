#include "raylib.h"

//creates a struct for objects in the game
struct AnimationData
{
    Rectangle rec;
    Vector2 pos;
    int frame;
    float updateTime;
    float runningTime;
};

//checks if the user is on the ground
bool isOnGround(AnimationData data, int windowHeight)
{
    return data.pos.y >= windowHeight - data.rec.height;
}

AnimationData updateAnimationData(AnimationData data, float deltaTime, int maxFrame)
{
            //update running time
            data.runningTime += deltaTime;

            if(data.runningTime >= data.updateTime)
            {
                data.runningTime = 0.0;

                //update animation frame
                data.rec.x = data.frame * data.rec.width;
                data.frame++;

                if(data.frame > maxFrame)
                {
                    data.frame = 0;
                }
            }
            return data;
}

int main()
{
    //window dimensions
    int windowDimensions[2] = {512, 380};


    //initialize the window
    InitWindow(windowDimensions[0], windowDimensions[1], "Dasher Game");

    //gravity acceleration update (pixel/s/s)
    const int gravity = 1'000;

    //hazard spritesheet load
    Texture2D hazard = LoadTexture("textures/12_nebula_spritesheet.png");

    //load scarfy texture map
    Texture2D scarfy = LoadTexture("textures/scarfy.png");

    //load background images
    Texture2D background = LoadTexture("textures/far-buildings.png");
    float bgX = 0;
    Texture2D midground = LoadTexture("textures/back-buildings.png");
    float mgX = 0;
    Texture2D foreground = LoadTexture("textures/foreground.png");
    float fgX = 0;

    //hazard initialization
    int sizeOfHazards = 10;
    AnimationData hazards[sizeOfHazards]{};

    for(int i = 0; i < sizeOfHazards; i++)
    {
        hazards[i].rec.x = 0.0;
        hazards[i].rec.y = 0.0;
        hazards[i].rec.width = hazard.width/8.0f;
        hazards[i].rec.height = hazard.height/8.0f;
        hazards[i].pos.x = windowDimensions[0] + 300*i;
        hazards[i].pos.y = windowDimensions[1] - hazard.height/8;
        hazards[i].frame = 0;
        hazards[i].runningTime = 0.0;
        hazards[i].updateTime = 0.0;
    }

    float finishLine = hazards[sizeOfHazards-1].pos.x;

    //initialize scary character
    AnimationData scarfyData{
        {0.0, 0.0, scarfy.width/6.0f, static_cast<float>(scarfy.height)}, //Rectangle rec
        {(windowDimensions[0]/2) - scarfyData.rec.width/2, windowDimensions[1] - scarfyData.rec.height}, //Vector2 pos
        0, //int frame
        1.0/12.0, //float updateTime
        0 //float runningTime
    };

    //hazard x velocity (pixels/second)
    int hazardVel = -200;

    //jump velocity pixels/second
    const int jumpVel = -600;

    //velocity of the player
    int velocity = 0;

    //check if player is in the air
    bool isInAir = false;

    //checks for character collision with hazards
    bool collision{};

    SetTargetFPS(60);
    while(!WindowShouldClose())
    {
        //delta time
        const float dT = GetFrameTime();


        BeginDrawing();
        ClearBackground(WHITE);

        //game logic begins

        //background scroll speed
        bgX -= 20 * dT;
        if(bgX <= -background.width*2)
        {
            bgX = 0.0;
        }
        //midground scroll speed
        mgX -= 40 * dT;
        if(mgX <= -midground.width*2)
        {
            mgX = 0.0;
        }
        //foreground scroll speed
        fgX -= 80 * dT;
        if(fgX <= -foreground.width*2)
        {
            fgX = 0.0;
        }

        //draw background
        Vector2 bg1Pos{bgX, 0};
        Vector2 bg2Pos{bgX + background.width*2, 0};
        DrawTextureEx(background, bg1Pos, 0.0, 2.0, WHITE);
        DrawTextureEx(background, bg2Pos, 0.0, 2.0, WHITE);

        //draw midground
        Vector2 mg1Pos{mgX, 0};
        Vector2 mg2Pos{mgX + midground.width*2, 0};
        DrawTextureEx(midground, mg1Pos, 0.0, 2.0, WHITE);
        DrawTextureEx(midground, mg2Pos, 0.0, 2.0, WHITE);

        //draw foreground
        Vector2 fg1Pos{fgX, 0};
        Vector2 fg2Pos{fgX + foreground.width*2, 0};
        DrawTextureEx(foreground, fg1Pos, 0.0, 2.0, WHITE);
        DrawTextureEx(foreground, fg2Pos, 0.0, 2.0, WHITE);

        //check if on ground
        if(isOnGround(scarfyData, windowDimensions[1]))
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

        //update hazard position
        for(int i = 0; i < sizeOfHazards; i++)
        {
            hazards[i].pos.x += hazardVel * dT;     
        }

        //update finish line
        finishLine += hazardVel *dT; 
        
        //update character position
        scarfyData.pos.y += velocity * dT;

        //update character animation frame
        if(!isInAir)
        {
            //update character animation
            scarfyData = updateAnimationData(scarfyData, dT, 5);
        }


        //update hazard animation frame
        for(int i = 0; i < sizeOfHazards; i++)
        {
            hazards[i] = updateAnimationData(hazards[i], dT, 7);
        }

        for(AnimationData haz : hazards)
        {
            float pad = 50;
            Rectangle hazRec{
                haz.pos.x + pad,
                haz.pos.y + pad,
                haz.rec.width - 2*pad,
                haz.rec.height - 2*pad
            };
            Rectangle scarfyRec{
                scarfyData.pos.x,
                scarfyData.pos.y,
                scarfyData.rec.width,
                scarfyData.rec.height
            };
            if(CheckCollisionRecs(hazRec, scarfyRec))
            {
                collision = true;
            }
        }

        if(collision)
        {
            DrawText("You Lose!", windowDimensions[0]/3, windowDimensions[1]/2, 40, RED);
        }
        else if (scarfyData.pos.x >= finishLine)
        {
            DrawText("You Win!", windowDimensions[0]/3, windowDimensions[1]/2, 40, GREEN);
        }
        else
        {
            //draw hazards
            for(int i = 0; i < sizeOfHazards; i++)
            {
                DrawTextureRec(hazard, hazards[i].rec, hazards[i].pos, WHITE);           
            }

            //draw character
            DrawTextureRec(scarfy, scarfyData.rec, scarfyData.pos, WHITE);
        }

        //game logic ends
        EndDrawing();
    }
    UnloadTexture(scarfy);
    UnloadTexture(hazard);
    UnloadTexture(background);
    UnloadTexture(midground);
    UnloadTexture(foreground);
    CloseWindow();
}