#include "PORT_LIB.h"

/**
 * @brief Seri porttan gelen veriyi okur ve ayırıcı karaktere göre bölümlere ayırır.
 *
 * Bu fonksiyon newline (`\n`) karakterine kadar olan veriyi `veri_pakedi` dizisine okur.
 * Ayırıcı karakter (`#` gibi) kullanılarak veri blokları tespit edilir.
 *
 * @example
 * // Örn: Serial üzerinden "#V1#V2#V3#" geldiğinde
 * port.oku();  // _ayirac_konumlar → [0, 3, 6, 9]
 *              // veri_paket_sayisi = 2
 */
void Port::oku()
{
    _seri_gelen_boyut = Serial.readBytesUntil('\n', veri_pakedi, _max_boyut);
    doluluk = _seri_gelen_boyut;
    if (_seri_gelen_boyut > 1)
    {
        byte k = 0;
        for (int y = 0; y < _seri_gelen_boyut; y++)
            if (veri_pakedi[y] == _ayirac_karakter)
            {
                if (k < _ayirac_konum_boyut)
                {
                    _ayirac_konumlar[k] = y;
                    k++;
                }
            }
        if (k > 1)
            veri_paket_sayisi = k - 1;
    }
}

/**
 * @brief Okunan paketten, indise göre veri döndürür.
 *
 * @param veri_indisi Erişmek istediğiniz veri bloğunun indisi (0'dan başlar)
 * @return String türünde, istenen veri bloğu. Hatalı indis verilirse uyarı döner.
 *
 * @example
 * // Girdi: "#merhaba#dunya#"
 * port.oku();
 * port.veri(0);  // "merhaba"
 * port.veri(1);  // "dunya"
 */
String Port::veri(uint16_t veri_indisi)
{
    if (veri_indisi > veri_paket_sayisi)
        return "indis veriden fazla";
    uint16_t baslangic = _ayirac_konumlar[veri_indisi] + 1;
    uint16_t bitis = _ayirac_konumlar[veri_indisi + 1];
    char dondurulecek[abs(bitis - baslangic) + 2];
    byte k = 0;
    while (baslangic < bitis)
    {
        dondurulecek[k++] = veri_pakedi[baslangic];
        baslangic++;
    }
    dondurulecek[k] = '\0';
    return dondurulecek;
}

/**
 * @brief Hafızadaki okuma verilerini temizler.
 *
 * `veri_pakedi`, `_ayirac_konumlar`, `veri_paket_sayisi` ve `doluluk` sıfırlanır.
 * Yeni veri gelmeden önce veya veri işlendikten sonra çağrılması önerilir.
 *
 * @example
 * port.temizle();
 */
void Port::temizle()
{
    memset(veri_pakedi, bos_karakter, _max_boyut);
    memset(_ayirac_konumlar, 0xFF, _ayirac_konum_boyut);
    veri_paket_sayisi = 0;
    doluluk = 0;
    _seri_gelen_boyut = 0;
    paket.boyut_ogren();
    yaz(F("\ntemizlendi\n"));
}

/**
 * @brief Mevcut veri bloğu sayısını döndürür.
 *
 * `oku()` fonksiyonu çağrıldıktan sonra kaç tane veri parçası okunduğunu verir.
 *
 * @return Toplam veri adedi (kaç tane #veri# bölümü var)
 *
 * @example
 * // Girdi: "#x#y#z#"
 * port.oku();
 * port.veri_adedi();  // 2
 */
uint16_t Port::veri_adedi() { return veri_paket_sayisi; }

/**
 * @brief Port nesnesinin kurulumu — basit yapılandırma.
 *
 * @param max_boyut veri_pakedi için maksimum karakter sayısı
 * @param ayirac_karakter Verileri ayırmak için kullanılacak karakter (örnek: '#')
 *
 * @example
 * Port port(50, '#');  // 50 karaktere kadar # ile ayrılmış veriler için yapılandırma
 */
Port::Port(uint16_t max_boyut, char ayirac_karakter)
{
    paket.paket_priv = this;
    _ayirac_karakter = ayirac_karakter;
    _max_boyut = max_boyut;
    veri_pakedi = new char[_max_boyut];
    _ayirac_konum_boyut = 15;
    _ayirac_konumlar = new uint8_t[_ayirac_konum_boyut];
    memset(veri_pakedi, bos_karakter, _max_boyut);
    memset(_ayirac_konumlar, 0xFF, _ayirac_konum_boyut);
}

/**
 * @brief Port nesnesinin kurulumu — gelişmiş yapılandırma.
 *
 * @param max_bolme En fazla kaç veri bölümü (# veri #) olabileceği (çekmece sayısı gibi düşünebilirsiniz)
 * @param max_icerik Her bir verinin max karakter uzunluğu (çekmece büyüklüğü gibi)
 * @param ayirac_karakter Ayırıcı karakter (örnek: '#')
 *
 * @example
 * Port port(5, 10, '#');  // 5 adet veri, her biri 10 karaktere kadar olabilir
 */
Port::Port(uint8_t max_bolme, uint8_t max_icerik, char ayirac_karakter)
{
    paket.paket_priv = this;
    _ayirac_konum_boyut = max_bolme;
    _ayirac_konumlar = new uint8_t[_ayirac_konum_boyut];
    _ayirac_karakter = ayirac_karakter;
    _max_boyut = max_bolme * max_icerik;
    veri_pakedi = new char[_max_boyut];
    memset(veri_pakedi, bos_karakter, _max_boyut);
    memset(_ayirac_konumlar, 0xFF, _ayirac_konum_boyut);
}

/**
 * @brief Hafızayı temizler.
 *
 * Bu fonksiyon kullanılması önerilmez, çünkü port hafızası kendiliğinden yönetilmektedir.
 * Eğer manuel bellek temizlemeniz gerekiyorsa `temizle()` fonksiyonu kullanılmalıdır.
 */
Port::~Port()
{
    delete[] veri_pakedi;
    delete[] _ayirac_konumlar;
}
