#ifndef __PARALLAX_SCENE_H__
#define __PARALLAX_SCENE_H__

#include "cocos2d.h"
#include "ParallaxScene.h"

class ParallaxScene : public cocos2d::Scene
{
public:
    ParallaxScene();
    ~ParallaxScene();
    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(ParallaxScene);

    void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);

private:
    cocos2d::Sprite* background;
    cocos2d::Sprite* foreground;
    float lastTouchX;  // Track the last touch position
};

#endif // __PARALLAX_SCENE_H__

