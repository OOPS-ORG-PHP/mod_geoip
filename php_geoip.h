/*
 * $Id$
 */

#ifndef PHP_GEOIP_H
#define PHP_GEOIP_H

extern zend_module_entry geoip_module_entry;
#define phpext_geoip_ptr &geoip_module_entry

#ifdef PHP_WIN32
#define PHP_GEOIP_API __declspec(dllexport)
#else
#define PHP_GEOIP_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

/* PHP_VERSION_ID is supported from 5.2 */
#ifndef PHP_VERSION_ID
#  if ZEND_MODULE_API_NO >= 20050922
#    define PHP_VERSION_ID 50100
#  elif ZEND_MODULE_API_NO >= 20041030
#    define PHP_VERSION_ID 50000
#  elif ZEND_MODULE_API_NO >= 20020429
#    define PHP_VERSION_ID 40400
#  elif ZEND_MODULE_API_NO >= 20020429
#    define PHP_VERSION_ID 40300
#  elif ZEND_MODULE_API_NO >= 20020429
#    define PHP_VERSION_ID 40200
#  elif ZEND_MODULE_API_NO >= 20010901
#    define PHP_VERSION_ID 40100
#  elif ZEND_MODULE_API_NO == 20001222
#    define PHP_VERSION_ID 40000
#  endif

#  ifndef ZEND_MODULE_API_NO
#    define PHP_VERSION_ID 30000
#  endif
#endif /* end of ifndef PHP_VERSION_ID */

#if PHP_MAJOR_VERSION < 5
#  ifdef getThis
#    undef getThis
#  endif
#  define getThis() NULL
#endif

#include <GeoIP.h>
#include <GeoIPCity.h>

PHP_MINIT_FUNCTION(geoip);
PHP_MSHUTDOWN_FUNCTION(geoip);
PHP_RINIT_FUNCTION(geoip);
PHP_RSHUTDOWN_FUNCTION(geoip);
PHP_MINFO_FUNCTION(geoip);


PHP_FUNCTION(geoip_open);
PHP_FUNCTION(geoip_close);
PHP_FUNCTION(geoip_database_info);
PHP_FUNCTION(geoip_db_avail);
PHP_FUNCTION(geoip_country_code_by_name);
PHP_FUNCTION(geoip_country_name_by_name);
PHP_FUNCTION(geoip_id_by_name);
PHP_FUNCTION(geoip_record_by_name);
PHP_FUNCTION(geoip_org_by_name);


ZEND_BEGIN_MODULE_GLOBALS(geoip)
	char* database_standard;
ZEND_END_MODULE_GLOBALS(geoip)

#ifdef ZTS
#define GEOIP_G(v) TSRMG(geoip_globals_id, zend_geoip_globals *, v)
#else
#define GEOIP_G(v) (geoip_globals.v)
#endif

typedef struct geo_resource {
	GeoIP *gi;
	int type;
	int rsrc;
} GeoIP_API;

#endif /* PHP_GEOIP_H */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */

