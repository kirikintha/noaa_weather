<?php
/**
 * @file noaa.inc
 * NOAA Weather Module Functions
 * Handles all the settings for the NOAA Weather Module
 * Version: 1.0
 */

/**
 * @name _noaa_weather_get_radar_overlays()
 * Get Rada Overlays from radar.weather.gov
 */
function _noaa_weather_get_radar_overlays($radar_station) {
  $radar_station = strtoupper($radar_station);
  //This is an array of radar overlays to be created
  $radar_overlays_array = array(
    'Topo_Short.jpg'    =>'http://radar.weather.gov/ridge/Overlays/Topo/Short/radarstation_Topo_Short.jpg',
    'County_Short.gif'  =>'http://radar.weather.gov/ridge/Overlays/County/Short/radarstation_County_Short.gif',
    'Rivers_Short.gif'  =>'http://radar.weather.gov/ridge/Overlays/Rivers/Short/radarstation_Rivers_Short.gif',
    'Highways_Short.gif'=>'http://radar.weather.gov/ridge/Overlays/Highways/Short/radarstation_Highways_Short.gif',
    'City_Short.gif'    =>'http://radar.weather.gov/ridge/Overlays/Cities/Short/radarstation_City_Short.gif',
    'Warnings_0.gif'    =>'http://radar.weather.gov/ridge/Warnings/Short/radarstation_Warnings_0.gif',
    'N0R_Legend_0.gif'  =>'http://radar.weather.gov/ridge/Legend/N0R/radarstation_N0R_Legend_0.gif',
  );
  //Make a directory name
  $noaa_weather_radar_directory = file_directory_path().'/noaa_weather_radar/'.$radar_station;
  file_check_directory( $noaa_weather_radar_directory, FILE_CREATE_DIRECTORY | FILE_MODIFY_PERMISSIONS );
  //Save the radar arrays as images
  foreach ($radar_overlays_array as $key => $value) {
    //Get Remote Image
    $remote   = str_replace('radarstation', $radar_station, $value);
    $ch       = curl_init();
    curl_setopt( $ch, CURLOPT_RETURNTRANSFER, 1 ); //Set curl to return the data instead of printing it to the browser.
    curl_setopt( $ch, CURLOPT_URL, $remote ); //Set the URL for the image
    $data     = curl_exec( $ch );
    curl_close( $ch );
    //Save It to the local server
    $local    = $noaa_weather_radar_directory .'/' .$radar_station .'_' .$key;
    $filepath = file_save_data( $data, $local, FILE_EXISTS_REPLACE ); //Replace the file
    usleep(1000); //Just in case this is timing out
  }
  _noaa_weather_get_radar($radar_station);
  return TRUE;
}

/**
 * @name _noaa_weather_get_radar()
 * Get The Current radar for a radar station
 */
function _noaa_weather_get_radar($radar_station) {
  $radar_station      = strtoupper($radar_station);
  $destination_path   = file_directory_path().'/noaa_weather_radar/'.$radar_station;
  //Current radar image.
  $remote_radar_url   = 'http://radar.weather.gov/ridge/RadarImg/N0R/'.$radar_station.'_N0R_0.gif';
  $destination_radar  = $destination_path.'/current_radar.gif';
  $ch       = curl_init();
  curl_setopt( $ch, CURLOPT_RETURNTRANSFER, 1 ); //Set curl to return the data instead of printing it to the browser.
  curl_setopt( $ch, CURLOPT_URL, $remote_radar_url ); //Set the URL for the image
  $data     = curl_exec( $ch );
  curl_close( $ch );
  //Save It to the local server
  $filepath = file_save_data( $data, $destination_radar, FILE_EXISTS_REPLACE ); //Replace the file
  //Current radar legend.
  $remote_legend_url  = 'http://radar.weather.gov/ridge/Legend/N0R/'.$radar_station.'_N0R_Legend_0.gif';
  $destination_legend = $destination_path.'/current_legend.gif';
  $ch       = curl_init();
  curl_setopt( $ch, CURLOPT_RETURNTRANSFER, 1 ); //Set curl to return the data instead of printing it to the browser.
  curl_setopt( $ch, CURLOPT_URL, $remote_legend_url ); //Set the URL for the image
  $data     = curl_exec( $ch );
  curl_close( $ch );
  //Save It to the local server
  $filepath = file_save_data( $data, $destination_legend, FILE_EXISTS_REPLACE ); //Replace the file
  return true;
}

