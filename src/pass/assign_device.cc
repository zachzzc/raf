/*!
 * Copyright (c) 2021 by Contributors
 * \file assign_device.cc
 * \brief Assign the target device to init and constant ops.
 */
#include "mnm/op.h"
#include "mnm/ir.h"

#include "mnm/pass.h"
#include "tvm/relay/expr.h"
#include "tvm/relay/op_attr_types.h"
#include "../op/schema/init.h"
#include "../op/schema/transform.h"
#include "../op/schema/nn.h"

namespace mnm {
namespace pass {
namespace assign_device {

using namespace mnm::ir;
using namespace mnm::op;
using namespace mnm::op::schema;
using namespace mnm::value;

mnm::Device GetDeviceFromConstExpr(const Expr& expr) {
  static auto* str2dev = tvm::runtime::Registry::Get("mnm._core.core_utils.str2dev");
  auto device_name_node = expr.as<ir::ConstantNode>();
  CHECK(device_name_node);
  auto device_name_string_obj = device_name_node->value.as<StringValueObj>();
  CHECK(device_name_string_obj);
  std::string device_name_str = device_name_string_obj->value;
  return Device(static_cast<tvm::Device>((*str2dev)(device_name_str)));
}

Expr AssignDeviceFullOp(const CallNode* node, const Array<Expr> args,
                        std::string target_device_str) {
  Array<Expr> new_args;

  // Get the device of the current node.
  const auto* str2dev = tvm::runtime::Registry::Get("mnm._core.core_utils.str2dev");
  Device call_device;
  if (node->args.size() < 4) {
    call_device = Device(static_cast<tvm::Device>((*str2dev)("cpu")));
  } else {
    call_device = GetDeviceFromConstExpr(node->args[3]);
  }

  // Get the target device.
  Device target_device = Device(static_cast<tvm::Device>((*str2dev)(target_device_str)));

  // Current node is not on the desired device, adjust the device argument.
  if (target_device.device_type != call_device.device_type) {
    size_t arg_idx = 0;
    // Do nothing with required arguments.
    for (; arg_idx < 2; ++arg_idx) {
      new_args.push_back(args[arg_idx]);
    }

    // Process optional arguments.
    for (; arg_idx < 4; ++arg_idx) {
      if (arg_idx == 3) {
        // Set/Override the target device.
        new_args.push_back(MakeConstant(StringValue::make(target_device_str)));
      } else if (arg_idx < node->args.size()) {
        // Optional argument is specified.
        new_args.push_back(args[arg_idx]);
      } else if (arg_idx == 2) {
        // Make up the default argument value for dtype.
        new_args.push_back(MakeConstant(StringValue::make("int")));
      }
    }
  } else {
    new_args = args;
  }
  return Call(node->op, new_args);
}

Expr AssignDeviceOneHotOp(const CallNode* node, const Array<Expr> args,
                          std::string target_device_str) {
  Array<Expr> new_args;

  // Get the device of the current node.
  const auto* str2dev = tvm::runtime::Registry::Get("mnm._core.core_utils.str2dev");
  Device call_device;
  if (node->args.size() < 7) {
    call_device = Device(static_cast<tvm::Device>((*str2dev)("cpu")));
  } else {
    call_device = GetDeviceFromConstExpr(node->args[6]);
  }

  // Get the target device.
  Device target_device = Device(static_cast<tvm::Device>((*str2dev)(target_device_str)));

  // Current node is not on the desired device, adjust the device argument.
  if (target_device.device_type != call_device.device_type) {
    size_t arg_idx = 0;
    // Do nothing with required arguments.
    for (; arg_idx < 4; ++arg_idx) {
      new_args.push_back(args[arg_idx]);
    }

    // Process optional arguments.
    for (; arg_idx < 7; ++arg_idx) {
      if (arg_idx == 6) {
        // Set/Override the target device.
        new_args.push_back(MakeConstant(StringValue::make(target_device_str)));
      } else if (arg_idx < node->args.size()) {
        // Optional argument is specified.
        new_args.push_back(args[arg_idx]);
      } else if (arg_idx == 4) {
        // Make up the default argument value for axis.
        new_args.push_back(MakeConstant(ScalarValue::make(-1)));
      } else if (arg_idx == 5) {
        // Make up the default argument value for dtype.
        new_args.push_back(MakeConstant(StringValue::make("int")));
      }
    }
  } else {
    new_args = args;
  }
  return Call(node->op, new_args);
}

Expr AssignDeviceInitOp(const CallNode* node, const Array<Expr> args,
                        std::string target_device_str) {
  Array<Expr> new_args;

  // Get the device of the current node.
  const auto* str2dev = tvm::runtime::Registry::Get("mnm._core.core_utils.str2dev");
  Device call_device;
  if (node->args.size() < 3) {
    call_device = Device(static_cast<tvm::Device>((*str2dev)("cpu")));
  } else {
    call_device = GetDeviceFromConstExpr(node->args[2]);
  }

  // Get the target device.
  Device target_device = Device(static_cast<tvm::Device>((*str2dev)(target_device_str)));

  // Current node is not on the desired device, adjust the device argument.
  if (target_device.device_type != call_device.device_type) {
    size_t arg_idx = 0;
    // Do nothing with required arguments.
    for (; arg_idx < 1; ++arg_idx) {
      new_args.push_back(args[arg_idx]);
    }

    // Process optional arguments.
    for (; arg_idx < 3; ++arg_idx) {
      if (arg_idx == 2) {
        // Set/Override the target device.
        new_args.push_back(MakeConstant(StringValue::make(target_device_str)));
      } else if (arg_idx < node->args.size()) {
        // Optional argument is specified.
        new_args.push_back(args[arg_idx]);
      } else if (arg_idx == 1) {
        // Make up the default argument value for dtype.
        new_args.push_back(MakeConstant(StringValue::make("float")));
      }
    }
  } else {
    new_args = args;
  }
  return Call(node->op, new_args);
}

Expr AssignDeviceArangeOp(const CallNode* node, const Array<Expr> args,
                          std::string target_device_str) {
  Array<Expr> new_args;

  // Get the device of the current node.
  const auto* str2dev = tvm::runtime::Registry::Get("mnm._core.core_utils.str2dev");
  Device call_device;
  if (node->args.size() < 5) {
    call_device = Device(static_cast<tvm::Device>((*str2dev)("cpu")));
  } else {
    call_device = GetDeviceFromConstExpr(node->args[2]);
  }

  // Get the target device.
  Device target_device = Device(static_cast<tvm::Device>((*str2dev)(target_device_str)));

  // Current node is not on the desired device, adjust the device argument.
  if (target_device.device_type != call_device.device_type) {
    size_t arg_idx = 0;
    // Do nothing with required arguments.
    for (; arg_idx < 3; ++arg_idx) {
      new_args.push_back(args[arg_idx]);
    }

    // Process optional arguments.
    for (; arg_idx < 5; ++arg_idx) {
      if (arg_idx == 4) {
        // Set/Override the target device.
        new_args.push_back(MakeConstant(StringValue::make(target_device_str)));
      } else if (arg_idx < node->args.size()) {
        // Optional argument is specified.
        new_args.push_back(args[arg_idx]);
      } else if (arg_idx == 3) {
        // Make up the default argument value for dtype.
        new_args.push_back(MakeConstant(StringValue::make("int")));
      }
    }
  } else {
    new_args = args;
  }
  return Call(node->op, new_args);
}

typedef Expr (*AssignDeviceOpFuncType)(const CallNode* node, const Array<Expr> args,
                                       std::string target_device);
std::unordered_map<String, AssignDeviceOpFuncType> fmap = {
    {"mnm.op.full", &AssignDeviceFullOp},
    {"mnm.op.one_hot", &AssignDeviceOneHotOp},
    {"mnm.op.zeros", &AssignDeviceInitOp},
    {"mnm.op.ones", &AssignDeviceInitOp},
    {"mnm.op.arange", &AssignDeviceArangeOp}};

class DeviceAssigner : public ExprMutator {
 public:
  DeviceAssigner(std::string device) : device_str_(device){};

