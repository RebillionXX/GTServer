#pragma once
#include <array>
#include <memory>
#include <queue>
#include <utility>
#include <functional>
#include <event/event_context.h>

namespace GTServer {
    class EventQueue { //unused for now
      public:
        using EventHandler = std::function<void(EventContext &)>;
      private:
        std::array<EventHandler, static_cast<std::size_t>(EventType::TOTAL_EVENTS)> handlers{};
        std::queue<EventContext> queue{};
    };
}