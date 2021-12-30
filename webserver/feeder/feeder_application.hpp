#pragma once
#include "webserver_access.hpp"

#include <QCoreApplication>

#include <vector>
#include <functional>
#include <memory>

namespace recipe::communication {
class webserver_access;
}
namespace recipe::webserver {

class feeder_application : public QCoreApplication
{
public:
  using QCoreApplication::QCoreApplication;
  ~feeder_application();

  void start();

  void parseCommandline();

private:
  std::vector<std::pair<std::string, std::function<void()>>> _uploads;
  std::unique_ptr<communication::webserver_access> _webAccess;
  size_t _formatting_length{0};
};

}
