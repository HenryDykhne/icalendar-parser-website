'use strict'

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