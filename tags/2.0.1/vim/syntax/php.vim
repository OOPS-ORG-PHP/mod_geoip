" Vim mod_geoip syntax file
" Language: PHP 5
" Maintainer: JoungKyun.Kim <http://oops.org>
" Last Change: 2013.02.17

" mod_geoip
syn keyword phpFunctions geoip_open geoip_close geoip_database_info geoip_db_avail geoip_country_code_by_name geoip_country_name_by_name geoip_id_by_name geoip_record_by_name geoip_org_by_name contained
syn keyword phpClasses GeoIPException geoip contained
syn keyword phpConstants GEOIP_STANDARD GEOIP_MEMORY_CACHE GEOIP_CHECK_CACHE GEOIP_INDEX_CACHE GEOIP_COUNTRY_EDITION GEOIP_REGION_EDITION_REV0 GEOIP_CITY_EDITION_REV0 GEOIP_ORG_EDITION GEOIP_ISP_EDITION GEOIP_CITY_EDITION_REV1 GEOIP_REGION_EDITION_REV1 GEOIP_PROXY_EDITION GEOIP_ASNUM_EDITION GEOIP_NETSPEED_EDITION GEOIP_DOMAIN_EDITION contained
