#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>
static const int RXPin = D7, TXPin = D6;
static const uint32_t GPSBaud = 9600;
TinyGPSPlus gps;
SoftwareSerial ss(RXPin, TXPin);
// Adafruit IO Location Example
//
// Adafruit invests time and resources providing this open source code.
// Please support Adafruit and open source hardware by purchasing
// products from Adafruit!
//
// Written by Todd Treece for Adafruit Industries
// Copyright (c) 2016 Adafruit Industries
// Licensed under the MIT license.
//
// All text above must be included in any redistribution.

/************************** Configuration ***********************************/

// edit the config.h tab and enter your Adafruit IO credentials
// and any additional configuration needed for WiFi, cellular,
// or ethernet clients.
#include "config.h"
#include <math.h
/************************ Example Starts Here *******************************/

// this value integer will hold the current count value for our sketch
int value = 0;

// these double values will hold our fake GPS data
double lat = 28.545684;
double lon = 77.185579;
double ele = 0;

// track time of last published messages and limit feed->save events to once
// every IO_LOOP_DELAY milliseconds
#define IO_LOOP_DELAY 2000
unsigned long lastUpdate;

// set up the 'location' feed
// AdafruitIO_Feed *latitudeFeed = io.feed("location");
AdafruitIO_Feed *latitudeFeed = io.feed("latitude");
// AdafruitIO_Feed *longitudeFeed = io.feed("longitude");
void setup()
{

    // start the serial connection
    Serial.begin(115200);
    ss.begin(GPSBaud);

    // wait for serial monitor to open
    while (!Serial)
        ;

    Serial.print("Connecting to Adafruit IO");

    // connect to io.adafruit.com
    io.connect();

    // set up a message handler for the location feed.
    // location->onMessage(handleMessage);
    latitudeFeed->onMessage(handleMessage);
    // longitudeFeed->onMessage(handleMessage);

    // wait for a connection
    while (io.status() < AIO_CONNECTED)
    {
        Serial.print(".");
        delay(500);
    }

    // we are connected
    Serial.println();
    Serial.println(io.statusText());
    latitudeFeed->get();
}

#include <math.h> // Needed for mathematical functions

struct Coordinate
{
    double lat;
    double lon;
};

// Haversine function to calculate distance between two points (latitude and longitude)
double haversine(Coordinate point1, Coordinate point2)
{
    const float R = 6371.0; // Earth radius in kilometers
    double latDiff = radians(point2.lat - point1.lat);
    double lonDiff = radians(point2.lon - point1.lon);

    double a = sin(latDiff / 2) * sin(latDiff / 2) +
               cos(radians(point1.lat)) * cos(radians(point2.lat)) *
                   sin(lonDiff / 2) * sin(lonDiff / 2);

    double c = 2 * atan2(sqrt(a), sqrt(1 - a));

    return R * c; // Distance in kilometers
}

void findClosestBusStop(Coordinate buspath[], int pathLength, Coordinate point, int &closestIndex)
{
    float minDistance = INFINITY; // Initialize minDistance to infinity

    for (int i = 0; i < pathLength; i++)
    {
        float distance = haversine(point, buspath[i]);

        if (distance < minDistance)
        {
            minDistance = distance;
            closestIndex = i;
        }
    }
    
    return closestIndex
}

