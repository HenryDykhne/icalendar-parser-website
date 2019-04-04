'use strict'
//sql
const mysql = require('mysql');

// C library API
const ffi = require('ffi');
//TODO:check if you must relocate your shared library
let sharedLib = ffi.Library('./libcombo', {
  'createCalJSONWrap': [ 'string', [ 'string' ] ], 
  'createEvtListJSONWrap': [ 'string', [ 'string' ] ], 
  'addEventWrap': [ 'string', [ 'string', 'string' ] ], 
  'addCalWrap': [ 'string', [ 'string', 'string' ] ],
  'readableAlmListJSONWrap': [ 'string', [ 'string', 'int' ] ],
  'readablePropListJSONWrap': [ 'string', [ 'string', 'int' ] ],
  'createAlmListJSONWrap': [ 'string', [ 'string', 'int' ] ],
  
});
// Express App (Routes)
const express = require("express"); 
const app     = express();
const path    = require("path");
const fileUpload = require('express-fileupload');

app.use(fileUpload());

// Minimization
const fs = require('fs');
const JavaScriptObfuscator = require('javascript-obfuscator');

// Important, pass in port as in `npm run dev 1234`, do not change
const portNum = process.argv[2];

// Send HTML at root, do not change
app.get('/',function(req,res){
  res.sendFile(path.join(__dirname+'/public/index.html'));
});

// Send Style, do not change
app.get('/style.css',function(req,res){
  //Feel free to change the contents of style.css to prettify your Web app
  res.sendFile(path.join(__dirname+'/public/style.css'));
});

// Send obfuscated JS, do not change
app.get('/index.js',function(req,res){
  fs.readFile(path.join(__dirname+'/public/index.js'), 'utf8', function(err, contents) {
    const minimizedContents = JavaScriptObfuscator.obfuscate(contents, {compact: true, controlFlowFlattening: true});
    res.contentType('application/javascript');
    res.send(minimizedContents._obfuscatedCode);
  });
});

//Respond to POST requests that upload files to uploads/ directory
app.post('/upload', function(req, res) {
  console.log(req.query.upload);
  if(!req.files) {
    return res.status(400).send('No files were uploaded.');
  }
  
  let uploadFile = req.files.uploadFile;
  console.log("uploading "+JSON.stringify(uploadFile));
  // Use the mv() method to place the file somewhere on your server
  uploadFile.mv('uploads/' + uploadFile.name, function(err) {
  if(err) {
    return res.status(500).send(err);
  }

    res.redirect('/');
  });
});

//Respond to GET requests for files in the uploads/ directory
app.get('/uploads/:name', function(req , res){
  fs.stat('uploads/' + req.params.name, function(err, stat) {
    console.log(req.params.name)
    console.log(err);
    if(err == null) {
      res.sendFile(path.join(__dirname+'/uploads/' + req.params.name));
    } else {
      res.send('');
    }
  });
});

//******************** Your code goes here ******************** 
/**** Set up functions from our shared library ****

We create a new object called sharedLib and the C functions become its methods

*/

//*************************************************************
const folder = './uploads';
app.get('/files', function(req,res) {
  let filenames;
  filenames = fs.readdirSync(folder);
  res.send(filenames);
});


//Sample endpoint
app.get('/someendpoint', function(req , res){
  res.send({
    foo: "bar"
  });
});

app.get('/getcals', function(req,res){
  console.log(req.query.filename);
  let path = "./uploads/"+ req.query.filename;
  let calJSONstr = sharedLib.createCalJSONWrap(path);
  console.log(calJSONstr);
  let calJSON = JSON.parse(calJSONstr);
  calJSON.filename = req.query.filename;
  console.log(calJSON);
  res.send({
    calJSON
  });
});

app.get('/addEvent', function(req,res){
  console.log("filename: "+req.query.filename);
  let path = "./uploads/"+ req.query.filename;
  console.log(JSON.stringify(req.query.event));
  let calJSON = sharedLib.addEventWrap(path, req.query.event);
  calJSON = JSON.parse(calJSON);
  res.send({
    calJSON
  });
});

app.get('/addCal', function(req,res){
  console.log("Got Here");
  console.log("filename: "+req.query.filename);
  let path = "./uploads/"+ req.query.filename;
  console.log(JSON.stringify(req.query.calendar));
  let calJSON = sharedLib.addCalWrap(path, req.query.calendar);
  console.log(JSON.stringify(calJSON));
  calJSON = JSON.parse(calJSON);
  res.send({
    calJSON
  });
});

app.get('/inspectCal', function(req,res){
  console.log("filename: "+req.query.filename);
  let path = "./uploads/"+ req.query.filename;

  let calJSON = sharedLib.createEvtListJSONWrap(path);
  //let calJSON = JSON.parse(calJSONstr);
  //calJSON.filename = req.query.filename;
  console.log("Inspect cal: "+calJSON);
  res.send(calJSON);
});

