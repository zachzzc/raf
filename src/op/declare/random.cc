/*!
 * Copyright (c) 2021 by Contributors
 * \file src/op/declare/random.cc
 * \brief Declaration of random operators
 */
#include "mnm/op.h"
#include "mnm/tensor.h"
#include "../schema/random.h"
namespace mnm {
namespace op {
namespace declare {

using namespace mnm::op::schema;
using namespace mnm::value;

MNM_OP_DECLARE("mnm.op.threefry_generate", [](const CallValues& call) {
  const auto* args = call->args.as<ThreefryGenerateArgs>();
  CHECK(args != nullptr);
  DLTensor* key = args->key;
  CHECK_EQ(tvm::runtime::DLDataType2String(key->dtype), "uint64")
      << "The type of key must be uint64";
  std::vector<int64_t> kshape(key->shape, key->shape + key->ndim);
  std::vector<int64_t> oshape(args->shape.begin(), args->shape.end());

  TensorValue new_key = TensorValue::Assemble(/*ctx=*/key->ctx,
                                              /*dtype=*/DType(DTypeCode::kUInt(), 64),
                                              /*shape=*/kshape);
  TensorValue random_array = TensorValue::Assemble(/*ctx=*/key->ctx,
                                                   /*dtype=*/DType(DTypeCode::kUInt(), 64),
                                                   /*shape=*/oshape);
  call->out = TupleValue::make({new_key, random_array});
  call->device = key->ctx;
}).set_attr<TOpPattern>("TOpPattern", kOpaque);

MNM_OP_DECLARE("mnm.op.threefry_split", [](const CallValues& call) {
  const auto* args = call->args.as<ThreefrySplitArgs>();
  CHECK(args != nullptr);
  DLTensor* key = args->key;
  CHECK_EQ(tvm::runtime::DLDataType2String(key->dtype), "uint64")
      << "The type of key must be uint64";
  std::vector<int64_t> kshape(key->shape, key->shape + key->ndim);

  TensorValue new_key = TensorValue::Assemble(/*ctx=*/key->ctx,
                                              /*dtype=*/key->dtype,
                                              /*shape=*/kshape);
  TensorValue new_subkey = TensorValue::Assemble(/*ctx=*/key->ctx,
                                                 /*dtype=*/key->dtype,
                                                 /*shape=*/kshape);
  call->out = TupleValue::make({new_key, new_subkey});
  call->device = key->ctx;
}).set_attr<TOpPattern>("TOpPattern", kOpaque);

}  // namespace declare
}  // namespace op
}  // namespace mnm
