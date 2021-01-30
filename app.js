'use strict'

// C library API
const ffi = require('ffi-napi');

// Express App (Routes)
const express = require("express");
const app     = express();
const path    = require("path");
const fileUpload = require('express-fileupload');

app.use(fileUpload());
app.use(express.static(path.join(__dirname+'/uploads')));

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
  if(!req.files) {
    return res.status(400).send('No files were uploaded.');
  }
 
  let uploadFile = req.files.uploadFile;
 
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
    if(err == null) {
      res.sendFile(path.join(__dirname+'/uploads/' + req.params.name));
    } else {
      console.log('Error in file downloading route: '+err);
      res.send('');
    }
  });
});

//******************** Your code goes here ******************** 


let sharedLib = ffi.Library('./libsvgparse', {
  'validateSVG': ['bool', ['string'] ],
  'getImgJson': [ 'string', [ 'string' ] ],
  'dataMaker': [ 'string', [ 'string' ] ],
  'changeTit': [ 'void', [ 'string' , 'string', 'string' ] ],
  'editAdd': ['void', ['string', 'string', 'string', 'int', 'string']],
  'createEmpty': ['void', ['string']],
  'createShape': ['void', ['string', 'float', 'float', 'float', 'float', 'float', 'string', 'string']],
  'scaler': ['void', ['int', 'string', 'string']]
});

function getFiles(){

  let files = fs.readdirSync('./uploads');

  let svgStr = "";
  let svgJson; 
  let array = [];

  for(var i = 0; i < files.length; ++i){

    var valid = sharedLib.validateSVG("./uploads/" + files[i]);

    if(valid == true){
      svgStr = sharedLib.getImgJson("./uploads/" + files[i]);

      if(svgStr != "{}"){
        svgJson = JSON.parse(svgStr);
        array.push(svgJson);
      }
    }
  }
  return array;
}

function getData(){

  let files = fs.readdirSync('./uploads');
  let svgStr;
  let svgData = "";
  let svgJsonData;
  let dataArray = [];

  for(var i = 0; i < files.length; ++i){

    var valid = sharedLib.validateSVG("./uploads/" + files[i]);

    if(valid == true ){
      svgStr = sharedLib.getImgJson("./uploads/" + files[i]);
      svgData = sharedLib.dataMaker("./uploads/" + files[i]);
        
      if(svgStr != "{}"){
        svgJsonData = JSON.parse(svgData);
        dataArray.push(svgJsonData);
      }
    }
  }
  return dataArray;
}

function newTitle(array){
  
  sharedLib.changeTit(array[0], array[1], array[2]);

}

function editAddOther(array){
  
  sharedLib.editAdd(array[0], array[1], array[2], array[3], array[4]);
  console.log(array);
}

function newSVG(string){
  sharedLib.createEmpty(string);
}

function newShape(array){
  sharedLib.createShape(array[0], array[1], array[2], array[3], array[4], array[5], array[6], array[7]);
}

function scaleShapes(array){
  sharedLib.scaler(array[0], array[1], array[2]);
}

// console.log(getFiles());

app.get('/files', function(req , res){
  // console.log(req.query.test);

  res.send({
    obj: getFiles(),
    components: getData()
  });

});

app.get('/test', function(req , res){
  newTitle(req.query.test);

  res.send({
    obj: getFiles(),
    components: getData()
  });

});

app.get('/otherAttributes', function(req , res){
  // newTitle(req.query.test);
  editAddOther(req.query.test);

  res.send({
    obj: getFiles(),
    components: getData()
  });

});

app.get('/create', function(req , res){
  // newTitle(req.query.test);
  // editAddOther(req.query.test);
  newSVG(req.query.test);

  res.send({
    obj: getFiles(),
    components: getData()
  });

});

app.get('/shape', function(req , res){
  newShape(req.query.test);
  console.log(req.query.test);

  res.send({
    obj: getFiles(),
    components: getData()
  });

});

app.get('/scaler', function(req , res){
  console.log(req.query.test);
  scaleShapes(req.query.test);

  res.send({
    obj: getFiles(),
    components: getData()
  });

});

app.listen(portNum);
console.log('Running app at localhost: ' + portNum);