app.listen(portNum);
console.log('Running app at localhost: ' + portNum);

app.get('/almList', function(req,res){
  let path = "./uploads/"+ req.query.filename;
  let almListJSON = sharedLib.readableAlmListJSONWrap(path, req.query.evtNum);
  console.log(almListJSON);
  res.send({
    almListJSON
  });
});

app.get('/propList', function(req,res){
  let path = "./uploads/"+ req.query.filename;
  let propListJSON = sharedLib.readablePropListJSONWrap(path, req.query.evtNum);
  console.log(propListJSON);
  res.send({
    propListJSON
  });
});

let connection;
app.get('/login', function(req , res){
  connection = mysql.createConnection({
    host     : 'dursley.socs.uoguelph.ca',
    user     : req.query.user,
    password : req.query.password,
    database : req.query.db
  });

  connection.connect(function (err){
    if(err){
      console.log(err);
      res.send({connected: "Unsucessfull connection.\n"});
    }else{
      connection.query("CREATE TABLE IF NOT EXISTS FILE ( cal_id INT AUTO_INCREMENT PRIMARY KEY, \
      file_Name VARCHAR(60) NOT NULL, \
      version INT NOT NULL, \
      prod_id VARCHAR(256) NOT NULL )", function (err, rows, fields) {
        if (err) console.log("Something went wrong. "+err);
      });

      connection.query("CREATE TABLE IF NOT EXISTS EVENT ( event_id INT AUTO_INCREMENT PRIMARY KEY, \
      summary VARCHAR(1024), \
      start_time DATETIME NOT NULL, \
      location VARCHAR(60), \
      organizer VARCHAR(256), \
      cal_file INT NOT NULL, \
      FOREIGN KEY(cal_file) REFERENCES FILE(cal_id) ON DELETE CASCADE )", function (err, rows, fields) {
        if (err) console.log("Something went wrong. "+err);
      });

      connection.query("CREATE TABLE IF NOT EXISTS ALARM ( alarm_id INT AUTO_INCREMENT PRIMARY KEY, \
      action VARCHAR(256) NOT NULL, \
      `trigger` VARCHAR(256) NOT NULL, \
      event INT NOT NULL, \
      FOREIGN KEY(event) REFERENCES EVENT(event_id) ON DELETE CASCADE )", function (err, rows, fields) {
        if (err) console.log("Something went wrong. "+err);
      });
      res.send({connected: "Sucessfull connection.\n"});
    }
  });
});


app.get('/storeFiles', function(req,res){
  let filenames;
  filenames = fs.readdirSync(folder);
  //connection.query("DELETE FROM FILE");
  let i = 0;
  for(i = 0; i<filenames.length; i++){
    let path = "./uploads/"+filenames[i];
    let calJSONstr = sharedLib.createCalJSONWrap(path);
    let calJSON = JSON.parse(calJSONstr);
    calJSON.filename = filenames[i];
    let temp = filenames[i];
    connection.query("SELECT EXISTS(SELECT 1 FROM FILE WHERE file_Name='"+temp+"') AS fileExists;", function(err3, result, fields3){
      if(err3){
            console.log(err3);
            res.send({
              error: "failed connection\n"
            });
      }
      if(calJSON.version != undefined && result[0].fileExists === 0){
        connection.query("INSERT INTO FILE (file_Name, version, prod_id) VALUES ('"+calJSON.filename+"','"+calJSON.version+"','"+calJSON.prodID+"');");
        connection.query("SELECT * FROM FILE WHERE \""+temp+"\" = file_Name;", function(err, rows, fields){
          if(err){
            console.log(err);
            res.send({
              error: "failed connection\n"
            });
          }

          console.log("fields = "+ fields);
          if(rows === null || rows === undefined){
            res.send({
              error: "could not load file\n"
            });
          }
          console.log("rows = "+ rows[0].cal_id);
          let evtList = JSON.parse(sharedLib.createEvtListJSONWrap(path));
          console.log("im right here\n");
          let j = 0;
          for(j = 0; j<evtList.length; j++){
            let evtJSON = evtList[j];
            console.log(evtJSON);
            let start = evtJSON.startDT.date.slice(0,4)+"/"+evtJSON.startDT.date.slice(4,6)+"/"+evtJSON.startDT.date.slice(6,8)+" "+evtJSON.startDT.time.slice(0,2)+":"+evtJSON.startDT.time.slice(2,4)+":"+evtJSON.startDT.time.slice(4,6);
            console.log(start);
            connection.query("INSERT INTO EVENT (summary, start_time, location, organizer, cal_file) VALUES ('"+evtJSON.summary+"','"+start+"','"+evtJSON.location+"', '"+evtJSON.organizer+"', '"+ rows[0].cal_id +"');");
          
            let almList = JSON.parse(sharedLib.createAlmListJSONWrap(path, j));
            console.log(almList);
            let k = 0;
            for(k = 0; k<almList.length; k++){
              let almJSON = almList[k];
              console.log(almJSON);
              connection.query("SELECT * FROM EVENT ORDER BY event_id DESC LIMIT 1;", function(err2, rows2, fields2){
                if(err2){
                  console.log(err2);
                  res.send({
                    error: "could not load event\n"
                  });
                }else{
                  connection.query("INSERT INTO ALARM (action, `trigger`, event) VALUES ('"+almJSON.action+"','"+almJSON.trigger+"','"+rows2[0].event_id+"');");
                }
              });
            }
          }
        });
      }
    });
  }
  res.send({
    temp: "done"
  });
});

