#include "PORT_LIB.h"

/// @brief Tarih ve saat verilerinin işaretçiler üzerinden atanmasını sağlar.
/// Bu fonksiyon, zamanı dışarıdan bağlayarak yazılarda zaman damgası eklemeyi mümkün kılar.
/// @param yil_   Geçerli yılın adresi
/// @param ay_    Geçerli ayın adresi
/// @param gun_   Geçerli günün adresi
/// @param saat_  Geçerli saatin adresi
/// @param dk_    Geçerli dakikanın adresi
/// @param sn_    Geçerli saniyenin adresi
/// @example
/// uint16_t yil = 2025;
/// uint8_t ay = 5, gun = 26, saat = 14, dk = 45, sn = 30;
/// port.tarih_cikti(&yil, &ay, &gun, &saat, &dk, &sn);
void Port::tarih_cikti(uint16_t *yil_, uint8_t *ay_, uint8_t *gun_, uint8_t *saat_, uint8_t *dk_, uint8_t *sn_)
{
    _yil = yil_;
    _ay = ay_;
    _gun = gun_;
    _saat = saat_;
    _dk = dk_;
    _sn = sn_;
}

/// @brief Ayarlanmış tarih/saat değerlerini string olarak döndürür.
/// @return "YYYY/MM/DD HH:MM:SS" formatında string değer
/// @example
/// // _yil, _ay, vb. önceden tarih_cikti ile atanmış olmalı
/// String t = port._tarih_ciktisi_();
/// Serial.println(t);  // Örn: "2025/05/26 14:45:30"
String Port::_tarih_ciktisi_()
{
    char buffer[32];
    sprintf(buffer, "%04u/%02u/%02u %02u:%02u:%02u", *_yil, *_ay, *_gun, *_saat, *_dk, *_sn);
    return String(buffer);
}

/// @brief Yazılarda tarih/saat bilgisinin görünüp görünmeyeceğini ayarlar.
/// @param k true ise tarih damgası gösterilir, false ise gösterilmez
/// @example port.ciktida_tarih_bilgisi(true);
void Port::ciktida_tarih_bilgisi(bool k) { zaman_damga = k; }

/// @brief Yazı çıktısını Serial'e gönderir. Eğer zaman damgası açıksa başına tarih eklenir.
/// @param k Yazdırılacak metin
/// @example port.yaz("Sistem başlatıldı");
void Port::yaz(String k)
{
    if (zaman_damga)
    {
        String c = _tarih_ciktisi_() + " --> " + k;
        Serial.print(c);
    }
    else
        Serial.print(k);
}

// Aşağıdaki overload fonksiyonlar farklı veri türleriyle yaz() kullanımına olanak tanır.

/// @brief Flash bellekten string yazdırır.
/// @param k Flash bellekteki yazı
void Port::yaz(const __FlashStringHelper *k) { yaz(String(k)); }

/// @brief Karakter dizisini yazdırır.
/// @param k const char* türünde string
void Port::yaz(const char *k) { yaz(String(k)); }

/// @brief Tek karakter yazar
/// @param k karakter
void Port::yaz(char k) { yaz(String(k)); }

/// @brief Tamsayı değeri yazdırır
void Port::yaz(int k) { yaz(String(k)); }

/// @brief Pozitif tamsayı değeri yazdırır
void Port::yaz(unsigned int k) { yaz(String(k)); }

/// @brief Uzun tamsayı değeri yazdırır
void Port::yaz(long k) { yaz(String(k)); }

/// @brief Pozitif uzun tamsayı değeri yazdırır
void Port::yaz(unsigned long k) { yaz(String(k)); }

/// @brief Ondalıklı sayı yazdırır
/// @param k sayı değeri
/// @param precision virgülden sonraki basamak sayısı
void Port::yaz(float k, int precision) { yaz(String(k, precision)); }

/// @brief Double sayı yazdırır
/// @param k sayı değeri
/// @param precision virgülden sonraki basamak
void Port::yaz(double k, int precision) { yaz(String(k, precision)); }

/// @brief Veri paketinin mevcut dolu eleman sayısını verir.
/// @return Paket boyutu (kaç karakter yazılı)
/// @example uint16_t len = port.paket.boyut_ogren();
uint16_t Port::paket::boyut_ogren() { return paket_priv->doluluk; }

/// @brief Verilen indisteki karakteri döndürür
/// @param indis Karakterin bulunduğu konum
/// @return Belirtilen indisteki karakter (hatalıysa '\0')
/// @example char ch = port.paket.veri(3);
char Port::paket::veri(uint16_t indis)
{
    if (indis > paket_priv->doluluk)
        return '\0';
    return paket_priv->veri_pakedi[indis];
}

/// @brief Paketteki tekrar eden karakterleri filtreler.
/// Örn: "aaabccc" → "abc"
/// @param max_cikti Maksimum kontrol edilecek karakter sayısı. 0 ise otomatik doluluk kadar olur.
/// @example
/// // Paketteki tekrar eden karakterleri kaldır
/// port.paket.filtrele();
void Port::paket::filtrele(uint16_t max_cikti)
{
    if (max_cikti == 0 || max_cikti > paket_priv->_max_boyut)
        max_cikti = paket_priv->doluluk;

    char onceki_karakter = '\0';
    uint16_t yaz_indis = 0;

    for (uint16_t i = 0; i < max_cikti; i++)
    {
        char karakter = paket_priv->veri_pakedi[i];

        if (karakter != onceki_karakter && karakter != paket_priv->bos_karakter)
        {
            paket_priv->veri_pakedi[yaz_indis++] = karakter;
            onceki_karakter = karakter;
        }
    }

    // Geri kalanı boş karakter ile temizle
    paket_priv->doluluk = yaz_indis;
    while (yaz_indis < paket_priv->_max_boyut)
        paket_priv->veri_pakedi[yaz_indis++] = '\0';
}
