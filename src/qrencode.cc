#include <errno.h>
#include <qrencode.h>
#include <node.h>
#include <nan.h>

using v8::Integer;
using v8::String;
using v8::Object;
using v8::Handle;
using v8::FunctionTemplate;
using v8::PropertyAttribute;
using v8::ReadOnly;
using v8::DontDelete;

NAN_METHOD(Encode) {

  NanScope();

  if (args.Length() == 0) {
    NanThrowTypeError("Data is required");
    NanReturnUndefined();
  }

  int version = 0;
  QRcode *code = NULL;
  QRecLevel level = QR_ECLEVEL_M;

  if (args.Length() >= 2) {
    if (!args[1]->IsInt32()) {
      NanThrowTypeError("Level must be ECLEVEL_L, ECLEVEL_M, ECLEVEL_Q or ECLEVEL_H");
      NanReturnUndefined();
    }
    level = static_cast<QRecLevel>(args[1]->Int32Value());
  }

  if (args.Length() >= 3) {
    if (!args[2]->IsInt32()) {
      NanThrowTypeError("Version must be int");
      NanReturnUndefined();
    }
    version = args[2]->Int32Value();
  }

  if (node::Buffer::HasInstance(args[0])) {
    code = QRcode_encodeData(node::Buffer::Length(args[0]),
        (unsigned char*)node::Buffer::Data(args[0]), version, level);
  } else if (args[0]->IsString()) {
    String::Utf8Value str(args[0]);
    code = QRcode_encodeData(str.length(),
        (unsigned char*)*str, version, level);
  } else if (args[0]->IsObject()) {
    NanThrowTypeError("Data must be buffer or string");
    NanReturnUndefined();
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
    NanThrowError(message);
    NanReturnUndefined();
  }

  Handle<Object> result = NanNew<Object>();

  result->Set(NanNew<String>("version"), NanNew<Integer>(code->version));
  result->Set(NanNew<String>("width"), NanNew<Integer>(code->width));
  result->Set(NanNew<String>("data"), NanNewBufferHandle(
      (char *)code->data, code->width * code->width));

  QRcode_free(code);

  NanReturnValue(result);

}

void InitAll(Handle<Object> exports) {

  exports->Set(NanNew<String>("encode"),
      NanNew<FunctionTemplate>(Encode)->GetFunction());

  PropertyAttribute enumFieldAttr =
      static_cast<PropertyAttribute>(ReadOnly | DontDelete);

#define DEFINE_ENUM(name, value) \
  exports->Set(NanNew<String>(name), \
      NanNew<Integer>(value), enumFieldAttr)

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
