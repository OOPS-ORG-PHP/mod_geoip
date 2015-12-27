/**
 * Project: mod_geoip :: GeoIP php exntension
 * File:    php_geoip.c
 *
 * Copyright (c) 2012 JoungKyun.Kim
 *
 * LICENSE: GPL
 *
 * @category    Database
 * @package     mod_geoip
 * @author      JoungKyun.Kim <http://oops.org>
 * @copyright   2012 OOPS.org
 * @license     GPL
 * @version     SVN: $Id: php_geoip.c 26 2015-12-27 18:31:40Z oops $
 * @since       File available since release 0.0.1
 */

/*
 * PHP5 GeoIP module "geoip"
 */

#define EXTENSION_VERSION "2.0.0"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_geoip.h"

#if PHP_API_VERSION < 20151012
#error "************ PHP version dependency problems *******************"
#error "This package requires over php 7.0.0 !!"
#error "If you build with php under 7.0.0, use mod_geoip 1.x version"
#error "You can download mod_krisp 1.x at http://mirror.oops.org/pub/oops/php/extensions/mod_geoip/"
#endif

ZEND_DECLARE_MODULE_GLOBALS(geoip)

static int le_geoip;

/* {{{ INCLUDE Classify header */
#include "php_geoip_class.h"
/* }}} */

