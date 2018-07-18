# absolute humidity service
Simple cgi program that computes 
absolute humidity using the standard WMO algorithm.

## usage

```
http://foo.com/abs\_humidity?air\_temp=20
```

returns
```json
 {
   "in" : {
     "air_temp": { "value": 20, "uom": "C"}
   },
   "out" : {
     "absolute_humidity": { "value": 0.017293, "uom": "kg/m^3"},
     "vapor_pressure": { "value": 23.397013, "uom": "hPa"}
   }
 }

```