void setup()
{
    Serial.begin(9600);

    // Define some example bus stops and a reference point
    Coordinate buspath[] = {
            {'28.54668', '77.18527'}, {'28.5467', '77.18523'}, {'28.5467', '77.18523'}, {'28.54667', '77.18521'}, {'28.54666', '77.18519'}, {'28.54665', '77.18516'}, {'28.54665', '77.18507'}, {'28.54667', '77.18503'}, {'28.54671', '77.185'}, {'28.54676', '77.18499'}, {'28.5468', '77.185'}, {'28.54683', '77.18502'}, {'28.54686', '77.18506'}, {'28.54687', '77.1851'}, {'28.54687', '77.18515'}, {'28.54699', '77.18519'}, {'28.54711', '77.18523'}, {'28.54711', '77.18523'}, {'28.54722', '77.18527'}, {'28.54722', '77.18527'}, {'28.54731667', '77.18530333'}, {'28.54741333', '77.18533667'}, {'28.54751', '77.18537'}, {'28.54760667', '77.18540333'}, {'28.54770333', '77.18543667'}, {'28.5478', '77.18547'}, {'28.5478', '77.18547'}, {'28.54783', '77.18549'}, {'28.547925', '77.185515'}, {'28.54802', '77.18554'}, {'28.54802', '77.18554'}, {'28.548125', '77.185565'}, {'28.54823', '77.18559'}, {'28.54823', '77.18559'}, {'28.54833', '77.185625'}, {'28.54843', '77.18566'}, {'28.54843', '77.18566'}, {'28.54849', '77.18568'}, {'28.54853', '77.18569'}, {'28.54858', '77.18568'}, {'28.54861', '77.18567'}, {'28.54861', '77.18567'}, {'28.54864', '77.18561'}, {'28.54867', '77.18554'}, {'28.54870857', '77.18543857'}, {'28.54874714', '77.18533714'}, {'28.54878571', '77.18523571'}, {'28.54882429', '77.18513429'}, {'28.54886286', '77.18503286'}, {'28.54890143', '77.18493143'}, {'28.54894', '77.18483'}, {'28.54894', '77.18483'}, {'28.548975', '77.18472'}, {'28.54901', '77.18461'}, {'28.54901', '77.18461'}, {'28.54901', '77.18461'}, {'28.54898', '77.18449'}, {'28.54898', '77.18449'}, {'28.54898', '77.18449'}, {'28.54889444', '77.18444333'}, {'28.54880889', '77.18439667'}, {'28.54872333', '77.18435'}, {'28.54863778', '77.18430333'}, {'28.54855222', '77.18425667'}, {'28.54846667', '77.18421'}, {'28.54838111', '77.18416333'}, {'28.54829556', '77.18411667'}, {'28.54821', '77.18407'}, {'28.54821', '77.18407'}, {'28.54806', '77.18399'}, {'28.54806', '77.18399'}, {'28.54797625', '77.183945'}, {'28.5478925', '77.1839'}, {'28.54780875', '77.183855'}, {'28.547725', '77.18381'}, {'28.54764125', '77.183765'}, {'28.5475575', '77.18372'}, {'28.54747375', '77.183675'}, {'28.54739', '77.18363'}, {'28.54734786', '77.18372714'}, {'28.54730571', '77.18382429'}, {'28.54726357', '77.18392143'}, {'28.54722143', '77.18401857'}, {'28.54717929', '77.18411571'}, {'28.54713714', '77.18421286'}, {'28.547095', '77.18431'}, {'28.54705286', '77.18440714'}, {'28.54701071', '77.18450429'}, {'28.54696857', '77.18460143'}, {'28.54692643', '77.18469857'}, {'28.54688429', '77.18479571'}, {'28.54684214', '77.18489286'}, {'28.54668', '77.18527'}, {'28.5467', '77.18523'}, {'28.5467', '77.18523'}, {'28.54667', '77.18521'}, {'28.54666', '77.18519'}, {'28.54665', '77.18516'}, {'28.54665', '77.18507'}, {'28.54667', '77.18503'}, {'28.54671', '77.185'}, {'28.54676', '77.18499'}, {'28.5468', '77.185'}, {'28.54683', '77.18502'}, {'28.54686', '77.18506'}, {'28.54687', '77.1851'}, {'28.54687', '77.18515'}, {'28.54664', '77.18507'}, {'28.54665', '77.18507'}, {'28.54665', '77.18507'}, {'28.54667', '77.18503'}, {'28.54671', '77.185'}, {'28.54676', '77.18499'}, {'28.5468', '77.185'}, {'28.54683', '77.18502'}, {'28.54686', '77.18506'}, {'28.54687', '77.1851'}, {'28.54687', '77.18515'}, {'28.54685', '77.18519'}, {'28.54683', '77.18522'}, {'28.54681', '77.18523'}, {'28.54677', '77.18525'}, {'28.54673', '77.18525'}, {'28.5467', '77.18523'}, {'28.54666125', '77.1853225'}, {'28.5466225', '77.185415'}, {'28.54658375', '77.1855075'}, {'28.546545', '77.1856'}, {'28.54650625', '77.1856925'}, {'28.5464675', '77.185785'}, {'28.54642875', '77.1858775'}, {'28.54639', '77.18597'}, {'28.54635125', '77.1860625'}, {'28.5463125', '77.186155'}, {'28.54627375', '77.1862475'}, {'28.546235', '77.18634'}, {'28.54619625', '77.1864325'}, {'28.5461575', '77.186525'}, {'28.54611875', '77.1866175'}, {'28.54608', '77.18671'}, {'28.54608', '77.18671'}, {'28.54604143', '77.18680429'}, {'28.54600286', '77.18689857'}, {'28.54596429', '77.18699286'}, {'28.54592571', '77.18708714'}, {'28.54588714', '77.18718143'}, {'28.54584857', '77.18727571'}, {'28.54581', '77.18737'}, {'28.54581', '77.18737'}, {'28.54578', '77.18742'}, {'28.54577', '77.18746'}, {'28.5457', '77.18763'}, {'28.5457', '77.18763'}, {'28.54565833', '77.18773833'}, {'28.54561667', '77.18784667'}, {'28.545575', '77.187955'}, {'28.54553333', '77.18806333'}, {'28.54549167', '77.18817167'}, {'28.54545', '77.18828'}, {'28.54545', '77.18828'}, {'28.54541', '77.18837563'}, {'28.54537', '77.18847125'}, {'28.54533', '77.18856688'}, {'28.54529', '77.1886625'}, {'28.54525', '77.18875813'}, {'28.54521', '77.18885375'}, {'28.54517', '77.18894938'}, {'28.54513', '77.189045'}, {'28.54509', '77.18914063'}, {'28.54505', '77.18923625'}, {'28.54501', '77.18933188'}, {'28.54497', '77.1894275'}, {'28.54493', '77.18952313'}, {'28.54489', '77.18961875'}, {'28.54485', '77.18971438'}, {'28.54481', '77.18981'}, {'28.54481', '77.18981'}, {'28.54477', '77.1899'}, {'28.54472', '77.19000333'}, {'28.54467', '77.19010667'}, {'28.54462', '77.19021'}, {'28.54462', '77.19021'}, {'28.54456', '77.19034'}, {'28.54456', '77.19034'}, {'28.54451', '77.19045'}, {'28.54451', '77.19045'}, {'28.54447167', '77.19056'}, {'28.54443333', '77.19067'}, {'28.544395', '77.19078'}, {'28.54435667', '77.19089'}, {'28.54431833', '77.191'}, {'28.54428', '77.19111'}, {'28.54428', '77.19111'}, {'28.54418', '77.19128'}, {'28.54418', '77.19128'}, {'28.54414', '77.19138'}, {'28.54414', '77.19138'}, {'28.54409', '77.19157'}, {'28.54409', '77.19157'}, {'28.54407', '77.19167'}, {'28.54407', '77.19167'}, {'28.54407', '77.19177'}, {'28.54407', '77.19177'}, {'28.5441', '77.19182'}, {'28.54415', '77.19201'}, {'28.54415', '77.19201'}, {'28.54418429', '77.19210714'}, {'28.54421857', '77.19220429'}, {'28.54425286', '77.19230143'}, {'28.54428714', '77.19239857'}, {'28.54432143', '77.19249571'}, {'28.54435571', '77.19259286'}, {'28.54439', '77.19269'}, {'28.54439', '77.19269'}, {'28.54442333', '77.19278833'}, {'28.54445667', '77.19288667'}, {'28.54449', '77.192985'}, {'28.54452333', '77.19308333'}, {'28.54455667', '77.19318167'}, {'28.54459', '77.19328'}, {'28.54459', '77.19328'}, {'28.544622', '77.193377'}, {'28.544654', '77.193474'}, {'28.544686', '77.193571'}, {'28.544718', '77.193668'}, {'28.54475', '77.193765'}, {'28.544782', '77.193862'}, {'28.544814', '77.193959'}, {'28.544846', '77.194056'}, {'28.544878', '77.194153'}, {'28.54491', '77.19425'}, {'28.54491', '77.19425'}, {'28.54492', '77.19431'}, {'28.54492', '77.19431'}, {'28.54496', '77.19434'}, {'28.54498', '77.19438'}, {'28.545', '77.19443'}, {'28.54499', '77.19449'}, {'28.54499', '77.19449'}, {'28.54502714', '77.19458571'}, {'28.54506429', '77.19468143'}, {'28.54510143', '77.19477714'}, {'28.54513857', '77.19487286'}, {'28.54517571', '77.19496857'}, {'28.54521286', '77.19506429'}, {'28.54525', '77.19516'}, {'28.54525', '77.19516'}, {'28.54528222', '77.19526333'}, {'28.54531444', '77.19536667'}, {'28.54534667', '77.19547'}, {'28.54537889', '77.19557333'}, {'28.54541111', '77.19567667'}, {'28.54544333', '77.19578'}, {'28.54547556', '77.19588333'}, {'28.54550778', '77.19598667'}, {'28.54554', '77.19609'}, {'28.54554', '77.19609'}, {'28.54556', '77.19615'}, {'28.54557', '77.19621'}, {'28.54558', '77.19623'}, {'28.54558', '77.19631'}, {'28.54563', '77.19643'}, {'28.54563', '77.19643'}, {'28.54566', '77.19646'}, {'28.54569', '77.19649'}, {'28.54569', '77.19649'}, {'28.54479', '77.19458'}, {'28.5447694', '77.1945653'}, {'28.54475', '77.19454'}, {'28.5447223', '77.1945067'}, {'28.5447106', '77.1944627'}, {'28.5447112', '77.1944127'}, {'28.5447194', '77.1943633'}, {'28.5447535', '77.1943139'}, {'28.5448035', '77.1942852'}, {'28.5448571', '77.1942752'}, {'28.54489', '77.19429'}, {'28.5446137', '77.1946158'}, {'28.5446672', '77.1944761'}, {'28.544639', '77.1945543'}, {'28.5445912', '77.1946768'}, {'28.5449835', '77.1944893'}, {'28.5449688', '77.1945293'}, {'28.54495', '77.19456'}, {'28.54492', '77.19458'}, {'28.5448865', '77.1945926'}, {'28.54485', '77.1946'}, {'28.54481', '77.19459'}, {'28.54481', '77.19459'}, {'28.544705', '77.19468'}, {'28.5445788', '77.194762'}, {'28.5446459', '77.1947137'}, {'28.5445382', '77.1948413'}, {'28.5444931', '77.1949574'}, {'28.5444463', '77.1950835'}, {'28.5444207', '77.1951513'}, {'28.5443892', '77.1952312'}, {'28.5443665', '77.1952923'}, {'28.5443488', '77.1953379'}, {'28.5443212', '77.1954085'}, {'28.5442876', '77.1954726'}, {'28.5442425', '77.1955639'}, {'28.5442045', '77.1956659'}, {'28.5441571', '77.1957572'}, {'28.5441156', '77.1958619'}, {'28.5440835', '77.1959478'}, {'28.544054', '77.1960269'}, {'28.5440205', '77.1961146'}, {'28.5439941', '77.1961862'}, {'28.5439693', '77.196252'}, {'28.5439152', '77.1964112'}, {'28.5439376', '77.1963375'}, {'28.5438824', '77.1965052'}, {'28.5438461', '77.1965952'}, {'28.5438097', '77.1966945'}, {'28.5437757', '77.1967804'}, {'28.5436934', '77.1968784'}, {'28.5437393', '77.1968959'}, {'28.5437547', '77.1968409'}, {'28.5437541', '77.1968583'}, {'28.543737', '77.1969039'}, {'28.5437299', '77.1969535'}, {'28.5437153', '77.1969956'}, {'28.5436988', '77.1970412'}, {'28.54365', '77.19701714'}, {'28.54356', '77.19697429'}, {'28.54347', '77.19693143'}, {'28.54338', '77.19688857'}, {'28.54329', '77.19684571'}, {'28.5432', '77.19680286'}, {'28.54311', '77.19676'}, {'28.54311', '77.19676'}, {'28.54302', '77.19672'}, {'28.54293', '77.19668'}, {'28.54284', '77.19664'}, {'28.54275', '77.1966'}, {'28.54266', '77.19656'}, {'28.54266', '77.19656'}, {'28.54257667', '77.19651333'}, {'28.54249333', '77.19646667'}, {'28.5423876', '77.1964213'}, {'28.5424406', '77.1964441'}, {'28.5423287', '77.1963954'}, {'28.5423022', '77.1963177'}, {'28.5422799', '77.1962588'}, {'28.5422578', '77.1961991'}, {'28.5422321', '77.1961394'}, {'28.5421876', '77.1960194'}, {'28.5422218', '77.1959979'}, {'28.5422088', '77.1960797'}, {'28.54228', '77.19598'}, {'28.54232', '77.19598'}, {'28.5423671', '77.1959793'}, {'28.5424118', '77.195992'}, {'28.542433', '77.196'}, {'28.5425113', '77.1960354'}, {'28.5425838', '77.19607'}, {'28.5426751', '77.1961086'}, {'28.54277', '77.19617'}, {'28.54277', '77.19617'}, {'28.5428625', '77.1962175'}, {'28.542955', '77.196265'}, {'28.5430169', '77.1963018'}, {'28.5431282', '77.196366'}, {'28.5430737', '77.1963335'}, {'28.5431885', '77.196411'}, {'28.5431602', '77.1963889'}, {'28.5431947', '77.1964337'}, {'28.5431952', '77.1964712'}, {'28.5431958', '77.1964535'}, {'28.5431879', '77.1964993'}, {'28.5431773', '77.1965371'}, {'28.5431762', '77.1966335'}, {'28.5431526', '77.1966194'}, {'28.5431641', '77.1965785'}, {'28.5432486', '77.1966716'}, {'28.543332', '77.1967199'}, {'28.543423', '77.1967701'}, {'28.5435111', '77.1968083'}, {'28.5435968', '77.1968431'}, {'28.5436578', '77.1968645'}, {'28.5437011', '77.1968812'}, {'28.54297', '77.19903'}, {'28.54298', '77.19904'}, {'28.54298', '77.19904'}, {'28.54301667', '77.19894083'}, {'28.54305333', '77.19884167'}, {'28.54309', '77.1987425'}, {'28.54312667', '77.19864333'}, {'28.54316333', '77.19854417'}, {'28.5432', '77.198445'}, {'28.54323667', '77.19834583'}, {'28.54327333', '77.19824667'}, {'28.54331', '77.1981475'}, {'28.54334667', '77.19804833'}, {'28.54338333', '77.19794917'}, {'28.54342', '77.19785'}, {'28.54342', '77.19785'}, {'28.54346', '77.19775125'}, {'28.5435', '77.1976525'}, {'28.54354', '77.19755375'}, {'28.54358', '77.197455'}, {'28.54362', '77.19735625'}, {'28.54366', '77.1972575'}, {'28.5437', '77.19715875'}, {'28.54374', '77.19706'}, {'28.54374', '77.19706'}, {'28.54374', '77.19706'}, {'28.54374', '77.19706'}, {'28.54374', '77.19706'}, {'28.54374', '77.19706'}
    }
    Coordinate point = { 28.5455,
                         77.2735 } // Example point

    int pathLength = sizeof(buspath) / sizeof(buspath[0]);
    int closestIndex = -1;

    // Find the closest bus stop
    findClosestBusStop(buspath, pathLength, point, closestIndex);

    Serial.print("Closest bus stop index: ");
    Serial.println(closestIndex);
}

