/* blobilism.cpp
 * Implements an interactive circle-brush drawing program that allows user to
 * draw, change brush size and transparency, pick brush color from palette,
 * and clear canvas
 * JL
 * January 26, 2023
 */

#include <iostream>
#include <vector>
#include "tinygl-cpp.h"
using namespace tinygl;

// represents a 3-tuple RGB color as float values from 0 to 1
struct rgb {
  float red;
  float green;
  float blue;
};

// stores info for one circle
struct circle {
  struct rgb color;
  float alpha;
  int size;  // diameter
  float x;
  float y;
};

class MyWindow : public Window {
  public:
    MyWindow(int w, int h) : Window(w, h) {}

    // initial setup before drawing loop starts
    void setup() override {
      std::cout << "Window size: " << width() << ", " << height() << std::endl;
    }

    // responds to mouse movement
    virtual void mouseMotion (int x, int y, int dx, int dy) override {
      if (mouseIsDown(GLFW_MOUSE_BUTTON_LEFT)) {
        // store a circle with the current color, transparency, size, x, y
        struct circle newCircle = {{_color.red, _color.green, _color.blue},
                                    _alpha, _size, (float) x, (float) y};
        // add new circle to list of existing circles
        _drawnCircles.push_back(newCircle);
      }
    }

    // responds to a mouse button press
    virtual void mouseDown(int button, int mods) override {
      if (button == GLFW_MOUSE_BUTTON_LEFT) {
        float mx = mouseX();  // current mouse pos x
        float my = mouseY();  // current mouse pos y
        // if distance between mouse and each color circle is less than radius,
        // color has been clicked
        for (int i = 0; i < 4; i++) {
          if (computeDistance(mx, my, width() * 0.2f * (i + 1), 35) < 25) {
            _color = _palette[i];
            printf("Set color to (%f, %f, %f)\n", _color.red, _color.green, _color.blue);
            break;
          }
        }
      }
    }

    // responds to a key press
    void keyDown(int key, int mods) override {
      if (key == GLFW_KEY_UP) {
        // increase size of circle
        _size += 5;
        std::cout << "Pressed UP: Increase brush size to " << _size << std::endl;
      } else if (key == GLFW_KEY_DOWN) {
        // decrease size of circle if result will still be positive
        if (_size > 5) {
          _size -= 5;
          std::cout << "Pressed DOWN: Decrease brush size to " << _size << std::endl;
        } else {
          std::cout << "Pressed DOWN: Reached minimum brush size " << _size << std::endl;
        }
      } else if (key == GLFW_KEY_LEFT) {
        // decrease alpha if result will still be nonnegative
        if (_alpha >= 0.05) {
          _alpha -= 0.05;
          std::cout << "Pressed LEFT: Decrease transparency to " << _alpha << std::endl;
        } else {
          std::cout << "Pressed LEFT: Reached minimum transparency " << _alpha << std::endl;
        }
      } else if (key == GLFW_KEY_RIGHT) {
        // increase alpha if result will not exceed 1
        if (_alpha <= 0.95) {
          _alpha += 0.05;
          std::cout << "Pressed RIGHT: Increase transparency to " << _alpha << std::endl;
        } else {
          std::cout << "Pressed RIGHT: Reached maximum transparency " << _alpha << std::endl;
        }
      } else if (key == GLFW_KEY_C) {
        // clear canvas
        _drawnCircles.clear();
        std::cout << "Pressed C: Clear canvas" << std::endl;
      }
    }

    // called in a loop to draw to window/canvas
    void draw() override {
      background(0.95f, 0.95f, 0.95f); // parameters: r, g, b

      // draw pallette
      color(0.1f, 0.1f, 0.1f);
      square(width()/2.0f, 35, width(), 70);
      for (int i = 0; i < 4; i++) {
        color(_palette[i].red, _palette[i].green, _palette[i].blue);
        circle(width() * 0.2f * (i + 1), 35, 50);
      }

      // draw user-defined circles
      for (int i = 0; i < _drawnCircles.size(); i++) {
        color(_drawnCircles[i].color.red, _drawnCircles[i].color.green,
          _drawnCircles[i].color.blue, _drawnCircles[i].alpha);
        circle(_drawnCircles[i].x, _drawnCircles[i].y, _drawnCircles[i].size);
      }
    }

  private:
    struct rgb _color = {0.1f, 0.1f, 0.1f}; // current color
    float _alpha = 1.0f; // current transparency
    int _size = 20; // current circle size
    // list of circles to draw each frame
    std::vector<struct circle> _drawnCircles;
    // list of palette colors
    struct rgb _palette[4] = {{0xC8 / 255.0f, 0x86 / 255.0f, 0x65 / 255.0f},
                              {0xB4 / 255.0f, 0x32 / 255.0f, 0x1D / 255.0f},
                              {0x56 / 255.0f, 0x72 / 255.0f, 0x7B / 255.0f},
                              {0xC5 / 255.0f, 0xBF / 255.0f, 0xBA / 255.0f}};

    // helper function to compute distance between two positions
    float computeDistance(float x1, float y1, float x2, float y2) {
      return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
    }
};

int main() {
  MyWindow window(500, 500);
  window.run();  // opens window, invokes setup(), starts draw loop
}