  Expr VisitExpr_(const RelayConstantNode* node) final {
    auto value = Downcast<Value>(ConstantExtractValue(GetRef<Constant>(node)));

    // Only focus on constant tensor.
    if (value.as<TensorValueObj>()) {
      DLTensor* dlt = value;

      const auto* str2dev = tvm::runtime::Registry::Get("mnm._core.core_utils.str2dev");
      tvm::Device target_tvm_ctx = (*str2dev)(device_str_);
      Device target_device = Device(target_tvm_ctx);

      // Do nothing if the constant is already on the target device.
      if (target_tvm_ctx.device_type == dlt->device.device_type) {
        return GetRef<Expr>(node);
      }

      std::vector<int64_t> shape;
      DType dtype = DType(DLDataType(dlt->dtype));
      for (auto i = 0; i < dlt->ndim; ++i) {
        shape.push_back(dlt->shape[i]);
      }

      auto array = tvm::runtime::NDArray::Empty(shape, dtype, target_device);

      // Move tensor to the target device.
      array.CopyFrom(dlt);
      auto tv = TensorValue::Assemble(target_device, dtype, shape);
      tv->tensor = std::move(array);
      return MakeConstant(tv);
    }
    return GetRef<Expr>(node);
  }

  Expr VisitExpr_(const CallNode* node) final {
    if (node->op.as<OpNode>() == nullptr) {
      return ExprMutator::VisitExpr_(node);
    }

    const Op& node_op = Downcast<Op>(node->op);
    CHECK(node_op.defined());

    if (fmap.count(node_op->name) != 0) {
      Array<Expr> visited_args;
      for (auto arg : node->args) {
        visited_args.push_back(this->Mutate(arg));
      }
      return (*fmap[node_op->name])(node, visited_args, device_str_);
    }
    return ExprMutator::VisitExpr_(node);
  }

 private:
  /*! \brief The target device string. */
  std::string device_str_;
};
}  // namespace assign_device

Pass AssignDevice(std::string device) {
  runtime::TypedPackedFunc<Function(Function, IRModule, PassContext)> pass_func =
      [=](Function f, IRModule m, PassContext pc) {
        auto assigner = assign_device::DeviceAssigner(device);
        return Downcast<Function>(assigner.Mutate(f));
      };
  return CreateMNMFunctionPass(pass_func, 0, "AssignDevice", {});
}

MNM_REGISTER_GLOBAL("mnm.pass_.AssignDevice").set_body_typed(AssignDevice);

}  // namespace pass
}  // namespace mnm
