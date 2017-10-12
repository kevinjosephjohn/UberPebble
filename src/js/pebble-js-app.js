var data={};

function success(e){
  console.log(
              "[JAVASCRIPT] Successfully delivered message with " + " transactionId=" + e.data.transactionId);
}

function fail(e) {
  console.log(
              "[JAVASCRIPT] Unable to deliver message with transactionId=" + e.data.transactionId + " Error is: " + e.error.message);
}







Pebble.addEventListener("ready", function(e) {
  console.log(
              "[JAVASCRIPT] Ready event fired.");
              
  //Uber API token
  var token='xxxxx';
  url="https://api.uber.com/v1/estimates/time?server_token="+token+"&start_latitude=";

  var lat, lng;
  navigator.geolocation.getCurrentPosition(function(position) {
    lat = position.coords.latitude;
    lng = position.coords.longitude;
    
    url=url+lat+"&start_longitude="+lng;
    console.log(url);
    
    var req = new XMLHttpRequest();
    req.open('GET', url, false);
    req.onload = function(e) {
      if (req.readyState == 4 && req.status == 200) {
        if(req.status == 200) {
          var response = JSON.parse(req.responseText);
          
          console.log("Found "+response.times.length+" cars");
          len=response.times.length;
          var names=[];
          var est=[];
          

          for(var i=0;i<len;i++){
            names[i]=response.times[i].display_name;
            est[i]=response.times[i].estimate;
            est[i]=est[i]/60;
            est[i]=Math.round(est[i]);
            
            est[i]=est[i]+" minutes away";


            key=i.toString();
            data[key]=names[i]+"|"+est[i];


          }

          console.log(JSON.stringify(data));
          Pebble.sendAppMessage(data,success,fail);







        }




        else {
         console.log("Error"); 
       }
     }
   }
   req.send(null);


 });
} );









