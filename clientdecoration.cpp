#include "clientdecoration.h"

#include <QPainter>
#include <QTimer>
#include <QGuiApplication>
#include <QIcon>
#include <QQuickWindow>
#include <QQmlComponent>

#include "clientdecorationstyle.h"

ClientDecoration::ClientDecoration(QQuickItem *parent)
    : QQuickPaintedItem(parent)
    , m_style(ClientDecorationStyleFactory::create())
    , m_trafficLights(new TrafficLights(this))
{
    setFlag(ItemHasContents, true);
    setAcceptedMouseButtons(Qt::LeftButton);

    if (m_style->prefersLeftSide()) {
        m_trafficLights->setX(1);
    }
    else {
        m_trafficLights->setX(width() - m_trafficLights->width() - 1);
    }
    m_trafficLights->setY(1);
    m_trafficLights->setHeight(m_trafficLights->height() - 1);

    connect(this, &ClientDecoration::widthChanged, [=](){
        if (m_style->prefersLeftSide()) {
            m_trafficLights->setX(1);
        }
        else {
            m_trafficLights->setX(width() - m_trafficLights->width() - 1);
        }
    });
}

ClientDecoration::~ClientDecoration() {

}

ClientDecorationProperties *ClientDecoration::qmlAttachedProperties(QObject *object) {
    return new ClientDecorationProperties(object);
}

void ClientDecoration::updateDimensions() {
    m_trafficLights->updateDimensions();

    bool iconVisible = true;
    bool titleVisible = true;

    QSizeF iconSize = m_style->sizeElement(ClientDecorationStyle::ICON);

    // TODO the widths are mostly wrong

    if (titleVisible) {
        if (m_style->prefersLeftSide()) {
            m_titleRect = boundingRect().adjusted(m_trafficLights->width() + m_style->titlePadding(), 0, - m_trafficLights->boundingRect().width() - m_style->titleMargins(), 0);
        }
        else {
            m_titleRect = boundingRect().adjusted(m_style->titlePadding(), 0, - m_trafficLights->boundingRect().width() - m_style->titleMargins(), 0);
        }
    }
    else {
        m_titleRect = QRectF();
    }

    if (iconVisible) {
        if (m_style->prefersLeftSide()) {
            m_iconRect = QRectF(QPointF(m_trafficLights->width() +m_style->titlePadding(), (boundingRect().height() - iconSize.height()) / 2.0), iconSize);
        }
        else {
            m_iconRect = QRectF(QPointF(m_style->titlePadding(), (boundingRect().height() - iconSize.height()) / 2.0), iconSize);
        }
        if (titleVisible) {
            m_titleRect.setLeft(m_iconRect.right() + m_style->titleMargins());
        }
    }
    else {
        m_iconRect = QRectF();
    }

/*
    m_iconRect = QRectF(header.left() + margin, header.top(), header.height(), header.height());
    m_titleRect = icon.isNull() ? QRectF(iconRect.left() + margin, header.top(), m_trafficLights->x() - iconRect.left() - margin * 2, header.height())
                                     : QRectF(iconRect.right() + margin, header.top(), m_trafficLights->x() - iconRect.right() - margin * 2, header.height());

*/
}

void ClientDecoration::paint(QPainter *p) {
    int margin = 3;

    updateDimensions();

    m_style->paintElement(p, ClientDecorationStyle::BACKGROUND, boundingRect().adjusted(0, 0, -0.5, 0));
    if (!m_iconRect.isNull())
        m_style->paintElement(p, ClientDecorationStyle::ICON, m_iconRect);
    if (!m_titleRect.isNull())
        m_style->paintElement(p, ClientDecorationStyle::TITLE, m_titleRect);

/*
    QIcon icon = QGuiApplication::windowIcon();

    QRectF header = boundingRect();
    QRectF iconRect = QRectF(header.left() + margin, header.top(), header.height(), header.height());
    QRectF titleRect = icon.isNull() ? QRectF(iconRect.left() + margin, header.top(), m_trafficLights->x() - iconRect.left() - margin * 2, header.height())
                                     : QRectF(iconRect.right() + margin, header.top(), m_trafficLights->x() - iconRect.right() - margin * 2, header.height());

    QColor activeBackgroundColor("#0070da");
    QColor inactiveBackgroundColor("#ffffff");

    QColor activeTextColor("#ffffff");
    QColor inactiveTextColor("#000000");

    QColor borderColor("#88000000");

    bool windowActive = window()->windowState() & Qt::WindowActive;
*/

}

