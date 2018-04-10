#include "clientdecorationstyle.h"


QStringList ClientDecorationStyleFactory::styles() {
    return { "windows" };
}

ClientDecorationStyle *ClientDecorationStyleFactory::create(QString name) {
    return new WindowsClientDecorationStyle();
}

ClientDecorationStyle::ClientDecorationStyle(QObject *parent, bool prefersLeftSide)
    : QObject(parent)
    , m_prefersLeftSide(prefersLeftSide)
{

}

qreal ClientDecorationStyle::titlePadding() const {
    return m_titlePadding;
}

qreal ClientDecorationStyle::titleMargins() const {
    return m_titleMargins;
}

qreal ClientDecorationStyle::buttonPadding() const {
    return m_buttonPadding;
}

qreal ClientDecorationStyle::buttonMargins() const {
    return m_buttonMargins;
}

bool ClientDecorationStyle::prefersLeftSide() const {
    return m_prefersLeftSide;
}

void ClientDecorationStyle::setTitlePadding(const qreal &val) {
    if (m_titlePadding != val) {
        m_titlePadding = val;
        emit titlePaddingChanged();
    }
}

void ClientDecorationStyle::setTitleMargins(const qreal &val) {
    if (m_titleMargins != val) {
        m_titleMargins = val;
        emit titleMarginsChanged();
    }
}

void ClientDecorationStyle::setButtonPadding(const qreal &val) {
    if (m_buttonPadding != val) {
        m_buttonPadding = val;
        emit buttonPaddingChanged();
    }
}

void ClientDecorationStyle::setButtonMargins(const qreal &val) {
    if (m_buttonMargins != val) {
        m_buttonMargins = val;
        emit buttonMarginsChanged();
    }
}


WindowsClientDecorationStyle::WindowsClientDecorationStyle(QObject *parent)
    : ClientDecorationStyle(parent, false)
{
    Q_UNUSED(parent);
    setTitlePadding(6);
    setTitleMargins(6);
    setButtonMargins(0);
    setButtonPadding(0);
}

void WindowsClientDecorationStyle::paintElement(QPainter *p, ClientDecorationStyle::Element e, const QRectF &rect) {
    switch (e) {
    case BACKGROUND:
        paintBackground(p, rect);
        break;
    case TITLE:
        paintTitle(p, rect);
        break;
    case ICON:
        paintIcon(p, rect);
        break;
    case BUTTON_MINIMIZE:
        paintMinimizeButton(p, rect);
        break;
    case BUTTON_MAXIMIZE:
        paintMaximizeButton(p, rect);
        break;
    case BUTTON_CLOSE:
        paintCloseButton(p, rect);
        break;
    default:
        break;;
    }
}

QSize WindowsClientDecorationStyle::sizeElement(ClientDecorationStyle::Element e) {
    switch (e) {
    case BACKGROUND:
        return sizeBackground();
    case TITLE:
        return sizeTitle();
    case ICON:
        return sizeIcon();
    case BUTTON_MINIMIZE:
        return sizeMinimizeButton();
    case BUTTON_MAXIMIZE:
        return sizeMaximizeButton();
    case BUTTON_CLOSE:
        return sizeCloseButton();
    default:
        return QSize();
    }
}

void WindowsClientDecorationStyle::paintBackground(QPainter *p, const QRectF &rect) {
    p->save();
    p->setPen(Qt::darkGray);
    //p->setBrush(QBrush(windowActive ? activeBackgroundColor : inactiveBackgroundColor));
    p->setBrush(Qt::white);
    p->setClipRect(rect);
    p->drawRect(rect.adjusted(0, 0, 0, 1));
    p->restore();
}

void WindowsClientDecorationStyle::paintIcon(QPainter *p, const QRectF &rect) {
    /*
    if (!icon.isNull()) {
        QGuiApplication::windowIcon().paint(p, iconRect.toRect());
    }
    */
    p->setPen(Qt::blue);
    p->setBrush(Qt::blue);
    p->drawRect(rect);
}

void WindowsClientDecorationStyle::paintTitle(QPainter *p, const QRectF &rect) {
    //p->setPen(windowActive ? activeTextColor : inactiveTextColor);
    p->setPen(Qt::black);
    p->setFont(QFont("sans", 8));
    // TODO
    //p->drawText(rect, Qt::AlignVCenter, QGuiApplication::applicationDisplayName());
    p->drawText(rect, Qt::AlignVCenter, "Very Long Application Name");
}

void WindowsClientDecorationStyle::paintMinimizeButton(QPainter *p, const QRectF &rect) {
    p->save();
    p->setPen(Qt::NoPen);
    //p->setBrush(QColor("#88ffffff"));
    p->setBrush(Qt::transparent);
    p->drawRect(rect);
    p->setPen(Qt::black);
    p->drawLine(rect.left() + 18, rect.center().y(), rect.left() + rect.width() - 17, rect.center().y());
    p->restore();
}

void WindowsClientDecorationStyle::paintMaximizeButton(QPainter *p, const QRectF &rect) {
    p->save();
    p->setPen(Qt::NoPen);
    //p->setBrush(QColor("#88ffffff"));
    p->setBrush(Qt::transparent);
    p->drawRect(rect);
    p->setPen(Qt::black);
    p->setBrush(Qt::transparent);
    p->drawRect(rect.adjusted(18, 11, -17, -12));
    p->restore();
}

void WindowsClientDecorationStyle::paintCloseButton(QPainter *p, const QRectF &rect) {
    p->save();
    p->setPen(Qt::NoPen);
    p->setBrush(Qt::red);
    p->drawRect(rect);

    p->setRenderHint(QPainter::Antialiasing, true);
    p->setPen(QPen(Qt::black, 1.1));
    p->drawLine(rect.left() + 20, rect.bottom() - 11, rect.left() + rect.width() - 19, rect.top() + 12);
    p->drawLine(rect.left() + 20, rect.top() + 12, rect.left() + rect.width() - 19, rect.bottom() - 11);
    p->restore();
}

QSize WindowsClientDecorationStyle::sizeBackground() {
    return QSize();
}

QSize WindowsClientDecorationStyle::sizeIcon() {
    return QSize(16, 16);
}

QSize WindowsClientDecorationStyle::sizeTitle() {
    return QSize();
}

QSize WindowsClientDecorationStyle::sizeMinimizeButton() {
    return { 44, 31 };
}

QSize WindowsClientDecorationStyle::sizeMaximizeButton() {
    return { 44, 31 };
}

QSize WindowsClientDecorationStyle::sizeCloseButton() {
    return { 48, 31 };
}

MacClientDecorationStyle::MacClientDecorationStyle(QObject *parent)
    : ClientDecorationStyle(parent) {

}

void MacClientDecorationStyle::paintElement(QPainter *p, ClientDecorationStyle::Element e, const QRectF &rect) {

}

QSize MacClientDecorationStyle::sizeElement(ClientDecorationStyle::Element e) {

}

AdwaitaClientDecorationStyle::AdwaitaClientDecorationStyle(QObject *parent)
    : ClientDecorationStyle(parent) {

}

void AdwaitaClientDecorationStyle::paintElement(QPainter *p, ClientDecorationStyle::Element e, const QRectF &rect) {

}

QSize AdwaitaClientDecorationStyle::sizeElement(ClientDecorationStyle::Element e) {

}
