// Put all onload AJAX calls here, and event listeners
$(document).ready(function() {
    // On page-load AJAX Example
    $.ajax({
        type: 'get',            //Request type
        dataType: 'json',       //Data type - we will use JSON for almost everything 
        url: '/someendpoint',   //The server endpoint we are connecting to
        success: function (data) {
            /*  Do something with returned object
                Note that what we get is an object, not a string, 
                so we do not need to parse it on the server.
                JavaScript really does handle JSONs seamlessly
            */
            $('#blah').html("On page load, Received string '"+JSON.stringify(data)+"' from server");
            //We write the object to the console to show that the request was successful
            console.log(data); 

        },
        fail: function(error) {
            // Non-200 return, do something with error
            console.log(error); 
        }
    });

    $.ajax({
        type:'get',
        dataType:'json',
        url:'/files',
        success: function(data){
            console.log("firstData:"+data);
            if(data.length != 0){
                for (index = 0; index < data.length; index++) { 
                    let name = data[index];
                    nameSender(name);
                } 
            }else{
                $('#calendar_list > tbody').append("<tr><td>No Files Found</td></tr>");
            }
            
            


        },
        fail: function(error) {
            // Non-200 return, do something with error
            console.log(error); 
        }
    });

    function nameSender(name){
        
        $.ajax({
            type:'get',
            dataType:'json',
            data: {filename: name},
            url:'/getcals',
            success: function(data2){
                //console.log(name+3);
                console.log(data2.calJSON)
                console.log(data2.calJSON.filename)
                if(data2.calJSON.version == undefined){
                    $('#calendar_list > tbody').append("<tr><td><a href=\"/uploads/"+data2.calJSON.filename+"\">"+data2.calJSON.filename+"</a></td><td></td><td>Invalid File</td><td></td><td></td></tr>");
                }else{
                    $('#calendar_list > tbody').append("<tr><td><a href=\"/uploads/"+data2.calJSON.filename+"\">"+data2.calJSON.filename+"</a></td><td>"+data2.calJSON.version+"</td><td>"+data2.calJSON.prodID+"</td><td>"+data2.calJSON.numEvents+"</td><td>"+data2.calJSON.numProps+"</td></tr>");
                    $('#calendar_select').append("<option value="+data2.calJSON.filename+">"+data2.calJSON.filename+"</option>");
                
                }//console.log(name+3.5 );
            },
            fail: function(error) {
                // Non-200 return, do something with error
                console.log(error); 
            }
        });
    }

    

    // Event listener form replacement example, building a Single-Page-App, no redirects if possible
    $('#someform').submit(function(e){
        $('#blah').html("Callback from the form");
        e.preventDefault();
        //Pass data to the Ajax call, so it gets passed to the 
        $.ajax({});
    });

    $('#statusBox').submit(function(e){
        $('#clear').value="Callback from the form";
        document.getElementById("status").value = "";
        e.preventDefault();
        //Pass data to the Ajax call, so it gets passed to the 
        $.ajax({});
    });


});

function selectedFile(){
    let dropdown = document.getElementById('calendar_select');
    let selected = dropdown.options[dropdown.selectedIndex].text;
    console.log("On change: "+selected);
    $.ajax({
            type:'get',
            dataType:'json',
            data: {filename: selected},
            url:'/inspectCal',
            success: function(data){
                console.log("file success!");
                //console.log(name+3);
                $('#calendar_contents > tbody').remove();

                for (index = 0; index < data.length; index++) { 
                    let cal = data[index];
                    cal.startDT.date = cal.startDT.date.slice(0, 4) + "/" + cal.startDT.date.slice(4, 6)+"/" + cal.startDT.date.slice(6);
                    cal.startDT.time = cal.startDT.time.slice(0, 2) + ":" + cal.startDT.time.slice(2, 4)+":" + cal.startDT.time.slice(4);
                    if(cal.startDT.isUTC == true){
                        cal.startDT.time+=" (UTC)";
                    }
                    $('#calendar_contents').append("<tbody><tr><td>"+index+"</td><td>"+cal.startDT.date+"</td><td>"+cal.startDT.time+"</td><td>"+cal.summary+"</td><td>"+cal.numProps+"</td><td>"+cal.numAlarms+"</td></tr></tbody>");

                } 


            },
            fail: function(error) {
                // Non-200 return, do something with error
                console.log(error); 
            }
        });
}

function validator(){
    let filename = document.getElementById("file_selector").value;
    let suffix = filename.substring(filename.length-4);
    if(suffix != ".ics"){
        document.getElementById("upload_cal").disabled = true;
        alert("Invalid File");
    }else{
        document.getElementById("upload_cal").disabled = false;
    }
}