void ClientDecoration::mousePressEvent(QMouseEvent *event) {
}

void ClientDecoration::mouseMoveEvent(QMouseEvent *event) {
    if (event->buttons() & Qt::LeftButton) {
        window()->startSystemMove(event->pos());
    }
}

void ClientDecoration::mouseReleaseEvent(QMouseEvent *event) {
}

void ClientDecoration::mouseDoubleClickEvent(QMouseEvent *event) {
    Q_UNUSED(event);
    if (window()->windowState() & Qt::WindowMaximized)
        window()->showNormal();
    else
        window()->showMaximized();
    QTimer::singleShot(0, this, &QQuickItem::update);
}

TrafficLights::TrafficLights(QQuickItem *parent, ClientDecorationStyle *style)
    : QQuickPaintedItem(parent)
    , m_style(style ? style : ClientDecorationStyleFactory::create())
{
    setFlag(ItemHasContents, true);

    setAcceptHoverEvents(true);
    setAcceptedMouseButtons(Qt::LeftButton);

    updateDimensions();


}

TrafficLights::~TrafficLights() {

}

ClientDecorationStyle *TrafficLights::style() {
    return m_style;
}

void TrafficLights::updateDimensions() {
    QRectF result;

    bool minimizeVisible { true };
    bool maximizeVisible { true };
    bool closeVisible { true };

    QSizeF minimizeSize = m_style->sizeElement(ClientDecorationStyle::BUTTON_MINIMIZE);
    QSizeF maximizeSize = m_style->sizeElement(ClientDecorationStyle::BUTTON_MAXIMIZE);
    QSizeF closeSize = m_style->sizeElement(ClientDecorationStyle::BUTTON_CLOSE);

    if (minimizeSize.isNull())
        minimizeVisible = false;
    if (maximizeSize.isNull())
        maximizeVisible = false;
    if (closeSize.isNull())
        closeVisible = false;

    if (closeVisible && !m_closeButton) {
        m_closeButton = new TrafficLightCloseButton(this);
    }
    else if (!closeVisible && m_closeButton) {
        m_closeButton->deleteLater();
        m_closeButton = nullptr;
    }

    if (m_style->prefersLeftSide() && closeVisible) {
        m_closeButtonRect = QRectF(result.topRight() + QPointF(m_style->buttonMargins(), 0), closeSize);
        m_closeButton->setX(m_closeButtonRect.x());
        m_closeButton->setY(m_closeButtonRect.y());
        m_closeButton->setWidth(m_closeButtonRect.width());
        m_closeButton->setHeight(m_closeButtonRect.height());
        result = result.united(m_closeButtonRect);
    }
    if (minimizeVisible) {
        if (!m_minimizeButton) {
            m_minimizeButton = new TrafficLightMinimizeButton(this);
        }
        m_minimizeButtonRect = QRectF(result.topRight() + QPointF(m_style->buttonMargins(), 0), minimizeSize);
        m_minimizeButton->setX(m_minimizeButtonRect.x());
        m_minimizeButton->setY(m_minimizeButtonRect.y());
        m_minimizeButton->setWidth(m_minimizeButtonRect.width());
        m_minimizeButton->setHeight(m_minimizeButtonRect.height());
        result = result.united(m_minimizeButtonRect);
    }
    else if (m_minimizeButton) {
        m_minimizeButton->deleteLater();
        m_minimizeButton = nullptr;
    }
    if (maximizeVisible) {
        if (!m_maximizeButton) {
            m_maximizeButton = new TrafficLightMaximizeButton(this);
        }
        m_maximizeButtonRect = QRectF(result.topRight() + QPointF(m_style->buttonMargins(), 0), maximizeSize);
        m_maximizeButton->setX(m_maximizeButtonRect.x());
        m_maximizeButton->setY(m_maximizeButtonRect.y());
        m_maximizeButton->setWidth(m_maximizeButtonRect.width());
        m_maximizeButton->setHeight(m_maximizeButtonRect.height());
        result = result.united(m_maximizeButtonRect);
    }
    else if (m_maximizeButton) {
        m_maximizeButton->deleteLater();
        m_maximizeButton = nullptr;
    }
    if (!m_style->prefersLeftSide() && closeVisible) {
        m_closeButtonRect = QRectF(result.topRight() + QPointF(m_style->buttonMargins(), 0), closeSize);
        m_closeButton->setX(m_closeButtonRect.x());
        m_closeButton->setY(m_closeButtonRect.y());
        m_closeButton->setWidth(m_closeButtonRect.width());
        m_closeButton->setHeight(m_closeButtonRect.height());
        result = result.united(m_closeButtonRect);
    }
    if (boundingRect() != result) {
        setWidth(result.width());
        setHeight(result.height());
        update();
    }
}

