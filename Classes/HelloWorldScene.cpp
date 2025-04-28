#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "ui/CocosGUI.h"
#include "cocos2d.h"

#define USE_SIMPLE_AUDIO_ENGINE 1

USING_NS_CC;
using namespace CocosDenshion;

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

bool HelloWorld::init()                                                                                                                                         //init start
{
    if (!Scene::init())
    {
        return false;
    }


    /*~~~~~~~~~~~~~~~~   2 layers of foreground & background with parallex effect   ~~~~~~~~~~~~~*/

    Size visibleSize = Director::getInstance()->getVisibleSize(); //setting the screen size
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    background = Sprite::create("background.png");  // assign the background/foreground
    foreground = Sprite::create("foreground.png");
    
    float backgroundAspectRatio = background->getContentSize().width / background->getContentSize().height; // Get the background image aspect ratio



    // Scale the background to fit the screen width, and adjust hight accordingly to maintain aspect ratio
    float scaleFactorX = visibleSize.width / background->getContentSize().width;     
    float scaleFactorY = visibleSize.height / background->getContentSize().height;
    float scaleFactor = MAX(scaleFactorX, scaleFactorY);                             // Use the larger scale factor to ensure the background fills the screen horizontally without distortion
    background->setScale(scaleFactor);                                               // Apply the scale factor

    // Scale the foreground image to be slightly wider than the background
    float foregroundScaleFactorX = visibleSize.width * 1.2f / foreground->getContentSize().width; // 20% wider than back
    float foregroundScaleFactorY = visibleSize.height / foreground->getContentSize().height;
    float foregroundScaleFactor = MAX(foregroundScaleFactorX, foregroundScaleFactorY);
    foreground->setScale(foregroundScaleFactor);

    background->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y)); // Just cenering the background & foreground
    foreground->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));


    this->addChild(background, 0); // The background is child of this scene - first layer
    this->addChild(foreground, 1);  // The foreground is child of this scene - second layer
    
    /*~~~~~~~~~~~~~~~~   2 layers of foreground & background with parallex effect   ~~~~~~~~~~~~~*/
    

    

    /*~~~~~~~~~~~~~~~~           Drag functionallity using Mouse/Touch             ~~~~~~~~~~~~~*/

    
    isMousePressed = false;          // Initialize mouse press state
    previousMouseX = Director::getInstance()->getWinSize().width / 2; // Initialize previous mouse position

    leftBoundary = 300.0f;              // Set left boundary
    rightBoundary = 1700.0f;            // Set right boundary
    foregroundLeftBoundary = 450.0f;    // Set left boundary for foreground
    foregroundRightBoundary = 3400.0f;  // Set right boundary for foreground

    // Mouse listeners to track press and drag events
    auto listener = EventListenerMouse::create();
    listener->onMouseDown = CC_CALLBACK_1(HelloWorld::onMouseDown, this);       // Mouse button press
    listener->onMouseMove = CC_CALLBACK_1(HelloWorld::onMouseMove, this);       // Mouse move event
    listener->onMouseUp = CC_CALLBACK_1(HelloWorld::onMouseUp, this);           // Mouse button release
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    // Touch event listener for swipe/dragging support (for touchscreens)
    auto listenerTouch = EventListenerTouchOneByOne::create();
    listenerTouch->setSwallowTouches(true);                                      // preventing any touches to go through it to the underlying views below it.
    listenerTouch->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this); // Touch press
    listenerTouch->onTouchMoved = CC_CALLBACK_2(HelloWorld::onTouchMoved, this); // Touch move event
    listenerTouch->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this); // Touch release
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listenerTouch, this);





    /*~~~~~~~~~~~~~~~~           Character & Jump Functionality              ~~~~~~~~~~~~~*/
    


    character = Sprite::create("char_normal.png");                                                        // Create the character sprite
    character->setScale(0.25f);                                                                           // Scale the character size
    character->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 3.3 + origin.y));  // Position at the bottom of the screen
    this->addChild(character, 3);                                                                         // The Character is child of this scene - third layer


    auto jumpButton = ui::Button::create("CloseNormal.png", "CloseSelected.png");    // Create the jump button
    jumpButton->setPosition(Vec2(visibleSize.width - 200, 800));                     // Position the button on the screen
    this->addChild(jumpButton, 2);                                                   // make the UI in the top layer

    jumpButton->addClickEventListener([this](Ref* sender) {
        if (!isJumping) {
            isJumping = true;

            // Change sprite to jump image
            character->setTexture("char_jump.png");                          // When the Character jumps, change the sprite into the jump one

            character->runAction(JumpBy::create(0.5f, Vec2(0, 0), 150, 1));  // Jump 100 pixels high for the duration of one 1 second

            SimpleAudioEngine::getInstance()->playEffect("jump_sound.mp3");  // Play sound

            this->scheduleOnce([this](float dt) {
                character->setTexture("char_normal.png");  // Replace with normal character image when the jump ends
                isJumping = false;                         // Set jumping to false
                }, 0.5f, "reset_character");
        }
        });
    
    /*~~~~~~~~~~~~~~~~           Character & Jump Functionality              ~~~~~~~~~~~~~*/




    return true;                                                                                                                                         //init end
}




