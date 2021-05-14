/*
 * Copyright 2021. JoungKyun.Kim all rights reserved
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

/**
 * Project: mod_geoip :: GeoIP php exntension
 * File:    php_geoip.c
 *
 * Copyright 2021. JoungKyun.Kim all rights reserved.
 *
 * LICENSE: GPL
 *
 * @category    Database
 * @package     mod_geoip
 * @author      JoungKyun.Kim
 * @copyright   2021. JoungKyun.Kim
 * @license     GPL
 * @since       File available since release 0.0.1
 */

/*
 * PHP5 GeoIP module "geoip"
 */

#define EXTENSION_VERSION "2.0.4"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "zend_exceptions.h"
#include "php_geoip.h"

#if PHP_API_VERSION < 20151012
#error "************ PHP version dependency problems *******************"
#error "This package requires over php 7.0.0 !!"
#error "If you build with php under 7.0.0, use mod_geoip 1.x version"
#error "You can download mod_krisp 1.x at http://mirror.oops.org/pub/oops/php/extensions/mod_geoip/"
#endif

ZEND_DECLARE_MODULE_GLOBALS(geoip)

static int le_geoip;

#include "php_geoip_arginfo.h"

/* {{{ INCLUDE Classify header */
#include "php_geoip_class.h"
/* }}} */

/* {{{ geoip_functions[] */
const zend_function_entry geoip_functions[] = {
	ZEND_FE(geoip_open,                 arginfo_geoip_open)
	ZEND_FE(geoip_close,                arginfo_geoip_close)
	ZEND_FE(geoip_database_info,        arginfo_geoip_database_info)
	ZEND_FE(geoip_db_avail,             arginfo_geoip_db_avail)
	ZEND_FE(geoip_country_code_by_name, arginfo_geoip_country_code_by_name)
	ZEND_FE(geoip_country_name_by_name, arginfo_geoip_country_name_by_name)
	ZEND_FE(geoip_id_by_name,           arginfo_gepip_id_by_name)
	ZEND_FE(geoip_record_by_name,       arginfo_geoip_record_by_name)
	ZEND_FE(geoip_org_by_name,          arginfo_geoip_org_by_name)
	ZEND_FE_END
};
/* }}} */

/* {{{ geoip_module_entry
 */
zend_module_entry geoip_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"geoip",
	geoip_functions,
	PHP_MINIT(geoip),
	PHP_MSHUTDOWN(geoip),
	PHP_RINIT(geoip),
	PHP_RSHUTDOWN(geoip),
	PHP_MINFO(geoip),
#if ZEND_MODULE_API_NO >= 20010901
	EXTENSION_VERSION, /* version number of the extension */
#endif
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_GEOIP
ZEND_GET_MODULE(geoip)
#endif

/* {{{ INCLUDE GEOIP Classify API */
#include "php_geoip_class.c"
/* }}} */

#ifdef GEOIPDATADIR
#define GEOIPDATABASE GEOIPDATADIR "/GeoIP.dat"
#else
#define GEOIPDATABASE "GeoIP.dat"
#endif

/* {{{ PHP_INI
 */
PHP_INI_BEGIN()
	STD_PHP_INI_ENTRY("geoip.database_standard", GEOIPDATABASE, PHP_INI_ALL, OnUpdateString, database_standard, zend_geoip_globals, geoip_globals)
PHP_INI_END()
/* }}} */

/* {{{ php_geoip_init_globals
 */
static void php_geoip_init_globals(zend_geoip_globals *geoip_globals)
{
	geoip_globals->database_standard = NULL;
}
/* }}} */

/* {{{ _close_geoip_link (zend_resource * res)
 */
