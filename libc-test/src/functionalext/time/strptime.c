/**
 * Copyright (c) 2022 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdlib.h>
#include <time.h>
#include "functionalext.h"

struct strptime_struct {
    const char *tz;
    const char *result;
};

static const struct strptime_struct gResultData[] = {
    {"Pacific/Rarotonga", "Sat Jul 30 00:40:14 2022"},
    {"Pacific/Wake", "Sat Jul 30 22:40:14 2022"},
    {"Pacific/Tarawa", "Sat Jul 30 22:40:14 2022"},
    {"Pacific/Fakaofo", "Sat Jul 30 23:40:14 2022"},
    {"Pacific/Apia", "Sat Jul 30 23:40:14 2022"},
    {"Pacific/Kwajalein", "Sat Jul 30 22:40:14 2022"},
    {"Pacific/Tahiti", "Sat Jul 30 00:40:14 2022"},
    {"Pacific/Easter", "Sat Jul 30 04:40:14 2022"},
    {"Pacific/Tongatapu", "Sat Jul 30 23:40:14 2022"},
    {"Pacific/Guadalcanal", "Sat Jul 30 21:40:14 2022"},
    {"Pacific/Chuuk", "Sat Jul 30 20:40:14 2022"},
    {"Pacific/Pitcairn", "Sat Jul 30 02:40:14 2022"},
    {"Pacific/Pohnpei", "Sat Jul 30 21:40:14 2022"},
    {"Pacific/Fiji", "Sat Jul 30 22:40:14 2022"},
    {"Pacific/Majuro", "Sat Jul 30 22:40:14 2022"},
    {"Pacific/Kiritimati", "Sun Jul 31 00:40:14 2022"},
    {"Pacific/Port_Moresby", "Sat Jul 30 20:40:14 2022"},
    {"Pacific/Galapagos", "Sat Jul 30 04:40:14 2022"},
    {"Pacific/Saipan", "Sat Jul 30 20:40:14 2022"},
    {"Pacific/Marquesas", "Sat Jul 30 01:10:14 2022"},
    {"Pacific/Wallis", "Sat Jul 30 22:40:14 2022"},
    {"Pacific/Palau", "Sat Jul 30 19:40:14 2022"},
    {"Pacific/Bougainville", "Sat Jul 30 21:40:14 2022"},
    {"Pacific/Honolulu", "Sat Jul 30 00:40:14 2022"},
    {"Pacific/Noumea", "Sat Jul 30 21:40:14 2022"},
    {"Pacific/Pago_Pago", "Fri Jul 29 23:40:14 2022"},
    {"Pacific/Guam", "Sat Jul 30 20:40:14 2022"},
    {"Pacific/Auckland", "Sat Jul 30 22:40:14 2022"},
    {"Pacific/Norfolk", "Sat Jul 30 21:40:14 2022"},
    {"Pacific/Kosrae", "Sat Jul 30 21:40:14 2022"},
    {"Pacific/Nauru", "Sat Jul 30 22:40:14 2022"},
    {"Pacific/Chatham", "Sat Jul 30 23:25:14 2022"},
    {"Pacific/Efate", "Sat Jul 30 21:40:14 2022"},
    {"Pacific/Midway", "Fri Jul 29 23:40:14 2022"},
    {"Pacific/Niue", "Fri Jul 29 23:40:14 2022"},
    {"Pacific/Enderbury", "Sat Jul 30 23:40:14 2022"},
    {"Pacific/Funafuti", "Sat Jul 30 22:40:14 2022"},
    {"Pacific/Gambier", "Sat Jul 30 01:40:14 2022"},
    {"EET", "Sat Jul 30 13:40:14 2022"},
    {"MST", "Sat Jul 30 03:40:14 2022"},
    {"Europe/Dublin", "Sat Jul 30 11:40:14 2022"},
    {"Europe/Minsk", "Sat Jul 30 13:40:14 2022"},
    {"Europe/Budapest", "Sat Jul 30 12:40:14 2022"},
    {"Europe/Riga", "Sat Jul 30 13:40:14 2022"},
    {"Europe/Jersey", "Sat Jul 30 11:40:14 2022"},
    {"Europe/London", "Sat Jul 30 11:40:14 2022"},
    {"Europe/Istanbul", "Sat Jul 30 13:40:14 2022"},
    {"Europe/Moscow", "Sat Jul 30 13:40:14 2022"},
    {"Europe/Chisinau", "Sat Jul 30 13:40:14 2022"},
    {"Europe/Andorra", "Sat Jul 30 12:40:14 2022"},
    {"Europe/Vaduz", "Sat Jul 30 12:40:14 2022"},
    {"Europe/Zaporozhye", "Sat Jul 30 13:40:14 2022"},
    {"Europe/San_Marino", "Sat Jul 30 12:40:14 2022"},
    {"Europe/Monaco", "Sat Jul 30 12:40:14 2022"},
    {"Europe/Ulyanovsk", "Sat Jul 30 14:40:14 2022"},
    {"Europe/Isle_of_Man", "Sat Jul 30 11:40:14 2022"},
    {"Europe/Tirane", "Sat Jul 30 12:40:14 2022"},
    {"Europe/Vatican", "Sat Jul 30 12:40:14 2022"},
    {"Europe/Belgrade", "Sat Jul 30 12:40:14 2022"},
    {"Europe/Paris", "Sat Jul 30 12:40:14 2022"},
    {"Europe/Nicosia", "Sat Jul 30 13:40:14 2022"},
    {"Europe/Athens", "Sat Jul 30 13:40:14 2022"},
    {"Europe/Malta", "Sat Jul 30 12:40:14 2022"},
    {"Europe/Astrakhan", "Sat Jul 30 14:40:14 2022"},
    {"Europe/Bratislava", "Sat Jul 30 12:40:14 2022"},
    {"Europe/Uzhgorod", "Sat Jul 30 13:40:14 2022"},
    {"Europe/Zurich", "Sat Jul 30 12:40:14 2022"},
    {"Europe/Brussels", "Sat Jul 30 12:40:14 2022"},
    {"Europe/Sofia", "Sat Jul 30 13:40:14 2022"},
    {"Europe/Kiev", "Sat Jul 30 13:40:14 2022"},
    {"Europe/Guernsey", "Sat Jul 30 11:40:14 2022"},
    {"Europe/Busingen", "Sat Jul 30 12:40:14 2022"},
    {"Europe/Copenhagen", "Sat Jul 30 12:40:14 2022"},
    {"Europe/Skopje", "Sat Jul 30 12:40:14 2022"},
    {"Europe/Luxembourg", "Sat Jul 30 12:40:14 2022"},
    {"Europe/Lisbon", "Sat Jul 30 11:40:14 2022"},
    {"Europe/Saratov", "Sat Jul 30 14:40:14 2022"},
    {"Europe/Zagreb", "Sat Jul 30 12:40:14 2022"},
    {"Europe/Simferopol", "Sat Jul 30 13:40:14 2022"},
    {"Europe/Amsterdam", "Sat Jul 30 12:40:14 2022"},
    {"Europe/Oslo", "Sat Jul 30 12:40:14 2022"},
    {"Europe/Samara", "Sat Jul 30 14:40:14 2022"},
    {"Europe/Warsaw", "Sat Jul 30 12:40:14 2022"},
    {"Europe/Prague", "Sat Jul 30 12:40:14 2022"},
    {"Europe/Vienna", "Sat Jul 30 12:40:14 2022"},
    {"Europe/Mariehamn", "Sat Jul 30 13:40:14 2022"},
    {"Europe/Gibraltar", "Sat Jul 30 12:40:14 2022"},
    {"Europe/Stockholm", "Sat Jul 30 12:40:14 2022"},
    {"Europe/Tallinn", "Sat Jul 30 13:40:14 2022"},
    {"Europe/Kaliningrad", "Sat Jul 30 12:40:14 2022"},
    {"Europe/Podgorica", "Sat Jul 30 12:40:14 2022"},
    {"Europe/Kirov", "Sat Jul 30 13:40:14 2022"},
    {"Europe/Helsinki", "Sat Jul 30 13:40:14 2022"},
    {"Europe/Sarajevo", "Sat Jul 30 12:40:14 2022"},
    {"Europe/Rome", "Sat Jul 30 12:40:14 2022"},
    {"Europe/Bucharest", "Sat Jul 30 13:40:14 2022"},
    {"Europe/Berlin", "Sat Jul 30 12:40:14 2022"},
    {"Europe/Volgograd", "Sat Jul 30 13:40:14 2022"},
    {"Europe/Ljubljana", "Sat Jul 30 12:40:14 2022"},
    {"Europe/Vilnius", "Sat Jul 30 13:40:14 2022"},
    {"Europe/Madrid", "Sat Jul 30 12:40:14 2022"},
    {"Antarctica/Casey", "Sat Jul 30 21:40:14 2022"},
    {"Antarctica/Macquarie", "Sat Jul 30 20:40:14 2022"},
    {"Antarctica/Rothera", "Sat Jul 30 07:40:14 2022"},
    {"Antarctica/Vostok", "Sat Jul 30 16:40:14 2022"},
    {"Antarctica/Davis", "Sat Jul 30 17:40:14 2022"},
    {"Antarctica/Troll", "Sat Jul 30 12:40:14 2022"},
    {"Antarctica/Mawson", "Sat Jul 30 15:40:14 2022"},
    {"Antarctica/McMurdo", "Sat Jul 30 22:40:14 2022"},
    {"Antarctica/DumontDUrville", "Sat Jul 30 20:40:14 2022"},
    {"Antarctica/Palmer", "Sat Jul 30 07:40:14 2022"},
    {"Antarctica/Syowa", "Sat Jul 30 13:40:14 2022"},
    {"WET", "Sat Jul 30 11:40:14 2022"},
    {"CST6CDT", "Sat Jul 30 05:40:14 2022"},
    {"MET", "Sat Jul 30 12:40:14 2022"},
    {"Arctic/Longyearbyen", "Sat Jul 30 12:40:14 2022"},
    {"Atlantic/Faroe", "Sat Jul 30 11:40:14 2022"},
    {"Atlantic/Madeira", "Sat Jul 30 11:40:14 2022"},
    {"Atlantic/Canary", "Sat Jul 30 11:40:14 2022"},
    {"Atlantic/Reykjavik", "Sat Jul 30 10:40:14 2022"},
    {"Atlantic/St_Helena", "Sat Jul 30 10:40:14 2022"},
    {"Atlantic/South_Georgia", "Sat Jul 30 08:40:14 2022"},
    {"Atlantic/Azores", "Sat Jul 30 10:40:14 2022"},
    {"Atlantic/Bermuda", "Sat Jul 30 07:40:14 2022"},
    {"Atlantic/Stanley", "Sat Jul 30 07:40:14 2022"},
    {"Atlantic/Cape_Verde", "Sat Jul 30 09:40:14 2022"},
    {"Africa/Niamey", "Sat Jul 30 11:40:14 2022"},
    {"Africa/Algiers", "Sat Jul 30 11:40:14 2022"},
    {"Africa/Maputo", "Sat Jul 30 12:40:14 2022"},
    {"Africa/Johannesburg", "Sat Jul 30 12:40:14 2022"},
    {"Africa/Addis_Ababa", "Sat Jul 30 13:40:14 2022"},
    {"Africa/Malabo", "Sat Jul 30 11:40:14 2022"},
    {"Africa/Cairo", "Sat Jul 30 12:40:14 2022"},
    {"Africa/Bissau", "Sat Jul 30 10:40:14 2022"},
    {"Africa/Ndjamena", "Sat Jul 30 11:40:14 2022"},
    {"Africa/Tunis", "Sat Jul 30 11:40:14 2022"},
    {"Africa/Harare", "Sat Jul 30 12:40:14 2022"},
    {"Africa/El_Aaiun", "Sat Jul 30 11:40:14 2022"},
    {"Africa/Kampala", "Sat Jul 30 13:40:14 2022"},
    {"Africa/Libreville", "Sat Jul 30 11:40:14 2022"},
    {"Africa/Blantyre", "Sat Jul 30 12:40:14 2022"},
    {"Africa/Brazzaville", "Sat Jul 30 11:40:14 2022"},
    {"Africa/Bangui", "Sat Jul 30 11:40:14 2022"},
    {"Africa/Luanda", "Sat Jul 30 11:40:14 2022"},
    {"Africa/Freetown", "Sat Jul 30 10:40:14 2022"},
    {"Africa/Casablanca", "Sat Jul 30 11:40:14 2022"},
    {"Africa/Conakry", "Sat Jul 30 10:40:14 2022"},
    {"Africa/Kinshasa", "Sat Jul 30 11:40:14 2022"},
    {"Africa/Dakar", "Sat Jul 30 10:40:14 2022"},
    {"Africa/Ouagadougou", "Sat Jul 30 10:40:14 2022"},
    {"Africa/Nouakchott", "Sat Jul 30 10:40:14 2022"},
    {"Africa/Monrovia", "Sat Jul 30 10:40:14 2022"},
    {"Africa/Kigali", "Sat Jul 30 12:40:14 2022"},
    {"Africa/Windhoek", "Sat Jul 30 12:40:14 2022"},
    {"Africa/Juba", "Sat Jul 30 12:40:14 2022"},
    {"Africa/Sao_Tome", "Sat Jul 30 10:40:14 2022"},
    {"Africa/Banjul", "Sat Jul 30 10:40:14 2022"},
    {"Africa/Mbabane", "Sat Jul 30 12:40:14 2022"},
    {"Africa/Asmara", "Sat Jul 30 13:40:14 2022"},
    {"Africa/Djibouti", "Sat Jul 30 13:40:14 2022"},
    {"Africa/Tripoli", "Sat Jul 30 12:40:14 2022"},
    {"Africa/Ceuta", "Sat Jul 30 12:40:14 2022"},
    {"Africa/Khartoum", "Sat Jul 30 12:40:14 2022"},
    {"Africa/Bamako", "Sat Jul 30 10:40:14 2022"},
    {"Africa/Porto-Novo", "Sat Jul 30 11:40:14 2022"},
    {"Africa/Mogadishu", "Sat Jul 30 13:40:14 2022"},
    {"Africa/Abidjan", "Sat Jul 30 10:40:14 2022"},
    {"Africa/Lusaka", "Sat Jul 30 12:40:14 2022"},
    {"Africa/Lagos", "Sat Jul 30 11:40:14 2022"},
    {"Africa/Lubumbashi", "Sat Jul 30 12:40:14 2022"},
    {"Africa/Nairobi", "Sat Jul 30 13:40:14 2022"},
    {"Africa/Accra", "Sat Jul 30 10:40:14 2022"},
    {"Africa/Maseru", "Sat Jul 30 12:40:14 2022"},
    {"Africa/Lome", "Sat Jul 30 10:40:14 2022"},
    {"Africa/Douala", "Sat Jul 30 11:40:14 2022"},
    {"Africa/Bujumbura", "Sat Jul 30 12:40:14 2022"},
    {"Africa/Dar_es_Salaam", "Sat Jul 30 13:40:14 2022"},
    {"Africa/Gaborone", "Sat Jul 30 12:40:14 2022"},
    {"Australia/Adelaide", "Sat Jul 30 20:10:14 2022"},
    {"Australia/Broken_Hill", "Sat Jul 30 20:10:14 2022"},
    {"Australia/Perth", "Sat Jul 30 18:40:14 2022"},
    {"Australia/Darwin", "Sat Jul 30 20:10:14 2022"},
    {"Australia/Lord_Howe", "Sat Jul 30 21:10:14 2022"},
    {"Australia/Hobart", "Sat Jul 30 20:40:14 2022"},
    {"Australia/Brisbane", "Sat Jul 30 20:40:14 2022"},
    {"Australia/Sydney", "Sat Jul 30 20:40:14 2022"},
    {"Australia/Lindeman", "Sat Jul 30 20:40:14 2022"},
    {"Australia/Melbourne", "Sat Jul 30 20:40:14 2022"},
    {"Australia/Eucla", "Sat Jul 30 19:25:14 2022"},
    {"America/Grand_Turk", "Sat Jul 30 06:40:14 2022"},
    {"America/St_Thomas", "Sat Jul 30 06:40:14 2022"},
    {"America/Havana", "Sat Jul 30 06:40:14 2022"},
    {"America/Thunder_Bay", "Sat Jul 30 06:40:14 2022"},
    {"America/Inuvik", "Sat Jul 30 04:40:14 2022"},
    {"America/Winnipeg", "Sat Jul 30 05:40:14 2022"},
    {"America/Maceio", "Sat Jul 30 07:40:14 2022"},
    {"America/Chihuahua", "Sat Jul 30 04:40:14 2022"},
    {"America/North_Dakota/Beulah", "Sat Jul 30 05:40:14 2022"},
    {"America/North_Dakota/Center", "Sat Jul 30 05:40:14 2022"},
    {"America/North_Dakota/New_Salem", "Sat Jul 30 05:40:14 2022"},
    {"America/Campo_Grande", "Sat Jul 30 06:40:14 2022"},
    {"America/Scoresbysund", "Sat Jul 30 10:40:14 2022"},
    {"America/Tijuana", "Sat Jul 30 03:40:14 2022"},
    {"America/Bahia", "Sat Jul 30 07:40:14 2022"},
    {"America/St_Vincent", "Sat Jul 30 06:40:14 2022"},
    {"America/Creston", "Sat Jul 30 03:40:14 2022"},
    {"America/Dawson", "Sat Jul 30 03:40:14 2022"},
    {"America/Tegucigalpa", "Sat Jul 30 04:40:14 2022"},
    {"America/Lima", "Sat Jul 30 05:40:14 2022"},
    {"America/Mexico_City", "Sat Jul 30 05:40:14 2022"},
    {"America/Dominica", "Sat Jul 30 06:40:14 2022"},
    {"America/Atikokan", "Sat Jul 30 05:40:14 2022"},
    {"America/Port-au-Prince", "Sat Jul 30 06:40:14 2022"},
    {"America/Yakutat", "Sat Jul 30 02:40:14 2022"},
    {"America/St_Kitts", "Sat Jul 30 06:40:14 2022"},
    {"America/Guatemala", "Sat Jul 30 04:40:14 2022"},
    {"America/St_Barthelemy", "Sat Jul 30 06:40:14 2022"},
    {"America/Vancouver", "Sat Jul 30 03:40:14 2022"},
    {"America/Jamaica", "Sat Jul 30 05:40:14 2022"},
    {"America/Whitehorse", "Sat Jul 30 03:40:14 2022"},
    {"America/Asuncion", "Sat Jul 30 06:40:14 2022"},
    {"America/Toronto", "Sat Jul 30 06:40:14 2022"},
    {"America/Hermosillo", "Sat Jul 30 03:40:14 2022"},
    {"America/Denver", "Sat Jul 30 04:40:14 2022"},
    {"America/Mazatlan", "Sat Jul 30 04:40:14 2022"},
    {"America/Iqaluit", "Sat Jul 30 06:40:14 2022"},
    {"America/Nuuk", "Sat Jul 30 08:40:14 2022"},
    {"America/Cayenne", "Sat Jul 30 07:40:14 2022"},
    {"America/Santarem", "Sat Jul 30 07:40:14 2022"},
    {"America/Noronha", "Sat Jul 30 08:40:14 2022"},
    {"America/Ojinaga", "Sat Jul 30 04:40:14 2022"},
    {"America/Fort_Nelson", "Sat Jul 30 03:40:14 2022"},
    {"America/Aruba", "Sat Jul 30 06:40:14 2022"},
    {"America/Indiana/Vevay", "Sat Jul 30 06:40:14 2022"},
    {"America/Indiana/Marengo", "Sat Jul 30 06:40:14 2022"},
    {"America/Indiana/Indianapolis", "Sat Jul 30 06:40:14 2022"},
    {"America/Indiana/Tell_City", "Sat Jul 30 05:40:14 2022"},
    {"America/Indiana/Petersburg", "Sat Jul 30 06:40:14 2022"},
    {"America/Indiana/Winamac", "Sat Jul 30 06:40:14 2022"},
    {"America/Indiana/Knox", "Sat Jul 30 05:40:14 2022"},
    {"America/Indiana/Vincennes", "Sat Jul 30 06:40:14 2022"},
    {"America/Cayman", "Sat Jul 30 05:40:14 2022"},
    {"America/Sao_Paulo", "Sat Jul 30 07:40:14 2022"},
    {"America/Curacao", "Sat Jul 30 06:40:14 2022"},
    {"America/Anchorage", "Sat Jul 30 02:40:14 2022"},
    {"America/Guyana", "Sat Jul 30 06:40:14 2022"},
    {"America/Rankin_Inlet", "Sat Jul 30 05:40:14 2022"},
    {"America/Antigua", "Sat Jul 30 06:40:14 2022"},
    {"America/Juneau", "Sat Jul 30 02:40:14 2022"},
    {"America/Merida", "Sat Jul 30 05:40:14 2022"},
    {"America/Punta_Arenas", "Sat Jul 30 07:40:14 2022"},
    {"America/Moncton", "Sat Jul 30 07:40:14 2022"},
    {"America/Fortaleza", "Sat Jul 30 07:40:14 2022"},
    {"America/Resolute", "Sat Jul 30 05:40:14 2022"},
    {"America/Nassau", "Sat Jul 30 06:40:14 2022"},
    {"America/Edmonton", "Sat Jul 30 04:40:14 2022"},
    {"America/Regina", "Sat Jul 30 04:40:14 2022"},
    {"America/Halifax", "Sat Jul 30 07:40:14 2022"},
    {"America/Danmarkshavn", "Sat Jul 30 10:40:14 2022"},
    {"America/Metlakatla", "Sat Jul 30 02:40:14 2022"},
    {"America/Guadeloupe", "Sat Jul 30 06:40:14 2022"},
    {"America/Caracas", "Sat Jul 30 06:40:14 2022"},
    {"America/Detroit", "Sat Jul 30 06:40:14 2022"},
    {"America/Matamoros", "Sat Jul 30 05:40:14 2022"},
    {"America/Glace_Bay", "Sat Jul 30 07:40:14 2022"},
    {"America/Phoenix", "Sat Jul 30 03:40:14 2022"},
    {"America/La_Paz", "Sat Jul 30 06:40:14 2022"},
    {"America/Swift_Current", "Sat Jul 30 04:40:14 2022"},
    {"America/Nipigon", "Sat Jul 30 06:40:14 2022"},
    {"America/Cancun", "Sat Jul 30 05:40:14 2022"},
    {"America/Port_of_Spain", "Sat Jul 30 06:40:14 2022"},
    {"America/Kralendijk", "Sat Jul 30 06:40:14 2022"},
    {"America/Grenada", "Sat Jul 30 06:40:14 2022"},
    {"America/Belem", "Sat Jul 30 07:40:14 2022"},
    {"America/Managua", "Sat Jul 30 04:40:14 2022"},
    {"America/Adak", "Sat Jul 30 01:40:14 2022"},
    {"America/St_Lucia", "Sat Jul 30 06:40:14 2022"},
    {"America/Rio_Branco", "Sat Jul 30 05:40:14 2022"},
    {"America/Santiago", "Sat Jul 30 06:40:14 2022"},
    {"America/Puerto_Rico", "Sat Jul 30 06:40:14 2022"},
    {"America/Miquelon", "Sat Jul 30 08:40:14 2022"},
    {"America/Cuiaba", "Sat Jul 30 06:40:14 2022"},
    {"America/Martinique", "Sat Jul 30 06:40:14 2022"},
    {"America/Argentina/Buenos_Aires", "Sat Jul 30 07:40:14 2022"},
    {"America/Argentina/San_Luis", "Sat Jul 30 07:40:14 2022"},
    {"America/Argentina/Catamarca", "Sat Jul 30 07:40:14 2022"},
    {"America/Argentina/Jujuy", "Sat Jul 30 07:40:14 2022"},
    {"America/Argentina/Salta", "Sat Jul 30 07:40:14 2022"},
    {"America/Argentina/Ushuaia", "Sat Jul 30 07:40:14 2022"},
    {"America/Argentina/Mendoza", "Sat Jul 30 07:40:14 2022"},
    {"America/Argentina/Rio_Gallegos", "Sat Jul 30 07:40:14 2022"},
    {"America/Argentina/Tucuman", "Sat Jul 30 07:40:14 2022"},
    {"America/Argentina/San_Juan", "Sat Jul 30 07:40:14 2022"},
    {"America/Argentina/Cordoba", "Sat Jul 30 07:40:14 2022"},
    {"America/Argentina/La_Rioja", "Sat Jul 30 07:40:14 2022"},
    {"America/Barbados", "Sat Jul 30 06:40:14 2022"},
    {"America/Monterrey", "Sat Jul 30 05:40:14 2022"},
    {"America/Thule", "Sat Jul 30 07:40:14 2022"},
    {"America/Montserrat", "Sat Jul 30 06:40:14 2022"},
    {"America/Guayaquil", "Sat Jul 30 05:40:14 2022"},
    {"America/St_Johns", "Sat Jul 30 08:10:14 2022"},
    {"America/Boise", "Sat Jul 30 04:40:14 2022"},
    {"America/Manaus", "Sat Jul 30 06:40:14 2022"},
    {"America/Nome", "Sat Jul 30 02:40:14 2022"},
    {"America/Goose_Bay", "Sat Jul 30 07:40:14 2022"},
    {"America/Belize", "Sat Jul 30 04:40:14 2022"},
    {"America/Sitka", "Sat Jul 30 02:40:14 2022"},
    {"America/Dawson_Creek", "Sat Jul 30 03:40:14 2022"},
    {"America/Recife", "Sat Jul 30 07:40:14 2022"},
    {"America/Kentucky/Louisville", "Sat Jul 30 06:40:14 2022"},
    {"America/Kentucky/Monticello", "Sat Jul 30 06:40:14 2022"},
    {"America/New_York", "Sat Jul 30 06:40:14 2022"},
    {"America/Tortola", "Sat Jul 30 06:40:14 2022"},
    {"America/Chicago", "Sat Jul 30 05:40:14 2022"},
    {"America/Pangnirtung", "Sat Jul 30 06:40:14 2022"},
    {"America/Boa_Vista", "Sat Jul 30 06:40:14 2022"},
    {"America/Bogota", "Sat Jul 30 05:40:14 2022"},
    {"America/Rainy_River", "Sat Jul 30 05:40:14 2022"},
    {"America/El_Salvador", "Sat Jul 30 04:40:14 2022"},
    {"America/Bahia_Banderas", "Sat Jul 30 05:40:14 2022"},
    {"America/Yellowknife", "Sat Jul 30 04:40:14 2022"},
    {"America/Montevideo", "Sat Jul 30 07:40:14 2022"},
    {"America/Cambridge_Bay", "Sat Jul 30 04:40:14 2022"},
    {"America/Panama", "Sat Jul 30 05:40:14 2022"},
    {"America/Santo_Domingo", "Sat Jul 30 06:40:14 2022"},
    {"America/Anguilla", "Sat Jul 30 06:40:14 2022"},
    {"America/Paramaribo", "Sat Jul 30 07:40:14 2022"},
    {"America/Marigot", "Sat Jul 30 06:40:14 2022"},
    {"America/Menominee", "Sat Jul 30 05:40:14 2022"},
    {"America/Blanc-Sablon", "Sat Jul 30 06:40:14 2022"},
    {"America/Costa_Rica", "Sat Jul 30 04:40:14 2022"},
    {"America/Los_Angeles", "Sat Jul 30 03:40:14 2022"},
    {"America/Lower_Princes", "Sat Jul 30 06:40:14 2022"},
    {"America/Eirunepe", "Sat Jul 30 05:40:14 2022"},
    {"America/Araguaina", "Sat Jul 30 07:40:14 2022"},
    {"America/Porto_Velho", "Sat Jul 30 06:40:14 2022"},
    {"EST", "Sat Jul 30 05:40:14 2022"},
    {"CET", "Sat Jul 30 12:40:14 2022"},
    {"MST7MDT", "Sat Jul 30 04:40:14 2022"},
    {"Indian/Chagos", "Sat Jul 30 16:40:14 2022"},
    {"Indian/Kerguelen", "Sat Jul 30 15:40:14 2022"},
    {"Indian/Reunion", "Sat Jul 30 14:40:14 2022"},
    {"Indian/Antananarivo", "Sat Jul 30 13:40:14 2022"},
    {"Indian/Cocos", "Sat Jul 30 17:10:14 2022"},
    {"Indian/Mauritius", "Sat Jul 30 14:40:14 2022"},
    {"Indian/Christmas", "Sat Jul 30 17:40:14 2022"},
    {"Indian/Maldives", "Sat Jul 30 15:40:14 2022"},
    {"Indian/Comoro", "Sat Jul 30 13:40:14 2022"},
    {"Indian/Mahe", "Sat Jul 30 14:40:14 2022"},
    {"Indian/Mayotte", "Sat Jul 30 13:40:14 2022"},
    {"Asia/Novokuznetsk", "Sat Jul 30 17:40:14 2022"},
    {"Asia/Magadan", "Sat Jul 30 21:40:14 2022"},
    {"Asia/Yangon", "Sat Jul 30 17:10:14 2022"},
    {"Asia/Samarkand", "Sat Jul 30 15:40:14 2022"},
    {"Asia/Tehran", "Sat Jul 30 15:10:14 2022"},
    {"Asia/Dili", "Sat Jul 30 19:40:14 2022"},
    {"Asia/Yerevan", "Sat Jul 30 14:40:14 2022"},
    {"Asia/Qatar", "Sat Jul 30 13:40:14 2022"},
    {"Asia/Chita", "Sat Jul 30 19:40:14 2022"},
    {"Asia/Makassar", "Sat Jul 30 18:40:14 2022"},
    {"Asia/Hovd", "Sat Jul 30 17:40:14 2022"},
    {"Asia/Istanbul", "Sat Jul 30 13:40:14 2022"},
    {"Asia/Irkutsk", "Sat Jul 30 18:40:14 2022"},
    {"Asia/Pontianak", "Sat Jul 30 17:40:14 2022"},
    {"Asia/Qyzylorda", "Sat Jul 30 15:40:14 2022"},
    {"Asia/Srednekolymsk", "Sat Jul 30 21:40:14 2022"},
    {"Asia/Aden", "Sat Jul 30 13:40:14 2022"},
    {"Asia/Kabul", "Sat Jul 30 15:10:14 2022"},
    {"Asia/Macau", "Sat Jul 30 18:40:14 2022"},
    {"Asia/Riyadh", "Sat Jul 30 13:40:14 2022"},
    {"Asia/Taipei", "Sat Jul 30 18:40:14 2022"},
    {"Asia/Manila", "Sat Jul 30 18:40:14 2022"},
    {"Asia/Aqtau", "Sat Jul 30 15:40:14 2022"},
    {"Asia/Thimphu", "Sat Jul 30 16:40:14 2022"},
    {"Asia/Shanghai", "Sat Jul 30 18:40:14 2022"},
    {"Asia/Choibalsan", "Sat Jul 30 18:40:14 2022"},
    {"Asia/Nicosia", "Sat Jul 30 13:40:14 2022"},
    {"Asia/Dubai", "Sat Jul 30 14:40:14 2022"},
    {"Asia/Baku", "Sat Jul 30 14:40:14 2022"},
    {"Asia/Kuching", "Sat Jul 30 18:40:14 2022"},
    {"Asia/Gaza", "Sat Jul 30 13:40:14 2022"},
    {"Asia/Jayapura", "Sat Jul 30 19:40:14 2022"},
    {"Asia/Tokyo", "Sat Jul 30 19:40:14 2022"},
    {"Asia/Muscat", "Sat Jul 30 14:40:14 2022"},
    {"Asia/Kolkata", "Sat Jul 30 16:10:14 2022"},
    {"Asia/Ho_Chi_Minh", "Sat Jul 30 17:40:14 2022"},
    {"Asia/Tashkent", "Sat Jul 30 15:40:14 2022"},
    {"Asia/Bangkok", "Sat Jul 30 17:40:14 2022"},
    {"Asia/Karachi", "Sat Jul 30 15:40:14 2022"},
    {"Asia/Krasnoyarsk", "Sat Jul 30 17:40:14 2022"},
    {"Asia/Yekaterinburg", "Sat Jul 30 15:40:14 2022"},
    {"Asia/Famagusta", "Sat Jul 30 13:40:14 2022"},
    {"Asia/Ulaanbaatar", "Sat Jul 30 18:40:14 2022"},
    {"Asia/Bahrain", "Sat Jul 30 13:40:14 2022"},
    {"Asia/Barnaul", "Sat Jul 30 17:40:14 2022"},
    {"Asia/Jerusalem", "Sat Jul 30 13:40:14 2022"},
    {"Asia/Qostanay", "Sat Jul 30 16:40:14 2022"},
    {"Asia/Ust-Nera", "Sat Jul 30 20:40:14 2022"},
    {"Asia/Tomsk", "Sat Jul 30 17:40:14 2022"},
    {"Asia/Kuala_Lumpur", "Sat Jul 30 18:40:14 2022"},
    {"Asia/Brunei", "Sat Jul 30 18:40:14 2022"},
    {"Asia/Aqtobe", "Sat Jul 30 15:40:14 2022"},
    {"Asia/Vladivostok", "Sat Jul 30 20:40:14 2022"},
    {"Asia/Damascus", "Sat Jul 30 13:40:14 2022"},
    {"Asia/Omsk", "Sat Jul 30 16:40:14 2022"},
    {"Asia/Almaty", "Sat Jul 30 16:40:14 2022"},
    {"Asia/Dhaka", "Sat Jul 30 16:40:14 2022"},
    {"Asia/Tbilisi", "Sat Jul 30 14:40:14 2022"},
    {"Asia/Baghdad", "Sat Jul 30 13:40:14 2022"},
    {"Asia/Phnom_Penh", "Sat Jul 30 17:40:14 2022"},
    {"Asia/Kamchatka", "Sat Jul 30 22:40:14 2022"},
    {"Asia/Ashgabat", "Sat Jul 30 15:40:14 2022"},
    {"Asia/Sakhalin", "Sat Jul 30 21:40:14 2022"},
    {"Asia/Novosibirsk", "Sat Jul 30 17:40:14 2022"},
    {"Asia/Kuwait", "Sat Jul 30 13:40:14 2022"},
    {"Asia/Hong_Kong", "Sat Jul 30 18:40:14 2022"},
    {"Asia/Amman", "Sat Jul 30 13:40:14 2022"},
    {"Asia/Beirut", "Sat Jul 30 13:40:14 2022"},
    {"Asia/Yakutsk", "Sat Jul 30 19:40:14 2022"},
    {"Asia/Colombo", "Sat Jul 30 16:10:14 2022"},
    {"Asia/Oral", "Sat Jul 30 15:40:14 2022"},
    {"Asia/Kathmandu", "Sat Jul 30 16:25:14 2022"},
    {"Asia/Bishkek", "Sat Jul 30 16:40:14 2022"},
    {"Asia/Hebron", "Sat Jul 30 13:40:14 2022"},
    {"Asia/Singapore", "Sat Jul 30 18:40:14 2022"},
    {"Asia/Atyrau", "Sat Jul 30 15:40:14 2022"},
    {"Asia/Khandyga", "Sat Jul 30 19:40:14 2022"},
    {"Asia/Vientiane", "Sat Jul 30 17:40:14 2022"},
    {"Asia/Jakarta", "Sat Jul 30 17:40:14 2022"},
    {"Asia/Dushanbe", "Sat Jul 30 15:40:14 2022"},
    {"Asia/Seoul", "Sat Jul 30 19:40:14 2022"},
    {"Asia/Pyongyang", "Sat Jul 30 19:40:14 2022"},
    {"Asia/Urumqi", "Sat Jul 30 16:40:14 2022"},
    {"Asia/Anadyr", "Sat Jul 30 22:40:14 2022"},
    {"PST8PDT", "Sat Jul 30 03:40:14 2022"},
    {"HST", "Sat Jul 30 00:40:14 2022"},
    {"EST5EDT", "Sat Jul 30 06:40:14 2022"},
};
static const struct strptime_struct gResultDataZ[] = {
    {"Arctic/Longyearbyen", "2022-6-30 12:40:14 wday=6,yday=0,isdst=1,gmtoff=7200,zone=CEST"},
    {"MST", "2022-6-30 3:40:14 wday=6,yday=0,isdst=0,gmtoff=-25200,zone=MST"},
    {"EST5EDT", "2022-6-30 6:40:14 wday=6,yday=0,isdst=1,gmtoff=-14400,zone=EDT"},
    {"MET", "2022-6-30 12:40:14 wday=6,yday=0,isdst=1,gmtoff=7200,zone=MEST"},
    {"CET", "2022-6-30 12:40:14 wday=6,yday=0,isdst=1,gmtoff=7200,zone=CEST"},
    {"Europe/Zagreb", "2022-6-30 12:40:14 wday=6,yday=0,isdst=1,gmtoff=7200,zone=CEST"},
    {"Europe/Madrid", "2022-6-30 12:40:14 wday=6,yday=0,isdst=1,gmtoff=7200,zone=CEST"},
    {"Europe/Copenhagen", "2022-6-30 12:40:14 wday=6,yday=0,isdst=1,gmtoff=7200,zone=CEST"},
    {"Europe/Dublin", "2022-6-30 11:40:14 wday=6,yday=0,isdst=0,gmtoff=3600,zone=IST"},
    {"Europe/Podgorica", "2022-6-30 12:40:14 wday=6,yday=0,isdst=1,gmtoff=7200,zone=CEST"},
    {"Europe/Ljubljana", "2022-6-30 12:40:14 wday=6,yday=0,isdst=1,gmtoff=7200,zone=CEST"},
    {"Europe/Astrakhan", "2022-6-30 14:40:14 wday=6,yday=0,isdst=0,gmtoff=14400,zone=+04"},
    {"Europe/Nicosia", "2022-6-30 13:40:14 wday=6,yday=0,isdst=1,gmtoff=10800,zone=EEST"},
    {"Europe/Helsinki", "2022-6-30 13:40:14 wday=6,yday=0,isdst=1,gmtoff=10800,zone=EEST"},
    {"Europe/Oslo", "2022-6-30 12:40:14 wday=6,yday=0,isdst=1,gmtoff=7200,zone=CEST"},
    {"Europe/Simferopol", "2022-6-30 13:40:14 wday=6,yday=0,isdst=0,gmtoff=10800,zone=MSK"},
    {"Europe/Kaliningrad", "2022-6-30 12:40:14 wday=6,yday=0,isdst=0,gmtoff=7200,zone=EET"},
    {"Europe/Moscow", "2022-6-30 13:40:14 wday=6,yday=0,isdst=0,gmtoff=10800,zone=MSK"},
    {"Europe/Samara", "2022-6-30 14:40:14 wday=6,yday=0,isdst=0,gmtoff=14400,zone=+04"},
    {"Europe/Ulyanovsk", "2022-6-30 14:40:14 wday=6,yday=0,isdst=0,gmtoff=14400,zone=+04"},
    {"Europe/Prague", "2022-6-30 12:40:14 wday=6,yday=0,isdst=1,gmtoff=7200,zone=CEST"},
    {"Europe/Amsterdam", "2022-6-30 12:40:14 wday=6,yday=0,isdst=1,gmtoff=7200,zone=CEST"},
    {"Europe/San_Marino", "2022-6-30 12:40:14 wday=6,yday=0,isdst=1,gmtoff=7200,zone=CEST"},
    {"Europe/Riga", "2022-6-30 13:40:14 wday=6,yday=0,isdst=1,gmtoff=10800,zone=EEST"},
    {"Europe/Chisinau", "2022-6-30 13:40:14 wday=6,yday=0,isdst=1,gmtoff=10800,zone=EEST"},
    {"Europe/Athens", "2022-6-30 13:40:14 wday=6,yday=0,isdst=1,gmtoff=10800,zone=EEST"},
    {"Europe/Mariehamn", "2022-6-30 13:40:14 wday=6,yday=0,isdst=1,gmtoff=10800,zone=EEST"},
    {"Europe/Minsk", "2022-6-30 13:40:14 wday=6,yday=0,isdst=0,gmtoff=10800,zone=+03"},
    {"Europe/Berlin", "2022-6-30 12:40:14 wday=6,yday=0,isdst=1,gmtoff=7200,zone=CEST"},
    {"Europe/Istanbul", "2022-6-30 13:40:14 wday=6,yday=0,isdst=0,gmtoff=10800,zone=+03"},
    {"Europe/Bucharest", "2022-6-30 13:40:14 wday=6,yday=0,isdst=1,gmtoff=10800,zone=EEST"},
    {"Europe/Uzhgorod", "2022-6-30 13:40:14 wday=6,yday=0,isdst=1,gmtoff=10800,zone=EEST"},
    {"Europe/Isle_of_Man", "2022-6-30 11:40:14 wday=6,yday=0,isdst=1,gmtoff=3600,zone=BST"},
    {"Europe/Tallinn", "2022-6-30 13:40:14 wday=6,yday=0,isdst=1,gmtoff=10800,zone=EEST"},
    {"Europe/Gibraltar", "2022-6-30 12:40:14 wday=6,yday=0,isdst=1,gmtoff=7200,zone=CEST"},
    {"Europe/Paris", "2022-6-30 12:40:14 wday=6,yday=0,isdst=1,gmtoff=7200,zone=CEST"},
    {"Europe/Guernsey", "2022-6-30 11:40:14 wday=6,yday=0,isdst=1,gmtoff=3600,zone=BST"},
    {"Europe/Zurich", "2022-6-30 12:40:14 wday=6,yday=0,isdst=1,gmtoff=7200,zone=CEST"},
    {"Europe/Bratislava", "2022-6-30 12:40:14 wday=6,yday=0,isdst=1,gmtoff=7200,zone=CEST"},
    {"Europe/Vilnius", "2022-6-30 13:40:14 wday=6,yday=0,isdst=1,gmtoff=10800,zone=EEST"},
    {"Europe/Sofia", "2022-6-30 13:40:14 wday=6,yday=0,isdst=1,gmtoff=10800,zone=EEST"},
    {"Europe/Vienna", "2022-6-30 12:40:14 wday=6,yday=0,isdst=1,gmtoff=7200,zone=CEST"},
    {"Europe/Malta", "2022-6-30 12:40:14 wday=6,yday=0,isdst=1,gmtoff=7200,zone=CEST"},
    {"Europe/Saratov", "2022-6-30 14:40:14 wday=6,yday=0,isdst=0,gmtoff=14400,zone=+04"},
    {"Europe/Vatican", "2022-6-30 12:40:14 wday=6,yday=0,isdst=1,gmtoff=7200,zone=CEST"},
    {"Europe/Brussels", "2022-6-30 12:40:14 wday=6,yday=0,isdst=1,gmtoff=7200,zone=CEST"},
    {"Europe/Tirane", "2022-6-30 12:40:14 wday=6,yday=0,isdst=1,gmtoff=7200,zone=CEST"},
    {"Europe/Warsaw", "2022-6-30 12:40:14 wday=6,yday=0,isdst=1,gmtoff=7200,zone=CEST"},
    {"Europe/Kirov", "2022-6-30 13:40:14 wday=6,yday=0,isdst=0,gmtoff=10800,zone=+03"},
    {"Europe/Kiev", "2022-6-30 13:40:14 wday=6,yday=0,isdst=1,gmtoff=10800,zone=EEST"},
    {"Europe/Zaporozhye", "2022-6-30 13:40:14 wday=6,yday=0,isdst=1,gmtoff=10800,zone=EEST"},
    {"Europe/London", "2022-6-30 11:40:14 wday=6,yday=0,isdst=1,gmtoff=3600,zone=BST"},
    {"Europe/Sarajevo", "2022-6-30 12:40:14 wday=6,yday=0,isdst=1,gmtoff=7200,zone=CEST"},
    {"Europe/Vaduz", "2022-6-30 12:40:14 wday=6,yday=0,isdst=1,gmtoff=7200,zone=CEST"},
    {"Europe/Stockholm", "2022-6-30 12:40:14 wday=6,yday=0,isdst=1,gmtoff=7200,zone=CEST"},
    {"Europe/Belgrade", "2022-6-30 12:40:14 wday=6,yday=0,isdst=1,gmtoff=7200,zone=CEST"},
    {"Europe/Jersey", "2022-6-30 11:40:14 wday=6,yday=0,isdst=1,gmtoff=3600,zone=BST"},
    {"Europe/Budapest", "2022-6-30 12:40:14 wday=6,yday=0,isdst=1,gmtoff=7200,zone=CEST"},
    {"Europe/Volgograd", "2022-6-30 13:40:14 wday=6,yday=0,isdst=0,gmtoff=10800,zone=+03"},
    {"Europe/Monaco", "2022-6-30 12:40:14 wday=6,yday=0,isdst=1,gmtoff=7200,zone=CEST"},
    {"Europe/Busingen", "2022-6-30 12:40:14 wday=6,yday=0,isdst=1,gmtoff=7200,zone=CEST"},
    {"Europe/Andorra", "2022-6-30 12:40:14 wday=6,yday=0,isdst=1,gmtoff=7200,zone=CEST"},
    {"Europe/Lisbon", "2022-6-30 11:40:14 wday=6,yday=0,isdst=1,gmtoff=3600,zone=WEST"},
    {"Europe/Skopje", "2022-6-30 12:40:14 wday=6,yday=0,isdst=1,gmtoff=7200,zone=CEST"},
    {"Europe/Rome", "2022-6-30 12:40:14 wday=6,yday=0,isdst=1,gmtoff=7200,zone=CEST"},
    {"Europe/Luxembourg", "2022-6-30 12:40:14 wday=6,yday=0,isdst=1,gmtoff=7200,zone=CEST"},
    {"Australia/Adelaide", "2022-6-30 20:10:14 wday=6,yday=0,isdst=0,gmtoff=34200,zone=ACST"},
    {"Australia/Darwin", "2022-6-30 20:10:14 wday=6,yday=0,isdst=0,gmtoff=34200,zone=ACST"},
    {"Australia/Lindeman", "2022-6-30 20:40:14 wday=6,yday=0,isdst=0,gmtoff=36000,zone=AEST"},
    {"Australia/Melbourne", "2022-6-30 20:40:14 wday=6,yday=0,isdst=0,gmtoff=36000,zone=AEST"},
    {"Australia/Perth", "2022-6-30 18:40:14 wday=6,yday=0,isdst=0,gmtoff=28800,zone=AWST"},
    {"Australia/Eucla", "2022-6-30 19:25:14 wday=6,yday=0,isdst=0,gmtoff=31500,zone=+0845"},
    {"Australia/Sydney", "2022-6-30 20:40:14 wday=6,yday=0,isdst=0,gmtoff=36000,zone=AEST"},
    {"Australia/Broken_Hill", "2022-6-30 20:10:14 wday=6,yday=0,isdst=0,gmtoff=34200,zone=ACST"},
    {"Australia/Brisbane", "2022-6-30 20:40:14 wday=6,yday=0,isdst=0,gmtoff=36000,zone=AEST"},
    {"Australia/Hobart", "2022-6-30 20:40:14 wday=6,yday=0,isdst=0,gmtoff=36000,zone=AEST"},
    {"Australia/Lord_Howe", "2022-6-30 21:10:14 wday=6,yday=0,isdst=0,gmtoff=37800,zone=+1030"},
    {"America/Phoenix", "2022-6-30 3:40:14 wday=6,yday=0,isdst=0,gmtoff=-25200,zone=MST"},
    {"America/Belem", "2022-6-30 7:40:14 wday=6,yday=0,isdst=0,gmtoff=-10800,zone=-03"},
    {"America/Kentucky/Monticello", "2022-6-30 6:40:14 wday=6,yday=0,isdst=1,gmtoff=-14400,zone=EDT"},
    {"America/Kentucky/Louisville", "2022-6-30 6:40:14 wday=6,yday=0,isdst=1,gmtoff=-14400,zone=EDT"},
    {"America/Punta_Arenas", "2022-6-30 7:40:14 wday=6,yday=0,isdst=0,gmtoff=-10800,zone=-03"},
    {"America/Antigua", "2022-6-30 6:40:14 wday=6,yday=0,isdst=0,gmtoff=-14400,zone=AST"},
    {"America/Iqaluit", "2022-6-30 6:40:14 wday=6,yday=0,isdst=1,gmtoff=-14400,zone=EDT"},
    {"America/Campo_Grande", "2022-6-30 6:40:14 wday=6,yday=0,isdst=0,gmtoff=-14400,zone=-04"},
    {"America/Lima", "2022-6-30 5:40:14 wday=6,yday=0,isdst=0,gmtoff=-18000,zone=-05"},
    {"America/Moncton", "2022-6-30 7:40:14 wday=6,yday=0,isdst=1,gmtoff=-10800,zone=ADT"},
    {"America/Nuuk", "2022-6-30 8:40:14 wday=6,yday=0,isdst=1,gmtoff=-7200,zone=-02"},
    {"America/Asuncion", "2022-6-30 6:40:14 wday=6,yday=0,isdst=0,gmtoff=-14400,zone=-04"},
    {"America/Edmonton", "2022-6-30 4:40:14 wday=6,yday=0,isdst=1,gmtoff=-21600,zone=MDT"},
    {"America/Tegucigalpa", "2022-6-30 4:40:14 wday=6,yday=0,isdst=0,gmtoff=-21600,zone=CST"},
    {"America/Guyana", "2022-6-30 6:40:14 wday=6,yday=0,isdst=0,gmtoff=-14400,zone=-04"},
    {"America/Argentina/Jujuy", "2022-6-30 7:40:14 wday=6,yday=0,isdst=0,gmtoff=-10800,zone=-03"},
    {"America/Argentina/Salta", "2022-6-30 7:40:14 wday=6,yday=0,isdst=0,gmtoff=-10800,zone=-03"},
    {"America/Argentina/San_Juan", "2022-6-30 7:40:14 wday=6,yday=0,isdst=0,gmtoff=-10800,zone=-03"},
    {"America/Argentina/Mendoza", "2022-6-30 7:40:14 wday=6,yday=0,isdst=0,gmtoff=-10800,zone=-03"},
    {"America/Argentina/Tucuman", "2022-6-30 7:40:14 wday=6,yday=0,isdst=0,gmtoff=-10800,zone=-03"},
    {"America/Argentina/Catamarca", "2022-6-30 7:40:14 wday=6,yday=0,isdst=0,gmtoff=-10800,zone=-03"},
    {"America/Argentina/Cordoba", "2022-6-30 7:40:14 wday=6,yday=0,isdst=0,gmtoff=-10800,zone=-03"},
    {"America/Argentina/Buenos_Aires", "2022-6-30 7:40:14 wday=6,yday=0,isdst=0,gmtoff=-10800,zone=-03"},
    {"America/Argentina/La_Rioja", "2022-6-30 7:40:14 wday=6,yday=0,isdst=0,gmtoff=-10800,zone=-03"},
    {"America/Argentina/San_Luis", "2022-6-30 7:40:14 wday=6,yday=0,isdst=0,gmtoff=-10800,zone=-03"},
    {"America/Argentina/Ushuaia", "2022-6-30 7:40:14 wday=6,yday=0,isdst=0,gmtoff=-10800,zone=-03"},
    {"America/Argentina/Rio_Gallegos", "2022-6-30 7:40:14 wday=6,yday=0,isdst=0,gmtoff=-10800,zone=-03"},
    {"America/Port_of_Spain", "2022-6-30 6:40:14 wday=6,yday=0,isdst=0,gmtoff=-14400,zone=AST"},
    {"America/Boise", "2022-6-30 4:40:14 wday=6,yday=0,isdst=1,gmtoff=-21600,zone=MDT"},
    {"America/Aruba", "2022-6-30 6:40:14 wday=6,yday=0,isdst=0,gmtoff=-14400,zone=AST"},
    {"America/Martinique", "2022-6-30 6:40:14 wday=6,yday=0,isdst=0,gmtoff=-14400,zone=AST"},
    {"America/Matamoros", "2022-6-30 5:40:14 wday=6,yday=0,isdst=1,gmtoff=-18000,zone=CDT"},
    {"America/Guatemala", "2022-6-30 4:40:14 wday=6,yday=0,isdst=0,gmtoff=-21600,zone=CST"},
    {"America/Bogota", "2022-6-30 5:40:14 wday=6,yday=0,isdst=0,gmtoff=-18000,zone=-05"},
    {"America/St_Vincent", "2022-6-30 6:40:14 wday=6,yday=0,isdst=0,gmtoff=-14400,zone=AST"},
    {"America/Managua", "2022-6-30 4:40:14 wday=6,yday=0,isdst=0,gmtoff=-21600,zone=CST"},
    {"America/Montserrat", "2022-6-30 6:40:14 wday=6,yday=0,isdst=0,gmtoff=-14400,zone=AST"},
    {"America/Rio_Branco", "2022-6-30 5:40:14 wday=6,yday=0,isdst=0,gmtoff=-18000,zone=-05"},
    {"America/Tortola", "2022-6-30 6:40:14 wday=6,yday=0,isdst=0,gmtoff=-14400,zone=AST"},
    {"America/El_Salvador", "2022-6-30 4:40:14 wday=6,yday=0,isdst=0,gmtoff=-21600,zone=CST"},
    {"America/Sao_Paulo", "2022-6-30 7:40:14 wday=6,yday=0,isdst=0,gmtoff=-10800,zone=-03"},
    {"America/Whitehorse", "2022-6-30 3:40:14 wday=6,yday=0,isdst=0,gmtoff=-25200,zone=MST"},
    {"America/Dominica", "2022-6-30 6:40:14 wday=6,yday=0,isdst=0,gmtoff=-14400,zone=AST"},
    {"America/Goose_Bay", "2022-6-30 7:40:14 wday=6,yday=0,isdst=1,gmtoff=-10800,zone=ADT"},
    {"America/Sitka", "2022-6-30 2:40:14 wday=6,yday=0,isdst=1,gmtoff=-28800,zone=AKDT"},
    {"America/Panama", "2022-6-30 5:40:14 wday=6,yday=0,isdst=0,gmtoff=-18000,zone=EST"},
    {"America/Regina", "2022-6-30 4:40:14 wday=6,yday=0,isdst=0,gmtoff=-21600,zone=CST"},
    {"America/Fort_Nelson", "2022-6-30 3:40:14 wday=6,yday=0,isdst=0,gmtoff=-25200,zone=MST"},
    {"America/Dawson_Creek", "2022-6-30 3:40:14 wday=6,yday=0,isdst=0,gmtoff=-25200,zone=MST"},
    {"America/La_Paz", "2022-6-30 6:40:14 wday=6,yday=0,isdst=0,gmtoff=-14400,zone=-04"},
    {"America/Thunder_Bay", "2022-6-30 6:40:14 wday=6,yday=0,isdst=1,gmtoff=-14400,zone=EDT"},
    {"America/Bahia_Banderas", "2022-6-30 5:40:14 wday=6,yday=0,isdst=1,gmtoff=-18000,zone=CDT"},
    {"America/Chicago", "2022-6-30 5:40:14 wday=6,yday=0,isdst=1,gmtoff=-18000,zone=CDT"},
    {"America/Mexico_City", "2022-6-30 5:40:14 wday=6,yday=0,isdst=1,gmtoff=-18000,zone=CDT"},
    {"America/Rankin_Inlet", "2022-6-30 5:40:14 wday=6,yday=0,isdst=1,gmtoff=-18000,zone=CDT"},
    {"America/St_Barthelemy", "2022-6-30 6:40:14 wday=6,yday=0,isdst=0,gmtoff=-14400,zone=AST"},
    {"America/Blanc-Sablon", "2022-6-30 6:40:14 wday=6,yday=0,isdst=0,gmtoff=-14400,zone=AST"},
    {"America/Jamaica", "2022-6-30 5:40:14 wday=6,yday=0,isdst=0,gmtoff=-18000,zone=EST"},
    {"America/Cambridge_Bay", "2022-6-30 4:40:14 wday=6,yday=0,isdst=1,gmtoff=-21600,zone=MDT"},
    {"America/Belize", "2022-6-30 4:40:14 wday=6,yday=0,isdst=0,gmtoff=-21600,zone=CST"},
    {"America/Caracas", "2022-6-30 6:40:14 wday=6,yday=0,isdst=0,gmtoff=-14400,zone=-04"},
    {"America/Pangnirtung", "2022-6-30 6:40:14 wday=6,yday=0,isdst=1,gmtoff=-14400,zone=EDT"},
    {"America/Tijuana", "2022-6-30 3:40:14 wday=6,yday=0,isdst=1,gmtoff=-25200,zone=PDT"},
    {"America/Eirunepe", "2022-6-30 5:40:14 wday=6,yday=0,isdst=0,gmtoff=-18000,zone=-05"},
    {"America/Hermosillo", "2022-6-30 3:40:14 wday=6,yday=0,isdst=0,gmtoff=-25200,zone=MST"},
    {"America/Thule", "2022-6-30 7:40:14 wday=6,yday=0,isdst=1,gmtoff=-10800,zone=ADT"},
    {"America/Santo_Domingo", "2022-6-30 6:40:14 wday=6,yday=0,isdst=0,gmtoff=-14400,zone=AST"},
    {"America/Yellowknife", "2022-6-30 4:40:14 wday=6,yday=0,isdst=1,gmtoff=-21600,zone=MDT"},
    {"America/Anguilla", "2022-6-30 6:40:14 wday=6,yday=0,isdst=0,gmtoff=-14400,zone=AST"},
    {"America/Boa_Vista", "2022-6-30 6:40:14 wday=6,yday=0,isdst=0,gmtoff=-14400,zone=-04"},
    {"America/Grand_Turk", "2022-6-30 6:40:14 wday=6,yday=0,isdst=1,gmtoff=-14400,zone=EDT"},
    {"America/Ojinaga", "2022-6-30 4:40:14 wday=6,yday=0,isdst=1,gmtoff=-21600,zone=MDT"},
    {"America/Bahia", "2022-6-30 7:40:14 wday=6,yday=0,isdst=0,gmtoff=-10800,zone=-03"},
    {"America/Inuvik", "2022-6-30 4:40:14 wday=6,yday=0,isdst=1,gmtoff=-21600,zone=MDT"},
    {"America/Yakutat", "2022-6-30 2:40:14 wday=6,yday=0,isdst=1,gmtoff=-28800,zone=AKDT"},
    {"America/Santiago", "2022-6-30 6:40:14 wday=6,yday=0,isdst=0,gmtoff=-14400,zone=-04"},
    {"America/Merida", "2022-6-30 5:40:14 wday=6,yday=0,isdst=1,gmtoff=-18000,zone=CDT"},
    {"America/North_Dakota/New_Salem", "2022-6-30 5:40:14 wday=6,yday=0,isdst=1,gmtoff=-18000,zone=CDT"},
    {"America/North_Dakota/Beulah", "2022-6-30 5:40:14 wday=6,yday=0,isdst=1,gmtoff=-18000,zone=CDT"},
    {"America/North_Dakota/Center", "2022-6-30 5:40:14 wday=6,yday=0,isdst=1,gmtoff=-18000,zone=CDT"},
    {"America/Guayaquil", "2022-6-30 5:40:14 wday=6,yday=0,isdst=0,gmtoff=-18000,zone=-05"},
    {"America/Toronto", "2022-6-30 6:40:14 wday=6,yday=0,isdst=1,gmtoff=-14400,zone=EDT"},
    {"America/Curacao", "2022-6-30 6:40:14 wday=6,yday=0,isdst=0,gmtoff=-14400,zone=AST"},
    {"America/Puerto_Rico", "2022-6-30 6:40:14 wday=6,yday=0,isdst=0,gmtoff=-14400,zone=AST"},
    {"America/Vancouver", "2022-6-30 3:40:14 wday=6,yday=0,isdst=1,gmtoff=-25200,zone=PDT"},
    {"America/Cayman", "2022-6-30 5:40:14 wday=6,yday=0,isdst=0,gmtoff=-18000,zone=EST"},
    {"America/Adak", "2022-6-30 1:40:14 wday=6,yday=0,isdst=1,gmtoff=-32400,zone=HDT"},
    {"America/Mazatlan", "2022-6-30 4:40:14 wday=6,yday=0,isdst=1,gmtoff=-21600,zone=MDT"},
    {"America/Anchorage", "2022-6-30 2:40:14 wday=6,yday=0,isdst=1,gmtoff=-28800,zone=AKDT"},
    {"America/Porto_Velho", "2022-6-30 6:40:14 wday=6,yday=0,isdst=0,gmtoff=-14400,zone=-04"},
    {"America/Denver", "2022-6-30 4:40:14 wday=6,yday=0,isdst=1,gmtoff=-21600,zone=MDT"},
    {"America/New_York", "2022-6-30 6:40:14 wday=6,yday=0,isdst=1,gmtoff=-14400,zone=EDT"},
    {"America/Los_Angeles", "2022-6-30 3:40:14 wday=6,yday=0,isdst=1,gmtoff=-25200,zone=PDT"},
    {"America/St_Johns", "2022-6-30 8:10:14 wday=6,yday=0,isdst=1,gmtoff=-9000,zone=NDT"},
    {"America/Nassau", "2022-6-30 6:40:14 wday=6,yday=0,isdst=1,gmtoff=-14400,zone=EDT"},
    {"America/Menominee", "2022-6-30 5:40:14 wday=6,yday=0,isdst=1,gmtoff=-18000,zone=CDT"},
    {"America/Dawson", "2022-6-30 3:40:14 wday=6,yday=0,isdst=0,gmtoff=-25200,zone=MST"},
    {"America/Danmarkshavn", "2022-6-30 10:40:14 wday=6,yday=0,isdst=0,gmtoff=0,zone=GMT"},
    {"America/Metlakatla", "2022-6-30 2:40:14 wday=6,yday=0,isdst=1,gmtoff=-28800,zone=AKDT"},
    {"America/Nome", "2022-6-30 2:40:14 wday=6,yday=0,isdst=1,gmtoff=-28800,zone=AKDT"},
    {"America/Rainy_River", "2022-6-30 5:40:14 wday=6,yday=0,isdst=1,gmtoff=-18000,zone=CDT"},
    {"America/Winnipeg", "2022-6-30 5:40:14 wday=6,yday=0,isdst=1,gmtoff=-18000,zone=CDT"},
    {"America/St_Kitts", "2022-6-30 6:40:14 wday=6,yday=0,isdst=0,gmtoff=-14400,zone=AST"},
    {"America/Cayenne", "2022-6-30 7:40:14 wday=6,yday=0,isdst=0,gmtoff=-10800,zone=-03"},
    {"America/Cuiaba", "2022-6-30 6:40:14 wday=6,yday=0,isdst=0,gmtoff=-14400,zone=-04"},
    {"America/Costa_Rica", "2022-6-30 4:40:14 wday=6,yday=0,isdst=0,gmtoff=-21600,zone=CST"},
    {"America/Paramaribo", "2022-6-30 7:40:14 wday=6,yday=0,isdst=0,gmtoff=-10800,zone=-03"},
    {"America/Guadeloupe", "2022-6-30 6:40:14 wday=6,yday=0,isdst=0,gmtoff=-14400,zone=AST"},
    {"America/Miquelon", "2022-6-30 8:40:14 wday=6,yday=0,isdst=1,gmtoff=-7200,zone=-02"},
    {"America/St_Lucia", "2022-6-30 6:40:14 wday=6,yday=0,isdst=0,gmtoff=-14400,zone=AST"},
    {"America/Noronha", "2022-6-30 8:40:14 wday=6,yday=0,isdst=0,gmtoff=-7200,zone=-02"},
    {"America/Resolute", "2022-6-30 5:40:14 wday=6,yday=0,isdst=1,gmtoff=-18000,zone=CDT"},
    {"America/Grenada", "2022-6-30 6:40:14 wday=6,yday=0,isdst=0,gmtoff=-14400,zone=AST"},
    {"America/Barbados", "2022-6-30 6:40:14 wday=6,yday=0,isdst=0,gmtoff=-14400,zone=AST"},
    {"America/Juneau", "2022-6-30 2:40:14 wday=6,yday=0,isdst=1,gmtoff=-28800,zone=AKDT"},
    {"America/Havana", "2022-6-30 6:40:14 wday=6,yday=0,isdst=1,gmtoff=-14400,zone=CDT"},
    {"America/Maceio", "2022-6-30 7:40:14 wday=6,yday=0,isdst=0,gmtoff=-10800,zone=-03"},
    {"America/Scoresbysund", "2022-6-30 10:40:14 wday=6,yday=0,isdst=1,gmtoff=0,zone=+00"},
    {"America/Indiana/Tell_City", "2022-6-30 5:40:14 wday=6,yday=0,isdst=1,gmtoff=-18000,zone=CDT"},
    {"America/Indiana/Petersburg", "2022-6-30 6:40:14 wday=6,yday=0,isdst=1,gmtoff=-14400,zone=EDT"},
    {"America/Indiana/Vevay", "2022-6-30 6:40:14 wday=6,yday=0,isdst=1,gmtoff=-14400,zone=EDT"},
    {"America/Indiana/Indianapolis", "2022-6-30 6:40:14 wday=6,yday=0,isdst=1,gmtoff=-14400,zone=EDT"},
    {"America/Indiana/Knox", "2022-6-30 5:40:14 wday=6,yday=0,isdst=1,gmtoff=-18000,zone=CDT"},
    {"America/Indiana/Marengo", "2022-6-30 6:40:14 wday=6,yday=0,isdst=1,gmtoff=-14400,zone=EDT"},
    {"America/Indiana/Vincennes", "2022-6-30 6:40:14 wday=6,yday=0,isdst=1,gmtoff=-14400,zone=EDT"},
    {"America/Indiana/Winamac", "2022-6-30 6:40:14 wday=6,yday=0,isdst=1,gmtoff=-14400,zone=EDT"},
    {"America/Monterrey", "2022-6-30 5:40:14 wday=6,yday=0,isdst=1,gmtoff=-18000,zone=CDT"},
    {"America/Recife", "2022-6-30 7:40:14 wday=6,yday=0,isdst=0,gmtoff=-10800,zone=-03"},
    {"America/Detroit", "2022-6-30 6:40:14 wday=6,yday=0,isdst=1,gmtoff=-14400,zone=EDT"},
    {"America/Manaus", "2022-6-30 6:40:14 wday=6,yday=0,isdst=0,gmtoff=-14400,zone=-04"},
    {"America/Swift_Current", "2022-6-30 4:40:14 wday=6,yday=0,isdst=0,gmtoff=-21600,zone=CST"},
    {"America/Nipigon", "2022-6-30 6:40:14 wday=6,yday=0,isdst=1,gmtoff=-14400,zone=EDT"},
    {"America/Marigot", "2022-6-30 6:40:14 wday=6,yday=0,isdst=0,gmtoff=-14400,zone=AST"},
    {"America/St_Thomas", "2022-6-30 6:40:14 wday=6,yday=0,isdst=0,gmtoff=-14400,zone=AST"},
    {"America/Atikokan", "2022-6-30 5:40:14 wday=6,yday=0,isdst=0,gmtoff=-18000,zone=EST"},
    {"America/Halifax", "2022-6-30 7:40:14 wday=6,yday=0,isdst=1,gmtoff=-10800,zone=ADT"},
    {"America/Cancun", "2022-6-30 5:40:14 wday=6,yday=0,isdst=0,gmtoff=-18000,zone=EST"},
    {"America/Araguaina", "2022-6-30 7:40:14 wday=6,yday=0,isdst=0,gmtoff=-10800,zone=-03"},
    {"America/Lower_Princes", "2022-6-30 6:40:14 wday=6,yday=0,isdst=0,gmtoff=-14400,zone=AST"},
    {"America/Creston", "2022-6-30 3:40:14 wday=6,yday=0,isdst=0,gmtoff=-25200,zone=MST"},
    {"America/Kralendijk", "2022-6-30 6:40:14 wday=6,yday=0,isdst=0,gmtoff=-14400,zone=AST"},
    {"America/Glace_Bay", "2022-6-30 7:40:14 wday=6,yday=0,isdst=1,gmtoff=-10800,zone=ADT"},
    {"America/Fortaleza", "2022-6-30 7:40:14 wday=6,yday=0,isdst=0,gmtoff=-10800,zone=-03"},
    {"America/Santarem", "2022-6-30 7:40:14 wday=6,yday=0,isdst=0,gmtoff=-10800,zone=-03"},
    {"America/Chihuahua", "2022-6-30 4:40:14 wday=6,yday=0,isdst=1,gmtoff=-21600,zone=MDT"},
    {"America/Port-au-Prince", "2022-6-30 6:40:14 wday=6,yday=0,isdst=1,gmtoff=-14400,zone=EDT"},
    {"America/Montevideo", "2022-6-30 7:40:14 wday=6,yday=0,isdst=0,gmtoff=-10800,zone=-03"},
    {"HST", "2022-6-30 0:40:14 wday=6,yday=0,isdst=0,gmtoff=-36000,zone=HST"},
    {"Asia/Irkutsk", "2022-6-30 18:40:14 wday=6,yday=0,isdst=0,gmtoff=28800,zone=+08"},
    {"Asia/Almaty", "2022-6-30 16:40:14 wday=6,yday=0,isdst=0,gmtoff=21600,zone=+06"},
    {"Asia/Phnom_Penh", "2022-6-30 17:40:14 wday=6,yday=0,isdst=0,gmtoff=25200,zone=+07"},
    {"Asia/Hong_Kong", "2022-6-30 18:40:14 wday=6,yday=0,isdst=0,gmtoff=28800,zone=HKT"},
    {"Asia/Dushanbe", "2022-6-30 15:40:14 wday=6,yday=0,isdst=0,gmtoff=18000,zone=+05"},
    {"Asia/Yakutsk", "2022-6-30 19:40:14 wday=6,yday=0,isdst=0,gmtoff=32400,zone=+09"},
    {"Asia/Aqtobe", "2022-6-30 15:40:14 wday=6,yday=0,isdst=0,gmtoff=18000,zone=+05"},
    {"Asia/Nicosia", "2022-6-30 13:40:14 wday=6,yday=0,isdst=1,gmtoff=10800,zone=EEST"},
    {"Asia/Pontianak", "2022-6-30 17:40:14 wday=6,yday=0,isdst=0,gmtoff=25200,zone=WIB"},
    {"Asia/Tbilisi", "2022-6-30 14:40:14 wday=6,yday=0,isdst=0,gmtoff=14400,zone=+04"},
    {"Asia/Tehran", "2022-6-30 15:10:14 wday=6,yday=0,isdst=1,gmtoff=16200,zone=+0430"},
    {"Asia/Tashkent", "2022-6-30 15:40:14 wday=6,yday=0,isdst=0,gmtoff=18000,zone=+05"},
    {"Asia/Jayapura", "2022-6-30 19:40:14 wday=6,yday=0,isdst=0,gmtoff=32400,zone=WIT"},
    {"Asia/Beirut", "2022-6-30 13:40:14 wday=6,yday=0,isdst=1,gmtoff=10800,zone=EEST"},
    {"Asia/Sakhalin", "2022-6-30 21:40:14 wday=6,yday=0,isdst=0,gmtoff=39600,zone=+11"},
    {"Asia/Anadyr", "2022-6-30 22:40:14 wday=6,yday=0,isdst=0,gmtoff=43200,zone=+12"},
    {"Asia/Yekaterinburg", "2022-6-30 15:40:14 wday=6,yday=0,isdst=0,gmtoff=18000,zone=+05"},
    {"Asia/Urumqi", "2022-6-30 16:40:14 wday=6,yday=0,isdst=0,gmtoff=21600,zone=+06"},
    {"Asia/Damascus", "2022-6-30 13:40:14 wday=6,yday=0,isdst=1,gmtoff=10800,zone=EEST"},
    {"Asia/Shanghai", "2022-6-30 18:40:14 wday=6,yday=0,isdst=0,gmtoff=28800,zone=CST"},
    {"Asia/Baghdad", "2022-6-30 13:40:14 wday=6,yday=0,isdst=0,gmtoff=10800,zone=+03"},
    {"Asia/Ho_Chi_Minh", "2022-6-30 17:40:14 wday=6,yday=0,isdst=0,gmtoff=25200,zone=+07"},
    {"Asia/Famagusta", "2022-6-30 13:40:14 wday=6,yday=0,isdst=1,gmtoff=10800,zone=EEST"},
    {"Asia/Barnaul", "2022-6-30 17:40:14 wday=6,yday=0,isdst=0,gmtoff=25200,zone=+07"},
    {"Asia/Tomsk", "2022-6-30 17:40:14 wday=6,yday=0,isdst=0,gmtoff=25200,zone=+07"},
    {"Asia/Samarkand", "2022-6-30 15:40:14 wday=6,yday=0,isdst=0,gmtoff=18000,zone=+05"},
    {"Asia/Oral", "2022-6-30 15:40:14 wday=6,yday=0,isdst=0,gmtoff=18000,zone=+05"},
    {"Asia/Hovd", "2022-6-30 17:40:14 wday=6,yday=0,isdst=0,gmtoff=25200,zone=+07"},
    {"Asia/Qatar", "2022-6-30 13:40:14 wday=6,yday=0,isdst=0,gmtoff=10800,zone=+03"},
    {"Asia/Macau", "2022-6-30 18:40:14 wday=6,yday=0,isdst=0,gmtoff=28800,zone=CST"},
    {"Asia/Kathmandu", "2022-6-30 16:25:14 wday=6,yday=0,isdst=0,gmtoff=20700,zone=+0545"},
    {"Asia/Krasnoyarsk", "2022-6-30 17:40:14 wday=6,yday=0,isdst=0,gmtoff=25200,zone=+07"},
    {"Asia/Istanbul", "2022-6-30 13:40:14 wday=6,yday=0,isdst=0,gmtoff=10800,zone=+03"},
    {"Asia/Ashgabat", "2022-6-30 15:40:14 wday=6,yday=0,isdst=0,gmtoff=18000,zone=+05"},
    {"Asia/Omsk", "2022-6-30 16:40:14 wday=6,yday=0,isdst=0,gmtoff=21600,zone=+06"},
    {"Asia/Amman", "2022-6-30 13:40:14 wday=6,yday=0,isdst=1,gmtoff=10800,zone=EEST"},
    {"Asia/Chita", "2022-6-30 19:40:14 wday=6,yday=0,isdst=0,gmtoff=32400,zone=+09"},
    {"Asia/Singapore", "2022-6-30 18:40:14 wday=6,yday=0,isdst=0,gmtoff=28800,zone=+08"},
    {"Asia/Vladivostok", "2022-6-30 20:40:14 wday=6,yday=0,isdst=0,gmtoff=36000,zone=+10"},
    {"Asia/Kuching", "2022-6-30 18:40:14 wday=6,yday=0,isdst=0,gmtoff=28800,zone=+08"},
    {"Asia/Bahrain", "2022-6-30 13:40:14 wday=6,yday=0,isdst=0,gmtoff=10800,zone=+03"},
    {"Asia/Choibalsan", "2022-6-30 18:40:14 wday=6,yday=0,isdst=0,gmtoff=28800,zone=+08"},
    {"Asia/Kolkata", "2022-6-30 16:10:14 wday=6,yday=0,isdst=0,gmtoff=19800,zone=IST"},
    {"Asia/Colombo", "2022-6-30 16:10:14 wday=6,yday=0,isdst=0,gmtoff=19800,zone=+0530"},
    {"Asia/Ulaanbaatar", "2022-6-30 18:40:14 wday=6,yday=0,isdst=0,gmtoff=28800,zone=+08"},
    {"Asia/Qyzylorda", "2022-6-30 15:40:14 wday=6,yday=0,isdst=0,gmtoff=18000,zone=+05"},
    {"Asia/Tokyo", "2022-6-30 19:40:14 wday=6,yday=0,isdst=0,gmtoff=32400,zone=JST"},
    {"Asia/Yerevan", "2022-6-30 14:40:14 wday=6,yday=0,isdst=0,gmtoff=14400,zone=+04"},
    {"Asia/Kamchatka", "2022-6-30 22:40:14 wday=6,yday=0,isdst=0,gmtoff=43200,zone=+12"},
    {"Asia/Ust-Nera", "2022-6-30 20:40:14 wday=6,yday=0,isdst=0,gmtoff=36000,zone=+10"},
    {"Asia/Vientiane", "2022-6-30 17:40:14 wday=6,yday=0,isdst=0,gmtoff=25200,zone=+07"},
    {"Asia/Taipei", "2022-6-30 18:40:14 wday=6,yday=0,isdst=0,gmtoff=28800,zone=CST"},
    {"Asia/Baku", "2022-6-30 14:40:14 wday=6,yday=0,isdst=0,gmtoff=14400,zone=+04"},
    {"Asia/Makassar", "2022-6-30 18:40:14 wday=6,yday=0,isdst=0,gmtoff=28800,zone=WITA"},
    {"Asia/Novosibirsk", "2022-6-30 17:40:14 wday=6,yday=0,isdst=0,gmtoff=25200,zone=+07"},
    {"Asia/Riyadh", "2022-6-30 13:40:14 wday=6,yday=0,isdst=0,gmtoff=10800,zone=+03"},
    {"Asia/Aden", "2022-6-30 13:40:14 wday=6,yday=0,isdst=0,gmtoff=10800,zone=+03"},
    {"Asia/Bangkok", "2022-6-30 17:40:14 wday=6,yday=0,isdst=0,gmtoff=25200,zone=+07"},
    {"Asia/Magadan", "2022-6-30 21:40:14 wday=6,yday=0,isdst=0,gmtoff=39600,zone=+11"},
    {"Asia/Dubai", "2022-6-30 14:40:14 wday=6,yday=0,isdst=0,gmtoff=14400,zone=+04"},
    {"Asia/Kuwait", "2022-6-30 13:40:14 wday=6,yday=0,isdst=0,gmtoff=10800,zone=+03"},
    {"Asia/Novokuznetsk", "2022-6-30 17:40:14 wday=6,yday=0,isdst=0,gmtoff=25200,zone=+07"},
    {"Asia/Aqtau", "2022-6-30 15:40:14 wday=6,yday=0,isdst=0,gmtoff=18000,zone=+05"},
    {"Asia/Thimphu", "2022-6-30 16:40:14 wday=6,yday=0,isdst=0,gmtoff=21600,zone=+06"},
    {"Asia/Hebron", "2022-6-30 13:40:14 wday=6,yday=0,isdst=1,gmtoff=10800,zone=EEST"},
    {"Asia/Brunei", "2022-6-30 18:40:14 wday=6,yday=0,isdst=0,gmtoff=28800,zone=+08"},
    {"Asia/Khandyga", "2022-6-30 19:40:14 wday=6,yday=0,isdst=0,gmtoff=32400,zone=+09"},
    {"Asia/Kuala_Lumpur", "2022-6-30 18:40:14 wday=6,yday=0,isdst=0,gmtoff=28800,zone=+08"},
    {"Asia/Srednekolymsk", "2022-6-30 21:40:14 wday=6,yday=0,isdst=0,gmtoff=39600,zone=+11"},
    {"Asia/Dili", "2022-6-30 19:40:14 wday=6,yday=0,isdst=0,gmtoff=32400,zone=+09"},
    {"Asia/Karachi", "2022-6-30 15:40:14 wday=6,yday=0,isdst=0,gmtoff=18000,zone=PKT"},
    {"Asia/Muscat", "2022-6-30 14:40:14 wday=6,yday=0,isdst=0,gmtoff=14400,zone=+04"},
    {"Asia/Qostanay", "2022-6-30 16:40:14 wday=6,yday=0,isdst=0,gmtoff=21600,zone=+06"},
    {"Asia/Dhaka", "2022-6-30 16:40:14 wday=6,yday=0,isdst=0,gmtoff=21600,zone=+06"},
    {"Asia/Kabul", "2022-6-30 15:10:14 wday=6,yday=0,isdst=0,gmtoff=16200,zone=+0430"},
    {"Asia/Jakarta", "2022-6-30 17:40:14 wday=6,yday=0,isdst=0,gmtoff=25200,zone=WIB"},
    {"Asia/Manila", "2022-6-30 18:40:14 wday=6,yday=0,isdst=0,gmtoff=28800,zone=PST"},
    {"Asia/Gaza", "2022-6-30 13:40:14 wday=6,yday=0,isdst=1,gmtoff=10800,zone=EEST"},
    {"Asia/Bishkek", "2022-6-30 16:40:14 wday=6,yday=0,isdst=0,gmtoff=21600,zone=+06"},
    {"Asia/Jerusalem", "2022-6-30 13:40:14 wday=6,yday=0,isdst=1,gmtoff=10800,zone=IDT"},
    {"Asia/Atyrau", "2022-6-30 15:40:14 wday=6,yday=0,isdst=0,gmtoff=18000,zone=+05"},
    {"Asia/Pyongyang", "2022-6-30 19:40:14 wday=6,yday=0,isdst=0,gmtoff=32400,zone=KST"},
    {"Asia/Yangon", "2022-6-30 17:10:14 wday=6,yday=0,isdst=0,gmtoff=23400,zone=+0630"},
    {"Asia/Seoul", "2022-6-30 19:40:14 wday=6,yday=0,isdst=0,gmtoff=32400,zone=KST"},
    {"WET", "2022-6-30 11:40:14 wday=6,yday=0,isdst=1,gmtoff=3600,zone=WEST"},
    {"Indian/Mayotte", "2022-6-30 13:40:14 wday=6,yday=0,isdst=0,gmtoff=10800,zone=EAT"},
    {"Indian/Comoro", "2022-6-30 13:40:14 wday=6,yday=0,isdst=0,gmtoff=10800,zone=EAT"},
    {"Indian/Chagos", "2022-6-30 16:40:14 wday=6,yday=0,isdst=0,gmtoff=21600,zone=+06"},
    {"Indian/Reunion", "2022-6-30 14:40:14 wday=6,yday=0,isdst=0,gmtoff=14400,zone=+04"},
    {"Indian/Mahe", "2022-6-30 14:40:14 wday=6,yday=0,isdst=0,gmtoff=14400,zone=+04"},
    {"Indian/Christmas", "2022-6-30 17:40:14 wday=6,yday=0,isdst=0,gmtoff=25200,zone=+07"},
    {"Indian/Mauritius", "2022-6-30 14:40:14 wday=6,yday=0,isdst=0,gmtoff=14400,zone=+04"},
    {"Indian/Maldives", "2022-6-30 15:40:14 wday=6,yday=0,isdst=0,gmtoff=18000,zone=+05"},
    {"Indian/Kerguelen", "2022-6-30 15:40:14 wday=6,yday=0,isdst=0,gmtoff=18000,zone=+05"},
    {"Indian/Antananarivo", "2022-6-30 13:40:14 wday=6,yday=0,isdst=0,gmtoff=10800,zone=EAT"},
    {"Indian/Cocos", "2022-6-30 17:10:14 wday=6,yday=0,isdst=0,gmtoff=23400,zone=+0630"},
    {"CST6CDT", "2022-6-30 5:40:14 wday=6,yday=0,isdst=1,gmtoff=-18000,zone=CDT"},
    {"EST", "2022-6-30 5:40:14 wday=6,yday=0,isdst=0,gmtoff=-18000,zone=EST"},
    {"Africa/Bujumbura", "2022-6-30 12:40:14 wday=6,yday=0,isdst=0,gmtoff=7200,zone=CAT"},
    {"Africa/Casablanca", "2022-6-30 11:40:14 wday=6,yday=0,isdst=0,gmtoff=3600,zone=+01"},
    {"Africa/Lome", "2022-6-30 10:40:14 wday=6,yday=0,isdst=0,gmtoff=0,zone=GMT"},
    {"Africa/Monrovia", "2022-6-30 10:40:14 wday=6,yday=0,isdst=0,gmtoff=0,zone=GMT"},
    {"Africa/Dar_es_Salaam", "2022-6-30 13:40:14 wday=6,yday=0,isdst=0,gmtoff=10800,zone=EAT"},
    {"Africa/Lagos", "2022-6-30 11:40:14 wday=6,yday=0,isdst=0,gmtoff=3600,zone=WAT"},
    {"Africa/Tripoli", "2022-6-30 12:40:14 wday=6,yday=0,isdst=0,gmtoff=7200,zone=EET"},
    {"Africa/Tunis", "2022-6-30 11:40:14 wday=6,yday=0,isdst=0,gmtoff=3600,zone=CET"},
    {"Africa/Nairobi", "2022-6-30 13:40:14 wday=6,yday=0,isdst=0,gmtoff=10800,zone=EAT"},
    {"Africa/Banjul", "2022-6-30 10:40:14 wday=6,yday=0,isdst=0,gmtoff=0,zone=GMT"},
    {"Africa/Djibouti", "2022-6-30 13:40:14 wday=6,yday=0,isdst=0,gmtoff=10800,zone=EAT"},
    {"Africa/Luanda", "2022-6-30 11:40:14 wday=6,yday=0,isdst=0,gmtoff=3600,zone=WAT"},
    {"Africa/Lusaka", "2022-6-30 12:40:14 wday=6,yday=0,isdst=0,gmtoff=7200,zone=CAT"},
    {"Africa/Maputo", "2022-6-30 12:40:14 wday=6,yday=0,isdst=0,gmtoff=7200,zone=CAT"},
    {"Africa/Juba", "2022-6-30 12:40:14 wday=6,yday=0,isdst=0,gmtoff=7200,zone=CAT"},
    {"Africa/Harare", "2022-6-30 12:40:14 wday=6,yday=0,isdst=0,gmtoff=7200,zone=CAT"},
    {"Africa/Libreville", "2022-6-30 11:40:14 wday=6,yday=0,isdst=0,gmtoff=3600,zone=WAT"},
    {"Africa/Khartoum", "2022-6-30 12:40:14 wday=6,yday=0,isdst=0,gmtoff=7200,zone=CAT"},
    {"Africa/Bangui", "2022-6-30 11:40:14 wday=6,yday=0,isdst=0,gmtoff=3600,zone=WAT"},
    {"Africa/Lubumbashi", "2022-6-30 12:40:14 wday=6,yday=0,isdst=0,gmtoff=7200,zone=CAT"},
    {"Africa/Brazzaville", "2022-6-30 11:40:14 wday=6,yday=0,isdst=0,gmtoff=3600,zone=WAT"},
    {"Africa/Cairo", "2022-6-30 12:40:14 wday=6,yday=0,isdst=0,gmtoff=7200,zone=EET"},
    {"Africa/Ouagadougou", "2022-6-30 10:40:14 wday=6,yday=0,isdst=0,gmtoff=0,zone=GMT"},
    {"Africa/Addis_Ababa", "2022-6-30 13:40:14 wday=6,yday=0,isdst=0,gmtoff=10800,zone=EAT"},
    {"Africa/Accra", "2022-6-30 10:40:14 wday=6,yday=0,isdst=0,gmtoff=0,zone=GMT"},
    {"Africa/Windhoek", "2022-6-30 12:40:14 wday=6,yday=0,isdst=0,gmtoff=7200,zone=CAT"},
    {"Africa/Malabo", "2022-6-30 11:40:14 wday=6,yday=0,isdst=0,gmtoff=3600,zone=WAT"},
    {"Africa/Mbabane", "2022-6-30 12:40:14 wday=6,yday=0,isdst=0,gmtoff=7200,zone=SAST"},
    {"Africa/Ceuta", "2022-6-30 12:40:14 wday=6,yday=0,isdst=1,gmtoff=7200,zone=CEST"},
    {"Africa/Maseru", "2022-6-30 12:40:14 wday=6,yday=0,isdst=0,gmtoff=7200,zone=SAST"},
    {"Africa/Bamako", "2022-6-30 10:40:14 wday=6,yday=0,isdst=0,gmtoff=0,zone=GMT"},
    {"Africa/Conakry", "2022-6-30 10:40:14 wday=6,yday=0,isdst=0,gmtoff=0,zone=GMT"},
    {"Africa/Douala", "2022-6-30 11:40:14 wday=6,yday=0,isdst=0,gmtoff=3600,zone=WAT"},
    {"Africa/Dakar", "2022-6-30 10:40:14 wday=6,yday=0,isdst=0,gmtoff=0,zone=GMT"},
    {"Africa/Freetown", "2022-6-30 10:40:14 wday=6,yday=0,isdst=0,gmtoff=0,zone=GMT"},
    {"Africa/Bissau", "2022-6-30 10:40:14 wday=6,yday=0,isdst=0,gmtoff=0,zone=GMT"},
    {"Africa/Sao_Tome", "2022-6-30 10:40:14 wday=6,yday=0,isdst=0,gmtoff=0,zone=GMT"},
    {"Africa/Kinshasa", "2022-6-30 11:40:14 wday=6,yday=0,isdst=0,gmtoff=3600,zone=WAT"},
    {"Africa/Abidjan", "2022-6-30 10:40:14 wday=6,yday=0,isdst=0,gmtoff=0,zone=GMT"},
    {"Africa/El_Aaiun", "2022-6-30 11:40:14 wday=6,yday=0,isdst=0,gmtoff=3600,zone=+01"},
    {"Africa/Gaborone", "2022-6-30 12:40:14 wday=6,yday=0,isdst=0,gmtoff=7200,zone=CAT"},
    {"Africa/Kampala", "2022-6-30 13:40:14 wday=6,yday=0,isdst=0,gmtoff=10800,zone=EAT"},
    {"Africa/Porto-Novo", "2022-6-30 11:40:14 wday=6,yday=0,isdst=0,gmtoff=3600,zone=WAT"},
    {"Africa/Nouakchott", "2022-6-30 10:40:14 wday=6,yday=0,isdst=0,gmtoff=0,zone=GMT"},
    {"Africa/Algiers", "2022-6-30 11:40:14 wday=6,yday=0,isdst=0,gmtoff=3600,zone=CET"},
    {"Africa/Ndjamena", "2022-6-30 11:40:14 wday=6,yday=0,isdst=0,gmtoff=3600,zone=WAT"},
    {"Africa/Mogadishu", "2022-6-30 13:40:14 wday=6,yday=0,isdst=0,gmtoff=10800,zone=EAT"},
    {"Africa/Niamey", "2022-6-30 11:40:14 wday=6,yday=0,isdst=0,gmtoff=3600,zone=WAT"},
    {"Africa/Blantyre", "2022-6-30 12:40:14 wday=6,yday=0,isdst=0,gmtoff=7200,zone=CAT"},
    {"Africa/Kigali", "2022-6-30 12:40:14 wday=6,yday=0,isdst=0,gmtoff=7200,zone=CAT"},
    {"Africa/Johannesburg", "2022-6-30 12:40:14 wday=6,yday=0,isdst=0,gmtoff=7200,zone=SAST"},
    {"Africa/Asmara", "2022-6-30 13:40:14 wday=6,yday=0,isdst=0,gmtoff=10800,zone=EAT"},
    {"Atlantic/Reykjavik", "2022-6-30 10:40:14 wday=6,yday=0,isdst=0,gmtoff=0,zone=GMT"},
    {"Atlantic/Faroe", "2022-6-30 11:40:14 wday=6,yday=0,isdst=1,gmtoff=3600,zone=WEST"},
    {"Atlantic/Cape_Verde", "2022-6-30 9:40:14 wday=6,yday=0,isdst=0,gmtoff=-3600,zone=-01"},
    {"Atlantic/St_Helena", "2022-6-30 10:40:14 wday=6,yday=0,isdst=0,gmtoff=0,zone=GMT"},
    {"Atlantic/Bermuda", "2022-6-30 7:40:14 wday=6,yday=0,isdst=1,gmtoff=-10800,zone=ADT"},
    {"Atlantic/Stanley", "2022-6-30 7:40:14 wday=6,yday=0,isdst=0,gmtoff=-10800,zone=-03"},
    {"Atlantic/Madeira", "2022-6-30 11:40:14 wday=6,yday=0,isdst=1,gmtoff=3600,zone=WEST"},
    {"Atlantic/Canary", "2022-6-30 11:40:14 wday=6,yday=0,isdst=1,gmtoff=3600,zone=WEST"},
    {"Atlantic/South_Georgia", "2022-6-30 8:40:14 wday=6,yday=0,isdst=0,gmtoff=-7200,zone=-02"},
    {"Atlantic/Azores", "2022-6-30 10:40:14 wday=6,yday=0,isdst=1,gmtoff=0,zone=+00"},
    {"MST7MDT", "2022-6-30 4:40:14 wday=6,yday=0,isdst=1,gmtoff=-21600,zone=MDT"},
    {"Antarctica/Mawson", "2022-6-30 15:40:14 wday=6,yday=0,isdst=0,gmtoff=18000,zone=+05"},
    {"Antarctica/Macquarie", "2022-6-30 20:40:14 wday=6,yday=0,isdst=0,gmtoff=36000,zone=AEST"},
    {"Antarctica/DumontDUrville", "2022-6-30 20:40:14 wday=6,yday=0,isdst=0,gmtoff=36000,zone=+10"},
    {"Antarctica/Davis", "2022-6-30 17:40:14 wday=6,yday=0,isdst=0,gmtoff=25200,zone=+07"},
    {"Antarctica/Rothera", "2022-6-30 7:40:14 wday=6,yday=0,isdst=0,gmtoff=-10800,zone=-03"},
    {"Antarctica/Casey", "2022-6-30 21:40:14 wday=6,yday=0,isdst=0,gmtoff=39600,zone=+11"},
    {"Antarctica/Vostok", "2022-6-30 16:40:14 wday=6,yday=0,isdst=0,gmtoff=21600,zone=+06"},
    {"Antarctica/Palmer", "2022-6-30 7:40:14 wday=6,yday=0,isdst=0,gmtoff=-10800,zone=-03"},
    {"Antarctica/McMurdo", "2022-6-30 22:40:14 wday=6,yday=0,isdst=0,gmtoff=43200,zone=NZST"},
    {"Antarctica/Troll", "2022-6-30 12:40:14 wday=6,yday=0,isdst=1,gmtoff=7200,zone=+02"},
    {"Antarctica/Syowa", "2022-6-30 13:40:14 wday=6,yday=0,isdst=0,gmtoff=10800,zone=+03"},
    {"PST8PDT", "2022-6-30 3:40:14 wday=6,yday=0,isdst=1,gmtoff=-25200,zone=PDT"},
    {"EET", "2022-6-30 13:40:14 wday=6,yday=0,isdst=1,gmtoff=10800,zone=EEST"},
    {"Pacific/Port_Moresby", "2022-6-30 20:40:14 wday=6,yday=0,isdst=0,gmtoff=36000,zone=+10"},
    {"Pacific/Rarotonga", "2022-6-30 0:40:14 wday=6,yday=0,isdst=0,gmtoff=-36000,zone=-10"},
    {"Pacific/Efate", "2022-6-30 21:40:14 wday=6,yday=0,isdst=0,gmtoff=39600,zone=+11"},
    {"Pacific/Midway", "2022-6-29 23:40:14 wday=5,yday=0,isdst=0,gmtoff=-39600,zone=SST"},
    {"Pacific/Gambier", "2022-6-30 1:40:14 wday=6,yday=0,isdst=0,gmtoff=-32400,zone=-09"},
    {"Pacific/Wake", "2022-6-30 22:40:14 wday=6,yday=0,isdst=0,gmtoff=43200,zone=+12"},
    {"Pacific/Pago_Pago", "2022-6-29 23:40:14 wday=5,yday=0,isdst=0,gmtoff=-39600,zone=SST"},
    {"Pacific/Fiji", "2022-6-30 22:40:14 wday=6,yday=0,isdst=0,gmtoff=43200,zone=+12"},
    {"Pacific/Auckland", "2022-6-30 22:40:14 wday=6,yday=0,isdst=0,gmtoff=43200,zone=NZST"},
    {"Pacific/Chatham", "2022-6-30 23:25:14 wday=6,yday=0,isdst=0,gmtoff=45900,zone=+1245"},
    {"Pacific/Pitcairn", "2022-6-30 2:40:14 wday=6,yday=0,isdst=0,gmtoff=-28800,zone=-08"},
    {"Pacific/Galapagos", "2022-6-30 4:40:14 wday=6,yday=0,isdst=0,gmtoff=-21600,zone=-06"},
    {"Pacific/Tahiti", "2022-6-30 0:40:14 wday=6,yday=0,isdst=0,gmtoff=-36000,zone=-10"},
    {"Pacific/Tongatapu", "2022-6-30 23:40:14 wday=6,yday=0,isdst=0,gmtoff=46800,zone=+13"},
    {"Pacific/Kwajalein", "2022-6-30 22:40:14 wday=6,yday=0,isdst=0,gmtoff=43200,zone=+12"},
    {"Pacific/Chuuk", "2022-6-30 20:40:14 wday=6,yday=0,isdst=0,gmtoff=36000,zone=+10"},
    {"Pacific/Fakaofo", "2022-6-30 23:40:14 wday=6,yday=0,isdst=0,gmtoff=46800,zone=+13"},
    {"Pacific/Honolulu", "2022-6-30 0:40:14 wday=6,yday=0,isdst=0,gmtoff=-36000,zone=HST"},
    {"Pacific/Saipan", "2022-6-30 20:40:14 wday=6,yday=0,isdst=0,gmtoff=36000,zone=ChST"},
    {"Pacific/Guadalcanal", "2022-6-30 21:40:14 wday=6,yday=0,isdst=0,gmtoff=39600,zone=+11"},
    {"Pacific/Nauru", "2022-6-30 22:40:14 wday=6,yday=0,isdst=0,gmtoff=43200,zone=+12"},
    {"Pacific/Enderbury", "2022-6-30 23:40:14 wday=6,yday=0,isdst=0,gmtoff=46800,zone=+13"},
    {"Pacific/Bougainville", "2022-6-30 21:40:14 wday=6,yday=0,isdst=0,gmtoff=39600,zone=+11"},
    {"Pacific/Tarawa", "2022-6-30 22:40:14 wday=6,yday=0,isdst=0,gmtoff=43200,zone=+12"},
    {"Pacific/Kiritimati", "2022-6-31 0:40:14 wday=0,yday=0,isdst=0,gmtoff=50400,zone=+14"},
    {"Pacific/Apia", "2022-6-30 23:40:14 wday=6,yday=0,isdst=0,gmtoff=46800,zone=+13"},
    {"Pacific/Niue", "2022-6-29 23:40:14 wday=5,yday=0,isdst=0,gmtoff=-39600,zone=-11"},
    {"Pacific/Marquesas", "2022-6-30 1:10:14 wday=6,yday=0,isdst=0,gmtoff=-34200,zone=-0930"},
    {"Pacific/Kosrae", "2022-6-30 21:40:14 wday=6,yday=0,isdst=0,gmtoff=39600,zone=+11"},
    {"Pacific/Funafuti", "2022-6-30 22:40:14 wday=6,yday=0,isdst=0,gmtoff=43200,zone=+12"},
    {"Pacific/Easter", "2022-6-30 4:40:14 wday=6,yday=0,isdst=0,gmtoff=-21600,zone=-06"},
    {"Pacific/Noumea", "2022-6-30 21:40:14 wday=6,yday=0,isdst=0,gmtoff=39600,zone=+11"},
    {"Pacific/Norfolk", "2022-6-30 21:40:14 wday=6,yday=0,isdst=0,gmtoff=39600,zone=+11"},
    {"Pacific/Guam", "2022-6-30 20:40:14 wday=6,yday=0,isdst=0,gmtoff=36000,zone=ChST"},
    {"Pacific/Wallis", "2022-6-30 22:40:14 wday=6,yday=0,isdst=0,gmtoff=43200,zone=+12"},
    {"Pacific/Pohnpei", "2022-6-30 21:40:14 wday=6,yday=0,isdst=0,gmtoff=39600,zone=+11"},
    {"Pacific/Palau", "2022-6-30 19:40:14 wday=6,yday=0,isdst=0,gmtoff=32400,zone=+09"},
    {"Pacific/Majuro", "2022-6-30 22:40:14 wday=6,yday=0,isdst=0,gmtoff=43200,zone=+12"},
};

static int16_t gBufferSize = 256;
static time_t gTime = 1659177614;
static int16_t gYearBase = 1900;

/**
 * @tc.name      : strptime_0100
 * @tc.desc      : according to different time zones, convert a string to a time
 * type according to a specific time format
 * @tc.level     : Level 0
 */
