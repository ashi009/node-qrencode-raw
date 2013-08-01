#include <errno.h>

#include <v8.h>
#include <node.h>
#include <node_buffer.h>
#include <qrencode.h>

#include <iostream>

using namespace v8;
using namespace node;

Handle<Value> HandleQRResult(QRcode *code, HandleScope &scope) {

  if (!code) {
    char const *message;
    switch (errno) {
      case EINVAL:
        message = "invalid input Object";
      break;
      case ENOMEM:
        message = "unable to allocate memory for input";
      break;
      case ERANGE:
        message = "input data is too large";
      break;
    }
    ThrowException(Exception::Error(String::New(message)));
    return scope.Close(Undefined());
  }

  Handle<Object> result = Object::New();

  result->Set(String::New("version"), Integer::New(code->version));
  result->Set(String::New("width"), Integer::New(code->width));
  result->Set(String::New("data"), Buffer::New(
      const_cast<const char *>(reinterpret_cast<char *>(code->data)),
      code->width * code->width)->handle_);

  QRcode_free(code);

  return scope.Close(result);

}

Handle<Value> EncodeBuffer(const Arguments& args) {

  HandleScope scope;

  if (args.Length() < 1) {
    ThrowException(Exception::TypeError(String::New("data is required")));
    return scope.Close(Undefined());
  }

  if (!Buffer::HasInstance(args[0])) {
    ThrowException(Exception::TypeError(String::New("data should be Buffer")));
    return scope.Close(Undefined());
  }

  int version = 0;
  QRecLevel level = QR_ECLEVEL_M;

  if (args.Length() == 2) {
    if (!args[1]->IsInt32()) {
      ThrowException(Exception::TypeError(String::New("version must be int")));
      return scope.Close(Undefined());
    }
    version = args[1]->Int32Value();
  }

  if (args.Length() == 3) {
    if (!args[2]->IsInt32()) {
      ThrowException(Exception::TypeError(String::New("level must be QRecLevel")));
      return scope.Close(Undefined());
    }
    level = static_cast<QRecLevel>(args[2]->Int32Value());
  }

  return HandleQRResult(QRcode_encodeData(
      Buffer::Length(args[0]),
      const_cast<const unsigned char *>(
          reinterpret_cast<unsigned char *>(Buffer::Data(args[0]))),
      version, level), scope);

}

Handle<Value> EncodeString(const Arguments& args) {

  HandleScope scope;

  if (args.Length() < 1) {
    ThrowException(Exception::TypeError(String::New("str is required")));
    return scope.Close(Undefined());
  }

  if (!args[0]->IsString()) {
    ThrowException(Exception::TypeError(String::New("str should be string")));
    return scope.Close(Undefined());
  }

  String::Utf8Value utf8_str(args[0]);

  char *str = new char[utf8_str.length() + 1];
  memcpy(str, *utf8_str, utf8_str.length());
  str[utf8_str.length()] = '\0';

  int version = 0;
  QRecLevel level = QR_ECLEVEL_M;
  int case_sensitive = 0;

  if (args.Length() == 2) {
    if (!args[1]->IsInt32()) {
      ThrowException(Exception::TypeError(String::New("version must be int")));
      return scope.Close(Undefined());
    }
    version = args[1]->Int32Value();
  }

  if (args.Length() == 3) {
    if (!args[2]->IsInt32()) {
      ThrowException(Exception::TypeError(String::New("level must be QRecLevel")));
      return scope.Close(Undefined());
    }
    level = static_cast<QRecLevel>(args[2]->Int32Value());
  }

  if (args.Length() == 4) {
    if (!args[3]->IsBoolean()) {
      ThrowException(Exception::TypeError(String::New("case_sensitive must be bool")));
      return scope.Close(Undefined());
    }
    case_sensitive = args[3]->BooleanValue() ? 1 : 0;
  }

  std::cout << str << " " << version << " " << level << " " << case_sensitive << std::endl;

  return HandleQRResult(
      QRcode_encodeString(str, version, level, QR_MODE_8, case_sensitive),
      scope);

}

void init(Handle<Object> exports) {

  exports->Set(String::NewSymbol("encodeBuffer"),
      FunctionTemplate::New(EncodeBuffer)->GetFunction());
  exports->Set(String::NewSymbol("encodeString"),
      FunctionTemplate::New(EncodeString)->GetFunction());

  PropertyAttribute enumFieldAttr = static_cast<PropertyAttribute>(ReadOnly | DontDelete);

  exports->Set(String::NewSymbol("ECLEVEL_L"),
      Integer::New(QR_ECLEVEL_L), enumFieldAttr);
  exports->Set(String::NewSymbol("ECLEVEL_M"),
      Integer::New(QR_ECLEVEL_M), enumFieldAttr);
  exports->Set(String::NewSymbol("ECLEVEL_Q"),
      Integer::New(QR_ECLEVEL_Q), enumFieldAttr);
  exports->Set(String::NewSymbol("ECLEVEL_H"),
      Integer::New(QR_ECLEVEL_H), enumFieldAttr);

}

NODE_MODULE(qrencode, init)
