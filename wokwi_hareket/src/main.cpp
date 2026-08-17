#include <Arduino.h>

#include <TensorFlowLite_ESP32.h>
#include "tensorflow/lite/micro/all_ops_resolver.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/micro/micro_error_reporter.h"
#include "tensorflow/lite/schema/schema_generated.h"

#include "model_data.h"        // bizim modelimiz (C dizisi)

// Hareket isimleri (etiket sırasına göre)
const char* hareketler[] = {
  "WALKING", "W_UPSTAIRS", "W_DOWNSTAIRS", "SITTING", "STANDING", "LAYING"
};

// TFLite Micro için global nesneler
namespace {
  tflite::MicroErrorReporter micro_error_reporter;
  tflite::ErrorReporter* error_reporter = &micro_error_reporter;
  const tflite::Model* model = nullptr;
  tflite::MicroInterpreter* interpreter = nullptr;
  TfLiteTensor* input = nullptr;
  TfLiteTensor* output = nullptr;

  constexpr int kTensorArenaSize = 20 * 1024;
  uint8_t tensor_arena[kTensorArenaSize];
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("=== TinyML Hareket Tanima ===");

  // 1) Modeli yükle
  model = tflite::GetModel(model_data);
  if (model->version() != TFLITE_SCHEMA_VERSION) {
    Serial.println("HATA: Model surumu uyumsuz!");
    return;
  }

  // 2) Operasyonları çöz
  static tflite::AllOpsResolver resolver;

  // 3) Interpreter'ı kur
  static tflite::MicroInterpreter static_interpreter(
    model, resolver, tensor_arena, kTensorArenaSize, error_reporter);
  interpreter = &static_interpreter;

  // 4) Bellek ayır
  if (interpreter->AllocateTensors() != kTfLiteOk) {
    Serial.println("HATA: Tensor bellegi ayrilamadi!");
    return;
  }

  input = interpreter->input(0);
  output = interpreter->output(0);

  Serial.println("Model yuklendi ve hazir.");
  Serial.print("Girdi boyutu: ");
  for (int i = 0; i < input->dims->size; i++) {
    Serial.print(input->dims->data[i]);
    if (i < input->dims->size - 1) Serial.print(" x ");
  }
  Serial.println();
  Serial.print("Cikti sinif sayisi: ");
  Serial.println(output->dims->data[output->dims->size - 1]);
}

void loop() {
  delay(5000);
  Serial.println("Model bekliyor... (tahmin bir sonraki adimda)");
}