static void _close_geoip_link (zend_resource * res)
{
	GE_PRINT_CALL_API_NAME;

	if ( ! res )
		return;

	if ( res->ptr ) {
		GeoIP_API *ge = (GeoIP_API *) res->ptr;
		ge_printf ("ge before free             : %ld\n", ge);
		ge_printf ("ge before free             : %ld\n", ge->gi);
		GeoIP_delete (ge->gi);
		ge->gi = NULL;
		ge_printf ("ge after free              : %ld\n", ge->gi);
		gefree (ge);
		ge_printf ("ge after free              : %ld\n", ge);
	}

}
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(geoip)
{
	GE_PRINT_CALL_API_NAME;

	ZEND_INIT_MODULE_GLOBALS(geoip, php_geoip_init_globals, NULL);
	REGISTER_INI_ENTRIES();

	_GeoIP_setup_dbfilename();

	le_geoip = zend_register_list_destructors_ex (_close_geoip_link, NULL, "GeoIP link", module_number);

	/* {{{ Class declear */
	REGISTER_GEOIP_CLASS(NULL);

#if defined(HAVE_SPL)
	REGISTER_GEOIP_PER_CLASS(Exception, exception, spl_ce_RuntimeException);
#else
	REGISTER_GEOIP_PER_CLASS(Exception, exception, zend_ce_exception);
#endif
	/* }}} */

	/* define geoip open method */
	REGISTER_LONG_CONSTANT ("GEOIP_STANDARD", GEOIP_STANDARD, CONST_PERSISTENT | CONST_CS);
	REGISTER_LONG_CONSTANT ("GEOIP_MEMORY_CACHE", GEOIP_MEMORY_CACHE, CONST_PERSISTENT | CONST_CS);
	REGISTER_LONG_CONSTANT ("GEOIP_CHECK_CACHE", GEOIP_CHECK_CACHE, CONST_PERSISTENT | CONST_CS);
	REGISTER_LONG_CONSTANT ("GEOIP_INDEX_CACHE", GEOIP_INDEX_CACHE, CONST_PERSISTENT | CONST_CS);

	/* define geoip database type */
	REGISTER_LONG_CONSTANT ("GEOIP_COUNTRY_EDITION", GEOIP_COUNTRY_EDITION, CONST_PERSISTENT | CONST_CS);
	REGISTER_LONG_CONSTANT ("GEOIP_REGION_EDITION_REV0", GEOIP_REGION_EDITION_REV0, CONST_PERSISTENT | CONST_CS);
	REGISTER_LONG_CONSTANT ("GEOIP_CITY_EDITION_REV0", GEOIP_CITY_EDITION_REV0, CONST_PERSISTENT | CONST_CS);
	REGISTER_LONG_CONSTANT ("GEOIP_ORG_EDITION", GEOIP_ORG_EDITION, CONST_PERSISTENT | CONST_CS);
	REGISTER_LONG_CONSTANT ("GEOIP_ISP_EDITION", GEOIP_ISP_EDITION, CONST_PERSISTENT | CONST_CS);
	REGISTER_LONG_CONSTANT ("GEOIP_CITY_EDITION_REV1", GEOIP_CITY_EDITION_REV1, CONST_PERSISTENT | CONST_CS);
	REGISTER_LONG_CONSTANT ("GEOIP_REGION_EDITION_REV1", GEOIP_REGION_EDITION_REV1, CONST_PERSISTENT | CONST_CS);
	REGISTER_LONG_CONSTANT ("GEOIP_PROXY_EDITION", GEOIP_PROXY_EDITION, CONST_PERSISTENT | CONST_CS);
	REGISTER_LONG_CONSTANT ("GEOIP_ASNUM_EDITION", GEOIP_ASNUM_EDITION, CONST_PERSISTENT | CONST_CS);
	REGISTER_LONG_CONSTANT ("GEOIP_NETSPEED_EDITION", GEOIP_NETSPEED_EDITION, CONST_PERSISTENT | CONST_CS);
	REGISTER_LONG_CONSTANT ("GEOIP_DOMAIN_EDITION", GEOIP_DOMAIN_EDITION, CONST_PERSISTENT | CONST_CS);

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(geoip)
{
#if LIBGEOIP_VERSION >= 1004007
	GeoIP_cleanup ();
#else
	if ( GeoIPDBFileName != NULL ) {
		int i;
		for ( i=0; i<NUM_DB_TYPES; i++ ) {
			gfree (GeoIPDBFileName[i]);
		}
		gfree(GeoIPDBFileName);
	}
#endif
	return SUCCESS;
}
/* }}} */


/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(geoip)
{
	return SUCCESS;
}
/* }}} */


/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(geoip)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(geoip)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "geoip support", "enabled");
	php_info_print_table_row(2, "geoip extension version", EXTENSION_VERSION);
	php_info_print_table_row(2, "geoip extension support", "COUNTRY,CITY,ISP,ORG");
	php_info_print_table_end();
	DISPLAY_INI_ENTRIES();
}
/* }}} */

/* {{{ proto geoip_open (string|int database = NULL|GEOIP_COUNTRY_EDITION, int flag = GEOIP_MEMORY_CACHE|GEOIP_CHECK_CACHE): resource
 */