void strptime_0100(void)
{
    for ( int32_t i = 0; i < (int32_t)(sizeof(gResultData) / sizeof(gResultData[0])); i++) {
        const char *tz = gResultData[i].tz;
        const char *handlerChar;
#ifdef TIME_ZONE_SUB_TAG
        char *str = strrchr(tz, TIME_ZONE_SUB_TAG);
        if (str) {
            handlerChar = ++str;
        } else {
            handlerChar = tz;
        }
#else
        handlerChar = tz;
#endif
        setenv("TZ", handlerChar, 1);
        tzset();
        char buffer[gBufferSize];
        struct tm *timeptr = localtime(&gTime);
        if (!timeptr) {
            EXPECT_PTRNE("strptime_0100", timeptr, NULL);
            return;
        }
        size_t cnt = strftime(buffer, sizeof(buffer) - 1, "%c", timeptr);
        EXPECT_TRUE("strptime_0100", cnt > 0);

        struct tm tmTime = {0};
        strptime(buffer, "%c", &tmTime);
        char *result = asctime(&tmTime);
        if (result == NULL) {
            EXPECT_FALSE("strptime_0100", result == NULL);
            return;
        }
        result[strlen(result) - 1] = 0x00;
        EXPECT_STREQ("strptime_0100", gResultData[i].result, result);
    }
}

