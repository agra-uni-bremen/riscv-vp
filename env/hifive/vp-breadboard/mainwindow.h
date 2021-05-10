#pragma once
#include <QMainWindow>
#include <cassert>

#include <gpio/gpio-client.hpp>
#include <oled/common.hpp>

namespace Ui {
class VPBreadboard;
}

static constexpr unsigned max_num_buttons = 7;

struct Sevensegment {
	QPoint offs;
	QPoint extent;
	uint8_t linewidth;
	uint8_t map;
	void draw(QPainter& p);
	Sevensegment() : offs(50, 50), extent(100, 100), linewidth(10), map(0){};
	Sevensegment(QPoint offs, QPoint extent, uint8_t linewidth)
	    : offs(offs), extent(extent), linewidth(linewidth), map(0){};
};

struct RGBLed {
	QPoint offs;
	uint8_t linewidth;
	uint8_t map;
	void draw(QPainter& p);
	RGBLed(QPoint offs, uint8_t linewidth) : offs(offs), linewidth(linewidth){};
};

struct OLED
{
	ss1106::State* state;
	QPoint offs;
	QPoint margin;
	QImage image;
	float scale;
	void draw(QPainter& p);
	OLED(QPoint offs, unsigned margin, float scale = 1) : offs(offs),
			margin(QPoint(margin, margin)), scale(scale),
			image(ss1106::width - 2*ss1106::padding_lr, ss1106::height, QImage::Format_Grayscale8)
	{
		state = reinterpret_cast<ss1106::State*>(ss1106::getSharedState());
		state->changed = 1;
	};
};

struct Button
{
	QRect area;
	uint8_t pin;
	QKeySequence keybinding;
	QString name;
	bool pressed;
Button(QRect area, uint8_t pin, QKeySequence keybinding, QString name = "") :
	area(area), pin(pin), keybinding(keybinding), name(name),
		pressed(false){};
};

class VPBreadboard : public QWidget {
	Q_OBJECT
	GpioClient gpio;
	Sevensegment* sevensegment;
	RGBLed* rgbLed;
	OLED* oled;
	Button* buttons[max_num_buttons];
	const char* host;
	const char* port;

	bool debugmode = false;
	unsigned moving_button = 0;
	bool inited = false;

	uint64_t translateGpioToExtPin(GpioCommon::Reg reg);
	uint8_t translatePinNumberToSevensegment(uint64_t pinmap);
	uint8_t translatePinNumberToRGBLed(uint64_t pinmap);
	uint8_t translatePinToGpioOffs(uint8_t pin);

   public:
	VPBreadboard(const char* configfile, const char* host, const char* port, QWidget* mparent = 0);
	~VPBreadboard();
	void showConnectionErrorOverlay(QPainter& p);
	void paintEvent(QPaintEvent*) override;
	void keyPressEvent(QKeyEvent* e) override;
	void keyReleaseEvent(QKeyEvent* e) override;
	void mousePressEvent(QMouseEvent* e) override;
	void mouseReleaseEvent(QMouseEvent* e) override;

	void notifyChange(bool success);
};
