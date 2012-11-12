#!/usr/bin/php
<?
#dl ("../../../../usr/local/src/mycvs/php/mod_geoip/modules/geoip.so");
$searches = array ('oops.org', 'kornet.net', 'yahoo.com');

/*
 * resource GeoIP_open (database, flag)
 * resource GeoIP_open (flag)
 * resource GeoIP_open ()
 * 
 * open the geoip database
 *
 * argument info:
 *      database => GeoIP datafile path
 *      flag     => GEOIP_STANDARD
 *                  GEOIP_MEMORY_CACHE
 *                  GEOIP_CHECK_CACHE
 *                  GEOIP_INDEX_CACHE
 *
 * if flag is not specifyed, default GEOIP_MEMORY_CACHE
 *
 * if failed, return FALSE
 *
 */
$c = GeoIP_open ();

#echo "TYPE: " . geoip_database_info ($c) ."\n";

/*
 * search geoip database
 *
 * array geoip_id_by_name (geoip handle, host);
 *  return:
 *          array (
 *                  country_id,
 *                  code,        // code of nation
 *                  name         // name of nation
 *                );
 *
 * string geoip_country_code_by_name (geoip handle, host)
 * string geoip_country_code_by_name (geoip handle, host)
 *
 */
foreach ( $searches as $v ) :
	$r = geoip_id_by_name ($c, $v);
	print_r ($r);

	#echo "### " . geoip_country_code_by_name ($c, $v) . "\n";
	#echo "### " . geoip_country_name_by_name ($c, $v) . "\n";
endforeach;


/*
 * close geoip database
 */
GeoIP_close ($c);
?>
