# Port Veri Yönetimi Kütüphanesi

Bu kütüphane, ESP8266/ESP32 tabanlı sistemlerde seri port üzerinden alınan verileri ayrıştırmak ve yönetmek için temel işlevler sunar.

## Sınıf Yapısı

Kütüphane, ana `Port` sınıfı ve onun içinde yer alan `Port::paket` yardımcı sınıfından oluşur.

## `Port` Sınıfı Fonksiyonları

* **`Port(uint8_t max_bolme, uint8_t max_icerik, char ayirac_karakter = '#')`**
    * Belirli sayıda veri bölümü ve her bir bölümün maksimum karakter uzunluğu ile `Port` nesnesini başlatır.

* **`Port(uint16_t max_boyut, char ayirac_karakter = '#')`**
    * Seri porttan okunacak toplam verinin maksimum boyutuna göre `Port` nesnesini başlatır.

* **`void oku()`**
    * Seri porttan `\n` (newline) karakterine kadar olan veriyi okur ve ayırıcı karaktere göre bölümlere ayırır.

* **`String veri(uint16_t veri_indisi)`**
    * Okunan veri paketindeki belirtilen indisli ayrıştırılmış veri bölümünü `String` olarak döndürür.

* **`void temizle()`**
    * Dahili veri tamponunu ve ayrıştırılmış veri konum bilgilerini temizler.

* **`uint16_t veri_adedi()`**
    * Okunan veri paketindeki ayrıştırılan toplam veri bölümü sayısını döndürür.

* **`void ciktida_tarih_bilgisi(bool k)`**
    * Seri porta yazma işlemlerinde zaman damgası ekleme özelliğini etkinleştirir veya devre dışı bırakır.

* **`void tarih_cikti(uint16_t *yil_, uint8_t *ay_, uint8_t *gun_, uint8_t *saat_, uint8_t *dk_, uint8_t *sn_)`**
    * Yazma fonksiyonlarında kullanılacak yıl, ay, gün, saat, dakika ve saniye değişkenlerinin adreslerini kütüphaneye atar.

* **`void yaz(String k)`**
    * Belirtilen `String` veriyi seri porta yazar. Zaman damgası aktifse metnin başına eklenir.

* **`void yaz(const __FlashStringHelper *k)`**
    * Flash bellekteki (`F()`) bir metni seri porta yazar.

* **`void yaz(const char *k)`**
    * Bir `const char*` metni seri porta yazar.

## `Port::paket` Sınıfı Fonksiyonları

* **`char veri(uint16_t indis)`**
    * Okunan tüm ham veri paketinden belirli bir indisteki karakteri döndürür.

* **`String veri_oku_string(uint16_t baslangic, uint16_t bitis)`**
    * Ham veri paketinden belirli bir başlangıç ve bitiş indisi arasındaki karakterleri `String` olarak okur.

* **`String veri_oku_string(uint16_t veri_indisi)`**
    * `Port::veri()` fonksiyonu ile aynı işlevi görür; belirtilen indisli veri bölümünü `String` olarak döndürür.

* **`uint16_t boyut_ogren()`**
    * Seri porttan en son okunan paketin toplam karakter boyutunu döndürür.

* **`void filtrele(uint16_t max_cikti = 0)`**
    * Paketteki tekrar eden ardışık karakterleri filtreler (örn: "aaabccc" → "abc"). `max_cikti` belirtilirse o kadar karakter kontrol edilir.