/**
 * @tc.name      : strptime_0200
 * @tc.desc      : according to different time zones, convert a string to a time
 * type according to a specific time format
 * @tc.level     : Level 0
 */
void strptime_0200(void)
{
    for ( int32_t i = 0; i < (int32_t)(sizeof(gResultDataZ)/sizeof(gResultDataZ[0])); i++) {
        const char *tz = gResultDataZ[i].tz;
        const char *handlerChar;
#ifdef TIME_ZONE_SUB_TAG
        char *str = strrchr(tz, TIME_ZONE_SUB_TAG);
        if (str) {
            handlerChar = ++str;
        } else {
            handlerChar = tz;
        }
#else
        handlerChar = tz;
#endif
        setenv("TZ", handlerChar, 1);
        tzset();
        char buffer[gBufferSize];
        struct tm *timeptr = localtime(&gTime);
        if (!timeptr) {
            EXPECT_TRUE("strptime_0200", timeptr == NULL);
            return;
        }
        size_t len = strftime(buffer, sizeof(buffer) - 1, "%c %Z%z", timeptr);
        EXPECT_TRUE("strptime_0200", len > 0);
        struct tm tmTime = {0};
        strptime(buffer, "%c %Z%z", &tmTime);
        char buffResult[gBufferSize];

        int cnt = sprintf(buffResult, "%d-%d-%d %d:%d:%d wday=%d,yday=%d,isdst=%d,gmtoff=%ld,zone=%s",
            (tmTime.tm_year+gYearBase), tmTime.tm_mon, tmTime.tm_mday, tmTime.tm_hour,
            tmTime.tm_min, tmTime.tm_sec, tmTime.tm_wday, tmTime.tm_yday, tmTime.tm_isdst,
            tmTime.tm_gmtoff, tmTime.tm_zone);
        EXPECT_TRUE("strptime_0200", cnt > 0);
        EXPECT_STREQ("strptime_0200", gResultDataZ[i].result, buffResult);
    }
}

