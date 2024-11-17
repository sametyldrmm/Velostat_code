#include <WiFi.h>
#include <PubSubClient.h>
#include <WiFiClientSecure.h>

// WiFi Ayarları
const char *ssid = "Bilal";         // WiFi ağınızın adı
const char *password = "bilal...."; // WiFi şifreniz

// MQTT Broker Ayarları
const char *mqtt_server = "8824e3a9df204b2098e07f76ba74b9aa.s1.eu.hivemq.cloud";
const int mqtt_port = 8883;
const char *mqtt_topic = "test/topic";
const char *mqtt_username = "hivemq.webclient.1731763114191";
const char *mqtt_password = "heKT$w@0517F*BI!Cgzl";

WiFiClientSecure espClient;
PubSubClient client(espClient);

char data[100];

void setup_wifi()
{
    delay(10);
    Serial.println();
    Serial.print("WiFi'ye bağlanılıyor: ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi bağlantısı kuruldu.");
    Serial.print("IP Adresi: ");
    Serial.println(WiFi.localIP());
}

// MQTT Brokerına bağlanma fonksiyonu
void reconnect()
{
    while (!client.connected())
    {
        Serial.print("MQTT Broker'a bağlanılıyor...");
        if (client.connect("ESP32_Client", mqtt_username, mqtt_password))
        {
            Serial.println("Bağlantı başarılı!");
        }
        else
        {
            Serial.print("Bağlantı başarısız, hata kodu: ");
            Serial.print(client.state());
            Serial.println(" Yeniden dene...");
            delay(5000);
        }
    }
}

char *create_json_string(int a, int b, int c, int d, int e, int f)
{
    // JSON formatında string oluştur
    snprintf(data, sizeof(data), "{ \"a\": %d, \"b\": %d, \"c\": %d, \"d\": %d, \"e\": %d, \"f\": %d }", a, b, c, d, e, f);
    return data; // Global değişken
}

int durum_sayisi = 1;
int pozisyon_sayisi = 1;
int alt_ust_matris_sayisi = 1;
int dikey_kutucuk_sayisi = 30;
int yatay_kutucuk_sayisi = 30;

int _All_pozisyon[2][1][30][30];
int degerlendirilecek_veri[1][30][30];
char durumlar[2][100];

// durum 0 argüman
int main_veri[1][30][30];

int temp[6][1][30][30]; //  [6]-> her bir while dongusu
                        //  [2]->alt matris ust matris
                        //  [28][28] değerlerimiz

char str_pin[4];

int sol_ust[14 * 12];
int sag_ust[14 * 12];
int orta_ust[14 * 7];
int orta_alt[17 * 7];
int sol_alt[17 * 12];
int sag_alt[17 * 12];

int temp_sol_ust = 0;
int temp_sag_ust = 0;
int temp_orta_ust = 0;
int temp_orta_alt = 0;
int temp_sol_alt = 0;
int temp_sag_alt = 0;

int ort_temp_sol_ust = 0;
int ort_temp_sag_ust = 0;
int ort_temp_orta_ust = 0;
int ort_temp_orta_alt = 0;
int ort_temp_sol_alt = 0;
int ort_temp_sag_alt = 0;

void str_pin_upload(const char *str)
{
    for (size_t i = 0; str[i]; i++)
    {
        str_pin[i] = str[i];
    }
    str_pin[4] = '\0';
}

void pin(int a)
{
    if (a == 0)
        str_pin_upload("0000");
    if (a == 1)
        str_pin_upload("1000");
    if (a == 2)
        str_pin_upload("0100");
    if (a == 3)
        str_pin_upload("1100");
    if (a == 4)
        str_pin_upload("0010");
    if (a == 5)
        str_pin_upload("1010");
    if (a == 6)
        str_pin_upload("0110");
    if (a == 7)
        str_pin_upload("1110");
    if (a == 8)
        str_pin_upload("0001");
    if (a == 9)
        str_pin_upload("1001");
    if (a == 10)
        str_pin_upload("0101");
    if (a == 11)
        str_pin_upload("1101");
    if (a == 12)
        str_pin_upload("0011");
    if (a == 13)
        str_pin_upload("1011");
    if (a == 14)
        str_pin_upload("0111");
    if (a == 15)
        str_pin_upload("1111");
}

int connect_pin[2][4];
int en_pin[2][2];
int signal_pin[2][2];