/**
 * Request NOAA Current Conditions Feed, Parse, Return array containing specific weather data
 *
 * This function takes the value of noaaweather_current_station var and loads the NOAA Current Conditions
 * for that station into a SimpleXML object.
 * 
 * @return array containing current conditions data as specified by $current_conditions_namespaces
 */

function _noaa_weather_get_current_conditions($current_station) {
  //Load current conditions via xml
  $current_condtions = array();  
  $xml               = simplexml_load_file('http://www.weather.gov/xml/current_obs/'. $current_station .'.xml');
  //If we have a valid object, get current conditions
  if (!is_object($xml)) {
    watchdog('noaa_weather', t('Custom Weather Current Weather could not load!'), WATCHDOG_ERROR, NULL);
  } else {
    $current_conditions_namespaces = array(
      'location',
      'station_id',
      'latitude',
      'longitude',
      'observation_time',
      'observation_time_rfc822',
      'weather',
      'temperature_string',
      'temp_f',
      'temp_c',
      'relative_humidity',
      'wind_string',
      'wind_dir',
      'wind_degrees',
      'wind_mph',
      'wind_kt',
      'pressure_string',
      'pressure_mb',
      'pressure_in',
      'dewpoint_string',
      'dewpoint_f',
      'dewpoint_c',
      'windchill_string',
      'windchill_f',
      'windchill_c',
      'visibility_mi',
      'icon_url_base',
      'icon_url_name'
    );
    //loop all children of object root node
    foreach ($xml->children() as $child) {
      //extract namespace of child node
      $namespace = $child->getName();
      //look for match in $current_conditions_namespaces array. if found then add to $current_conditions array
      if(in_array($namespace,$current_conditions_namespaces)) {
        $current_condtions[$namespace] = trim($child[0]);
      }
    }
    //Log to watchdog.
    watchdog('noaa_weather', t('Custom Weather Current Weather loaded successfully!'), WATCHDOG_NOTICE, NULL);
    //Set the Current Conditions variable
    variable_set('noaa_weather_current_conditions',$current_condtions);
  }
}

