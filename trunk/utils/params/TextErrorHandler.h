#ifndef TextErrorHandler_h
#define TextErrorHandler_h

#include <qxml.h>

class TextErrorHandler : public QXmlErrorHandler
{
  typedef QXmlErrorHandler Super;

 public:

  virtual ~TextErrorHandler();

  virtual bool warning ( const QXmlParseException & exception );
  virtual bool error ( const QXmlParseException & exception );
  virtual bool fatalError ( const QXmlParseException & exception );
  virtual QString errorString ();

 protected:

  void createErrorMessage(const QString& type,
			  const QXmlParseException & exception);

  QString error_;
};

#endif