void setup()
{
    Serial.begin(9600);
    delay(1000);
    setup_wifi();
    delay(500);
    // tek tek yazılacak
    connect_pin[0][0] = 23;
    connect_pin[0][1] = 22;
    connect_pin[0][2] = 21;
    connect_pin[0][3] = 19;
    en_pin[0][0] = 14;
    en_pin[0][1] = 17;
    signal_pin[0][0] = 33;
    signal_pin[0][1] = 13;

    connect_pin[1][0] = 4;
    connect_pin[1][1] = 26;
    connect_pin[1][2] = 27;
    connect_pin[1][3] = 18;
    en_pin[1][0] = 32;
    en_pin[1][1] = 16;
    signal_pin[1][0] = 25;
    signal_pin[1][1] = 12;

    // MQTT için güvenli bağlantı ayarları
    espClient.setInsecure(); // SSL doğrulamasını devre dışı bırak
    client.setServer(mqtt_server, mqtt_port);

    pinMode(connect_pin[0][0], OUTPUT);
    pinMode(connect_pin[0][1], OUTPUT);
    pinMode(connect_pin[0][2], OUTPUT);
    pinMode(connect_pin[0][3], OUTPUT);
    pinMode(en_pin[0][0], OUTPUT);
    pinMode(en_pin[0][1], OUTPUT);
    pinMode(signal_pin[0][0], OUTPUT);
    pinMode(signal_pin[0][1], OUTPUT);
    pinMode(connect_pin[1][0], OUTPUT);
    pinMode(connect_pin[1][1], OUTPUT);
    pinMode(connect_pin[1][2], OUTPUT);
    pinMode(connect_pin[1][3], OUTPUT);
    pinMode(en_pin[1][0], OUTPUT);
    pinMode(en_pin[1][1], OUTPUT);
    pinMode(signal_pin[1][0], INPUT);
    pinMode(signal_pin[1][1], INPUT);
    digitalWrite(en_pin[0][0], LOW);
    digitalWrite(en_pin[0][1], LOW);
    digitalWrite(en_pin[1][0], LOW);
    digitalWrite(en_pin[1][1], LOW);
}

int low_high(char a)
{
    if (a == '1')
        return (HIGH);
    return (LOW);
}

void bir_fonk(int a, int alt_ust)
{
    pin(a);
    digitalWrite(connect_pin[alt_ust][0], low_high(str_pin[0]));
    digitalWrite(connect_pin[alt_ust][1], low_high(str_pin[1]));
    digitalWrite(connect_pin[alt_ust][2], low_high(str_pin[2]));
    digitalWrite(connect_pin[alt_ust][3], low_high(str_pin[3]));
}

void string_update(const char *string, int kacinci_durum)
{
    for (size_t i = 0; string[i] != '\0'; i++)
        durumlar[kacinci_durum][i] = string[i];
}

int percent_value(int elde_olan, int istenen, int yuzde)
{
    if ((istenen - (istenen * yuzde / 100)) < elde_olan && (istenen + (istenen * yuzde / 100)) > elde_olan)
        return (1);
    return (0);
}

//
//
//
//
//
//
//

void dizi_karsilastir_yuzdeye_gore_dogrilik_ogren()
{
    // elde edilen matris _All_pozisyondaki matrisle karşılaştırılıp yuzde x uzeri yakın olan seçenekler ona denk kabul edilecek
    //  mesala 3,5 noktası dik pozisyonunda 600 ise yuzde 20 sapma payı verirsek 480 - 720 arasında bir değer geldiğinde o kutucuk doğru kabul edilir
    //  elimizdeki kutucukların %x kadarı doğru ise pozisyonumuz diktir teşhisini yaparız
    int total_dogru[durum_sayisi];
    for (size_t i = 0; i < durum_sayisi; i++)
    {
        total_dogru[i] = 0;
    }
    for (size_t i = 0; i < pozisyon_sayisi; i++)
    {
        for (size_t j = 0; j < dikey_kutucuk_sayisi; j++)
        {
            for (size_t h = 0; h < yatay_kutucuk_sayisi; h++)
            {
                for (size_t g = 0; g < durum_sayisi; g++)
                {
                    if (h <= 14) // sol taraf
                    {
                        if (j <= 12) // sol alt taraf
                        {
                            // sol_alt[temp_sol_alt++] = degerlendirilecek_veri[i][j][h];
                            ort_temp_sol_alt += degerlendirilecek_veri[i][j][h];
                        }
                        if (j > 12 && j < 19) // orta alt taraf
                        {
                            // orta_alt[temp_orta_alt++] = degerlendirilecek_veri[i][j][h];
                            ort_temp_orta_alt += degerlendirilecek_veri[i][j][h];
                        }
                        if (j >= 19 && j < 30) //  sol yukarı taraf
                        {
                            // sol_ust[temp_sol_ust++] = degerlendirilecek_veri[i][j][h];
                            ort_temp_sol_ust += degerlendirilecek_veri[i][j][h];
                        }
                    }
                    if (h > 14 && h < 30) // sağ taraf
                    {
                        if (j <= 12) // sağ alt
                        {
                            // sag_alt[temp_sag_alt++] = degerlendirilecek_veri[i][j][h];
                            ort_temp_sag_alt += degerlendirilecek_veri[i][j][h];
                        }
                        if (j > 12 && j < 19) // orta üst taraf
                        {
                            // orta_ust[temp_orta_ust++] = degerlendirilecek_veri[i][j][h];
                            ort_temp_orta_ust += degerlendirilecek_veri[i][j][h];
                        }
                        if (j >= 19 && j < 30) //  sağ yukarı taraf
                        {
                            // sag_ust[temp_sag_ust++] = degerlendirilecek_veri[i][j][h];
                            ort_temp_sag_ust += degerlendirilecek_veri[i][j][h];
                        }
                    }
                    // if (percent_value(_All_pozisyon[0][i][j][h], degerlendirilecek_veri[i][j][h], 10))
                    // {
                    //     // Serial.print(_All_pozisyon[0][i][j][h]);
                    //     // delay(3);
                    //     // Serial.print(degerlendirilecek_veri[i][j][h]);
                    //     total_dogru[i]++;
                    //     delay(3);
                    // }
                }
            }
        }

        // Serial.print(total_dogru[i]);
        delay(3);
    }

    Serial.println(ort_temp_sol_ust);
    Serial.println(ort_temp_sag_ust);
    Serial.println(ort_temp_orta_ust);
    Serial.println(ort_temp_orta_alt);
    Serial.println(ort_temp_sol_alt);
    Serial.println(ort_temp_sag_alt);
    Serial.println("---------------");
    ort_temp_sol_ust = 0;
    ort_temp_sag_ust = 0;
    ort_temp_orta_ust = 0;
    ort_temp_orta_alt = 0;
    ort_temp_sol_alt = 0;
    ort_temp_sag_alt = 0;

    // burada hangi i değerine geldiğine gore bir print işlemi yapılır ve kullanıcı bilgilendirilir
    int max_dogru = 0;
    int max_pozisyon = 0;
    for (size_t i = 0; i < durum_sayisi; i++)
    {
        if (total_dogru[i] > max_dogru)
        {
            max_dogru = total_dogru[i];
            max_pozisyon = i;
            // Serial.print("%d\n",max_dogru);
        }
    }
    // değerlerimizin doğruluğunu sorgulaycaz
    // Serial.print("%d",yatay_kutucuk_sayisi * dikey_kutucuk_sayisi * alt_ust_matris_sayisi  );
    if (max_dogru >= (yatay_kutucuk_sayisi * dikey_kutucuk_sayisi * alt_ust_matris_sayisi) * 70 / 100)
        Serial.print(durumlar[max_pozisyon]);
    else
        Serial.print("yanliş bir pozisyonda oturuyor\n");
}

