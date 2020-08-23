/**
 * Copyright 2019-2020 Huawei Technologies Co., Ltd
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "src/ops/depthwise_conv2d.h"
#include <memory>
#include <string>
#ifdef PRIMITIVE_WRITEABLE
#include "tools/converter/quantizer/quantize_util.h"
#endif
namespace mindspore {
namespace lite {
#ifdef PRIMITIVE_WRITEABLE
int DepthwiseConv2D::GetFormat() const { return this->primitive_->value.AsDepthwiseConv2D()->format; }
int DepthwiseConv2D::GetChannelIn() const { return this->primitive_->value.AsDepthwiseConv2D()->channelIn; }
int DepthwiseConv2D::GetChannelMultiplier() const {
  return this->primitive_->value.AsDepthwiseConv2D()->channelMultiplier;
}
int DepthwiseConv2D::GetKernelW() const { return this->primitive_->value.AsDepthwiseConv2D()->kernelW; }
int DepthwiseConv2D::GetKernelH() const { return this->primitive_->value.AsDepthwiseConv2D()->kernelH; }
int DepthwiseConv2D::GetStrideW() const { return this->primitive_->value.AsDepthwiseConv2D()->strideW; }
int DepthwiseConv2D::GetStrideH() const { return this->primitive_->value.AsDepthwiseConv2D()->strideH; }
int DepthwiseConv2D::GetPadMode() const { return this->primitive_->value.AsDepthwiseConv2D()->padMode; }
int DepthwiseConv2D::GetPadUp() const { return this->primitive_->value.AsDepthwiseConv2D()->padUp; }
int DepthwiseConv2D::GetPadDown() const { return this->primitive_->value.AsDepthwiseConv2D()->padDown; }
int DepthwiseConv2D::GetPadLeft() const { return this->primitive_->value.AsDepthwiseConv2D()->padLeft; }
int DepthwiseConv2D::GetPadRight() const { return this->primitive_->value.AsDepthwiseConv2D()->padRight; }
int DepthwiseConv2D::GetDilateW() const { return this->primitive_->value.AsDepthwiseConv2D()->dilateW; }
int DepthwiseConv2D::GetDilateH() const { return this->primitive_->value.AsDepthwiseConv2D()->dilateH; }
bool DepthwiseConv2D::GetHasBias() const { return this->primitive_->value.AsDepthwiseConv2D()->hasBias; }
int DepthwiseConv2D::GetActivationType() const { return this->primitive_->value.AsDepthwiseConv2D()->activationType; }

void DepthwiseConv2D::SetFormat(int format) {
  this->primitive_->value.AsDepthwiseConv2D()->format = (schema::Format)format;
}
void DepthwiseConv2D::SetChannelIn(int channel_in) {
  this->primitive_->value.AsDepthwiseConv2D()->channelIn = channel_in;
}
void DepthwiseConv2D::SetChannelMultiplier(int channel_multiplier) {
  this->primitive_->value.AsDepthwiseConv2D()->channelMultiplier = channel_multiplier;
}
void DepthwiseConv2D::SetKernelW(int kernel_w) { this->primitive_->value.AsDepthwiseConv2D()->kernelW = kernel_w; }
void DepthwiseConv2D::SetKernelH(int kernel_h) { this->primitive_->value.AsDepthwiseConv2D()->kernelH = kernel_h; }
void DepthwiseConv2D::SetStrideW(int stride_w) { this->primitive_->value.AsDepthwiseConv2D()->strideW = stride_w; }
void DepthwiseConv2D::SetStrideH(int stride_h) { this->primitive_->value.AsDepthwiseConv2D()->strideH = stride_h; }
void DepthwiseConv2D::SetPadMode(int pad_mode) {
  this->primitive_->value.AsDepthwiseConv2D()->padMode = (schema::PadMode)pad_mode;
}
void DepthwiseConv2D::SetPadUp(int pad_up) { this->primitive_->value.AsDepthwiseConv2D()->padUp = pad_up; }
void DepthwiseConv2D::SetPadDown(int pad_down) { this->primitive_->value.AsDepthwiseConv2D()->padDown = pad_down; }
void DepthwiseConv2D::SetPadLeft(int pad_left) { this->primitive_->value.AsDepthwiseConv2D()->padLeft = pad_left; }
void DepthwiseConv2D::SetPadRight(int pad_right) { this->primitive_->value.AsDepthwiseConv2D()->padRight = pad_right; }
void DepthwiseConv2D::SetDilateW(int dilate_w) { this->primitive_->value.AsDepthwiseConv2D()->dilateW = dilate_w; }
void DepthwiseConv2D::SetDilateH(int dilate_h) { this->primitive_->value.AsDepthwiseConv2D()->dilateH = dilate_h; }
void DepthwiseConv2D::SetHasBias(bool has_bias) { this->primitive_->value.AsDepthwiseConv2D()->hasBias = has_bias; }
void DepthwiseConv2D::SetActivationType(int activation_type) {
  this->primitive_->value.AsDepthwiseConv2D()->activationType = (schema::ActivationType)activation_type;
}

void DepthwiseConv2D::CalQuantParam(const double &mean, const double &stdDev, float *mMin, float *mMax) {
  constexpr float qmin = 0;
  constexpr float qmax = 255;
  *mMin = static_cast<float>((qmin - mean) / stdDev);
  *mMax = static_cast<float>((qmax - mean) / stdDev);
}

void DepthwiseConv2D::PopulaterQuantParam(const Primitive &prim,
                                          std::vector<std::vector<schema::QuantParamT>> *vecInputQuantParam,
                                          std::vector<std::vector<schema::QuantParamT>> *vecOutputQuantParam) {
  auto narrow_range = prim.GetAttr("narrow_range");
  bool narrowRangeQuantParam = GetValue<bool>(narrow_range);
  auto num_bits = prim.GetAttr("num_bits");
  int32_t numbitsRangeQuantParam = GetValue<int32_t>(num_bits);

  std::vector<schema::QuantParamT> quants;
  schema::QuantParamT quantParam;
  auto mean = prim.GetAttr("mean");
  auto std_dev = prim.GetAttr("std_dev");
  if (mean != nullptr && std_dev != nullptr) {
    auto meanQuantOaram = GetValue<double>(mean);
    double stddevQuantOaram = GetValue<double>(std_dev);
    float mMin = 0.0;
    float mMax = 0.0;
    CalQuantParam(meanQuantOaram, stddevQuantOaram, &mMin, &mMax);
    quantParam.min = mMin;
    quantParam.max = mMax;
  } else {
    auto inputMin = prim.GetAttr("input_minq");
    auto inputMax = prim.GetAttr("input_maxq");
    auto inputMinPtr = inputMin->cast<lite::tensor::TensorPtr>();
    auto inputMaxPtr = inputMax->cast<lite::tensor::TensorPtr>();
    float *minBuf = static_cast<float *>(inputMinPtr->Data());
    float *maxBuf = static_cast<float *>(inputMaxPtr->Data());
    quantParam.min = *minBuf;
    quantParam.max = *maxBuf;
  }
  quant::CalQuantizationParams(&quantParam, quantParam.min, quantParam.max, narrowRangeQuantParam,
                               numbitsRangeQuantParam);
  quants.emplace_back(quantParam);
  vecInputQuantParam->emplace_back(quants);

  quants.clear();
  int biasQuantSize = 0;
  auto filterMin = prim.GetAttr("filter_minq");
  auto filterMax = prim.GetAttr("filter_maxq");
  if (filterMin != nullptr && filterMax != nullptr) {
    auto filterMinPtr = filterMin->cast<lite::tensor::TensorPtr>();
    auto filterMaxPtr = filterMax->cast<lite::tensor::TensorPtr>();
    float *minBuf = static_cast<float *>(filterMinPtr->Data());
    float *maxBuf = static_cast<float *>(filterMaxPtr->Data());
    biasQuantSize = filterMinPtr->DataSize();
    for (int i = 0; i < biasQuantSize; ++i) {
      quantParam.min = *(minBuf++);
      quantParam.max = *(maxBuf++);
      quant::CalQuantizationParams(&quantParam, quantParam.min, quantParam.max, narrowRangeQuantParam,
                                   numbitsRangeQuantParam);
      quants.emplace_back(quantParam);
    }
    vecInputQuantParam->emplace_back(quants);
  }

  quants.clear();
  for (int i = 0; i < biasQuantSize; ++i) {
    quantParam.min = 0.0;
    quantParam.max = 0.0;
    quantParam.zeroPoint = 0;

    quantParam.scale = vecInputQuantParam->at(0).at(0).scale * vecInputQuantParam->at(1).at(i).scale;
    quants.emplace_back(quantParam);
  }
  vecInputQuantParam->emplace_back(quants);

  quants.clear();
  auto outputMin = prim.GetAttr("output_minq");
  auto outputMax = prim.GetAttr("output_maxq");
  if (outputMin != nullptr && outputMax != nullptr) {
    auto outputMinPtr = outputMin->cast<lite::tensor::TensorPtr>();
    auto outputMaxPtr = outputMax->cast<lite::tensor::TensorPtr>();
    float *minBuf = static_cast<float *>(outputMinPtr->Data());
    float *maxBuf = static_cast<float *>(outputMaxPtr->Data());
    quantParam.min = *minBuf;
    quantParam.max = *maxBuf;
    quant::CalQuantizationParams(&quantParam, quantParam.min, quantParam.max, narrowRangeQuantParam,
                                 numbitsRangeQuantParam);
    quants.emplace_back(quantParam);
    vecOutputQuantParam->emplace_back(quants);
  }
}

int DepthwiseConv2D::UnPackAttr(const Primitive &prim, const std::vector<AnfNodePtr> &inputs) {
  this->primitive_ = new (schema::PrimitiveT);
  auto attr = std::make_unique<schema::DepthwiseConv2DT>();

  auto format = GetValue<std::string>(prim.GetAttr("data_format"));
  if (format == "NCHW") {
    attr->format = schema::Format_NCHW;
  } else if (format == "NHWC") {
    attr->format = schema::Format_NHWC;
  } else {
    attr->format = schema::Format_NUM_OF_FORMAT;
  }
  auto pad_list = GetValue<std::vector<int>>(prim.GetAttr("pads"));
  attr->padUp = pad_list[0];
  attr->padDown = pad_list[1];
  attr->padLeft = pad_list[2];
  attr->padRight = pad_list[3];

  auto dilation = GetValue<std::vector<int>>(prim.GetAttr("dilation"));
  attr->dilateH = dilation[0];
  attr->dilateW = dilation[1];

  auto kernel_size = GetValue<std::vector<int>>(prim.GetAttr("kernel_size"));
  attr->kernelH = kernel_size[0];
  attr->kernelW = kernel_size[1];

  auto stride = GetValue<std::vector<int>>(prim.GetAttr("stride"));
  attr->strideH = stride[2];
  attr->strideW = stride[3];

  auto pad_mode = GetValue<std::string>(prim.GetAttr("pad_mode"));
  if (pad_mode == "valid") {
    attr->padMode = schema::PadMode_VALID;
  } else if (pad_mode == "same") {
    attr->padMode = schema::PadMode_SAME;
  } else {
    attr->padMode = schema::PadMode_NOTSET;
  }

  auto channel_multiplier = GetValue<int>(prim.GetAttr("channel_multiplier"));
  attr->channelMultiplier = channel_multiplier;

  MS_ASSERT(inputs.size() == kAnfPopulaterTwo);
  auto inputNode = inputs[kAnfPopulaterOne];
  MS_ASSERT(inputNode != nullptr);
  if (inputNode->isa<Parameter>()) {
    auto paramNode = inputNode->cast<ParameterPtr>();
    auto abstractBase = paramNode->abstract();
    MS_ASSERT(abstractBase != nullptr);
    if (utils::isa<abstract::AbstractTensorPtr>(abstractBase)) {
      auto abstractTensor = utils::cast<abstract::AbstractTensorPtr>(abstractBase);
      MS_ASSERT(abstractTensor != nullptr);
      if (utils::isa<abstract::ShapePtr>(abstractTensor->BuildShape())) {
        auto dims = utils::cast<abstract::ShapePtr>(abstractTensor->BuildShape())->shape();
        attr->channelIn = dims[kAnfPopulaterOne];
      }
    }
  }

  this->primitive_->value.type = schema::PrimitiveType_DepthwiseConv2D;
  this->primitive_->value.value = attr.release();

  if (GetQuantType() == schema::QuantType_AwareTraining) {
    std::vector<std::vector<schema::QuantParamT>> vecInputQuantParam;
    std::vector<std::vector<schema::QuantParamT>> vecOutputQuantParam;
    PopulaterQuantParam(prim, &vecInputQuantParam, &vecOutputQuantParam);
    SetInputQuantParam(vecInputQuantParam);
    SetOutputQuantParam(vecOutputQuantParam);
  }
  return RET_OK;
}

#else

int DepthwiseConv2D::GetFormat() const { return this->primitive_->value_as_DepthwiseConv2D()->format(); }
int DepthwiseConv2D::GetChannelIn() const { return this->primitive_->value_as_DepthwiseConv2D()->channelIn(); }
int DepthwiseConv2D::GetChannelMultiplier() const {
  return this->primitive_->value_as_DepthwiseConv2D()->channelMultiplier();
}
int DepthwiseConv2D::GetKernelW() const { return this->primitive_->value_as_DepthwiseConv2D()->kernelW(); }
int DepthwiseConv2D::GetKernelH() const { return this->primitive_->value_as_DepthwiseConv2D()->kernelH(); }
int DepthwiseConv2D::GetStrideW() const { return this->primitive_->value_as_DepthwiseConv2D()->strideW(); }
int DepthwiseConv2D::GetStrideH() const { return this->primitive_->value_as_DepthwiseConv2D()->strideH(); }
int DepthwiseConv2D::GetPadMode() const { return this->primitive_->value_as_DepthwiseConv2D()->padMode(); }
int DepthwiseConv2D::GetPadUp() const { return this->primitive_->value_as_DepthwiseConv2D()->padUp(); }
int DepthwiseConv2D::GetPadDown() const { return this->primitive_->value_as_DepthwiseConv2D()->padDown(); }
int DepthwiseConv2D::GetPadLeft() const { return this->primitive_->value_as_DepthwiseConv2D()->padLeft(); }
int DepthwiseConv2D::GetPadRight() const { return this->primitive_->value_as_DepthwiseConv2D()->padRight(); }
int DepthwiseConv2D::GetDilateW() const { return this->primitive_->value_as_DepthwiseConv2D()->dilateW(); }
int DepthwiseConv2D::GetDilateH() const { return this->primitive_->value_as_DepthwiseConv2D()->dilateH(); }
bool DepthwiseConv2D::GetHasBias() const { return this->primitive_->value_as_DepthwiseConv2D()->hasBias(); }
int DepthwiseConv2D::GetActivationType() const {
  return this->primitive_->value_as_DepthwiseConv2D()->activationType();
}

void DepthwiseConv2D::SetFormat(int format) {}
void DepthwiseConv2D::SetChannelIn(int channel_in) {}
void DepthwiseConv2D::SetChannelMultiplier(int channel_multiplier) {}
void DepthwiseConv2D::SetKernelW(int kernel_w) {}
void DepthwiseConv2D::SetKernelH(int kernel_h) {}
void DepthwiseConv2D::SetStrideW(int stride_w) {}
void DepthwiseConv2D::SetStrideH(int stride_h) {}
void DepthwiseConv2D::SetPadMode(int pad_mode) {}
void DepthwiseConv2D::SetPadUp(int pad_up) {}
void DepthwiseConv2D::SetPadDown(int pad_down) {}
void DepthwiseConv2D::SetPadLeft(int pad_left) {}
void DepthwiseConv2D::SetPadRight(int pad_right) {}
void DepthwiseConv2D::SetDilateW(int dilate_w) {}
void DepthwiseConv2D::SetDilateH(int dilate_h) {}
void DepthwiseConv2D::SetHasBias(bool has_bias) {}
void DepthwiseConv2D::SetActivationType(int activation_type) {}
#endif
int DepthwiseConv2D::InferShape(std::vector<lite::tensor::Tensor *> inputs_,
                                std::vector<lite::tensor::Tensor *> outputs_) {
  if (inputs_.size() != kDoubleNum && inputs_.size() != kMultiNum) {
    MS_LOG(ERROR) << "inputs number is invalid";
    return 1;
  }
  if (outputs_.size() != kSingleNum) {
    MS_LOG(ERROR) << "output number is invalid";
    return 1;
  }
  MS_ASSERT(this->primitive_ != nullptr);
  auto input = inputs_.front();
  MS_ASSERT(input != nullptr);
  auto weight = inputs_.at(1);
  MS_ASSERT(weight != nullptr);
  auto output = outputs_.front();
  MS_ASSERT(output != nullptr);
  output->SetFormat(input->GetFormat());
  output->set_data_type(input->data_type());
  if (!GetInferFlag()) {
    return RET_OK;
  }
  auto in_shape = input->shape();
  int input_h = in_shape.at(1);
  int input_w = in_shape.at(2);
  int input_channel = in_shape.at(3);
  int output_w = 0, output_h = 0;

  pad_l_ = GetPadLeft();
  pad_u_ = GetPadUp();
  pad_d_ = GetPadDown();
  pad_r_ = GetPadRight();
  if (GetPadMode() == schema::PadMode_SAME) {
    output_h = std::ceil(static_cast<float>(input_h) / static_cast<float>(GetStrideH()));
    output_w = std::ceil(static_cast<float>(input_w) / static_cast<float>(GetStrideW()));
    auto pad_h_all = ((output_h - 1) * GetStrideH() + (GetKernelH() - 1) * GetDilateH() + 1 - input_h);
    auto pad_w_all = ((output_w - 1) * GetStrideW() + (GetKernelW() - 1) * GetDilateW() + 1 - input_w);
    pad_u_ = pad_h_all / 2;
    pad_d_ = pad_h_all - pad_u_;
    pad_l_ = pad_w_all / 2;
    pad_r_ = pad_w_all - pad_l_;
  } else {
    output_h = std::ceil((static_cast<float>(input_h) + pad_u_ + pad_d_ -
                          (static_cast<float>(GetKernelH()) - 1) * static_cast<float>(GetDilateH())) /
                         static_cast<float>(GetStrideH()));
    output_w = std::ceil((static_cast<float>(input_w) + pad_l_ + pad_r_ -
                          (static_cast<float>(GetKernelW()) - 1) * static_cast<float>(GetDilateW())) /
                         static_cast<float>(GetStrideW()));
  }
  std::vector<int> out_shape{input->shape()};
  out_shape.at(1) = output_h;
  out_shape.at(2) = output_w;
  if (GetChannelMultiplier() * input_channel != weight->shape()[0]) {
    MS_LOG(ERROR) << "Conv depthwise only support group equals output channel.";
    return 1;
  }
  out_shape.at(3) = weight->shape()[0] * weight->shape()[3];  // in_channel * out_channel

  output->set_shape(out_shape);
  return 0;
}
}  // namespace lite
}  // namespace mindspore
