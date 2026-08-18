#include <Arduino.h>

#include <TensorFlowLite_ESP32.h>
#include "tensorflow/lite/micro/all_ops_resolver.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/micro/micro_error_reporter.h"
#include "tensorflow/lite/schema/schema_generated.h"

#include "model_data.h"          // eğitilmiş model (C dizisi)
#include "test_ornekleri.h"      // test hareket örnekleri

const char* hareketler[] = {
  "WALKING", "W_UPSTAIRS", "W_DOWNSTAIRS", "SITTING", "STANDING", "LAYING"
};

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

  model = tflite::GetModel(model_data);
  if (model->version() != TFLITE_SCHEMA_VERSION) {
    Serial.println("HATA: Model surumu uyumsuz!");
    return;
  }

  static tflite::AllOpsResolver resolver;
  static tflite::MicroInterpreter static_interpreter(
    model, resolver, tensor_arena, kTensorArenaSize, error_reporter);
  interpreter = &static_interpreter;

  if (interpreter->AllocateTensors() != kTfLiteOk) {
    Serial.println("HATA: Tensor bellegi ayrilamadi!");
    return;
  }

  input = interpreter->input(0);
  output = interpreter->output(0);
  Serial.println("Model hazir. Test ornekleri deneniyor...\n");

  int dogru = 0;

  // Her test örneğini modele ver
  for (int n = 0; n < ORNEK_SAYISI; n++) {
    // Örneği modelin giriş tensörüne kopyala
    for (int i = 0; i < ORNEK_UZUNLUK; i++) {
      input->data.int8[i] = test_verileri[n][i];
    }

    // Tahmini çalıştır
    if (interpreter->Invoke() != kTfLiteOk) {
      Serial.println("HATA: Tahmin calistirilamadi!");
      continue;
    }

    // En yüksek olasılıklı sınıfı bul
    int en_iyi = 0;
    int8_t en_yuksek = output->data.int8[0];
    for (int c = 1; c < 6; c++) {
      if (output->data.int8[c] > en_yuksek) {
        en_yuksek = output->data.int8[c];
        en_iyi = c;
      }
    }

    int gercek = gercek_etiketler[n];
    bool ok = (en_iyi == gercek);
    if (ok) dogru++;

    Serial.print("Ornek ");
    Serial.print(n + 1);
    Serial.print(" | Gercek: ");
    Serial.print(hareketler[gercek]);
    Serial.print(" | Tahmin: ");
    Serial.print(hareketler[en_iyi]);
    Serial.println(ok ? "  [DOGRU]" : "  [YANLIS]");
  }

  Serial.print("\nSonuc: ");
  Serial.print(dogru);
  Serial.print(" / ");
  Serial.print(ORNEK_SAYISI);
  Serial.println(" dogru tahmin.");
}

void loop() {
  // Tahminler setup'ta bir kez yapıldı; loop bos.
  delay(10000);
}