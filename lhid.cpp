#include <X11/extensions/XTest.h>
#include <X11/keysym.h>
#include <X11/Xlib.h>
#include <cmath>
#include <iostream>
#include "Leap.h"
using namespace Leap;

Display *display;
int lock = 0;

class SampleListener : public Listener {
    public:
        virtual void onInit(const Controller&);
        virtual void onConnect(const Controller&);
        virtual void onDisconnect(const Controller&);
        virtual void onExit(const Controller&);
        virtual void onFrame(const Controller&);
        virtual void onFocusGained(const Controller&);
        virtual void onFocusLost(const Controller&);
};

void SampleListener::onInit(const Controller& controller) {
    std::cout << "Initialized" << std::endl;
}

void SampleListener::onConnect(const Controller& controller) {
    std::cout << "Connected" << std::endl;
    controller.enableGesture(Gesture::TYPE_CIRCLE);
    controller.enableGesture(Gesture::TYPE_KEY_TAP);
    controller.enableGesture(Gesture::TYPE_SCREEN_TAP);
    controller.enableGesture(Gesture::TYPE_SWIPE);

}

void SampleListener::onDisconnect(const Controller& controller) {
    //Note: not dispatched when running in a debugger.
    std::cout << "Disconnected" << std::endl;
}

void SampleListener::onExit(const Controller& controller) {
    std::cout << "Exited" << std::endl;
}

