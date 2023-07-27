#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>

using namespace ftxui;

Component DummyWindowContent() {
  class Impl : public ComponentBase {
   private:
    bool checked[3] = {false, false, false};
    float slider = 50;

   public:
    Impl() {
      Add(Container::Vertical({
          Checkbox("Check me", &checked[0]),
          Checkbox("Check me", &checked[1]),
          Checkbox("Check me", &checked[2]),
          Slider("Slider", &slider, 0.f, 100.f),
      }));
    }
  };
  return Make<Impl>();
}

int main() {
  int window_1_left = 20;
  int window_1_top = 10;
  int window_1_width = 40;
  int window_1_height = 20;

  auto window_1 = Window({
#if __cplusplus >= 202002L
      .inner = DummyWindowContent(),
      .title = "First window",
      .left = &window_1_left,
      .top = &window_1_top,
      .width = &window_1_width,
      .height = &window_1_height,
#else
      DummyWindowContent(),
      "First window",
      &window_1_left,
      &window_1_top,
      &window_1_width,
      &window_1_height,
#endif
  });

  auto window_2 = Window({
#if __cplusplus >= 202002L
      .inner = DummyWindowContent(),
      .title = "My window",
      .left = 40,
      .top = 20,
#else
      DummyWindowContent(),
      "My window",
      40,
      20
#endif
  });

  auto window_3 = Window({
#if __cplusplus >= 202002L
      .inner = DummyWindowContent(),
      .title = "My window",
      .left = 60,
      .top = 30,
#else
      DummyWindowContent(),
      "My window",
      60,
      30
#endif
  });

  auto window_4 = Window({
#if __cplusplus >= 202002L
      .inner = DummyWindowContent(),
#else
      DummyWindowContent()
#endif
  });

  auto window_5 = Window({});

  auto window_container = Container::Stacked({
      window_1,
      window_2,
      window_3,
      window_4,
      window_5,
  });

  auto display_win_1 = Renderer([&] {
    return text("window_1: " +  //
                std::to_string(window_1_width) + "x" +
                std::to_string(window_1_height) + " + " +
                std::to_string(window_1_left) + "," +
                std::to_string(window_1_top));
  });

  auto layout = Container::Vertical({
      display_win_1,
      window_container,
  });

  auto screen = ScreenInteractive::Fullscreen();
  screen.Loop(layout);

  return EXIT_SUCCESS;
}

// Copyright 2023 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
