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
	// for Class
	zend_resource *rsrc;
	GeoIP *gi;
	int type;
} GeoIP_API;

#define GEOIP_FETCH_RESOURCE(a,b,c,d,e) \
	if ( (a = (b) zend_fetch_resource_ex (c, d, e)) == NULL ) \
		RETURN_FALSE

#define gemalloc emalloc
#define gfree(x) if (x!=NULL) { free (x); x = NULL; }
#define gefree(x) if (x!=NULL) { efree (x); x = NULL; }

//define GEOIP_DEBUG
#ifdef GEOIP_DEBUG
#	define GE_PRINT_CALL_API_NAME php_printf ("*** ---------------> Call %s (%s:%d)\n", __func__, __FILE__, __LINE__)
#	define ge_printf(...) \
		php_printf ("    "); \
		php_printf (__VA_ARGS__)
#else
#	define GE_PRINT_CALL_API_NAME
#	define ge_printf
#endif

#define ZARGC ZEND_NUM_ARGS()
#endif /* PHP_GEOIP_H */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */

