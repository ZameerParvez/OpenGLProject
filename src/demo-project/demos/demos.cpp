#include "demos.h"

using namespace Demos;

extern void Demos::ImguiDemoSelector(Demos::demoFunc& currDemo) {
    const auto demoSelect = [&](const char* label, const Demos::demoFunc& select) {
      if (ImGui::Button(label)) {
        currDemo = select;
      }
    };

    ImGui::Begin("Demo Selector");
    demoSelect("Simple Triangle", Demos::SimpleTriangle);
    demoSelect("Simple Rectangle", Demos::SimpleRectangle);
    demoSelect("Simple Texture", Demos::SimpleTexture);
    demoSelect("Simple 3D", Demos::Simple3D);
    ImGui::End();
}

extern void Demos::RunDemos() {
    static Demos::demoFunc currentDemo = Demos::SimpleTriangle;

    ImguiDemoSelector(currentDemo);

    currentDemo();
}