app.get('/clearTables', function(req,res){
 //connection.query("DELETE FROM ALARM");
 //connection.query("DELETE FROM EVENT");
 connection.query("DELETE FROM FILE");
  res.send({
    temp: "temp"
  });
});

app.get('/getFilesInDB', function(req,res){
  connection.query("SELECT file_Name FROM FILE;", function(err, result, fields){
    if(err){
      res.send({
        error: err + "\n"
      });
    }
    let namesJSON = [];
    for(let i = 0; i<result.length; i++){
      namesJSON.push(result[i].file_Name);
    }
    res.send(namesJSON);
  });
});

app.get('/DBStatus', function(req,res){
  connection.query("SELECT COUNT(*) AS numFiles FROM FILE;", function(err, result, fields){
    if(err){
      res.send({
        error: err + "\n"
      });
    }
    const files = result[0].numFiles;
    connection.query("SELECT COUNT(*) AS numEvents FROM EVENT;", function(err, result, fields){
      if(err){
        res.send({
          error: err + "\n"
        });
      }
      const events = result[0].numEvents;
      connection.query("SELECT COUNT(*) AS numAlarms FROM ALARM;", function(err, result, fields){
        if(err){
          res.send({
            error: err + "\n"
          });
        }
        const alarms = result[0].numAlarms;
        res.send({
          numFiles: files,
          numEvents: events,
          numAlarms: alarms
        });
      });
    });
  });
});

app.get('/events_by_start_date', function(req,res){
  connection.query("SELECT * FROM EVENT ORDER BY start_time;", function(err, result, fields){
    if(err){
      console.log(err);
      res.send({
        error: err + "\n"
      });
    }
    res.send({events:result});
  });
});

app.get('/events_from_specific_file', function(req,res){
  connection.query("SELECT * FROM FILE, EVENT WHERE ( FILE.cal_id=EVENT.cal_file AND file_Name='"+req.query.file+"' ) ORDER BY start_time;", function(err, result, fields){
    if(err){
      console.log(err);
      res.send({
        error: err + "\n"
      });
    }
    res.send({events:result});
  });
});

app.get('/conflicting_events', function(req,res){
  connection.query("SELECT * FROM EVENT WHERE start_time IN (SELECT start_time FROM EVENT GROUP BY start_time HAVING COUNT(*) > 1) ORDER BY start_time;", function(err, result, fields){
    if(err){
      console.log(err);
      res.send({
        error: err + "\n"
      });
    }
    res.send({events:result});
  });
});

app.get('/alarms_from_specific_file', function(req,res){
  connection.query("SELECT * FROM FILE, EVENT, ALARM WHERE ( FILE.cal_id=EVENT.cal_file AND EVENT.event_id=ALARM.event AND file_Name='"+req.query.file+"' ) ORDER BY start_time;", function(err, result, fields){
    if(err){
      console.log(err);
      res.send({
        error: err + "\n"
      });
    }
    res.send({alarms:result});
  });
});

app.get('/events_with_alarms', function(req,res){
  connection.query(" SELECT * FROM EVENT, ALARM WHERE ( EVENT.event_id=ALARM.event) GROUP BY event_id ORDER BY event_id;", function(err, result, fields){
    if(err){
      console.log(err);
      res.send({
        error: err + "\n"
      });
    }
    res.send({events:result});
  });
});

app.get('/files_with_events_in_guelph', function(req,res){
  connection.query("SELECT * FROM FILE, EVENT WHERE (EVENT.location='GUELPH' AND FILE.cal_id=EVENT.cal_file) GROUP BY cal_id ORDER BY cal_id", function(err, result, fields){
    if(err){
      console.log(err);
      res.send({
        error: err + "\n"
      });
    }
    res.send({files:result});
  });
});