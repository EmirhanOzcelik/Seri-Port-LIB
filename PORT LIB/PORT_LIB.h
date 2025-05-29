#pragma once

#include "Arduino.h"

#if defined(__AVR__)
#elif defined(ESP8266) || defined(ESP32)
#endif

class Port
{
private:
    friend class paket;
    char bos_karakter = '\0';       // boşkaraktr
    char _ayirac_karakter = '#';    // ayıraç karakter
    uint16_t _seri_gelen_boyut;     // seri Porta yazılan veri uzunlugunu tutan degisken
    uint16_t _max_boyut;            // veri  tutcak dizi için max ayrılacak bellek
    char *veri_pakedi;              // veri tutacak paket
    uint8_t *_ayirac_konumlar;      // veri konumlarını tutacak paket
    uint16_t _ayirac_konum_boyut;   // ayirac konumları dizisi boyutu
    uint16_t *_yil;                 // adres ttc
    uint8_t *_ay;                   // adres ttc
    uint8_t *_gun;                  // adres ttc
    uint8_t *_saat;                 // adres ttc
    uint8_t *_dk;                   // adres ttc
    uint8_t *_sn;                   // adres ttc
    String _tarih_ciktisi_();       // adres ttc için özelkod
    uint16_t veri_paket_sayisi = 0; // kac çekmece var
    bool zaman_damga = false;       // zaman damgas kullanılacak mı?
    uint16_t doluluk = 0;           // veri_pakedi doluluk oranıdır(kaç karakter içeriyor?)

public:
    Port(uint8_t max_bolme, uint8_t max_icerik, char ayirac_karakter = '#');
    Port(uint16_t max_boyut, char ayirac_karakter = '#');
    ~Port();
    void oku();
    String veri(uint16_t veri_indisi);
    void temizle();
    uint16_t veri_adedi();

    void ciktida_tarih_bilgisi(bool k);
    void tarih_cikti(uint16_t *yil_, uint8_t *ay_, uint8_t *gun_, uint8_t *saat_, uint8_t *dk_, uint8_t *sn_);
    void yaz(String k);
    void yaz(const __FlashStringHelper *k);
    void yaz(const char *k);
    void yaz(char k);
    void yaz(int k);
    void yaz(unsigned int k);
    void yaz(long k);
    void yaz(unsigned long k);
    void yaz(float k, int precision = 2);
    void yaz(double k, int precision = 2);

    class paket
    {
    private:
        Port *paket_priv;
        friend class Port;

    public:
        uint16_t boyut_ogren();
        char veri(uint16_t indis);
        void filtrele(uint16_t max_cikti = 0);
    } paket;
};