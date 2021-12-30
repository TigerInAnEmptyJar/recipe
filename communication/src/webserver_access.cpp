#include "webserver_access.hpp"

#include "web_serializer_json.hpp"

#include <QFile>
#include <QHttpMultiPart>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

#include <boost/uuid/uuid_io.hpp>

#include <iostream>
namespace recipe::communication {

struct webserver_access::Impl
{
  QNetworkAccessManager _manager;
  web_serializer_json _serializer;
};

webserver_access::webserver_access(std::string const& address, std::string const& port)
    : _p(std::make_unique<Impl>()), _address(address), _port(port)
{
}

webserver_access::~webserver_access() = default;

void webserver_access::sendRecipe(recipe const& r, std::function<void()> onFinished)
{
  auto body = _p->_serializer.serialize(r);
  QNetworkRequest request{QUrl{QString::fromStdString(_address + ":" + _port + "/v1/recipe")}};
  request.setRawHeader("Content-Type", "application/json");
  request.setRawHeader("Accept", "application/problem+json");
  request.setRawHeader("api_key", "12345");

  auto reply = _p->_manager.post(request, QByteArray::fromStdString(body));
  QObject::connect(reply, &QNetworkReply::finished, reply,
                   [reply, onFinished = std::move(onFinished)]() {
                     reply->deleteLater();
                     onFinished();
                   });
}

void webserver_access::updateRecipe(recipe const& r, std::function<void()> onFinished)
{
  auto body = _p->_serializer.serialize(r);
  QNetworkRequest request{QUrl{QString::fromStdString(_address + ":" + _port + "/v1/recipe/" +
                                                      boost::uuids::to_string(r.id()))}};
  request.setRawHeader("Content-Type", "application/json");
  request.setRawHeader("Accept", "application/problem+json");
  request.setRawHeader("api_key", "12345");

  auto reply = _p->_manager.put(request, QByteArray::fromStdString(body));
  QObject::connect(reply, &QNetworkReply::finished, reply,
                   [reply, onFinished = std::move(onFinished)]() {
                     reply->deleteLater();
                     onFinished();
                   });
}

void webserver_access::sendRecipeImage(recipe const& r, std::filesystem::path const& database_path,
                                       std::function<void()> onFinished)
{
  auto filepath = database_path / r.image_path();
  auto id = boost::uuids::to_string(r.id());
  if (!std::filesystem::exists(filepath)) {
    onFinished();
    return;
  }
  QNetworkRequest request{
      QUrl{QString::fromStdString("http://localhost:8080/v1/recipe/image/" + id)}};
  request.setRawHeader("Content-Type", "multipart/form-data");
  request.setRawHeader("api_key", "12345");

  QHttpMultiPart* multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
  QHttpPart imagePart;
  imagePart.setHeader(
      QNetworkRequest::ContentDispositionHeader,
      QVariant("form-data; name=\"" + QString::fromStdString(filepath.filename().native()) +
               "\"; filename=\"" + QString::fromStdString(filepath.native()) + "\""));
  imagePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("image/jpeg"));

  QFile* file = new QFile(QString::fromStdString(filepath.native()));
  if (!file->open(QIODevice::ReadOnly)) {
    std::cout << "could not open file" << std::endl;
  }
  QByteArray fileContent(file->readAll());
  imagePart.setBody(fileContent);
  multiPart->append(imagePart);

  request.setHeader(QNetworkRequest::ContentLengthHeader, file->size());
  auto reply = _p->_manager.post(request, multiPart);
  file->setParent(reply);
  QObject::connect(reply, &QNetworkReply::finished, reply,
                   [reply, onFinished = std::move(onFinished)]() {
                     reply->deleteLater();
                     onFinished();
                   });
}

void webserver_access::removeRecipe(boost::uuids::uuid const& id, std::function<void()> onFinished)
{
  QNetworkRequest request{QUrl{QString::fromStdString(_address + ":" + _port + "/v1/recipe/" +
                                                      boost::uuids::to_string(id))}};
  request.setRawHeader("Content-Type", "application/json");
  request.setRawHeader("Accept", "application/problem+json");
  request.setRawHeader("api_key", "12345");

  auto reply = _p->_manager.sendCustomRequest(request, "DELETE");
  QObject::connect(reply, &QNetworkReply::finished, reply,
                   [reply, onFinished = std::move(onFinished)]() {
                     reply->deleteLater();
                     onFinished();
                   });
}