void all_update(int durum, int ***veri)
{
    for (size_t k = 0; k < pozisyon_sayisi; k++)
    {
        for (size_t i = 0; i < dikey_kutucuk_sayisi; i++)
        {
            for (size_t j = 0; j < yatay_kutucuk_sayisi; j++)
            {
                _All_pozisyon[durum][k][i][j] = veri[k][i][j];
            }
        }
    }
}

void degerlendicek_update(int ***veri)
{
    for (size_t k = 0; k < pozisyon_sayisi; k++)
    {
        for (size_t i = 0; i < dikey_kutucuk_sayisi; i++)
        {
            for (size_t j = 0; yatay_kutucuk_sayisi > j; j++)
            {
                degerlendirilecek_veri[k][i][j] = veri[k][i][j];
            }
        }
    }
}

void printt_dizi(int third_pointer, int row, int eleman_sayi)
{
    int ort_temp_sol_ust = 0;
    int ort_temp_sag_ust = 0;
    int ort_temp_orta_ust = 0;
    int ort_temp_orta_alt = 0;
    int ort_temp_sol_alt = 0;
    int ort_temp_sag_alt = 0; 
    for (size_t h = 0; h < third_pointer; h++)
    {
        for (size_t i = 0; i < row; i++)
        {
            for (size_t j = 0; j < eleman_sayi; j++)
            {
                Serial.print(main_veri[h][i][j]);
                if (i <= 14) 
                {
                    if (j <= 12) // sol alt taraf
                    {
                        ort_temp_sol_alt += main_veri[h][i][j];
                    }
                    if (j > 12 && j < 19) // orta alt taraf
                    {
                        ort_temp_orta_alt += main_veri[h][i][j];
                    }
                    if (j >= 19 && j < 30) //  sol yukarı taraf
                    {
                        ort_temp_sol_ust += main_veri[h][i][j];
                    }
                }
                if (i > 14 && i < 30) // sağ taraf
                {
                    if (j <= 12) // sağ alt
                    {
                        ort_temp_sag_alt += main_veri[h][i][j];
                    }
                    if (j > 12 && j < 19) // orta üst taraf
                    {
                        ort_temp_orta_ust += main_veri[h][i][j];
                    }
                    if (j >= 19 && j < 30) //  sağ yukarı taraf
                    {
                        ort_temp_sag_ust += main_veri[h][i][j];
                    }
                }
            }
        }
    }
    Serial.println(ort_temp_sol_ust)
    Serial.println(ort_temp_sag_ust)
    Serial.println(ort_temp_orta_ust)
    Serial.println(ort_temp_orta_alt)
    Serial.println(ort_temp_sol_alt)
    Serial.println(ort_temp_sag_alt)

    client.publish(mqtt_topic, create_json_string(int(ort_temp_sol_ust / (14 * 12)), int(ort_temp_orta_ust / (14 * 7)), int(ort_temp_sag_ust / (14 * 12)), int(ort_temp_sol_alt / (17 * 12)), int(ort_temp_orta_alt / (17 * 7)), int(ort_temp_sag_alt / (17 * 12))));
    ort_temp_sol_ust = 0;
    ort_temp_sag_ust = 0;
    ort_temp_orta_ust = 0;
    ort_temp_orta_alt = 0;
    ort_temp_sol_alt = 0;
    ort_temp_sag_alt = 0;
}

