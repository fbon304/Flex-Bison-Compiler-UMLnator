#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include "Utils.h"

#define TIME_PATTERNS_COUNT 9
#define DATE_PATTERNS_COUNT 16
#define TIMESTAMP_PATTERNS_COUNT 3

static unsigned int g_fileIndex = 0;

/*static char *build_filepath(const char *env_var_name,
                            const char *filename_prefix,
                            const char *extension,
                            unsigned int index)
{
    const char *base = getStringOrDefault(env_var_name, "/src/test/c/result");
    if (!base || !*base) {
        fprintf(stderr, "Error: environment variable %s not set\n", env_var_name);
        return NULL;
    }

    size_t needed = strlen(base)   
                  + 1              
                  + strlen(filename_prefix)
                  + 1 
                  + 20 
                  + strlen(extension) 
                  + 1;           

    char *buf = malloc(needed);
    if (!buf) return NULL;

    // e.g. "/path" + "/" + "log" + "_" + "42" + ".txt"
    snprintf(buf, needed, "%s/%s_%u%s",
             base, filename_prefix, index, extension);

    return buf;
}

static FILE *open_session_file(void)
{
    char *path = build_filepath("MY_APP_PATH", "session", ".log", g_fileIndex++);
    if (!path) return NULL;

    FILE *f = fopen(path, "r+");
    if (!f) {
        f = fopen(path, "w+");
        if (!f) {
            fprintf(stderr, "Failed to create %s: %s\n", path, strerror(errno));
        }
    }
    free(path);
    return f;
}*/

static int _match(const char *str, const char *pattern)
{
    regex_t re;
    int ret;

    ret = regcomp(&re, pattern, REG_EXTENDED | REG_NOSUB);
    if (ret)
    {
        char errbuf[128];
        regerror(ret, &re, errbuf, sizeof(errbuf));
        fprintf(stderr, "regex compile error: %s\n", errbuf);
        return 0;
    }

    ret = (regexec(&re, str, 0, NULL, 0) == 0);

    regfree(&re);
    return ret;
}

boolean isTimestamp(char * ts) {
    static const char * patterns[] = {
        "^([0-9]{4}-[0-9]{2}-[0-9]{2}|[0-9]{8})[ T]"
        "([01][0-9]|2[0-4]):[0-5][0-9]"
        "(:[0-5][0-9](\\.[0-9]+)?)?"
        "( ?(Z|[-+][0-9]{1,4}(:[0-9]{2})?))?"
        "( (AD|BC))?$",

        "^(January|February|March|April|May|June|July|August|September|October|November|December)"
        " ([1-9]|[12][0-9]|3[01]) "
        "([01]?[0-9]|2[0-3]):[0-5][0-9]"
        "(:[0-5][0-9](\\.[0-9]+)?)?"
        "( (AM|PM))? "
        "([0-9]{4}|[0-9]{2})"
        "( [A-Za-z/_]+)?"
        "( (AD|BC))?$",

        "^(Jan|Feb|Mar|Apr|May|Jun|Jul|Aug|Sep|Oct|Nov|Dec)"
        "-(0[1-9]|[12][0-9]|3[01])"
        "-([0-9]{2}|[0-9]{4}) "
        "(0?[1-9]|1[0-2]):[0-5][0-9](:[0-5][0-9])? "
        "(AM|PM)"
        "( [A-Za-z/_]+)?"
        "( (AD|BC))?$"};

    for (int i = 0; i < TIMESTAMP_PATTERNS_COUNT; i++) {
        if (_match(ts, patterns[i]))
            return true;
    }
    return false;
}