void SampleListener::onFrame(const Controller& controller) {
    // Get the most recent frame and report some basic information
    const Frame frame = controller.frame();
    /*  std::cout << "Frame id: " << frame.id()
        << ", timestamp: " << frame.timestamp()
        << ", hands: " << frame.hands().count()
        << ", fingers: " << frame.fingers().count()
        << ", tools: " << frame.tools().count()
        << ", gestures: " << frame.gestures().count() << std::endl;
        */
    if (!frame.hands().isEmpty()) {
        // Get the first hand
        const Hand hand = frame.hands()[0];

        // Check if the hand has any fingers
        const FingerList fingers = hand.fingers();
        if (!fingers.isEmpty()) {
            // Calculate the hand's average finger tip position
            Vector avgPos;
            for (int i = 0; i < fingers.count(); ++i) {
                avgPos += fingers[i].tipPosition();
            }
            avgPos /= (float)fingers.count();

            int x = 8 * (avgPos.x / 300);
            int y = -(8 * ((avgPos.y - 230) / 300));
            int z = avgPos.z;

            XTestFakeRelativeMotionEvent(display, x, y, 0);

            if(z < -25 && lock == 0)
            {
                lock = 90;
                if(fingers.count() <= 2)
                {
                    XTestFakeButtonEvent(display, 1, true, 0);
                    //XTestFakeButtonEvent(display, 1, false, 0);
                }
                else if(fingers.count() > 2 && fingers.count() <=4)
                {
                    XTestFakeButtonEvent(display, 1, true, 0);
                    XTestFakeButtonEvent(display, 1, false, 0);
                    XTestFakeButtonEvent(display, 1, true, 0);
                    XTestFakeButtonEvent(display, 1, false, 0);
                }
                else if(fingers.count() > 4)
                {
                    XTestFakeButtonEvent(display, 3, true, 0);
                    //XTestFakeButtonEvent(display, 3, false, 0);
                }

            }
            if(z >= -25)
            {
                XTestFakeButtonEvent(display, 1, false, 0);
                XTestFakeButtonEvent(display, 3, false, 0);
            }
            XFlush(display);
            if(lock > 0)
            {
                lock--;
            }
            std::cout << "Hand has " << fingers.count()
                << " fingers, average finger tip position" << avgPos << std::endl;


            /*
               int x = (avgPos.x > 0 ? 1 : -1);
               int y = (avgPos.y > 230 ? -1 : 1);
               int xSpeed = 0;
               int ySpeed = 0;
               if(std::abs(avgPos.x) < 20)
               {
               x = 0;
               }
               else if(std::abs(avgPos.x) < 50)
               {
               xSpeed = 20;
               }
               else if(std::abs(avgPos.x) < 100)
               {
               xSpeed = 7;
               }
               else
               {
               xSpeed = 3;
               }

               int yAdjustedPos = std::abs(avgPos.y - 230);
               if(yAdjustedPos < 20)
               {
               y = 0;
               }
               else if(yAdjustedPos < 50)
               {
               ySpeed = 20;
               }
               else if(yAdjustedPos < 100)
               {
               ySpeed = 7;
               }
               else
               {
               ySpeed = 3;
               }

               XTestFakeRelativeMotionEvent(display, x, 0, xSpeed);
               XTestFakeRelativeMotionEvent(display, 0, y, ySpeed);

               XFlush(display);
               std::cout << "Hand has " << fingers.count()
               << " fingers, average finger tip position" << avgPos << std::endl;
               */
        }
        /*
        // Get the hand's sphere radius and palm position
        std::cout << "Hand sphere radius: " << hand.sphereRadius()
        << " mm, palm position: " << hand.palmPosition() << std::endl;

        // Get the hand's normal vector and direction
        const Vector normal = hand.palmNormal();
        const Vector direction = hand.direction();

        // Calculate the hand's pitch, roll, and yaw angles
        std::cout << "Hand pitch: " << direction.pitch() * RAD_TO_DEG << " degrees, "
        << "roll: " << normal.roll() * RAD_TO_DEG << " degrees, "
        << "yaw: " << direction.yaw() * RAD_TO_DEG << " degrees" << std::endl;
        */
    }
    /*
    // Get gestures
    const GestureList gestures = frame.gestures();
    for (int g = 0; g < gestures.count(); ++g) {
    Gesture gesture = gestures[g];

    switch (gesture.type()) {
    case Gesture::TYPE_CIRCLE:
    {
    CircleGesture circle = gesture;
    std::string clockwiseness;

    if (circle.pointable().direction().angleTo(circle.normal()) <= PI/4) {
    clockwiseness = "clockwise";
    } else {
    clockwiseness = "counterclockwise";
    }

    // Calculate angle swept since last frame
    float sweptAngle = 0;
    if (circle.state() != Gesture::STATE_START) {
    CircleGesture previousUpdate = CircleGesture(controller.frame(1).gesture(circle.id()));
    sweptAngle = (circle.progress() - previousUpdate.progress()) * 2 * PI;
    }
    std::cout << "Circle id: " << gesture.id()
    << ", state: " << gesture.state()
    << ", progress: " << circle.progress()
    << ", radius: " << circle.radius()
    << ", angle " << sweptAngle * RAD_TO_DEG
    <<  ", " << clockwiseness << std::endl;
    break;
    }
    case Gesture::TYPE_SWIPE:
    {
    SwipeGesture swipe = gesture;
    std::cout << "Swipe id: " << gesture.id()
    << ", state: " << gesture.state()
    << ", direction: " << swipe.direction()
    << ", speed: " << swipe.speed() << std::endl;
    break;
    }
    case Gesture::TYPE_KEY_TAP:
    {
    KeyTapGesture tap = gesture;
    std::cout << "Key Tap id: " << gesture.id()
    << ", state: " << gesture.state()
    << ", position: " << tap.position()
    << ", direction: " << tap.direction()<< std::endl;
    break;
    }
    case Gesture::TYPE_SCREEN_TAP:
    {
    ScreenTapGesture screentap = gesture;
    std::cout << "Screen Tap id: " << gesture.id()
    << ", state: " << gesture.state()
    << ", position: " << screentap.position()
    << ", direction: " << screentap.direction()<< std::endl;
    break;
    }
    default:
    std::cout << "Unknown gesture type." << std::endl;
    break;
    }
    }

    if (!frame.hands().isEmpty() || !gestures.isEmpty()) {
    std::cout << std::endl;
    }
    */
}

void SampleListener::onFocusGained(const Controller& controller) {
    std::cout << "Focus Gained" << std::endl;
}

void SampleListener::onFocusLost(const Controller& controller) {
    std::cout << "Focus Lost" << std::endl;
}

int main() {
    //attach our display to the display object
    display = XOpenDisplay(NULL);

    // Create a sample listener and controller
    SampleListener listener;
    Controller controller;

    // Have the sample listener receive events from the controller
    controller.addListener(listener);

    // Keep this process running until Enter is pressed
    std::cout << "Press Enter to quit..." << std::endl;
    std::cin.get();

    // Remove the sample listener when done
    controller.removeListener(listener);

    return 0;
}