void setup2()
{
    // durum adlandirma
    string_update("tamamen duzgun", 0);
    // string_update("yarim dogru", 1);
    //_All_pozisyon doldurulacak ama buradakaki veriler hali hazırda zaten elimizde olucak
    // all_update(0,(int **[]){(int *[]){(int []){0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},(int []){0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},(int []){0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},(int []){0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},(int []){0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},(int []){0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},(int []){0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},(int []){0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},(int []){0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},(int []){0,0,0,0,0,0,-97634488,5442,0,3,0,931766,-294065,346865086,295333474,310806428,-50010211,0,2,0,931766,0,0,0,0,0,0,0},(int []){0,0,0,0,0,0,83697200,5460,-97660928,5442,-97614309,5442,83697125,5460,83697037,5460,83697314,5460,-97633786,5442,0,0,23210,0,22737,0,22737,0},(int []){0,0,0,0,23210,0,279893,0,279472,0,279472,0,23210,0,0,0,279893,0,333141,0,332576,0,332576,0,279893,0,0,0},(int []){333141,0,337237,0,336880,0,339557,0,332458,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},(int []){0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},(int []){0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},(int []){0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},(int []){0,0,0,0,0,0,0,0,0,0,-97630140,5442,313174613,0,83697314,5460,-97660928,5442,-97633856,5442,1,0,10,0,10,0,10,0},(int []){130,0,130,0,1,0,0,0,304752,0,304752,0,304752,0,4,0,4,0,2,0,0,0,0,0,0,0,0,0},(int []){22737,0,22737,0,682,0,0,0,23210,0,23210,0,23210,0,256262,0,256262,0,682,0,0,0,279893,0,279893,0,279893,0},(int []){52683,0,52683,0,682,0,0,1,332780,0,333462,0,333462,0,3417,0,6094,0,682,0,0,1,334997,0,335680,0,335680,0},(int []){80,0,80,0,1,0,0,0,141,0,141,0,141,0,5,0,5,0,1,0,0,0,146,0,146,0,146,0},(int []){11,0,11,0,0,0,1,0,332780,0,333462,0,333462,0,2,0,24,0,1,0,280897080,0,141,0,141,0,141,0},(int []){5,0,5,0,1,0,280897080,0,304756,0,304756,0,304756,0,4046,0,4046,0,0,0,280897080,1,0,0,0,0,0,0},(int []){0,0,0,0,2,0,280897080,0,332780,0,333462,0,333462,0,2409,0,2409,0,0,0,83697338,5460,-97634364,5442,332780,0,333462,0},(int []){-97661338,5442,2,0,339557,0,0,0,83697125,5460,0,0,-97606397,5442,104743113,3671,1,0,-97663658,5442,-97606938,5442,83697338,5460,0,0,83697013,5460},(int []){11051,0,306410,0,11051,0,306410,0,0,0,5543,0,0,0,0,0,338265,0,682,0,661,0,278799814,0,47990706,0,274882446,0},(int []){0,0,276432650,0,63421590,0,0,0,0,0,0,0,0,0,0,0,-97606930,5442,-97626288,5442,-97607664,5442,0,0,0,0,-97632046,5442},(int []){0,0,0,0,83697337,5460,0,0,0,0,0,0,-97606397,5442,-97606930,5442,0,0,0,0,1,0,0,0,0,0,0,0}},(int *[]){(int []){-97606397,5442,83697901,5460,138,0,196567274,8399573,0,0,677205,0,24650,0,10,0,337540,0,0,611680,699053,731830,1,0,10,0,10,0},(int []){10,0,130,0,130,0,1,0,0,0,304752,0,304752,0,304752,0,4,0,4,0,2,0,0,0,0,0,0,0},(int []){0,0,22737,0,22737,0,682,0,0,0,23210,0,23210,0,23210,0,256262,0,256262,0,682,0,0,0,279893,0,279893,0},(int []){279893,0,52683,0,52683,0,682,0,0,1,332780,0,333462,0,333462,0,3417,0,6094,0,682,0,0,1,334997,0,335680,0},(int []){335680,0,80,0,80,0,1,0,0,0,141,0,141,0,141,0,5,0,5,0,1,0,0,0,146,0,146,0},(int []){146,0,11,0,11,0,0,0,1,0,332780,0,333462,0,333462,0,2,0,24,0,1,0,280897080,0,141,0,141,0},(int []){141,0,5,0,5,0,1,0,280897080,0,304756,0,304756,0,304756,0,4046,0,4046,0,0,0,280897080,1,0,0,0,0},(int []){0,0,0,0,0,0,2,0,280897080,0,332780,0,333462,0,83697576,5460,83697765,5460,0,0,-97660928,5442,-97984600,5442,0,0,-97628625,5442},(int []){0,0,-97618742,5442,83697570,5460,0,0,-97628634,5442,83697576,5460,83697570,5460,0,0,83697570,5460,-97621810,5442,10922,0,-344631978,-84827,-97665216,5442,83697501,5460},(int []){0,0,-97660712,5442,0,0,339105109,0,104743113,3671,0,0,335544320,-84827,0,0,0,0,-97660713,5442,0,0,83697498,5460,0,0,0,0},(int []){0,0,-97660714,5442,0,0,0,0,83697765,5460,-97628221,5442,0,5442,-97607257,5442,0,0,-97628442,5442,0,3671,-97660928,5442,83697530,5460,-97628442,5442},(int []){0,0,-97606930,5442,83697536,5460,0,0,0,0,0,0,0,0,0,0,83697541,5460,83697586,5460,83697536,5460,0,0,0,0,83697530,5460},(int []){0,0,0,0,83697530,5460,-97606930,5442,83697576,5460,0,5460,0,0,-97988666,5442,0,0,0,0,0,0,0,0,-97660928,5442,0,0},(int []){-97988666,5442,-97984600,5442,-97607257,5442,0,0,83697586,5460,170,0,-97607257,5442,0,0,0,0,0,0,0,0,0,0,0,0,0,0},(int []){0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},(int []){0,0,0,0,0,0,0,0,0,0,0,0,0,0,-97631387,5442,-97660928,5442,289402639,0,-97638848,5442,-97660513,5442,-97999596,5442,-97631209,5442},(int []){32,0,-97988666,5442,-97660928,5442,83697665,5460,83697664,5460,0,0,-97988666,5442,0,0,0,0,-97998146,5442,-97638666,5442,289402639,0,4521916,0,83697664,5460},(int []){0,0,83697698,5460,-97638848,5442,83697696,5460,0,0,-97660709,5442,0,0,0,0,0,0,0,0,0,0,289402639,0,-97606785,5442,83697745,5460},(int []){-97638666,5442,83697696,5460,83697698,5460,-97630807,5442,0,0,-97660513,5442,1,0,0,0,0,0,-97607257,5442,83697710,5460,-97660513,5442,-97607257,5442,-97606785,5442},(int []){0,0,83762593,5460,-97613706,5442,0,0,0,0,83697710,5460,-97997362,5442,-97660928,5442,83697792,5460,83697741,5460,0,0,83697709,5460,83697749,5460,-97626288,5442},(int []){-97608054,5442,-97638413,5442,83697765,5460,1,0,1,1,-97638848,5442,-97607257,5442,-97629562,5442,1,0,-97629198,5442,83697736,5460,-97983558,5442,0,0,0,0},(int []){83697757,5460,0,0,0,0,0,0,-97638744,5442,-97606785,5442,83697757,5460,-97639082,5442,-97638652,5442,-97638880,5442,-97607668,5442,0,0,0,0,-97997362,5442},(int []){-97638613,5442,204,0,7,0,0,0,0,0,0,0,0,0,0,0,-97660336,5442,-16534806,18,0,0,-97606930,5442,-1,0,-97621354,5442},(int []){83697850,5460,-97636022,5442,0,0,0,0,-104743047,-3671,0,0,0,0,0,0,-97606698,5442,-97660021,5442,-97606938,5442,83697792,5460,-97607257,5442,0,0},(int []){0,0,0,0,0,0,0,0,83762609,5460,0,0,0,5,0,0,0,0,0,0,0,0,10294185,0,0,0,0,0},(int []){0,0,0,0,0,0,0,0,0,0,96,138,149,149,149,149,149,149,149,149,149,149,149,149,149,149,149,149},(int []){149,149,149,149,149,149,149,149,149,149,149,149,0,0,42,10,0,0,0,0,0,0,0,0,0,0,0,0},(int []){0,0,0,0,0,0,0,0,104742922,3671,2629205,0,32,0,83697857,5460,83697857,5460,104750527,3671,-97661486,5442,104750514,3671,0,0,104743626,3671}}});
    // all_update(1,(int **[]){(int *[]){(int []){110,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},(int []){0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},(int []){0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},(int []){0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},(int []){0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},(int []){0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},(int []){0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},(int []){0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},(int []){0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},(int []){0,0,0,0,0,0,-218334734,5423,0,3,0,931766,-294065,346865086,295333474,310806428,-50010211,0,2,0,931766,0,0,0,0,0,0,0},(int []){0,0,0,0,0,0,78481482,5461,-218361173,5423,-218314554,5423,78481408,5461,78481320,5461,78481597,5461,-218334031,5423,0,0,23210,0,22737,0,22737,0},(int []){0,0,0,0,23210,0,279893,0,279472,0,279472,0,23210,0,0,0,279893,0,333141,0,332576,0,332576,0,279893,0,0,0},(int []){333141,0,337237,0,336880,0,339557,0,332458,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},(int []){0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},(int []){0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},(int []){0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},(int []){0,0,0,0,0,0,0,0,0,0,-218330386,5423,313174613,0,78481597,5461,-218361173,5423,-218334101,5423,1,0,10,0,10,0,10,0},(int []){130,0,130,0,1,0,0,0,304752,0,304752,0,304752,0,4,0,4,0,2,0,0,0,0,0,0,0,0,0},(int []){22737,0,22737,0,682,0,0,0,23210,0,23210,0,23210,0,256262,0,256262,0,682,0,0,0,279893,0,279893,0,279893,0},(int []){52683,0,52683,0,682,0,0,1,332780,0,333462,0,333462,0,3417,0,6094,0,682,0,0,1,334997,0,335680,0,335680,0},(int []){80,0,80,0,1,0,0,0,141,0,141,0,141,0,5,0,5,0,1,0,0,0,146,0,146,0,146,0},(int []){11,0,11,0,0,0,1,0,332780,0,333462,0,333462,0,2,0,24,0,1,0,280897080,0,141,0,141,0,141,0},(int []){5,0,5,0,1,0,280897080,0,304756,0,304756,0,304756,0,4046,0,4046,0,0,0,280897080,1,0,0,0,0,0,0},(int []){0,0,0,0,2,0,280897080,0,332780,0,333462,0,333462,0,2409,0,2409,0,0,0,78481621,5461,-218334609,5423,332780,0,333462,0},(int []){-218361584,5423,2,0,339557,0,0,0,78481408,5461,0,0,-218306642,5423,125036745,3656,1,0,-218363904,5423,-218307184,5423,78481621,5461,0,0,78481296,5461},(int []){11051,0,306410,0,11051,0,306410,0,0,0,5543,0,0,0,0,0,338265,0,682,0,661,0,278799814,0,47990706,0,274882446,0},(int []){0,0,276432650,0,63421590,0,0,0,0,0,0,0,0,0,0,0,-218307176,5423,-218326534,5423,-218307909,5423,0,0,0,0,-218332291,5423},(int []){0,0,0,0,78481620,5461,0,0,0,0,0,0,-218306642,5423,-218307176,5423,0,0,0,0,1,0,0,0,0,0,0,0}},(int *[]){(int []){-218306642,5423,78482184,5461,138,0,196567274,8399573,0,0,677205,0,24650,0,10,0,337540,0,0,611680,699053,731830,1,0,10,0,10,0},(int []){10,0,130,0,130,0,1,0,0,0,304752,0,304752,0,304752,0,4,0,4,0,2,0,0,0,0,0,0,0},(int []){0,0,22737,0,22737,0,682,0,0,0,23210,0,23210,0,23210,0,256262,0,256262,0,682,0,0,0,279893,0,279893,0},(int []){279893,0,52683,0,52683,0,682,0,0,1,332780,0,333462,0,333462,0,3417,0,6094,0,682,0,0,1,334997,0,335680,0},(int []){335680,0,80,0,80,0,1,0,0,0,141,0,141,0,141,0,5,0,5,0,1,0,0,0,146,0,146,0},(int []){146,0,11,0,11,0,0,0,1,0,332780,0,333462,0,333462,0,2,0,24,0,1,0,280897080,0,141,0,141,0},(int []){141,0,5,0,5,0,1,0,280897080,0,304756,0,304756,0,304756,0,4046,0,4046,0,0,0,280897080,1,0,0,0,0},(int []){0,0,0,0,0,0,2,0,280897080,0,332780,0,333462,0,78481858,5461,78482048,5461,0,0,-218361173,5423,-218684846,5423,0,0,-218328871,5423},(int []){0,0,-218318987,5423,78481853,5461,0,0,-218328880,5423,78481858,5461,78481853,5461,0,0,78481853,5461,-218322056,5423,10922,0,352321536,-41295,-218365461,5423,78481784,5461},(int []){0,0,-218360957,5423,0,0,-287244288,0,125036745,3656,0,0,316669952,-41295,0,0,0,0,-218360958,5423,0,0,78481781,5461,0,0,0,0},(int []){0,0,-218360960,5423,0,0,0,0,78482048,5461,-218328466,5423,0,5423,-218307502,5423,0,0,-218328687,5423,0,3656,-218361173,5423,78481813,5461,-218328687,5423},(int []){0,0,-218307176,5423,78481818,5461,0,0,0,0,0,0,0,0,0,0,78481824,5461,78481869,5461,78481818,5461,0,0,0,0,78481813,5461},(int []){0,0,0,0,78481813,5461,-218307176,5423,78481858,5461,0,5461,0,0,-218688912,5423,0,0,0,0,0,0,0,0,-218361173,5423,0,0},(int []){-218688912,5423,-218684846,5423,-218307502,5423,0,0,78481869,5461,170,0,-218307502,5423,0,0,0,0,0,0,0,0,0,0,0,0,0,0},(int []){0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},(int []){0,0,0,0,0,0,0,0,0,0,0,0,0,0,-218331632,5423,-218361173,5423,289402639,0,-218339093,5423,-218360758,5423,-218699842,5423,-218331454,5423},(int []){32,0,-218688912,5423,-218361173,5423,78481948,5461,78481947,5461,0,0,-218688912,5423,0,0,0,0,-218698392,5423,-218338911,5423,289402639,0,4521916,0,78481947,5461},(int []){0,0,78481981,5461,-218339093,5423,78481978,5461,0,0,-218360954,5423,0,0,0,0,0,0,0,0,0,0,289402639,0,-218307030,5423,78482028,5461},(int []){-218338911,5423,78481978,5461,78481981,5461,-218331053,5423,0,0,-218360758,5423,1,0,0,0,0,0,-218307502,5423,78481992,5461,-218360758,5423,-218307502,5423,-218307030,5423},(int []){0,0,78567500,5461,-218313952,5423,0,0,0,0,78481992,5461,-218697608,5423,-218361173,5423,78482074,5461,78482024,5461,0,0,78481992,5461,78482032,5461,-218326534,5423},(int []){-218308299,5423,-218338658,5423,78482048,5461,1,0,1,1,-218339093,5423,-218307502,5423,-218329807,5423,1,0,-218329443,5423,78482018,5461,-218683804,5423,0,0,0,0},(int []){78482040,5461,0,0,0,0,0,0,-218338989,5423,-218307030,5423,78482040,5461,-218339328,5423,-218338898,5423,-218339125,5423,-218307913,5423,0,0,0,0,-218697608,5423},(int []){-218338858,5423,204,0,7,0,0,0,0,0,0,0,0,0,0,0,-218360581,5423,34112929,241,0,0,-218307176,5423,-1,0,-218321600,5423},(int []){78482133,5461,-218336267,5423,0,0,0,0,-125036679,-3656,0,0,0,0,0,0,-218306944,5423,-218360266,5423,-218307184,5423,78482048,5461,-218307502,5423,0,0},(int []){0,0,0,0,0,0,0,0,78567516,5461,0,0,0,5,0,0,0,0,0,0,0,0,10294185,0,0,0,0,0},(int []){0,0,0,0,0,0,0,0,0,0,96,138,149,149,149,149,149,149,149,149,149,149,149,149,149,149,149,149},(int []){149,149,149,149,149,149,149,149,149,149,149,149,0,0,42,10,0,0,0,0,0,0,0,0,0,0,0,0},(int []){0,0,0,0,0,0,0,0,125036554,3656,2629205,0,32,0,78482139,5461,78482139,5461,125037738,3656,-218361732,5423,125037725,3656,0,0,125037258,3656}}}); // durum 0 dan aldığımız verileri direk buraya yapıştırırız stringteki durumlarla eş gittiğinden emin olalım
    // all_update(0,(int **[]){(int *[]){(int []){291,336,339,355,373,367,356,350,347,346,335,334,336,362,467,431,313,306,306,309,291,311,319,335,315,325,336,280,275,210},(int []){403,443,451,475,492,491,486,480,467,470,458,447,451,493,600,580,434,425,423,423,397,427,411,430,416,424,431,378,377,299},(int []){548,595,601,634,653,654,646,641,627,627,605,584,597,640,774,727,553,558,561,554,533,547,540,561,539,546,553,503,488,394},(int []){555,599,609,646,668,663,658,656,637,654,629,607,607,628,775,712,565,570,579,565,538,555,564,579,549,567,590,535,517,417},(int []){586,637,645,689,706,716,698,697,675,679,669,647,637,656,792,723,596,591,607,589,563,567,576,599,580,593,611,554,525,417},(int []){637,691,695,742,763,768,757,752,736,745,728,705,696,718,857,772,631,625,663,640,608,606,619,645,623,619,633,578,544,446},(int []){601,671,674,721,746,752,740,725,711,728,704,684,684,696,829,748,637,613,649,611,589,582,609,644,626,607,602,548,534,441},(int []){646,719,731,768,789,806,787,772,765,769,739,715,726,735,868,773,668,633,663,623,611,612,617,632,625,631,624,563,536,411},(int []){618,688,695,742,752,768,755,745,739,747,720,675,697,702,845,724,613,595,616,598,586,579,585,602,601,612,611,531,514,383},(int []){598,663,663,709,710,725,715,724,708,724,691,643,650,660,804,676,586,569,605,570,580,557,565,583,566,561,557,484,469,358},(int []){657,717,723,773,787,799,784,776,766,760,745,710,707,727,875,743,641,624,664,636,633,591,598,621,610,614,600,527,508,384},(int []){733,803,808,867,856,871,855,839,831,831,806,772,760,771,932,807,697,670,706,677,682,650,662,693,674,677,651,568,557,434},(int []){762,825,836,876,891,905,894,883,859,862,831,800,795,799,961,847,701,684,727,696,701,662,671,687,684,681,660,567,558,408},(int []){717,778,795,840,853,866,854,831,805,805,782,748,737,747,895,785,665,631,667,643,616,597,607,626,620,617,603,524,517,375},(int []){826,880,901,964,971,972,952,923,900,904,876,834,820,843,1032,913,750,724,748,725,697,676,693,711,710,705,695,612,604,451},(int []){876,947,966,1013,1035,1052,1032,1009,990,989,960,926,921,962,1228,1098,837,790,828,785,753,727,738,762,764,762,755,688,677,543},(int []){580,628,635,661,672,703,693,680,669,695,670,638,644,627,818,716,533,514,538,513,491,473,485,500,500,499,494,446,443,354},(int []){614,655,667,686,693,722,697,686,681,667,657,632,610,608,772,653,516,486,522,503,478,460,476,494,490,487,482,429,427,341},(int []){453,484,499,523,527,545,533,524,508,523,502,516,454,453,583,481,375,356,381,366,352,333,347,360,362,358,358,312,311,240},(int []){470,505,511,520,529,569,557,540,527,518,496,478,455,467,585,497,392,378,398,394,378,363,365,378,382,378,369,322,320,246},(int []){552,572,570,600,600,615,619,604,590,594,567,535,521,536,650,551,454,439,467,462,438,428,431,441,439,434,425,368,367,282},(int []){485,509,523,542,540,561,561,551,530,524,505,486,472,476,589,509,465,422,430,418,400,397,395,405,402,412,406,348,350,263},(int []){575,604,615,631,634,649,660,655,623,613,591,567,553,557,678,573,491,488,522,516,484,473,466,470,465,473,466,401,397,297},(int []){541,562,572,598,595,603,611,609,586,581,560,531,516,526,656,542,457,457,501,467,449,435,438,445,453,436,433,372,375,284},(int []){518,552,566,601,615,636,624,611,593,604,580,546,521,528,639,553,474,468,497,477,450,443,442,458,451,444,441,391,401,304},(int []){529,561,591,615,651,668,669,633,601,594,571,546,541,540,653,568,487,475,491,483,446,446,451,465,467,468,466,408,412,314},(int []){450,489,513,531,545,562,565,543,528,535,499,468,460,463,562,497,418,405,423,430,390,399,396,408,412,406,408,357,359,276},(int []){366,387,403,414,455,457,462,449,429,431,409,385,388,398,491,421,350,347,359,367,328,334,329,337,339,335,342,312,309,230},(int []){314,340,348,364,382,387,387,379,375,374,365,352,356,361,450,383,320,315,324,321,300,306,307,322,313,311,323,284,296,214},(int []){330,360,366,383,397,402,401,402,402,401,385,382,366,372,461,392,330,337,340,334,322,315,319,331,341,332,347,301,295,222}}});
}

