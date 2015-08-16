/*
var locationOptions = {
  enableHighAccuracy: true, 
  maximumAge: 10000, 
  timeout: 10000
};

var m_lat;
var m_lon;
var response; 

function locationCoords(pos) {
  console.log('lat= ' + pos.coords.latitude + ' lon= ' + pos.coords.longitude);
  m_lat = pos.coords.latitude; 
  m_lon= pos.coords.longitude; 
}

function locationError2(err) {
  console.log('location error (' + err.code + '): ' + err.message);
}

Pebble.addEventListener('ready',
  function(e) {
    // Request current position
    console.log("getting current position");
    navigator.geolocation.getCurrentPosition(locationCoords, locationError2, locationOptions);
    
  }
);

//var locationRequest = 'https://maps.googleapis.com/maps/api/geocode/json?latlng' + m_lat +',' + m_lon+ '&key='+'AIzaSyCpN5HZRYzk8yadDpuRNtmba-69Ic9GUTE';

var locationRequest = "https://maps.googleapis.com/maps/api/geocode/json?latlng=40.714224,-73.961452&key=AIzaSyCpN5HZRYzk8yadDpuRNtmba-69Ic9GUTE";
var req = new XMLHttpRequest();
req.open('GET', locationRequest, true);
req.onload = function(e) {
  if (req.readyState == 4 && req.status == 200) {
    if(req.status == 200) {
      response = JSON.parse(req.responseText);
      console.log(response.results[0].formatted_address);
     }
  }
}
req.send(null);

var address = response.results[0].formatted_address;

console.log(address);
*/