function _noaa_weather_get_forecast($lat, $lon, $days) {
  //Set uri and send request to NOAA.
  $forecast = "http://www.weather.gov/forecasts/xml/SOAP_server/ndfdSOAPclientByDay.php?lat=$lat&lon=$lon&format=12+hourly&startDate=&numDays=$days&Submit=Submit";
  $xml      = simplexml_load_file($forecast);
  $extended = array();
  //If we have a valid object, run our forecast
  if (!is_object($xml)) {
    watchdog('noaa_weather', t('NOAA Weather Extended Forecast failed to load!'), WATCHDOG_ERROR, NULL); 
  } else {
  	//Set parameter and timelayout parameters from simpleXML generated from raw NOAA data
  	$data = $xml->data;
  	$parameters = $xml->data->parameters;
   	$timelayout0 = $xml->data->{'time-layout'}[0];
   	$timelayout1 = $xml->data->{'time-layout'}[1];
   	$timelayout2 = $xml->data->{'time-layout'}[2];
    //Days.
		$dayi=0;
    foreach ($timelayout2->{'start-valid-time'} as $daytime) {
    	$attribs       = $daytime->attributes();
    	$period_name   = $attribs['period-name'];
    	$daytime_array = explode('T', $daytime);
    	$day_array     = explode('-', $daytime_array[0]);
    	$time_array    = explode('-', $daytime_array[1]);
    	$day_ts        = mktime(0, 0, 0, (int)$day_array[1], (int)$day_array[2], (int)$day_array[0]);
    	if ($dayi % 2) {
	    	$curdayi = $dayi-1;
	    	$extended[$curdayi/2]['period_name2']    = t(trim($period_name));
	    	$extended[$curdayi/2]['dayts2']          = t(trim($day_ts)); 
	      $extended[$curdayi/2]['daystr2']         = t(trim($daytime));
	      $extended[$curdayi/2]['daystrformated2'] = t(date('l',$day_ts));
    	} else {
	    	$extended[$dayi/2]['period_name1']    = t(trim($period_name));
	    	$extended[$dayi/2]['dayts1']          = t(trim($day_ts)); 
	      $extended[$dayi/2]['daystr1']         = t(trim($daytime));
	      $extended[$dayi/2]['daystrformated1'] = t(date('l',$day_ts)); 
    	}
      $dayi++;
    }
    //Max daily temp.
    $max_temp_array = $parameters->temperature[0]->value;
    $maxi           = 0;
    foreach ($max_temp_array as $max_temp) {  
      $extended[$maxi][ 'temperature_max' ] = trim($max_temp);  
      $maxi++;
    }
    //Min daily temp.
    $min_temp_array = $parameters->temperature[1]->value;
    $mini           =0 ;
    foreach ($min_temp_array as $min_temp) {  
      $extended[$mini][ 'temperature_min' ] = trim($min_temp);  
      $mini++;
    }
    //Probability of percipitation.s
    $popi = 0;
    foreach ($parameters->{'probability-of-precipitation'}->value as $pop) {
      $popval = (array)$pop;
      if($popi%2){  
        $popi2 = ($popi-1)/2; // set index for correct day
        $extended[$popi2]['probability_of_precipitation2'] = t($popval[0]);
      }else{
        $popi1 = $popi/2; // set index for correct day
        $extended[$popi1]['probability_of_precipitation1'] = t($popval[0]);
      }
      $popi++;
    }
    //loop weather-condition obj assigning 0 and 12 hour values for each day index
    $wsi = 0;//set weather summary index 
    foreach ($parameters->weather->{'weather-conditions'} as $weather_summary) {
      $wsvalarray = (array)$weather_summary;
      $wsval      = $wsvalarray['@attributes']['weather-summary'];
      if($wsi%2){  
        $wsi2 = ($wsi-1)/2; // set index for correct day
        $extended[$wsi2]['weather_summary2'] = t($wsval);
      }else{
        $wsi1 = $wsi/2; // set index for correct day
        $extended[$wsi1]['weather_summary1'] = t($wsval);
      }
      $wsi++;
    }
    //loop conditions-icon obj assigning 0 and 12 hour values for each day index
    $ci = 0;//set conditions icon index 
    foreach ($parameters->{'conditions-icon'}->{'icon-link'} as $conditions_icon){
      $icon   = (array)$conditions_icon;
      $cipval = str_replace('http://www.nws.noaa.gov/weather/images/fcicons/','',$conditions_icon[0]);
      if($ci%2){  
        $ci2 = ($ci-1)/2;
        $extended[$ci2]['conditions_icon2']       = $cipval;
        $extended[$ci2]['local_conditions_icon2'] = $cipval;
      }else{
        $ci1 = $ci/2;
        $extended[$ci1]['conditions_icon1']       = $cipval;
        $extended[$ci1]['local_conditions_icon1'] = $cipval;
      }
      $ci++;
    }
    //Log to watchdog.
    watchdog('noaa_weather', t('Custom Weather Future Forecast loaded successfully!'), WATCHDOG_NOTICE, NULL);
    //Set the Current Conditions variable
    variable_set('noaa_weather_forecast',$extended);
  }
}

/**
 * Module Callbacks
 */
function _noaa_weather_update_all_weather() {
  //Current Forecast
	_noaa_weather_get_current_conditions(NOAA_WEATHER_CURRENT_WEATHER_STATION_DEFAULT);
  //Future Forecast
  _noaa_weather_get_forecast(NOAA_WEATHER_FORECAST_LAT_DEFAULT,NOAA_WEATHER_FORECAST_LON_DEFAULT,NOAA_WEATHER_FORECAST_DAYS_DEFAULT);
  //Radar
  _noaa_weather_get_radar(NOAA_WEATHER_RADAR_STATION_DEFAULT);
  if ($_GET['destination']) {
    drupal_goto($_GET['destination']);
    $MSG = t('All Weather data updated.');
    drupal_set_message($MSG);
  }
}