/**
 * @tc.name      : strptime_0300
 * @tc.desc      : according to different time zones, convert a string to a time
 * type according to a specific time format
 * @tc.level     : Level 0
 */
void strptime_0300(void)
{
    char *buffer = "2022-04-10";
    struct tm tmTime = {0};
    strptime(buffer, "%F", &tmTime);
    char buffResult[gBufferSize];
    int cnt = sprintf(buffResult, "%04d-%02d-%02d",
        (tmTime.tm_year+gYearBase), tmTime.tm_mon + 1, tmTime.tm_mday);
    EXPECT_TRUE("strptime_0300", cnt > 0);
    EXPECT_STREQ("strptime_0300", buffer, buffResult);
}

/**
 * @tc.name      : strptime_0400
 * @tc.desc      : according to different time zones, convert a string to a time
 * type according to a specific time format
 * @tc.level     : Level 0
 */
void strptime_0400(void)
{
    char *buffer = "23";
    struct tm tmTime = {0};
    strptime(buffer, "%g", &tmTime);
    char buffResult[gBufferSize];
    int cnt = sprintf(buffResult, "%d", tmTime.tm_year);
    EXPECT_TRUE("strptime_0400", cnt > 0);
    EXPECT_STREQ("strptime_0400", buffer, buffResult);
}

