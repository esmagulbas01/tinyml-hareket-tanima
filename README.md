# TinyML — Hareket Tanıma

İvmeölçer verisinden hareket sınıflandıran, bir mikrodenetleyici (ESP32) üzerinde
çalışan minik bir yapay zekâ modeli. Model bilgisayarda eğitilir, int8 kuantizasyonla
küçültülür ve LiteRT for Microcontrollers ile çip üzerinde çalıştırılır.

## Amaç
Sınırlı belleğe sahip bir çipte, buluta bağlanmadan, yerel olarak çalışan bir
makine öğrenmesi modeli kurmak.

## Boru hattı
Veri toplama → Model eğitimi → int8 kuantizasyon → C dizisine dönüştürme →
Çipe gömme → Gerçek zamanlı tahmin

## Klasör yapısı
- `data/` — ham ve işlenmiş hareket verisi
- `notebooks/` — eğitim ve dönüşüm çalışmaları
- `model/` — eğitilmiş ve küçültülmüş model dosyaları
- `firmware/` — çip üzerinde çalışan kod (ESP32 / Wokwi)
- `docs/` — şema ve görseller

## Durum
🚧 Geliştirme aşamasında.

## Sonuçlar
_(Model boyutu, doğruluk, çıkarım süresi buraya eklenecek.)_