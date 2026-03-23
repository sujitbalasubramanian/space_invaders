#include "game.h"

#if PLATFORM_WEB
#include <emscripten.h>
#include <emscripten/em_js.h>
#include <emscripten/html5.h>

// clang-format off
EM_JS(void, call_js_toggle_pause, (), {
  if (typeof _PauseGame === 'function') {
    _PauseGame();
  } else {
    console.log("C++ requested PauseGame, but JS function not found.");
  }
});
// clang-format on

EM_BOOL on_window_switch(int eventType, const EmscriptenFocusEvent *focusEvent,
                         void *userData) {
  call_js_toggle_pause();
  return 1;
}
#endif

int main(void) {
#ifdef PLATFORM_WEB
  emscripten_set_blur_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, nullptr, 1,
                               on_window_switch);
  emscripten_set_focus_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, nullptr, 1,
                                on_window_switch);
#endif

  InitGame();

  while (!WindowShouldClose()) {
    CheckKeyBindingEvents();

    if (IsPaused()) {
      PauseCanvas();
    } else {
      UpdateGameCanvas();
    }
  }

  EndGame();
  CloseWindow();
  return 0;
}
