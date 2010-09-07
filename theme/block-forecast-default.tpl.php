<?php
/**
 * This is the template file for all weather widget blocks
 * More documentation to come.
 */
?>
<div id="noaa-weather-forecast">
  <div id="forecast-day-0" class="forecast-day">
    <?php print $widget[0]['daystrformated1']; ?>
    <?php if ($forecast_dayspan == '24' ): ?>
    <div class="dayspan-twenty-four"><?php print $widget[0]['period_name2'] .$widget[0]['conditions_icon1_rendered'] .$widget[0]['weather_summary2']; ?></div>;
    <?php else: ?>
    <div class="dayspan-twelve">
      <?php print $widget[0]['conditions_icon1_rendered']; ?>
      <div><?php print $widget[0]['temperature_max']; ?></div>
      <div><?php print $widget[0]['temperature_min']; ?></div>
    </div>
    <?php endif; ?>
  </div>
  <div id="forecast-day-1" class="forecast-day">
    <?php print $widget[1]['daystrformated1']; ?>
    <?php if ($forecast_dayspan == '24' ): ?>
    <div class="dayspan-twenty-four"><?php print $widget[1]['period_name2'] .$widget[1]['conditions_icon1_rendered'] .$widget[1]['weather_summary2']; ?></div>;
    <?php else: ?>
    <div class="dayspan-twelve">
      <?php print $widget[1]['conditions_icon1_rendered']; ?>
      <div><?php print $widget[1]['temperature_max']; ?></div>
      <div><?php print $widget[1]['temperature_min']; ?></div>
    </div>
    <?php endif; ?>
  </div>
  <div id="forecast-day-2" class="forecast-day">
    <?php print $widget[2]['daystrformated1']; ?>
    <?php if ($forecast_dayspan == '24' ): ?>
    <div class="dayspan-twenty-four"><?php print $widget[2]['period_name2'] .$widget[2]['conditions_icon1_rendered'] .$widget[2]['weather_summary2']; ?></div>;
    <?php else: ?>
    <div class="dayspan-twelve">
      <?php print $widget[2]['conditions_icon1_rendered']; ?>
      <div><?php print $widget[2]['temperature_max']; ?></div>
      <div><?php print $widget[2]['temperature_min']; ?></div>
    </div>
    <?php endif; ?>
  </div>
  <div id="forecast-day-3" class="forecast-day">
    <?php print $widget[3]['daystrformated1']; ?>
    <?php if ($forecast_dayspan == '24' ): ?>
    <div class="dayspan-twenty-four"><?php print $widget[3]['period_name2'] .$widget[3]['conditions_icon1_rendered'] .$widget[3]['weather_summary2']; ?></div>;
    <?php else: ?>
    <div class="dayspan-twelve">
      <?php print $widget[3]['conditions_icon1_rendered']; ?>
      <div><?php print $widget[3]['temperature_max']; ?></div>
      <div><?php print $widget[3]['temperature_min']; ?></div>
    </div>
    <?php endif; ?>
  </div>
  <div id="forecast-day-4" class="forecast-day">
    <?php print $widget[4]['daystrformated1']; ?>
    <?php if ($forecast_dayspan == '24' ): ?>
    <div class="dayspan-twenty-four"><?php print $widget[4]['period_name2'] .$widget[4]['conditions_icon1_rendered'] .$widget[4]['weather_summary2']; ?></div>;
    <?php else: ?>
    <div class="dayspan-twelve">
      <?php print $widget[4]['conditions_icon1_rendered']; ?>
      <div><?php print $widget[4]['temperature_max']; ?></div>
      <div><?php print $widget[4]['temperature_min']; ?></div>
    </div>
    <?php endif; ?>
  </div>
</div>