void TrafficLights::paint(QPainter *p) {
    /*
    paintCloseButton(p);
    paintMaximizeButton(p);
    paintMinimizeButton(p);
    */
    //m_style->paintElement(p, ClientDecorationStyle::BUTTON_MINIMIZE, m_minimizeButtonRect);
    //m_style->paintElement(p, ClientDecorationStyle::BUTTON_MAXIMIZE, m_maximizeButtonRect);
    //m_style->paintElement(p, ClientDecorationStyle::BUTTON_CLOSE, m_closeButtonRect);
}

void TrafficLights::mousePressEvent(QMouseEvent *event) {

}

void TrafficLights::mouseMoveEvent(QMouseEvent *event) {
}

void TrafficLights::mouseReleaseEvent(QMouseEvent *event) {

}

TrafficLightButton::TrafficLightButton(TrafficLights *parent)
    : QQuickPaintedItem(parent)
{
    setFlag(ItemHasContents, true);

    setAcceptHoverEvents(true);
    setAcceptedMouseButtons(Qt::LeftButton);
}

TrafficLights *TrafficLightButton::trafficLights() {
    return qobject_cast<TrafficLights*>(parent());
}

void TrafficLightButton::mousePressEvent(QMouseEvent *event) {
    m_lastPress.start();
}

void TrafficLightButton::mouseMoveEvent(QMouseEvent *event) {

}

void TrafficLightButton::mouseReleaseEvent(QMouseEvent *event) {
    if (m_lastPress.elapsed() < 500) {
        activate();
    }
}

ClientDecorationProperties::ClientDecorationProperties(QObject *parent)
    : QObject(parent)
{

}

bool ClientDecorationProperties::prefersLeftSide() {
    auto o = qobject_cast<ClientDecoration*>(parent());
    if (o)
        return o->style()->prefersLeftSide();
    return false;
}

TrafficLightMinimizeButton::TrafficLightMinimizeButton(TrafficLights *parent)
    : TrafficLightButton(parent) {

}

void TrafficLightMinimizeButton::paint(QPainter *p) {
    trafficLights()->style()->paintElement(p, ClientDecorationStyle::BUTTON_MINIMIZE, boundingRect());
}

void TrafficLightMinimizeButton::activate() {
    window()->showMinimized();
}

TrafficLightMaximizeButton::TrafficLightMaximizeButton(TrafficLights *parent)
    : TrafficLightButton(parent) {

}

void TrafficLightMaximizeButton::paint(QPainter *p) {
    trafficLights()->style()->paintElement(p, ClientDecorationStyle::BUTTON_MAXIMIZE, boundingRect());
}

void TrafficLightMaximizeButton::activate() {
    if (window()->windowState() & Qt::WindowMaximized)
        window()->showNormal();
    else
        window()->showMaximized();
}

TrafficLightCloseButton::TrafficLightCloseButton(TrafficLights *parent)
    : TrafficLightButton(parent) {

}

void TrafficLightCloseButton::paint(QPainter *p) {
    trafficLights()->style()->paintElement(p, ClientDecorationStyle::BUTTON_CLOSE, boundingRect());
}

void TrafficLightCloseButton::activate() {
    window()->close();
}
