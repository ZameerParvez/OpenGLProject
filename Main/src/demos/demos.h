#pragma once
#include "../renderer/renderer.h"

// Contains defenitions of the functions that will be created in seperate cpp files
namespace Demos {

using demoFunc = void (*)(void);

// === Demos ===
extern void SimpleTriangle();
extern void SimpleRectangle();
// =============

// NOTE: if the same thing is selected then the same shape will be initialised again
extern void ImguiDemoSelector(Demos::demoFunc& currDemo);
extern void RunDemos();

}

