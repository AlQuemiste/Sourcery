#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTextEdit>
#include <QLabel>
#include <QApplication>
#include <QMainWindow>
#include <QPushButton>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);

  QMainWindow window;
  QWidget *widget = new QWidget(&window);
  QVBoxLayout *layout = new QVBoxLayout(widget);

  window.setCentralWidget(widget);
  widget->setLayout(layout);

  // Define QTextEdit
  QString s = "Your workspace will be converted into directory.";
  s.append("It could take a long time if there are\nmany data items."
	   "This added text");
  QTextEdit *message = new QTextEdit(widget);
  message->setReadOnly(true);
  message->setAcceptRichText(false);
  message->setDocumentTitle("Messages");
  message->setLineWrapColumnOrWidth(20);
  message->setFontFamily("Courier");
  message->setFontPointSize(14);
  message->setFontUnderline(true);
  const QColor textcolor0 {message->textColor()};
  const QColor textcolor {"Blue"};
  message->setTextColor(textcolor);
  message->insertPlainText(s);
  message->setFontUnderline(false);
  message->setTextColor(textcolor0);
  message->setStatusTip("STATUS");
  // END define QTextEdit

  layout->addWidget(message);

  layout->addWidget(new QPushButton("Set level", widget));
  layout->addWidget(new QPushButton("Save", widget));
  layout->addWidget(new QPushButton("Clear", widget));


  for (int i = 0; i < 10; ++i) {
    message->append("This is appended text.");
  }

  window.show();

  return a.exec();
}