boolean isTime(char * time) {
    static const char * timePatterns[] = {
        "^(0[0-9]|1[0-9]|2[0-4]):([0-5][0-9]|60):([0-5][0-9]|60)(.[0-9]{3})?",
        "^(0[0-9]|1[0-9]|2[0-4]):([0-5][0-9]|60)((\\+|-)((0?[0-9]|1[0-9]|2[0-4])|((0?[0-9]|1[0-9]|2[0-4]):(0[0-9]|[0-5][0-9]))))?$",
        "^(0[0-9]|1[0-2]):([0-5][0-9]|60) (AM|PM)$",
        "^(0[0-9]|1[0-9]|2[0-4])([0-5][0-9]|60)([0-5][0-9]|60)(-((0?[0-9]|1[0-9]|2[0-4])|((0?[0-9]|1[0-9]|2[0-4]):(0[0-9]|[0-5][0-9]))))?$",
        "^(0[0-9]|1[0-9]|2[0-4])([0-5][0-9]|60)([0-5][0-9]|60)((\\+|-)(0[0-9]|1[0-9]|2[0-3])(0[0-9]|[1-5][0-9]|60)?)?$",
        "^(0[0-9]|1[0-9]|2[0-4])([0-5][0-9]|60)([0-5][0-9]|60)((\\+|-)(0[0-9]|1[0-9]|2[0-3]):(0[0-9]|[1-5][0-9]|60)(:(0[0-9]|[1-5][0-9]|60))?)?$",
        "^(0[0-9]|1[0-9]|2[0-4]):([0-5][0-9]|60):([0-5][0-9]|60)(.[0-9]{3})?-((0?[0-9]|1[0-9]|2[0-4])|((0?[0-9]|1[0-9]|2[0-4]):(0[0-9]|[0-5][0-9])))$",
        "^(0[0-9]|1[0-9]|2[0-4]):([0-5][0-9]|60):([0-5][0-9]|60) (ACDT|ACSST|ACST|ACT|ACWST|ADT|AEDT|AESST|AEST|AFT|AKDT|AKST|ALMST|ALMT|AMST|AMT|ANAST|ANAT|ARST|ART|AST|AWSST|AWST|AZOST|AZOT|AZST|AZT|BDST|BDT|BNT|BORT|BOT|BRA|BRST|BRT|BST|BTT|CADT|CAST|CCT|CDT|CEST|CET|CETDST|CHADT|CHAST|CHUT|CKT|CLST|CLT|COT|CST|CXT|DAVT|DDUT|EASST|EAST|EAT|EDT|EEST|EET|EETDST|EGST|EGT|EST|FET|FJST|FJT|FKST|FKT|FNST|FNT|GALT|GAMT|GEST|GET|GFT|GILT|GMT|GYT|HKT|HST|ICT|IDT|IOT|IRKST|IRKT|IRT|IST|JAYT|JST|KDT|KGST|KGT|KOST|KRAST|KRAT|KST|LHDT|LHST|LIGT|LINT|LKT|MAGST|MAGT|MART|MAWT|MDT|MEST|MESZ|MET|METDST|MEZ|MHT|MMT|MPT|MSD|MSK|MST|MUST|MUT|MVT|MYT|NDT|NFT|NOVST|NOVT|NPT|NST|NUT|NZDT|NZST|NZT|OMSST|OMST|PDT|PET|PETST|PETT|PGT|PHT|PKST|PKT|PMDT|PMST|PONT|PST|PWT|PYST|PYT|RET|SADT|SAST|SCT|SGT|TAHT|TFT|TJT|TKT|TMT|TOT|TRUT|TVT|UCT|ULAST|ULAT|UT|UTC|UYST|UYT|UZST|UZT|VET|VLAST|VLAT|VOLT|VUT|WADT|WAKT|WAST|WAT|WDT|WET|WETDST|WFT|WGST|WGT|XJT|YAKST|YAKT|YAPT|YEKST|YEKT|Z|ZULU)$",
        "^[0-9]{4}-(0[0-9]|[1-2][0-9]|3[0-1])-(0?[0-9]|1[0-2]) (0[0-9]|1[0-9]|2[0-4]):([0-5][0-9]|60):([0-5][0-9]|60) (Africa/Abidjan|Africa/Accra|Africa/Addis_Ababa|Africa/Algiers|Africa/Asmara|Africa/Asmera|Africa/Bamako|Africa/Bangui|Africa/Banjul|Africa/Bissau|Africa/Blantyre|Africa/Brazzaville|Africa/Bujumbura|Africa/Cairo|Africa/Casablanca|Africa/Ceuta|Africa/Conakry|Africa/Dakar|Africa/Dar_es_Salaam|Africa/Djibouti|Africa/Douala|Africa/El_Aaiun|Africa/Freetown|Africa/Gaborone|Africa/Harare|Africa/Johannesburg|Africa/Juba|Africa/Kampala|Africa/Khartoum|Africa/Kigali|Africa/Kinshasa|Africa/Lagos|Africa/Libreville|Africa/Lome|Africa/Luanda|Africa/Lubumbashi|Africa/Lusaka|Africa/Malabo|Africa/Maputo|Africa/Maseru|Africa/Mbabane|Africa/Mogadishu|Africa/Monrovia|Africa/Nairobi|Africa/Ndjamena|Africa/Niamey|Africa/Nouakchott|Africa/Ouagadougou|Africa/Porto-Novo|Africa/Sao_Tome|Africa/Timbuktu|Africa/Tripoli|Africa/Tunis|Africa/Windhoek|America/Adak|America/Anchorage|America/Anguilla|America/Antigua|America/Araguaina|America/Argentina/Buenos_Aires|America/Argentina/Catamarca|America/Argentina/ComodRivadavia|America/Argentina/Cordoba|America/Argentina/Jujuy|America/Argentina/La_Rioja|America/Argentina/Mendoza|America/Argentina/Rio_Gallegos|America/Argentina/Salta|America/Argentina/San_Juan|America/Argentina/San_Luis|America/Argentina/Tucuman|America/Argentina/Ushuaia|America/Aruba|America/Asuncion|America/Atikokan|America/Atka|America/Bahia|America/Bahia_Banderas|America/Barbados|America/Belem|America/Belize|America/Blanc-Sablon|America/Boa_Vista|America/Bogota|America/Boise|America/Buenos_Aires|America/Cambridge_Bay|America/Campo_Grande|America/Cancun|America/Caracas|America/Catamarca|America/Cayenne|America/Cayman|America/Chicago|America/Chihuahua|America/Ciudad_Juarez|America/Coral_Harbour|America/Cordoba|America/Costa_Rica|America/Creston|America/Cuiaba|America/Curacao|America/Danmarkshavn|America/Dawson|America/Dawson_Creek|America/Denver|America/Detroit|America/Dominica|America/Edmonton|America/Eirunepe|America/El_Salvador|America/Ensenada|America/Fort_Nelson|America/Fort_Wayne|America/Fortaleza|America/Glace_Bay|America/Godthab|America/Goose_Bay|America/Grand_Turk|America/Grenada|America/Guadeloupe|America/Guatemala|America/Guayaquil|America/Guyana|America/Halifax|America/Havana|America/Hermosillo|America/Indiana/Indianapolis|America/Indiana/Knox|America/Indiana/Marengo|America/Indiana/Petersburg|America/Indiana/Tell_City|America/Indiana/Vevay|America/Indiana/Vincennes|America/Indiana/Winamac|America/Indianapolis|America/Inuvik|America/Iqaluit|America/Jamaica|America/Jujuy|America/Juneau|America/Kentucky/Louisville|America/Kentucky/Monticello|America/Knox_IN|America/Kralendijk|America/La_Paz|America/Lima|America/Los_Angeles|America/Louisville|America/Lower_Princes|America/Maceio|America/Managua|America/Manaus|America/Marigot|America/Martinique|America/Matamoros|America/Mazatlan|America/Mendoza|America/Menominee|America/Merida|America/Metlakatla|America/Mexico_City|America/Miquelon|America/Moncton|America/Monterrey|America/Montevideo|America/Montreal|America/Montserrat|America/Nassau|America/New_York|America/Nipigon|America/Nome|America/Noronha|America/North_Dakota/Beulah|America/North_Dakota/Center|America/North_Dakota/New_Salem|America/Nuuk|America/Ojinaga|America/Panama|America/Pangnirtung|America/Paramaribo|America/Phoenix|America/Port-au-Prince|America/Port_of_Spain|America/Porto_Acre|America/Porto_Velho|America/Puerto_Rico|America/Punta_Arenas|America/Rainy_River|America/Rankin_Inlet|America/Recife|America/Regina|America/Resolute|America/Rio_Branco|America/Rosario|America/Santa_Isabel|America/Santarem|America/Santiago|America/Santo_Domingo|America/Sao_Paulo|America/Scoresbysund|America/Shiprock|America/Sitka|America/St_Barthelemy|America/St_Johns|America/St_Kitts|America/St_Lucia|America/St_Thomas|America/St_Vincent|America/Swift_Current|America/Tegucigalpa|America/Thule|America/Thunder_Bay|America/Tijuana|America/Toronto|America/Tortola|America/Vancouver|America/Virgin|America/Whitehorse|America/Winnipeg|America/Yakutat|America/Yellowknife|Antarctica/Casey|Antarctica/Davis|Antarctica/DumontDUrville|Antarctica/Macquarie|Antarctica/Mawson|Antarctica/McMurdo|Antarctica/Palmer|Antarctica/Rothera|Antarctica/South_Pole|Antarctica/Syowa|Antarctica/Troll|Antarctica/Vostok|Arctic/Longyearbyen|Asia/Aden|Asia/Almaty|Asia/Amman|Asia/Anadyr|Asia/Aqtau|Asia/Aqtobe|Asia/Ashgabat|Asia/Ashkhabad|Asia/Atyrau|Asia/Baghdad|Asia/Bahrain|Asia/Baku|Asia/Bangkok|Asia/Barnaul|Asia/Beirut|Asia/Bishkek|Asia/Brunei|Asia/Calcutta|Asia/Chita|Asia/Choibalsan|Asia/Chongqing|Asia/Chungking|Asia/Colombo|Asia/Dacca|Asia/Damascus|Asia/Dhaka|Asia/Dili|Asia/Dubai|Asia/Dushanbe|Asia/Famagusta|Asia/Gaza|Asia/Harbin|Asia/Hebron|Asia/Ho_Chi_Minh|Asia/Hong_Kong|Asia/Hovd|Asia/Irkutsk|Asia/Istanbul|Asia/Jakarta|Asia/Jayapura|Asia/Jerusalem|Asia/Kabul|Asia/Kamchatka|Asia/Karachi|Asia/Kashgar|Asia/Kathmandu|Asia/Katmandu|Asia/Khandyga|Asia/Kolkata|Asia/Krasnoyarsk|Asia/Kuala_Lumpur|Asia/Kuching|Asia/Kuwait|Asia/Macao|Asia/Macau|Asia/Magadan|Asia/Makassar|Asia/Manila|Asia/Muscat|Asia/Nicosia|Asia/Novokuznetsk|Asia/Novosibirsk|Asia/Omsk|Asia/Oral|Asia/Phnom_Penh|Asia/Pontianak|Asia/Pyongyang|Asia/Qatar|Asia/Qostanay|Asia/Qyzylorda|Asia/Rangoon|Asia/Riyadh|Asia/Saigon|Asia/Sakhalin|Asia/Samarkand|Asia/Seoul|Asia/Shanghai|Asia/Singapore|Asia/Srednekolymsk|Asia/Taipei|Asia/Tashkent|Asia/Tbilisi|Asia/Tehran|Asia/Tel_Aviv|Asia/Thimbu|Asia/Thimphu|Asia/Tokyo|Asia/Tomsk|Asia/Ujung_Pandang|Asia/Ulaanbaatar|Asia/Ulan_Bator|Asia/Urumqi|Asia/Ust-Nera|Asia/Vientiane|Asia/Vladivostok|Asia/Yakutsk|Asia/Yangon|Asia/Yekaterinburg|Asia/Yerevan|Atlantic/Azores|Atlantic/Bermuda|Atlantic/Canary|Atlantic/Cape_Verde|Atlantic/Faeroe|Atlantic/Faroe|Atlantic/Jan_Mayen|Atlantic/Madeira|Atlantic/Reykjavik|Atlantic/South_Georgia|Atlantic/St_Helena|Atlantic/Stanley|Australia/ACT|Australia/Adelaide|Australia/Brisbane|Australia/Broken_Hill|Australia/Canberra|Australia/Currie|Australia/Darwin|Australia/Eucla|Australia/Hobart|Australia/LHI|Australia/Lindeman|Australia/Lord_Howe|Australia/Melbourne|Australia/North|Australia/NSW|Australia/Perth|Australia/Queensland|Australia/South|Australia/Sydney|Australia/Tasmania|Australia/Victoria|Australia/West|Australia/Yancowinna|Brazil/Acre|Brazil/DeNoronha|Brazil/East|Brazil/West|Canada/Atlantic|Canada/Central|Canada/Eastern|Canada/Mountain|Canada/Newfoundland|Canada/Pacific|Canada/Saskatchewan|Canada/Yukon|CET|Chile/Continental|Chile/EasterIsland|CST6CDT|Cuba|EET|Egypt|Eire|EST|EST5EDT|Etc/GMT|Etc/GMT-0|Etc/GMT-1|Etc/GMT-10|Etc/GMT-11|Etc/GMT-12|Etc/GMT-13|Etc/GMT-14|Etc/GMT-2|Etc/GMT-3|Etc/GMT-4|Etc/GMT-5|Etc/GMT-6|Etc/GMT-7|Etc/GMT-8|Etc/GMT-9|Etc/GMT\\+0|Etc/GMT\\+1|Etc/GMT\\+10|Etc/GMT\\+11|Etc/GMT\\+12|Etc/GMT\\+2|Etc/GMT\\+3|Etc/GMT\\+4|Etc/GMT\\+5|Etc/GMT\\+6|Etc/GMT\\+7|Etc/GMT\\+8|Etc/GMT\\+9|Etc/GMT0|Etc/Greenwich|Etc/UCT|Etc/Universal|Etc/UTC|Etc/Zulu|Europe/Amsterdam|Europe/Andorra|Europe/Astrakhan|Europe/Athens|Europe/Belfast|Europe/Belgrade|Europe/Berlin|Europe/Bratislava|Europe/Brussels|Europe/Bucharest|Europe/Budapest|Europe/Busingen|Europe/Chisinau|Europe/Copenhagen|Europe/Dublin|Europe/Gibraltar|Europe/Guernsey|Europe/Helsinki|Europe/Isle_of_Man|Europe/Istanbul|Europe/Jersey|Europe/Kaliningrad|Europe/Kiev|Europe/Kirov|Europe/Kyiv|Europe/Lisbon|Europe/Ljubljana|Europe/London|Europe/Luxembourg|Europe/Madrid|Europe/Malta|Europe/Mariehamn|Europe/Minsk|Europe/Monaco|Europe/Moscow|Europe/Nicosia|Europe/Oslo|Europe/Paris|Europe/Podgorica|Europe/Prague|Europe/Riga|Europe/Rome|Europe/Samara|Europe/San_Marino|Europe/Sarajevo|Europe/Saratov|Europe/Simferopol|Europe/Skopje|Europe/Sofia|Europe/Stockholm|Europe/Tallinn|Europe/Tirane|Europe/Tiraspol|Europe/Ulyanovsk|Europe/Uzhgorod|Europe/Vaduz|Europe/Vatican|Europe/Vienna|Europe/Vilnius|Europe/Volgograd|Europe/Warsaw|Europe/Zagreb|Europe/Zaporozhye|Europe/Zurich|Factory|GB|GB-Eire|GMT|GMT-0|GMT\\+0|GMT0|Greenwich|Hongkong|HST|Iceland|Indian/Antananarivo|Indian/Chagos|Indian/Christmas|Indian/Cocos|Indian/Comoro|Indian/Kerguelen|Indian/Mahe|Indian/Maldives|Indian/Mauritius|Indian/Mayotte|Indian/Reunion|Iran|Israel|Jamaica|Japan|Kwajalein|Libya|MET|Mexico/BajaNorte|Mexico/BajaSur|Mexico/General|MST|MST7MDT|Navajo|NZ|NZ-CHAT|Pacific/Apia|Pacific/Auckland|Pacific/Bougainville|Pacific/Chatham|Pacific/Chuuk|Pacific/Easter|Pacific/Efate|Pacific/Enderbury|Pacific/Fakaofo|Pacific/Fiji|Pacific/Funafuti|Pacific/Galapagos|Pacific/Gambier|Pacific/Guadalcanal|Pacific/Guam|Pacific/Honolulu|Pacific/Johnston|Pacific/Kanton|Pacific/Kiritimati|Pacific/Kosrae|Pacific/Kwajalein|Pacific/Majuro|Pacific/Marquesas|Pacific/Midway|Pacific/Nauru|Pacific/Niue|Pacific/Norfolk|Pacific/Noumea|Pacific/Pago_Pago|Pacific/Palau|Pacific/Pitcairn|Pacific/Pohnpei|Pacific/Ponape|Pacific/Port_Moresby|Pacific/Rarotonga|Pacific/Saipan|Pacific/Samoa|Pacific/Tahiti|Pacific/Tarawa|Pacific/Tongatapu|Pacific/Truk|Pacific/Wake|Pacific/Wallis|Pacific/Yap|Poland|Portugal|PRC|PST8PDT|ROC|ROK|Singapore|Turkey|UCT|Universal|US/Alaska|US/Aleutian|US/Arizona|US/Central|US/East-Indiana|US/Eastern|US/Hawaii|US/Indiana-Starke|US/Michigan|US/Mountain|US/Pacific|US/Samoa|UTC|W-SU|WET|Zulu)$"};

    for (int i = 0; i < TIME_PATTERNS_COUNT; i++) {
        if (_match(time, timePatterns[i]))
            return true;
    }
    return false;
}