PHP_FUNCTION(geoip_open)
{
	zval      * database;
	zend_long   flag;
	char      * dbname = NULL;
	int         dbtype = GEOIP_COUNTRY_EDITION;
	int         dbl = 0;
	struct      stat f;
	int         r;

	zval      * object = getThis ();
	zend_error_handling error_handling;

	GeoIP_API * ge;
	geoip_object * obj;

	GE_PRINT_CALL_API_NAME;

	if ( object )
		obj = Z_GEOIP_P (object);

	GEOIP_REPLACE_ERROR_HANDLING;
	switch (ZARGC) {
		case 2 :
			if ( zend_parse_parameters (ZARGC, "zl", &database, &flag) == FAILURE ) {
				GEOIP_RESTORE_ERROR_HANDLING;
				return;
			}

			if ( Z_TYPE_P (database) == IS_STRING ) {
				dbname = Z_STRVAL_P (database);
				if ( ! strlen (dbname) ) {
					php_error_docref (NULL, E_WARNING, "First argument is empty");
					GEOIP_RESTORE_ERROR_HANDLING;
					RETURN_FALSE;
				}

				r = stat (dbname, &f);

				if ( r == -1 || f.st_size < 1 ) {
					php_error_docref (NULL, E_WARNING, "%s is invalude path", dbname);
					GEOIP_RESTORE_ERROR_HANDLING;
					RETURN_FALSE;
				}

				if ( php_check_open_basedir (dbname) ) {
					GEOIP_RESTORE_ERROR_HANDLING;
					RETURN_FALSE;
				}

				dbl = f.st_size;
			} else if ( Z_TYPE_P (database) == IS_LONG ) {
				dbtype = Z_LVAL_P (database);
				dbl = -1;
			} else {
				php_error_docref (NULL, E_WARNING, "First argument is must string or numeric.");
				GEOIP_RESTORE_ERROR_HANDLING;
				RETURN_FALSE;
			}
			break;
		case 1 :
			if ( zend_parse_parameters (ZARGC, "l", &flag) == FAILURE ) {
				GEOIP_RESTORE_ERROR_HANDLING;
				return;
			}
			break;
		case 0 :
			flag = GEOIP_MEMORY_CACHE | GEOIP_CHECK_CACHE;
			break;
		defailt :
			GEOIP_RESTORE_ERROR_HANDLING;
			WRONG_PARAM_COUNT;
	}

	/*
	if ( ! dbl && GEOIP_G (database_standard) ) {
		dbname = GEOIP_G (database_standard);
		dbl = 1;
	}
	*/

	ge = (GeoIP_API *) gemalloc (sizeof (GeoIP_API));
	ge_printf ("ge                         : %ld\n", ge);

	ge->type = dbtype;

	if ( ! dbl ) {
		ge->gi = GeoIP_new (flag);
		ge_printf ("ge->gi                     : %ld\n", ge->gi);
	} else {
		if ( dbl != -1 )
			ge->gi = GeoIP_open (dbname, flag);
		else
			ge->gi = GeoIP_open_type (dbtype, flag);
		ge_printf ("ge->gi                     : %ld\n", ge->gi);
	}

	if ( ge->gi == NULL ) {
		GEOIP_RESTORE_ERROR_HANDLING;
		RETURN_FALSE;
	}

	if ( object )
		obj->db = ge;

	ge->rsrc = zend_register_resource (ge, le_geoip);
	if ( ! object )
		RETVAL_RES (ge->rsrc);

	GEOIP_RESTORE_ERROR_HANDLING;
}
/* }}} */

/* {{{ proto geoip_close (resource link): bool
 */
PHP_FUNCTION(geoip_close)
{
	zval         * ge_link = NULL;
	GeoIP_API    * ge;

	zval         * object = getThis ();
	geoip_object * obj;
	zend_error_handling error_handling;

	GE_PRINT_CALL_API_NAME;

	if ( object ) {
		obj = (geoip_object *) Z_GEOIP_P (object);
		ge = obj->db;

		ge_printf ("ge                         : %ld\n", ge);

		if ( ! ge || ge->gi != NULL )
			RETURN_TRUE;

		ge_printf ("ge->gi                     : %ld\n", ge->gi);

		zend_list_close (obj->db->rsrc);
	} else {
		if ( zend_parse_parameters (ZARGC, "r", &ge_link) == FAILURE )
			return;

		GEOIP_FETCH_RESOURCE (ge, GeoIP_API *, ge_link, "GeoIP link", le_geoip);
		ge_printf ("ge                         : %ld\n", ge);
		ge_printf ("ge->gi                     : %ld\n", ge->gi);
		zend_list_close (Z_RES_P (ge_link));
	}

	RETURN_TRUE;
}
/* }}} */