/*~~~~~~~~~~~~~~~~                 Mouse & Touch events                     ~~~~~~~~~~~~~*/



void HelloWorld::onMouseDown(cocos2d::Event* event)
{
    isMousePressed = true;
   
    auto mouseEvent = dynamic_cast<cocos2d::EventMouse*>(event); // Update previousMouseX to the current mouse position
    if (mouseEvent) {
        previousMouseX = mouseEvent->getLocation().x;
    }
}
bool HelloWorld::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
    // Store the touch's start position
    previousMouseX = touch->getLocation().x;
    isMousePressed = true;
    return true; 
}

// Those functions will be called when the mouse/touch is released

void HelloWorld::onMouseUp(cocos2d::Event* event)
{
    isMousePressed = false;
}
void HelloWorld::onTouchEnded(Touch* touch, Event* event)
{
    isMousePressed = false;
}



// Those functions are called when the mouse/touch is moved (only moves background if button is pressed/Touch has started)

void HelloWorld::onMouseMove(cocos2d::Event* event)
{
    auto mouseEvent = dynamic_cast<cocos2d::EventMouse*>(event);

    if (mouseEvent && isMousePressed)
    {
        float currentMouseX = mouseEvent->getLocation().x;               // Get the current mouse position
        float deltaX = currentMouseX - previousMouseX;                   // Calculate the horizontal movement (deltaX)

       
        float newBackgroundX = background->getPositionX() + deltaX * 0.5f;  // Move the background slower (parallax effect) horizontally, but avoid leaving empty space

        // Apply boundaries for background
        if (newBackgroundX < leftBoundary) {
            newBackgroundX = leftBoundary;  // Stop at the left boundary
        }
        else if (newBackgroundX > rightBoundary) {
            newBackgroundX = rightBoundary;  // Stop at the right boundary
        }

        // Update the background position
        background->setPositionX(newBackgroundX);

        // Move the foreground 1.5x faster than the background, but only if the background is moving
        if (newBackgroundX > leftBoundary && newBackgroundX < rightBoundary) {
            float newForegroundX = foreground->getPositionX() + deltaX * 1.5f; // foreground moves faster
            foreground->setPositionX(newForegroundX);
            character->setPositionX(character->getPositionX() + deltaX * 1.5f);  // apply same rule for the character since it's in the same layer
        }

        // Update the previous mouse position for the next horizontal movement
        previousMouseX = currentMouseX;
    }
}

void HelloWorld::onTouchMoved(Touch* touch, Event* event)
{
    if (isMousePressed)
    {
        // Get the new touch position
        float currentMouseX = touch->getLocation().x;
        
        float deltaX = currentMouseX - previousMouseX;    // Calculate the movement delta

        // Move the background slower (parallax effect) horizontally
        float newBackgroundX = background->getPositionX() + deltaX * 0.5f;

        // Apply boundaries for background
        if (newBackgroundX < leftBoundary) {
            newBackgroundX = leftBoundary;  // Stop at the left boundary
        }
        else if (newBackgroundX > rightBoundary) {
            newBackgroundX = rightBoundary;  // Stop at the right boundary
        }

        // Update the background position
        background->setPositionX(newBackgroundX);

        // Move the foreground 1.5x faster than the background, but only if the background is moving
        if (newBackgroundX > leftBoundary && newBackgroundX < rightBoundary) {
            float newForegroundX = foreground->getPositionX() + deltaX * 1.5f; // foreground moves faster 1.5
            foreground->setPositionX(newForegroundX);

            // Move the character with the same speed as the foreground
            character->setPositionX(character->getPositionX() + deltaX * 1.5f); // apply same rule for the character since it's in the same layer
        }

        // Update the previous position for the next movement
        previousMouseX = currentMouseX;
    }
}




/*~~~~~~~~~~~~~~~~                 Mouse & Touch events                     ~~~~~~~~~~~~~*/



void HelloWorld::menuCloseCallback(Ref* pSender)
{
    
    Director::getInstance()->end();
}
