#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"





class HelloWorld : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();
    cocos2d::Sprite* character;

    virtual bool init();
    
    
    void onMouseDown(cocos2d::Event* event);
    void onMouseUp(cocos2d::Event* event);                            //Mouse event call back (in case the player is using a PC)
    void onMouseMove(cocos2d::Event* event);
    
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);  
    void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);  //Touch event call back (in case the player is using mobile)
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);  

    void menuCloseCallback(cocos2d::Ref* pSender);

    CREATE_FUNC(HelloWorld);
    
private:

    cocos2d::Sprite* background;                                        //Sprites
    cocos2d::Sprite* foreground;

    
    float leftBoundary;                                                // Limits for the drag functionality
    float rightBoundary;
    float foregroundLeftBoundary;
    float foregroundRightBoundary;

    bool isMousePressed;
    float previousMouseX;

    bool isJumping;                                                     // Boolean to track if character is currently jumping
};


#endif