/* {{{ proto geoip_database_info (resource link): string
 */
PHP_FUNCTION(geoip_database_info)
{
	zval         * ge_link = NULL;
	GeoIP_API    * ge;
	char         * db_info;

	zval         * object = getThis ();
	geoip_object * obj;
	zend_error_handling error_handling;

	GE_PRINT_CALL_API_NAME;

	GEOIP_REPLACE_ERROR_HANDLING;
	if ( object ) {
		obj = (geoip_object *) Z_GEOIP_P (object);
		ge = obj->db;
		if ( ! ge || ge->gi == NULL ) {
			php_error_docref (NULL, E_WARNING, "No GeoIP object available");
			GEOIP_RESTORE_ERROR_HANDLING;
			RETURN_EMPTY_STRING ();
		}
	} else {
		if ( zend_parse_parameters (ZARGC, "r", &ge_link) == FAILURE ) {
			GEOIP_RESTORE_ERROR_HANDLING;
			return;
		}
		GEOIP_FETCH_RESOURCE (ge, GeoIP_API *, ge_link, "GeoIP link", le_geoip);
	}

	db_info = GeoIP_database_info (ge->gi);

	RETVAL_STRING (db_info);
	gfree (db_info);
	GEOIP_RESTORE_ERROR_HANDLING;
}
/* }}} */

/* {{{ proto geoip_db_avail (int type): int
 */
PHP_FUNCTION(geoip_db_avail)
{
	zend_long type = 0;

	GE_PRINT_CALL_API_NAME;

	if ( zend_parse_parameters (ZARGC, "l", &type) == FAILURE )
		return;

	RETVAL_LONG (GeoIP_db_avail (type));
}
/* }}} */

/* {{{ proto geoip_country_code_by_name (resource link, string hostname): string
 */
PHP_FUNCTION(geoip_country_code_by_name)
{
	zval         * ge_link = NULL;
	GeoIP_API    * ge;
	zend_string  * host = NULL;
	const char   * country_code;

	zval         * object = getThis ();
	geoip_object * obj;
	zend_error_handling error_handling;

	GE_PRINT_CALL_API_NAME;

	GEOIP_REPLACE_ERROR_HANDLING;
	if ( object ) {
		if ( zend_parse_parameters (ZARGC, "S", &host) == FAILURE ) {
			GEOIP_RESTORE_ERROR_HANDLING;
			return;
		}
	} else {
		if ( zend_parse_parameters (ZARGC, "rS", &ge_link, &host) == FAILURE ) {
			GEOIP_RESTORE_ERROR_HANDLING;
			return;
		}
	}

	if ( ZSTR_LEN (host) == 0 ) {
		php_error_docref (NULL, E_WARNING, "geoip_country_name_by_name: 2th argument is empty");
		GEOIP_RESTORE_ERROR_HANDLING;
		RETURN_EMPTY_STRING ();
	}

	if ( object ) {
		obj = (geoip_object *) Z_GEOIP_P (object);
		ge = obj->db;
		if ( ! ge || ge->gi == NULL ) {
			php_error_docref (NULL, E_WARNING, "No GeoIP object available");
			GEOIP_RESTORE_ERROR_HANDLING;
			RETURN_EMPTY_STRING ();
		}
	} else
		GEOIP_FETCH_RESOURCE (ge, GeoIP_API *, ge_link, "GeoIP link", le_geoip);

	country_code = GeoIP_country_code_by_name (ge->gi, ZSTR_VAL (host));

	if ( country_code == NULL )
		RETURN_EMPTY_STRING ();

	GEOIP_RESTORE_ERROR_HANDLING;

	RETURN_STRING ((char*) country_code);
}
/* }}} */

/* {{{ proto geoip_country_name_by_name (resource link, string hostname): string
 */
