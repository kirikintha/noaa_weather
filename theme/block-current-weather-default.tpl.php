<?php
/**
 * This is the template file for all weather widget blocks
 * More documentation to come.
 */
?>
<div id="noaa-weather-current-conditions">
  <div class="observation-time-rfc"><?php print $widget['observation_time_rfc822']; ?></div>
  <span class="observation-time"><?php print $widget['observation_time']; ?></span>
  <div class="noaa-weather-icon"><?php print $widget['icon_rendered']; ?></div>
  <div class="current-data1">
    <div class="weather">Weather: <?php print $widget['weather']; ?></div>
    <div class="current">Currently: <?php print $widget['temperature_string']; ?></div>
    <div class="humidity">Humidity: <?php print $widget['relative_humidity']; ?></div>
  </div>
  <div class="current-data2">
    <div class="wind">Wind: <?php print $widget['wind_string']; ?></div>
    <div class="barometer">Barometer: <?php print $widget['pressure_in']; ?></div>
    <div class="visibility">Visibility: <?php print $widget['visibility_mi']; ?> miles</div>
  </div>
  </div>
</div>