/* {{{ geoip_functions[] */
const zend_function_entry geoip_functions[] = {
	PHP_FE(geoip_open,                 NULL)   
	PHP_FE(geoip_close,                NULL)   
	PHP_FE(geoip_database_info,        NULL)   
	PHP_FE(geoip_db_avail,             NULL)   
	PHP_FE(geoip_country_code_by_name, NULL)
	PHP_FE(geoip_country_name_by_name, NULL)
	PHP_FE(geoip_id_by_name,           NULL)
	PHP_FE(geoip_record_by_name,       NULL)
	PHP_FE(geoip_org_by_name,          NULL)
	{NULL, NULL, NULL}   
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

/* {{{ _close_geoip_link (zend_resource *rsrc TSRMLS_DC)
 */
static void _close_geoip_link (zend_resource *rsrc TSRMLS_DC)
{
	GeoIP_API *ge = (GeoIP_API *) rsrc->ptr;
	int i;
	GeoIP_delete (ge->gi);
	efree (ge);
	ge = NULL;

	/*
	if ( GeoIPDBFileName != NULL ) {
		for ( i = 1; i <= 11 ; i++ )
			free (GeoIPDBFileName[i]);
		free (GeoIPDBFileName);
	}
	*/
}
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(geoip)
{
	ZEND_INIT_MODULE_GLOBALS(geoip, php_geoip_init_globals, NULL);
	REGISTER_INI_ENTRIES();

	_GeoIP_setup_dbfilename();

	le_geoip = zend_register_list_destructors_ex (NULL, _close_geoip_link, "GeoIP link", module_number);

	/* {{{ Class declear */
	REGISTER_GEOIP_CLASS(NULL);

#if defined(HAVE_SPL)
	REGISTER_GEOIP_PER_CLASS(Exception, exception, spl_ce_RuntimeException);
#else
	REGISTER_GEOIP_PER_CLASS(Exception, exception, zend_exception_get_default(TSRMLS_C));
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

/* {{{ proto (resource) geoip_open ([string database|int database_type[, int flag]])
 */
PHP_FUNCTION(geoip_open)
{
	zval      * database;
	zend_long   flag;
	char      * dbname = NULL;
	int         dbtype = 0;
	int         dbl = 0;
	struct      stat f;
	int         r;

	zval      * object = getThis ();
	zend_error_handling error_handling;

	GeoIP_API * ge;
	geoip_object * obj;

	if ( object ) {
		obj = Z_GEOIP_P (object);
		obj->u.ptr = NULL;
	}

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
					php_error (E_WARNING, "First argument is empty");
					GEOIP_RESTORE_ERROR_HANDLING;
					RETURN_FALSE;
				}

				r = stat (dbname, &f);

				if ( r == -1 || f.st_size < 1 ) {
					php_error (E_WARNING, "%s is invalid path", dbname);
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
				php_error (E_WARNING, "First argument is must string or numeric.");
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

	ge = (GeoIP_API *) emalloc (sizeof (GeoIP_API));

	ge->type = dbtype;

	if ( ! dbl ) {
		ge->gi = GeoIP_new (flag);
	} else {
		if ( dbl != -1 )
			ge->gi = GeoIP_open (dbname, flag);
		else
			ge->gi = GeoIP_open_type (dbtype, flag);
	}

	if ( ge->gi == NULL ) {
		GEOIP_RESTORE_ERROR_HANDLING;
		RETURN_FALSE;
	}

	if ( object )
		obj->u.db = ge;

	ge->rsrc = zend_register_resource (ge, le_geoip);
	RETVAL_RES (ge->rsrc);

	GEOIP_RESTORE_ERROR_HANDLING;
}
/* }}} */

/* {{{ proto (bool) geoip_close (resource link)
 */
PHP_FUNCTION(geoip_close)
{
	zval         * ge_link = NULL;
	GeoIP_API    * ge;

	zval         * object = getThis ();
	geoip_object * obj;
	zend_error_handling error_handling;

	if ( object ) {
		obj = (geoip_object *) Z_GEOIP_P (object);
		ge = obj->u.db;
		if ( ! ge || ge->gi != NULL )
			RETURN_TRUE;
		zend_list_delete (obj->u.db->rsrc);
	} else {
		if ( zend_parse_parameters (ZARGC, "r", &ge_link) == FAILURE )
			return;

		GEOIP_FETCH_RESOURCE (ge, GeoIP_API *, ge_link, "GeoIP link", le_geoip);
		zend_list_delete (Z_RES_P (ge_link));
	}

	RETURN_TRUE;
}
/* }}} */

/* {{{ proto (string) geoip_database_info (resource link)
 */
PHP_FUNCTION(geoip_database_info)
{
	zval         * ge_link = NULL;
	GeoIP_API    * ge;
	char         * db_info;

	zval         * object = getThis ();
	geoip_object * obj;
	zend_error_handling error_handling;

	GEOIP_REPLACE_ERROR_HANDLING;
	if ( object ) {
		obj = (geoip_object *) Z_GEOIP_P (object);
		ge = obj->u.db;
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
	free (db_info);
	GEOIP_RESTORE_ERROR_HANDLING;
}
/* }}} */

/* {{{ proto (long) geoip_db_avail (int type)
 */
PHP_FUNCTION(geoip_db_avail)
{
	zend_long type = 0;

	if ( zend_parse_parameters (ZARGC, "l", &type) == FAILURE )
		return;

	RETVAL_LONG (GeoIP_db_avail (type));
}
/* }}} */

/* {{{ proto (string) geoip_country_code_by_name (resource link, string hostname)
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
		php_error (E_WARNING, "geoip_country_name_by_name: 2th argument is empty");
		GEOIP_RESTORE_ERROR_HANDLING;
		RETURN_EMPTY_STRING ();
	}

	if ( object ) {
		obj = (geoip_object *) Z_GEOIP_P (object);
		ge = obj->u.db;
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

/* {{{ proto (string) geoip_country_name_by_name (resource link, string hostname)
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
		php_error (E_WARNING, "geoip_country_name_by_name: 2th argument is empty");
		RETURN_EMPTY_STRING ();
	}

	if ( object ) {
		obj = (geoip_object *) Z_GEOIP_P (object);
		ge = obj->u.db;
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

/* {{{ proto (array|false) gepip_id_by_name (resource link, string hostname)
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
		php_error (E_WARNING, "geoip_id_by_name: 2th argument is empty");
		GEOIP_RESTORE_ERROR_HANDLING;
		RETURN_FALSE;
	}

	if ( object ) {
		obj = (geoip_object *) Z_GEOIP_P (object);
		ge = obj->u.db;
		if ( ! ge || ge->gi == NULL ) {
			php_error_docref (NULL, E_WARNING, "No GeoIP object available");
			GEOIP_RESTORE_ERROR_HANDLING;
			RETURN_FALSE;
		}
	} else
		GEOIP_FETCH_RESOURCE (ge, GeoIP_API *, ge_link, "GeoIP link", le_geoip);


	country_id = GeoIP_id_by_name (ge->gi, ZSTR_VAL (host));

	if ( array_init (return_value) == FAILURE ) {
		php_error (E_WARNING, "geoip_id_by_name: Failure array init");
		GEOIP_RESTORE_ERROR_HANDLING;
		RETURN_FALSE;
	}

	add_assoc_long (return_value, "country_id", country_id);
	add_assoc_string (return_value, "code",
			GeoIP_country_code[country_id] ? (char *) GeoIP_country_code[country_id] : "--");
	add_assoc_string (return_value, "name",
			GeoIP_country_name[country_id] ? (char *) GeoIP_country_name[country_id] : "N/A");

	GEOIP_RESTORE_ERROR_HANDLING;
}
/* }}} */

/* {{{ proto (array|false) geoip_record_by_name (resource link, string hostname))
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
		php_error (E_WARNING, "geoip_record_by_name: 2th argument is empty");
		GEOIP_RESTORE_ERROR_HANDLING;
		RETURN_FALSE;
	}

	if ( object ) {
		obj = (geoip_object *) Z_GEOIP_P (object);
		ge = obj->u.db;
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

	if ( array_init (return_value) == FAILURE ) {
		php_error (E_WARNING, "geoip_record_by_name: Failure array init");
		GEOIP_RESTORE_ERROR_HANDLING;
		RETURN_FALSE;
	}

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

/* {{{ proto (string) geoip_org_by_name (resource link, string hostname)
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
		php_error (E_WARNING, "geoip_org_by_name: 2th argument is empty");
		GEOIP_RESTORE_ERROR_HANDLING;
		RETURN_EMPTY_STRING ();
	}

	if ( object ) {
		obj = (geoip_object *) Z_GEOIP_P (object);
		ge = obj->u.db;
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
	free ((char *) name);
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