void loop()
{

    if (!client.connected())
    {
        reconnect();
    }
    client.loop();

    int setup_first = 1;
    // int mod = 0; //-> veri toplama
    int mod = 1; //-> veri işleme
    if (mod == 0)
    {
        int f = 0;
        int alt_ust = 0;
        while (f < 6)
        {
            for (size_t i = 0; i < 30; i++)
            {
                bir_fonk(i % 16, 0);
                analogWrite(signal_pin[0][i / 16], 255);
                delay(10);
                for (size_t j = 0; j < 30; j++)
                {
                    bir_fonk(j % 16, 1);
                    temp[f][0][i][j] = analogRead(signal_pin[1][j / 16]);
                    delay(3);
                }
                analogWrite(signal_pin[0][i / 16], 0);
                delay(10);
            }
            f++;
        }
        // Serial.print("verilerimiz alindi");
        delay(3);
        // verileri işle
        for (int h = 0; k < 1; k++)
        {
            for (int i = 0; i < 30; i++)
            {
                for (int j = 0; j < 30; j++)
                {
                    main_veri[h][i][j] = (temp[0][h][i][j] + temp[1][h][i][j] + temp[2][h][i][j] + temp[3][h][i][j] + temp[4][h][i][j] + temp[5][h][i][j]) / 6;
                }
            }
        }

        // verileri print et
        printt_dizi(alt_ust_matris_sayisi, dikey_kutucuk_sayisi, yatay_kutucuk_sayisi);
    }
    if (mod == 1)
    {
        // ALL_UPDATE fonksiyonu kullanılarak daha once elde edilen veriler all_update doldurulur
        if (setup_first++)
            setup2();

        int f = 0;
        int alt_ust = 0;
        while (f < 1)
        {
            // MAİN.C DEKİ YERŞ ALABİLİRSİN GEREKİRSE
            for (int i = 0; i < 30; i++)
            {
                bir_fonk(i % 16, 0);
                analogWrite(signal_pin[0][i / 16], 255);
                delay(10);
                for (size_t j = 0; j < 30; j++)
                {
                    bir_fonk(j % 16, 1);
                    degerlendirilecek_veri[0][i][j] = analogRead(signal_pin[1][j / 16]);
                    delay(3);
                }
                analogWrite(signal_pin[0][i / 16], 0);
                delay(10);
            }
            f++;
        }
        Serial.print("verilerimiz alindi");
        delay(3);
        dizi_karsilastir_yuzdeye_gore_dogrilik_ogren();
    }

    // MQTT Mesaj Gönderme
}