PHP_FUNCTION(geoip_country_name_by_name)
{
	zval         * ge_link = NULL;
	GeoIP_API    * ge;
	zend_string  * host = NULL;
	const char   * country_name;

	zval         * object = getThis ();
	geoip_object * obj;
	zend_error_handling error_handling;

	GE_PRINT_CALL_API_NAME;

	GEOIP_REPLACE_ERROR_HANDLING;
	if ( object ) {
		if ( zend_parse_parameters (ZARGC, "S", &host) == FAILURE ) {
			GEOIP_RESTORE_ERROR_HANDLING;
			return;
		}
	} else {
		if ( zend_parse_parameters (ZARGC, "rS", &ge_link, &host) == FAILURE ) {
			GEOIP_RESTORE_ERROR_HANDLING;
			return;
		}
	}

	if ( ZSTR_LEN (host) == 0 ) {
		php_error_docref (NULL, E_WARNING, "geoip_country_name_by_name: 2th argument is empty");
		RETURN_EMPTY_STRING ();
	}

	if ( object ) {
		obj = (geoip_object *) Z_GEOIP_P (object);
		ge = obj->db;
		if ( ! ge || ge->gi == NULL ) {
			php_error_docref (NULL, E_WARNING, "No GeoIP object available");
			GEOIP_RESTORE_ERROR_HANDLING;
			RETURN_EMPTY_STRING ();
		}
	} else
		GEOIP_FETCH_RESOURCE (ge, GeoIP_API *, ge_link, "GeoIP link", le_geoip);

	country_name = GeoIP_country_name_by_name (ge->gi, ZSTR_VAL (host));

	if ( country_name == NULL ) {
		GEOIP_RESTORE_ERROR_HANDLING;
		RETURN_EMPTY_STRING ();
	}

	GEOIP_RESTORE_ERROR_HANDLING;
	RETURN_STRING ((char*) country_name);
}
/* }}} */

/* {{{ proto gepip_id_by_name (resource link, string hostname): array|false
 */
PHP_FUNCTION(geoip_id_by_name)
{
	zval         * ge_link = NULL;
	GeoIP_API    * ge;
	zend_string  * host = NULL;
	int            country_id;

	zval         * object = getThis ();
	geoip_object * obj;
	zend_error_handling error_handling;

	GE_PRINT_CALL_API_NAME;

	GEOIP_REPLACE_ERROR_HANDLING;
	if ( object ) {
		if ( zend_parse_parameters (ZARGC, "S", &host) == FAILURE ) {
			GEOIP_RESTORE_ERROR_HANDLING;
			return;
		}
	} else {
		if ( zend_parse_parameters (ZARGC, "rS", &ge_link, &host) == FAILURE ) {
			GEOIP_RESTORE_ERROR_HANDLING;
			return;
		}
	}

	if ( ZSTR_LEN (host) == 0 ) {
		php_error_docref (NULL, E_WARNING, "geoip_id_by_name: 2th argument is empty");
		GEOIP_RESTORE_ERROR_HANDLING;
		RETURN_FALSE;
	}

	if ( object ) {
		obj = (geoip_object *) Z_GEOIP_P (object);
		ge = obj->db;
		if ( ! ge || ge->gi == NULL ) {
			php_error_docref (NULL, E_WARNING, "No GeoIP object available");
			GEOIP_RESTORE_ERROR_HANDLING;
			RETURN_FALSE;
		}
	} else
		GEOIP_FETCH_RESOURCE (ge, GeoIP_API *, ge_link, "GeoIP link", le_geoip);


	country_id = GeoIP_id_by_name (ge->gi, ZSTR_VAL (host));

#if PHP_VERSION_ID < 70300
	if ( array_init (return_value) == FAILURE ) {
		php_error_docref (NULL, E_WARNING, "geoip_id_by_name: Failure array init");
		GEOIP_RESTORE_ERROR_HANDLING;
		RETURN_FALSE;
	}
#else
	array_init (return_value);
#endif

	add_assoc_long (return_value, "country_id", country_id);
	add_assoc_string (return_value, "code",
			GeoIP_country_code[country_id] ? (char *) GeoIP_country_code[country_id] : "--");
	add_assoc_string (return_value, "name",
			GeoIP_country_name[country_id] ? (char *) GeoIP_country_name[country_id] : "N/A");

	GEOIP_RESTORE_ERROR_HANDLING;
}
/* }}} */

/* {{{ proto geoip_record_by_name (resource link, string hostname): array|false
 */
