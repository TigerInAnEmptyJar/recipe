#include "feeder_application.hpp"

#include <QTimer>

int main(int argc, char** argv)
{
    recipe::webserver::feeder_application app(argc, argv);
    app.parseCommandline();

    QTimer::singleShot(1, &app, &recipe::webserver::feeder_application::start);
    return app.exec();
}
