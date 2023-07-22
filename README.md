# This project is not associated with Digital Extremes or Warframe Market (warframe.market).

This project is just an API intended to interact with the respective service's APIs.

# libwarframe

More information to come once I flesh the project out more :)


## Thoughts and Ideas

 - [ ] Cache mechanism to prevent strain on APIs
 - [ ] Ability to log in to WFmarket
 - [ ] Figure out whether to return cJSON object or "custom" array
    - cJSON: Would be easy, as response is in JSON format already. Harder to interact with...
    - "Custom" Array: Would probably be simpler to interact with, more processing required to convert from cJSON to array
 - [ ] Implement more endpoints
   - [ ] https://content.warframe.com/dynamic/worldState.php
   - [ ] https://n8k6e2y6.ssl.hwcdn.net/repos/hnfvc0o3jnfvc873njb03enrf56.html
     - https://lexbor.com/docs/lexbor/html/
   - [ ] https://warframe.fandom.com/wiki/Public_Export
     - https://github.com/lloyd/easylzma
 - [ ] Debug-only print statements
 - [ ] Static Libraries
