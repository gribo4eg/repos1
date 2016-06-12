#include <SFML/Graphics.hpp>
#include "enemyClass.h"
#include "level.h"
#include "subjectClass.h"
#include "map.h"

using namespace std;
using namespace sf;

Enemy::Enemy(Image &image, Level &level, float X, float Y,
              float Width, float Height, string Name):
                  Subject(image, X, Y, Width, Height, Name)
{
    obj = level.GetAllObjects();
    sprite.setTextureRect(IntRect(0, 0, width, height));
    //sprite.setScale(1, 1);
    //sprite.setPosition(x + width/2, y + height/2);
    speed_x = 0.1;

}

void Enemy::interactiveWithMap(float dx, float dy)
{
    for(int i = 0; i<obj.size(); i++)
        {
            if(getRect().intersects(obj[i].rect))
            {
                if(obj[i].name == "Solid")
                {
                    if(dy > 0)
                    {
                        y = obj[i].rect.top - height;
                        speed_y = 0;
                        gravity = true;
                    }
                    if(dy < 0)
                    {
                        y = obj[i].rect.top + obj[i].rect.height;
                        speed_y = 0;
                    }
                    if(dx > 0)
                    {
                        x = obj[i].rect.left - width;
                        speed_x = -0.1;
                        sprite.setScale(-1, 1);
                    }
                    if(dx < 0)
                    {
                        x = obj[i].rect.left + obj[i].rect.width;
                        speed_x = 0.1;
                        sprite.setScale(-1, 1);
                    }
                }
            }
        }
}

void Enemy::position(View *view, float time)
{

    moveTime += time;
    if(moveTime > 3500)
    {
        speed_x *=-1;
        moveTime = 0;
    }
    interactiveWithMap(speed_x, 0);
    x += speed_x * time;

    y += speed_y * time;
    interactiveWithMap(0, speed_y);

    sprite.setPosition(x + width/2, y + height/2);
    if(health <=0){
        life = false;
    }

    speed_y += 0.0015*time;

}