void webserver_access::retrieveRecipe(boost::uuids::uuid const& id,
                                      std::function<void(std::optional<recipe> r)> callback)
{
  QNetworkRequest request{QUrl{QString::fromStdString(_address + ":" + _port + "/v1/recipe/" +
                                                      boost::uuids::to_string(id))}};
  request.setRawHeader("Accept", "application/problem+json");

  auto reply = _p->_manager.get(request);
  QObject::connect(reply, &QNetworkReply::finished, reply,
                   [this, reply, callback = std::move(callback)]() {
                     if (reply->error() == QNetworkReply::NetworkError::NoError) {
                       auto content = reply->readAll();
                       //          auto r =
                       //          _p->_serializer.deserializeRecipe(content.toStdString());
                       callback(std::nullopt);
                     } else {
                       callback(std::nullopt);
                     }
                     reply->deleteLater();
                   });
}

void webserver_access::sendPlan(plan const& p, std::function<void()> onFinished)
{
  auto body = _p->_serializer.serialize(p);
  QNetworkRequest request{QUrl{QString::fromStdString(_address + ":" + _port + "/v1/plan")}};
  request.setRawHeader("Content-Type", "application/json");
  request.setRawHeader("Accept", "application/problem+json");
  request.setRawHeader("api_key", "12345");

  auto reply = _p->_manager.post(request, QByteArray::fromStdString(body));
  QObject::connect(reply, &QNetworkReply::finished, reply,
                   [reply, onFinished = std::move(onFinished)]() {
                     reply->deleteLater();
                     onFinished();
                   });
}

void webserver_access::updatePlan(plan const& p, std::function<void()> onFinished)
{
  auto body = _p->_serializer.serialize(p);
  QNetworkRequest request{QUrl{QString::fromStdString(_address + ":" + _port + "/v1/plan/" +
                                                      boost::uuids::to_string(p.id()))}};
  request.setRawHeader("Content-Type", "application/json");
  request.setRawHeader("Accept", "application/problem+json");
  request.setRawHeader("api_key", "12345");

  auto reply = _p->_manager.put(request, QByteArray::fromStdString(body));
  QObject::connect(reply, &QNetworkReply::finished, reply,
                   [reply, onFinished = std::move(onFinished)]() {
                     reply->deleteLater();
                     onFinished();
                   });
}

void webserver_access::subscribe(plan const& p, std::string const& subscriber, int mealIndex,
                                 int recipeIndex, std::function<void()> onFinished)
{
  QJsonObject body;
  body.insert("day", mealIndex);
  body.insert("recipe", recipeIndex);
  body.insert("subscriber", QString::fromStdString(subscriber));

  QNetworkRequest request{QUrl{QString::fromStdString(
      _address + ":" + _port + "/v1/plan/subscribe/" + boost::uuids::to_string(p.id()))}};
  request.setRawHeader("Content-Type", "application/json");
  request.setRawHeader("Accept", "application/problem+json");
  request.setRawHeader("api_key", "12345");

  auto reply = _p->_manager.put(request, QJsonDocument(body).toJson());
  QObject::connect(reply, &QNetworkReply::finished, reply,
                   [reply, onFinished = std::move(onFinished)]() {
                     reply->deleteLater();
                     onFinished();
                   });
}

void webserver_access::unsubscribe(plan const& p, std::string const& subscriber, int mealIndex,
                                   int recipeIndex, std::function<void()> onFinished)
{
  QJsonObject body;
  body.insert("day", mealIndex);
  body.insert("recipe", recipeIndex);
  body.insert("subscriber", QString::fromStdString(subscriber));

  QNetworkRequest request{QUrl{QString::fromStdString(
      _address + ":" + _port + "/v1/plan/unsubscribe/" + boost::uuids::to_string(p.id()))}};
  request.setRawHeader("Content-Type", "application/json");
  request.setRawHeader("Accept", "application/problem+json");
  request.setRawHeader("api_key", "12345");

  auto reply = _p->_manager.put(request, QJsonDocument(body).toJson());
  QObject::connect(reply, &QNetworkReply::finished, reply,
                   [reply, onFinished = std::move(onFinished)]() {
                     reply->deleteLater();
                     onFinished();
                   });
}