// kodumuzda 2 tane kısım vardır
//  1.kısımda veri alma kısmı mesala dik doğru poizsyonunun verilerini alacağız ve daha sonra bu verileri programımıza dahil edip 2.kısıma geçeceğiz
//  yani basit bir şekilde örnek vermek gerekirse 1.kısımda 50 değerini dik doğru pozisyon durumu için aldıysam programıma bunu el ile girip 2.kısımda
//  çalıştıracağım ve tekrar programım 50 değerine yakın bir değer verdiğinde ben şunu anlamış olacağım bu dik doğru pozisyon durumudur
//  bunu her durum için istediğimiz sayıda yapabiliriz , yanlış bir pozisyon içinde, spesifik bir durum içinde, kaç tane durum olacağı tamamen bize kalmış

// verileri alırken daha stabil veri alabilmek için 6 kerek aynı noktaların (karelerin) değerlerini alır ve her karenin ortalamalarını
// kaydederiz

// 2.kısımda programımızın hangi durumda olduğuna karar verebilmek için elimizdeki her durum matrisiyle bir karşılaştırma yapıp
// hangi duruma denk olduğunu anlamamız gerekir burada uyguladığımız yöntem her bir durum matrisini kutucuğunun o anlık verilerler karşılaştırılıp
// yüzde olarak en çok hangisine benzer ise o durum matrisi gerçekleştirildiği varsayılır burada yüzde oranları keyfimize ve programın verimliliğine bağlı
// değiştirilecektir daha kesin sonuçlar elde edebilmek için
// her bir kutucuğun yüzde olarak değerleri karşılaştırıldıktan sonra tüm kutucukların sayısının yüzde x i kadarı yakın olması gerekir mesala elimizde 2 durum var
// birinci durumla benzer kutucukların sayısı 300 2.durumla 400 ise 2.durum seçilir