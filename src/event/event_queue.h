#pragma once
#include <array>
#include <memory>
#include <queue>
#include <utility>
#include <functional>
#include <event/event_context.h>

namespace GTServer {
    class EventQueue {
      public:
        using EventHandler = std::function<void(EventContext &)>;

        template <EventType Tevent>
        bool register_handler(EventHandler handler) {
            handlers.at(static_cast<std::size_t>(Tevent)) = std::move(handler);
            return true;
        }

        template <EventType Tevent>
        void enqueue(EventContext &&event) {
            event.type = Tevent;
            queue.push(std::move(event));
        }

        void service() {
            while (!queue.empty()) {
                EventContext &context = queue.front();

                auto &handler = handlers.at(static_cast<std::size_t>(context.m_type));
                if (handler)
                    handler(context);
                
                queue.pop();
            }
        }

      private:
        std::array<EventHandler, static_cast<std::size_t>(EventType::NumEvents)> handlers{};
        std::queue<EventContext> queue{};
    };
}