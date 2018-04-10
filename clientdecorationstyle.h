#ifndef CLIENTDECORATIONSTYLE_H
#define CLIENTDECORATIONSTYLE_H

#include <QObject>
#include <QPainter>

class ClientDecorationStyle;

class ClientDecorationStyleFactory {
public:
    static QStringList styles();
    static ClientDecorationStyle *create(QString name = QString());
};

class ClientDecorationStyle : public QObject {
    Q_OBJECT
    Q_PROPERTY(qreal titlePadding READ titlePadding WRITE setTitlePadding NOTIFY titlePaddingChanged)
    Q_PROPERTY(qreal titleMargins READ titleMargins WRITE setTitleMargins NOTIFY titleMarginsChanged)
    Q_PROPERTY(qreal buttonPadding READ buttonPadding WRITE setButtonPadding NOTIFY buttonPaddingChanged)
    Q_PROPERTY(qreal buttonMargins READ buttonMargins WRITE setButtonMargins NOTIFY buttonMarginsChanged)
    Q_PROPERTY(bool prefersLeftSide READ prefersLeftSide CONSTANT)
public:
    ClientDecorationStyle(QObject *parent = nullptr, bool prefersLeftSide = false);

    qreal titlePadding() const;
    qreal titleMargins() const;
    qreal buttonPadding() const;
    qreal buttonMargins() const;
    bool prefersLeftSide() const;

    void setTitlePadding(const qreal &val);
    void setTitleMargins(const qreal &val);
    void setButtonPadding(const qreal &val);
    void setButtonMargins(const qreal &val);

    enum Element {
        BACKGROUND,
        ICON,
        TITLE,
        BUTTON_MINIMIZE,
        BUTTON_MAXIMIZE,
        BUTTON_CLOSE
    };

    virtual void paintElement(QPainter *p, Element e, const QRectF &rect) = 0;
    virtual QSize sizeElement(Element e) = 0;

signals:
    void titlePaddingChanged();
    void titleMarginsChanged();
    void buttonPaddingChanged();
    void buttonMarginsChanged();

private:
    qreal m_titlePadding { 0 };
    qreal m_titleMargins { 0 };
    qreal m_buttonPadding { 0 };
    qreal m_buttonMargins { 0 };
    bool m_prefersLeftSide { false };
};

class WindowsClientDecorationStyle : public ClientDecorationStyle {
    Q_OBJECT
public:
    WindowsClientDecorationStyle(QObject *parent = nullptr);

    void paintElement(QPainter *p, Element e, const QRectF &rect) override;
    QSize sizeElement(Element e) override;

private:
    void paintBackground(QPainter *p, const QRectF &rect);
    void paintIcon(QPainter *p, const QRectF &rect);
    void paintTitle(QPainter *p, const QRectF &rect);

    void paintMinimizeButton(QPainter *p, const QRectF &rect);
    void paintMaximizeButton(QPainter *p, const QRectF &rect);
    void paintCloseButton(QPainter *p, const QRectF &rect);

    QSize sizeBackground(); // totally unnecessary
    QSize sizeIcon();
    QSize sizeTitle(); // probably unnecessary - gets what is left

    QSize sizeMinimizeButton();
    QSize sizeMaximizeButton();
    QSize sizeCloseButton();
};

class MacClientDecorationStyle : public ClientDecorationStyle {
    Q_OBJECT
public:
    MacClientDecorationStyle(QObject *parent = nullptr);

    void paintElement(QPainter *p, Element e, const QRectF &rect) override;
    QSize sizeElement(Element e) override;
};

class AdwaitaClientDecorationStyle : public ClientDecorationStyle {
    Q_OBJECT
public:
    AdwaitaClientDecorationStyle(QObject *parent = nullptr);

    void paintElement(QPainter *p, Element e, const QRectF &rect) override;
    QSize sizeElement(Element e) override;
};


#endif // CLIENTDECORATIONSTYLE_H
