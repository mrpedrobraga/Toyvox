
#include "extern.hpp"
#include "topics.hpp"

namespace tvx {

  namespace {
    struct EitherAction {
        bool isSimple = true;
        Action action;
        SimpleAction simpleAction;
    };
    struct Topic {
      std::unordered_map<uint32_t, EitherAction> subscriptions;
      void publish(void* data) {
        for (const auto& sub : subscriptions) {
          sub.second.isSimple ? sub.second.simpleAction() : sub.second.action(data);
        }
      }
    };
    std::unordered_map<std::string, Topic> activeTopics;
  }

  Subscription::Subscription(const std::string &topic, const Action &action) : topic(topic), id(++nextId) {
    activeTopics[topic].subscriptions[id].action = action;
    activeTopics[topic].subscriptions[id].isSimple = false;
  }

  Subscription::Subscription(const std::string &topic, const SimpleAction &simpleAction) : topic(topic), id(++nextId){
    activeTopics[topic].subscriptions[id].simpleAction = simpleAction;
    activeTopics[topic].subscriptions[id].isSimple = true;
  }

  Subscription::~Subscription() {
    activeTopics[topic].subscriptions.erase(id);
  }

  std::atomic<uint32_t> Subscription::nextId(0);

	void publishf(const std::string &topic, const char *fmt, ...) {
		char buf[1024];
		va_list args;
		va_start(args, fmt);
		vsnprintf(buf, RTU_ARRAYSIZE(buf), fmt, args);
		buf[RTU_ARRAYSIZE(buf) - 1] = 0;
		va_end(args);
		publish(topic, buf);
	}

  void publishp(const std::string &topic, void *data) {
    activeTopics[topic].publish(data);
  }

  void publish(const std::string& topic) {
    activeTopics[topic].publish(nullptr);
  }

}
