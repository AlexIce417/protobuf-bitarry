#include "google/protobuf/compiler/rust/naming.h"

#include <string>
#include <vector>

#include <gtest/gtest.h>
#include "google/protobuf/compiler/rust/context.h"
#include "google/protobuf/descriptor.h"
#include "google/protobuf/io/zero_copy_stream_impl_lite.h"

using google::protobuf::compiler::rust::CamelToSnakeCase;
using google::protobuf::compiler::rust::Context;
using google::protobuf::compiler::rust::Kernel;
using google::protobuf::compiler::rust::Options;
using google::protobuf::compiler::rust::RustGeneratorContext;
using google::protobuf::compiler::rust::RustInternalModuleName;
using google::protobuf::compiler::rust::ScreamingSnakeToUpperCamelCase;
using google::protobuf::io::Printer;
using google::protobuf::io::StringOutputStream;

namespace {
TEST(RustProtoNaming, RustInternalModuleName) {
  google::protobuf::FileDescriptorProto foo_file;
  foo_file.set_name("strong_bad/lol.proto");
  google::protobuf::DescriptorPool pool;
  const google::protobuf::FileDescriptor* fd = pool.BuildFile(foo_file);

  const Options opts = {Kernel::kUpb};
  std::vector<const google::protobuf::FileDescriptor*> files{fd};
  const RustGeneratorContext rust_generator_context(&files);
  std::string output;
  StringOutputStream stream{&output};
  Printer printer(&stream);
  Context c = Context(&opts, &rust_generator_context, &printer);

  EXPECT_EQ(RustInternalModuleName(c, *fd), "strong__bad_slol");
}

TEST(RustProtoNaming, CamelToSnakeCase) {
  // TODO: Review this behavior.
  EXPECT_EQ(CamelToSnakeCase("CamelCase"), "camel_case");
  EXPECT_EQ(CamelToSnakeCase("_CamelCase"), "_camel_case");
  EXPECT_EQ(CamelToSnakeCase("camelCase"), "camel_case");
  EXPECT_EQ(CamelToSnakeCase("Number2020"), "number2020");
  EXPECT_EQ(CamelToSnakeCase("Number_2020"), "number_2020");
  EXPECT_EQ(CamelToSnakeCase("camelCase_"), "camel_case_");
  EXPECT_EQ(CamelToSnakeCase("CamelCaseTrio"), "camel_case_trio");
  EXPECT_EQ(CamelToSnakeCase("UnderIn_Middle"), "under_in_middle");
  EXPECT_EQ(CamelToSnakeCase("Camel_Case"), "camel_case");
  EXPECT_EQ(CamelToSnakeCase("Camel__Case"), "camel__case");
  EXPECT_EQ(CamelToSnakeCase("CAMEL_CASE"), "c_a_m_e_l_c_a_s_e");
}

TEST(RustProtoNaming, ScreamingSnakeToUpperCamelCase) {
  // TODO: Review this behavior.
  EXPECT_EQ(ScreamingSnakeToUpperCamelCase("CAMEL_CASE"), "CamelCase");
  EXPECT_EQ(ScreamingSnakeToUpperCamelCase("NUMBER2020"), "Number2020");
  EXPECT_EQ(ScreamingSnakeToUpperCamelCase("NUMBER_2020"), "Number2020");
  EXPECT_EQ(ScreamingSnakeToUpperCamelCase("FOO_4040_BAR"), "Foo4040Bar");
  EXPECT_EQ(ScreamingSnakeToUpperCamelCase("FOO_4040bar"), "Foo4040Bar");
  EXPECT_EQ(ScreamingSnakeToUpperCamelCase("_CAMEL_CASE"), "CamelCase");

  // This function doesn't currently preserve prefix/suffix underscore,
  // while CamelToSnakeCase does.
  EXPECT_EQ(ScreamingSnakeToUpperCamelCase("CAMEL_CASE_"), "CamelCase");
  EXPECT_EQ(ScreamingSnakeToUpperCamelCase("camel_case"), "CamelCase");
  EXPECT_EQ(ScreamingSnakeToUpperCamelCase("CAMEL_CASE_TRIO"), "CamelCaseTrio");
  EXPECT_EQ(ScreamingSnakeToUpperCamelCase("UNDER_IN__MIDDLE"),
            "UnderInMiddle");
}

}  // namespace
