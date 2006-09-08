#!/usr/bin/php
<?

if ( ! extension_loaded ('geoip') )
	dl ("geoip.so");

$searches = array ('oops.org', 'kornet.net', 'yahoo.com');

/*
 * resource GeoIP_open (database, flag)
 * resource GeoIP_open (database_code, flag)
 * resource GeoIP_open (flag)
 * resource GeoIP_open ()
 * 
 * open the geoip database
 *
 * argument info:
 *      database => GeoIP datafile path or database code
 *           database code :
 *                  GEOIP_COUNTRY_EDITION    -> default
 *                  GEOIP_ISP_EDITION        -> isp database (if you have)
 *                  GEOIP_CITY_EDITION_REV0  -> city database (if you have)
 *                  GEOIP_CITY_EDITION_REV1  -> city database (if you have)
 *                  GEOIP_ORG_EDITION        -> org database (if you have)
 *
 *      flag     => GEOIP_STANDARD
 *                  GEOIP_MEMORY_CACHE
 *                  GEOIP_CHECK_CACHE
 *                  GEOIP_INDEX_CACHE
 *
 * if flag is not specifyed, default "GEOIP_MEMORY_CACHE | GEOIP_CHECK_CACHE"
 *
 * if failed, return FALSE
 *
 */

/* open conutry database */
$g = GeoIP_open (GEOIP_MEMORY_CACHE|GEOIP_CHECK_CACHE);
/* open city database */
if ( GeoIP_db_avail (GEOIP_CITY_EDITION_REV0) )
	$gc = GeoIP_open (GEOIP_CITY_EDITION_REV0, GEOIP_INDEX_CACHE|GEOIP_CHECK_CACHE);
/* open isp database */
if ( GeoIP_db_avail (GEOIP_ISP_EDITION) )
	$gi = GeoIP_open (GEOIP_ISP_EDITION, GEOIP_INDEX_CACHE|GEOIP_CHECK_CACHE);

#echo "TYPE: " . geoip_database_info ($g) ."\n";

foreach ( $searches as $v ) :
	/*
	 * search geoip country database
	 *
	 * array geoip_id_by_name (geoip handle, host);
	 * return:
	 *      array (
	 *              country_id,
	 *              code,        // code of nation
	 *              name         // name of nation
	 *            );
	 *
	 * string geoip_country_code_by_name (geoip handle, host)
	 * string geoip_country_code_by_name (geoip handle, host)
	 *
	 */
	$r = geoip_id_by_name ($g, $v);
	print_r ($r);

	/* print city information
	 *
	 * array geoip_record_by_name (geoip handle, host)
	 * return:
	 *      array (
	 *              country_code,  // country 2 digit code
	 *              region,        // states or do (korean)
	 *              city,          // name of city
	 *              postal_code,   // postal code or post code
	 *              latitude,
	 *              longitude,
	 *              if ( DB is GEOIP_CITY_EDITION_REV1 ) {
	 *                  dma_code,
	 *                  area_code,
	 *              }
	 *            );
	 */
	if ( is_resource ($gc) ) {
		$rc = GeoIP_record_by_name ($gc, $v);
		print_r ($rc);
	}
	
	/* print isp information
	 *
	 * string geoip_org_by_name (geoip handle, host)
	 */
	if ( is_resource ($gi) ) {
		$ri = GeoIP_org_by_name ($gi, $v);
		echo "    $ri\n";
	}
	
	#echo "### " . geoip_country_code_by_name ($g, $v) . "\n";
	#echo "### " . geoip_country_name_by_name ($g, $v) . "\n";
endforeach;


/*
 * close geoip database
 */
GeoIP_close ($g);
if ( is_resource ($gc) ) GeoIP_close ($gc);
if ( is_resource ($gi) ) GeoIP_close ($gi);
?>