void webserver_access::removePlan(boost::uuids::uuid const& id, std::function<void()> onFinished)
{
  QNetworkRequest request{QUrl{
      QString::fromStdString(_address + ":" + _port + "/v1/plan/" + boost::uuids::to_string(id))}};
  request.setRawHeader("Content-Type", "application/json");
  request.setRawHeader("Accept", "application/problem+json");
  request.setRawHeader("api_key", "12345");

  auto reply = _p->_manager.sendCustomRequest(request, "DELETE");
  QObject::connect(reply, &QNetworkReply::finished, reply,
                   [reply, onFinished = std::move(onFinished)]() {
                     reply->deleteLater();
                     onFinished();
                   });
}

void webserver_access::retrievePlan(boost::uuids::uuid const& id,
                                    std::function<void(std::optional<plan> p)> callback)
{
  QNetworkRequest request{QUrl{
      QString::fromStdString(_address + ":" + _port + "/v1/plan/" + boost::uuids::to_string(id))}};
  request.setRawHeader("Accept", "application/problem+json");

  auto reply = _p->_manager.get(request);
  QObject::connect(reply, &QNetworkReply::finished, reply,
                   [this, reply, callback = std::move(callback)]() {
                     if (reply->error() == QNetworkReply::NetworkError::NoError) {
                       auto content = reply->readAll();
                       //          auto p = _p->_serializer.deserializePlan(content.toStdString());
                       callback(std::nullopt);
                     } else {
                       callback(std::nullopt);
                     }
                     reply->deleteLater();
                   });
}

void webserver_access::sendShopping(shopping_list const& s, std::function<void()> onFinished)
{
  auto body = _p->_serializer.serialize(s);
  QNetworkRequest request{QUrl{QString::fromStdString(_address + ":" + _port + "/v1/shopping")}};
  request.setRawHeader("Content-Type", "application/json");
  request.setRawHeader("Accept", "application/problem+json");
  request.setRawHeader("api_key", "12345");

  auto reply = _p->_manager.post(request, QByteArray::fromStdString(body));
  QObject::connect(reply, &QNetworkReply::finished, reply,
                   [reply, onFinished = std::move(onFinished)]() {
                     reply->deleteLater();
                     onFinished();
                   });
}

void webserver_access::updateShopping(shopping_list const& s, std::function<void()> onFinished)
{
  auto body = _p->_serializer.serialize(s);
  QNetworkRequest request{QUrl{QString::fromStdString(_address + ":" + _port + "/v1/shopping/" +
                                                      boost::uuids::to_string(s.id()))}};
  request.setRawHeader("Content-Type", "application/json");
  request.setRawHeader("Accept", "application/problem+json");
  request.setRawHeader("api_key", "12345");

  auto reply = _p->_manager.put(request, QByteArray::fromStdString(body));
  QObject::connect(reply, &QNetworkReply::finished, reply,
                   [reply, onFinished = std::move(onFinished)]() {
                     reply->deleteLater();
                     onFinished();
                   });
}

void webserver_access::removeShopping(boost::uuids::uuid const& id,
                                      std::function<void()> onFinished)
{
  QNetworkRequest request{QUrl{QString::fromStdString(_address + ":" + _port + "/v1/shopping/" +
                                                      boost::uuids::to_string(id))}};
  request.setRawHeader("Content-Type", "application/json");
  request.setRawHeader("Accept", "application/problem+json");
  request.setRawHeader("api_key", "12345");

  auto reply = _p->_manager.sendCustomRequest(request, "DELETE");
  QObject::connect(reply, &QNetworkReply::finished, reply,
                   [reply, onFinished = std::move(onFinished)]() {
                     reply->deleteLater();
                     onFinished();
                   });
}

void webserver_access::retrieveShopping(
    boost::uuids::uuid const& id, std::function<void(std::optional<shopping_list> s)> callback)
{
  QNetworkRequest request{QUrl{QString::fromStdString(_address + ":" + _port + "/v1/shopping/" +
                                                      boost::uuids::to_string(id))}};
  request.setRawHeader("Accept", "application/problem+json");

  auto reply = _p->_manager.get(request);
  QObject::connect(reply, &QNetworkReply::finished, reply,
                   [this, reply, callback = std::move(callback)]() {
                     if (reply->error() == QNetworkReply::NetworkError::NoError) {
                       auto content = reply->readAll();
                       //          auto s =
                       //          _p->_serializer.deserializeShopping(content.toStdString());
                       callback(std::nullopt);
                     } else {
                       callback(std::nullopt);
                     }

                     reply->deleteLater();
                   });
}

} // namespace recipe::communication
