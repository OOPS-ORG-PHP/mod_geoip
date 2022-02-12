/*
 * Copyright 2022. JoungKyun.Kim All rights reserved.
 *
 * This file is part of mod_geoip
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU LESSER GENERAL PUBLIC LICENSE as published
 * by the Free Software Foundation; either version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU LESSER GENERAL PUBLIC LICENSE for more details.
 *
 * You should have received a copy of the GNU LESSER GENERAL PUBLIC LICENSE
 * along with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
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

#if PHP_VERSION_ID >= 80000
#define TSRMLS_DC
#define TSRMLS_CC
#endif

#if PHP_VERSION_ID < 80000
#  define ZEND_BEGIN_ARG_WITH_RETURN_TYPE_MASK_EX(a,b,c,d) ZEND_BEGIN_ARG_INFO_EX(a,0,b,c)
#  define ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(a,b,c,d,e) ZEND_ARG_TYPE_INFO(a,b,c,d)
#  define ZEND_ARG_TYPE_MASK(a,b,c,d) ZEND_ARG_INFO(a,b)
#  ifdef ZEND_ARG_OBJ_INFO
#    undef ZEND_ARG_OBJ_INFO
#  endif
#  define ZEND_ARG_OBJ_INFO(a,b,c,d) ZEND_ARG_TYPE_INFO(a,b,IS_RESOURCE,d)
#endif

#if PHP_VERSION_ID < 70200
#  ifdef ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX
#    undef ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX
#  endif
#  define ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(name, return_reference, required_num_args, type, allow_null) \
            static const zend_internal_arg_info name[] = { \
                { (const char*)(zend_uintptr_t)(required_num_args), NULL, type, return_reference, allow_null, 0 },
#  define ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(a,b,c,d,e) ZEND_BEGIN_ARG_INFO_EX(a,0,b,c)
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

