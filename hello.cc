#include <v8.h>
#include <node.h>
#include <node_buffer.h>
#include <qrencode.h>

using namespace v8;
using namespace node;

Handle<Value> Method(const Arguments& args) {

  HandleScope scope;

  if (args.Length() < 1) {
    ThrowException(Exception::TypeError(String::New("Wrong number of arguments")));
    return scope.Close(Undefined());
  }

  Handle<Object> result = Object::New();

  QRcode *code = QRcode_encodeData(
      Buffer::Length(args[0]),
      const_cast<const unsigned char *>(
          reinterpret_cast<unsigned char *>(Buffer::Data(args[0]))),
      0, QR_ECLEVEL_H);

  result->Set(String::New("version"), Integer::New(code->version));
	result->Set(String::New("width"), Integer::New(code->width));
	result->Set(String::New("data"), Buffer::New(
				const_cast<const char *>(
            reinterpret_cast<char *>(code->data)),
				code->width * code->width)->handle_);

	QRcode_free(code);

  return scope.Close(result);

}

void init(Handle<Object> exports) {
  exports->Set(String::NewSymbol("hello"),
      FunctionTemplate::New(Method)->GetFunction());

  PropertyAttribute enumFieldAttr = static_cast<PropertyAttribute>(ReadOnly | DontDelete);

  exports->Set(String::NewSymbol("ECLEVEL_L"),
      Integer::New(QR_ECLEVEL_L), enumFieldAttr);
  exports->Set(String::NewSymbol("ECLEVEL_M"),
      Integer::New(QR_ECLEVEL_M), enumFieldAttr);
  exports->Set(String::NewSymbol("ECLEVEL_Q"),
      Integer::New(QR_ECLEVEL_Q), enumFieldAttr);
  exports->Set(String::NewSymbol("ECLEVEL_H"),
      Integer::New(QR_ECLEVEL_H), enumFieldAttr);

  exports->Set(String::NewSymbol("MODE_NUM"),
      Integer::New(QR_MODE_NUM), enumFieldAttr);
  exports->Set(String::NewSymbol("MODE_AN"),
      Integer::New(QR_MODE_AN), enumFieldAttr);
  exports->Set(String::NewSymbol("MODE_8"),
      Integer::New(QR_MODE_8), enumFieldAttr);
  exports->Set(String::NewSymbol("MODE_KANJI"),
      Integer::New(QR_MODE_KANJI), enumFieldAttr);
  exports->Set(String::NewSymbol("MODE_ECI"),
      Integer::New(QR_MODE_ECI), enumFieldAttr);
  exports->Set(String::NewSymbol("MODE_FNC1FIRST"),
      Integer::New(QR_MODE_FNC1FIRST), enumFieldAttr);
  exports->Set(String::NewSymbol("MODE_FNC1SECOND"),
      Integer::New(QR_MODE_FNC1SECOND), enumFieldAttr);

}

NODE_MODULE(hello, init)
