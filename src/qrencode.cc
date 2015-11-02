#include <errno.h>
#include <qrencode.h>
#include <node.h>
#include <nan.h>

using v8::Integer;
using v8::String;
using v8::Object;
using v8::Value;
using v8::Local;
using v8::Function;
using v8::FunctionTemplate;
using v8::PropertyAttribute;
using v8::ReadOnly;
using v8::DontDelete;

void Encode(const Nan::FunctionCallbackInfo<Value>& info) {

  if (info.Length() == 0) {
    Nan::ThrowTypeError("Data is required");
    info.GetReturnValue().SetUndefined();
    return;
  }

  int version = 0;
  QRcode *code = NULL;
  QRecLevel level = QR_ECLEVEL_M;

  if (info.Length() >= 2) {
    if (!info[1]->IsInt32()) {
      Nan::ThrowTypeError("Level must be ECLEVEL_L, ECLEVEL_M, ECLEVEL_Q or ECLEVEL_H");
      info.GetReturnValue().SetUndefined();
      return;
    }
    level = static_cast<QRecLevel>(info[1]->Int32Value());
  }

  if (info.Length() >= 3) {
    if (!info[2]->IsInt32()) {
      Nan::ThrowTypeError("Version must be int");
      info.GetReturnValue().SetUndefined();
      return;
    }
    version = info[2]->Int32Value();
  }

  if (node::Buffer::HasInstance(info[0])) {
    code = QRcode_encodeData(node::Buffer::Length(info[0]),
        (unsigned char*)node::Buffer::Data(info[0]), version, level);
  } else if (info[0]->IsString()) {
    String::Utf8Value str(info[0]);
    code = QRcode_encodeData(str.length(),
        (unsigned char*)*str, version, level);
  } else if (info[0]->IsObject()) {
    Nan::ThrowTypeError("Data must be buffer or string");
    info.GetReturnValue().SetUndefined();
    return;
  }

  if (!code) {
    char const *message = "Uknown Error";
    switch (errno) {
      case EINVAL:
        message = "Invalid input Object";
      break;
      case ENOMEM:
        message = "Unable to allocate memory for input";
      break;
      case ERANGE:
        message = "Input data is too large";
      break;
    }
    Nan::ThrowError(message);
    info.GetReturnValue().SetUndefined();
    return;
  }

  Local<Object> result = Nan::New<Object>();

  Nan::Set(result,Nan::New("version").ToLocalChecked(), Nan::New(code->version));
  Nan::Set(result,Nan::New("width").ToLocalChecked(), Nan::New(code->width));
  Nan::Set(result,Nan::New("data").ToLocalChecked(), Nan::CopyBuffer(
      (char *)code->data, code->width * code->width).ToLocalChecked());

  QRcode_free(code);

  info.GetReturnValue().Set(result);

}

NAN_MODULE_INIT(InitAll) {

  Nan::Set(target,Nan::New<String>("encode").ToLocalChecked(),
    Nan::GetFunction(Nan::New<FunctionTemplate>(Encode)).ToLocalChecked());

  PropertyAttribute enumFieldAttr =
      static_cast<PropertyAttribute>(ReadOnly | DontDelete);

#define DEFINE_ENUM(name, value) \
  Nan::Set(target,Nan::New(name).ToLocalChecked(), \
      Nan::New(value))

  DEFINE_ENUM("ECLEVEL_L", QR_ECLEVEL_L);
  DEFINE_ENUM("ECLEVEL_M", QR_ECLEVEL_M);
  DEFINE_ENUM("ECLEVEL_Q", QR_ECLEVEL_Q);
  DEFINE_ENUM("ECLEVEL_H", QR_ECLEVEL_H);

  DEFINE_ENUM("DOT_BLACK",    1 << 0);
  DEFINE_ENUM("DOT_DATA_ECC", 1 << 1);
  DEFINE_ENUM("DOT_FORMAT",   1 << 2);
  DEFINE_ENUM("DOT_VERSION",  1 << 3);
  DEFINE_ENUM("DOT_TIMING",   1 << 4);
  DEFINE_ENUM("DOT_ALIGN",    1 << 5);
  DEFINE_ENUM("DOT_FINDER",   1 << 6);
  DEFINE_ENUM("DOT_NON_DATA", 1 << 7);

#undef DEFINE_ENUM

}

NODE_MODULE(qrencode, InitAll)