/**
 * @tc.name      : strptime_0500
 * @tc.desc      : according to different time zones, convert a string to a time
 * type according to a specific time format
 * @tc.level     : Level 0
 */
void strptime_0500(void)
{
    const char *buffer = "16";
    struct tm tmTime = {0};
    strptime(buffer, " %k", &tmTime);
    char buffResult[gBufferSize];
    int cnt = sprintf(buffResult, "%d", tmTime.tm_hour);
    EXPECT_TRUE("strptime_0500", cnt > 0);
    EXPECT_STREQ("strptime_0500", buffer, buffResult);
}

/**
 * @tc.name      : strptime_0600
 * @tc.desc      : according to different time zones, convert a string to a time
 * type according to a specific time format
 * @tc.level     : Level 0
 */
void strptime_0600(void)
{
    const char *buffer = " 4";
    struct tm tmTime = {0};
    strptime(buffer, " %l", &tmTime);
    char buffResult[gBufferSize];
    int cnt = sprintf(buffResult, "%d", tmTime.tm_hour);
    EXPECT_TRUE("strptime_0600", cnt > 0);
    EXPECT_STREQ("strptime_0600", "4", buffResult);
}

/**
 * @tc.name      : strptime_0700
 * @tc.desc      : according to different time zones, convert a string to a time
 * type according to a specific time format
 * @tc.level     : Level 0
 */