PHP_FUNCTION(geoip_record_by_name)
{
	zval          * ge_link = NULL;
	GeoIP_API     * ge;
	GeoIPRecord   * gir;
	zend_string   * host = NULL;
	const char    * country_name;

	zval         * object = getThis ();
	geoip_object * obj;
	zend_error_handling error_handling;

	GE_PRINT_CALL_API_NAME;

	GEOIP_REPLACE_ERROR_HANDLING;
	if ( object ) {
		if ( zend_parse_parameters (ZARGC, "S", &host) == FAILURE ) {
			GEOIP_RESTORE_ERROR_HANDLING;
			return;
		}
	} else {
		if ( zend_parse_parameters (ZARGC, "rS", &ge_link, &host) == FAILURE ) {
			GEOIP_RESTORE_ERROR_HANDLING;
			return;
		}
	}

	if ( ZSTR_LEN (host) == 0 ) {
		php_error_docref (NULL, E_WARNING, "geoip_record_by_name: 2th argument is empty");
		GEOIP_RESTORE_ERROR_HANDLING;
		RETURN_FALSE;
	}

	if ( object ) {
		obj = (geoip_object *) Z_GEOIP_P (object);
		ge = obj->db;
		if ( ! ge || ge->gi == NULL ) {
			php_error_docref (NULL, E_WARNING, "No GeoIP object available");
			GEOIP_RESTORE_ERROR_HANDLING;
			RETURN_FALSE;
		}
	} else
		GEOIP_FETCH_RESOURCE (ge, GeoIP_API *, ge_link, "GeoIP link", le_geoip);


	gir = GeoIP_record_by_name (ge->gi, ZSTR_VAL (host));

	if ( gir == NULL ) {
		GEOIP_RESTORE_ERROR_HANDLING;
		RETURN_FALSE;
	}

#if PHP_VERSION_ID < 70300
	if ( array_init (return_value) == FAILURE ) {
		php_error_docref (NULL, E_WARNING, "geoip_record_by_name: Failure array init");
		GEOIP_RESTORE_ERROR_HANDLING;
		RETURN_FALSE;
	}
#else
	array_init (return_value);
#endif

	add_assoc_string (return_value, "country_code", gir->country_code ? gir->country_code : "");
	add_assoc_string (return_value, "region", gir->region ? gir->region : "");
	add_assoc_string (return_value, "city", gir->city ? gir->city : "");
	add_assoc_string (return_value, "postal_code", gir->postal_code ? gir->postal_code : "");
	add_assoc_double (return_value, "latitude", gir->latitude ? gir->latitude : (double) 0);
	add_assoc_double (return_value, "longitude", gir->longitude ? gir->longitude : (double) 0);
	if ( ge->type == GEOIP_CITY_EDITION_REV1 ) {
		add_assoc_long (return_value, "dma_code", gir->dma_code ? gir->dma_code : (int) 0);
		add_assoc_long (return_value, "area_code", gir->area_code ? gir->area_code : (int) 0);
	}

	GeoIPRecord_delete (gir);
	GEOIP_RESTORE_ERROR_HANDLING;
}
/* }}} */

/* {{{ proto geoip_org_by_name (resource link, string hostname): string
 */
PHP_FUNCTION(geoip_org_by_name)
{
	zval         * ge_link = NULL;
	GeoIP_API    * ge;
	zend_string  * host = NULL;
	char         * name;

	zval         * object = getThis ();
	geoip_object * obj;
	zend_error_handling error_handling;

	GE_PRINT_CALL_API_NAME;

	GEOIP_REPLACE_ERROR_HANDLING;
	if ( object ) {
		if ( zend_parse_parameters (ZARGC, "S", &host) == FAILURE ) {
			GEOIP_RESTORE_ERROR_HANDLING;
			return;
		}
	} else {
		if ( zend_parse_parameters (ZARGC, "rS", &ge_link, &host) == FAILURE ) {
			GEOIP_RESTORE_ERROR_HANDLING;
			return;
		}
	}

	if ( ZSTR_LEN (host) == 0 ) {
		php_error_docref (NULL, E_WARNING, "geoip_org_by_name: 2th argument is empty");
		GEOIP_RESTORE_ERROR_HANDLING;
		RETURN_EMPTY_STRING ();
	}

	if ( object ) {
		obj = (geoip_object *) Z_GEOIP_P (object);
		ge = obj->db;
		if ( ! ge || ge->gi == NULL ) {
			php_error_docref (NULL, E_WARNING, "No GeoIP object available");
			GEOIP_RESTORE_ERROR_HANDLING;
			RETURN_FALSE;
		}
	} else
		GEOIP_FETCH_RESOURCE (ge, GeoIP_API *, ge_link, "GeoIP link", le_geoip);

	name = GeoIP_org_by_name (ge->gi, ZSTR_VAL (host));

	if ( name == NULL ) {
		GEOIP_RESTORE_ERROR_HANDLING;
		RETURN_EMPTY_STRING ();
	}

	RETVAL_STRING ((char*) name);
	gfree (name);
	GEOIP_RESTORE_ERROR_HANDLING;
}
/* }}} */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