boolean isDate(char * date) {
    static const char * datePatterns[] = {
        "^[0-9]{4}-(0[1-9]|1[0-2])-(0[1-9]|[12][0-9]|3[01])$",

        "^[0-9]{8}$",
        "^[0-9]{2}(0[0-9]|1[0-2])(0[0-9]|[1-2][0-9]|3[0-1])$",

        "^[0-9]{4}\\.(00[1-9]|0[1-9][0-9]|[12][0-9]{2}|3[0-5][0-9]|36[0-6])$",

        "^J[0-9]{1,7}$",

        "^([0-9]|1[0-2])/(0[0-9]|[1-2][0-9]|3[0-1])/[0-9]{4}$",
        "^(|[0-9]|1[0-2])/(0[0-9]|[1-2][0-9]|3[0-1])/[0-9]{4}$",
        "^(0?[0-9]|[1-2][0-9]|3[0-1])/(0?[0-9]|1[0-2])/[0-9]{4}$",
        "^([0-9]|[1-2][0-9]|3[0-1])/(0[0-9]|[1-2][0-9]|3[0-1])/[0-9]{4}$",
        "^(0[0-9]|[1-2][0-9]|3[0-1])/(0[0-9]|[1-2][0-9]|3[0-1])/[0-9]{2}$",

        "^(January|February|March|April|May|June|July|August|September|October|November|December) (0?[1-9]|[12][0-9]|3[01]), ([0-9]{4}|[0-9]{2} (BC|AD))",
        "^(Jan|Feb|Mar|Apr|Jun|Jul|Aug|Sep|Oct|Nov|Dec)-(0[0-9]|[1-2][0-9]|3[0-1])-([0-9]{2}|[0-9]{4})$",
        "^(0[0-9]|[1-2][0-9]|3[0-1])-(Jan|Feb|Mar|Apr|Jun|Jul|Aug|Sep|Oct|Nov|Dec)-([0-9]{2}|[0-9]{4})$",
        "^[0-9]{2}-(Jan|Feb|Mar|Apr|Jun|Jul|Aug|Sep|Oct|Nov|Dec)-(0[0-9]|[1-2][0-9]|3[0-1])$",
        "^[0-9]{4}-(Jan|Feb|Mar|Apr|Jun|Jul|Aug|Sep|Oct|Nov|Dec)-(0[0-9]|[1-2][0-9]|3[0-1])$",
        "^(now|today|tomorrow|yesterday|infinity|-infinity|epoch|allballs)$"};

    for (int i = 0; i < DATE_PATTERNS_COUNT; i++) {
        if (_match(date, datePatterns[i]))
            return true;
    }
    return false;
}

boolean is_hex_char(char c) {
    return (c >= '0' && c <= '9') ||
           (c >= 'a' && c <= 'f') ||
           (c >= 'A' && c <= 'F');
}

boolean is_uuid(const char * uuid) {
    if (!uuid) return false;
    if (strlen(uuid) != 36) return false;

    const int groups[] = {8, 4, 4, 4, 12};
    int pos = 0;

    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < groups[j]; ++j) {
            if (!is_hex_char(uuid[pos++])) {
                return false;
            }
        }
        if (i < 4) {
            if (uuid[pos++] != '-') {
                return false;
            }
        }
    }

    return true;
}