void strptime_0700(void)
{
    const char *buffer = "1659177614";
    const char *tz = gResultData[0].tz;
    const char *handlerChar;
#ifdef TIME_ZONE_SUB_TAG
    char *str = strrchr(tz, TIME_ZONE_SUB_TAG);
    if (str) {
        handlerChar = ++str;
    } else {
        handlerChar = tz;
    }
#else
    handlerChar = tz;
#endif
    setenv("TZ", handlerChar, 1);
    time_t second = 0;
    struct tm tmTime = {0};
    strptime(buffer, "%s", &tmTime);
    second = mktime(&tmTime);
    char buffResult[gBufferSize];
    int cnt = sprintf(buffResult, "%ld", second);
    EXPECT_TRUE("strptime_0700", cnt > 0);
    EXPECT_STREQ("strptime_0700", buffer, buffResult);
}

/**
 * @tc.name      : strptime_0800
 * @tc.desc      : according to different time zones, convert a string to a time
 * type according to a specific time format
 * @tc.level     : Level 0
 */
void strptime_0800(void)
{
    const char *buffer = "1";
    struct tm tmTime = {0};
    strptime(buffer, "%u", &tmTime);
    char buffResult[gBufferSize];
    int cnt = sprintf(buffResult, "%d", tmTime.tm_wday);
    EXPECT_TRUE("strptime_0800", cnt > 0);
    EXPECT_STREQ("strptime_0800", buffer, buffResult);
}

/**
 * @tc.name      : strptime_0900
 * @tc.desc      : according to different time zones, convert a string to a time
 * type according to a specific time format
 * @tc.level     : Level 0
 */
void strptime_0900(void)
{
    const char *buffer = "30-Oct-2021";
    struct tm tmTime = {0};
    strptime(buffer, "%v", &tmTime);
    char buffResult[gBufferSize];
    int cnt = sprintf(buffResult, "%d-%d-%d",
        (tmTime.tm_year+gYearBase), tmTime.tm_mon, tmTime.tm_mday);
    EXPECT_TRUE("strptime_0900", cnt > 0);
    EXPECT_STREQ("strptime_0900", "2021-9-30", buffResult);
}

int main(void)
{
    strptime_0100();
    strptime_0200();
    strptime_0300();
    strptime_0400();
    strptime_0500();
    strptime_0600();
    strptime_0700();
    strptime_0800();
    strptime_0900();
    return t_status;
}