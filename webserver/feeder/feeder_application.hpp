#include <QCoreApplication>
#include <QNetworkAccessManager>

#include <vector>
#include <functional>
#include <memory>

namespace recipe::webserver {

class feeder_application : public QCoreApplication
{
public:
  using QCoreApplication::QCoreApplication;
  ~feeder_application();

  void start();

  void parseCommandline();

private:

  std::vector<std::pair<std::string,std::function<void()>>> _uploads;
  std::unique_ptr<QNetworkAccessManager> _manager;
  size_t _formatting_length{0};
};

}
