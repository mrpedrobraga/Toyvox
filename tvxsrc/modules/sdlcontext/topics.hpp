#pragma once

#include "extern.hpp"

#if defined(__clang__) || defined(__GNUC__)
#define RTU_FMTARGS(FMT) __attribute__((format(printf, FMT, FMT+1))) // Apply printf-style warnings to user functions.
#define RTU_FMTLIST(FMT) __attribute__((format(printf, FMT, 0)))
#else
#define RTU_FMTARGS(FMT)
#define RTU_FMTLIST(FMT)
#endif

#define RTU_ARRAYSIZE(_ARR) ((int)(sizeof(_ARR)/sizeof(*_ARR))) // Size of static C-style array. Don't use on pointers!

namespace tvx {

	void publishf(const std::string &topic, const char *fmt, ...) RTU_FMTARGS(2);

  void publishp(const std::string &topic, void *data);

  void publish(const std::string& topic);

  template <typename DataType>
  void publish(const std::string& topic, const DataType&& data) {
	  publishp(topic, (void *) &data);
  }

  template <typename DataType>
  void publish(const std::string& topic, const DataType& data) {
	  publishp(topic, (void *) &data);
  }

  typedef std::function<void(void*)> Action;
  typedef std::function<void()> SimpleAction;

  class Subscription {
    public:
      Subscription(const std::string &topic, const Action &action);
      Subscription(const std::string &topic, const SimpleAction &simpleAction);
      Subscription(const Subscription&) = delete; // no copies
      Subscription& operator=(const Subscription&) = delete; // no self-assignments
      Subscription(Subscription&&) = delete; // no moves
      Subscription& operator=(Subscription&&) = delete; // no move assignments
      virtual ~Subscription();
    private:
      std::string topic;
      uint32_t id;
      static std::atomic<uint32_t> nextId;
  };

	typedef std::unique_ptr<Subscription> SubUptr;
}
