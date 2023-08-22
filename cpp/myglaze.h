#ifndef MYGLAZE
#define MYGLAZE

#include "data-structs.h"
#include <glaze/glaze.hpp>

template <>
struct glz::meta<BatchOpRequest> {
  using T = BatchOpRequest;
  static constexpr auto value = object("operations", &T::Operations);
};

template <>
struct glz::meta<BatchSubOp> {
  using T = BatchSubOp;
  static constexpr auto value = object("method", &T::Method, "relative-url",
                                       &T::RelativeURL, "body", &T::Body);
};

template <>
struct glz::meta<PKReadBody> {
  using T = PKReadBody;
  static constexpr auto value =
      object("filters", &T::Filters, "readColumns", &T::ReadColumns,
             "operationId", &T::OperationID);
};

template <>
struct glz::meta<Filter> {
  using T = Filter;
  static constexpr auto value =
      object("column", &T::Column, "value", &T::Value);
};

template <>
struct glz::meta<ReadColumn> {
  using T = ReadColumn;
  static constexpr auto value =
      object("column", &T::Column, "dataReturnType", &T::DataReturnType);
};
void glazeParse(BatchOpRequest *req);
#endif
