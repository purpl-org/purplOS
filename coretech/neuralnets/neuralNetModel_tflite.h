/**
 * File: neuralNetModel_tflite.h
 *
 * Author: Andrew Stein
 * Date:   12/5/2017
 *
 * Description: Implementation of NeuralNetModel class which wraps TensorFlow Lite.
 *
 * NOTE: this implementation (.cpp) of this wrapper completely compiles out if
 *       not using the TFLite platform for neural nets.
 *
 * Copyright: Anki, Inc. 2017
 **/

#ifndef __Anki_NeuralNets_NeuralNetModel_TFLite_H__
#define __Anki_NeuralNets_NeuralNetModel_TFLite_H__

#include "coretech/neuralnets/neuralNetModel_interface.h"
#include <list>
#include <memory>
#include <string>
#include <json/json.h>

#include "tensorflow/lite/model.h"
#include "tensorflow/lite/interpreter.h"

namespace Anki {
namespace NeuralNets {

class TFLiteModel : public INeuralNetModel
{
public:
  TFLiteModel();
  virtual ~TFLiteModel();

  virtual Result Detect(Vision::ImageRGB& img,
                        std::list<Vision::SalientPoint>& salientPoints) override;

protected:
  virtual Result LoadModelInternal(const std::string& modelPath,
                                   const Json::Value& config) override;

private:
  void ScaleImage(Vision::ImageRGB& img);

  std::unique_ptr<tflite::FlatBufferModel> _model;
  std::unique_ptr<tflite::Interpreter>     _interpreter;
};

} // namespace NeuralNets
} // namespace Anki

#endif /* __Anki_NeuralNets_NeuralNetModel_TFLite_H__ */
