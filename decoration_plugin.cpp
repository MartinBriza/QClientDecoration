#include "decoration_plugin.h"
#include "clientdecoration.h"

#include <qqml.h>

void DecorationPlugin::registerTypes(const char *uri)
{
    // @uri com.whatever.components
    qmlRegisterType<ClientDecoration>(uri, 1, 0, "ClientDecoration");
    qmlRegisterType<TrafficLights>(uri, 1, 0, "TrafficLights");
    qmlRegisterUncreatableType<TrafficLightButton>(uri, 1, 0, "TrafficLightButton", "Traffic Light Buttons are created in TrafficLights");
}