void loop()
{
    // Serial.print(gps.location.lat(), 6);
    while (ss.available() > 0)
    {
        gps.encode(ss.read());
    }
    if (gps.location.isUpdated())
    {
        double lat1[100];

        // Serial.print("LAT="); Serial.print(gps.location.lat(), 6);
        // Serial.print("LNG="); Serial.println(gps.location.lng(), 6);
    }
    // process messages and keep connection alive
    io.run();

    if (millis() > (lastUpdate + IO_LOOP_DELAY))
    {
        // Serial.println("----- sending -----");
        // Serial.print("value: ");
        Serial.print(value);
        Serial.print(",");
        Serial.print(lat, 6);
        Serial.print(",");
        Serial.println(lon, 6);
        // Serial.print("ele: ");
        // Serial.println(ele, 2);

        // save value and location data to the
        // 'location' feed on Adafruit IO
        let ind=findClosestBusStop(buspath, pathLength, point, closestIndex);
        let nlat=buspath[ind].lat
        let nlon=buspath[ind].lon
        latitudeFeed->save(value, nlat, nlon, ele);

        // shift all values (for demo purposes)
        value += 1;
        lat = gps.location.lat();
        lon = gps.location.lng();
        ele += 1;

        // wait one second (1000 milliseconds == 1 second)
        lastUpdate = millis();
    }
}

void handleMessage(AdafruitIO_Data *data)
{

    // since we sent an int, we can use toInt()
    // to get the int value from the received IO
    // data.
    int received_value = data->toInt();

    // the lat() lon() and ele() methods
    // will allow you to get the double values
    // for the location data sent by IO
    double received_lat = data->lat();
    double received_lon = data->lon();
    double received_ele = data->ele();

    // print out the received values
    // Serial.println("----- received -----");
    // Serial.print("value: ");
    // Serial.println(received_value);
    // Serial.print("lat: ");
    // Serial.println(received_lat, 6);
    // Serial.print("lon: ");
    // Serial.println(received_lon, 6);
    // Serial.print("ele: ");
    // Serial.println(received_ele, 2);
}
